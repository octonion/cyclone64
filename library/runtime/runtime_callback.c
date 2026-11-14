/* Runtime ABI bridge for native Cyclone function values.
 * Signatures: F<return>(<args>), scalar codes, p<len>:<C pointer type>,
 * and S<len>:<C struct name>(<fields>). Metadata and closures are cached for
 * process lifetime. Explicit shutdown is only safe after all callbacks cease. */
#ifdef CYC_LIBFFI
#include <ffi.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <stdarg.h>

// libffi closures use mmap, outside LeakSanitizer's allocation graph. Each
// cached closure is a live root and owns trampoline-table pointers in malloc
// storage. Register precisely that live mapped record, and unregister it when
// freed; this preserves leak detection for genuinely unreachable allocations.
extern void __lsan_register_root_region(const void *,size_t) __attribute__((weak));
extern void __lsan_unregister_root_region(const void *,size_t) __attribute__((weak));
static void cb_closure_free(ffi_closure *closure) {
  if(!closure) return;
  if(__lsan_unregister_root_region) __lsan_unregister_root_region(closure,sizeof *closure);
  ffi_closure_free(closure);
}

extern void *GC_malloc(size_t);
extern void GC_free(void *);
extern void *_throw_badalloc_fn(const char *, unsigned);

struct cb_type {
  char kind;
  ffi_type *ffi;
  ffi_type aggregate;
  struct cb_type *result;
  struct cb_type **parts;
  size_t count;
  ffi_type **ffi_parts;
  ffi_cif cif;
  char *signature;
};
struct cb_adapter {
  struct cb_adapter *next;
  void *original, *code;
  ffi_closure *closure;
  struct cb_type *from, *to;
  size_t bound_count;
  unsigned long *bound;
};
static struct cb_adapter *cb_cache;
static pthread_mutex_t cb_lock = PTHREAD_MUTEX_INITIALIZER;
static void *cb_adapt(void *, const char *, const char *);

static _Noreturn void cb_error(const char *message) {
  fprintf(stderr,"Cyclone callback ABI: %s\n",message);
  exit(1);
}
static void *cb_alloc(size_t n) {
  void *p=GC_malloc(n);
  if (!p) { _throw_badalloc_fn(__FILE__,__LINE__); cb_error("allocation handler returned"); }
  memset(p,0,n);
  return p;
}
static char *cb_copy(const char *p, size_t n) {
  char *r=cb_alloc(n+1); memcpy(r,p,n); return r;
}
static void cb_free_type(struct cb_type *t) {
  if (!t) return;
  for (size_t i=0;i<t->count;++i) cb_free_type(t->parts[i]);
  cb_free_type(t->result); GC_free(t->parts); GC_free(t->ffi_parts);
  GC_free(t->signature); GC_free(t);
}
static struct cb_type *cb_parse(const char **text) {
  const char *start=*text;
  struct cb_type *t=cb_alloc(sizeof *t);
  t->kind=*(*text)++;
  switch (t->kind) {
  case 'v': t->ffi=&ffi_type_void; break;
  case 'b': t->ffi=&ffi_type_sint8; break;
  case 'B': t->ffi=&ffi_type_uint8; break;
  case 'h': t->ffi=&ffi_type_sint16; break;
  case 'H': t->ffi=&ffi_type_uint16; break;
  case 'i': t->ffi=&ffi_type_sint32; break;
  case 'I': t->ffi=&ffi_type_uint32; break;
  case 'l': t->ffi=&ffi_type_slong; break;
  case 'L': t->ffi=&ffi_type_ulong; break;
  case 'q': t->ffi=&ffi_type_sint64; break;
  case 'Q': t->ffi=&ffi_type_uint64; break;
  case 'f': t->ffi=&ffi_type_float; break;
  case 'd': t->ffi=&ffi_type_double; break;
  case 'e': t->ffi=&ffi_type_longdouble; break;
  case 'p': case 'S': case 'E': {
    size_t n=0;
    while (**text>='0' && **text<='9') {
      if(n>1000000) cb_error("type label too long");
      n=n*10+(unsigned)(*(*text)++-'0');
    }
    if (*(*text)++!=':' || strlen(*text)<n) cb_error("invalid type label");
    *text+=n;
    if (t->kind=='p') { t->ffi=&ffi_type_pointer; break; }
    if (t->kind=='E') { t->ffi=&ffi_type_sint32; break; }
    goto fields;
  }
  case 'F':
    t->result=cb_parse(text);
  fields:
    if (*(*text)++!='(') cb_error("missing parameter list");
    while (**text && **text!=')') {
      if (t->count>=1024) cb_error("too many fields or parameters");
      struct cb_type **p=cb_alloc((t->count+1)*sizeof *p);
      if(t->count) memcpy(p,t->parts,t->count*sizeof *p);
      GC_free(t->parts); t->parts=p; t->parts[t->count++]=cb_parse(text);
    }
    if (*(*text)++!=')') cb_error("unterminated parameter list");
    t->ffi_parts=cb_alloc((t->count+1)*sizeof *t->ffi_parts);
    for (size_t i=0;i<t->count;++i) t->ffi_parts[i]=t->parts[i]->ffi;
    if (t->kind=='F') {
      t->ffi=&ffi_type_pointer;
      if (ffi_prep_cif(&t->cif,FFI_DEFAULT_ABI,t->count,t->result->ffi,t->ffi_parts)!=FFI_OK)
        cb_error("unsupported function ABI");
    } else {
      t->aggregate.type=FFI_TYPE_STRUCT; t->aggregate.elements=t->ffi_parts;
      t->ffi=&t->aggregate;
    }
    break;
  default: cb_error("unsupported type code");
  }
  t->signature=cb_copy(start,*text-start);
  return t;
}
static int cb_integer(const struct cb_type *t) { return strchr("bBhHiIlLqQE",t->kind)!=NULL; }
static int cb_pointer(const struct cb_type *t) { return t->kind=='p' || t->kind=='F'; }
static uint64_t cb_bits(const struct cb_type *t,const void *p) {
  if (cb_pointer(t)) { void *v; memcpy(&v,p,sizeof v); return (uintptr_t)v; }
  switch(t->kind) {
#define READ(K,T) case K: { T v; memcpy(&v,p,sizeof v); return (uint64_t)v; }
  READ('b',int8_t) READ('B',uint8_t) READ('h',int16_t) READ('H',uint16_t)
  READ('E',int32_t) READ('i',int32_t) READ('I',uint32_t) READ('l',long) READ('L',unsigned long)
  READ('q',int64_t) READ('Q',uint64_t)
#undef READ
  default: cb_error("nonintegral conversion");
  }
}
static void cb_write(const struct cb_type *t,void *p,uint64_t bits) {
  if (cb_pointer(t)) { void *v=(void *)(uintptr_t)bits; memcpy(p,&v,sizeof v); return; }
  switch(t->kind) {
#define WRITE(K,T) case K: { T v=(T)bits; memcpy(p,&v,sizeof v); return; }
  WRITE('b',int8_t) WRITE('B',uint8_t) WRITE('h',int16_t) WRITE('H',uint16_t)
  WRITE('E',int32_t) WRITE('i',int32_t) WRITE('I',uint32_t) WRITE('l',long) WRITE('L',unsigned long)
  WRITE('q',int64_t) WRITE('Q',uint64_t)
#undef WRITE
  default: cb_error("nonintegral conversion");
  }
}
static void cb_convert(const struct cb_type *from,const struct cb_type *to,const void *src,void *dst) {
  if (to->kind=='v') return;
  if (from->kind=='F' && to->kind=='F') {
    void *f; memcpy(&f,src,sizeof f);
    f=cb_adapt(f,from->signature,to->signature); memcpy(dst,&f,sizeof f); return;
  }
  if ((cb_integer(from)||cb_pointer(from)) && (cb_integer(to)||cb_pointer(to))) {
    uint64_t bits=cb_bits(from,src);
    // Match Toc's canonical boxed int representation, including negative ints.
    if (cb_integer(from) && from->ffi->size==4 && cb_pointer(to) && sizeof(void *)>4)
      bits=(uint32_t)bits;
    cb_write(to,dst,bits); return;
  }
  if (!strcmp(from->signature,to->signature)) { memcpy(dst,src,from->ffi->size); return; }
  cb_error("unsupported value conversion");
}
static size_t cb_slot_size(const struct cb_type *t) {
  size_t n=t->ffi->size;
  if (n<sizeof(ffi_arg)) n=sizeof(ffi_arg);
  const size_t a=_Alignof(max_align_t);
  if(n>SIZE_MAX-a) cb_error("argument size overflow");
  return (n+a-1)&~(a-1);
}
static void cb_invoke(ffi_cif *cif,void *ret,void **args,void *data) {
  struct cb_adapter *a=data;
  size_t total=cb_slot_size(a->from->result)+cb_slot_size(a->to->result);
  for(size_t i=0;i<a->from->count;++i) {
    size_t n=cb_slot_size(a->from->parts[i]);
    if(n>SIZE_MAX-total) cb_error("argument size overflow");
    total+=n;
  }
  _Alignas(max_align_t) unsigned char local_scratch[256];
  unsigned char *scratch=total<=sizeof local_scratch ? local_scratch : GC_malloc(total);
  if(!scratch) { _throw_badalloc_fn(__FILE__,__LINE__); return; }
  void *values[a->from->count ? a->from->count : 1];
  unsigned char *source_result=scratch;
  unsigned char *target_result=source_result+cb_slot_size(a->from->result);
  unsigned char *position=target_result+cb_slot_size(a->to->result);
  for(size_t i=0;i<a->from->count;++i) {
    values[i]=position;
    if(i<a->bound_count) memcpy(position,&a->bound[i],sizeof(unsigned long));
    else cb_convert(a->to->parts[i-a->bound_count],a->from->parts[i],args[i-a->bound_count],position);
    position+=cb_slot_size(a->from->parts[i]);
  }
  ffi_call(&a->from->cif,FFI_FN(a->original),source_result,values);
  if(cb_integer(a->from->result) && a->from->result->ffi->size<sizeof(ffi_arg)) {
    ffi_arg word; memcpy(&word,source_result,sizeof word);
    cb_write(a->from->result,source_result,word);
  }
  cb_convert(a->from->result,a->to->result,source_result,target_result);
  if(cb_integer(a->to->result) && a->to->result->ffi->size<sizeof(ffi_arg)) {
    ffi_arg word=(ffi_arg)cb_bits(a->to->result,target_result); memcpy(ret,&word,sizeof word);
  } else if(a->to->result->kind!='v') memcpy(ret,target_result,a->to->result->ffi->size);
  if(scratch!=local_scratch) GC_free(scratch);
}
static int cb_bound_equal(const struct cb_adapter *a,size_t n,const unsigned long *values) {
  return a->bound_count==n && (!n || !memcmp(a->bound,values,n*sizeof *values));
}
static void *cb_adapt_bound(void *fn,const char *from_text,const char *to_text,
                            size_t bound_count,const unsigned long *bound) {
  if (!fn || (!bound_count && !strcmp(from_text,to_text))) return fn;
  pthread_mutex_lock(&cb_lock);
  // Flatten adapters and recover the original callable on a round trip.
  for(struct cb_adapter *p=cb_cache;p;p=p->next) if(!bound_count && p->code==fn) {
    fn=p->original; from_text=p->from->signature;
    bound_count=p->bound_count; bound=p->bound; break;
  }
  if(!bound_count && !strcmp(from_text,to_text)) { pthread_mutex_unlock(&cb_lock); return fn; }
  for(struct cb_adapter *p=cb_cache;p;p=p->next)
    if(p->original==fn && cb_bound_equal(p,bound_count,bound) && !strcmp(p->from->signature,from_text) && !strcmp(p->to->signature,to_text)) {
      pthread_mutex_unlock(&cb_lock); return p->code;
    }
  pthread_mutex_unlock(&cb_lock);
  // Allocate without holding the mutex: a Cyclone allocation exception may
  // unwind the caller, and metadata construction can trigger collection.
  struct cb_adapter *a=cb_alloc(sizeof *a);
  const char *f=from_text,*t=to_text;
  a->from=cb_parse(&f); a->to=cb_parse(&t);
  if(*f || *t || a->from->kind!='F' || a->to->kind!='F' || a->from->count!=a->to->count+bound_count)
    cb_error("incompatible callback signatures");
  for(size_t i=0;i<bound_count;++i)
    if(a->from->parts[i]->kind!='L') cb_error("bound layout parameter is not unsigned long");
  a->bound_count=bound_count;
  if(bound_count) { a->bound=cb_alloc(bound_count*sizeof *bound); memcpy(a->bound,bound,bound_count*sizeof *bound); }
  a->original=fn;
  a->closure=ffi_closure_alloc(sizeof *a->closure,&a->code);
  if(!a->closure || ffi_prep_closure_loc(a->closure,&a->to->cif,cb_invoke,a,a->code)!=FFI_OK)
    cb_error("closure allocation failed");
  if(__lsan_register_root_region) __lsan_register_root_region(a->closure,sizeof *a->closure);
  pthread_mutex_lock(&cb_lock);
  for(struct cb_adapter *p=cb_cache;p;p=p->next)
    if(p->original==fn && cb_bound_equal(p,bound_count,bound) && !strcmp(p->from->signature,from_text) && !strcmp(p->to->signature,to_text)) {
      void *code=p->code;
      pthread_mutex_unlock(&cb_lock);
      if(a->closure) cb_closure_free(a->closure);
    cb_free_type(a->from); cb_free_type(a->to); GC_free(a->bound); GC_free(a);
      return code;
    }
  a->next=cb_cache; cb_cache=a;
  pthread_mutex_unlock(&cb_lock);
  return a->code;
}
static void *cb_adapt(void *fn,const char *from,const char *to) {
  return cb_adapt_bound(fn,from,to,0,NULL);
}
void *_cyc_callback_adapt(void *fn,const char *from,const char *to) { return cb_adapt(fn,from,to); }
void *_cyc_callback_bind(void *fn,const char *from,const char *to,unsigned n,...) {
  if(n>1024) cb_error("too many bound layouts");
  unsigned long values[n?n:1];
  va_list ap; va_start(ap,n);
  for(unsigned i=0;i<n;++i) values[i]=va_arg(ap,unsigned long);
  va_end(ap);
  return cb_adapt_bound(fn,from,to,n,values);
}
static void cb_register_static(void *wrapper,void *original,const char *from,const char *to,
                               unsigned n,const unsigned long *values) {
  struct cb_adapter *a=cb_alloc(sizeof *a);
  const char *f=from,*t=to;
  a->from=cb_parse(&f); a->to=cb_parse(&t);
  if(*f || *t || a->from->kind!='F' || a->to->kind!='F' || a->from->count!=a->to->count+n)
    cb_error("incompatible static callback signatures");
  a->bound_count=n;
  if(n) { a->bound=cb_alloc(n*sizeof *values); memcpy(a->bound,values,n*sizeof *values); }
  for(unsigned i=0;i<n;++i)
    if(a->from->parts[i]->kind!='L') cb_error("bound layout parameter is not unsigned long");
  a->original=original; a->code=wrapper;
  pthread_mutex_lock(&cb_lock);
  a->next=cb_cache; cb_cache=a;
  pthread_mutex_unlock(&cb_lock);
}
void _cyc_callback_register_static(void *wrapper,void *original,const char *from,const char *to) {
  cb_register_static(wrapper,original,from,to,0,NULL);
}
void _cyc_callback_register_bound_static(void *wrapper,void *original,const char *from,const char *to,unsigned n,...) {
  if(n>1024) cb_error("too many bound layouts");
  unsigned long values[n?n:1];
  va_list ap; va_start(ap,n);
  for(unsigned i=0;i<n;++i) values[i]=va_arg(ap,unsigned long);
  va_end(ap);
  cb_register_static(wrapper,original,from,to,n,values);
}
// Equality is based on the original function, including independently emitted
// static adapters in separate translation units. The cache lock keeps metadata
// stable while resolving identities; shutdown still requires quiescent callers.
static void *cb_original(void *fn) {
  for (;;) {
    struct cb_adapter *found=NULL;
    for(struct cb_adapter *p=cb_cache;p;p=p->next) if(p->code==fn) { found=p; break; }
    if(!found) return fn;
    fn=found->original;
  }
}
int _cyc_callback_equal(void *a,void *b) {
  pthread_mutex_lock(&cb_lock);
  int equal=cb_original(a)==cb_original(b);
  pthread_mutex_unlock(&cb_lock);
  return equal;
}
void _cyc_callback_shutdown(void) {
  pthread_mutex_lock(&cb_lock);
  while(cb_cache) {
    struct cb_adapter *a=cb_cache; cb_cache=a->next;
    if(a->closure) cb_closure_free(a->closure);
    cb_free_type(a->from); cb_free_type(a->to); GC_free(a->bound); GC_free(a);
  }
  pthread_mutex_unlock(&cb_lock);
}

#endif /* CYC_LIBFFI */

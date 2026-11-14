#include "polyref.h"
#include <string.h>
#include <limits.h>

size_t cyc_ref_width(const struct cyc_ref_layout *l) {
  switch(l->kind) {
  case CR_I8: case CR_U8: return 1;
  case CR_I16: case CR_U16: return 2;
  case CR_I32: case CR_U32: return 4;
  case CR_I64: case CR_U64: return 8;
  case CR_POINTER: case CR_REFERENCE: return sizeof(void *);
  case CR_FAT_POINTER: return sizeof(struct _fat_ptr);
  }
  _throw_arraybounds(); return 0;
}
static struct cyc_poly_ref *make_ref(uintptr_t current,uintptr_t base,uintptr_t end,
                                    const struct cyc_ref_layout *layout) {
  struct cyc_poly_ref *r=GC_malloc(sizeof *r);
  if(!r) { _throw_badalloc(); return NULL; }
  *r=(struct cyc_poly_ref){current,base,end,layout};return r;
}
struct cyc_poly_ref *cyc_ref_wrap(void *p,size_t n,const struct cyc_ref_layout *l) {
  if(!p) return NULL;
  uintptr_t b=(uintptr_t)p;size_t size=cyc_ref_width(l);
  if(n>(UINTPTR_MAX-b)/size) { _throw_arraybounds(); return NULL; }
  return make_ref(b,b,b+n*size,l);
}
struct cyc_poly_ref *cyc_ref_wrap_fat(struct _fat_ptr p,const struct cyc_ref_layout *l) {
  if(!p.curr && !p.base && !p.last_plus_one) return NULL;
  if((uintptr_t)p.base>(uintptr_t)p.last_plus_one) { _throw_arraybounds(); return NULL; }
  return make_ref((uintptr_t)p.curr,(uintptr_t)p.base,(uintptr_t)p.last_plus_one,l);
}
void *cyc_ref_address(const struct cyc_poly_ref *r) { return r ? (void *)r->current : NULL; }
struct _fat_ptr cyc_ref_fat(const struct cyc_poly_ref *r) {
  if(!r) return (struct _fat_ptr){0};
  return (struct _fat_ptr){(unsigned char *)r->current,(unsigned char *)r->base,(unsigned char *)r->end};
}
static void *checked_address(const struct cyc_poly_ref *r) {
  if(!r) { _throw_null(); return NULL; }
  size_t n=cyc_ref_width(r->layout);
  if(r->current<r->base || r->current>r->end || n>r->end-r->current) {
    _throw_arraybounds();return NULL;
  }
  return (void *)r->current;
}
uintptr_t cyc_ref_load_word(const struct cyc_poly_ref *r) {
  const void *p=checked_address(r);
  switch(r->layout->kind) {
#define LOAD(K,T) case K: { T v;memcpy(&v,p,sizeof v);return (uintptr_t)v; }
  LOAD(CR_I8,int8_t) LOAD(CR_U8,uint8_t) LOAD(CR_I16,int16_t) LOAD(CR_U16,uint16_t)
  /* Canonical boxed 32-bit values are zero extended, including signed ints. */
  LOAD(CR_I32,uint32_t) LOAD(CR_U32,uint32_t) LOAD(CR_I64,int64_t) LOAD(CR_U64,uint64_t)
  LOAD(CR_POINTER,uintptr_t) LOAD(CR_REFERENCE,uintptr_t)
#undef LOAD
  default:_throw_arraybounds();return 0;
  }
}
void cyc_ref_store_word(const struct cyc_poly_ref *r,uintptr_t word) {
  void *p=checked_address(r);
  switch(r->layout->kind) {
#define STORE(K,T) case K: { T v=(T)word;memcpy(p,&v,sizeof v);return; }
  STORE(CR_I8,int8_t) STORE(CR_U8,uint8_t) STORE(CR_I16,int16_t) STORE(CR_U16,uint16_t)
  STORE(CR_I32,uint32_t) STORE(CR_U32,uint32_t) STORE(CR_I64,int64_t) STORE(CR_U64,uint64_t)
  STORE(CR_POINTER,uintptr_t) STORE(CR_REFERENCE,uintptr_t)
#undef STORE
  default:_throw_arraybounds();
  }
}
struct cyc_poly_ref *cyc_ref_load_reference(const struct cyc_poly_ref *r) {
  const void *p=checked_address(r);
  switch(r->layout->kind) {
  case CR_REFERENCE: { struct cyc_poly_ref *v;memcpy(&v,p,sizeof v);return v; }
  case CR_POINTER: { void *v;memcpy(&v,p,sizeof v);
    return cyc_ref_wrap(v,r->layout->pointee_count,r->layout->pointee); }
  case CR_FAT_POINTER: { struct _fat_ptr v;memcpy(&v,p,sizeof v);
    return cyc_ref_wrap_fat(v,r->layout->pointee); }
  default:_throw_arraybounds();return NULL;
  }
}
void cyc_ref_store_reference(const struct cyc_poly_ref *r,struct cyc_poly_ref *v) {
  void *p=checked_address(r);
  switch(r->layout->kind) {
  case CR_REFERENCE:memcpy(p,&v,sizeof v);return;
  case CR_POINTER: { void *a=cyc_ref_address(v);memcpy(p,&a,sizeof a);return; }
  case CR_FAT_POINTER: { struct _fat_ptr a=cyc_ref_fat(v);memcpy(p,&a,sizeof a);return; }
  default:_throw_arraybounds();
  }
}
struct cyc_poly_ref *cyc_ref_step(const struct cyc_poly_ref *r,uintptr_t magnitude,int negative) {
  if(!r) { if(magnitude) _throw_arraybounds();return NULL; }
  size_t size=cyc_ref_width(r->layout);
  uintptr_t room=negative ? r->current : UINTPTR_MAX-r->current;
  if(magnitude>room/size) { _throw_arraybounds();return NULL; }
  uintptr_t bytes=magnitude*size;
  uintptr_t next=negative ? r->current-bytes : r->current+bytes;
  // Like Cyclone fat pointers, an out-of-bounds position may be reversed.
  // Dereference, not arithmetic alone, enforces the allocation's bounds.
  return make_ref(next,r->base,r->end,r->layout);
}
ptrdiff_t cyc_ref_difference(const struct cyc_poly_ref *a,const struct cyc_poly_ref *b) {
  if(!a || !b || a->base!=b->base || a->end!=b->end || a->layout!=b->layout) {
    _throw_arraybounds();return 0;
  }
  int negative=a->current<b->current;
  uintptr_t delta=negative ? b->current-a->current : a->current-b->current;
  size_t size=cyc_ref_width(a->layout);
  if(delta%size) { _throw_arraybounds();return 0; }
  uintptr_t n=delta/size;
  if(n>(uintptr_t)PTRDIFF_MAX+(negative ? 1U : 0U)) { _throw_arraybounds();return 0; }
  if(negative && n==(uintptr_t)PTRDIFF_MAX+1U) return PTRDIFF_MIN;
  return negative ? -(ptrdiff_t)n : (ptrdiff_t)n;
}
int cyc_ref_equal(const struct cyc_poly_ref *a,const struct cyc_poly_ref *b) {
  return cyc_ref_address(a)==cyc_ref_address(b);
}

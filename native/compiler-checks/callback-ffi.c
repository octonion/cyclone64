#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"FAIL %d: %s\n",__LINE__,#x); _exit(1); } } while(0)
void *GC_malloc(size_t n) { return calloc(1,n); }
void GC_free(void *p) { free(p); }
void *_throw_badalloc_fn(const char *f,unsigned l) { _exit(99); }
extern void *_cyc_callback_adapt(void *,const char *,const char *);
extern void _cyc_callback_shutdown(void);
extern void *_cyc_callback_bind(void *,const char *,const char *,unsigned,...);
extern void _cyc_callback_register_bound_static(void *,void *,const char *,const char *,unsigned,...);
extern int _cyc_callback_equal(void *,void *);
extern void _cyc_callback_register_static(void *,void *,const char *,const char *);
static void *layout_read(unsigned long width,void *p) {
  uintptr_t v=0;
  if(width==4) { uint32_t n; memcpy(&n,p,4); v=n; }
  else { CHECK(width==8); memcpy(&v,p,8); }
  return (void *)v;
}
static int bound_read32(int *p) { return (int)(uintptr_t)layout_read(4,p); }
static unsigned long layout_pair(unsigned long a,unsigned long b,unsigned long x) { return a*100+b*10+x; }
static int negate(int x) { return -x; }
static void *static_negate(void *x) { return (void *)(uintptr_t)(uint32_t)negate((int)(uintptr_t)x); }
static unsigned invert(unsigned x) { return ~x; }
static long long_value(long x) { return x+0x1234567800000000L; }
static int *identity(int *p) { return p; }
static double add_real(double x, double y) { return x+y; }
struct fat { void *curr,*base,*end; };
static struct fat fat_identity(int x,struct fat p) { CHECK(x==3); return p; }
static int call_fn(int (*f)(int),int x) { return f(x); }
static int (*return_fn(void))(int) { return negate; }
static void *thread(void *unused) {
  for(int i=0;i<1000;++i) {
    void *p=_cyc_callback_adapt((void *)negate,"Fi(i)","Fp1:v(p1:v)");
    CHECK((uintptr_t)((void *(*)(void *))p)((void *)(uintptr_t)7)==0xfffffff9U);
  }
  return NULL;
}
__attribute__((noinline)) static void leak_probe(void) {
  volatile unsigned char *p=malloc(73); p[0]=42;
}
int main(int argc,char **argv) {
  void *p=_cyc_callback_adapt((void *)negate,"Fi(i)","Fp1:v(p1:v)");
  CHECK((uintptr_t)((void *(*)(void *))p)((void *)(uintptr_t)7)==0xfffffff9U);
  CHECK(p==_cyc_callback_adapt((void *)negate,"Fi(i)","Fp1:v(p1:v)"));
  CHECK((void *)negate==_cyc_callback_adapt(p,"Fp1:v(p1:v)","Fi(i)"));
  CHECK(!_cyc_callback_adapt(NULL,"Fi(i)","Fp1:v(p1:v)"));
  p=_cyc_callback_adapt((void *)invert,"FI(I)","Fp1:v(p1:v)");
  CHECK((uintptr_t)((void *(*)(void *))p)(NULL)==UINT32_MAX);
  p=_cyc_callback_adapt((void *)long_value,"Fl(l)","Fp1:v(p1:v)");
  CHECK((uintptr_t)((void *(*)(void *))p)((void *)9)==0x1234567800000009UL);
  int n=2;
  p=_cyc_callback_adapt((void *)identity,"Fp1:i(p1:i)","Fp1:v(p1:v)");
  CHECK(((void *(*)(void *))p)(&n)==&n);
  // Same machine ABI, distinct nominal pointer signature: still adapt.
  CHECK(p!=(void *)identity);
  p=_cyc_callback_adapt((void *)add_real,"Fd(dd)","Fd(dd)");
  CHECK(((double (*)(double,double))p)(1.5,2.25)==3.75);
  struct fat f={&n,&n,&n+1};
  p=_cyc_callback_adapt((void *)fat_identity,"FS3:fat(p1:vp1:vp1:v)(iS3:fat(p1:vp1:vp1:v))","FS3:fat(p1:vp1:vp1:v)(p1:vS3:fat(p1:vp1:vp1:v))");
  struct fat got=((struct fat (*)(void *,struct fat))p)((void *)3,f);
  CHECK(!memcmp(&got,&f,sizeof f));
  p=_cyc_callback_adapt((void *)call_fn,"Fi(Fi(i)i)","Fp1:v(Fp1:v(p1:v)p1:v)");
  void *neg=_cyc_callback_adapt((void *)negate,"Fi(i)","Fp1:v(p1:v)");
  CHECK((uintptr_t)((void *(*)(void *(*)(void *),void *))p)(neg,(void *)11)==0xfffffff5U);
  p=_cyc_callback_adapt((void *)return_fn,"FFi(i)()","FFp1:v(p1:v)()");
  void *fn=((void *(*(*)(void))(void *))p)();
  CHECK(fn==neg);
  CHECK((uintptr_t)((void *(*)(void *))fn)((void *)13)==0xfffffff3U);
  int small=-17;
  long large=0x1234567800000012L;
  void *read32=_cyc_callback_bind((void *)layout_read,"Fp1:v(Lp1:v)","Fi(p1:i)",1,4UL);
  void *read64=_cyc_callback_bind((void *)layout_read,"Fp1:v(Lp1:v)","Fl(p1:l)",1,8UL);
  CHECK(((int (*)(int *))read32)(&small)==small);
  CHECK(((long (*)(long *))read64)(&large)==large);
  CHECK(read32==_cyc_callback_bind((void *)layout_read,"Fp1:v(Lp1:v)","Fi(p1:i)",1,4UL));
  void *readbox=_cyc_callback_adapt(read32,"Fi(p1:i)","Fp1:v(p1:v)");
  CHECK((uintptr_t)((void *(*)(void *))readbox)(&small)==(uint32_t)small);
  CHECK(read32==_cyc_callback_adapt(readbox,"Fp1:v(p1:v)","Fi(p1:i)"));
  _cyc_callback_register_bound_static((void *)bound_read32,(void *)layout_read,"Fp1:v(Lp1:v)","Fi(p1:i)",1,4UL);
  CHECK(_cyc_callback_equal(read32,(void *)bound_read32));
  void *again=_cyc_callback_adapt((void *)bound_read32,"Fi(p1:i)","Fp1:v(p1:v)");
  CHECK((uintptr_t)((void *(*)(void *))again)(&small)==(uint32_t)small);
  void *pair=_cyc_callback_bind((void *)layout_pair,"FL(LLL)","FL(L)",2,4UL,8UL);
  CHECK(((unsigned long (*)(unsigned long))pair)(3)==483);
  void *different=_cyc_callback_bind((void *)layout_pair,"FL(LLL)","FL(L)",2,8UL,4UL);
  CHECK(((unsigned long (*)(unsigned long))different)(3)==843);
  CHECK(pair!=different);
  // Exercise new executable pages, including a first trampoline at a page
  // boundary. Clang's function sanitizer reads metadata before each target.
  for(unsigned long i=1;i<=2048;++i) {
    void *many=_cyc_callback_bind((void *)layout_pair,"FL(LLL)","FL(L)",2,i,8UL);
    CHECK((uintptr_t)many % (unsigned long)sysconf(_SC_PAGESIZE) >= 8);
    CHECK(((unsigned long (*)(unsigned long))many)(3)==i*100+83);
  }
  pthread_t ts[4]; for(int i=0;i<4;++i) CHECK(!pthread_create(&ts[i],NULL,thread,NULL));
  for(int i=0;i<4;++i) CHECK(!pthread_join(ts[i],NULL));
  _cyc_callback_register_static((void *)static_negate,(void *)negate,"Fi(i)","Fp1:v(p1:v)");
  CHECK(_cyc_callback_adapt((void *)static_negate,"Fp1:v(p1:v)","Fi(i)")==(void *)negate);
  CHECK(_cyc_callback_equal((void *)static_negate,(void *)negate));
  CHECK(_cyc_callback_equal(neg,(void *)static_negate));
  CHECK(!_cyc_callback_equal(neg,(void *)invert));
  CHECK(_cyc_callback_equal(NULL,NULL));
  CHECK(!_cyc_callback_equal(NULL,neg));
  if(argc>1 && !strcmp(argv[1],"--leak-probe")) leak_probe();
  if(!(argc>1 && !strcmp(argv[1],"--keep-cache"))) _cyc_callback_shutdown();
  puts("libffi callback ABI: signed/unsigned, long, pointers, aggregate, higher order, identity, threads: PASS");
}

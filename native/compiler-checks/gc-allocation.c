#define DISABLE_REAPS
#include "cyc_include.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
extern void *GC_realloc(void *, size_t);
extern void GC_free(void *);
extern size_t GC_size(const void *);
static jmp_buf badalloc;
static int expect_badalloc;
void *_throw_badalloc_fn(const char *f, unsigned line) {
  if (!expect_badalloc) _exit(99);
  longjmp(badalloc, 1);
}
void *_throw_bad_reapalloc_fn(const char *f,unsigned l) { return _throw_badalloc_fn(f,l); }
#define REJECT(expr) do { expect_badalloc=1; if (!setjmp(badalloc)) { \
  (void)(expr); _exit(98); } expect_badalloc=0; } while (0)
int main(void) {
  unsigned char *p=GC_malloc(64);
  assert((uintptr_t)p % _Alignof(max_align_t)==0 && GC_size(p)==64);
  for (size_t i=0;i<64;++i) assert(p[i]==0);
  memset(p,0x5a,64);
  p=GC_realloc(p,4096);
  for (size_t i=0;i<64;++i) assert(p[i]==0x5a);
  for(size_t i=64;i<4096;++i) assert(p[i]==0);
  memset(p+64,0x6b,4096-64);
  assert(GC_size(p)==4096 && p[4095]==0x6b);
  p=GC_realloc(p,17); assert(GC_size(p)==17 && p[16]==0x5a);
  GC_free(p); GC_free(NULL);
  p=GC_realloc(NULL,31); assert(GC_size(p)==31); GC_free(p);
  p=GC_malloc(0); assert(p && GC_size(p)==0); GC_free(p);
  p=GC_calloc(7,13); for(int i=0;i<91;++i) assert(p[i]==0); GC_free(p);
  p=GC_calloc_atomic(9,11); for(int i=0;i<99;++i) assert(p[i]==0); GC_free(p);
  REJECT(GC_malloc(SIZE_MAX));
  REJECT(GC_malloc_atomic(SIZE_MAX));
  REJECT(GC_realloc(NULL,SIZE_MAX));
  REJECT(GC_calloc(SIZE_MAX/2+1,2));
  REJECT(GC_calloc_atomic(SIZE_MAX/2+1,2));
  REJECT(_bounded_GC_malloc((size_t)PTRDIFF_MAX,__FILE__,__LINE__));
  REJECT(_bounded_GC_malloc_atomic((size_t)PTRDIFF_MAX,__FILE__,__LINE__));
  REJECT(_bounded_GC_calloc(SIZE_MAX/2+1,2,__FILE__,__LINE__));
  REJECT(_bounded_GC_calloc_atomic(SIZE_MAX/2+1,2,__FILE__,__LINE__));
  p=_bounded_GC_calloc(SIZE_MAX,0,__FILE__,__LINE__); GC_free(p);
  assert(_check_times(0x100000001UL,1)==0x100000001UL);
  assert(_check_times(1,0x100000001UL)==0x100000001UL);
  assert(_check_times(INT_MAX,2)==4294967294UL);
  assert(_check_times(UINT_MAX,2U)==8589934590UL);
  assert(_check_alloc_plus(INT_MAX,1)==2147483648UL);
  assert(_check_alloc_plus(UINT_MAX,1U)==4294967296UL);
  REJECT(_check_times(PTRDIFF_MAX,1));
  REJECT(_check_times(SIZE_MAX,2));
  REJECT(_check_times(-1L,1));
  REJECT(_check_times(1,-1L));
  REJECT(_check_times(((unsigned __int128)1<<100),1));
  REJECT(_check_alloc_plus(SIZE_MAX,1));
  REJECT(_check_alloc_plus(-1L,1));
  REJECT(_check_alloc_plus(((unsigned __int128)1<<100),1));
  assert(_check_alloc_plus(MAX_MALLOC_SIZE-1,1)==MAX_MALLOC_SIZE);
  REJECT(_check_alloc_size(-1L));
  REJECT(_check_alloc_size(((unsigned __int128)1<<100)+16));
  assert(_check_alloc_size(SIZE_MAX)==SIZE_MAX);
  unsigned count=2; assert(_check_times(count++,5)==10 && count==3);
  assert(_check_alloc_size(count++)==3 && count==4);
  assert(_check_alloc_plus(count++,2)==6 && count==5);
  REJECT(_check_times(((unsigned __int128)1<<100),0));
  REJECT(_check_alloc_plus(0,-1L));
  assert(_check_times(MAX_MALLOC_SIZE,1)==MAX_MALLOC_SIZE);
  for (unsigned aq=0;aq<3;++aq) {
    REJECT(_aqual_malloc(aq,SIZE_MAX));
    REJECT(_aqual_calloc(aq,SIZE_MAX/2+1,2));
    REJECT(_region_malloc(NULL,aq,SIZE_MAX));
    REJECT(_region_calloc(NULL,aq,SIZE_MAX/2+1,2));
    for (unsigned plain=0;plain<2;++plain) {
      struct _RegionHandle r = _new_region(plain,"allocation-test");
      _push_region(&r);
      p=_region_calloc(&r,aq,7,13);
      assert((uintptr_t)p % _Alignof(max_align_t)==0);
      for(int i=0;i<91;++i) assert(p[i]==0);
      memset(p,0x42,91);
      REJECT(_region_malloc(&r,aq,SIZE_MAX));
      REJECT(_region_calloc(&r,aq,SIZE_MAX/2+1,2));
      REJECT(_fast_region_malloc(&r,aq,(size_t)PTRDIFF_MAX));
      if (plain) {
        void *a=_fast_region_malloc(&r,aq,0);
        void *b=_fast_region_malloc(&r,aq,0);
        assert(a!=b);
      }
      _pop_region();
    }
  }
  extern int Cyc_set_default_region_page_size(size_t);
  assert(!Cyc_set_default_region_page_size(SIZE_MAX));
  puts("GC allocation: size, alignment, zeroing, realloc, null, and overflow: PASS");
}

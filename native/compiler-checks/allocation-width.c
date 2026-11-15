/* Validate the exact byte count handed to the collector without huge memory. */
#include "cyc_include.h"
#include <assert.h>
#include <stdio.h>
static size_t requested, calls;
static max_align_t storage[64];
static jmp_buf rejected;
void *GC_malloc(size_t n) { requested=n; ++calls; return storage; }
void *GC_malloc_atomic(size_t n) { return GC_malloc(n); }
void *_throw_badalloc_fn(const char *file, unsigned line) { longjmp(rejected,1); }
void *_throw_bad_reapalloc_fn(const char *file, unsigned line) { longjmp(rejected,1); }
#define REJECT(e) do { size_t before=calls; if (!setjmp(rejected)) { (void)(e); assert(!"missing rejection"); } assert(calls==before); } while(0)
int main(void) {
  const size_t n=((size_t)1<<32)+17;
  _bounded_GC_malloc(n,__FILE__,__LINE__); assert(requested==n);
  _bounded_GC_malloc_atomic(n,__FILE__,__LINE__); assert(requested==n);
  _bounded_GC_calloc(n,8,__FILE__,__LINE__); assert(requested==n*8);
  _region_malloc(NULL,0,n); assert(requested==n);
  REJECT(_bounded_GC_malloc(PTRDIFF_MAX,__FILE__,__LINE__));
  REJECT(_bounded_GC_calloc(PTRDIFF_MAX/2+1,2,__FILE__,__LINE__));
  REJECT(_region_malloc(NULL,2,PTRDIFF_MAX-1)); // reference-count header
  for (unsigned plain=0;plain<2;++plain) {
    struct _RegionHandle r=_new_region(plain,"overflow");
    for (unsigned aq=0;aq<3;++aq) {
      REJECT(_region_malloc(&r,aq,PTRDIFF_MAX-1)); // alignment/page header
      REJECT(_region_calloc(&r,aq,SIZE_MAX,2));
    }
  }
  puts("collector requests retain >4-GiB widths; overhead rejects before allocation: PASS");
}

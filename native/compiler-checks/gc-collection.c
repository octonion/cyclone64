#include <gc/gc.h>
#include "cyc_include.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#x); _exit(1); } } while(0)
void *_throw_badalloc_fn(const char *f, unsigned line) { _exit(99); }
void *_throw_bad_reapalloc_fn(const char *f, unsigned line) { _exit(99); }
extern void *cyc_vmalloc(size_t);
extern void cyc_vfree(void *);
static unsigned char *interior_root;

__attribute__((noinline)) static void garbage(void) {
  for (int i=0;i<4096;++i) {
    unsigned char *p = GC_calloc_atomic(1, 8192);
    CHECK(p && p[0]==0 && p[8191]==0);
    memset(p, 0x71, 8192);
    GC_reachable_here(p);
  }
}

int main(void) {
  GC_INIT();
  GC_set_max_heap_size(64UL*1024*1024);
  interior_root = (unsigned char *)cyc_vmalloc(4096) + 37;
  memset(interior_root, 0x5a, 1024);
  unsigned long before = GC_get_gc_no();
  for (int i=0;i<8;++i) {
    garbage();
    GC_gcollect();
    for (int j=0;j<1024;++j) CHECK(interior_root[j]==0x5a);
    CHECK(GC_get_heap_size() < 32UL*1024*1024);
  }
  CHECK(GC_get_gc_no()>before && GC_get_total_bytes()>256UL*1024*1024);
  printf("GC collection: allocated=%zu heap=%zu collections=%lu; interior root intact: PASS\n",
         GC_get_total_bytes(), GC_get_heap_size(), (unsigned long)GC_get_gc_no()-before);
  cyc_vfree(interior_root-37);
  interior_root=NULL;
  return 0;
}

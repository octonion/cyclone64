/* Platform allocator bridge for an explicitly selected BDW-GC build.
 * The malloc-backed nogc build remains available for ASan redzones. */
#ifdef CYC_USE_SYSTEM_GC
#include <gc/gc.h>
#include <stdio.h>
#include "cyc_include.h"

void *cyc_vmalloc(size_t size) {
  void *p = GC_malloc(size);
  if (!p) _throw_badalloc();
  return p;
}

void cyc_vfree(void *p) { GC_free(p); }

/* The historical collector exported this helper; current BDW-GC only
 * exports the setter/getter. Preserve the runtime's warning controls. */
void GC_default_warn_proc(char *message, GC_word argument) {
  fputs("GC Warning: ", stderr);
  fprintf(stderr, message, argument);
}
#endif

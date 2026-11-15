/* Platform allocator bridge for an explicitly selected BDW-GC build.
 * The malloc-backed nogc build remains available for ASan redzones. */
#ifdef CYC_USE_SYSTEM_GC
#define GC_THREADS 1
#include <gc/gc.h>
#include <gc/gc_pthread_redirects.h>
#include <stdio.h>
#include "cyc_include.h"

#ifndef __has_feature
#define __has_feature(x) 0
#endif
#if (defined(__SANITIZE_ADDRESS__) || __has_feature(address_sanitizer)) && !defined(CYC_CUSTOM_ASAN_DEFAULTS)
/* BDW-GC 8.2.12 scans real stacks, not ASan's fake stacks. Match upstream's
 * os_dep.c defaults in the executable when using the unsanitized system DSO:
 * https://github.com/bdwgc/bdwgc/blob/v8.2.12/os_dep.c#L479-L500
 * Heap/stack bounds, use-after-free and leak checking remain enabled. The
 * malloc-backed test configuration retains use-after-return instrumentation.
 */
const char *__asan_default_options(void) {
  return "allow_user_segv_handler=1:detect_stack_use_after_return=0";
}
#endif

int _cyc_pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                        void *(*start)(void *), void *arg) {
  return GC_pthread_create(thread, attr, start, arg);
}

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

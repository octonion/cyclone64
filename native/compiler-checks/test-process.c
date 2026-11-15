/* Test-local crash reporting control; never linked into production libraries. */
#include <stdio.h>
#include <sys/resource.h>
#ifdef __linux__
#include <sys/prctl.h>
#endif
__attribute__((constructor)) static void test_process_setup(void) {
  struct rlimit limit = {0, 0};
  setrlimit(RLIMIT_CORE, &limit);
#ifdef __linux__
  prctl(PR_SET_DUMPABLE, 0, 0, 0, 0);
#endif
  /* Signal tests terminate deliberately; preserve their assertions/output. */
  setvbuf(stdout, NULL, _IONBF, 0);
}

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "regions.h"
/* Only the child expected to call abort installs this handler. The parent
 * still distinguishes a missing abort from the expected rejection. Exiting
 * from the handler avoids systemd-coredump/desktop notifications entirely. */
static void expected_abort(int signal_number) { _exit(100 + signal_number); }
static void rejected(int multiply) {
  pid_t pid = fork(); assert(pid >= 0);
  if (!pid) {
    struct sigaction action = {0};
    action.sa_handler = expected_abort;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGABRT, &action, NULL)) _exit(99);
    region r = newregion();
    if (multiply) typed_rarrayalloc(r, SIZE_MAX / 2 + 1, 2, NULL);
    else rstralloc(r, (size_t)UINT32_MAX + 1);
    _exit(0);
  }
  int status; assert(waitpid(pid, &status, 0) == pid);
  assert(WIFEXITED(status) && WEXITSTATUS(status) == 100 + SIGABRT);
}
int main(void) {
  assert(sizeof(void *) == 8 && sizeof(long) == 8);
  region_init(); int local;
  assert(regionof(NULL) == &zeroregion && regionof(&local) == &zeroregion);
  const size_t sizes[] = {0, 1, 7, 32, 512, 513, 2048, 8192, 1048576};
  for (int round=0; round<8; ++round) {
    region r = newregion(), child = newsubregion(r);
    for (size_t i=0; i<sizeof(sizes)/sizeof(*sizes); ++i) {
      char *p = rstralloc(r, sizes[i]);
      assert((uintptr_t)p > UINT32_MAX && (uintptr_t)p % _Alignof(void *) == 0);
      assert(regionof(p) == r);
      if (sizes[i]) { assert(!p[0] && !p[sizes[i]-1]); memset(p, 0x5a, sizes[i]); }
    }
    char *p = rstrextend0(child, NULL, 32); strcpy(p, "native LP64");
    p = rstrextend0(child, p, 4096); assert(!strcmp(p, "native LP64") && !p[4095]);
    p = rstrextend0(child, p, 6); assert(!memcmp(p, "native", 6));
    deleteregion(child); assert(regionof(p) == &zeroregion);
    deleteregion(r);
  }
  rejected(0); rejected(1);
  puts("region: ownership, page reuse, arrays, extension, overflow rejection: PASS");
}

/* Historical blocker, now expected to pass. Extended coverage lives in tests/setst-rollback.c. */
#include <stdio.h>
#include "setst-sort.h"
static void match(gen_e a, gen_e b) {}
static void print(FILE *f, gen_e e) {}
static void incl(gen_e a, gen_e b) { setst_inclusion(match, print, a, b); }
int main(void) {
  engine_init(); setst_init();
  gen_e v = setst_fresh("v"), c = setst_constant("c");
  banshee_clock_tick(); incl(c, v);
  if (gen_e_list_length(setst_tlb(v, incl)) != 1) return 2;
  banshee_rollback();
  if (gen_e_list_length(setst_tlb(v, incl)) != 0) {
    fputs("BLOCKER: setst retains constraint after banshee_rollback\n", stderr);
    return 1;
  }
  return 0;
}

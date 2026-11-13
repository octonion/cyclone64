#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "banshee.h"
#include "ufind.h"
#include "term-sort.h"
#include "setif-sort.h"
#include "setst-sort.h"
static int errors;
static void error(gen_e a, gen_e b, banshee_error_kind k) { ++errors; }
static void match(gen_e a, gen_e b) { assert(!"unexpected constructed term"); }
static bool occurs(term_var a, gen_e b) { return FALSE; }
static bool project(setif_var a, gen_e b) { return FALSE; }
static void print(FILE *f, gen_e e) {}
static void st_incl(gen_e a, gen_e b) { setst_inclusion(match, print, a, b); }
static bool contains(gen_e_list l, gen_e x) {
  gen_e_list_scanner scan; gen_e e;
  gen_e_list_scan(l, &scan);
  while (gen_e_list_next(&scan, &e)) if (e == x) return TRUE;
  return FALSE;
}
int main(void) {
  engine_init(); term_init(); setif_init(); setst_init(); handle_error = error;
  region r = newregion();
  int a = 1, b = 2;
  uf_element u = new_uf_element(r, &a, 0), v = new_uf_element(r, &b, 0);
  uf_tick(); uf_union(u, v); assert(uf_eq(u, v)); uf_rollback();
  assert(!uf_eq(u, v)); assert(uf_get_info(u) == &a); assert(uf_get_info(v) == &b);
  gen_e x = term_fresh("x"), y = term_fresh("y"), c = term_constant("c");
  banshee_clock_tick();
  term_unify(match, occurs, x, y); term_unify(match, occurs, y, c);
  assert(term_eq(term_get_ecr(x), c));
  banshee_rollback(); assert(term_is_var(term_get_ecr(x))); assert(!term_eq(x, y));
  term_unify(match, occurs, c, term_constant("different")); assert(errors == 1);
  gen_e sx = setif_fresh("sx"), sy = setif_fresh("sy"), sc = setif_constant("sc");
  banshee_clock_tick();
  setif_inclusion(match, project, print, sc, sx);
  setif_inclusion(match, project, print, sx, sy);
  assert(contains(setif_tlb(sy), sc));
  banshee_rollback(); assert(!contains(setif_tlb(sy), sc));
  gen_e tx = setst_fresh("tx"), ty = setst_fresh("ty"), tc = setst_constant("tc");
  st_incl(tc, tx); st_incl(tx, ty); assert(contains(setst_tlb(ty, st_incl), tc));
  puts("solver: union-find, term, setif rollback; setst propagation: PASS");
}

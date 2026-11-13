#include <assert.h>
#include <stdio.h>
#include "setst-sort.h"
static int matches;
static gen_e projection(gen_e_list sinks) { return gen_e_list_empty(sinks) ? NULL : gen_e_list_head(sinks); }
static void match(gen_e a, gen_e b) { ++matches; }
static void print(FILE *f, gen_e e) {}
static void incl(gen_e a, gen_e b) { setst_inclusion(match, print, a, b); }
static int has(gen_e v, gen_e c) {
  gen_e_list_scanner scan; gen_e next;
  gen_e_list_scan(setst_tlb(v, incl), &scan);
  while (gen_e_list_next(&scan, &next)) if (next == c) return 1;
  return 0;
}
int main(void) {
  engine_init(); setst_init();
  gen_e a=setst_fresh("a"), b=setst_fresh("b"), c=setst_fresh("c");
  gen_e x=setst_constant("x"), y=setst_constant("y"), sink=setst_constant("sink");
  incl(x,a); assert(has(a,x));
  banshee_clock_tick(); incl(a,b); incl(y,b); assert(has(b,x)&&has(b,y));
  banshee_clock_tick(); incl(b,a); incl(b,c); incl(c,sink);
  assert(has(c,x)&&has(c,y)); assert(setst_eq(a,b)); assert(matches>0);
  banshee_rollback(); assert(!setst_eq(a,b)); assert(!has(a,y));
  assert(has(b,x)&&has(b,y)); assert(!has(c,x)&&!has(c,y));
  banshee_rollback(); assert(has(a,x)); assert(!has(b,x)&&!has(b,y));
  /* Reuse the same clock values on a different branch; re-propagate sinks. */
  matches=0; banshee_clock_tick(); incl(y,c); incl(c,sink); assert(has(c,y)); assert(matches>0);
  assert(st_get_ub_proj((setst_var)c,projection)==sink);
  banshee_rollback(); assert(!has(c,y));
  assert(st_get_ub_proj((setst_var)c,projection)==NULL);
  matches=0; banshee_clock_tick(); incl(x,c); incl(c,sink); assert(has(c,x)); assert(matches>0);
  banshee_rollback();
  region r=newregion(); gen_e_list l=new_gen_e_list(r);
  gen_e_list_cons(a,l); gen_e_list_cons(b,l);
  gen_e u=setst_union(l), inter=setst_inter(l);
  assert(setst_is_inter(inter)); assert(!setst_is_union(inter));
  banshee_clock_tick(); setst_set_proj_cache(u,x);
  assert(gen_e_list_length(setst_get_proj_cache(u))==1);
  banshee_clock_tick(); setst_set_proj_cache(u,y);
  assert(gen_e_list_length(setst_get_proj_cache(u))==2);
  banshee_rollback(); assert(gen_e_list_length(setst_get_proj_cache(u))==1);
  banshee_rollback(); assert(gen_e_list_empty(setst_get_proj_cache(u)));
  /* Constant <= intersection decomposes into constraints on both operands. */
  banshee_clock_tick(); incl(y,inter); assert(has(a,y)&&has(b,y));
  banshee_rollback(); assert(!has(a,y)&&!has(b,y));
  puts("setst: nested rollback, cycle splitting, branch reuse, sinks, projections: PASS");
}

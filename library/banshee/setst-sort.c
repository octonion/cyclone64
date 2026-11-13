/*
 * Copyright (c) 2000-2004
 *      The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include "setst-sort.h"
#include "banshee_persist_kinds.h"

#include <assert.h>
#include <stdio.h>

#include "bounds.h"
#include "regions.h"

// You probably don't want to turn this off-- the thing won't terminate
bool flag_eliminate_cycles_st = TRUE;

struct setst_union_ {
#ifdef NONSPEC
  sort_kind sort;
#endif
  int type;
  stamp st;
  gen_e_list exprs;
  gen_e_list proj_cache;
};

struct setst_inter_ {
#ifdef NONSPEC
  sort_kind sort;
#endif
  int type;
  stamp st;
  gen_e_list exprs;
};

struct setst_constant_ {
#ifdef NONSPEC
  sort_kind sort;
#endif
  int type;
  stamp st;
  char *name;
};

typedef struct setst_inter_ *setst_inter_;
typedef struct setst_union_ *setst_union_;
typedef struct setst_constant_ *setst_constant_;

static region tlb_cache_region;
static jcoll_dict tlb_dict;
static setst_var_list setst_vars;
static bool setst_changed = FALSE;

region setst_region;
term_hash setst_hash;
struct setst_stats setst_stats;

struct setst_cache_undo {
  setst_union_ term;
  gen_e_list previous;
  struct setst_cache_undo *next;
};
struct setst_rollback_info_ {
  int time;
  sort_kind kind;
  struct setst_cache_undo *caches;
  struct setst_rollback_info_ *previous;
};
static struct setst_rollback_info_ *setst_current_rollback;

void setst_register_rollback(void) {
#ifdef BANSHEE_ROLLBACK
  if (!banshee_check_rollback(setst_sort)) {
    struct setst_rollback_info_ *info = ralloc(setst_region, struct setst_rollback_info_);
    banshee_set_time((banshee_rollback_info)info);
    info->kind = setst_sort;
    info->previous = setst_current_rollback;
    setst_current_rollback = info;
    banshee_register_rollback((banshee_rollback_info)info);
  }
#endif
}

stamp setst_get_stamp(gen_e e) {
#ifdef NONSPEC
  assert(e->sort == setst_sort);
#endif

  if (((setst_term)e)->type == VAR_TYPE)
    return st_get_stamp((setst_var)e);

  else
    return ((setst_term)e)->st;
}

static bool eq(gen_e e1, gen_e e2) { return (setst_get_stamp(e1) == setst_get_stamp(e2)); }

bool setst_eq(gen_e e1, gen_e e2) { return eq(e1, e2); }

static gen_e_list get_union(gen_e e) {
  assert(((setst_term)e)->type == UNION_TYPE);

  return ((setst_union_)e)->exprs;
}

static gen_e_list get_inter(gen_e e) {
  assert(((setst_term)e)->type == INTER_TYPE);

  return ((setst_inter_)e)->exprs;
}

static void update_lower_bound(setst_var v, gen_e e) {
  if (setst_is_var(e)) {
    if (st_add_lb(v, (setst_var)e)) {
      setst_stats.redundant_var++;
    } else {
      setst_stats.added_var++;
      setst_changed = TRUE;
    }
  } else {
    if (st_add_source(v, e, setst_get_stamp(e))) {
      setst_stats.redundant_source++;
    } else {
      setst_stats.added_source++;
      setst_changed = TRUE;
    }
  }
}

static void update_upper_bound(setst_var v, gen_e e) {
  assert(!setst_is_var(e));

  if (st_add_sink(v, e, setst_get_stamp(e))) {
    setst_stats.redundant_sink++;
  } else {
    setst_stats.added_sink++;
    setst_changed = TRUE;
  }
}

void setst_inclusion(con_match_fn_ptr con_match, gen_e_pr_fn_ptr pr, gen_e e1, gen_e e2) {
  //   pr(stdout, e1);
  //   fprintf(stdout, "<= ");
  //   pr(stdout, e2);
  //   fprintf(stdout, "\n");

  if (eq(e1, e2))
    return;

  else if (setst_is_zero(e1) || setst_is_one(e2))
    return;

  else if (setst_is_union(e1)) {
    gen_e_list_scanner scan;
    gen_e temp;

    gen_e_list exprs = get_union(e1);

    gen_e_list_scan(exprs, &scan);
    while (gen_e_list_next(&scan, &temp)) {
      setst_inclusion(con_match, pr, temp, e2);
    }

    return;
  }

  else if (setst_is_inter(e2)) {
    gen_e_list_scanner scan;
    gen_e temp;

    gen_e_list exprs = get_inter(e2);

    gen_e_list_scan(exprs, &scan);
    while (gen_e_list_next(&scan, &temp)) {
      setst_inclusion(con_match, pr, e1, temp);
    }

    return;
  }

  else if (setst_is_var(e2)) {
    setst_var v = (setst_var)e2;

    update_lower_bound(v, e1);
  }

  else if (setst_is_var(e1)) {
    setst_var v = (setst_var)e1;

    update_upper_bound(v, e2);
  }

  else
    con_match(e1, e2);
}

#ifdef NONSPEC
static struct setst_term zero = {ZERO_TYPE, setst_sort, ZERO_TYPE};
static struct setst_term one = {ONE_TYPE, setst_sort, ONE_TYPE};
#else
static struct setst_term zero = {ZERO_TYPE, ZERO_TYPE};
static struct setst_term one = {ONE_TYPE, ONE_TYPE};
#endif /* NONSPEC */

gen_e setst_zero(void) { return (gen_e)&zero; }

gen_e setst_one(void) { return (gen_e)&one; }

gen_e setst_fresh(const char *name) {
  setst_var v = st_fresh(setst_region, name);
  setst_var_list_cons(v, setst_vars);
  return (gen_e)v;
}

gen_e setst_fresh_large(const char *name) {
  setst_var v = st_fresh_large(setst_region, name);
  setst_var_list_cons(v, setst_vars);
  return (gen_e)v;
}

gen_e setst_fresh_small(const char *name) {
  setst_var v = st_fresh_small(setst_region, name);
  setst_var_list_cons(v, setst_vars);
  return (gen_e)v;
}

gen_e setst_constant(const char *str) deletes {
  stamp st[2];
  gen_e result;
  char *name = rstrdup(setst_region, str);

  assert(str != NULL);

  st[0] = CONSTANT_TYPE;
  st[1] = stamp_string(name);

  if ((result = term_hash_find(setst_hash, st, 2)) == NULL) {
    setst_constant_ c = ralloc(setst_region, struct setst_constant_);
    c->type = CONSTANT_TYPE;
    c->st = stamp_fresh();
    c->name = name;

    result = (gen_e)c;
    term_hash_insert(setst_hash, result, st, 2);

    setst_stats.distinct_constants++;

    return result;
  }

  else {
    setst_stats.hashed_constants++;
    return result;
  }
}

static bool filter_zero(const gen_e e) { return (!setst_is_zero(e)); }

static bool filter_one(const gen_e e) { return (!setst_is_one(e)); }

gen_e setst_union(gen_e_list exprs) deletes {
  gen_e_list filtered = gen_e_list_filter(setst_region, exprs, filter_zero);

  if (gen_e_list_empty(filtered)) {
    setst_stats.filtered_unions++;
    return setst_zero();
  } else if (gen_e_list_length(filtered) == 1) {
    setst_stats.filtered_unions++;
    return gen_e_list_head(filtered);
  }

  else {
    int i = 0;
    gen_e temp, result;
    gen_e_list_scanner scan;
    stamp st[gen_e_list_length(filtered) + 1];

    st[0] = UNION_TYPE;

    gen_e_list_scan(filtered, &scan);
    while (gen_e_list_next(&scan, &temp)) {
      st[++i] = setst_get_stamp(temp);
    }

    if ((result = term_hash_find(setst_hash, st, gen_e_list_length(filtered) + 1)) == NULL) {
      struct setst_union_ *u = ralloc(setst_region, struct setst_union_);

      u->type = UNION_TYPE;
      u->st = stamp_fresh();
      u->proj_cache = new_gen_e_list(setst_region);
      u->exprs = filtered;

      result = (gen_e)u;
      term_hash_insert(setst_hash, result, st, gen_e_list_length(filtered) + 1);

      setst_stats.distinct_unions++;
      return result;
    } else {
      setst_stats.hashed_unions++;
      return result;
    }
  }
}

gen_e setst_inter(gen_e_list exprs) deletes {
  gen_e_list filtered = gen_e_list_filter(setst_region, exprs, filter_one);

  if (gen_e_list_empty(filtered)) {
    setst_stats.filtered_intersections++;
    return setst_one();
  } else if (gen_e_list_length(filtered) == 1) {
    setst_stats.filtered_intersections++;
    return gen_e_list_head(filtered);
  }

  else {
    int i = 0;
    gen_e temp, result;
    gen_e_list_scanner scan;
    stamp st[gen_e_list_length(filtered) + 1];

    st[0] = INTER_TYPE;

    gen_e_list_scan(filtered, &scan);
    while (gen_e_list_next(&scan, &temp)) {
      st[++i] = setst_get_stamp(temp);
    }

    if ((result = term_hash_find(setst_hash, st, gen_e_list_length(filtered) + 1)) == NULL) {
      struct setst_inter_ *u = ralloc(setst_region, struct setst_inter_);

      u->type = INTER_TYPE;
      u->st = stamp_fresh();
      u->exprs = filtered;

      result = (gen_e)u;
      term_hash_insert(setst_hash, result, st, gen_e_list_length(filtered) + 1);

      setst_stats.distinct_intersections++;

      return result;
    } else {
      setst_stats.hashed_intersections++;
      return result;
    }
  }
}

gen_e_list setst_get_union(gen_e e) {
  assert(((setst_term)e)->type == UNION_TYPE);

  return ((setst_union_)e)->exprs;
}

gen_e_list setst_get_inter(gen_e e) {
  assert(((setst_term)e)->type == INTER_TYPE);

  return ((setst_inter_)e)->exprs;
}

static void invalidate_tlb_cache(void) {
  assert(tlb_cache_region);

  jcoll_delete_dict(tlb_dict);
  setst_var_list_app(setst_vars, st_clear_tlb_cache);
  deleteregion_ptr(&tlb_cache_region);

  tlb_cache_region = newregion();
  tlb_dict = jcoll_create_dict(tlb_cache_region, setst_get_stamp);
}

static void set_tlb_cache(setst_var v, jcoll j) { st_set_tlb_cache(v, j); }

static void collect_sinks(bounds b, setst_var v) {
  gen_e sink;
  bounds_scanner scan;

  bounds_scan(st_get_sinks(v), &scan);

  while (bounds_next(&scan, &sink)) {
    bounds_add(b, sink, setst_get_stamp(sink));
  }
}

static void collect_sources(bounds b, setst_var v) {
  gen_e source;
  bounds_scanner scan;

  bounds_scan(st_get_sources(v), &scan);

  while (bounds_next(&scan, &source)) {
    bounds_add(b, source, setst_get_stamp(source));
  }
}

static void collect_lower_bounds(bounds b, setst_var v) {
  gen_e lb;
  bounds_scanner scan;

  bounds_scan(st_get_lbs(v), &scan);

  while (bounds_next(&scan, &lb)) {
    bounds_add(b, lb, setst_get_stamp(lb));
  }
}

static void apply_sources(setst_var witness, bounds sources) {
  gen_e source;
  bounds_scanner scan;

  bounds_scan(sources, &scan);

  while (bounds_next(&scan, &source)) {
    if (st_add_source(witness, source, setst_get_stamp(source)))
      setst_stats.redundant_source++;

    else {
      setst_stats.added_source++;
      // setst_changed = TRUE;
    }
  }
}

static void apply_sinks(setst_var witness, bounds sinks) {
  gen_e sink;
  bounds_scanner scan;

  bounds_scan(sinks, &scan);

  while (bounds_next(&scan, &sink)) {
    if (st_add_sink(witness, sink, setst_get_stamp(sink)))
      setst_stats.redundant_sink++;

    else {
      // setst_changed = TRUE;
      setst_stats.added_sink++;
    }
  }
}

static void apply_lower_bounds(setst_var witness, bounds lower) {
  gen_e lb;
  bounds_scanner scan;

  bounds_scan(lower, &scan);

  while (bounds_next(&scan, &lb)) {
    if (st_add_lb(witness, (setst_var)lb))
      setst_stats.redundant_var++;
    else {
      // setst_changed = TRUE;
      setst_stats.added_var++;
    }
  }
}

static void collapse_cycle(setst_var witness, setst_var_list cycle) deletes {
  setst_var_list_scanner var_scan;
  setst_var temp;
  region scratch_rgn = newregion();

  bounds sources = bounds_create(scratch_rgn);
  bounds sinks = bounds_create(scratch_rgn);
  bounds lower = bounds_create(scratch_rgn);

  setst_stats.cycles_collapsed++;

  /* force at least another iteration */
  setst_changed = TRUE;

  /* collect all bounds */
  setst_var_list_scan(cycle, &var_scan);
  while (setst_var_list_next(&var_scan, &temp)) {
    collect_sources(sources, temp);
    collect_sinks(sinks, temp);
    collect_lower_bounds(lower, temp);
  }

  /* unify all vars */
  st_unify(witness, cycle);

  /* add all bounds back */
  apply_sources(witness, sources);
  apply_sinks(witness, sinks);
  apply_lower_bounds(witness, lower);

  /* cleanup */
  bounds_delete(sources);
  bounds_delete(sinks);
  bounds_delete(lower);
  deleteregion(scratch_rgn);

  /* remove self edges */
  st_repair_bounds(witness);
}

static bool cycle_detect(setst_var goal, setst_var_list path, setst_var_list *result) {
  int pos = st_get_path_pos(goal);
  setst_stats.cycles_searched++;

  if (pos) {
    setst_var_list_scanner scan;
    setst_var temp;
    setst_var_list cycle = new_setst_var_list(tlb_cache_region);

    setst_var_list_scan(path, &scan);
    while (setst_var_list_next(&scan, &temp)) {
      if (st_get_path_pos(temp) >= pos)
        setst_var_list_cons(temp, cycle);
    }

    *result = cycle;
    return TRUE;
  }

  else
    return FALSE;
}

/*
static bool cycle_detect(setst_var goal, setst_var_list path,
                         setst_var_list *result)
{
  setst_var_list cycle =
    setst_var_list_reverse(setst_var_list_copy(tlb_cache_region,path));

  setst_stats.cycles_searched++;

  while (!setst_var_list_empty(cycle) &&
         !eq((gen_e)setst_var_list_head(cycle),(gen_e)goal))
    {
      setst_var_list_tail(cycle);
    }

  if (setst_var_list_empty(cycle))
    {
      return FALSE;
    }
  else
    {
      *result = cycle;
      return TRUE;
    }
}
*/
static jcoll st_tlb_aux(gen_e e, int path_len, setst_var_list path) deletes {
  if (setst_is_var(e)) {
    setst_var_list cycle = NULL;
    setst_var v = (setst_var)e;
    if (flag_eliminate_cycles_st && cycle_detect(v, path, &cycle)) {
      setst_stats.cycles_length += setst_var_list_length(cycle);
      collapse_cycle(v, cycle);
      return NULL;
    } else {
      if (st_get_tlb_cache(v) != NULL)
        return st_get_tlb_cache(v);
      else {
        jcoll result;
        gen_e temp;
        bounds_scanner b_scan;
        gen_e lb;
        jcoll_list jvars = new_jcoll_list(tlb_cache_region);

        gen_e_list sources = new_gen_e_list(tlb_cache_region);

        bounds_scan(st_get_sources(v), &b_scan);

        while (bounds_next(&b_scan, &temp)) {
          gen_e_list_cons(temp, sources);
        }

        st_set_path_pos(v, path_len);

        bounds_scan(st_get_lbs(v), &b_scan);
        while (bounds_next(&b_scan, &lb)) {
          setst_var_list_cons(v, path);
          jcoll_list_cons(st_tlb_aux(lb, ++path_len, path), jvars);
          setst_var_list_tail(path);
        }

        if (!gen_e_list_empty(sources))
          jcoll_list_cons(jcoll_create_chain(tlb_dict, sources), jvars);
        result = jcoll_jjoin(tlb_dict, jvars);
        set_tlb_cache(v, result);
        st_set_path_pos(v, 0);
        return result;
      }
    }
  } else if (setst_is_union(e)) {
    gen_e_list_scanner scan;
    gen_e temp;
    jcoll_list jexprs = new_jcoll_list(tlb_cache_region);

    gen_e_list_scan(setst_get_union(e), &scan);
    while (gen_e_list_next(&scan, &temp)) {
      jcoll_list_cons(st_tlb_aux(temp, ++path_len, path), jexprs);
    }

    return jcoll_jjoin(tlb_dict, jexprs);
  } else {
    fail("Unmatched case in setst tlb computation\n");
    return NULL;
  }
}

static gen_e_list st_tlb(gen_e e) {
  return jcoll_flatten(tlb_dict, st_tlb_aux(e, 1, new_setst_var_list(tlb_cache_region)));
}

static void match_sinks(incl_fn_ptr setst_incl) {
  gen_e_list_scanner tlb_scanner;
  bounds_scanner sink_scanner;
  setst_var_list_scanner var_scanner;
  setst_var v;
  gen_e lb, sink;

  setst_var_list_scan(setst_vars, &var_scanner);

  while (setst_var_list_next(&var_scanner, &v)) {
    gen_e_list tlbs = st_tlb((gen_e)v);
    bounds snks = st_get_sinks(v);

    if (bounds_empty(snks)) {
      setst_stats.no_sinks++;
      continue;
    } else if (st_get_seen(v)) {
      setst_stats.incycle_vars++;
      continue;
    } else if (gen_e_list_length(tlbs) == st_get_src_sz(v) &&
               bounds_size(snks) == st_get_snk_sz(v)) {
      setst_stats.unchanged_vars++;
      continue;
    }

    st_set_seen(v, TRUE);

    st_set_src_sz(v, gen_e_list_length(tlbs));
    st_set_snk_sz(v, bounds_size(snks));

    gen_e_list_scan(tlbs, &tlb_scanner);

    while (gen_e_list_next(&tlb_scanner, &lb)) {
      bounds_scan(snks, &sink_scanner);

      while (bounds_next(&sink_scanner, &sink))
        setst_incl(lb, sink);
    }
  }
}

static void iterate(incl_fn_ptr setst_incl) {
  setst_var_list_scanner var_scanner;
  setst_var v;
  /* static int iterations = 0; */
  setst_changed = FALSE;

  setst_var_list_scan(setst_vars, &var_scanner);
  while (setst_var_list_next(&var_scanner, &v)) {
    st_set_seen(v, FALSE);
  }

  invalidate_tlb_cache();
  match_sinks(setst_incl);

  /*  fprintf(stderr,"Iterations : %d\n",++iterations); */

  if (setst_changed)
    iterate(setst_incl);
}

gen_e_list setst_tlb(gen_e e, incl_fn_ptr setst_incl) deletes {
  if (!setst_changed) {
    return st_tlb(e);
  } else {
    iterate(setst_incl);
    return st_tlb(e);
  }
}

void setst_set_proj_cache(gen_e e, gen_e elem) {
  if (setst_is_union(e)) {
    setst_union_ u = (setst_union_)e;
#ifdef BANSHEE_ROLLBACK
    setst_register_rollback();
    struct setst_cache_undo *undo = ralloc(setst_region, struct setst_cache_undo);
    undo->term = u;
    undo->previous = u->proj_cache;
    undo->next = setst_current_rollback->caches;
    setst_current_rollback->caches = undo;
    u->proj_cache = gen_e_list_copy(setst_region, u->proj_cache);
#endif
    gen_e_list_cons(elem, u->proj_cache);
  }
}

gen_e_list setst_get_proj_cache(gen_e e) {
  if (setst_is_union(e)) {
    setst_union_ u = (setst_union_)e;
    return u->proj_cache;
  } else {
    fail("Term does not cache projections\n");
    return NULL;
  }
}

void setst_init(void) {
  setst_current_rollback = NULL;
  setst_changed = FALSE;
  setst_region = newregion();
  tlb_cache_region = newregion();
  setst_hash = make_term_hash(NULL);
  setst_vars = new_setst_var_list(setst_region);
  tlb_dict = jcoll_create_dict(tlb_cache_region, setst_get_stamp);
}

void setst_reset(void) deletes {
  setst_current_rollback = NULL;
  term_hash_delete(setst_hash);
  deleteregion_ptr(&setst_region);

  setst_region = newregion();
  setst_hash = make_term_hash(NULL);
  setst_vars = new_setst_var_list(setst_region);
  invalidate_tlb_cache();
  setst_changed = FALSE;
}

bool setst_is_zero(gen_e e) { return ((setst_term)e)->type == ZERO_TYPE; }

bool setst_is_one(gen_e e) { return ((setst_term)e)->type == ONE_TYPE; }

bool setst_is_var(gen_e e) { return ((setst_term)e)->type == VAR_TYPE; }

bool setst_is_union(gen_e e) { return ((setst_term)e)->type == UNION_TYPE; }

bool setst_is_inter(gen_e e) { return ((setst_term)e)->type == INTER_TYPE; }

bool setst_is_constant(gen_e e) { return ((setst_term)e)->type == CONSTANT_TYPE; }

char *setst_get_constant_name(gen_e e) {
  assert(((setst_term)e)->type == CONSTANT_TYPE);

  return ((setst_constant_)e)->name;
}

void setst_print_stats(FILE *f) {
  fprintf(f, "\n========== SetST Var Stats ==========\n");
  fprintf(f, "(SetST) Fresh : %d\n", setst_stats.fresh);
  fprintf(f, "(SetST) Fresh Small : %d\n", setst_stats.fresh_small);
  fprintf(f, "(SetST) Fresh Large : %d\n", setst_stats.fresh_large);
  fprintf(f, "(SetST) Total Vars: %d\n",
          setst_stats.fresh + setst_stats.fresh_small + setst_stats.fresh_large);
  fprintf(f, "\n========== SetST Sort Stats ==========\n");
  fprintf(f, "\n");
  fprintf(f, "\n------------------------------\n");
  fprintf(f, "(SetST) Additions");
  fprintf(f, "\n------------------------------\n");
  fprintf(f, "(SetST) Var Added:   %d\n", setst_stats.added_var);
  fprintf(f, "(SetST) Source Added: %d\n", setst_stats.added_source);
  fprintf(f, "(SetST) Sink Added:   %d", setst_stats.added_sink);
  fprintf(f, "\n------------------------------\n");
  fprintf(f, "(SetST) Total Added:  %d",
          setst_stats.added_var + setst_stats.added_source + setst_stats.added_sink);
  fprintf(f, "\n");
  fprintf(f, "\n------------------------------\n");
  fprintf(f, "(SetST) Redundant");
  fprintf(f, "\n------------------------------\n");
  fprintf(f, "(SetST) Var Redundant:   %d\n", setst_stats.redundant_var);
  fprintf(f, "(SetST) Source Redundant: %d\n", setst_stats.redundant_source);
  fprintf(f, "(SetST) Sink Redundant:   %d", setst_stats.redundant_sink);
  fprintf(f, "\n------------------------------\n");
  fprintf(f, "(SetST) Total Redundant:  %d\n",
          setst_stats.redundant_var + setst_stats.redundant_source + setst_stats.redundant_sink);

  fprintf(f, "\n");
  fprintf(f, "\n------------------------------\n");
  fprintf(f, "(SetST) Iteration Optimizations");
  fprintf(f, "\n------------------------------\n");
  fprintf(f, "(SetST) Skipped vars:   %d\n", setst_stats.incycle_vars);
  fprintf(f, "(SetST) Unchanged vars: %d\n", setst_stats.unchanged_vars);
  fprintf(f, "(SetST) Vars w/o sinks: %d\n", setst_stats.no_sinks);
  fprintf(f, "\n------------------------------\n");
  fprintf(f, "(SetST) Cycles");
  fprintf(f, "\n------------------------------\n");
  fprintf(f, "(SetST) Collapsed:      %d\n", setst_stats.cycles_collapsed);
  fprintf(f, "(SetST) Searched:       %d\n", setst_stats.cycles_searched);
  fprintf(f, "(SetST) Hit rate:       %f\n",
          ((float)setst_stats.cycles_collapsed) / ((float)setst_stats.cycles_searched));
  fprintf(f, "(SetST) Average Length: %f\n",
          ((float)setst_stats.cycles_length) / ((float)setst_stats.cycles_collapsed));
  fprintf(f, "=====================================\n");
}

void setst_rollback(banshee_rollback_info info) {
  struct setst_rollback_info_ *saved = (struct setst_rollback_info_ *)info;
  assert(info->kind == setst_sort);
  /* engine rollback has already restored both UF links and bounds snapshots. */
  for (struct setst_cache_undo *u = saved->caches; u; u = u->next)
    u->term->proj_cache = u->previous;
  setst_current_rollback = saved->previous;
  invalidate_tlb_cache();
  setst_var_list_scanner scan;
  setst_var v;
  setst_var_list_scan(setst_vars, &scan);
  while (setst_var_list_next(&scan, &v)) {
    st_set_seen(v, FALSE);
    st_set_path_pos(v, 0);
    st_set_src_sz(v, -1);
    st_set_snk_sz(v, -1);
  }
  setst_changed = TRUE;
}

bool setst_rollback_serialize(FILE *f, banshee_rollback_info obj) {
  struct setst_rollback_info_ *i = (void *)obj;
  unsigned count = 0;
  for (struct setst_cache_undo *u=i->caches; u; u=u->next) ++count;
  fwrite_s(&i->previous, sizeof(i->previous), 1, f);
  serialize_banshee_object(i->previous, banshee_rollback_info);
  fwrite_s(&count, sizeof(count), 1, f);
  for (struct setst_cache_undo *u=i->caches; u; u=u->next) {
    fwrite_s(&u->term, sizeof(u->term), 1, f);
    fwrite_s(&u->previous, sizeof(u->previous), 1, f);
    serialize_banshee_object(u->term, gen_e);
    serialize_banshee_object(u->previous, list);
  }
  return TRUE;
}
banshee_rollback_info setst_rollback_deserialize(FILE *f) {
  struct setst_rollback_info_ *i = ralloc(setst_region, struct setst_rollback_info_);
  unsigned count;
  fread_s(&i->previous, sizeof(i->previous), 1, f);
  fread_s(&count, sizeof(count), 1, f);
  struct setst_cache_undo **tail=&i->caches;
  while (count--) {
    struct setst_cache_undo *u=ralloc(setst_region, struct setst_cache_undo);
    fread_s(&u->term, sizeof(u->term), 1, f);
    fread_s(&u->previous, sizeof(u->previous), 1, f);
    *tail=u; tail=&u->next;
  }
  return (banshee_rollback_info)i;
}
bool setst_rollback_set_fields(banshee_rollback_info obj) {
  struct setst_rollback_info_ *i=(void *)obj;
  deserialize_set_obj((void **)&i->previous);
  for (struct setst_cache_undo *u=i->caches; u; u=u->next) {
    deserialize_set_obj((void **)&u->term);
    deserialize_set_obj((void **)&u->previous);
  }
  return TRUE;
}
void setst_serialize(FILE *f) {
  fwrite_s(&setst_current_rollback, sizeof(setst_current_rollback), 1, f);
  fwrite_s(&setst_vars, sizeof(setst_vars), 1, f);
  fwrite_s(&setst_hash, sizeof(setst_hash), 1, f);
  serialize_banshee_object(setst_current_rollback, banshee_rollback_info);
  serialize_banshee_object(setst_vars, list);
  serialize_banshee_object(setst_hash, term_hash);
}
void setst_deserialize(FILE *f) {
  invalidate_tlb_cache();
  fread_s(&setst_current_rollback, sizeof(setst_current_rollback), 1, f);
  fread_s(&setst_vars, sizeof(setst_vars), 1, f);
  fread_s(&setst_hash, sizeof(setst_hash), 1, f);
  setst_changed=TRUE;
}
void setst_set_fields(void) {
  deserialize_set_obj((void **)&setst_current_rollback);
  deserialize_set_obj((void **)&setst_vars);
  deserialize_set_obj((void **)&setst_hash);
}
bool setst_expr_serialize(FILE *f, gen_e e) {
  int type=((setst_term)e)->type;
  fwrite_s(&((setst_term)e)->st, sizeof(stamp), 1, f);
  if (type==CONSTANT_TYPE) return string_data_serialize(f, ((setst_constant_)e)->name);
  gen_e_list exprs=((setst_inter_)e)->exprs;
  fwrite_s(&exprs,sizeof(exprs),1,f);
  serialize_banshee_object(exprs,list);
  if(type==UNION_TYPE) {
    gen_e_list cache=((setst_union_)e)->proj_cache;
    fwrite_s(&cache,sizeof(cache),1,f);
    serialize_banshee_object(cache,list);
  }
  return TRUE;
}
void *setst_expr_deserialize(FILE *f, int type) {
  size_t size=type==CONSTANT_TYPE ? sizeof(struct setst_constant_) :
              type==UNION_TYPE ? sizeof(struct setst_union_) : sizeof(struct setst_inter_);
  gen_e e=(gen_e)rstralloc(setst_region,size);
  ((setst_term)e)->type=type;
  fread_s(&((setst_term)e)->st,sizeof(stamp),1,f);
  if(type==CONSTANT_TYPE) {
    char *name=string_data_deserialize(f);
    ((setst_constant_)e)->name=rstrdup(setst_region,name); free(name);
  } else {
    fread_s(&((setst_inter_)e)->exprs,sizeof(gen_e_list),1,f);
    if(type==UNION_TYPE) fread_s(&((setst_union_)e)->proj_cache,sizeof(gen_e_list),1,f);
  }
  return e;
}
bool setst_expr_set_fields(gen_e e) {
  int type=((setst_term)e)->type;
  if(type==UNION_TYPE) deserialize_set_obj((void **)&((setst_union_)e)->proj_cache);
  if(type==UNION_TYPE || type==INTER_TYPE) deserialize_set_obj((void **)&((setst_inter_)e)->exprs);
  return TRUE;
}

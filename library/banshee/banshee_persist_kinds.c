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

#include "banshee_persist_kinds.h"

#include "hash.h"

bool nonptr_data_serialize(FILE *f, void *obj);
bool funptr_data_serialize(FILE *f, void *obj);
bool string_data_serialize(FILE *f, void *obj);
bool uf_element_serialize(FILE *f, void *obj);
bool setif_var_serialize(FILE *f, void *obj);
bool sv_info_serialize(FILE *f, void *obj);
bool list_serialize(FILE *f, void *obj);
bool bounds_serialize(FILE *f, void *obj);
bool contour_serialize(FILE *f, void *obj);
bool gen_e_serialize(FILE *f, void *obj);
bool banshee_rollback_info_serialize(FILE *f, void *obj);
bool flow_var_serialize(FILE *f, void *obj);
bool flowrow_field_serialize(FILE *f, void *obj);
bool cons_group_serialize(FILE *f, void *obj);
bool setst_var_serialize(FILE *f, void *obj);
bool st_info_serialize(FILE *f, void *obj);
bool term_var_serialize(FILE *f, void *obj);
bool ustack_elt_serialize(FILE *f, void *obj);
bool hash_table_serialize(FILE *f, void *obj);
bool constructor_serialize(FILE *f, void *obj);
bool term_hash_serialize(FILE *f, void *obj);
bool added_edge_info_serialize(FILE *f, void *obj);
bool added_ub_proj_info_serialize(FILE *f, void *obj);

void *nonptr_data_deserialize(FILE *f);
void *funptr_data_deserialize(FILE *f);
void *string_data_deserialize(FILE *f);
void *uf_element_deserialize(FILE *f);
void *setif_var_deserialize(FILE *f);
void *sv_info_deserialize(FILE *f);
void *list_deserialize(FILE *f);
void *bounds_deserialize(FILE *f);
void *contour_deserialize(FILE *f);
void *gen_e_deserialize(FILE *f);
void *banshee_rollback_info_deserialize(FILE *f);
void *flow_var_deserialize(FILE *f);
void *flowrow_field_deserialize(FILE *f);
void *cons_group_deserialize(FILE *f);
void *setst_var_deserialize(FILE *f);
void *st_info_deserialize(FILE *f);
void *term_var_deserialize(FILE *f);
void *ustack_elt_deserialize(FILE *f);
void *hash_table_deserialize(FILE *f);
void *constructor_deserialize(FILE *f);
void *term_hash_deserialize(FILE *f);
void *added_edge_info_deserialize(FILE *f);
void *added_ub_proj_info_deserialize(FILE *f);

bool nonptr_data_set_fields(void *obj);
bool funptr_data_set_fields(void *obj);
bool string_data_set_fields(void *obj);
bool uf_element_set_fields(void *obj);
bool setif_var_set_fields(void *obj);
bool sv_info_set_fields(void *obj);
bool list_set_fields(void *obj);
bool bounds_set_fields(void *obj);
bool contour_set_fields(void *obj);
bool gen_e_set_fields(void *obj);
bool banshee_rollback_info_set_fields(void *obj);
bool flow_var_set_fields(void *obj);
bool flowrow_field_set_fields(void *obj);
bool cons_group_set_fields(void *obj);
bool setst_var_set_fields(void *obj);
bool st_info_set_fields(void *obj);
bool term_var_set_fields(void *obj);
bool ustack_elt_set_fields(void *obj);
bool hash_table_set_fields(void *obj);
bool constructor_set_fields(void *obj);
bool term_hash_set_fields(void *obj);
bool added_edge_info_set_fields(void *obj);
bool added_ub_proj_info_set_fields(void *obj);

/* The specialized build has no sort tag in gen_e. Built-in expressions can
 * be identified by their owning regions. Client-defined constructors require
 * a client codec and are deliberately rejected rather than silently omitted.
 * This format is a same-build checkpoint, not an interchange format. */
#ifndef NONSPEC
#include "setif-sort.h"
#include "setst-sort.h"
#include "term-sort.h"
extern region term_var_region, term_constant_region;
extern region setif_var_region, setif_union_region, setif_inter_region;
extern region setif_constant_region, setif_term_region;
static int builtin_sort(void *obj) {
  if (obj == term_zero() || obj == term_one() || obj == term_wild()) return term_sort;
  if (obj == setif_zero() || obj == setif_one() || obj == setif_wild()) return setif_sort;
  if (obj == setst_zero() || obj == setst_one()) return setst_sort;
  region r = regionof(obj);
  if (r && (r == term_var_region || r == term_constant_region)) return term_sort;
  if (r && (r == setif_var_region || r == setif_union_region || r == setif_inter_region ||
            r == setif_constant_region || r == setif_term_region)) return setif_sort;
  if (r && r == setst_region) return setst_sort;
  return -1;
}
bool gen_e_serialize(FILE *f, void *obj) {
  int sort = builtin_sort(obj), type = *(int *)obj;
  if (sort < 0 || !(type == VAR_TYPE || type == ZERO_TYPE || type == ONE_TYPE ||
      type == CONSTANT_TYPE || (type == WILD_TYPE && sort != setst_sort) ||
      ((type == UNION_TYPE || type == INTER_TYPE) && sort != term_sort))) return FALSE;
  fwrite_s(&sort, sizeof(sort), 1, f);
  fwrite_s(&type, sizeof(type), 1, f);
  if (type == ZERO_TYPE || type == ONE_TYPE || type == WILD_TYPE) return TRUE;
  switch (sort) {
    case term_sort: return type == VAR_TYPE ? term_var_serialize(f,obj) : term_constant_serialize(f,obj);
    case setst_sort: return type == VAR_TYPE ? setst_var_serialize(f,obj) : setst_expr_serialize(f,obj);
    case setif_sort:
      switch(type) {
        case VAR_TYPE: return setif_var_serialize(f,obj);
        case UNION_TYPE: return setif_union_serialize(f,obj);
        case INTER_TYPE: return setif_inter_serialize(f,obj);
        default: return setif_constant_serialize(f,obj);
      }
  }
  return FALSE;
}
void *gen_e_deserialize(FILE *f) {
  int sort, type;
  fread_s(&sort,sizeof(sort),1,f); fread_s(&type,sizeof(type),1,f);
  switch(sort) {
    case term_sort:
      switch(type) {
        case ZERO_TYPE: return term_zero(); case ONE_TYPE: return term_one();
        case WILD_TYPE: return term_wild(); case VAR_TYPE: return term_var_deserialize(f);
        case CONSTANT_TYPE: return term_constant_deserialize(f); default: return NULL;
      }
    case setif_sort:
      switch(type) {
        case ZERO_TYPE: return setif_zero(); case ONE_TYPE: return setif_one();
        case WILD_TYPE: return setif_wild(); case VAR_TYPE: return setif_var_deserialize(f);
        case CONSTANT_TYPE: return setif_constant_deserialize(f);
        case UNION_TYPE: return setif_union_deserialize(f);
        case INTER_TYPE: return setif_inter_deserialize(f); default: return NULL;
      }
    case setst_sort:
      switch(type) {
        case ZERO_TYPE: return setst_zero(); case ONE_TYPE: return setst_one();
        case VAR_TYPE: return setst_var_deserialize(f);
        case CONSTANT_TYPE: case UNION_TYPE: case INTER_TYPE: return setst_expr_deserialize(f,type);
        default: return NULL;
      }
  }
  return NULL;
}
bool gen_e_set_fields(void *obj) {
  int sort = builtin_sort(obj), type = *(int *)obj;
  switch(sort) {
    case term_sort: return type == VAR_TYPE ? term_var_set_fields(obj) : TRUE;
    case setst_sort: return type == VAR_TYPE ? setst_var_set_fields(obj) : setst_expr_set_fields(obj);
    case setif_sort:
      switch(type) {
        case VAR_TYPE: return setif_var_set_fields(obj);
        case UNION_TYPE: return setif_union_set_fields(obj);
        case INTER_TYPE: return setif_inter_set_fields(obj); default: return TRUE;
      }
  }
  return FALSE;
}
void serialize_cs(const char *filename, hash_table *entry_points, unsigned long sz) { return; }
#endif /* NONSPEC */

static serialize_fn_ptr serialize_fns[NUM_PERSIST_KINDS] = {
    nonptr_data_serialize,
    funptr_data_serialize,
    string_data_serialize,
    uf_element_serialize,
    setif_var_serialize,
    sv_info_serialize,
    list_serialize,
    bounds_serialize,
    contour_serialize,
    banshee_rollback_info_serialize,
    flow_var_serialize,
    flowrow_field_serialize,
    setst_var_serialize,
    st_info_serialize,
    term_var_serialize,
    ustack_elt_serialize,
    hash_table_serialize,
    term_hash_serialize,
    gen_e_serialize,
    added_edge_info_serialize,
    added_ub_proj_info_serialize,
#ifdef NONSPEC
    cons_group_serialize,
    constructor_serialize,
#endif
};

/* Graph strings have graph lifetime, unlike inline string_data_deserialize
 * results whose ownership is transferred to their individual caller. */
static void *graph_string_deserialize(FILE *f) {
  char *text = string_data_deserialize(f);
  char *result = rstrdup(permanent, text);
  free(text);
  return result;
}

static deserialize_fn_ptr deserialize_fns[NUM_PERSIST_KINDS] = {
    nonptr_data_deserialize,
    funptr_data_deserialize,
    graph_string_deserialize,
    uf_element_deserialize,
    setif_var_deserialize,
    sv_info_deserialize,
    list_deserialize,
    bounds_deserialize,
    contour_deserialize,
    banshee_rollback_info_deserialize,
    flow_var_deserialize,
    flowrow_field_deserialize,
    setst_var_deserialize,
    st_info_deserialize,
    term_var_deserialize,
    ustack_elt_deserialize,
    hash_table_deserialize,
    term_hash_deserialize,
    gen_e_deserialize,
    added_edge_info_deserialize,
    added_ub_proj_info_deserialize,
#ifdef NONSPEC
    cons_group_deserialize,
    constructor_deserialize,
#endif
};

static set_fields_fn_ptr set_fields_fns[NUM_PERSIST_KINDS] = {
    nonptr_data_set_fields,
    funptr_data_set_fields,
    string_data_set_fields,
    uf_element_set_fields,
    setif_var_set_fields,
    sv_info_set_fields,
    list_set_fields,
    bounds_set_fields,
    contour_set_fields,
    banshee_rollback_info_set_fields,
    flow_var_set_fields,
    flowrow_field_set_fields,
    setst_var_set_fields,
    st_info_set_fields,
    term_var_set_fields,
    ustack_elt_set_fields,
    hash_table_set_fields,
    term_hash_set_fields,
    gen_e_set_fields,
    added_edge_info_set_fields,
    added_ub_proj_info_set_fields,
#ifdef NONSPEC
    cons_group_set_fields,
    constructor_set_fields,
#endif
};

void banshee_serialize_start(FILE *f) { serialize_start(f, serialize_fns, NUM_PERSIST_KINDS); }

void banshee_serialize_end() { serialize_end(); }
bool banshee_serialize_finish(void) { return serialize_finish(); }

void banshee_deserialize_all(FILE *f) {
  deserialize_all(f, deserialize_fns, set_fields_fns, NUM_PERSIST_KINDS);
}

void banshee_deserialize_end(void) { deserialize_end(); }

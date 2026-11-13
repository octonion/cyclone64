#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "banshee.h"
#include "banshee_persist_kinds.h"
#include "ufind.h"
#include "hash.h"
#include "term-sort.h"
#include "setif-sort.h"
#include "setst-sort.h"
static void match(gen_e a, gen_e b) {}
static bool occurs(term_var a, gen_e b) { return FALSE; }
static bool project(setif_var a, gen_e b) { return FALSE; }
static void print(FILE *f, gen_e e) {}
static void incl(gen_e a, gen_e b) { setst_inclusion(match,print,a,b); }
static int contains(gen_e_list l, gen_e x) {
  gen_e_list_scanner s; gen_e e; gen_e_list_scan(l,&s);
  while(gen_e_list_next(&s,&e)) if(e==x) return 1;
  return 0;
}
int main(void) {
  engine_init(); term_init(); setif_init(); setst_init();
  enum {TX,TY,TC,IX,IY,IC,SX,SY,SC,SU,SI,IU,II,N};
  gen_e root[N]={term_fresh("tx"),term_fresh("ty"),term_constant("tc"),
    setif_fresh("ix"),setif_fresh("iy"),setif_constant("ic"),
    setst_fresh("sx"),setst_fresh("sy"),setst_constant("sc")};
  region r=newregion(); gen_e_list list=new_gen_e_list(r);
  gen_e_list_cons(root[SX],list); gen_e_list_cons(root[SY],list);
  root[SU]=setst_union(list); root[SI]=setst_inter(list);
  list=new_gen_e_list(r); gen_e_list_cons(root[IX],list); gen_e_list_cons(root[IY],list);
  root[IU]=setif_union(list); root[II]=setif_inter(list);
  hash_table pointers=make_persistent_hash_table(8,ptr_hash,ptr_eq,
      BANSHEE_PERSIST_KIND_gen_e,BANSHEE_PERSIST_KIND_gen_e);
  hash_table_insert(pointers,root[TX],root[SC]);
  hash_table_insert(pointers,root[SX],root[IC]);
  banshee_clock_tick();
  term_unify(match,occurs,root[TX],root[TY]); term_unify(match,occurs,root[TY],root[TC]);
  setif_inclusion(match,project,print,root[IC],root[IX]);
  setif_inclusion(match,project,print,root[IX],root[IY]);
  incl(root[SC],root[SX]); incl(root[SX],root[SY]);
  banshee_clock_tick(); incl(root[SY],root[SX]);
  assert(contains(setst_tlb(root[SY],incl),root[SC])); assert(setst_eq(root[SX],root[SY]));
  setst_set_proj_cache(root[SU],root[SC]);
  for(int pass=0;pass<2;++pass) {
    FILE *meta=tmpfile(), *graph=tmpfile(); assert(meta&&graph);
    banshee_serialize_start(graph);
    engine_serialize(meta); uf_serialize(meta); term_serialize(meta);
    setif_serialize(meta); setst_serialize(meta); stamp_serialize(meta);
    for(int i=0;i<N;++i) assert(serialize_banshee_object(root[i],gen_e));
    serialize_banshee_object(pointers,hash_table);
    assert(banshee_serialize_finish()); rewind(meta); rewind(graph);
    /* Discard original expression regions before reading their replacements. */
    if (pass == 1) { term_reset(); setif_reset(); setst_reset(); engine_reset(); }
    engine_deserialize(meta); uf_deserialize(meta); term_deserialize(meta);
    setif_deserialize(meta); setst_deserialize(meta); stamp_deserialize(meta);
    banshee_deserialize_all(graph);
    engine_set_fields(); uf_set_fields(); term_set_fields();
    setif_set_fields(); setst_set_fields(); stamp_set_fields();
    for(int i=0;i<N;++i) { gen_e old=root[i]; root[i]=deserialize_get_obj(old); assert(root[i]); if (!pass) assert(root[i]!=old); }
    pointers=deserialize_get_obj(pointers); assert(pointers);
    gen_e value=NULL;
    assert(hash_table_lookup(pointers,root[TX],(void **)&value)&&value==root[SC]);
    assert(hash_table_lookup(pointers,root[SX],(void **)&value)&&value==root[IC]);
    banshee_deserialize_end(); fclose(meta); fclose(graph);
    assert(term_eq(root[TX],root[TC])); assert(!strcmp(term_get_constant_name(root[TX]),"tc"));
    assert(contains(setif_tlb(root[IY]),root[IC]));
    assert(contains(setst_tlb(root[SY],incl),root[SC])); assert(setst_eq(root[SX],root[SY]));
    assert(term_constant("tc") == root[TC]);
    assert(setif_constant("ic") == root[IC]);
    assert(setst_constant("sc") == root[SC]);
    assert(gen_e_list_length(setst_get_proj_cache(root[SU]))==1);
    assert(setst_is_inter(root[SI])&&setif_is_inter(root[II]));
  }
  banshee_rollback(); assert(!setst_eq(root[SX],root[SY]));
  assert(gen_e_list_empty(setst_get_proj_cache(root[SU])));
  assert(contains(setst_tlb(root[SY],incl),root[SC]));
  banshee_rollback(); assert(term_is_var(root[TX])); assert(!term_eq(root[TX],root[TY]));
  assert(!contains(setif_tlb(root[IY]),root[IC])); assert(!contains(setst_tlb(root[SY],incl),root[SC]));
  banshee_clock_tick(); incl(root[SC],root[SI]);
  assert(contains(setst_tlb(root[SY],incl),root[SC]));
  banshee_rollback(); assert(!contains(setst_tlb(root[SY],incl),root[SC]));
  /* The checked API must reject an unsupported client expression. */
  struct { int type; } custom={LARGEST_BUILTIN_TYPE};
  FILE *bad=tmpfile(); assert(bad); banshee_serialize_start(bad);
  serialize_banshee_object(&custom,gen_e); assert(!banshee_serialize_finish()); fclose(bad);
  puts("solver persistence: repeated graph relocation, all three sorts, rollback and rejection: PASS");
}

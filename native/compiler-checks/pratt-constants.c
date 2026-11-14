/* Exercise the current source-generated prover, not a duplicated evaluator. */
#include PRATT_GENERATED
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#define CHECK(e) do { if (!(e)) { fprintf(stderr,"FAIL %d: %s\n",__LINE__,#e); _exit(1); } } while (0)
static void check_fold(enum Cyc_Absyn_Primop op,unsigned a,unsigned b,int defined,unsigned value) {
  struct Cyc_AssnDef_Binop_AssnDef_Term_struct *t=_cycalloc(sizeof *t);
  *t=(struct Cyc_AssnDef_Binop_AssnDef_Term_struct){4,op,Cyc_AssnDef_uint(a),Cyc_AssnDef_uint(b),NULL};
  __auto_type got=Cyc_PrattProver_subst_term_with_const(Cyc_PrattProver_true_graph(),t);
  CHECK((!!(uintptr_t)got.f0)==defined);
  if (defined) CHECK((unsigned)(uintptr_t)got.f1==value);
}
static void check_dist(int a,int b,int defined,unsigned value) {
  struct Cyc_PrattProver_Node n={0},z={0};
  struct Cyc_PrattProver_Distance nz={NULL,&z,a},zn={NULL,&n,b};
  for (int sign=0;sign<2;++sign) {
    if (sign) { n.signed_distances=&nz; z.signed_distances=&zn; }
    else { n.unsigned_distances=&nz; z.unsigned_distances=&zn; }
    __auto_type got=Cyc_PrattProver_eq_node_const(&n,&z,sign);
    CHECK((!!(uintptr_t)got.f0)==defined);
    if (defined) CHECK((unsigned)(uintptr_t)got.f1==value);
  }
}
int Cyc_main(int argc,struct _fat_ptr argv) {
  check_fold(Cyc_Absyn_Bitlshift,1,31,1,0x80000000U);
  check_fold(Cyc_Absyn_Bitlrshift,0x80000000U,31,1,1);
  check_fold(Cyc_Absyn_Div,(unsigned)-42,7,1,(unsigned)-6);
  check_fold(Cyc_Absyn_Mod,(unsigned)-43,7,1,(unsigned)-1);
  check_fold(Cyc_Absyn_Plus,UINT_MAX,1,1,0);
  unsigned shifts[]={32,63,UINT_MAX};
  for (unsigned i=0;i<3;++i) {
    check_fold(Cyc_Absyn_Bitlshift,1,shifts[i],0,0);
    check_fold(Cyc_Absyn_Bitlrshift,1,shifts[i],0,0);
  }
  enum Cyc_Absyn_Primop divs[]={Cyc_Absyn_Div,Cyc_Absyn_Mod,Cyc_Absyn_UDiv,Cyc_Absyn_UMod};
  for (unsigned i=0;i<4;++i) check_fold(divs[i],1,0,0,0);
  check_fold(Cyc_Absyn_Div,0x80000000U,UINT_MAX,0,0);
  check_fold(Cyc_Absyn_Mod,0x80000000U,UINT_MAX,0,0);
  check_dist(17,-17,1,17);
  check_dist(-17,17,1,(unsigned)-17);
  check_dist(INT_MAX,-INT_MAX,1,INT_MAX);
  check_dist(INT_MIN,INT_MIN,0,0);
  check_dist(0,INT_MIN,0,0);
  puts("Pratt constant substitution: undefined arithmetic stays symbolic; distance boundaries: PASS");
  return 0;
}

/* Exercise the actual generated proof-term folder, including boundaries that
 * must remain symbolic rather than executing undefined C arithmetic. */
#include "../../stage0/library/compiler/assndef.c"
#include <assert.h>
#include <stdio.h>
static void symbolic(enum Cyc_Absyn_Primop op, unsigned a, unsigned b) {
  void *result=Cyc_AssnDef_binop(op,Cyc_AssnDef_uint(a),Cyc_AssnDef_uint(b),NULL);
  assert(*(int *)result==4);
  struct Cyc_AssnDef_Binop_AssnDef_Term_struct *expr=result;
  assert(expr->f1==op);
}
static void folded(enum Cyc_Absyn_Primop op,unsigned a,unsigned b,unsigned expected) {
  void *result=Cyc_AssnDef_binop(op,Cyc_AssnDef_uint(a),Cyc_AssnDef_uint(b),NULL);
  assert(*(int *)result==0);
  assert(((struct Cyc_AssnDef_Uint_AssnDef_Term_struct *)result)->f1==expected);
}
int Cyc_main(int argc,struct _fat_ptr argv) {
  folded(Cyc_Absyn_Bitlshift,1,31,0x80000000U);
  folded(Cyc_Absyn_Bitlrshift,0x80000000U,31,1);
  folded(Cyc_Absyn_Div,(unsigned)-42,7,(unsigned)-6);
  folded(Cyc_Absyn_Mod,(unsigned)-43,7,(unsigned)-1);
  symbolic(Cyc_Absyn_Bitlshift,1,32);
  symbolic(Cyc_Absyn_Bitlrshift,1,32);
  symbolic(Cyc_Absyn_Bitlshift,1,63);
  symbolic(Cyc_Absyn_Div,0x80000000U,0xffffffffU);
  symbolic(Cyc_Absyn_Mod,0x80000000U,0xffffffffU);
  symbolic(Cyc_Absyn_Div,1,0);
  puts("constant folding: shift and signed division boundaries: PASS");
  return 0;
}

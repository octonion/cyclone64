#include "cyc_include.h"
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
extern int Cyc_Absyn_cmp_ptr(void *,void *);
extern int Cyc_Core_nptrcmp(void *,void *);
extern int Cyc_Core_intcmp(int,int);
extern int Cyc_CfFlowInfo_root_cmp(void *,void *);
struct root { int tag; void *value; void *type; };
#define CHECK(e) do { if(!(e)) { fprintf(stderr,"FAIL %d: %s\n",__LINE__,#e); _exit(1); } } while(0)
int Cyc_main(int argc,struct _fat_ptr argv) {
  // Synthetic canonical addresses are only compared, never dereferenced.
  void *a=(void *)(uintptr_t)0x100000010UL;
  void *b=(void *)(uintptr_t)0x200000010UL;
  CHECK(Cyc_Absyn_cmp_ptr(a,b)<0);
  CHECK(Cyc_Absyn_cmp_ptr(b,a)>0);
  CHECK(Cyc_Absyn_cmp_ptr(a,a)==0);
  CHECK(Cyc_Core_nptrcmp(a,b)<0);
  CHECK(Cyc_Core_intcmp(INT_MIN,INT_MAX)<0);
  CHECK(Cyc_Core_intcmp(INT_MAX,INT_MIN)>0);
  for(int kind=0;kind<2;++kind) {
    struct root x={kind,a,NULL},y={kind,b,NULL};
    CHECK(Cyc_CfFlowInfo_root_cmp(&x,&y)<0);
    CHECK(Cyc_CfFlowInfo_root_cmp(&y,&x)>0);
    x.value=(void *)(uintptr_t)0x17fffffffUL;
    y.value=(void *)(uintptr_t)0x180000000UL;
    CHECK(Cyc_CfFlowInfo_root_cmp(&x,&y)<0);
  }
  puts("pointer ordering: high-word identity, antisymmetry, signed boundaries: PASS");
  return 0;
}

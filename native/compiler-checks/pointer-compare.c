#include "cyc_include.h"
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#ifdef CYC_NATIVE_LAYOUTS
extern int Cyc_Absyn_cmp_ptr(unsigned long,unsigned long,void *,void *);
#define Cyc_Absyn_cmp_ptr(a,b) (Cyc_Absyn_cmp_ptr)(sizeof(void *),sizeof(void *),(a),(b))
#else
extern int Cyc_Absyn_cmp_ptr(void *,void *);
#endif
extern int Cyc_AssnDef_termcmp(void *,void *);
struct logic_term { int tag; void *vd; int id; void *type; };
struct uint_term { int tag; unsigned value; void *type; };
struct addr_term { int tag; void *vd; void *invocation; void *type; };
struct alloc_term { int tag; void *expression; int id; void *nelts,*invocation,*type; };
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
  struct logic_term x={2,a,0,NULL},y={2,b,0,NULL};
  CHECK(Cyc_AssnDef_termcmp(&x,&y)<0);
  CHECK(Cyc_AssnDef_termcmp(&y,&x)>0);
  struct uint_term zero={0,0,NULL};
  struct addr_term ax={11,a,&zero,NULL},ay={11,b,&zero,NULL};
  CHECK(Cyc_AssnDef_termcmp(&ax,&ay)<0);
  CHECK(Cyc_AssnDef_termcmp(&ay,&ax)>0);
  struct alloc_term nx={12,a,0,&zero,&zero,NULL},ny={12,b,0,&zero,&zero,NULL};
  CHECK(Cyc_AssnDef_termcmp(&nx,&ny)<0);
  CHECK(Cyc_AssnDef_termcmp(&ny,&nx)>0);
  unsigned values[]={0,1,INT_MAX,(unsigned)INT_MAX+1U,UINT_MAX};
  for(unsigned i=0;i<5;++i) for(unsigned j=0;j<5;++j) {
    struct uint_term u={0,values[i],NULL},v={0,values[j],NULL};
    int c=Cyc_AssnDef_termcmp(&u,&v);
    CHECK(i==j ? c==0 : i<j ? c<0 : c>0);
  }
  x.vd=y.vd=NULL; x.id=INT_MIN; y.id=INT_MAX;
  CHECK(Cyc_AssnDef_termcmp(&x,&y)<0);
  CHECK(Cyc_AssnDef_termcmp(&y,&x)>0);
  puts("pointer ordering: high-word identity, antisymmetry, signed boundaries: PASS");
  return 0;
}

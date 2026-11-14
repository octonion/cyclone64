#include <cyc_include.h>
 struct Cyc_Core_Opt{void*v;};
# 121 "native/include/cyclone/library/std/core.h"
int Cyc_Core_ptrcmp(void*,void*);struct Cyc_List_List{void*hd;struct Cyc_List_List*tl;};
# 173 "native/include/cyclone/library/std/list.h"
extern struct Cyc_List_List*Cyc_List_rev(struct Cyc_List_List*);struct Cyc_AssnDef_ExistAssnFn;struct _union_Nmspace_Abs_n{int tag;struct Cyc_List_List*val;};struct _union_Nmspace_Rel_n{int tag;struct Cyc_List_List*val;};struct _union_Nmspace_C_n{int tag;struct Cyc_List_List*val;};struct _union_Nmspace_Loc_n{int tag;int val;};union Cyc_Absyn_Nmspace{struct _union_Nmspace_Abs_n Abs_n;struct _union_Nmspace_Rel_n Rel_n;struct _union_Nmspace_C_n C_n;struct _union_Nmspace_Loc_n Loc_n;};struct _tuple0{union Cyc_Absyn_Nmspace f0;struct _fat_ptr*f1;};
# 148 "library/compiler/absyn.h"
enum Cyc_Absyn_Scope{Cyc_Absyn_Static =0U,Cyc_Absyn_Abstract =1U,Cyc_Absyn_Public =2U,Cyc_Absyn_Extern =3U,Cyc_Absyn_ExternC =4U,Cyc_Absyn_Register =5U,Cyc_Absyn_Noreturn =6U};struct Cyc_Absyn_Tqual{long print_const: 1;long q_volatile: 1;long q_restrict: 1;long real_const: 1;unsigned loc;};
# 170
enum Cyc_Absyn_Size_of{Cyc_Absyn_Char_sz =0U,Cyc_Absyn_Short_sz =1U,Cyc_Absyn_Int_sz =2U,Cyc_Absyn_Long_sz =3U,Cyc_Absyn_LongLong_sz =4U,Cyc_Absyn_Int128_sz =5U};
# 180
enum Cyc_Absyn_Sign{Cyc_Absyn_Signed =0U,Cyc_Absyn_Unsigned =1U,Cyc_Absyn_None =2U};
# 186
enum Cyc_Absyn_AggrKind{Cyc_Absyn_StructA =0U,Cyc_Absyn_UnionA =1U};struct Cyc_Absyn_Tvar{struct _fat_ptr*name;int identity;void*kind;void*aquals_bound;};struct Cyc_Absyn_PtrLoc{unsigned ptr_loc;unsigned rgn_loc;unsigned zt_loc;};struct Cyc_Absyn_PtrAtts{void*eff;void*nullable;void*bounds;void*zero_term;struct Cyc_Absyn_PtrLoc*ptrloc;void*autoreleased;void*aqual;};struct Cyc_Absyn_PtrInfo{void*elt_type;struct Cyc_Absyn_Tqual elt_tq;struct Cyc_Absyn_PtrAtts ptr_atts;};struct Cyc_Absyn_VarargInfo{struct _fat_ptr*name;struct Cyc_Absyn_Tqual tq;void*type;long inject;};struct Cyc_Absyn_FnInfo{struct Cyc_List_List*tvars;void*effect;struct Cyc_Absyn_Tqual ret_tqual;void*ret_type;struct Cyc_List_List*args;long c_varargs;struct Cyc_Absyn_VarargInfo*cyc_varargs;struct Cyc_List_List*qual_bnd;struct Cyc_List_List*attributes;struct Cyc_Absyn_Exp*checks_clause;struct Cyc_AssnDef_ExistAssnFn*checks_assn;struct Cyc_Absyn_Exp*requires_clause;struct Cyc_AssnDef_ExistAssnFn*requires_assn;struct Cyc_Absyn_Exp*ensures_clause;struct Cyc_AssnDef_ExistAssnFn*ensures_assn;struct Cyc_Absyn_Exp*throws_clause;struct Cyc_AssnDef_ExistAssnFn*throws_assn;struct Cyc_Absyn_Vardecl*return_value;struct Cyc_List_List*arg_vardecls;struct Cyc_List_List*effconstr;};struct Cyc_Absyn_IntCon_Absyn_TyCon_struct{int tag;enum Cyc_Absyn_Sign f1;enum Cyc_Absyn_Size_of f2;};struct Cyc_Absyn_AppType_Absyn_Type_struct{int tag;void*f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_PointerType_Absyn_Type_struct{int tag;struct Cyc_Absyn_PtrInfo f1;};
# 600 "library/compiler/absyn.h"
enum Cyc_Absyn_Primop{Cyc_Absyn_Plus =0U,Cyc_Absyn_Times =1U,Cyc_Absyn_Minus =2U,Cyc_Absyn_Div =3U,Cyc_Absyn_Mod =4U,Cyc_Absyn_Eq =5U,Cyc_Absyn_Neq =6U,Cyc_Absyn_Gt =7U,Cyc_Absyn_Lt =8U,Cyc_Absyn_Gte =9U,Cyc_Absyn_Lte =10U,Cyc_Absyn_Not =11U,Cyc_Absyn_Bitnot =12U,Cyc_Absyn_Bitand =13U,Cyc_Absyn_Bitor =14U,Cyc_Absyn_Bitxor =15U,Cyc_Absyn_Bitlshift =16U,Cyc_Absyn_Bitlrshift =17U,Cyc_Absyn_Numelts =18U,Cyc_Absyn_Tagof =19U,Cyc_Absyn_UDiv =20U,Cyc_Absyn_UMod =21U,Cyc_Absyn_UGt =22U,Cyc_Absyn_ULt =23U,Cyc_Absyn_UGte =24U,Cyc_Absyn_ULte =25U};struct Cyc_Absyn_PrimopOpt{enum Cyc_Absyn_Primop v;};
# 635
enum Cyc_Absyn_Incrementor{Cyc_Absyn_PreInc =0U,Cyc_Absyn_PostInc =1U,Cyc_Absyn_PreDec =2U,Cyc_Absyn_PostDec =3U};struct Cyc_Absyn_VarargCallInfo{int num_varargs;struct Cyc_List_List*injectors;struct Cyc_Absyn_VarargInfo*vai;};
# 660
enum Cyc_Absyn_Coercion{Cyc_Absyn_Unknown_coercion =0U,Cyc_Absyn_No_coercion =1U,Cyc_Absyn_Null_to_NonNull =2U,Cyc_Absyn_Subset_coercion =3U,Cyc_Absyn_Other_coercion =4U};
# 675
enum Cyc_Absyn_MallocKind{Cyc_Absyn_Malloc =0U,Cyc_Absyn_Calloc =1U,Cyc_Absyn_Vmalloc =2U};struct Cyc_Absyn_MallocInfo{enum Cyc_Absyn_MallocKind mknd;struct Cyc_Absyn_Exp*rgn;struct Cyc_Absyn_Exp*aqual;void**elt_type;struct Cyc_Absyn_Exp*num_elts;long fat_result;long inline_call;};struct Cyc_Absyn_Var_e_Absyn_Raw_exp_struct{int tag;void*f1;};struct Cyc_Absyn_Primop_e_Absyn_Raw_exp_struct{int tag;enum Cyc_Absyn_Primop f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_AssignOp_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_PrimopOpt*f2;struct Cyc_Absyn_Exp*f3;};struct Cyc_Absyn_Increment_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;enum Cyc_Absyn_Incrementor f2;};struct Cyc_Absyn_Conditional_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Exp*f2;struct Cyc_Absyn_Exp*f3;};struct Cyc_Absyn_And_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Exp*f2;};struct Cyc_Absyn_Or_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Exp*f2;};struct Cyc_Absyn_SeqExp_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Exp*f2;};struct Cyc_Absyn_FnCall_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_List_List*f2;struct Cyc_Absyn_VarargCallInfo*f3;long f4;};struct Cyc_Absyn_Throw_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;long f2;};struct Cyc_Absyn_NoInstantiate_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_Instantiate_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_Cast_e_Absyn_Raw_exp_struct{int tag;void*f1;struct Cyc_Absyn_Exp*f2;long f3;enum Cyc_Absyn_Coercion f4;};struct Cyc_Absyn_Address_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_New_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Exp*f2;struct Cyc_Absyn_Exp*f3;};struct Cyc_Absyn_Sizeofexp_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_Deref_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_AggrMember_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct _fat_ptr*f2;long f3;long f4;};struct Cyc_Absyn_AggrArrow_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct _fat_ptr*f2;long f3;long f4;};struct Cyc_Absyn_Subscript_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Exp*f2;};struct Cyc_Absyn_Array_e_Absyn_Raw_exp_struct{int tag;struct Cyc_List_List*f1;};struct Cyc_Absyn_Comprehension_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Vardecl*f1;struct Cyc_Absyn_Exp*f2;struct Cyc_Absyn_Exp*f3;long f4;};struct Cyc_Absyn_ComprehensionNoinit_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;void*f2;long f3;};struct Cyc_Absyn_Aggregate_e_Absyn_Raw_exp_struct{int tag;struct _tuple0*f1;struct Cyc_List_List*f2;struct Cyc_List_List*f3;struct Cyc_Absyn_Aggrdecl*f4;};struct Cyc_Absyn_AnonStruct_e_Absyn_Raw_exp_struct{int tag;void*f1;long f2;struct Cyc_List_List*f3;};struct Cyc_Absyn_Datatype_e_Absyn_Raw_exp_struct{int tag;struct Cyc_List_List*f1;struct Cyc_Absyn_Datatypedecl*f2;struct Cyc_Absyn_Datatypefield*f3;};struct Cyc_Absyn_Malloc_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_MallocInfo f1;};struct Cyc_Absyn_Swap_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Exp*f2;};struct Cyc_Absyn_StmtExp_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Stmt*f1;};struct Cyc_Absyn_Tagcheck_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct _fat_ptr*f2;};struct Cyc_Absyn_Extension_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_Assert_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;long f2;long f3;};struct Cyc_Absyn_Assert_false_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_Exp{void*topt;void*r;unsigned loc;void*annot;};struct Cyc_Absyn_Exp_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_Seq_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Stmt*f1;struct Cyc_Absyn_Stmt*f2;};struct Cyc_Absyn_Return_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_IfThenElse_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Stmt*f2;struct Cyc_Absyn_Stmt*f3;};struct _tuple9{struct Cyc_Absyn_Exp*f0;struct Cyc_Absyn_Stmt*f1;};struct Cyc_Absyn_While_s_Absyn_Raw_stmt_struct{int tag;struct _tuple9 f1;struct Cyc_Absyn_Stmt*f2;};struct Cyc_Absyn_For_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Exp*f1;struct _tuple9 f2;struct _tuple9 f3;struct Cyc_Absyn_Stmt*f4;};struct Cyc_Absyn_Switch_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_List_List*f2;void*f3;};struct Cyc_Absyn_Fallthru_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_List_List*f1;struct Cyc_Absyn_Switch_clause**f2;};struct Cyc_Absyn_Decl_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Decl*f1;struct Cyc_Absyn_Stmt*f2;};struct Cyc_Absyn_Label_s_Absyn_Raw_stmt_struct{int tag;struct _fat_ptr*f1;struct Cyc_Absyn_Stmt*f2;};struct Cyc_Absyn_Do_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Stmt*f1;struct _tuple9 f2;};struct Cyc_Absyn_TryCatch_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Stmt*f1;struct Cyc_List_List*f2;void*f3;};struct Cyc_Absyn_Stmt{void*r;unsigned loc;void*annot;};struct Cyc_Absyn_Pat{void*r;void*topt;unsigned loc;};struct Cyc_Absyn_Switch_clause{struct Cyc_Absyn_Pat*pattern;struct Cyc_Core_Opt*pat_vars;struct Cyc_Absyn_Exp*where_clause;struct Cyc_Absyn_Stmt*body;unsigned loc;};struct Cyc_Absyn_Funname_b_Absyn_Binding_struct{int tag;struct Cyc_Absyn_Fndecl*f1;};struct Cyc_Absyn_Vardecl{enum Cyc_Absyn_Scope sc;struct _tuple0*name;unsigned varloc;struct Cyc_Absyn_Tqual tq;void*type;struct Cyc_Absyn_Exp*initializer;void*rgn;struct Cyc_List_List*attributes;long escapes;long is_proto;struct Cyc_Absyn_Exp*rename;};struct Cyc_Absyn_Fndecl{enum Cyc_Absyn_Scope sc;long is_inline;struct _tuple0*name;struct Cyc_Absyn_Stmt*body;struct Cyc_Absyn_FnInfo i;void*cached_type;struct Cyc_Core_Opt*param_vardecls;struct Cyc_Absyn_Vardecl*fn_vardecl;enum Cyc_Absyn_Scope orig_scope;long escapes;};struct Cyc_Absyn_AggrdeclImpl{struct Cyc_List_List*exist_vars;struct Cyc_List_List*qual_bnd;struct Cyc_List_List*fields;long tagged;struct Cyc_List_List*effconstr;};struct Cyc_Absyn_Aggrdecl{enum Cyc_Absyn_AggrKind kind;enum Cyc_Absyn_Scope sc;struct _tuple0*name;struct Cyc_List_List*tvs;struct Cyc_Absyn_AggrdeclImpl*impl;struct Cyc_List_List*attributes;long expected_mem_kind;};struct Cyc_Absyn_Datatypefield{struct _tuple0*name;struct Cyc_List_List*typs;unsigned loc;enum Cyc_Absyn_Scope sc;};struct Cyc_Absyn_Datatypedecl{enum Cyc_Absyn_Scope sc;struct _tuple0*name;struct Cyc_List_List*tvs;struct Cyc_Core_Opt*fields;long is_extensible;};struct Cyc_Absyn_Var_d_Absyn_Raw_decl_struct{int tag;struct Cyc_Absyn_Vardecl*f1;};struct Cyc_Absyn_Fn_d_Absyn_Raw_decl_struct{int tag;struct Cyc_Absyn_Fndecl*f1;};struct Cyc_Absyn_Let_d_Absyn_Raw_decl_struct{int tag;struct Cyc_Absyn_Pat*f1;struct Cyc_Core_Opt*f2;struct Cyc_Absyn_Exp*f3;void*f4;};struct Cyc_Absyn_Region_d_Absyn_Raw_decl_struct{int tag;struct Cyc_Absyn_Tvar*f1;struct Cyc_Absyn_Vardecl*f2;struct Cyc_Absyn_Exp*f3;};struct Cyc_Absyn_Namespace_d_Absyn_Raw_decl_struct{int tag;struct _fat_ptr*f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_Using_d_Absyn_Raw_decl_struct{int tag;struct _tuple0*f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_ExternC_d_Absyn_Raw_decl_struct{int tag;struct Cyc_List_List*f1;};struct Cyc_Absyn_Decl{void*r;unsigned loc;};extern char Cyc_Absyn_EmptyAnnot[11U];struct Cyc_Absyn_EmptyAnnot_Absyn_AbsynAnnot_struct{char*tag;};
# 1049 "library/compiler/absyn.h"
int Cyc_Absyn_qvar_cmp(struct _tuple0*,struct _tuple0*);
int Cyc_Absyn_hash_qvar(struct _tuple0*);
# 1070
void*Cyc_Absyn_compress(void*);
# 1133
void*Cyc_Absyn_bounds_one (void);
# 1192
struct Cyc_Absyn_Exp*Cyc_Absyn_uint_exp(unsigned,unsigned);
# 1359
void Cyc_Absyn_visit_stmt(long(*)(void*,struct Cyc_Absyn_Exp*),long(*)(void*,struct Cyc_Absyn_Stmt*),void*,struct Cyc_Absyn_Stmt*);
void Cyc_Absyn_visit_exp(long(*)(void*,struct Cyc_Absyn_Exp*),long(*)(void*,struct Cyc_Absyn_Stmt*),void*,struct Cyc_Absyn_Exp*);struct Cyc_Hashtable_Table;
# 42 "native/include/cyclone/library/std/hashtable.h"
extern struct Cyc_Hashtable_Table*Cyc_Hashtable_create(int,int(*)(void*,void*),int(*)(void*));
# 53
extern void Cyc_Hashtable_insert(struct Cyc_Hashtable_Table*,void*,void*);
# 59
extern void**Cyc_Hashtable_lookup_opt(struct Cyc_Hashtable_Table*,void*);struct Cyc_JumpAnalysis_Jump_Anal_Result{struct Cyc_Hashtable_Table*pop_tables;struct Cyc_Hashtable_Table*succ_tables;struct Cyc_Hashtable_Table*pat_pop_tables;};extern char Cyc_InsertChecks_FatBound[9U];struct Cyc_InsertChecks_FatBound_Absyn_AbsynAnnot_struct{char*tag;};extern char Cyc_InsertChecks_NoCheck[8U];struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct{char*tag;};extern char Cyc_InsertChecks_NullAndFatBound[16U];struct Cyc_InsertChecks_NullAndFatBound_Absyn_AbsynAnnot_struct{char*tag;};extern char Cyc_InsertChecks_NullAndThinBound[17U];struct Cyc_InsertChecks_NullAndThinBound_Absyn_AbsynAnnot_struct{char*tag;struct Cyc_Absyn_Exp*f1;};extern char Cyc_InsertChecks_NullOnly[9U];struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct{char*tag;};extern char Cyc_InsertChecks_ThinBound[10U];struct Cyc_InsertChecks_ThinBound_Absyn_AbsynAnnot_struct{char*tag;struct Cyc_Absyn_Exp*f1;};
# 35 "library/compiler/insert_checks.h"
extern struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct Cyc_InsertChecks_NoCheck_val;
extern struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct Cyc_InsertChecks_NullOnly_val;
extern struct Cyc_InsertChecks_NullAndFatBound_Absyn_AbsynAnnot_struct Cyc_InsertChecks_NullAndFatBound_val;
extern struct Cyc_InsertChecks_FatBound_Absyn_AbsynAnnot_struct Cyc_InsertChecks_FatBound_val;struct Cyc___cycFILE;
# 53 "native/include/cyclone/library/std/cycboot.h"
extern struct Cyc___cycFILE*Cyc_stderr;struct Cyc_String_pa_PrintArg_struct{int tag;struct _fat_ptr f1;};struct Cyc_Int_pa_PrintArg_struct{int tag;unsigned long f1;};struct Cyc_Double_pa_PrintArg_struct{int tag;double f1;};
# 75
extern struct _fat_ptr Cyc_aprintf(struct _fat_ptr,struct _fat_ptr);
# 100
extern int Cyc_fprintf(struct Cyc___cycFILE*,struct _fat_ptr,struct _fat_ptr);
# 69 "library/compiler/absynpp.h"
struct _fat_ptr Cyc_Absynpp_exp2string(struct Cyc_Absyn_Exp*);struct Cyc_Dict_T;struct Cyc_Dict_Dict{int(*rel)(void*,void*);struct _RegionHandle*r;const struct Cyc_Dict_T*t;};struct Cyc_Set_Set;
# 85 "native/include/cyclone/library/std/graph.h"
extern struct Cyc_List_List*Cyc_Graph_tsort(struct Cyc_Dict_Dict);
# 33 "library/compiler/callgraph.h"
struct Cyc_Dict_Dict Cyc_Callgraph_compute_callgraph(struct Cyc_List_List*);extern char Cyc_CfFlowInfo_IsZero[7U];struct Cyc_CfFlowInfo_IsZero_Absyn_AbsynAnnot_struct{char*tag;};extern char Cyc_CfFlowInfo_NotZero[8U];struct Cyc_CfFlowInfo_NotZero_Absyn_AbsynAnnot_struct{char*tag;};extern char Cyc_CfFlowInfo_UnknownZ[9U];struct Cyc_CfFlowInfo_UnknownZ_Absyn_AbsynAnnot_struct{char*tag;};struct _tuple11{unsigned f0;long f1;};
# 29 "library/compiler/evexp.h"
extern struct _tuple11 Cyc_Evexp_eval_const_uint_exp(struct Cyc_Absyn_Exp*);
# 33
extern long Cyc_Evexp_c_can_eval(struct Cyc_Absyn_Exp*);
# 43 "library/compiler/evexp.h"
extern long Cyc_Evexp_lte_const_exp(struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*);
# 28 "library/compiler/flags.h"
extern long Cyc_Flags_verbose;
# 70
extern long Cyc_Flags_warn_bounds_checks;
# 74
extern long Cyc_Flags_warn_all_null_deref;
# 242 "library/compiler/assndef.h"
extern struct _fat_ptr Cyc_AssnDef_assn2string(void*);struct Cyc_AssnDef_AssnFn{struct Cyc_List_List*actuals;void*assn;};struct Cyc_AssnDef_ExistAssnFn{struct Cyc_AssnDef_AssnFn*af;struct Cyc_Set_Set*existvars;};struct Cyc_AssnDef_AssnMap{void*assn;struct Cyc_Dict_Dict map;};
# 299
extern struct _fat_ptr Cyc_AssnDef_assnmap2string(struct Cyc_AssnDef_AssnMap);
# 324
extern long Cyc_AssnDef_simple_prove(void*,void*);
# 382 "library/compiler/assndef.h"
void Cyc_AssnDef_reset_hash_cons_table (void);
# 7 "library/compiler/pratt_prover.h"
long Cyc_PrattProver_constraint_prove(void*,void*);
# 47 "library/compiler/tcutil.h"
long Cyc_Tcutil_is_zeroterm_pointer_type(void*);
# 70
struct Cyc_Absyn_Exp*Cyc_Tcutil_get_bounds_exp_constrain(void*,void*,long);
# 73
struct Cyc_Absyn_Exp*Cyc_Tcutil_get_type_bound(void*);
# 219 "library/compiler/tcutil.h"
long Cyc_Tcutil_is_const_exp(struct Cyc_Absyn_Exp*);
# 246
long Cyc_Tcutil_force_type2bool(long,void*);struct Cyc_Vcgen_ExpChecks{struct Cyc_AssnDef_AssnMap ctxt;void*bounds_check;void*null_check;};
# 36 "library/compiler/vcgen.h"
extern void Cyc_Vcgen_vcgen_fundecl(unsigned,struct Cyc_JumpAnalysis_Jump_Anal_Result*,struct Cyc_Absyn_Fndecl*,struct Cyc_Hashtable_Table**,struct Cyc_Hashtable_Table*,long);
# 39
struct Cyc_Vcgen_ExpChecks*Cyc_Vcgen_exp2ctxt_checks(struct Cyc_Hashtable_Table**,struct Cyc_Absyn_Exp*);struct Cyc___cycFILE;
# 98 "native/generated/libc/stdio.h"
extern struct Cyc___cycFILE*Cyc_stderr;
# 127 "native/generated/libc/stdio.h"
extern struct _fat_ptr Cyc_aprintf(struct _fat_ptr,struct _fat_ptr);
# 161
extern int Cyc_fprintf(struct Cyc___cycFILE*,struct _fat_ptr,struct _fat_ptr);
# 41 "library/compiler/warn.h"
void Cyc_Warn_warn(unsigned,struct _fat_ptr,struct _fat_ptr);
# 46
void Cyc_Warn_err(unsigned,struct _fat_ptr,struct _fat_ptr);
# 51
void*Cyc_Warn_impos(struct _fat_ptr,struct _fat_ptr);
# 56
void*Cyc_Warn_impos_loc(unsigned,struct _fat_ptr,struct _fat_ptr);char Cyc_InsertChecks_FatBound[9U]="FatBound";char Cyc_InsertChecks_NoCheck[8U]="NoCheck";char Cyc_InsertChecks_NullAndFatBound[16U]="NullAndFatBound";char Cyc_InsertChecks_NullAndThinBound[17U]="NullAndThinBound";char Cyc_InsertChecks_NullOnly[9U]="NullOnly";char Cyc_InsertChecks_ThinBound[10U]="ThinBound";
# 66 "library/compiler/insert_checks.cyc"
struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct Cyc_InsertChecks_NoCheck_val={Cyc_InsertChecks_NoCheck};
struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct Cyc_InsertChecks_NullOnly_val={Cyc_InsertChecks_NullOnly};
struct Cyc_InsertChecks_NullAndFatBound_Absyn_AbsynAnnot_struct Cyc_InsertChecks_NullAndFatBound_val={Cyc_InsertChecks_NullAndFatBound};
struct Cyc_InsertChecks_FatBound_Absyn_AbsynAnnot_struct Cyc_InsertChecks_FatBound_val={Cyc_InsertChecks_FatBound};
# 73
static unsigned Cyc_InsertChecks_total_bounds_checks=0U;
static unsigned Cyc_InsertChecks_types_eliminated_bounds_checks=0U;
static unsigned Cyc_InsertChecks_simple_eliminated_bounds_checks=0U;
static unsigned Cyc_InsertChecks_constraints_eliminated_bounds_checks=0U;
# 78
static unsigned Cyc_InsertChecks_total_null_checks=0U;
static unsigned Cyc_InsertChecks_types_eliminated_null_checks=0U;
static unsigned Cyc_InsertChecks_flow_eliminated_null_checks=0U;
static unsigned Cyc_InsertChecks_simple_eliminated_null_checks=0U;
static unsigned Cyc_InsertChecks_constraints_eliminated_null_checks=0U;
# 84
static void Cyc_InsertChecks_reset_counters (void){
Cyc_InsertChecks_total_bounds_checks= 0U;
Cyc_InsertChecks_types_eliminated_bounds_checks= 0U;
Cyc_InsertChecks_simple_eliminated_bounds_checks= 0U;
Cyc_InsertChecks_constraints_eliminated_bounds_checks= 0U;
# 90
Cyc_InsertChecks_total_null_checks= 0U;
Cyc_InsertChecks_types_eliminated_null_checks= 0U;
Cyc_InsertChecks_flow_eliminated_null_checks= 0U;
Cyc_InsertChecks_simple_eliminated_null_checks= 0U;
Cyc_InsertChecks_constraints_eliminated_null_checks= 0U;}
# 97
static struct Cyc_Absyn_Exp*Cyc_InsertChecks_shared_zero_exp (void){struct Cyc_Absyn_Exp*_T0;unsigned _T1;struct Cyc_Absyn_Exp*_T2;
static struct Cyc_Absyn_Exp*ans=0;_T0= ans;_T1= (unsigned)_T0;
# 100
if(_T1)goto _TL0;else{goto _TL2;}
_TL2: ans= Cyc_Absyn_uint_exp(0U,0U);goto _TL1;_TL0: _TL1: _T2= ans;
return _T2;}struct Cyc_InsertChecks_Env{struct Cyc_JumpAnalysis_Jump_Anal_Result*tables;struct Cyc_Hashtable_Table**assn_info;struct Cyc_Hashtable_Table*fn_precond_info;unsigned pass_counter;long use_precond: 1;long in_sizeof: 1;long at_toplevel: 1;long vcgen_r: 1;};
# 118
static int Cyc_InsertChecks_hash_ptr(void*s){void*_T0;int _T1;_T0= s;_T1= (int)_T0;return _T1;}
# 120
static struct Cyc_InsertChecks_Env Cyc_InsertChecks_toplevel_env(struct Cyc_JumpAnalysis_Jump_Anal_Result*tables,long do_vcgen){struct Cyc_Hashtable_Table*(*_T0)(int,int(*)(struct _tuple0*,struct _tuple0*),int(*)(struct _tuple0*));struct Cyc_Hashtable_Table*(*_T1)(int,int(*)(void*,void*),int(*)(void*));int(*_T2)(struct _tuple0*,struct _tuple0*);int(*_T3)(struct _tuple0*);struct Cyc_InsertChecks_Env _T4;_T1= Cyc_Hashtable_create;{
struct Cyc_Hashtable_Table*(*_T5)(int,int(*)(struct _tuple0*,struct _tuple0*),int(*)(struct _tuple0*))=(struct Cyc_Hashtable_Table*(*)(int,int(*)(struct _tuple0*,struct _tuple0*),int(*)(struct _tuple0*)))_T1;_T0= _T5;}_T2= Cyc_Absyn_qvar_cmp;_T3= Cyc_Absyn_hash_qvar;{struct Cyc_Hashtable_Table*fn_precond_info=_T0(313,_T2,_T3);{struct Cyc_InsertChecks_Env _T5;
# 123
_T5.tables= tables;_T5.assn_info= 0;_T5.fn_precond_info= fn_precond_info;
_T5.pass_counter= 1U;
_T5.use_precond= (long)0;
_T5.in_sizeof= (long)0;
_T5.at_toplevel= (long)1;
_T5.vcgen_r= do_vcgen;_T4= _T5;}
# 123
return _T4;}}
# 131
static struct Cyc_InsertChecks_Env Cyc_InsertChecks_fn_body_env(struct Cyc_InsertChecks_Env nv){struct Cyc_InsertChecks_Env _T0;struct Cyc_InsertChecks_Env _T1;struct Cyc_InsertChecks_Env _T2;struct Cyc_InsertChecks_Env _T3;struct Cyc_InsertChecks_Env _T4;struct Cyc_InsertChecks_Env _T5;struct Cyc_InsertChecks_Env _T6;{struct Cyc_InsertChecks_Env _T7;_T1= nv;
_T7.tables= _T1.tables;_T2= nv;_T7.assn_info= _T2.assn_info;_T3= nv;_T7.fn_precond_info= _T3.fn_precond_info;_T4= nv;_T7.pass_counter= _T4.pass_counter;_T5= nv;_T7.use_precond= _T5.use_precond;_T7.in_sizeof= (long)0;
_T7.at_toplevel= (long)0;_T6= nv;_T7.vcgen_r= _T6.vcgen_r;_T0= _T7;}
# 132
return _T0;}
# 136
static struct Cyc_InsertChecks_Env Cyc_InsertChecks_enter_sizeof(struct Cyc_InsertChecks_Env nv){struct Cyc_InsertChecks_Env _T0;struct Cyc_InsertChecks_Env _T1;struct Cyc_InsertChecks_Env _T2;struct Cyc_InsertChecks_Env _T3;struct Cyc_InsertChecks_Env _T4;struct Cyc_InsertChecks_Env _T5;struct Cyc_InsertChecks_Env _T6;struct Cyc_InsertChecks_Env _T7;{struct Cyc_InsertChecks_Env _T8;_T1= nv;
_T8.tables= _T1.tables;_T2= nv;_T8.assn_info= _T2.assn_info;_T3= nv;_T8.fn_precond_info= _T3.fn_precond_info;_T4= nv;_T8.pass_counter= _T4.pass_counter;_T5= nv;_T8.use_precond= _T5.use_precond;_T8.in_sizeof= (long)1;_T6= nv;
_T8.at_toplevel= _T6.at_toplevel;_T7= nv;_T8.vcgen_r= _T7.vcgen_r;_T0= _T8;}
# 137
return _T0;}struct _tuple14{long f0;long f1;};
# 142
static struct _tuple14 Cyc_InsertChecks_need_bound_null_check_annot(struct Cyc_Absyn_Exp*e){struct Cyc_Absyn_Exp*_T0;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T1;char*_T2;char*_T3;struct Cyc_InsertChecks_FatBound_Absyn_AbsynAnnot_struct*_T4;char*_T5;char*_T6;struct Cyc_InsertChecks_ThinBound_Absyn_AbsynAnnot_struct*_T7;char*_T8;char*_T9;struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*_TA;char*_TB;char*_TC;struct _tuple14 _TD;
long bds_ck;long null_ck;_T0= e;{
# 145
void*_TE=_T0->annot;_T1= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_TE;_T2= _T1->tag;_T3= Cyc_InsertChecks_NoCheck;if(_T2!=_T3)goto _TL3;
# 147
bds_ck= (long)0;
null_ck= (long)0;goto _LL0;_TL3: _T4= (struct Cyc_InsertChecks_FatBound_Absyn_AbsynAnnot_struct*)_TE;_T5= _T4->tag;_T6= Cyc_InsertChecks_FatBound;if(_T5!=_T6)goto _TL5;
# 151
bds_ck= (long)1;
null_ck= (long)0;goto _LL0;_TL5: _T7= (struct Cyc_InsertChecks_ThinBound_Absyn_AbsynAnnot_struct*)_TE;_T8= _T7->tag;_T9= Cyc_InsertChecks_ThinBound;if(_T8!=_T9)goto _TL7;
# 155
bds_ck= (long)1;
null_ck= (long)0;goto _LL0;_TL7: _TA= (struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*)_TE;_TB= _TA->tag;_TC= Cyc_InsertChecks_NullOnly;if(_TB!=_TC)goto _TL9;
# 159
bds_ck= (long)0;
null_ck= (long)1;goto _LL0;_TL9:
# 163
 bds_ck= (long)1;
null_ck= (long)1;_LL0:;}{struct _tuple14 _TE;
# 166
_TE.f0= bds_ck;_TE.f1= null_ck;_TD= _TE;}return _TD;}
# 171
static long Cyc_InsertChecks_need_null_check(struct Cyc_InsertChecks_Env nv,struct Cyc_Absyn_Exp*e,struct Cyc_Absyn_Exp*e0){struct Cyc_InsertChecks_Env _T0;unsigned _T1;long _T2;struct Cyc_InsertChecks_Env _T3;long _T4;int _T5;long _T6;struct Cyc_Absyn_Exp*_T7;void*_T8;void*_T9;int*_TA;int _TB;struct Cyc_Absyn_PtrInfo _TC;struct Cyc_Absyn_PtrAtts _TD;long _TE;void*_TF;long _T10;int _T11;long _T12;int(*_T13)(unsigned,struct _fat_ptr,struct _fat_ptr);void*(*_T14)(unsigned,struct _fat_ptr,struct _fat_ptr);struct Cyc_Absyn_Exp*_T15;unsigned _T16;struct _fat_ptr _T17;struct _fat_ptr _T18;struct Cyc_Absyn_Exp*_T19;struct Cyc_CfFlowInfo_UnknownZ_Absyn_AbsynAnnot_struct*_T1A;char*_T1B;char*_T1C;struct Cyc_CfFlowInfo_NotZero_Absyn_AbsynAnnot_struct*_T1D;char*_T1E;char*_T1F;long _T20;struct Cyc_CfFlowInfo_IsZero_Absyn_AbsynAnnot_struct*_T21;char*_T22;char*_T23;struct Cyc_Absyn_Exp*_T24;unsigned _T25;struct _fat_ptr _T26;struct _fat_ptr _T27;long _T28;struct Cyc_Absyn_EmptyAnnot_Absyn_AbsynAnnot_struct*_T29;char*_T2A;char*_T2B;long _T2C;int(*_T2D)(unsigned,struct _fat_ptr,struct _fat_ptr);void*(*_T2E)(unsigned,struct _fat_ptr,struct _fat_ptr);struct Cyc_Absyn_Exp*_T2F;unsigned _T30;struct _fat_ptr _T31;struct _fat_ptr _T32;struct Cyc_InsertChecks_Env _T33;long _T34;int _T35;struct Cyc_Absyn_Exp*_T36;unsigned _T37;struct _fat_ptr _T38;struct _fat_ptr _T39;long _T3A;long _T3B;_T0= nv;_T1= _T0.pass_counter;
# 175
if(_T1 <= 1U)goto _TLB;{
struct _tuple14 _T3C=Cyc_InsertChecks_need_bound_null_check_annot(e0);long _T3D;_T3D= _T3C.f1;{long null_ck=_T3D;_T2= null_ck;
return _T2;}}_TLB: _T3= nv;_T4= _T3.in_sizeof;_T5= (int)_T4;
# 179
if(!_T5)goto _TLD;_T6= (long)0;
return _T6;_TLD:
# 182
 Cyc_InsertChecks_total_null_checks= Cyc_InsertChecks_total_null_checks + 1;_T7= e;_T8= _T7->topt;_T9=
_check_null(_T8);{void*_T3C=Cyc_Absyn_compress(_T9);void*_T3D;_TA= (int*)_T3C;_TB= *_TA;if(_TB!=4)goto _TLF;{struct Cyc_Absyn_PointerType_Absyn_Type_struct*_T3E=(struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_T3C;_TC= _T3E->f1;_TD= _TC.ptr_atts;_T3D= _TD.nullable;}{void*n=_T3D;_TE= (long)0;_TF= n;_T10=
# 185
Cyc_Tcutil_force_type2bool(_TE,_TF);_T11= (int)_T10;if(_T11)goto _TL11;else{goto _TL13;}
_TL13: Cyc_InsertChecks_types_eliminated_null_checks= Cyc_InsertChecks_types_eliminated_null_checks + 1;_T12= (long)0;
return _T12;_TL11: goto _LL3;}_TLF: _T14= Cyc_Warn_impos_loc;{
# 191
int(*_T3E)(unsigned,struct _fat_ptr,struct _fat_ptr)=(int(*)(unsigned,struct _fat_ptr,struct _fat_ptr))_T14;_T13= _T3E;}_T15= e;_T16= _T15->loc;_T17= _tag_fat("need_null_check: non-pointer type",sizeof(char),34U);_T18= _tag_fat(0U,sizeof(void*),0);_T13(_T16,_T17,_T18);_LL3:;}_T19= e;{
# 193
void*_T3C=_T19->annot;_T1A= (struct Cyc_CfFlowInfo_UnknownZ_Absyn_AbsynAnnot_struct*)_T3C;_T1B= _T1A->tag;_T1C= Cyc_CfFlowInfo_UnknownZ;if(_T1B!=_T1C)goto _TL14;goto _LL8;_TL14: _T1D= (struct Cyc_CfFlowInfo_NotZero_Absyn_AbsynAnnot_struct*)_T3C;_T1E= _T1D->tag;_T1F= Cyc_CfFlowInfo_NotZero;if(_T1E!=_T1F)goto _TL16;
# 197
Cyc_InsertChecks_flow_eliminated_null_checks= Cyc_InsertChecks_flow_eliminated_null_checks + 1;_T20= (long)0;
return _T20;_TL16: _T21= (struct Cyc_CfFlowInfo_IsZero_Absyn_AbsynAnnot_struct*)_T3C;_T22= _T21->tag;_T23= Cyc_CfFlowInfo_IsZero;if(_T22!=_T23)goto _TL18;_T24= e;_T25= _T24->loc;_T26=
# 200
_tag_fat("NULL pointer check will definitely fail",sizeof(char),40U);_T27= _tag_fat(0U,sizeof(void*),0);Cyc_Warn_err(_T25,_T26,_T27);_T28= (long)0;
return _T28;_TL18: _T29= (struct Cyc_Absyn_EmptyAnnot_Absyn_AbsynAnnot_struct*)_T3C;_T2A= _T29->tag;_T2B= Cyc_Absyn_EmptyAnnot;if(_T2A!=_T2B)goto _TL1A;
# 204
Cyc_InsertChecks_flow_eliminated_null_checks= Cyc_InsertChecks_flow_eliminated_null_checks + 1;_T2C= (long)0;
return _T2C;_TL1A: _T2E= Cyc_Warn_impos_loc;{
# 207
int(*_T3D)(unsigned,struct _fat_ptr,struct _fat_ptr)=(int(*)(unsigned,struct _fat_ptr,struct _fat_ptr))_T2E;_T2D= _T3D;}_T2F= e;_T30= _T2F->loc;_T31= _tag_fat("need_null_check: unexpected annotation",sizeof(char),39U);_T32= _tag_fat(0U,sizeof(void*),0);_T2D(_T30,_T31,_T32);_LL8:;}_T33= nv;_T34= _T33.at_toplevel;_T35= (int)_T34;
# 209
if(!_T35)goto _TL1C;_T36= e;_T37= _T36->loc;_T38=
_tag_fat("cannot perform NULL-pointer check at toplevel",sizeof(char),46U);_T39= _tag_fat(0U,sizeof(void*),0);Cyc_Warn_err(_T37,_T38,_T39);_T3A= (long)0;
return _T3A;_TL1C: _T3B= (long)1;
# 217
return _T3B;}
# 221
static long Cyc_InsertChecks_simple_need_bounds_check(struct Cyc_InsertChecks_Env nv,struct Cyc_Absyn_Exp*a,struct Cyc_Absyn_Exp*i){struct Cyc_InsertChecks_Env _T0;long _T1;int _T2;long _T3;struct Cyc_InsertChecks_Env _T4;long _T5;int _T6;int(*_T7)(unsigned,struct _fat_ptr,struct _fat_ptr);void*(*_T8)(unsigned,struct _fat_ptr,struct _fat_ptr);struct Cyc_Absyn_Exp*_T9;unsigned _TA;struct _fat_ptr _TB;struct _fat_ptr _TC;struct Cyc_Absyn_Exp*_TD;void*_TE;void*_TF;long _T10;long _T11;int _T12;long _T13;int _T14;long _T15;long _T16;int _T17;long _T18;int _T19;long _T1A;long _T1B;_T0= nv;_T1= _T0.in_sizeof;_T2= (int)_T1;
if(!_T2)goto _TL1E;_T3= (long)0;
return _T3;_TL1E:
 Cyc_InsertChecks_total_bounds_checks= Cyc_InsertChecks_total_bounds_checks + 1;_T4= nv;_T5= _T4.at_toplevel;_T6= (int)_T5;
if(!_T6)goto _TL20;_T8= Cyc_Warn_impos_loc;{
int(*_T1C)(unsigned,struct _fat_ptr,struct _fat_ptr)=(int(*)(unsigned,struct _fat_ptr,struct _fat_ptr))_T8;_T7= _T1C;}_T9= a;_TA= _T9->loc;_TB= _tag_fat("InsertChecks: memory read at top-level",sizeof(char),39U);_TC= _tag_fat(0U,sizeof(void*),0);_T7(_TA,_TB,_TC);goto _TL21;_TL20: _TL21: _TD= a;_TE= _TD->topt;_TF=
# 232
_check_null(_TE);{struct Cyc_Absyn_Exp*bound_opt=Cyc_Tcutil_get_type_bound(_TF);
# 234
if(bound_opt!=0)goto _TL22;_T10= (long)1;
return _T10;_TL22: {
struct Cyc_Absyn_Exp*bound=bound_opt;_T11=
# 238
Cyc_Tcutil_is_const_exp(bound);_T12= (int)_T11;if(_T12)goto _TL27;else{goto _TL26;}_TL27: _T13= Cyc_Tcutil_is_const_exp(i);_T14= (int)_T13;if(_T14)goto _TL24;else{goto _TL26;}
_TL26: _T15= (long)1;return _T15;_TL24: {
struct _tuple11 _T1C=Cyc_Evexp_eval_const_uint_exp(bound);long _T1D;unsigned _T1E;_T1E= _T1C.f0;_T1D= _T1C.f1;{unsigned b=_T1E;long b_known=_T1D;
struct _tuple11 _T1F=Cyc_Evexp_eval_const_uint_exp(i);long _T20;unsigned _T21;_T21= _T1F.f0;_T20= _T1F.f1;{unsigned i=_T21;long i_known=_T20;_T16= b_known;_T17= (int)_T16;
# 243
if(_T17)goto _TL2C;else{goto _TL2A;}_TL2C: _T18= i_known;_T19= (int)_T18;if(_T19)goto _TL2B;else{goto _TL2A;}_TL2B: if(i >= b)goto _TL2A;else{goto _TL28;}
_TL2A: _T1A= (long)1;return _T1A;_TL28:
# 246
 Cyc_InsertChecks_types_eliminated_bounds_checks= Cyc_InsertChecks_types_eliminated_bounds_checks + 1;_T1B= (long)0;
return _T1B;}}}}}}
# 250
static void Cyc_InsertChecks_do_stmt(struct Cyc_InsertChecks_Env,struct Cyc_Absyn_Stmt*);
static void Cyc_InsertChecks_do_exp(struct Cyc_InsertChecks_Env,struct Cyc_Absyn_Exp*);
# 253
static void Cyc_InsertChecks_do_exp_opt(struct Cyc_InsertChecks_Env nv,struct Cyc_Absyn_Exp*eo){
if(eo==0)goto _TL2D;
Cyc_InsertChecks_do_exp(nv,eo);goto _TL2E;_TL2D: _TL2E:;}
# 258
static long Cyc_InsertChecks_need_null_check_vcgen(struct Cyc_InsertChecks_Env nv,struct Cyc_Absyn_Exp*a){struct Cyc_InsertChecks_Env _T0;struct Cyc_Hashtable_Table**_T1;struct Cyc_Absyn_Exp*_T2;struct Cyc_Vcgen_ExpChecks*_T3;struct Cyc_AssnDef_AssnMap _T4;void*_T5;struct Cyc_Vcgen_ExpChecks*_T6;void*_T7;long _T8;int _T9;struct Cyc_Vcgen_ExpChecks*_TA;struct Cyc_AssnDef_AssnMap _TB;void*_TC;struct Cyc_Vcgen_ExpChecks*_TD;void*_TE;long _TF;int _T10;long _T11;int _T12;int _T13;long _T14;_T0= nv;_T1= _T0.assn_info;_T2= a;{
struct Cyc_Vcgen_ExpChecks*ec=Cyc_Vcgen_exp2ctxt_checks(_T1,_T2);_T3= ec;_T4= _T3->ctxt;_T5= _T4.assn;_T6= ec;_T7= _T6->null_check;{
long res=Cyc_AssnDef_simple_prove(_T5,_T7);_T8= res;_T9= (int)_T8;
# 262
if(!_T9)goto _TL2F;
Cyc_InsertChecks_simple_eliminated_null_checks= Cyc_InsertChecks_simple_eliminated_null_checks + 1;goto _TL30;
# 265
_TL2F: _TA= ec;_TB= _TA->ctxt;_TC= _TB.assn;_TD= ec;_TE= _TD->null_check;res= Cyc_PrattProver_constraint_prove(_TC,_TE);_TF= res;_T10= (int)_TF;
if(!_T10)goto _TL31;
Cyc_InsertChecks_constraints_eliminated_null_checks= Cyc_InsertChecks_constraints_eliminated_null_checks + 1;goto _TL32;_TL31: _TL32: _TL30: _T11= res;_T12= (int)_T11;_T13= !_T12;_T14= (long)_T13;
# 269
return _T14;}}}
# 272
static long Cyc_InsertChecks_need_bounds_check_vcgen(struct Cyc_InsertChecks_Env nv,struct Cyc_Absyn_Exp*a){struct Cyc_InsertChecks_Env _T0;struct Cyc_Hashtable_Table**_T1;struct Cyc_Absyn_Exp*_T2;struct Cyc_Vcgen_ExpChecks*_T3;struct Cyc_AssnDef_AssnMap _T4;void*_T5;struct Cyc_Vcgen_ExpChecks*_T6;void*_T7;long _T8;int _T9;struct Cyc_Vcgen_ExpChecks*_TA;struct Cyc_AssnDef_AssnMap _TB;void*_TC;struct Cyc_Vcgen_ExpChecks*_TD;void*_TE;long _TF;int _T10;long _T11;int _T12;int _T13;long _T14;_T0= nv;_T1= _T0.assn_info;_T2= a;{
struct Cyc_Vcgen_ExpChecks*ec=Cyc_Vcgen_exp2ctxt_checks(_T1,_T2);_T3= ec;_T4= _T3->ctxt;_T5= _T4.assn;_T6= ec;_T7= _T6->bounds_check;{
long res=Cyc_AssnDef_simple_prove(_T5,_T7);_T8= res;_T9= (int)_T8;
# 276
if(!_T9)goto _TL33;
Cyc_InsertChecks_simple_eliminated_bounds_checks= Cyc_InsertChecks_simple_eliminated_bounds_checks + 1;goto _TL34;
# 279
_TL33: _TA= ec;_TB= _TA->ctxt;_TC= _TB.assn;_TD= ec;_TE= _TD->bounds_check;res= Cyc_PrattProver_constraint_prove(_TC,_TE);_TF= res;_T10= (int)_TF;
if(!_T10)goto _TL35;
Cyc_InsertChecks_constraints_eliminated_bounds_checks= Cyc_InsertChecks_constraints_eliminated_bounds_checks + 1;goto _TL36;_TL35: _TL36: _TL34: _T11= res;_T12= (int)_T11;_T13= !_T12;_T14= (long)_T13;
# 283
return _T14;}}}struct _tuple15{void*f0;void*f1;};struct _tuple16{struct Cyc_Absyn_Exp*f0;struct Cyc_Absyn_Exp*f1;};struct _tuple17{struct Cyc_List_List*f0;struct Cyc_Absyn_Exp*f1;};
# 286
static void Cyc_InsertChecks_do_exp(struct Cyc_InsertChecks_Env nv,struct Cyc_Absyn_Exp*e){struct Cyc_Absyn_Exp*_T0;int*_T1;unsigned _T2;struct Cyc_Absyn_Exp*_T3;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T4;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T5;struct Cyc_Absyn_Exp*_T6;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T7;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T8;struct Cyc_InsertChecks_Env _T9;struct Cyc_List_List*_TA;void*_TB;struct Cyc_Absyn_Exp*_TC;struct Cyc_List_List*_TD;struct Cyc_Absyn_Exp*_TE;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_TF;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T10;struct Cyc_Absyn_Exp*_T11;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T12;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T13;struct Cyc_Absyn_Exp*_T14;void*_T15;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T16;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T17;void*_T18;struct Cyc_Absyn_Exp*_T19;void*_T1A;void*_T1B;struct Cyc_Absyn_Exp*_T1C;struct Cyc_Absyn_Exp*_T1D;void*_T1E;void*_T1F;long _T20;int _T21;long _T22;int _T23;long _T24;int _T25;long _T26;int _T27;struct Cyc_Absyn_Exp*_T28;unsigned _T29;struct _fat_ptr _T2A;struct _fat_ptr _T2B;struct Cyc_Absyn_Exp*_T2C;struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*_T2D;struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*_T2E;struct Cyc_Absyn_Exp*_T2F;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T30;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T31;long _T32;int _T33;long _T34;int _T35;struct Cyc_Absyn_Exp*_T36;struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*_T37;struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*_T38;struct Cyc_Absyn_Exp*_T39;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T3A;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T3B;struct Cyc_InsertChecks_Env _T3C;struct Cyc_List_List*_T3D;void*_T3E;struct Cyc_Absyn_Exp*_T3F;struct Cyc_List_List*_T40;void*_T41;struct _tuple15 _T42;struct Cyc_Absyn_Exp*_T43;void*_T44;void*_T45;void*_T46;int*_T47;int _T48;void*_T49;int*_T4A;int _T4B;void*_T4C;void*_T4D;long _T4E;struct Cyc_Absyn_PtrInfo _T4F;struct Cyc_Absyn_PtrAtts _T50;void*_T51;long _T52;struct Cyc_Absyn_PtrInfo _T53;struct Cyc_Absyn_PtrAtts _T54;void*_T55;void*_T56;struct Cyc_Absyn_PtrInfo _T57;struct Cyc_Absyn_PtrAtts _T58;void*_T59;long _T5A;void*_T5B;struct Cyc_Absyn_PtrInfo _T5C;struct Cyc_Absyn_PtrAtts _T5D;void*_T5E;long _T5F;long _T60;int _T61;long _T62;int _T63;enum Cyc_Absyn_Coercion _T64;int _T65;struct Cyc_Absyn_Exp*_T66;unsigned _T67;struct Cyc_Absyn_Exp*_T68;unsigned _T69;int(*_T6A)(unsigned,struct _fat_ptr,struct _fat_ptr);void*(*_T6B)(unsigned,struct _fat_ptr,struct _fat_ptr);struct Cyc_Absyn_Exp*_T6C;unsigned _T6D;struct _fat_ptr _T6E;struct _fat_ptr _T6F;struct Cyc_Absyn_Exp*_T70;unsigned _T71;struct Cyc_Absyn_Exp*_T72;unsigned _T73;enum Cyc_Absyn_Coercion _T74;int _T75;int(*_T76)(unsigned,struct _fat_ptr,struct _fat_ptr);void*(*_T77)(unsigned,struct _fat_ptr,struct _fat_ptr);struct Cyc_Absyn_Exp*_T78;unsigned _T79;struct _fat_ptr _T7A;struct _fat_ptr _T7B;struct _tuple16 _T7C;struct Cyc_Absyn_Exp*_T7D;struct Cyc_Absyn_Exp*_T7E;long _T7F;int _T80;int(*_T81)(unsigned,struct _fat_ptr,struct _fat_ptr);void*(*_T82)(unsigned,struct _fat_ptr,struct _fat_ptr);struct Cyc_Absyn_Exp*_T83;unsigned _T84;struct _fat_ptr _T85;struct _fat_ptr _T86;long _T87;int _T88;long _T89;int _T8A;long _T8B;int _T8C;long _T8D;int _T8E;long _T8F;int _T90;struct Cyc_Absyn_Exp*_T91;unsigned _T92;struct _fat_ptr _T93;struct _fat_ptr _T94;struct Cyc_Absyn_Exp*_T95;struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*_T96;struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*_T97;struct Cyc_Absyn_Exp*_T98;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T99;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T9A;struct Cyc_Absyn_Exp*_T9B;long _T9C;int _T9D;struct Cyc_Absyn_Exp*_T9E;unsigned _T9F;struct _fat_ptr _TA0;struct _fat_ptr _TA1;struct Cyc_Absyn_Exp*_TA2;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_TA3;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_TA4;struct Cyc_Absyn_Exp*_TA5;struct Cyc_InsertChecks_Env _TA6;long _TA7;int _TA8;struct Cyc_Absyn_Exp*_TA9;unsigned _TAA;struct _fat_ptr _TAB;struct _fat_ptr _TAC;long _TAD;int _TAE;long _TAF;int _TB0;struct Cyc_Absyn_Exp*_TB1;long _TB2;int _TB3;struct Cyc_Absyn_Exp*_TB4;unsigned _TB5;struct _fat_ptr _TB6;struct _fat_ptr _TB7;long _TB8;int _TB9;long _TBA;int _TBB;long _TBC;int _TBD;long _TBE;int _TBF;long _TC0;int _TC1;struct Cyc_Absyn_Exp*_TC2;unsigned _TC3;struct _fat_ptr _TC4;struct _fat_ptr _TC5;struct Cyc_Absyn_Exp*_TC6;void*_TC7;long _TC8;int _TC9;struct Cyc_InsertChecks_NullAndFatBound_Absyn_AbsynAnnot_struct*_TCA;struct Cyc_InsertChecks_NullAndFatBound_Absyn_AbsynAnnot_struct*_TCB;struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*_TCC;struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*_TCD;struct Cyc_Absyn_Exp*_TCE;void*_TCF;long _TD0;int _TD1;struct Cyc_InsertChecks_FatBound_Absyn_AbsynAnnot_struct*_TD2;struct Cyc_InsertChecks_FatBound_Absyn_AbsynAnnot_struct*_TD3;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_TD4;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_TD5;struct Cyc_Absyn_Exp*_TD6;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_TD7;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_TD8;struct Cyc_Absyn_Exp*_TD9;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_TDA;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_TDB;struct Cyc_Absyn_Exp*_TDC;void*_TDD;void*_TDE;int*_TDF;int _TE0;struct Cyc_Absyn_PtrInfo _TE1;struct Cyc_Absyn_PtrInfo _TE2;struct Cyc_Absyn_PtrInfo _TE3;struct Cyc_Absyn_PtrAtts _TE4;struct Cyc_Absyn_PtrInfo _TE5;struct Cyc_Absyn_PtrAtts _TE6;struct Cyc_Absyn_PtrInfo _TE7;struct Cyc_Absyn_PtrAtts _TE8;struct Cyc_Absyn_PtrInfo _TE9;struct Cyc_Absyn_PtrAtts _TEA;long _TEB;int _TEC;long _TED;int _TEE;struct Cyc_InsertChecks_Env _TEF;long _TF0;int _TF1;struct Cyc_Absyn_Exp*_TF2;void*_TF3;struct Cyc_Absyn_Exp*_TF4;void*_TF5;int*_TF6;int _TF7;struct Cyc_Absyn_AppType_Absyn_Type_struct*_TF8;void*_TF9;int*_TFA;int _TFB;struct Cyc_Absyn_AppType_Absyn_Type_struct*_TFC;void*_TFD;struct Cyc_Absyn_IntCon_Absyn_TyCon_struct*_TFE;enum Cyc_Absyn_Size_of _TFF;int _T100;long _T101;int _T102;long _T103;int _T104;struct Cyc_String_pa_PrintArg_struct _T105;struct Cyc_Absyn_Exp*_T106;unsigned _T107;struct _fat_ptr _T108;struct _fat_ptr _T109;void*_T10A;void*_T10B;long _T10C;int _T10D;long _T10E;int _T10F;int _T110;long _T111;int _T112;long _T113;long _T114;int _T115;long _T116;int _T117;struct Cyc_Absyn_Exp*_T118;unsigned _T119;struct _fat_ptr _T11A;struct _fat_ptr _T11B;long _T11C;int _T11D;struct Cyc_Absyn_Exp*_T11E;unsigned _T11F;long _T120;int _T121;struct Cyc_InsertChecks_Env _T122;struct Cyc_Hashtable_Table**_T123;struct Cyc_Absyn_Exp*_T124;struct Cyc_Absyn_Exp*_T125;unsigned _T126;struct _fat_ptr _T127;struct Cyc_String_pa_PrintArg_struct _T128;struct Cyc_Vcgen_ExpChecks*_T129;void*_T12A;struct _fat_ptr _T12B;struct _fat_ptr _T12C;struct _fat_ptr _T12D;struct Cyc_String_pa_PrintArg_struct _T12E;struct Cyc_Vcgen_ExpChecks*_T12F;struct Cyc_AssnDef_AssnMap _T130;struct Cyc___cycFILE*_T131;struct _fat_ptr _T132;struct _fat_ptr _T133;struct Cyc___cycFILE*_T134;struct _fat_ptr _T135;struct _fat_ptr _T136;struct Cyc_Absyn_Exp*_T137;void*_T138;long _T139;int _T13A;struct Cyc_InsertChecks_NullAndThinBound_Absyn_AbsynAnnot_struct*_T13B;struct Cyc_InsertChecks_ThinBound_Absyn_AbsynAnnot_struct*_T13C;struct Cyc_Absyn_Exp*_T13D;void*_T13E;long _T13F;int _T140;struct Cyc_InsertChecks_NullAndFatBound_Absyn_AbsynAnnot_struct*_T141;struct Cyc_InsertChecks_NullAndFatBound_Absyn_AbsynAnnot_struct*_T142;struct Cyc_InsertChecks_FatBound_Absyn_AbsynAnnot_struct*_T143;struct Cyc_InsertChecks_FatBound_Absyn_AbsynAnnot_struct*_T144;long _T145;int _T146;struct Cyc_Absyn_Exp*_T147;struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*_T148;struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*_T149;struct Cyc_Absyn_Exp*_T14A;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T14B;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T14C;int(*_T14D)(unsigned,struct _fat_ptr,struct _fat_ptr);void*(*_T14E)(unsigned,struct _fat_ptr,struct _fat_ptr);struct Cyc_Absyn_Exp*_T14F;unsigned _T150;struct _fat_ptr _T151;struct _fat_ptr _T152;struct Cyc_Absyn_Exp*_T153;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T154;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T155;struct Cyc_InsertChecks_Env _T156;struct Cyc_Absyn_Exp*_T157;struct Cyc_Absyn_Exp*_T158;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T159;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T15A;struct Cyc_Absyn_MallocInfo _T15B;struct Cyc_Absyn_MallocInfo _T15C;struct Cyc_Absyn_MallocInfo _T15D;struct Cyc_Absyn_Exp*_T15E;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T15F;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T160;struct Cyc_Absyn_Exp*_T161;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T162;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T163;struct Cyc_Absyn_Exp*_T164;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T165;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T166;struct Cyc_Absyn_Exp*_T167;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T168;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T169;struct Cyc_Absyn_Exp*_T16A;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T16B;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T16C;struct Cyc_Absyn_Exp*_T16D;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T16E;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T16F;struct Cyc_InsertChecks_Env _T170;struct Cyc_List_List*_T171;void*_T172;struct _tuple17*_T173;struct _tuple17 _T174;struct Cyc_Absyn_Exp*_T175;struct Cyc_List_List*_T176;struct Cyc_Absyn_Exp*_T177;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T178;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T179;struct Cyc_Absyn_Exp*_T17A;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T17B;struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*_T17C;int(*_T17D)(unsigned,struct _fat_ptr,struct _fat_ptr);void*(*_T17E)(unsigned,struct _fat_ptr,struct _fat_ptr);struct Cyc_Absyn_Exp*_T17F;unsigned _T180;struct _fat_ptr _T181;struct _fat_ptr _T182;_T0= e;{
void*_T183=_T0->r;struct Cyc_Absyn_Stmt*_T184;struct Cyc_Absyn_Exp*_T185;enum Cyc_Absyn_Coercion _T186;long _T187;struct Cyc_List_List*_T188;enum Cyc_Absyn_Incrementor _T189;struct Cyc_Absyn_Exp*_T18A;struct Cyc_Absyn_Exp*_T18B;void*_T18C;_T1= (int*)_T183;_T2= *_T1;switch(_T2){case 39: goto _LL4;case 0: _LL4: goto _LL6;case 1: _LL6: goto _LL8;case 2: _LL8: _T3= e;_T4= & Cyc_InsertChecks_NoCheck_val;_T5= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T4;
# 292
_T3->annot= (void*)_T5;goto _LL0;case 30:{struct Cyc_Absyn_Datatype_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Datatype_e_Absyn_Raw_exp_struct*)_T183;_T18C= _T18D->f1;}{struct Cyc_List_List*es=_T18C;_T18C= es;goto _LLC;}case 3:{struct Cyc_Absyn_Primop_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Primop_e_Absyn_Raw_exp_struct*)_T183;_T18C= _T18D->f2;}_LLC: {struct Cyc_List_List*es=_T18C;_T6= e;_T7= & Cyc_InsertChecks_NoCheck_val;_T8= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T7;
# 298
_T6->annot= (void*)_T8;
_TL3B: if(es!=0)goto _TL39;else{goto _TL3A;}
_TL39: _T9= nv;_TA= es;_TB= _TA->hd;_TC= (struct Cyc_Absyn_Exp*)_TB;Cyc_InsertChecks_do_exp(_T9,_TC);_TD= es;
# 299
es= _TD->tl;goto _TL3B;_TL3A: goto _LL0;}case 4:{struct Cyc_Absyn_AssignOp_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_AssignOp_e_Absyn_Raw_exp_struct*)_T183;_T18C= _T18D->f1;_T18B= _T18D->f3;}{struct Cyc_Absyn_Exp*e1=_T18C;struct Cyc_Absyn_Exp*e2=_T18B;_T18C= e1;_T18B= e2;goto _LL10;}case 9:{struct Cyc_Absyn_SeqExp_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_SeqExp_e_Absyn_Raw_exp_struct*)_T183;_T18C= _T18D->f1;_T18B= _T18D->f2;}_LL10: {struct Cyc_Absyn_Exp*e1=_T18C;struct Cyc_Absyn_Exp*e2=_T18B;_T18C= e1;_T18B= e2;goto _LL12;}case 7:{struct Cyc_Absyn_And_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_And_e_Absyn_Raw_exp_struct*)_T183;_T18C= _T18D->f1;_T18B= _T18D->f2;}_LL12: {struct Cyc_Absyn_Exp*e1=_T18C;struct Cyc_Absyn_Exp*e2=_T18B;_T18C= e1;_T18B= e2;goto _LL14;}case 8:{struct Cyc_Absyn_Or_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Or_e_Absyn_Raw_exp_struct*)_T183;_T18C= _T18D->f1;_T18B= _T18D->f2;}_LL14: {struct Cyc_Absyn_Exp*e1=_T18C;struct Cyc_Absyn_Exp*e2=_T18B;_TE= e;_TF= & Cyc_InsertChecks_NoCheck_val;_T10= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_TF;
# 312
_TE->annot= (void*)_T10;
Cyc_InsertChecks_do_exp(nv,e1);
Cyc_InsertChecks_do_exp(nv,e2);goto _LL0;}case 6:{struct Cyc_Absyn_Conditional_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Conditional_e_Absyn_Raw_exp_struct*)_T183;_T18C= _T18D->f1;_T18B= _T18D->f2;_T18A= _T18D->f3;}{struct Cyc_Absyn_Exp*e1=_T18C;struct Cyc_Absyn_Exp*e2=_T18B;struct Cyc_Absyn_Exp*e3=_T18A;_T11= e;_T12= & Cyc_InsertChecks_NoCheck_val;_T13= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T12;
# 317
_T11->annot= (void*)_T13;
Cyc_InsertChecks_do_exp(nv,e1);
Cyc_InsertChecks_do_exp(nv,e2);
Cyc_InsertChecks_do_exp(nv,e3);goto _LL0;}case 5:{struct Cyc_Absyn_Increment_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Increment_e_Absyn_Raw_exp_struct*)_T183;_T18C= _T18D->f1;_T189= _T18D->f2;}{struct Cyc_Absyn_Exp*e2=_T18C;enum Cyc_Absyn_Incrementor incr=_T189;_T14= e;_T15= _T14->annot;_T16= & Cyc_InsertChecks_NoCheck_val;_T17= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T16;_T18= (void*)_T17;
# 328
if(_T15==_T18)goto _TL3C;_T19= e2;_T1A= _T19->topt;_T1B= _check_null(_T1A);_T1C= Cyc_Tcutil_get_type_bound(_T1B);if(_T1C==0)goto _TL3C;_T1D= e2;_T1E= _T1D->topt;_T1F= 
_check_null(_T1E);_T20= Cyc_Tcutil_is_zeroterm_pointer_type(_T1F);_T21= (int)_T20;
# 328
if(!_T21)goto _TL3C;_T22= 
Cyc_InsertChecks_need_null_check(nv,e2,e);_T23= (int)_T22;
# 328
if(!_T23)goto _TL3C;_T24=
# 330
Cyc_InsertChecks_need_null_check_vcgen(nv,e2);_T25= (int)_T24;
# 328
if(!_T25)goto _TL3C;_T26= Cyc_Flags_warn_all_null_deref;_T27= (int)_T26;
# 331
if(!_T27)goto _TL3E;_T28= e;_T29= _T28->loc;_T2A=
_tag_fat("inserted null check",sizeof(char),20U);_T2B= _tag_fat(0U,sizeof(void*),0);Cyc_Warn_warn(_T29,_T2A,_T2B);goto _TL3F;_TL3E: _TL3F: _T2C= e;_T2D= & Cyc_InsertChecks_NullOnly_val;_T2E= (struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*)_T2D;
_T2C->annot= (void*)_T2E;goto _TL3D;
# 335
_TL3C: _T2F= e;_T30= & Cyc_InsertChecks_NoCheck_val;_T31= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T30;_T2F->annot= (void*)_T31;_TL3D:
 Cyc_InsertChecks_do_exp(nv,e2);goto _LL0;}case 10:{struct Cyc_Absyn_FnCall_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_FnCall_e_Absyn_Raw_exp_struct*)_T183;_T18C= _T18D->f1;_T188= _T18D->f2;}{struct Cyc_Absyn_Exp*e1=_T18C;struct Cyc_List_List*es=_T188;_T32=
# 341
Cyc_InsertChecks_need_null_check(nv,e1,e);_T33= (int)_T32;if(!_T33)goto _TL40;_T34= Cyc_InsertChecks_need_null_check_vcgen(nv,e1);_T35= (int)_T34;if(!_T35)goto _TL40;_T36= e;_T37= & Cyc_InsertChecks_NullOnly_val;_T38= (struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*)_T37;
_T36->annot= (void*)_T38;goto _TL41;
# 344
_TL40: _T39= e;_T3A= & Cyc_InsertChecks_NoCheck_val;_T3B= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T3A;_T39->annot= (void*)_T3B;_TL41:
 Cyc_InsertChecks_do_exp(nv,e1);
_TL45: if(es!=0)goto _TL43;else{goto _TL44;}
_TL43: _T3C= nv;_T3D= es;_T3E= _T3D->hd;_T3F= (struct Cyc_Absyn_Exp*)_T3E;Cyc_InsertChecks_do_exp(_T3C,_T3F);_T40= es;
# 346
es= _T40->tl;goto _TL45;_TL44: goto _LL0;}case 14:{struct Cyc_Absyn_Cast_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Cast_e_Absyn_Raw_exp_struct*)_T183;_T41= _T18D->f1;_T18C= (void*)_T41;_T18B= _T18D->f2;_T187= _T18D->f3;_T186= _T18D->f4;}{void*t=_T18C;struct Cyc_Absyn_Exp*e1=_T18B;long user_inserted=_T187;enum Cyc_Absyn_Coercion coercion=_T186;{struct _tuple15 _T18D;_T43= e1;_T44= _T43->topt;_T45=
# 354
_check_null(_T44);({__typeof__(Cyc_Absyn_compress(_T45))_T18E=Cyc_Absyn_compress(_T45);_T18D.f0= _T18E;});({__typeof__(Cyc_Absyn_compress(t))_T18E=Cyc_Absyn_compress(t);_T18D.f1= _T18E;});_T42= _T18D;}{struct _tuple15 _T18D=_T42;struct Cyc_Absyn_PtrInfo _T18E;struct Cyc_Absyn_PtrInfo _T18F;_T46= _T18D.f0;_T47= (int*)_T46;_T48= *_T47;if(_T48!=4)goto _TL46;_T49= _T18D.f1;_T4A= (int*)_T49;_T4B= *_T4A;if(_T4B!=4)goto _TL48;_T4C= _T18D.f0;{struct Cyc_Absyn_PointerType_Absyn_Type_struct*_T190=(struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_T4C;_T18F= _T190->f1;}_T4D= _T18D.f1;{struct Cyc_Absyn_PointerType_Absyn_Type_struct*_T190=(struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_T4D;_T18E= _T190->f1;}{struct Cyc_Absyn_PtrInfo p1=_T18F;struct Cyc_Absyn_PtrInfo p2=_T18E;_T4E= (long)0;_T4F= p1;_T50= _T4F.ptr_atts;_T51= _T50.nullable;{
# 356
long n1=Cyc_Tcutil_force_type2bool(_T4E,_T51);_T52= (long)0;_T53= p2;_T54= _T53.ptr_atts;_T55= _T54.nullable;{
long n2=Cyc_Tcutil_force_type2bool(_T52,_T55);_T56=
Cyc_Absyn_bounds_one();_T57= p1;_T58= _T57.ptr_atts;_T59= _T58.bounds;_T5A= (long)1;{struct Cyc_Absyn_Exp*b1=Cyc_Tcutil_get_bounds_exp_constrain(_T56,_T59,_T5A);_T5B=
Cyc_Absyn_bounds_one();_T5C= p2;_T5D= _T5C.ptr_atts;_T5E= _T5D.bounds;_T5F= (long)1;{struct Cyc_Absyn_Exp*b2=Cyc_Tcutil_get_bounds_exp_constrain(_T5B,_T5E,_T5F);_T60= n1;_T61= (int)_T60;
# 362
if(!_T61)goto _TL4A;_T62= n2;_T63= (int)_T62;if(_T63)goto _TL4A;else{goto _TL4C;}_TL4C: _T64= coercion;_T65= (int)_T64;if(_T65==2)goto _TL4A;_T66= b1;_T67= (unsigned)_T66;if(_T67)goto _TL4D;else{goto _TL4E;}_TL4E: _T68= b2;_T69= (unsigned)_T68;if(_T69)goto _TL4D;else{goto _TL4A;}
_TL4D: _T6B= Cyc_Warn_impos_loc;{int(*_T190)(unsigned,struct _fat_ptr,struct _fat_ptr)=(int(*)(unsigned,struct _fat_ptr,struct _fat_ptr))_T6B;_T6A= _T190;}_T6C= e;_T6D= _T6C->loc;_T6E= _tag_fat("null-check conversion mis-classified",sizeof(char),37U);_T6F= _tag_fat(0U,sizeof(void*),0);_T6A(_T6D,_T6E,_T6F);goto _TL4B;
_TL4A: _T70= b1;_T71= (unsigned)_T70;if(!_T71)goto _TL4F;_T72= b2;_T73= (unsigned)_T72;if(_T73)goto _TL4F;else{goto _TL51;}_TL51: _T74= coercion;_T75= (int)_T74;if(_T75!=2)goto _TL4F;_T77= Cyc_Warn_impos_loc;{
int(*_T190)(unsigned,struct _fat_ptr,struct _fat_ptr)=(int(*)(unsigned,struct _fat_ptr,struct _fat_ptr))_T77;_T76= _T190;}_T78= e;_T79= _T78->loc;_T7A= _tag_fat("conversion mis-classified as null-check",sizeof(char),40U);_T7B= _tag_fat(0U,sizeof(void*),0);_T76(_T79,_T7A,_T7B);goto _TL50;_TL4F: _TL50: _TL4B:{struct _tuple16 _T190;
_T190.f0= b1;_T190.f1= b2;_T7C= _T190;}{struct _tuple16 _T190=_T7C;_T7D= _T190.f0;if(_T7D==0)goto _TL52;_T7E= _T190.f1;if(_T7E==0)goto _TL54;_T7F=
# 369
Cyc_Evexp_lte_const_exp(b2,b1);_T80= (int)_T7F;if(_T80)goto _TL56;else{goto _TL58;}
_TL58: _T82= Cyc_Warn_impos_loc;{int(*_T191)(unsigned,struct _fat_ptr,struct _fat_ptr)=(int(*)(unsigned,struct _fat_ptr,struct _fat_ptr))_T82;_T81= _T191;}_T83= e;_T84= _T83->loc;_T85= _tag_fat("InsertChecks: cast to maybe-shorter pointer type",sizeof(char),49U);_T86= _tag_fat(0U,sizeof(void*),0);_T81(_T84,_T85,_T86);goto _TL57;_TL56: _TL57: _T87= n1;_T88= (int)_T87;
# 372
if(!_T88)goto _TL59;_T89= n2;_T8A= (int)_T89;if(_T8A)goto _TL59;else{goto _TL5B;}_TL5B: _T8B= Cyc_InsertChecks_need_null_check(nv,e1,e);_T8C= (int)_T8B;if(!_T8C)goto _TL59;_T8D= Cyc_InsertChecks_need_null_check_vcgen(nv,e1);_T8E= (int)_T8D;if(!_T8E)goto _TL59;_T8F= user_inserted;_T90= (int)_T8F;
if(_T90)goto _TL5C;else{goto _TL5E;}
_TL5E: _T91= e;_T92= _T91->loc;_T93=
# 376
_tag_fat("inserted null check due to implicit cast from * to @ type",sizeof(char),58U);_T94= _tag_fat(0U,sizeof(void*),0);
# 374
Cyc_Warn_warn(_T92,_T93,_T94);goto _TL5D;_TL5C: _TL5D: _T95= e;_T96= & Cyc_InsertChecks_NullOnly_val;_T97= (struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*)_T96;
# 377
_T95->annot= (void*)_T97;goto _TL5A;
# 379
_TL59: _T98= e;_T99= & Cyc_InsertChecks_NoCheck_val;_T9A= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T99;_T98->annot= (void*)_T9A;_TL5A: goto _LL5C;_TL54: _T9B=
# 383
_check_null(b1);_T9C= Cyc_Evexp_c_can_eval(_T9B);_T9D= (int)_T9C;if(_T9D)goto _TL5F;else{goto _TL61;}
_TL61: _T9E= e;_T9F= _T9E->loc;_TA0= _tag_fat("cannot perform coercion: numelts is statically unknown",sizeof(char),55U);_TA1= _tag_fat(0U,sizeof(void*),0);Cyc_Warn_err(_T9F,_TA0,_TA1);goto _TL60;_TL5F: _TL60: _TA2= e;_TA3= & Cyc_InsertChecks_NoCheck_val;_TA4= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_TA3;
_TA2->annot= (void*)_TA4;goto _LL5C;_TL52: _TA5= _T190.f1;if(_TA5==0)goto _TL62;_TA6= nv;_TA7= _TA6.at_toplevel;_TA8= (int)_TA7;
# 388
if(!_TA8)goto _TL64;_TA9= e;_TAA= _TA9->loc;_TAB=
_tag_fat("cannot coerce fat-pointer to thin-pointer at toplevel",sizeof(char),54U);_TAC= _tag_fat(0U,sizeof(void*),0);Cyc_Warn_err(_TAA,_TAB,_TAC);goto _TL65;_TL64: _TL65:{
# 393
struct _tuple14 _T191=Cyc_InsertChecks_need_bound_null_check_annot(e);long _T192;long _T193;_T193= _T191.f0;_T192= _T191.f1;{long bds_ck=_T193;long null_ck=_T192;_TAD= bds_ck;_TAE= (int)_TAD;
if(!_TAE)goto _TL66;
bds_ck= Cyc_InsertChecks_need_bounds_check_vcgen(nv,e1);goto _TL67;_TL66: _TL67: _TAF= bds_ck;_TB0= (int)_TAF;
# 398
if(!_TB0)goto _TL68;_TB1=
_check_null(b2);_TB2= Cyc_Evexp_c_can_eval(_TB1);_TB3= (int)_TB2;if(_TB3)goto _TL6A;else{goto _TL6C;}
_TL6C: _TB4= e;_TB5= _TB4->loc;_TB6= _tag_fat("cannot perform coercion: numelts is statically unknown",sizeof(char),55U);_TB7= _tag_fat(0U,sizeof(void*),0);Cyc_Warn_err(_TB5,_TB6,_TB7);goto _TL6B;_TL6A: _TL6B: goto _TL69;_TL68: _TL69: _TB8= null_ck;_TB9= (int)_TB8;
# 402
if(!_TB9)goto _TL6D;_TBA= n2;_TBB= (int)_TBA;if(_TBB)goto _TL6D;else{goto _TL6F;}_TL6F: _TBC= Cyc_InsertChecks_need_null_check(nv,e1,e);_TBD= (int)_TBC;if(!_TBD)goto _TL6D;_TBE= Cyc_InsertChecks_need_null_check_vcgen(nv,e);_TBF= (int)_TBE;if(!_TBF)goto _TL6D;_TC0= user_inserted;_TC1= (int)_TC0;
if(_TC1)goto _TL70;else{goto _TL72;}
_TL72: _TC2= e;_TC3= _TC2->loc;_TC4=
# 406
_tag_fat("inserted null check due to implicit cast from *@fat (?) to @ type",sizeof(char),66U);_TC5= _tag_fat(0U,sizeof(void*),0);
# 404
Cyc_Warn_warn(_TC3,_TC4,_TC5);goto _TL71;_TL70: _TL71: _TC6= e;_TC8= bds_ck;_TC9= (int)_TC8;
# 407
if(!_TC9)goto _TL73;_TCA= & Cyc_InsertChecks_NullAndFatBound_val;_TCB= (struct Cyc_InsertChecks_NullAndFatBound_Absyn_AbsynAnnot_struct*)_TCA;_TC7= (void*)_TCB;goto _TL74;_TL73: _TCC= & Cyc_InsertChecks_NullOnly_val;_TCD= (struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*)_TCC;_TC7= (void*)_TCD;_TL74: _TC6->annot= _TC7;goto _TL6E;
# 409
_TL6D: _TCE= e;_TD0= bds_ck;_TD1= (int)_TD0;if(!_TD1)goto _TL75;_TD2= & Cyc_InsertChecks_FatBound_val;_TD3= (struct Cyc_InsertChecks_FatBound_Absyn_AbsynAnnot_struct*)_TD2;_TCF= (void*)_TD3;goto _TL76;_TL75: _TD4= & Cyc_InsertChecks_NoCheck_val;_TD5= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_TD4;_TCF= (void*)_TD5;_TL76: _TCE->annot= _TCF;_TL6E: goto _LL5C;}}_TL62: _TD6= e;_TD7= & Cyc_InsertChecks_NoCheck_val;_TD8= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_TD7;
# 412
_TD6->annot= (void*)_TD8;goto _LL5C;_LL5C:;}goto _LL57;}}}}}_TL48: goto _LL5A;_TL46: _LL5A: _TD9= e;_TDA= & Cyc_InsertChecks_NoCheck_val;_TDB= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_TDA;
# 417
_TD9->annot= (void*)_TDB;goto _LL57;_LL57:;}
# 420
Cyc_InsertChecks_do_exp(nv,e1);goto _LL0;}case 22:{struct Cyc_Absyn_AggrArrow_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_AggrArrow_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;}{struct Cyc_Absyn_Exp*e1=_T18B;_T18B= e1;goto _LL20;}case 20:{struct Cyc_Absyn_Deref_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Deref_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;}_LL20: {struct Cyc_Absyn_Exp*e1=_T18B;_T18B= e1;_T18A=
# 426
Cyc_InsertChecks_shared_zero_exp();goto _LL22;}case 23:{struct Cyc_Absyn_Subscript_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Subscript_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;_T18A= _T18D->f2;}_LL22: {struct Cyc_Absyn_Exp*e1=_T18B;struct Cyc_Absyn_Exp*e2=_T18A;_TDC= e1;_TDD= _TDC->topt;_TDE=
# 428
_check_null(_TDD);{void*_T18D=Cyc_Absyn_compress(_TDE);void*_T18E;void*_T18F;void*_T190;void*_T191;struct Cyc_Absyn_Tqual _T192;void*_T193;_TDF= (int*)_T18D;_TE0= *_TDF;if(_TE0!=4)goto _TL77;{struct Cyc_Absyn_PointerType_Absyn_Type_struct*_T194=(struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_T18D;_TE1= _T194->f1;_T193= _TE1.elt_type;_TE2= _T194->f1;_T192= _TE2.elt_tq;_TE3= _T194->f1;_TE4= _TE3.ptr_atts;_T191= _TE4.eff;_TE5= _T194->f1;_TE6= _TE5.ptr_atts;_T190= _TE6.nullable;_TE7= _T194->f1;_TE8= _TE7.ptr_atts;_T18F= _TE8.bounds;_TE9= _T194->f1;_TEA= _TE9.ptr_atts;_T18E= _TEA.zero_term;}{void*ta=_T193;struct Cyc_Absyn_Tqual tq=_T192;void*rgn=_T191;void*n=_T190;void*b=_T18F;void*zt=_T18E;
# 430
struct _tuple14 _T194=Cyc_InsertChecks_need_bound_null_check_annot(e);long _T195;long _T196;_T196= _T194.f0;_T195= _T194.f1;{long bds_ck=_T196;long null_ck=_T195;_TEB= bds_ck;_TEC= (int)_TEB;
# 433
if(!_TEC)goto _TL79;
bds_ck= Cyc_InsertChecks_simple_need_bounds_check(nv,e1,e2);goto _TL7A;_TL79: _TL7A: _TED= bds_ck;_TEE= (int)_TED;
if(!_TEE)goto _TL7B;_TEF= nv;_TF0= _TEF.vcgen_r;_TF1= (int)_TF0;if(!_TF1)goto _TL7B;
# 437
bds_ck= Cyc_InsertChecks_need_bounds_check_vcgen(nv,e1);goto _TL7C;_TL7B: _TL7C: _TF2= e2;_TF3= _TF2->topt;
# 440
if(_TF3==0)goto _TL7D;_TF4= e2;_TF5= _TF4->topt;{void*_T197=Cyc_Absyn_compress(_TF5);_TF6= (int*)_T197;_TF7= *_TF6;if(_TF7!=0)goto _TL7F;_TF8= (struct Cyc_Absyn_AppType_Absyn_Type_struct*)_T197;_TF9= _TF8->f1;_TFA= (int*)_TF9;_TFB= *_TFA;if(_TFB!=1)goto _TL81;_TFC= (struct Cyc_Absyn_AppType_Absyn_Type_struct*)_T197;_TFD= _TFC->f1;_TFE= (struct Cyc_Absyn_IntCon_Absyn_TyCon_struct*)_TFD;_TFF= _TFE->f2;_T100= (int)_TFF;switch(_T100){case Cyc_Absyn_Long_sz:
# 442
 if(sizeof(long)<= sizeof(int))goto _TL84;bds_ck= (long)1;goto _TL85;_TL84: _TL85: goto _LL70;case Cyc_Absyn_LongLong_sz: goto _LL76;case Cyc_Absyn_Int128_sz: _LL76:
# 446
 bds_ck= (long)1;goto _LL70;default: goto _LL77;}goto _TL82;_TL81: goto _LL77;_TL82: goto _TL80;_TL7F: _LL77: goto _LL70;_TL80: _LL70:;}goto _TL7E;_TL7D: _TL7E: _T101= bds_ck;_T102= (int)_T101;
# 450
if(!_T102)goto _TL86;_T103= Cyc_Flags_warn_bounds_checks;_T104= (int)_T103;if(!_T104)goto _TL86;{struct Cyc_String_pa_PrintArg_struct _T197;_T197.tag= 0;
({__typeof__(Cyc_Absynpp_exp2string(e))_T198=Cyc_Absynpp_exp2string(e);_T197.f1= _T198;});_T105= _T197;}{struct Cyc_String_pa_PrintArg_struct _T197=_T105;void*_T198[1];_T198[0]= & _T197;_T106= e1;_T107= _T106->loc;_T108= _tag_fat("inserted bounds check at %s",sizeof(char),28U);_T109= _tag_fat(_T198,sizeof(void*),1);Cyc_Warn_warn(_T107,_T108,_T109);}goto _TL87;_TL86: _TL87: _T10A=
Cyc_Absyn_bounds_one();_T10B= b;_T10C= (long)1;{struct Cyc_Absyn_Exp*bd=Cyc_Tcutil_get_bounds_exp_constrain(_T10A,_T10B,_T10C);_T10D= bd==0;{
long fat_ptr=(long)_T10D;_T10E= null_ck;_T10F= (int)_T10E;
# 457
if(!_T10F)goto _TL88;_T111=
# 464
Cyc_InsertChecks_need_null_check(nv,e1,e);_T112= (int)_T111;if(!_T112)goto _TL8A;_T113= Cyc_InsertChecks_need_null_check_vcgen(nv,e1);_T110= (int)_T113;goto _TL8B;_TL8A: _T110= 0;_TL8B: null_ck= (long)_T110;_T114= null_ck;_T115= (int)_T114;
if(!_T115)goto _TL8C;_T116= Cyc_Flags_warn_all_null_deref;_T117= (int)_T116;if(!_T117)goto _TL8C;_T118= e;_T119= _T118->loc;_T11A=
_tag_fat("inserted null check",sizeof(char),20U);_T11B= _tag_fat(0U,sizeof(void*),0);Cyc_Warn_warn(_T119,_T11A,_T11B);goto _TL8D;_TL8C: _TL8D: goto _TL89;_TL88: _TL89: _T11C= bds_ck;_T11D= (int)_T11C;
# 471
if(!_T11D)goto _TL8E;_T11E= bd;_T11F= (unsigned)_T11E;
if(!_T11F)goto _TL90;_T120=
# 474
Cyc_Evexp_c_can_eval(bd);_T121= (int)_T120;if(_T121)goto _TL92;else{goto _TL94;}
# 478
_TL94: _T122= nv;_T123= _T122.assn_info;_T124= e1;{struct Cyc_Vcgen_ExpChecks*ec=Cyc_Vcgen_exp2ctxt_checks(_T123,_T124);_T125= e;_T126= _T125->loc;{struct Cyc_String_pa_PrintArg_struct _T197;_T197.tag= 0;_T129= ec;_T12A= _T129->bounds_check;
# 480
({__typeof__(Cyc_AssnDef_assn2string(_T12A))_T198=Cyc_AssnDef_assn2string(_T12A);_T197.f1= _T198;});_T128= _T197;}{struct Cyc_String_pa_PrintArg_struct _T197=_T128;void*_T198[1];_T198[0]= & _T197;_T12B=
# 479
_tag_fat("cannot determine subscript is in bounds (%s)",sizeof(char),45U);_T12C= _tag_fat(_T198,sizeof(void*),1);_T127= Cyc_aprintf(_T12B,_T12C);}_T12D= _tag_fat(0U,sizeof(void*),0);Cyc_Warn_err(_T126,_T127,_T12D);{struct Cyc_String_pa_PrintArg_struct _T197;_T197.tag= 0;_T12F= ec;_T130= _T12F->ctxt;
# 481
({__typeof__(Cyc_AssnDef_assnmap2string(_T130))_T198=Cyc_AssnDef_assnmap2string(_T130);_T197.f1= _T198;});_T12E= _T197;}{struct Cyc_String_pa_PrintArg_struct _T197=_T12E;void*_T198[1];_T198[0]= & _T197;_T131= Cyc_stderr;_T132= _tag_fat("[all that I can prove is %s",sizeof(char),28U);_T133= _tag_fat(_T198,sizeof(void*),1);Cyc_fprintf(_T131,_T132,_T133);}_T134= Cyc_stderr;_T135=
_tag_fat("]\n",sizeof(char),3U);_T136= _tag_fat(0U,sizeof(void*),0);Cyc_fprintf(_T134,_T135,_T136);}goto _TL93;_TL92: _TL93: _T137= e;_T139= null_ck;_T13A= (int)_T139;
# 484
if(!_T13A)goto _TL95;{struct Cyc_InsertChecks_NullAndThinBound_Absyn_AbsynAnnot_struct*_T197=_cycalloc(sizeof(struct Cyc_InsertChecks_NullAndThinBound_Absyn_AbsynAnnot_struct));_T197->tag= Cyc_InsertChecks_NullAndThinBound;_T197->f1= bd;_T13B= (struct Cyc_InsertChecks_NullAndThinBound_Absyn_AbsynAnnot_struct*)_T197;}_T138= (void*)_T13B;goto _TL96;_TL95:{struct Cyc_InsertChecks_ThinBound_Absyn_AbsynAnnot_struct*_T197=_cycalloc(sizeof(struct Cyc_InsertChecks_ThinBound_Absyn_AbsynAnnot_struct));_T197->tag= Cyc_InsertChecks_ThinBound;_T197->f1= bd;_T13C= (struct Cyc_InsertChecks_ThinBound_Absyn_AbsynAnnot_struct*)_T197;}_T138= (void*)_T13C;_TL96: _T137->annot= _T138;goto _TL91;
# 486
_TL90: _T13D= e;_T13F= null_ck;_T140= (int)_T13F;if(!_T140)goto _TL97;_T141= & Cyc_InsertChecks_NullAndFatBound_val;_T142= (struct Cyc_InsertChecks_NullAndFatBound_Absyn_AbsynAnnot_struct*)_T141;_T13E= (void*)_T142;goto _TL98;_TL97: _T143= & Cyc_InsertChecks_FatBound_val;_T144= (struct Cyc_InsertChecks_FatBound_Absyn_AbsynAnnot_struct*)_T143;_T13E= (void*)_T144;_TL98: _T13D->annot= _T13E;_TL91: goto _TL8F;
_TL8E: _T145= null_ck;_T146= (int)_T145;if(!_T146)goto _TL99;_T147= e;_T148= & Cyc_InsertChecks_NullOnly_val;_T149= (struct Cyc_InsertChecks_NullOnly_Absyn_AbsynAnnot_struct*)_T148;
_T147->annot= (void*)_T149;goto _TL9A;
# 490
_TL99: _T14A= e;_T14B= & Cyc_InsertChecks_NoCheck_val;_T14C= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T14B;_T14A->annot= (void*)_T14C;_TL9A: _TL8F: goto _LL68;}}}}_TL77: _T14E= Cyc_Warn_impos_loc;{
# 493
int(*_T194)(unsigned,struct _fat_ptr,struct _fat_ptr)=(int(*)(unsigned,struct _fat_ptr,struct _fat_ptr))_T14E;_T14D= _T194;}_T14F= e;_T150= _T14F->loc;_T151= _tag_fat("InsertChecks: non-pointer-type on dereference",sizeof(char),46U);_T152= _tag_fat(0U,sizeof(void*),0);_T14D(_T150,_T151,_T152);_LL68:;}
# 495
Cyc_InsertChecks_do_exp(nv,e1);
Cyc_InsertChecks_do_exp(nv,e2);goto _LL0;}case 18:{struct Cyc_Absyn_Sizeofexp_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Sizeofexp_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;}{struct Cyc_Absyn_Exp*e1=_T18B;_T153= e;_T154= & Cyc_InsertChecks_NoCheck_val;_T155= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T154;
# 500
_T153->annot= (void*)_T155;_T156=
Cyc_InsertChecks_enter_sizeof(nv);_T157= e1;Cyc_InsertChecks_do_exp(_T156,_T157);goto _LL0;}case 16:{struct Cyc_Absyn_New_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_New_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;_T18A= _T18D->f2;_T185= _T18D->f3;}{struct Cyc_Absyn_Exp*rgnopt=_T18B;struct Cyc_Absyn_Exp*e1=_T18A;struct Cyc_Absyn_Exp*qualopt=_T185;_T158= e;_T159= & Cyc_InsertChecks_NoCheck_val;_T15A= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T159;
# 505
_T158->annot= (void*)_T15A;
Cyc_InsertChecks_do_exp_opt(nv,rgnopt);
Cyc_InsertChecks_do_exp_opt(nv,qualopt);
Cyc_InsertChecks_do_exp(nv,e1);goto _LL0;}case 33:{struct Cyc_Absyn_Malloc_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Malloc_e_Absyn_Raw_exp_struct*)_T183;_T15B= _T18D->f1;_T18B= _T15B.rgn;_T15C= _T18D->f1;_T18A= _T15C.aqual;_T15D= _T18D->f1;_T185= _T15D.num_elts;}{struct Cyc_Absyn_Exp*rgnopt=_T18B;struct Cyc_Absyn_Exp*aqopt=_T18A;struct Cyc_Absyn_Exp*sizeexp=_T185;_T15E= e;_T15F= & Cyc_InsertChecks_NoCheck_val;_T160= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T15F;
# 513
_T15E->annot= (void*)_T160;
Cyc_InsertChecks_do_exp_opt(nv,rgnopt);
Cyc_InsertChecks_do_exp_opt(nv,aqopt);
Cyc_InsertChecks_do_exp(nv,sizeexp);goto _LL0;}case 41:{struct Cyc_Absyn_Assert_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Assert_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;}{struct Cyc_Absyn_Exp*e1=_T18B;_T18B= e1;goto _LL2C;}case 40:{struct Cyc_Absyn_Extension_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Extension_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;}_LL2C: {struct Cyc_Absyn_Exp*e1=_T18B;_T18B= e1;goto _LL2E;}case 11:{struct Cyc_Absyn_Throw_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Throw_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;}_LL2E: {struct Cyc_Absyn_Exp*e1=_T18B;_T18B= e1;goto _LL30;}case 12:{struct Cyc_Absyn_NoInstantiate_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_NoInstantiate_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;}_LL30: {struct Cyc_Absyn_Exp*e1=_T18B;_T18B= e1;goto _LL32;}case 13:{struct Cyc_Absyn_Instantiate_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Instantiate_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;}_LL32: {struct Cyc_Absyn_Exp*e1=_T18B;_T18B= e1;goto _LL34;}case 15:{struct Cyc_Absyn_Address_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Address_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;}_LL34: {struct Cyc_Absyn_Exp*e1=_T18B;_T18B= e1;goto _LL36;}case 42:{struct Cyc_Absyn_Assert_false_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Assert_false_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;}_LL36: {struct Cyc_Absyn_Exp*e1=_T18B;_T161= e;_T162= & Cyc_InsertChecks_NoCheck_val;_T163= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T162;
# 532
_T161->annot= (void*)_T163;
Cyc_InsertChecks_do_exp(nv,e1);goto _LL0;}case 31: goto _LL3A;case 32: _LL3A: goto _LL3C;case 17: _LL3C: goto _LL3E;case 19: _LL3E: _T164= e;_T165= & Cyc_InsertChecks_NoCheck_val;_T166= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T165;
# 539
_T164->annot= (void*)_T166;goto _LL0;case 21:{struct Cyc_Absyn_AggrMember_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_AggrMember_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;}{struct Cyc_Absyn_Exp*e1=_T18B;_T18B= e1;goto _LL42;}case 37:{struct Cyc_Absyn_Tagcheck_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Tagcheck_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;}_LL42: {struct Cyc_Absyn_Exp*e1=_T18B;_T18B= e1;goto _LL44;}case 27:{struct Cyc_Absyn_ComprehensionNoinit_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_ComprehensionNoinit_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;}_LL44: {struct Cyc_Absyn_Exp*e1=_T18B;_T167= e;_T168= & Cyc_InsertChecks_NoCheck_val;_T169= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T168;
# 546
_T167->annot= (void*)_T169;
Cyc_InsertChecks_do_exp(nv,e1);goto _LL0;}case 26:{struct Cyc_Absyn_Comprehension_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Comprehension_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f2;_T18A= _T18D->f3;}{struct Cyc_Absyn_Exp*e1=_T18B;struct Cyc_Absyn_Exp*e2=_T18A;_T18B= e1;_T18A= e2;goto _LL48;}case 34:{struct Cyc_Absyn_Swap_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Swap_e_Absyn_Raw_exp_struct*)_T183;_T18B= _T18D->f1;_T18A= _T18D->f2;}_LL48: {struct Cyc_Absyn_Exp*e1=_T18B;struct Cyc_Absyn_Exp*e2=_T18A;_T16A= e;_T16B= & Cyc_InsertChecks_NoCheck_val;_T16C= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T16B;
# 552
_T16A->annot= (void*)_T16C;
Cyc_InsertChecks_do_exp(nv,e1);
Cyc_InsertChecks_do_exp(nv,e2);goto _LL0;}case 25:{struct Cyc_Absyn_Array_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Array_e_Absyn_Raw_exp_struct*)_T183;_T188= _T18D->f1;}{struct Cyc_List_List*dles=_T188;_T188= dles;goto _LL4C;}case 28:{struct Cyc_Absyn_Aggregate_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_Aggregate_e_Absyn_Raw_exp_struct*)_T183;_T188= _T18D->f3;}_LL4C: {struct Cyc_List_List*dles=_T188;_T188= dles;goto _LL4E;}case 29:{struct Cyc_Absyn_AnonStruct_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_AnonStruct_e_Absyn_Raw_exp_struct*)_T183;_T188= _T18D->f3;}_LL4E: {struct Cyc_List_List*dles=_T188;_T16D= e;_T16E= & Cyc_InsertChecks_NoCheck_val;_T16F= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T16E;
# 561
_T16D->annot= (void*)_T16F;
_TL9E: if(dles!=0)goto _TL9C;else{goto _TL9D;}
_TL9C: _T170= nv;_T171= dles;_T172= _T171->hd;_T173= (struct _tuple17*)_T172;_T174= *_T173;_T175= _T174.f1;Cyc_InsertChecks_do_exp(_T170,_T175);_T176= dles;
# 562
dles= _T176->tl;goto _TL9E;_TL9D: goto _LL0;}case 36:{struct Cyc_Absyn_StmtExp_e_Absyn_Raw_exp_struct*_T18D=(struct Cyc_Absyn_StmtExp_e_Absyn_Raw_exp_struct*)_T183;_T184= _T18D->f1;}{struct Cyc_Absyn_Stmt*s=_T184;_T177= e;_T178= & Cyc_InsertChecks_NoCheck_val;_T179= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T178;
# 566
_T177->annot= (void*)_T179;
Cyc_InsertChecks_do_stmt(nv,s);goto _LL0;}case 38: _T17A= e;_T17B= & Cyc_InsertChecks_NoCheck_val;_T17C= (struct Cyc_InsertChecks_NoCheck_Absyn_AbsynAnnot_struct*)_T17B;
# 570
_T17A->annot= (void*)_T17C;goto _LL0;case 35: goto _LL56;default: _LL56: _T17E= Cyc_Warn_impos_loc;{
# 574
int(*_T18D)(unsigned,struct _fat_ptr,struct _fat_ptr)=(int(*)(unsigned,struct _fat_ptr,struct _fat_ptr))_T17E;_T17D= _T18D;}_T17F= e;_T180= _T17F->loc;_T181= _tag_fat("InsertChecks, unexpected exp form",sizeof(char),34U);_T182= _tag_fat(0U,sizeof(void*),0);_T17D(_T180,_T181,_T182);}_LL0:;}}
# 578
static void Cyc_InsertChecks_do_fndecl(struct Cyc_InsertChecks_Env env,struct Cyc_Absyn_Fndecl*fd){struct Cyc_Absyn_Fndecl*_T0;long _T1;int _T2;struct Cyc_InsertChecks_Env _T3;long _T4;int _T5;struct Cyc_Hashtable_Table**_T6;struct Cyc_Hashtable_Table*(*_T7)(int,int(*)(struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*),int(*)(struct Cyc_Absyn_Exp*));struct Cyc_Hashtable_Table*(*_T8)(int,int(*)(void*,void*),int(*)(void*));int(*_T9)(struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*);int(*_TA)(void*,void*);int(*_TB)(struct Cyc_Absyn_Exp*);struct Cyc_InsertChecks_Env _TC;long _TD;int _TE;struct Cyc_InsertChecks_Env _TF;long _T10;int _T11;struct Cyc_InsertChecks_Env _T12;unsigned _T13;int(*_T14)(struct _fat_ptr,struct _fat_ptr);void*(*_T15)(struct _fat_ptr,struct _fat_ptr);struct _fat_ptr _T16;struct _fat_ptr _T17;struct Cyc_InsertChecks_Env _T18;struct Cyc_JumpAnalysis_Jump_Anal_Result*_T19;struct Cyc_Absyn_Fndecl*_T1A;struct Cyc_InsertChecks_Env _T1B;struct Cyc_Hashtable_Table**_T1C;struct Cyc_InsertChecks_Env _T1D;struct Cyc_Hashtable_Table*_T1E;struct Cyc_InsertChecks_Env _T1F;long _T20;struct Cyc_InsertChecks_Env _T21;struct Cyc_Absyn_Fndecl*_T22;struct Cyc_Absyn_Stmt*_T23;_T0= fd;_T1= _T0->escapes;_T2= (int)_T1;
# 581
if(!_T2)goto _TL9F;_T3= env;_T4= _T3.use_precond;_T5= (int)_T4;if(!_T5)goto _TL9F;
return;
# 584
_TL9F:{struct Cyc_InsertChecks_Env nv=Cyc_InsertChecks_fn_body_env(env);{struct Cyc_Hashtable_Table**_T24=_cycalloc(sizeof(struct Cyc_Hashtable_Table*));_T8= Cyc_Hashtable_create;{
struct Cyc_Hashtable_Table*(*_T25)(int,int(*)(struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*),int(*)(struct Cyc_Absyn_Exp*))=(struct Cyc_Hashtable_Table*(*)(int,int(*)(struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*),int(*)(struct Cyc_Absyn_Exp*)))_T8;_T7= _T25;}_TA= Cyc_Core_ptrcmp;{int(*_T25)(struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*)=(int(*)(struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*))_TA;_T9= _T25;}{int(*_T25)(struct Cyc_Absyn_Exp*)=(int(*)(struct Cyc_Absyn_Exp*))Cyc_InsertChecks_hash_ptr;_TB= _T25;}({__typeof__(_T7(57,_T9,_TB))_T25=_T7(57,_T9,_TB);*_T24= _T25;});_T6= (struct Cyc_Hashtable_Table**)_T24;}nv.assn_info= _T6;_TC= nv;_TD= _TC.vcgen_r;_TE= (int)_TD;
if(!_TE)goto _TLA1;_TF= env;_T10= _TF.use_precond;_T11= (int)_T10;
# 595 "library/compiler/insert_checks.cyc"
if(!_T11)goto _TLA3;_T12= env;_T13= _T12.pass_counter;if(_T13 > 1U)goto _TLA3;_T15= Cyc_Warn_impos;{
int(*_T24)(struct _fat_ptr,struct _fat_ptr)=(int(*)(struct _fat_ptr,struct _fat_ptr))_T15;_T14= _T24;}_T16= _tag_fat("Trying to use preconditions in vcgen at the first pass\n",sizeof(char),56U);_T17= _tag_fat(0U,sizeof(void*),0);_T14(_T16,_T17);goto _TLA4;_TLA3: _TLA4: _T18= nv;_T19= _T18.tables;_T1A= fd;_T1B= nv;_T1C= _T1B.assn_info;_T1D= nv;_T1E= _T1D.fn_precond_info;_T1F= nv;_T20= _T1F.use_precond;
Cyc_Vcgen_vcgen_fundecl(0U,_T19,_T1A,_T1C,_T1E,_T20);goto _TLA2;_TLA1: _TLA2: _T21= nv;_T22= fd;_T23= _T22->body;
# 614 "library/compiler/insert_checks.cyc"
Cyc_InsertChecks_do_stmt(_T21,_T23);
Cyc_AssnDef_reset_hash_cons_table();};}
# 632 "library/compiler/insert_checks.cyc"
static void Cyc_InsertChecks_do_decl(struct Cyc_InsertChecks_Env nv,struct Cyc_Absyn_Decl*d){struct Cyc_Absyn_Decl*_T0;int*_T1;unsigned _T2;struct Cyc_InsertChecks_Env _T3;struct Cyc_Absyn_Vardecl*_T4;struct Cyc_Absyn_Exp*_T5;struct Cyc_InsertChecks_Env _T6;struct Cyc_List_List*_T7;void*_T8;struct Cyc_Absyn_Decl*_T9;struct Cyc_List_List*_TA;_T0= d;{
void*_TB=_T0->r;struct Cyc_List_List*_TC;struct Cyc_Absyn_Exp*_TD;struct Cyc_Absyn_Fndecl*_TE;struct Cyc_Absyn_Vardecl*_TF;_T1= (int*)_TB;_T2= *_T1;switch(_T2){case 0:{struct Cyc_Absyn_Var_d_Absyn_Raw_decl_struct*_T10=(struct Cyc_Absyn_Var_d_Absyn_Raw_decl_struct*)_TB;_TF= _T10->f1;}{struct Cyc_Absyn_Vardecl*vd=_TF;_T3= nv;_T4= vd;_T5= _T4->initializer;
# 635
Cyc_InsertChecks_do_exp_opt(_T3,_T5);goto _LL0;}case 1:{struct Cyc_Absyn_Fn_d_Absyn_Raw_decl_struct*_T10=(struct Cyc_Absyn_Fn_d_Absyn_Raw_decl_struct*)_TB;_TE= _T10->f1;}{struct Cyc_Absyn_Fndecl*fd=_TE;goto _LL0;}case 2:{struct Cyc_Absyn_Let_d_Absyn_Raw_decl_struct*_T10=(struct Cyc_Absyn_Let_d_Absyn_Raw_decl_struct*)_TB;_TD= _T10->f3;}{struct Cyc_Absyn_Exp*e=_TD;
# 640
Cyc_InsertChecks_do_exp(nv,e);goto _LL0;}case 3: goto _LL0;case 4:{struct Cyc_Absyn_Region_d_Absyn_Raw_decl_struct*_T10=(struct Cyc_Absyn_Region_d_Absyn_Raw_decl_struct*)_TB;_TD= _T10->f3;}{struct Cyc_Absyn_Exp*eo=_TD;
# 645
Cyc_InsertChecks_do_exp_opt(nv,eo);goto _LL0;}case 5: goto _LLE;case 6: _LLE: goto _LL10;case 7: _LL10: goto _LL12;case 8: _LL12: goto _LL14;case 13: _LL14: goto _LL16;case 14: _LL16: goto _LL18;case 15: _LL18: goto _LL1A;case 16: _LL1A: goto _LL1C;case 12: _LL1C: goto _LL0;case 11:{struct Cyc_Absyn_ExternC_d_Absyn_Raw_decl_struct*_T10=(struct Cyc_Absyn_ExternC_d_Absyn_Raw_decl_struct*)_TB;_TC= _T10->f1;}{struct Cyc_List_List*ds=_TC;_TC= ds;goto _LL20;}case 10:{struct Cyc_Absyn_Using_d_Absyn_Raw_decl_struct*_T10=(struct Cyc_Absyn_Using_d_Absyn_Raw_decl_struct*)_TB;_TC= _T10->f2;}_LL20: {struct Cyc_List_List*ds=_TC;_TC= ds;goto _LL22;}default:{struct Cyc_Absyn_Namespace_d_Absyn_Raw_decl_struct*_T10=(struct Cyc_Absyn_Namespace_d_Absyn_Raw_decl_struct*)_TB;_TC= _T10->f2;}_LL22: {struct Cyc_List_List*ds=_TC;
# 662
_TLA9: if(ds!=0)goto _TLA7;else{goto _TLA8;}
_TLA7: _T6= nv;_T7= ds;_T8= _T7->hd;_T9= (struct Cyc_Absyn_Decl*)_T8;Cyc_InsertChecks_do_decl(_T6,_T9);_TA= ds;
# 662
ds= _TA->tl;goto _TLA9;_TLA8:;}}_LL0:;}}
# 667
static void Cyc_InsertChecks_do_scs(struct Cyc_InsertChecks_Env nv,struct Cyc_List_List*scs){struct Cyc_List_List*_T0;void*_T1;struct Cyc_Absyn_Switch_clause*_T2;struct Cyc_Absyn_Exp*_T3;struct Cyc_InsertChecks_Env _T4;struct Cyc_List_List*_T5;void*_T6;struct Cyc_Absyn_Switch_clause*_T7;struct Cyc_Absyn_Exp*_T8;struct Cyc_InsertChecks_Env _T9;struct Cyc_List_List*_TA;void*_TB;struct Cyc_Absyn_Switch_clause*_TC;struct Cyc_Absyn_Stmt*_TD;struct Cyc_List_List*_TE;
# 670
_TLAD: if(scs!=0)goto _TLAB;else{goto _TLAC;}
_TLAB: _T0= scs;_T1= _T0->hd;_T2= (struct Cyc_Absyn_Switch_clause*)_T1;_T3= _T2->where_clause;if(_T3==0)goto _TLAE;_T4= nv;_T5= scs;_T6= _T5->hd;_T7= (struct Cyc_Absyn_Switch_clause*)_T6;_T8= _T7->where_clause;
Cyc_InsertChecks_do_exp(_T4,_T8);goto _TLAF;_TLAE: _TLAF: _T9= nv;_TA= scs;_TB= _TA->hd;_TC= (struct Cyc_Absyn_Switch_clause*)_TB;_TD= _TC->body;
Cyc_InsertChecks_do_stmt(_T9,_TD);_TE= scs;
# 670
scs= _TE->tl;goto _TLAD;_TLAC:;}
# 677
static void Cyc_InsertChecks_do_stmt(struct Cyc_InsertChecks_Env nv,struct Cyc_Absyn_Stmt*s){struct Cyc_Absyn_Stmt*_T0;int*_T1;unsigned _T2;struct Cyc_Absyn_Exp*_T3;unsigned _T4;struct _tuple9 _T5;struct _tuple9 _T6;struct _tuple9 _T7;struct _tuple9 _T8;struct Cyc_InsertChecks_Env _T9;struct Cyc_List_List*_TA;void*_TB;struct Cyc_Absyn_Exp*_TC;struct Cyc_List_List*_TD;_T0= s;{
void*_TE=_T0->r;struct Cyc_Absyn_Decl*_TF;struct Cyc_List_List*_T10;struct Cyc_Absyn_Exp*_T11;struct Cyc_Absyn_Exp*_T12;struct Cyc_Absyn_Stmt*_T13;struct Cyc_Absyn_Stmt*_T14;struct Cyc_Absyn_Exp*_T15;_T1= (int*)_TE;_T2= *_T1;switch(_T2){case 0: goto _LL0;case 1:{struct Cyc_Absyn_Exp_s_Absyn_Raw_stmt_struct*_T16=(struct Cyc_Absyn_Exp_s_Absyn_Raw_stmt_struct*)_TE;_T15= _T16->f1;}{struct Cyc_Absyn_Exp*e=_T15;
# 682
Cyc_InsertChecks_do_exp(nv,e);goto _LL0;}case 2:{struct Cyc_Absyn_Seq_s_Absyn_Raw_stmt_struct*_T16=(struct Cyc_Absyn_Seq_s_Absyn_Raw_stmt_struct*)_TE;_T14= _T16->f1;_T13= _T16->f2;}{struct Cyc_Absyn_Stmt*s1=_T14;struct Cyc_Absyn_Stmt*s2=_T13;
# 685
Cyc_InsertChecks_do_stmt(nv,s1);
Cyc_InsertChecks_do_stmt(nv,s2);goto _LL0;}case 3:{struct Cyc_Absyn_Return_s_Absyn_Raw_stmt_struct*_T16=(struct Cyc_Absyn_Return_s_Absyn_Raw_stmt_struct*)_TE;_T15= _T16->f1;}{struct Cyc_Absyn_Exp*eopt=_T15;_T3= eopt;_T4= (unsigned)_T3;
# 689
if(!_T4)goto _TLB1;
Cyc_InsertChecks_do_exp(nv,eopt);goto _TLB2;_TLB1: _TLB2: goto _LL0;}case 4:{struct Cyc_Absyn_IfThenElse_s_Absyn_Raw_stmt_struct*_T16=(struct Cyc_Absyn_IfThenElse_s_Absyn_Raw_stmt_struct*)_TE;_T15= _T16->f1;_T14= _T16->f2;_T13= _T16->f3;}{struct Cyc_Absyn_Exp*e=_T15;struct Cyc_Absyn_Stmt*s1=_T14;struct Cyc_Absyn_Stmt*s2=_T13;
# 693
Cyc_InsertChecks_do_exp(nv,e);
Cyc_InsertChecks_do_stmt(nv,s1);
Cyc_InsertChecks_do_stmt(nv,s2);goto _LL0;}case 14:{struct Cyc_Absyn_Do_s_Absyn_Raw_stmt_struct*_T16=(struct Cyc_Absyn_Do_s_Absyn_Raw_stmt_struct*)_TE;_T14= _T16->f1;_T5= _T16->f2;_T15= _T5.f0;}{struct Cyc_Absyn_Stmt*s1=_T14;struct Cyc_Absyn_Exp*e=_T15;_T15= e;_T14= s1;goto _LLE;}case 5:{struct Cyc_Absyn_While_s_Absyn_Raw_stmt_struct*_T16=(struct Cyc_Absyn_While_s_Absyn_Raw_stmt_struct*)_TE;_T6= _T16->f1;_T15= _T6.f0;_T14= _T16->f2;}_LLE: {struct Cyc_Absyn_Exp*e=_T15;struct Cyc_Absyn_Stmt*s1=_T14;
# 700
Cyc_InsertChecks_do_exp(nv,e);
Cyc_InsertChecks_do_stmt(nv,s1);goto _LL0;}case 9:{struct Cyc_Absyn_For_s_Absyn_Raw_stmt_struct*_T16=(struct Cyc_Absyn_For_s_Absyn_Raw_stmt_struct*)_TE;_T15= _T16->f1;_T7= _T16->f2;_T12= _T7.f0;_T8= _T16->f3;_T11= _T8.f0;_T14= _T16->f4;}{struct Cyc_Absyn_Exp*e1=_T15;struct Cyc_Absyn_Exp*e2=_T12;struct Cyc_Absyn_Exp*e3=_T11;struct Cyc_Absyn_Stmt*s1=_T14;
# 704
Cyc_InsertChecks_do_exp(nv,e1);
Cyc_InsertChecks_do_exp(nv,e2);
Cyc_InsertChecks_do_exp(nv,e3);
Cyc_InsertChecks_do_stmt(nv,s1);goto _LL0;}case 6: goto _LL14;case 7: _LL14: goto _LL16;case 8: _LL16:
# 712
 return;case 11:{struct Cyc_Absyn_Fallthru_s_Absyn_Raw_stmt_struct*_T16=(struct Cyc_Absyn_Fallthru_s_Absyn_Raw_stmt_struct*)_TE;_T10= _T16->f1;}{struct Cyc_List_List*es=_T10;
# 714
_TLB6: if(es!=0)goto _TLB4;else{goto _TLB5;}
_TLB4: _T9= nv;_TA= es;_TB= _TA->hd;_TC= (struct Cyc_Absyn_Exp*)_TB;Cyc_InsertChecks_do_exp(_T9,_TC);_TD= es;
# 714
es= _TD->tl;goto _TLB6;_TLB5: goto _LL0;}case 13:{struct Cyc_Absyn_Label_s_Absyn_Raw_stmt_struct*_T16=(struct Cyc_Absyn_Label_s_Absyn_Raw_stmt_struct*)_TE;_T14= _T16->f2;}{struct Cyc_Absyn_Stmt*s1=_T14;
# 718
Cyc_InsertChecks_do_stmt(nv,s1);goto _LL0;}case 12:{struct Cyc_Absyn_Decl_s_Absyn_Raw_stmt_struct*_T16=(struct Cyc_Absyn_Decl_s_Absyn_Raw_stmt_struct*)_TE;_TF= _T16->f1;_T14= _T16->f2;}{struct Cyc_Absyn_Decl*d=_TF;struct Cyc_Absyn_Stmt*s1=_T14;
# 721
Cyc_InsertChecks_do_decl(nv,d);
Cyc_InsertChecks_do_stmt(nv,s1);goto _LL0;}case 10:{struct Cyc_Absyn_Switch_s_Absyn_Raw_stmt_struct*_T16=(struct Cyc_Absyn_Switch_s_Absyn_Raw_stmt_struct*)_TE;_T15= _T16->f1;_T10= _T16->f2;}{struct Cyc_Absyn_Exp*e=_T15;struct Cyc_List_List*scs=_T10;
# 725
Cyc_InsertChecks_do_exp(nv,e);
Cyc_InsertChecks_do_scs(nv,scs);goto _LL0;}default:{struct Cyc_Absyn_TryCatch_s_Absyn_Raw_stmt_struct*_T16=(struct Cyc_Absyn_TryCatch_s_Absyn_Raw_stmt_struct*)_TE;_T14= _T16->f1;_T10= _T16->f2;}{struct Cyc_Absyn_Stmt*s1=_T14;struct Cyc_List_List*scs=_T10;
# 729
Cyc_InsertChecks_do_stmt(nv,s1);
Cyc_InsertChecks_do_scs(nv,scs);goto _LL0;}}_LL0:;}}
# 735
float Cyc_InsertChecks_percent(int i,int total){int _T0;float _T1;int _T2;float _T3;float _T4;float _T5;float _T6;
float ans=(float)1;
# 738
if(total==0)goto _TLB7;_T0= i;_T1= (float)_T0;_T2= total;_T3= (float)_T2;
ans= _T1 / _T3;goto _TLB8;_TLB7: _TLB8: _T4= ans;_T5= (float)100;_T6= _T4 * _T5;
return _T6;}
# 743
static long Cyc_InsertChecks_visit_esc_fn_exp(struct Cyc_Hashtable_Table*env,struct Cyc_Absyn_Exp*e){struct Cyc_Absyn_Exp*_T0;int*_T1;unsigned _T2;struct Cyc_Absyn_Var_e_Absyn_Raw_exp_struct*_T3;void*_T4;int*_T5;int _T6;void*_T7;struct Cyc_Absyn_Exp**(*_T8)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*);void**(*_T9)(struct Cyc_Hashtable_Table*,void*);struct Cyc_Absyn_Exp**_TA;unsigned _TB;struct Cyc_Absyn_Fndecl*_TC;struct Cyc_Absyn_Exp*_TD;int*_TE;unsigned _TF;void(*_T10)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*);void(*_T11)(struct Cyc_Hashtable_Table*,void*,void*);void(*_T12)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*);void(*_T13)(struct Cyc_Hashtable_Table*,void*,void*);long _T14;_T0= e;{
void*_T15=_T0->r;struct Cyc_Absyn_Exp*_T16;struct Cyc_Absyn_Fndecl*_T17;_T1= (int*)_T15;_T2= *_T1;switch(_T2){case 1: _T3= (struct Cyc_Absyn_Var_e_Absyn_Raw_exp_struct*)_T15;_T4= _T3->f1;_T5= (int*)_T4;_T6= *_T5;if(_T6!=2)goto _TLBA;{struct Cyc_Absyn_Var_e_Absyn_Raw_exp_struct*_T18=(struct Cyc_Absyn_Var_e_Absyn_Raw_exp_struct*)_T15;_T7= _T18->f1;{struct Cyc_Absyn_Funname_b_Absyn_Binding_struct*_T19=(struct Cyc_Absyn_Funname_b_Absyn_Binding_struct*)_T7;_T17= _T19->f1;}}{struct Cyc_Absyn_Fndecl*fd=_T17;_T9= Cyc_Hashtable_lookup_opt;{
# 748
struct Cyc_Absyn_Exp**(*_T18)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*)=(struct Cyc_Absyn_Exp**(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*))_T9;_T8= _T18;}_TA= _T8(env,e);_TB= (unsigned)_TA;if(_TB)goto _TLBC;else{goto _TLBE;}
_TLBE: _TC= fd;_TC->escapes= (long)1;goto _TLBD;_TLBC: _TLBD: goto _LL0;}_TLBA: goto _LL5;case 10:{struct Cyc_Absyn_FnCall_e_Absyn_Raw_exp_struct*_T18=(struct Cyc_Absyn_FnCall_e_Absyn_Raw_exp_struct*)_T15;_T16= _T18->f1;}{struct Cyc_Absyn_Exp*e1=_T16;_TD= e1;{
# 752
void*_T18=_TD->r;struct Cyc_Absyn_Exp*_T19;_TE= (int*)_T18;_TF= *_TE;switch(_TF){case 12:{struct Cyc_Absyn_NoInstantiate_e_Absyn_Raw_exp_struct*_T1A=(struct Cyc_Absyn_NoInstantiate_e_Absyn_Raw_exp_struct*)_T18;_T19= _T1A->f1;}{struct Cyc_Absyn_Exp*f=_T19;_T19= f;goto _LLB;}case 13:{struct Cyc_Absyn_Instantiate_e_Absyn_Raw_exp_struct*_T1A=(struct Cyc_Absyn_Instantiate_e_Absyn_Raw_exp_struct*)_T18;_T19= _T1A->f1;}_LLB: {struct Cyc_Absyn_Exp*f=_T19;_T11= Cyc_Hashtable_insert;{
# 756
void(*_T1A)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*)=(void(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*))_T11;_T10= _T1A;}_T10(env,f,f);goto _LL7;}default: _T13= Cyc_Hashtable_insert;{
# 759
void(*_T1A)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*)=(void(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*))_T13;_T12= _T1A;}_T12(env,e1,e1);goto _LL7;}_LL7:;}goto _LL0;}default: _LL5: goto _LL0;}_LL0:;}_T14= (long)1;
# 766
return _T14;}
# 769
static long Cyc_InsertChecks_visit_esc_fn_stmt(struct Cyc_Hashtable_Table*env,struct Cyc_Absyn_Stmt*s){long _T0;_T0= (long)1;return _T0;}
# 771
static void Cyc_InsertChecks_mark_escaped_fns(struct Cyc_List_List*ds){struct Cyc_Hashtable_Table*(*_T0)(int,int(*)(struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*),int(*)(struct Cyc_Absyn_Exp*));struct Cyc_Hashtable_Table*(*_T1)(int,int(*)(void*,void*),int(*)(void*));int(*_T2)(struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*);int(*_T3)(void*,void*);int(*_T4)(struct Cyc_Absyn_Exp*);struct Cyc_List_List*_T5;void*_T6;struct Cyc_Absyn_Decl*_T7;int*_T8;unsigned _T9;struct Cyc_Absyn_Vardecl*_TA;struct Cyc_Absyn_Exp*_TB;void(*_TC)(long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*),long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Stmt*),struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*);void(*_TD)(long(*)(void*,struct Cyc_Absyn_Exp*),long(*)(void*,struct Cyc_Absyn_Stmt*),void*,struct Cyc_Absyn_Exp*);struct Cyc_Hashtable_Table*_TE;struct Cyc_Absyn_Vardecl*_TF;struct Cyc_Absyn_Exp*_T10;struct Cyc_Absyn_Fndecl*_T11;enum Cyc_Absyn_Scope _T12;int _T13;struct Cyc_Absyn_Fndecl*_T14;void(*_T15)(long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*),long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Stmt*),struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Stmt*);void(*_T16)(long(*)(void*,struct Cyc_Absyn_Exp*),long(*)(void*,struct Cyc_Absyn_Stmt*),void*,struct Cyc_Absyn_Stmt*);struct Cyc_Hashtable_Table*_T17;struct Cyc_Absyn_Fndecl*_T18;struct Cyc_Absyn_Stmt*_T19;void(*_T1A)(long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*),long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Stmt*),struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*);void(*_T1B)(long(*)(void*,struct Cyc_Absyn_Exp*),long(*)(void*,struct Cyc_Absyn_Stmt*),void*,struct Cyc_Absyn_Exp*);struct Cyc_Hashtable_Table*_T1C;struct Cyc_Absyn_Exp*_T1D;void(*_T1E)(long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*),long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Stmt*),struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*);void(*_T1F)(long(*)(void*,struct Cyc_Absyn_Exp*),long(*)(void*,struct Cyc_Absyn_Stmt*),void*,struct Cyc_Absyn_Exp*);struct Cyc_Hashtable_Table*_T20;struct Cyc_Absyn_Exp*_T21;struct Cyc_List_List*_T22;
_TLC3: if(ds!=0)goto _TLC1;else{goto _TLC2;}
_TLC1: _T1= Cyc_Hashtable_create;{struct Cyc_Hashtable_Table*(*_T23)(int,int(*)(struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*),int(*)(struct Cyc_Absyn_Exp*))=(struct Cyc_Hashtable_Table*(*)(int,int(*)(struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*),int(*)(struct Cyc_Absyn_Exp*)))_T1;_T0= _T23;}_T3= Cyc_Core_ptrcmp;{int(*_T23)(struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*)=(int(*)(struct Cyc_Absyn_Exp*,struct Cyc_Absyn_Exp*))_T3;_T2= _T23;}{int(*_T23)(struct Cyc_Absyn_Exp*)=(int(*)(struct Cyc_Absyn_Exp*))Cyc_InsertChecks_hash_ptr;_T4= _T23;}{struct Cyc_Hashtable_Table*table=_T0(313,_T2,_T4);_T5= ds;_T6= _T5->hd;_T7= (struct Cyc_Absyn_Decl*)_T6;{
void*_T23=_T7->r;struct Cyc_List_List*_T24;struct Cyc_Absyn_Exp*_T25;struct Cyc_Absyn_Fndecl*_T26;struct Cyc_Absyn_Vardecl*_T27;_T8= (int*)_T23;_T9= *_T8;switch(_T9){case 0:{struct Cyc_Absyn_Var_d_Absyn_Raw_decl_struct*_T28=(struct Cyc_Absyn_Var_d_Absyn_Raw_decl_struct*)_T23;_T27= _T28->f1;}{struct Cyc_Absyn_Vardecl*vd=_T27;_TA= vd;_TB= _TA->initializer;
# 776
if(_TB==0)goto _TLC5;_TD= Cyc_Absyn_visit_exp;{
void(*_T28)(long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*),long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Stmt*),struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*)=(void(*)(long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*),long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Stmt*),struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*))_TD;_TC= _T28;}_TE= table;_TF= vd;_T10= _TF->initializer;_TC(Cyc_InsertChecks_visit_esc_fn_exp,Cyc_InsertChecks_visit_esc_fn_stmt,_TE,_T10);goto _TLC6;_TLC5: _TLC6: goto _LL0;}case 1:{struct Cyc_Absyn_Fn_d_Absyn_Raw_decl_struct*_T28=(struct Cyc_Absyn_Fn_d_Absyn_Raw_decl_struct*)_T23;_T26= _T28->f1;}{struct Cyc_Absyn_Fndecl*fd=_T26;_T11= fd;_T12= _T11->sc;_T13= (int)_T12;
# 781
if(_T13==0)goto _TLC7;_T14= fd;
_T14->escapes= (long)1;goto _TLC8;_TLC7: _TLC8: _T16= Cyc_Absyn_visit_stmt;{
void(*_T28)(long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*),long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Stmt*),struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Stmt*)=(void(*)(long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*),long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Stmt*),struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Stmt*))_T16;_T15= _T28;}_T17= table;_T18= fd;_T19= _T18->body;_T15(Cyc_InsertChecks_visit_esc_fn_exp,Cyc_InsertChecks_visit_esc_fn_stmt,_T17,_T19);goto _LL0;}case 2:{struct Cyc_Absyn_Let_d_Absyn_Raw_decl_struct*_T28=(struct Cyc_Absyn_Let_d_Absyn_Raw_decl_struct*)_T23;_T25= _T28->f3;}{struct Cyc_Absyn_Exp*e=_T25;_T1B= Cyc_Absyn_visit_exp;{
# 786
void(*_T28)(long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*),long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Stmt*),struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*)=(void(*)(long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*),long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Stmt*),struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*))_T1B;_T1A= _T28;}_T1C= table;_T1D= e;_T1A(Cyc_InsertChecks_visit_esc_fn_exp,Cyc_InsertChecks_visit_esc_fn_stmt,_T1C,_T1D);goto _LL0;}case 3: goto _LL0;case 4:{struct Cyc_Absyn_Region_d_Absyn_Raw_decl_struct*_T28=(struct Cyc_Absyn_Region_d_Absyn_Raw_decl_struct*)_T23;_T25= _T28->f3;}{struct Cyc_Absyn_Exp*eo=_T25;
# 791
if(eo==0)goto _TLC9;_T1F= Cyc_Absyn_visit_exp;{
void(*_T28)(long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*),long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Stmt*),struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*)=(void(*)(long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*),long(*)(struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Stmt*),struct Cyc_Hashtable_Table*,struct Cyc_Absyn_Exp*))_T1F;_T1E= _T28;}_T20= table;_T21= eo;_T1E(Cyc_InsertChecks_visit_esc_fn_exp,Cyc_InsertChecks_visit_esc_fn_stmt,_T20,_T21);goto _TLCA;_TLC9: _TLCA: goto _LL0;}case 5: goto _LLE;case 6: _LLE: goto _LL10;case 7: _LL10: goto _LL12;case 8: _LL12: goto _LL14;case 13: _LL14: goto _LL16;case 14: _LL16: goto _LL18;case 15: _LL18: goto _LL1A;case 16: _LL1A: goto _LL1C;case 12: _LL1C: goto _LL0;case 11:{struct Cyc_Absyn_ExternC_d_Absyn_Raw_decl_struct*_T28=(struct Cyc_Absyn_ExternC_d_Absyn_Raw_decl_struct*)_T23;_T24= _T28->f1;}{struct Cyc_List_List*ds=_T24;_T24= ds;goto _LL20;}case 10:{struct Cyc_Absyn_Using_d_Absyn_Raw_decl_struct*_T28=(struct Cyc_Absyn_Using_d_Absyn_Raw_decl_struct*)_T23;_T24= _T28->f2;}_LL20: {struct Cyc_List_List*ds=_T24;_T24= ds;goto _LL22;}default:{struct Cyc_Absyn_Namespace_d_Absyn_Raw_decl_struct*_T28=(struct Cyc_Absyn_Namespace_d_Absyn_Raw_decl_struct*)_T23;_T24= _T28->f2;}_LL22: {struct Cyc_List_List*ds=_T24;
# 809
Cyc_InsertChecks_mark_escaped_fns(ds);}}_LL0:;}}_T22= ds;
# 772
ds= _T22->tl;goto _TLC3;_TLC2:;}
# 814
void Cyc_InsertChecks_insert_checks(struct Cyc_List_List*tds,struct Cyc_JumpAnalysis_Jump_Anal_Result*tables,long do_vcgen){struct Cyc_InsertChecks_Env _T0;struct Cyc_List_List*_T1;void*_T2;struct Cyc_Absyn_Decl*_T3;struct Cyc_List_List*_T4;struct Cyc_InsertChecks_Env _T5;struct Cyc_List_List*_T6;void*_T7;struct Cyc_Absyn_Fndecl*_T8;struct Cyc_List_List*_T9;long _TA;int _TB;unsigned _TC;unsigned _TD;unsigned _TE;unsigned _TF;unsigned _T10;unsigned _T11;struct Cyc___cycFILE*_T12;struct _fat_ptr _T13;struct _fat_ptr _T14;struct Cyc_Int_pa_PrintArg_struct _T15;unsigned _T16;int _T17;struct Cyc_Int_pa_PrintArg_struct _T18;unsigned _T19;int _T1A;struct Cyc_Double_pa_PrintArg_struct _T1B;unsigned _T1C;int _T1D;unsigned _T1E;int _T1F;float _T20;struct Cyc___cycFILE*_T21;struct _fat_ptr _T22;struct _fat_ptr _T23;struct Cyc_Int_pa_PrintArg_struct _T24;unsigned _T25;int _T26;struct Cyc_Int_pa_PrintArg_struct _T27;unsigned _T28;int _T29;struct Cyc_Double_pa_PrintArg_struct _T2A;unsigned _T2B;int _T2C;unsigned _T2D;int _T2E;float _T2F;struct Cyc___cycFILE*_T30;struct _fat_ptr _T31;struct _fat_ptr _T32;struct Cyc_Int_pa_PrintArg_struct _T33;unsigned _T34;int _T35;struct Cyc_Double_pa_PrintArg_struct _T36;unsigned _T37;int _T38;unsigned _T39;int _T3A;float _T3B;struct Cyc___cycFILE*_T3C;struct _fat_ptr _T3D;struct _fat_ptr _T3E;struct Cyc_Int_pa_PrintArg_struct _T3F;unsigned _T40;int _T41;struct Cyc_Double_pa_PrintArg_struct _T42;unsigned _T43;int _T44;unsigned _T45;int _T46;float _T47;struct Cyc___cycFILE*_T48;struct _fat_ptr _T49;struct _fat_ptr _T4A;struct Cyc_Int_pa_PrintArg_struct _T4B;unsigned _T4C;int _T4D;struct Cyc_Double_pa_PrintArg_struct _T4E;unsigned _T4F;int _T50;unsigned _T51;int _T52;float _T53;struct Cyc___cycFILE*_T54;struct _fat_ptr _T55;struct _fat_ptr _T56;struct Cyc_Int_pa_PrintArg_struct _T57;unsigned _T58;int _T59;struct Cyc_Int_pa_PrintArg_struct _T5A;unsigned _T5B;int _T5C;struct Cyc_Double_pa_PrintArg_struct _T5D;unsigned _T5E;int _T5F;unsigned _T60;int _T61;float _T62;struct Cyc___cycFILE*_T63;struct _fat_ptr _T64;struct _fat_ptr _T65;struct Cyc_Int_pa_PrintArg_struct _T66;unsigned _T67;int _T68;struct Cyc_Double_pa_PrintArg_struct _T69;unsigned _T6A;int _T6B;unsigned _T6C;int _T6D;float _T6E;struct Cyc___cycFILE*_T6F;struct _fat_ptr _T70;struct _fat_ptr _T71;struct Cyc_Int_pa_PrintArg_struct _T72;unsigned _T73;int _T74;struct Cyc_Double_pa_PrintArg_struct _T75;unsigned _T76;int _T77;unsigned _T78;int _T79;float _T7A;struct Cyc___cycFILE*_T7B;struct _fat_ptr _T7C;struct _fat_ptr _T7D;struct Cyc_Int_pa_PrintArg_struct _T7E;unsigned _T7F;int _T80;struct Cyc_Double_pa_PrintArg_struct _T81;unsigned _T82;int _T83;unsigned _T84;int _T85;float _T86;struct Cyc___cycFILE*_T87;struct _fat_ptr _T88;struct _fat_ptr _T89;struct Cyc_Int_pa_PrintArg_struct _T8A;unsigned _T8B;int _T8C;struct Cyc_Double_pa_PrintArg_struct _T8D;unsigned _T8E;int _T8F;unsigned _T90;int _T91;float _T92;struct Cyc___cycFILE*_T93;struct _fat_ptr _T94;struct _fat_ptr _T95;struct Cyc___cycFILE*_T96;struct _fat_ptr _T97;struct _fat_ptr _T98;
# 816
Cyc_InsertChecks_reset_counters();{
# 818
struct Cyc_InsertChecks_Env nv=Cyc_InsertChecks_toplevel_env(tables,do_vcgen);
# 823
struct Cyc_Dict_Dict cg=Cyc_Callgraph_compute_callgraph(tds);
# 830
Cyc_InsertChecks_mark_escaped_fns(tds);
# 833
_TLCE: if(tds!=0)goto _TLCC;else{goto _TLCD;}
_TLCC: _T0= nv;_T1= tds;_T2= _T1->hd;_T3= (struct Cyc_Absyn_Decl*)_T2;Cyc_InsertChecks_do_decl(_T0,_T3);_T4= tds;
# 833
tds= _T4->tl;goto _TLCE;_TLCD: {
# 837
struct Cyc_List_List*cg_fds=Cyc_Graph_tsort(cg);{
# 842
struct Cyc_List_List*fds=Cyc_List_rev(cg_fds);_TLD2: if(fds!=0)goto _TLD0;else{goto _TLD1;}
_TLD0: _T5= nv;_T6= fds;_T7= _T6->hd;_T8= (struct Cyc_Absyn_Fndecl*)_T7;Cyc_InsertChecks_do_fndecl(_T5,_T8);_T9= fds;
# 842
fds= _T9->tl;goto _TLD2;_TLD1:;}_TA= Cyc_Flags_verbose;_TB= (int)_TA;
# 855 "library/compiler/insert_checks.cyc"
if(!_TB)goto _TLD3;_TC= Cyc_InsertChecks_types_eliminated_bounds_checks + Cyc_InsertChecks_simple_eliminated_bounds_checks;_TD= Cyc_InsertChecks_constraints_eliminated_bounds_checks;{
unsigned eliminated_bounds_checks=_TC + _TD;_TE= Cyc_InsertChecks_types_eliminated_null_checks + Cyc_InsertChecks_flow_eliminated_null_checks;_TF= Cyc_InsertChecks_simple_eliminated_null_checks;_T10= _TE + _TF;_T11= Cyc_InsertChecks_constraints_eliminated_null_checks;{
# 859
unsigned eliminated_null_checks=_T10 + _T11;_T12= Cyc_stderr;_T13=
# 861
_tag_fat("****INSERT CHECKS***********************************\n",sizeof(char),54U);_T14= _tag_fat(0U,sizeof(void*),0);Cyc_fprintf(_T12,_T13,_T14);{struct Cyc_Int_pa_PrintArg_struct _T99;_T99.tag= 1;_T16= eliminated_bounds_checks + eliminated_null_checks;_T17= (int)_T16;
# 863
_T99.f1= (unsigned long)_T17;_T15= _T99;}{struct Cyc_Int_pa_PrintArg_struct _T99=_T15;{struct Cyc_Int_pa_PrintArg_struct _T9A;_T9A.tag= 1;_T19= Cyc_InsertChecks_total_bounds_checks + Cyc_InsertChecks_total_null_checks;_T1A= (int)_T19;
_T9A.f1= (unsigned long)_T1A;_T18= _T9A;}{struct Cyc_Int_pa_PrintArg_struct _T9A=_T18;{struct Cyc_Double_pa_PrintArg_struct _T9B;_T9B.tag= 3;_T1C= eliminated_bounds_checks + eliminated_null_checks;_T1D= (int)_T1C;_T1E= Cyc_InsertChecks_total_bounds_checks + Cyc_InsertChecks_total_null_checks;_T1F= (int)_T1E;_T20=
Cyc_InsertChecks_percent(_T1D,_T1F);_T9B.f1= (double)_T20;_T1B= _T9B;}{struct Cyc_Double_pa_PrintArg_struct _T9B=_T1B;void*_T9C[3];_T9C[0]= & _T99;_T9C[1]= & _T9A;_T9C[2]= & _T9B;_T21= Cyc_stderr;_T22=
# 862
_tag_fat("eliminated %d out of a total of %d checks (%g%%).\n",sizeof(char),51U);_T23= _tag_fat(_T9C,sizeof(void*),3);Cyc_fprintf(_T21,_T22,_T23);}}}{struct Cyc_Int_pa_PrintArg_struct _T99;_T99.tag= 1;_T25= eliminated_bounds_checks;_T26= (int)_T25;
# 868
_T99.f1= (unsigned long)_T26;_T24= _T99;}{struct Cyc_Int_pa_PrintArg_struct _T99=_T24;{struct Cyc_Int_pa_PrintArg_struct _T9A;_T9A.tag= 1;_T28= Cyc_InsertChecks_total_bounds_checks;_T29= (int)_T28;_T9A.f1= (unsigned long)_T29;_T27= _T9A;}{struct Cyc_Int_pa_PrintArg_struct _T9A=_T27;{struct Cyc_Double_pa_PrintArg_struct _T9B;_T9B.tag= 3;_T2B= eliminated_bounds_checks;_T2C= (int)_T2B;_T2D= Cyc_InsertChecks_total_bounds_checks;_T2E= (int)_T2D;_T2F=
Cyc_InsertChecks_percent(_T2C,_T2E);_T9B.f1= (double)_T2F;_T2A= _T9B;}{struct Cyc_Double_pa_PrintArg_struct _T9B=_T2A;void*_T9C[3];_T9C[0]= & _T99;_T9C[1]= & _T9A;_T9C[2]= & _T9B;_T30= Cyc_stderr;_T31=
# 867
_tag_fat("eliminated %d out of a total of %d bounds checks (%g%%).\n",sizeof(char),58U);_T32= _tag_fat(_T9C,sizeof(void*),3);Cyc_fprintf(_T30,_T31,_T32);}}}{struct Cyc_Int_pa_PrintArg_struct _T99;_T99.tag= 1;_T34= Cyc_InsertChecks_types_eliminated_bounds_checks;_T35= (int)_T34;
# 871
_T99.f1= (unsigned long)_T35;_T33= _T99;}{struct Cyc_Int_pa_PrintArg_struct _T99=_T33;{struct Cyc_Double_pa_PrintArg_struct _T9A;_T9A.tag= 3;_T37= Cyc_InsertChecks_types_eliminated_bounds_checks;_T38= (int)_T37;_T39= Cyc_InsertChecks_total_bounds_checks;_T3A= (int)_T39;_T3B=
Cyc_InsertChecks_percent(_T38,_T3A);_T9A.f1= (double)_T3B;_T36= _T9A;}{struct Cyc_Double_pa_PrintArg_struct _T9A=_T36;void*_T9B[2];_T9B[0]= & _T99;_T9B[1]= & _T9A;_T3C= Cyc_stderr;_T3D=
# 870
_tag_fat("\tbounds checks eliminated by types            : %d (%g%%)\n",sizeof(char),59U);_T3E= _tag_fat(_T9B,sizeof(void*),2);Cyc_fprintf(_T3C,_T3D,_T3E);}}{struct Cyc_Int_pa_PrintArg_struct _T99;_T99.tag= 1;_T40= Cyc_InsertChecks_simple_eliminated_bounds_checks;_T41= (int)_T40;
# 874
_T99.f1= (unsigned long)_T41;_T3F= _T99;}{struct Cyc_Int_pa_PrintArg_struct _T99=_T3F;{struct Cyc_Double_pa_PrintArg_struct _T9A;_T9A.tag= 3;_T43= Cyc_InsertChecks_simple_eliminated_bounds_checks;_T44= (int)_T43;_T45= Cyc_InsertChecks_total_bounds_checks;_T46= (int)_T45;_T47=
Cyc_InsertChecks_percent(_T44,_T46);_T9A.f1= (double)_T47;_T42= _T9A;}{struct Cyc_Double_pa_PrintArg_struct _T9A=_T42;void*_T9B[2];_T9B[0]= & _T99;_T9B[1]= & _T9A;_T48= Cyc_stderr;_T49=
# 873
_tag_fat("\tbounds checks eliminated by simple prover    : %d (%g%%)\n",sizeof(char),59U);_T4A= _tag_fat(_T9B,sizeof(void*),2);Cyc_fprintf(_T48,_T49,_T4A);}}{struct Cyc_Int_pa_PrintArg_struct _T99;_T99.tag= 1;_T4C= Cyc_InsertChecks_constraints_eliminated_bounds_checks;_T4D= (int)_T4C;
# 877
_T99.f1= (unsigned long)_T4D;_T4B= _T99;}{struct Cyc_Int_pa_PrintArg_struct _T99=_T4B;{struct Cyc_Double_pa_PrintArg_struct _T9A;_T9A.tag= 3;_T4F= Cyc_InsertChecks_constraints_eliminated_bounds_checks;_T50= (int)_T4F;_T51= Cyc_InsertChecks_total_bounds_checks;_T52= (int)_T51;_T53=
Cyc_InsertChecks_percent(_T50,_T52);_T9A.f1= (double)_T53;_T4E= _T9A;}{struct Cyc_Double_pa_PrintArg_struct _T9A=_T4E;void*_T9B[2];_T9B[0]= & _T99;_T9B[1]= & _T9A;_T54= Cyc_stderr;_T55=
# 876
_tag_fat("\tbounds checks eliminated by constraint solver: %d (%g%%)\n",sizeof(char),59U);_T56= _tag_fat(_T9B,sizeof(void*),2);Cyc_fprintf(_T54,_T55,_T56);}}{struct Cyc_Int_pa_PrintArg_struct _T99;_T99.tag= 1;_T58= eliminated_null_checks;_T59= (int)_T58;
# 880
_T99.f1= (unsigned long)_T59;_T57= _T99;}{struct Cyc_Int_pa_PrintArg_struct _T99=_T57;{struct Cyc_Int_pa_PrintArg_struct _T9A;_T9A.tag= 1;_T5B= Cyc_InsertChecks_total_null_checks;_T5C= (int)_T5B;_T9A.f1= (unsigned long)_T5C;_T5A= _T9A;}{struct Cyc_Int_pa_PrintArg_struct _T9A=_T5A;{struct Cyc_Double_pa_PrintArg_struct _T9B;_T9B.tag= 3;_T5E= eliminated_null_checks;_T5F= (int)_T5E;_T60= Cyc_InsertChecks_total_null_checks;_T61= (int)_T60;_T62=
Cyc_InsertChecks_percent(_T5F,_T61);_T9B.f1= (double)_T62;_T5D= _T9B;}{struct Cyc_Double_pa_PrintArg_struct _T9B=_T5D;void*_T9C[3];_T9C[0]= & _T99;_T9C[1]= & _T9A;_T9C[2]= & _T9B;_T63= Cyc_stderr;_T64=
# 879
_tag_fat("eliminated %d out of a total of %d null checks (%g%%).\n",sizeof(char),56U);_T65= _tag_fat(_T9C,sizeof(void*),3);Cyc_fprintf(_T63,_T64,_T65);}}}{struct Cyc_Int_pa_PrintArg_struct _T99;_T99.tag= 1;_T67= Cyc_InsertChecks_types_eliminated_null_checks;_T68= (int)_T67;
# 883
_T99.f1= (unsigned long)_T68;_T66= _T99;}{struct Cyc_Int_pa_PrintArg_struct _T99=_T66;{struct Cyc_Double_pa_PrintArg_struct _T9A;_T9A.tag= 3;_T6A= Cyc_InsertChecks_types_eliminated_null_checks;_T6B= (int)_T6A;_T6C= Cyc_InsertChecks_total_null_checks;_T6D= (int)_T6C;_T6E= Cyc_InsertChecks_percent(_T6B,_T6D);_T9A.f1= (double)_T6E;_T69= _T9A;}{struct Cyc_Double_pa_PrintArg_struct _T9A=_T69;void*_T9B[2];_T9B[0]= & _T99;_T9B[1]= & _T9A;_T6F= Cyc_stderr;_T70=
# 882
_tag_fat("\tnull checks eliminated by types              : %d (%g%%)\n",sizeof(char),59U);_T71= _tag_fat(_T9B,sizeof(void*),2);Cyc_fprintf(_T6F,_T70,_T71);}}{struct Cyc_Int_pa_PrintArg_struct _T99;_T99.tag= 1;_T73= Cyc_InsertChecks_flow_eliminated_null_checks;_T74= (int)_T73;
# 885
_T99.f1= (unsigned long)_T74;_T72= _T99;}{struct Cyc_Int_pa_PrintArg_struct _T99=_T72;{struct Cyc_Double_pa_PrintArg_struct _T9A;_T9A.tag= 3;_T76= Cyc_InsertChecks_flow_eliminated_null_checks;_T77= (int)_T76;_T78= Cyc_InsertChecks_total_null_checks;_T79= (int)_T78;_T7A= Cyc_InsertChecks_percent(_T77,_T79);_T9A.f1= (double)_T7A;_T75= _T9A;}{struct Cyc_Double_pa_PrintArg_struct _T9A=_T75;void*_T9B[2];_T9B[0]= & _T99;_T9B[1]= & _T9A;_T7B= Cyc_stderr;_T7C=
# 884
_tag_fat("\tnull checks eliminated by flow analysis      : %d (%g%%)\n",sizeof(char),59U);_T7D= _tag_fat(_T9B,sizeof(void*),2);Cyc_fprintf(_T7B,_T7C,_T7D);}}{struct Cyc_Int_pa_PrintArg_struct _T99;_T99.tag= 1;_T7F= Cyc_InsertChecks_simple_eliminated_null_checks;_T80= (int)_T7F;
# 887
_T99.f1= (unsigned long)_T80;_T7E= _T99;}{struct Cyc_Int_pa_PrintArg_struct _T99=_T7E;{struct Cyc_Double_pa_PrintArg_struct _T9A;_T9A.tag= 3;_T82= Cyc_InsertChecks_simple_eliminated_null_checks;_T83= (int)_T82;_T84= Cyc_InsertChecks_total_null_checks;_T85= (int)_T84;_T86=
Cyc_InsertChecks_percent(_T83,_T85);_T9A.f1= (double)_T86;_T81= _T9A;}{struct Cyc_Double_pa_PrintArg_struct _T9A=_T81;void*_T9B[2];_T9B[0]= & _T99;_T9B[1]= & _T9A;_T87= Cyc_stderr;_T88=
# 886
_tag_fat("\tnull checks eliminated by simple prover      : %d (%g%%)\n",sizeof(char),59U);_T89= _tag_fat(_T9B,sizeof(void*),2);Cyc_fprintf(_T87,_T88,_T89);}}{struct Cyc_Int_pa_PrintArg_struct _T99;_T99.tag= 1;_T8B= Cyc_InsertChecks_constraints_eliminated_null_checks;_T8C= (int)_T8B;
# 890
_T99.f1= (unsigned long)_T8C;_T8A= _T99;}{struct Cyc_Int_pa_PrintArg_struct _T99=_T8A;{struct Cyc_Double_pa_PrintArg_struct _T9A;_T9A.tag= 3;_T8E= Cyc_InsertChecks_constraints_eliminated_null_checks;_T8F= (int)_T8E;_T90= Cyc_InsertChecks_total_null_checks;_T91= (int)_T90;_T92=
Cyc_InsertChecks_percent(_T8F,_T91);_T9A.f1= (double)_T92;_T8D= _T9A;}{struct Cyc_Double_pa_PrintArg_struct _T9A=_T8D;void*_T9B[2];_T9B[0]= & _T99;_T9B[1]= & _T9A;_T93= Cyc_stderr;_T94=
# 889
_tag_fat("\tnull checks eliminated by constraint solver  : %d (%g%%)\n",sizeof(char),59U);_T95= _tag_fat(_T9B,sizeof(void*),2);Cyc_fprintf(_T93,_T94,_T95);}}_T96= Cyc_stderr;_T97=
# 892
_tag_fat("****************************************************\n",sizeof(char),54U);_T98= _tag_fat(0U,sizeof(void*),0);Cyc_fprintf(_T96,_T97,_T98);}}goto _TLD4;_TLD3: _TLD4:;}}}

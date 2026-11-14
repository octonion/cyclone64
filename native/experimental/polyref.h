#ifndef CYC_EXPERIMENTAL_POLYREF_H
#define CYC_EXPERIMENTAL_POLYREF_H
#include "cyc_include.h"
/* Experimental representation. Not yet emitted by Toc or part of the runtime
 * ABI. A reference retains its actual storage width, bounds and nested layout;
 * mutations always reach the original object, including after a reference is
 * returned. Layout records must outlive their references. */
enum cyc_ref_kind { CR_I8, CR_U8, CR_I16, CR_U16, CR_I32, CR_U32,
                    CR_I64, CR_U64, CR_POINTER, CR_FAT_POINTER, CR_REFERENCE };
struct cyc_ref_layout {
  enum cyc_ref_kind kind;
  const struct cyc_ref_layout *pointee;
  size_t pointee_count;
};
struct cyc_poly_ref {
  uintptr_t current, base, end;
  const struct cyc_ref_layout *layout;
};
size_t cyc_ref_width(const struct cyc_ref_layout *);
struct cyc_poly_ref *cyc_ref_wrap(void *,size_t,const struct cyc_ref_layout *);
struct cyc_poly_ref *cyc_ref_wrap_fat(struct _fat_ptr,const struct cyc_ref_layout *);
void *cyc_ref_address(const struct cyc_poly_ref *);
struct _fat_ptr cyc_ref_fat(const struct cyc_poly_ref *);
uintptr_t cyc_ref_load_word(const struct cyc_poly_ref *);
void cyc_ref_store_word(const struct cyc_poly_ref *,uintptr_t);
struct cyc_poly_ref *cyc_ref_load_reference(const struct cyc_poly_ref *);
void cyc_ref_store_reference(const struct cyc_poly_ref *,struct cyc_poly_ref *);
struct cyc_poly_ref *cyc_ref_step(const struct cyc_poly_ref *,uintptr_t,int);
ptrdiff_t cyc_ref_difference(const struct cyc_poly_ref *,const struct cyc_poly_ref *);
int cyc_ref_equal(const struct cyc_poly_ref *,const struct cyc_poly_ref *);
#endif

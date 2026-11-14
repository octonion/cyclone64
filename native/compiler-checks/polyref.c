#include "../experimental/polyref.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
static void *allocated[256];static size_t allocated_count;
void *GC_malloc(size_t n) { void *p=calloc(1,n);if(allocated_count==256) _exit(98);allocated[allocated_count++]=p;return p; }
static jmp_buf failure;static int expected;
static void *fail(void) { if(!expected) _exit(99);longjmp(failure,1); }
void *_throw_null_fn(const char *f,unsigned l) { return fail(); }
void *_throw_arraybounds_fn(const char *f,unsigned l) { return fail(); }
void *_throw_badalloc_fn(const char *f,unsigned l) { return fail(); }
#define CHECK(e) do { if(!(e)) { fprintf(stderr,"FAIL %d: %s\n",__LINE__,#e);_exit(1); } } while(0)
#define REJECT(e) do {expected=1;if(!setjmp(failure)){(void)(e);_exit(97);}expected=0;} while(0)
static const struct cyc_ref_layout i32={CR_I32},u64={CR_U64},i8={CR_I8},i16={CR_I16};
static const struct cyc_ref_layout pi32={CR_POINTER,&i32,1},fi32={CR_FAT_POINTER,&i32,0},wrapped={CR_REFERENCE};
static struct cyc_poly_ref *escape(int *p) { return cyc_ref_wrap(p,1,&i32); }
int main(void) {
  struct {int before,value,after;} s={17,123,29};
  struct cyc_poly_ref *r=escape(&s.value);
  CHECK(cyc_ref_load_word(r)==123 && cyc_ref_address(r)==&s.value);
  cyc_ref_store_word(r,(uintptr_t)(uint32_t)-7);
  CHECK(s.before==17 && s.value==-7 && s.after==29);
  CHECK(cyc_ref_load_word(r)==UINT32_MAX-6U);
  int a[]={10,20,30};r=cyc_ref_wrap(a,3,&i32);
  struct cyc_poly_ref *middle=cyc_ref_step(r,1,0);
  CHECK(cyc_ref_load_word(middle)==20 && cyc_ref_difference(middle,r)==1);
  cyc_ref_store_word(middle,42);CHECK(a[0]==10 && a[1]==42 && a[2]==30);
  struct cyc_poly_ref *beyond=cyc_ref_step(r,4,0);REJECT(cyc_ref_load_word(beyond));
  CHECK(cyc_ref_equal(cyc_ref_step(beyond,4,1),r));
  REJECT(cyc_ref_step(r,UINTPTR_MAX,0));REJECT(cyc_ref_load_word(NULL));
  int *p=&s.value;struct cyc_poly_ref *nested=cyc_ref_wrap(&p,1,&pi32);
  struct cyc_poly_ref *inner=cyc_ref_load_reference(nested);
  cyc_ref_store_word(inner,51);CHECK(s.value==51);
  cyc_ref_store_reference(nested,middle);CHECK(p==&a[1]);
  struct cyc_poly_ref *stored=r,*outer=cyc_ref_wrap(&stored,1,&wrapped);
  CHECK(cyc_ref_load_reference(outer)==r);cyc_ref_store_reference(outer,middle);CHECK(stored==middle);
  struct _fat_ptr fat={(unsigned char *)a,(unsigned char *)a,(unsigned char *)(a+3)};
  outer=cyc_ref_wrap(&fat,1,&fi32);inner=cyc_ref_load_reference(outer);
  CHECK(cyc_ref_load_word(cyc_ref_step(inner,2,0))==30);
  cyc_ref_store_reference(outer,middle);CHECK(fat.curr==(unsigned char *)&a[1] && fat.last_plus_one==(unsigned char *)(a+3));
  uint64_t wide=UINT64_C(0x123456789abcdef0);r=cyc_ref_wrap(&wide,1,&u64);
  CHECK(cyc_ref_load_word(r)==wide);cyc_ref_store_word(r,UINT64_MAX);CHECK(wide==UINT64_MAX);
  int8_t small=-7;int16_t short_value=-300;
  CHECK(cyc_ref_load_word(cyc_ref_wrap(&small,1,&i8))==(uintptr_t)-7);
  CHECK(cyc_ref_load_word(cyc_ref_wrap(&short_value,1,&i16))==(uintptr_t)-300);
  for(size_t i=0;i<allocated_count;++i) free(allocated[i]);
  puts("experimental typed references: widths, mutation, escape, array stride, bounds, nesting: PASS");
}

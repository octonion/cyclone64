#include "cyc_include.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
static jmp_buf rejected;
static int expected;
void *_throw_arraybounds_fn(const char *f,unsigned l) { assert(expected); longjmp(rejected,1); }
void *_throw_null_fn(const char *f,unsigned l) { assert(expected); longjmp(rejected,1); }
#define REJECT(expr) do { expected=1; if(!setjmp(rejected)) { (void)(expr); assert(!"missing bounds exception"); } expected=0; } while(0)
int main(void) {
  int a[]={10,20,30};
  struct _fat_ptr p={(void *)a,(void *)a,(void *)(a+3)};
  assert(*(int *)_check_fat_subscript(p,sizeof(int),2UL)==30);
  unsigned long i=1; assert(*(int *)_check_fat_subscript(p,sizeof(int),i++)==20 && i==2);
  REJECT(_check_fat_subscript(p,sizeof(int),0x100000001UL));
  REJECT(_check_fat_subscript(p,sizeof(int),ULONG_MAX));
  REJECT(_check_fat_subscript(p,sizeof(int),LONG_MIN));
  REJECT(_check_fat_subscript(p,sizeof(int),((unsigned __int128)1<<100)+1));
  REJECT(_check_fat_subscript(p,sizeof(int),-1L));
  p.curr=(void *)(a+2);
  assert(*(int *)_check_fat_subscript(p,sizeof(int),-2L)==10);
  REJECT(_check_fat_subscript(p,sizeof(int),-3L));
  p.curr=(void *)(a+3);
  assert(*(int *)_check_fat_subscript(p,sizeof(int),-1L)==30);
  REJECT(_check_fat_subscript(p,sizeof(int),0));
  REJECT(_check_known_subscript_notnull(a,3,sizeof(int),0x100000001UL));
  REJECT(_check_known_subscript_null(a,3,sizeof(int),ULONG_MAX));
  REJECT(_check_known_subscript_notnull(a,3,sizeof(int),-1L));
  REJECT(_check_known_subscript_null(NULL,3,sizeof(int),0));
  assert(*(int *)_check_known_subscript_notnull(a,3,sizeof(int),2UL)==30);
  char z[]="abc";
  assert(*_zero_arr_plus_char(z,1,2UL)=='c');
  REJECT(_zero_arr_plus_char(z,1,0x100000001UL));
  REJECT(_zero_arr_plus_char(z,1,ULONG_MAX));
  REJECT(_zero_arr_plus_char(z,1,((unsigned __int128)1<<100)+1));
  int term[]={1,2,0};
  REJECT(_zero_arr_plus_other(sizeof(int),term,3,0x100000001UL));
  char *zp=z;
  assert(_zero_arr_inplace_plus_post_char(&zp,1L)==z && zp==z+1);
  int nz[]={1,2,0}; void *np=nz;
  assert(_zero_arr_inplace_plus_post_other(sizeof(int),&np,1L)==nz && np==nz+1);
  puts("runtime bounds: wide, negative, overflow, one-past, and single evaluation: PASS");
}

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "banshee.h"
#include "hash.h"
#include "persist.h"
#include "ufind.h"
static stamp applied[5];
static int applied_count;
static void collect_stamp(stamp value) {
  assert(applied_count < 5);
  applied[applied_count++] = value;
}
static bool positive(stamp value) { return value > 0; }
static bool reject_all(stamp value) { (void)value; return FALSE; }
static int stamp_compare(stamp a, stamp b) { return (a > b) - (a < b); }
static int stamp_reverse_compare(stamp a, stamp b) { return (a < b) - (a > b); }
static stamp_list nested;
static int nested_compare(stamp a, stamp b) {
  stamp_list_sort(nested, stamp_reverse_compare);
  return stamp_compare(a, b);
}
DECLARE_UFIND(test_int_uf, int);
DEFINE_NONPTR_UFIND(test_int_uf, int);
static int sum_info(int a, int b) { return a + b; }
int main(void) {
  engine_init(); region r = newregion();
  stamp s = stamp_string("same"); assert(s == stamp_string("same"));
  stamp input[] = {-2147483647, -1, 0, 1, 2147483647};
  stamp_list list = stamp_list_from_array(r, input, 5);
  assert(stamp_list_app(list, collect_stamp) == list);
  assert(applied_count == 5 && !memcmp(input, applied, sizeof(input)));
  stamp *out = stamp_list_array_from_list(r, list);
  assert(!memcmp(input, out, sizeof(input)));
  struct { stamp value; unsigned guard; } item = {0, 0xdeadbeef};
  stamp_list_scanner scan; stamp_list_scan(list, &scan);
  for (int i=0; i<5; ++i) {
    assert(stamp_list_next(&scan, &item.value)); assert(item.value == input[i]);
    assert(item.guard == 0xdeadbeef);
  }
  assert(!stamp_list_next(&scan, &item.value));
  hash_table h = make_hash_table(r, 1, ptr_hash, ptr_eq);
  assert(hash_table_insert(h, (void *)(uintptr_t)0x100000001, &item));
  void *p; assert(hash_table_lookup(h, (void *)(uintptr_t)0x100000001, &p) && p == &item);
  region destination=newregion();
  hash_table cloned=hash_table_copy(destination,h);
  deleteregion(r);
  assert(hash_table_lookup(cloned,(void *)(uintptr_t)0x100000001,&p)&&p==&item);
  deleteregion(destination);
  region callbacks = newregion();
  stamp values[] = {4, -2, 9, 1};
  stamp_list sorted = stamp_list_from_array(callbacks, values, 4);
  nested = stamp_list_from_array(callbacks, values, 4);
  stamp_list_sort(sorted, nested_compare);
  assert(stamp_list_head(sorted) == -2 && stamp_list_last(sorted) == 9);
  assert(stamp_list_head(nested) == 9 && stamp_list_last(nested) == -2);
  assert(stamp_list_find(sorted, positive) == 1);
  stamp_list filtered = stamp_list_filter(callbacks, sorted, positive);
  assert(stamp_list_length(filtered) == 3);
  assert(stamp_list_head(filtered) == 9 && stamp_list_last(filtered) == 1);
  assert(stamp_list_length(stamp_list_filter2(sorted, positive)) == 3);
  stamp_list_drop(sorted, positive);
  assert(stamp_list_length(sorted) == 3 && stamp_list_find(sorted, positive) == 4);
  stamp_list_keep(sorted, positive);
  assert(stamp_list_length(sorted) == 2 && stamp_list_head(sorted) == 4);
  stamp_list_merge(sorted, filtered, stamp_compare);
  assert(stamp_list_length(sorted) == 5 && stamp_list_last(sorted) == 9);
  stamp_list_keep(sorted, reject_all);
  assert(stamp_list_empty(sorted));
  stamp_list_append_tail(42, sorted);
  stamp_list_drop(sorted, positive);
  assert(stamp_list_empty(sorted));
  stamp_list_append_tail(-7, sorted);
  assert(stamp_list_head(sorted) == -7 && stamp_list_last(sorted) == -7);
  test_int_uf first = new_test_int_uf(callbacks, -9);
  test_int_uf second = new_test_int_uf(callbacks, 14);
  uf_tick();
  assert(test_int_uf_unify(sum_info, first, second));
  assert(test_int_uf_get_info(first) == 5);
  uf_rollback();
  assert(!test_int_uf_eq(first, second));
  assert(test_int_uf_get_info(first) == -9 && test_int_uf_get_info(second) == 14);
  deleteregion(callbacks);
  FILE *f = tmpfile(); assert(f);
  assert(string_data_serialize(f, "64-bit persistence")); rewind(f);
  char *copy = string_data_deserialize(f); assert(!strcmp(copy, "64-bit persistence"));
  free(copy); fclose(f);
  puts("containers: integer payloads, narrow buffers, strings, typed callbacks: PASS");
}

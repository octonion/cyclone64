#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "banshee.h"
#include "hash.h"
#include "persist.h"
int main(void) {
  engine_init(); region r = newregion();
  stamp s = stamp_string("same"); assert(s == stamp_string("same"));
  stamp input[] = {-2147483647, -1, 0, 1, 2147483647};
  stamp_list list = stamp_list_from_array(r, input, 5);
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
  FILE *f = tmpfile(); assert(f);
  assert(string_data_serialize(f, "64-bit persistence")); rewind(f);
  char *copy = string_data_deserialize(f); assert(!strcmp(copy, "64-bit persistence"));
  free(copy); fclose(f); deleteregion(r);
  puts("containers: integer payloads, narrow buffers, strings: PASS");
}

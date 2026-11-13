#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "pagemap.h"
int main(void) {
  struct page_map m = {0}; int values[4096];
  for (uintptr_t i = 0; i < 4096; ++i) {
    page_map_set(&m, (i << 32) | 123, &values[i]);
    assert(page_map_get(&m, (i << 32) | 123) == &values[i]);
  }
  for (uintptr_t i = 0; i < 4096; ++i) assert(page_map_get(&m, (i << 32) | 123) == &values[i]);
  page_map_set(&m, UINTPTR_MAX, &values[0]);
  assert(page_map_get(&m, UINTPTR_MAX) == &values[0]);
  for (uintptr_t i = 0; i < 4096; ++i) page_map_set(&m, (i << 32) | 123, NULL);
  assert(m.count == 1); page_map_destroy(&m); assert(!m.count && !m.capacity);
  puts("pagemap: high bits, growth, deletion: PASS");
}

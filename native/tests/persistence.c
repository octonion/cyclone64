#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "banshee.h"
#include "ufind.h"
#include "persist.h"
#include "banshee_persist_kinds.h"
struct node { struct node *next; uint64_t value; };
static int update_node(translation t, void *obj) {
  struct node *n = obj;
  UPDATEPOINTER(t, &n->next);
  return sizeof(*n);
}
int main(void) {
  engine_init();
  char dir[] = "/tmp/banshee64-XXXXXX", data[128], state[128];
  assert(mkdtemp(dir));
  snprintf(data, sizeof(data), "%s/data", dir);
  snprintf(state, sizeof(state), "%s/state", dir);
  region r = newregion(), temp = newregion(), regions[] = {r, NULL};
  struct node *head = NULL;
  for (int i = 0; i < 10000; ++i) {
    struct node *n = ralloc(r, struct node);
    n->next = head; n->value = UINT64_C(0xfedcba9800000000) + i; head = n;
  }
  assert(serialize(regions, data, state) == 0);
  Updater updates[] = {update_node};
  translation t = deserialize(data, state, updates, temp);
  assert(t); assert(translate_pointer(t, NULL) == NULL);
  struct node *copy = TRANSLATEPOINTER(t, head);
  region restored = regionof(copy);
  assert(copy && copy != head && restored != r);
  for (struct node *a = head, *b = copy; a; a = a->next, b = b->next) {
    assert(b && b != a && b->value == a->value);
    if (!a->next) assert(!b->next);
  }
  struct node *extra = ralloc(restored, struct node);
  extra->value = 42; assert(copy->value == UINT64_C(0xfedcba980000270f));
  delete_translation(t); deleteregion(restored); deleteregion(r); deleteregion(temp);
  /* Independent object graph persistence with a shared union-find class. */
  r = newregion();
  uf_element a = new_uf_element(r, (void *)(uintptr_t)UINT64_C(0x123456789abcdef0), 0);
  uf_element b = new_uf_element(r, NULL, 0);
  uf_union(a, b);
  void *info = (void *)(uintptr_t)UINT64_C(0x123456789abcdef0);
  uf_update(a, info);
  assert(uf_get_info(b) == info);
  FILE *f = tmpfile(); assert(f);
  banshee_serialize_start(f);
  assert(serialize_banshee_object(a, uf_element));
  assert(serialize_banshee_object(b, uf_element));
  banshee_serialize_end(); rewind(f);
  banshee_deserialize_all(f);
  uf_element aa = deserialize_get_obj(a), bb = deserialize_get_obj(b);
  assert(aa && bb && aa != a && bb != b && uf_eq(aa, bb));
  assert(uf_get_info(aa) == info);
  banshee_deserialize_end(); fclose(f);
  unlink(data); unlink(state); rmdir(dir);
  puts("persistence: relocated 10000-node regions and union-find graph: PASS");
}

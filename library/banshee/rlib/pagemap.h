/* Sparse page map. Keys retain every bit of uintptr_t; no virtual-address
 * layout assumption is made. NULL values remove entries. Not thread safe,
 * like the region allocator itself. */
#ifndef BANSHEE_PAGEMAP_H
#define BANSHEE_PAGEMAP_H
#include <stdint.h>
#include <stdlib.h>

struct page_map_entry {
  uintptr_t key;
  void *value;
  struct page_map_entry *next;
};
struct page_map {
  size_t capacity, count;
  struct page_map_entry **buckets;
};
static size_t page_map_hash(uintptr_t key, size_t capacity) {
  key ^= key >> 17;
  key *= (uintptr_t)0xed5ad4bbU;
  key ^= key >> 11;
  return key & (capacity - 1);
}
static void *page_map_get(const struct page_map *map, uintptr_t key) {
  if (!map->capacity) return NULL;
  for (struct page_map_entry *e = map->buckets[page_map_hash(key, map->capacity)]; e; e = e->next)
    if (e->key == key) return e->value;
  return NULL;
}
static void page_map_grow(struct page_map *map) {
  size_t capacity = map->capacity ? map->capacity * 2 : 256;
  if (capacity < map->capacity || capacity > SIZE_MAX / sizeof(*map->buckets)) abort();
  struct page_map_entry **buckets = calloc(capacity, sizeof(*buckets));
  if (!buckets) abort();
  for (size_t i = 0; i < map->capacity; ++i) {
    struct page_map_entry *e = map->buckets[i];
    while (e) {
      struct page_map_entry *next = e->next;
      size_t slot = page_map_hash(e->key, capacity);
      e->next = buckets[slot];
      buckets[slot] = e;
      e = next;
    }
  }
  free(map->buckets);
  map->buckets = buckets;
  map->capacity = capacity;
}
static void page_map_set(struct page_map *map, uintptr_t key, void *value) {
  if (!map->capacity) {
    if (!value) return;
    page_map_grow(map);
  }
  struct page_map_entry **slot = &map->buckets[page_map_hash(key, map->capacity)];
  while (*slot && (*slot)->key != key) slot = &(*slot)->next;
  if (*slot) {
    if (value) (*slot)->value = value;
    else {
      struct page_map_entry *old = *slot;
      *slot = old->next;
      free(old);
      --map->count;
    }
  } else if (value) {
    struct page_map_entry *e = malloc(sizeof(*e));
    if (!e) abort();
    *e = (struct page_map_entry){key, value, NULL};
    *slot = e;
    if (++map->count > map->capacity) page_map_grow(map);
  }
}
static void page_map_destroy(struct page_map *map) {
  for (size_t i = 0; i < map->capacity; ++i) {
    struct page_map_entry *e = map->buckets[i];
    while (e) {
      struct page_map_entry *next = e->next;
      free(e);
      e = next;
    }
  }
  free(map->buckets);
  *map = (struct page_map){0};
}
#endif

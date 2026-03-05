#ifndef ARENA_H
#define ARENA_H
#include "core.h"

struct Arena {
  byte *buffer;
  usize capacity;
  usize offset;
};

//
// Arena helpers
//

inline usize align_forward(usize ptr, usize align) {
  usize mod = ptr & (align - 1);
  if (mod != 0)
    ptr += align - mod;
  return ptr;
}

//
// Arena core
//

inline Arena arena_init(usize capacity) {
  Arena arena = {};

  arena.buffer = cast<byte *>(malloc(capacity));
  arena.capacity = capacity;
  arena.offset = 0;

  if (!arena.buffer) {
    fprintf(stderr, "Arena allocation failed\n");
    abort();
  }

  return arena;
}

inline void arena_destroy(Arena *arena) {
  free(arena->buffer);
  arena->buffer = nullptr;
  arena->capacity = 0;
  arena->offset = 0;
}

inline void arena_reset(Arena *arena) { arena->offset = 0; }

//
// Arena alloc
//

inline void *arena_alloc(Arena *arena, usize size, usize align = 8) {

  usize ptr = cast<usize>(arena->buffer + arena->offset);
  usize aligned = align_forward(ptr, align);

  usize new_offset = (aligned - cast<usize>(arena->buffer)) + size;

  if (new_offset > arena->capacity)
    return nullptr;

  void *result = arena->buffer + (aligned - cast<usize>(arena->buffer));
  arena->offset = new_offset;

  return result;
}

inline void *arena_alloc_zero(Arena *arena, usize size, usize align = 8) {
  void *ptr = arena_alloc(arena, size, align);
  if (ptr)
    memset(ptr, 0, size);
  return ptr;
}

//
// Snapshot (temp arena)
//

struct ArenaTemp {
  Arena *arena;
  usize offset;
};

inline ArenaTemp arena_temp_begin(Arena *arena) {
  ArenaTemp t;
  t.arena = arena;
  t.offset = arena->offset;
  return t;
}

inline void arena_temp_end(ArenaTemp temp) { temp.arena->offset = temp.offset; }

//
// Arena allocator wrapper
//

inline void *arena_allocator_alloc(Allocator *a, usize size) {
  Arena *arena = cast<Arena *>(a->user_data);
  return arena_alloc(arena, size);
}

inline void arena_allocator_dealloc(Allocator *, void *) {
  // arena does not free individual allocations
}

inline void arena_allocator_reset(Allocator *a) {
  Arena *arena = cast<Arena *>(a->user_data);
  arena_reset(arena);
}

inline Allocator arena_allocator(Arena *arena) {

  Allocator a = {};

  a.user_data = arena;
  a.alloc = arena_allocator_alloc;
  a.dealloc = arena_allocator_dealloc;
  a.reset = arena_allocator_reset;

  return a;
}

//
// Stats
//

inline usize arena_used(Arena *arena) { return arena->offset; }

inline usize arena_remaining(Arena *arena) {
  return arena->capacity - arena->offset;
}

inline void arena_print_stats(Arena *arena) {

  double used = arena->offset / 1024.0;
  double cap = arena->capacity / 1024.0;

  printf("Arena: %.2f / %.2f KB (%.1f%%)\n", used, cap, (used / cap) * 100.0);
}

#endif
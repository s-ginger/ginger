#ifndef CORE_H
#define CORE_H

#include <utility>
#if defined(__cplusplus)
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

typedef int8_t i8;
typedef uint8_t u8;

typedef int16_t i16;
typedef uint16_t u16;

typedef int32_t i32;
typedef uint32_t u32;

typedef int64_t i64;
typedef uint64_t u64;

typedef size_t usize;
typedef ptrdiff_t isize;

typedef float f32;
typedef double f64;

typedef u8 byte;

typedef i32 b32; // bool 32
typedef u8 b8;   // bool 8

template <typename T, typename U> inline T cast(U value) {
  return static_cast<T>(value);
}

template <typename T> constexpr usize size_of() { return sizeof(T); }

template <typename T> constexpr usize count_of(T &arr) {
  return sizeof(arr) / sizeof(arr[0]);
}

// --- Allocator ---

typedef struct Allocator {
  void *user_data;

  void *(*alloc)(struct Allocator *a, usize size);
  void (*dealloc)(struct Allocator *a, void *ptr);
  void (*reset)(struct Allocator *a);
} Allocator;

static inline void *alloc(Allocator *a, usize size) {
  return a && a->alloc ? a->alloc(a, size) : NULL;
}

static inline void dealloc(Allocator *a, void *ptr) {
  if (a && a->dealloc)
    a->dealloc(a, ptr);
}

static inline void reset(Allocator *a) {
  if (a && a->reset)
    a->reset(a);
}

// --- Arena Storage ---

struct Arena {
  byte *buffer;
  usize capacity;
  usize offset;
};

// Снимок состояния для отката
struct ArenaTemp {
  Arena *arena;
  usize prev_offset;
};

// --- Arena Core Functions ---

inline Arena arena_init(usize innitial_capacity) {
  Arena arena = {};
  arena.buffer = cast<byte *>(malloc(innitial_capacity));
  arena.capacity = innitial_capacity;
  arena.offset = 0;
  return arena;
}

inline void *arena_alloc(Arena *arena, usize size) {
  // Выравнивание по 8 байт
  usize aligned_size = (size + 7) & ~cast<usize>(7);

  if (arena->offset + aligned_size > arena->capacity) {
    usize new_capacity = arena->capacity * 2;
    if (new_capacity < arena->offset + aligned_size) {
      new_capacity = arena->offset + aligned_size + 1024;
    }

    byte *new_buffer = cast<byte *>(realloc(arena->buffer, new_capacity));
    if (!new_buffer)
      return nullptr;

    arena->buffer = new_buffer;
    arena->capacity = new_capacity;
  }

  void *ptr = &arena->buffer[arena->offset];
  arena->offset += aligned_size;
  return ptr;
}

inline void arena_reset(Arena *arena) { arena->offset = 0; }

inline void arena_destroy(Arena *arena) {
  free(arena->buffer);
  arena->buffer = nullptr;
  arena->capacity = 0;
  arena->offset = 0;
}

// --- Arena Temp/Snapshot ---

inline ArenaTemp arena_temp_begin(Arena *arena) {
  return {arena, arena->offset};
}

inline void arena_temp_end(ArenaTemp temp) {
  temp.arena->offset = temp.prev_offset;
}

// --- Helpers & Stats ---

inline void arena_print_stats(Arena *arena) {
  f32 used_kb = cast<f32>(arena->offset) / 1024.0f;
  f32 cap_kb = cast<f32>(arena->capacity) / 1024.0f;
  printf("Arena usage: %.2f / %.2f KB (%.1f%%)\n", used_kb, cap_kb,
         (used_kb / cap_kb) * 100.0f);
}

#define push_struct(arena, type) cast<type *>(arena_alloc(arena, sizeof(type)))
#define push_array(arena, type, count)                                         \
  cast<type *>(arena_alloc(arena, sizeof(type) * (count)))

#endif
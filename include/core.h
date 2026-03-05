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

// --- Context ---

typedef struct Context {
  void *data;
  Allocator *allocator;
} Context;


#define ASSERT(x)                                                              \
  if (!(x)) {                                                                  \
    fprintf(stderr, "ASSERT FAILED %s:%d\n", __FILE__, __LINE__);              \
    abort();                                                                   \
  }

#endif
#ifndef CORE_H
#define CORE_H

#include <utility>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>


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

template <typename F>
struct Defer {
    F f;
    Defer(F&& f) : f(std::forward<F>(f)) {}
    ~Defer() { f(); }
};
static const struct DeferHelper {} defer_helper_inst;
template <typename F> 
inline Defer<F> operator+(DeferHelper, F &&f) {
    return Defer<F>(std::forward<F>(f));
}
#define defer const auto defer_##__COUNTER__ = defer_helper_inst + [&]()

// --- Context ---

typedef struct Context {
  void *data;
} Context;


#define ASSERT(x)                                                              \
  if (!(x)) {                                                                  \
    fprintf(stderr, "ASSERT FAILED %s:%d\n", __FILE__, __LINE__);              \
    abort();                                                                   \
  }

#endif
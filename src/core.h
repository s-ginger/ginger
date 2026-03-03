#ifndef CORE_H
#define CORE_H
#include <utility>
#if defined(__cplusplus)
#include <cstdint> // int8_t, uint32_t, etc
#else
#include <stdint.h>
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

template <typename T, typename U> inline T cast(U value) { return static_cast<T>(value); }

template <typename T> constexpr usize size_of() { return sizeof(T); }

template <typename T> constexpr usize count_of(T &arr) { return sizeof(arr) / sizeof(arr[0]); }


#endif
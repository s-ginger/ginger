#ifndef ARENA_H
#define ARENA_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h> // для u8, uint8_t
#include <stddef.h> // для size_t

typedef uint8_t u8;
typedef size_t usize;

typedef struct Arena {
    u8 *buffer;   // блок памяти
    usize size;   // общий размер блока
    usize used;   // сколько уже выделено
} Arena;

// Инициализация фиксированной арены
inline Arena* arena_create(usize size) {
    Arena *arena = (Arena*)malloc(sizeof(Arena));
    if (!arena) return NULL;

    arena->buffer = (u8*)malloc(size);
    if (!arena->buffer) {
        free(arena);
        return NULL;
    }

    arena->size = size;
    arena->used = 0;
    return arena;
}

// Выделение памяти в арене
inline void* arena_alloc(Arena *arena, usize bytes) {
    usize align = 8;
    usize offset = (arena->used + (align - 1)) & ~(align - 1);

    if (offset + bytes > arena->size) {
        // Памяти не хватает
        return NULL;
    }

    void *ptr = arena->buffer + offset;
    arena->used = offset + bytes;
    return ptr;
}

// Сброс арены
inline void arena_reset(Arena *arena) {
    arena->used = 0;
}

// Полное освобождение
inline void arena_destroy(Arena *arena) {
    free(arena->buffer);
    free(arena);
}

#endif // ARENA_H
#ifndef DRAGONTONGUE_H
#define DRAGONTONGUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// Version
#define DRAGONTONGUE_VERSION "0.0.1"

// Debug macro
#ifdef DEBUG
    #define LOG(msg) fprintf(stderr, "[DEBUG] %s\n", msg)
#else
    #define LOG(msg)
#endif

// Arena allocator (simple memory pool)
typedef struct Arena {
    char* memory;
    size_t capacity;
    size_t used;
} Arena;

Arena* arena_new(size_t capacity);
void* arena_alloc(Arena* arena, size_t size);
void arena_free(Arena* arena);

#endif
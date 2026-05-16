#include "dragontongue.h"
#include <assert.h>

Arena* arena_new(size_t capacity) {
    Arena* arena = malloc(sizeof(Arena));
    arena->memory = malloc(capacity);
    arena->capacity = capacity;
    arena->used = 0;
    LOG("Arena created");
    return arena;
}

void* arena_alloc(Arena* arena, size_t size) {
    assert(arena->used + size <= arena->capacity);
    void* ptr = arena->memory + arena->used;
    arena->used += size;
    return ptr;
}

void arena_free(Arena* arena) {
    free(arena->memory);
    free(arena);
    LOG("Arena freed");
}
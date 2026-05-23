#ifndef DRAGONTONGUE_RUNTIME_H
#define DRAGONTONGUE_RUNTIME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// String type
typedef struct {
    char* data;
    int64_t length;
    int64_t capacity;
} dt_string_t;

// String functions
dt_string_t dt_string_new(const char* cstr);
dt_string_t dt_string_new_empty(void);
dt_string_t dt_string_concat(dt_string_t a, dt_string_t b);
dt_string_t dt_string_from_int(int64_t n);
int64_t dt_string_length(dt_string_t s);
void dt_string_free(dt_string_t* s);

// Built-in functions
void dt_print(dt_string_t s);
void dt_println(dt_string_t s);
dt_string_t dt_to_string(int64_t n);
int64_t dt_to_int(dt_string_t s);

// Memory
void* dt_alloc(size_t size);
void dt_free(void* ptr);

#endif
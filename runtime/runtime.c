#include "runtime.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

// String functions
dt_string_t dt_string_new(const char* cstr) {
    dt_string_t s;
    s.length = strlen(cstr);
    s.capacity = s.length + 1;
    s.data = (char*)dt_alloc(s.capacity);
    strcpy(s.data, cstr);
    return s;
}

dt_string_t dt_string_new_empty(void) {
    dt_string_t s;
    s.length = 0;
    s.capacity = 1;
    s.data = (char*)dt_alloc(1);
    s.data[0] = '\0';
    return s;
}

dt_string_t dt_string_concat(dt_string_t a, dt_string_t b) {
    dt_string_t result;
    result.length = a.length + b.length;
    result.capacity = result.length + 1;
    result.data = (char*)dt_alloc(result.capacity);
    
    strcpy(result.data, a.data);
    strcat(result.data, b.data);
    
    return result;
}

dt_string_t dt_string_from_int(int64_t n) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%lld", (long long)n);
    return dt_string_new(buffer);
}

int64_t dt_string_length(dt_string_t s) {
    return s.length;
}

void dt_string_free(dt_string_t* s) {
    if (s->data) {
        dt_free(s->data);
        s->data = NULL;
        s->length = 0;
        s->capacity = 0;
    }
}

// Built-in functions
void dt_print(dt_string_t s) {
    printf("%s", s.data);
}

void dt_println(dt_string_t s) {
    printf("%s\n", s.data);
}

dt_string_t dt_to_string(int64_t n) {
    return dt_string_from_int(n);
}

int64_t dt_to_int(dt_string_t s) {
    return atoll(s.data);
}

// Memory management
void* dt_alloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Runtime error: Out of memory\n");
        exit(1);
    }
    return ptr;
}

void dt_free(void* ptr) {
    free(ptr);
}
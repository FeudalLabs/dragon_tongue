# DragonTongue Runtime Library API Reference

**Version:** 1.0  
**Status:** ✅ Implemented in Day 6

## Table of Contents

1. [Overview](#overview)
2. [String Type](#string-type)
3. [String Functions](#string-functions)
4. [Built-in Functions](#built-in-functions)
5. [Memory Management](#memory-management)
6. [Usage Examples](#usage-examples)
7. [Integration Guide](#integration-guide)

## Overview

The DragonTongue Runtime Library provides essential functionality that C doesn't have natively:

- **String handling** - Creation, concatenation, length
- **Type conversion** - Integer to string, string to integer
- **Output functions** - Print with and without newlines
- **Memory management** - Allocation and deallocation

### Runtime Library Files

| File                | Purpose                         |
|---------------------|---------------------------------|
| `runtime/runtime.h` | Function declarations and types |
| `runtime/runtime.c` | Function implementations        |

## String Type

### `dt_string_t`

The core string type used by DragonTongue.

```c
typedef struct {
    char* data;      // Pointer to string data
    int64_t length;  // Current string length
    int64_t capacity; // Allocated memory size
} dt_string_t;
```
### Memory Layout
```
dt_string_t
┌────────────┬────────────┬────────────┐
│ data       │ length     │ capacity   │
│ (8 bytes)  │ (8 bytes)  │ (8 bytes)  │
└────────────┴────────────┴────────────┘
     │
     ▼
┌─────────────────────────────┐
│ 'H' 'e' 'l' 'l' 'o' '\0'    │
└─────────────────────────────┘
```
## String Functions

### `dt_string_t dt_string_new(const char* cstr)`

Creates a new string from a C string.

**Parameters:**

- `cstr` - Null-terminated C string

**Returns:** New `dt_string_t` with copied data

**Example:**

```c
dt_string_t s = dt_string_new("Hello");
// s.data = "Hello", s.length = 5, s.capacity = 6
```

### `dt_string_t dt_string_new_empty(void)`

Creates an empty string.

**Returns:** Empty `dt_string_t` with capacity 1

**Example:**

```c
dt_string_t s = dt_string_new_empty();
// s.data = "", s.length = 0, s.capacity = 1
```

### `dt_string_t dt_string_concat(dt_string_t a, dt_string_t b)`

Concatenates two strings.

**Parameters:**

- `a` - First string

- `b` - Second string

**Returns:** New string containing `a` + `b`

**Example:**

```c
dt_string_t hello = dt_string_new("Hello");
dt_string_t world = dt_string_new(" World");
dt_string_t result = dt_string_concat(hello, world);
// result.data = "Hello World"
```

### `dt_string_t dt_string_from_int(int64_t n)`

Converts an integer to a string.

**Parameters:**

- `n` - Integer value

**Returns:** String representation of the integer

**Example:**

```c
dt_string_t s = dt_string_from_int(42);
// s.data = "42"
```

### `int64_t dt_string_length(dt_string_t s)`

Returns the length of a string.

**Parameters:**

- `s` - String to measure

**Returns:** Number of characters in the string

**Example:**

```c
dt_string_t s = dt_string_new("Hello");
int64_t len = dt_string_length(s);  // len = 5
```

### `void dt_string_free(dt_string_t* s)`

Frees the memory used by a string.

**Parameters:**

- `s` - Pointer to string to free

**Example:**
```c
dt_string_t s = dt_string_new("Hello");
dt_string_free(&s);  // s.data becomes NULL
```

## Built-in Functions

### `void dt_print(dt_string_t s)`

Prints a string to stdout without a newline.

Parameters:

- `s` - String to print

**Example:**

```c
dt_string_t s = dt_string_new("Hello");
dt_print(s);        // Output: Hello (no newline)
```

### `void dt_println(dt_string_t s)`

Prints a string to stdout with a newline.

**Parameters:**

- `s` - String to print

**Example:**
```c
dt_string_t s = dt_string_new("Hello");
dt_println(s);      // Output: Hello\n
```

### `dt_string_t dt_to_string(int64_t n)`

Converts an integer to a string (alias for `dt_string_from_int`).

**Parameters:**

- `n` - Integer value

**Returns:** String representation

**Example:**

```c
dt_string_t s = dt_to_string(42);
// s.data = "42"
```

### `int64_t dt_to_int(dt_string_t s)`

Converts a string to an integer.

**Parameters:**

- `s` - String to convert

**Returns:** Integer value (0 if invalid)

**Example:**
```c
dt_string_t s = dt_string_new("123");
int64_t n = dt_to_int(s);  // n = 123
```

## Memory Management

### `void* dt_alloc(size_t size)`

Allocates memory with error handling.

**Parameters:**

- `size` - Number of bytes to allocate

**Returns:** Pointer to allocated memory

**Exits on failure:** Prints error and exits

**Example:**

```c
char* buffer = dt_alloc(100);
```

### `void dt_free(void* ptr)`

Frees allocated memory.

**Parameters:**

- `ptr` - Pointer to memory to free

**Example:**

```c
char* buffer = dt_alloc(100);
dt_free(buffer);
```

## Usage Examples

### Example 1: Basic String Operations

```c
#include "runtime.h"

int main() {
    dt_string_t greeting = dt_string_new("Hello");
    dt_string_t name = dt_string_new("World");
    dt_string_t message = dt_string_concat(greeting, name);
    
    dt_println(message);
    
    dt_string_free(&greeting);
    dt_string_free(&name);
    dt_string_free(&message);
    
    return 0;
}
```

### Example 2: String Conversion

```c
#include "runtime.h"

int main() {
    dt_string_t num_str = dt_string_from_int(42);
    dt_println(num_str);  // "42"
    
    int64_t num = dt_to_int(num_str);  // 42
    
    dt_string_t result = dt_to_string(num * 2);
    dt_println(result);  // "84"
    
    dt_string_free(&num_str);
    dt_string_free(&result);
    
    return 0;
}
```

### Example 3: String Length
```c
#include "runtime.h"

int main() {
    dt_string_t text = dt_string_new("DragonTongue");
    int64_t len = dt_string_length(text);
    
    dt_string_t len_str = dt_to_string(len);
    dt_print(len_str);
    dt_println(dt_string_new(" characters"));
    
    dt_string_free(&text);
    dt_string_free(&len_str);
    
    return 0;
}
```

## Integration Guide

### How DragonTongue Uses the Runtime

When DragonTongue compiles code, it generates C that calls these runtime functions:

| DragonTongue    | Generated C                    |
|-----------------|--------------------------------|
| `"hello"`       | `"hello"` (direct C string)    |
| `str1 + str2`   | `dt_string_concat(str1, str2)` |
| `to_string(42)` | `dt_to_string(42)`             |
| `to_int("123")` | `dt_to_int("123")`             |
| `len("hello")`  | `dt_string_length("hello")`    |

### Compilation Command

The compiler generates this command to build the final executable:

```bash
gcc -o output runtime/runtime.c output.c -I./runtime
```

### Header Inclusion

All generated C files include the runtime header:

```c
#include "runtime.h"
```

### Performance Considerations

| Operation          | Time Complexity | Notes                |
|--------------------|-----------------|----------------------|
| `dt_string_new`    | O(n)            | Copies input string  |
| `dt_string_concat` | O(n+m)          | Allocates new memory |
| `dt_string_length` | O(1)            | Uses cached length   |
| `dt_to_string`     | O(log n)        | sprintf conversion   |
| `dt_to_int`        | O(n)            | Parses string        |

### Error Handling

### Allocation Failure

```c
void* dt_alloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Runtime error: Out of memory\n");
        exit(1);
    }
    return ptr;
}
```

### nvalid Conversion

`dt_to_int` returns `0` for invalid strings:

```c
dt_string_t invalid = dt_string_new("not a number");
int64_t result = dt_to_int(invalid);  // result = 0
```

## Future Extensions

| Function            | Purpose              | Planned For |
|---------------------|----------------------|-------------|
| `dt_string_substr`  | Extract substring    | v1.1        |
| `dt_string_find`    | Search in string     | v1.1        |
| `dt_string_replace` | Replace substring    | v1.2        |
| `dt_string_split`   | Split by delimiter   | v1.2        |
| `dt_file_read`      | Read file to string  | v1.0        |
| `dt_file_write`     | Write string to file | v1.0        |

## Quick Reference Card

| Function                | Purpose              | Example                    |
|-------------------------|----------------------|----------------------------|
| `dt_string_new(cstr)`   | Create string        | `dt_string_new("hi")`      |
| `dt_string_new_empty()` | Empty string         | `dt_string_new_empty()`    |
| `dt_string_concat(a,b)` | Concatenate          | `dt_string_concat(s1, s2)` |
| `dt_string_from_int(n)` | Int to string        | `dt_string_from_int(42)`   |
| `dt_string_length(s)`   | Get length           | `dt_string_length(str)`    |
| `dt_string_free(s)`     | Free memory          | `dt_string_free(&str)`     |
| `dt_print(s)`           | Print (no newline)   | `dt_print(str)`            |
| `dt_println(s)`         | Print (with newline) | `dt_println(str)`          |
| `dt_to_string(n)`       | Int to string        | `dt_to_string(42)`         |
| `dt_to_int(s)`          | String to int        | `dt_to_int(str)`           |
| `dt_alloc(size)`        | Allocate memory      | `dt_alloc(100)`            |
| `dt_free(ptr)`          | Free memory          | `dt_free(buffer)`          |

The runtime library powers DragonTongue's high-level features in C. 🐉
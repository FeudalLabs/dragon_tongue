# DragonTongue Built-in Functions Reference

**Version:** 1.0  
**Status:** ✅ Implemented in Day 6

## Table of Contents

1. [Overview](#overview)
2. [Output Functions](#output-functions)
3. [Type Conversion Functions](#type-conversion-functions)
4. [String Functions](#string-functions)
5. [File I/O Functions](#file-io-functions)
6. [Usage Examples](#usage-examples)
7. [Implementation Details](#implementation-details)

## Overview

DragonTongue provides built-in functions that are always available without any imports. These functions handle common tasks like output, type conversion, and string manipulation.

### Available Built-in Functions

| Category        | Functions                               |
|-----------------|-----------------------------------------|
| Output          | `println()`, `print()`                  |
| Type Conversion | `to_string()`, `to_int()`               |
| String          | `len()`                                 |
| File I/O        | `read_file()`, `write_file()` (planned) |

## Output Functions

### `println(value)`

Prints a value to stdout followed by a newline.

**Parameters:**
- `value` - String or integer to print (optional)

**Returns:** Nothing (void)

**Examples:**
```dragontongue
println("Hello, World!")     // Prints: Hello, World!
println(42)                  // Prints: 42
println()                    // Prints: (empty line)
println("The answer is: " + to_string(42))  // String concatenation
```

**C Implementation:**

```c
// For integers
printf("%d\n", value);

// For strings
puts(value);

// For empty
printf("\n");
```

### `print(value)`

Prints a value to stdout without a newline.

**Parameters:**

- `value` - String or integer to print

**Returns:** Nothing (void)

**Examples:**

```
print("Loading")             // Prints: Loading (no newline)
print("...")                 // Prints: ... (no newline)
print(42)                    // Prints: 42 (no newline)

// Useful for progress indicators
print("Progress: ")
for i in 0..100 {
    print(".")
}
println(" Done!")
```

**C Implementation:**

```c
// For integers
printf("%d", value);

// For strings
printf("%s", value);
```

## Type Conversion Functions

### `to_string(value)`

Converts an integer to a string.

**Parameters:**

- `value` - Integer to convert

**Returns:** String representation of the integer

**Examples:**

```
let num = 42
let str = to_string(num)      // str = "42"

let message = "Value: " + to_string(100)  // "Value: 100"

println("The answer is " + to_string(7 * 6))  // "The answer is 42"
```

**C Implementation:**

```c
({ char buf[32]; sprintf(buf, "%d", value); strdup(buf); })
```
### `to_int(string)`

Converts a string to an integer.

**Parameters:**

- `string` - String to convert

**Returns:** Integer value (0 if conversion fails)

**Examples:**

```
let str = "123"
let num = to_int(str)         // num = 123

let user_input = "42"
let answer = to_int(user_input) + 8  // answer = 50

// Invalid conversion returns 0
let invalid = to_int("not a number")  // invalid = 0
```

**C Implementation:**

```c
atoi(string)
```

## String Functions

### `len(string)`

Returns the length of a string.

**Parameters:**

- `string` - String to measure

**Returns:** Number of characters in the string

**Examples:**
```
let name = "DragonTongue"
let length = len(name)        // length = 12

let empty = ""
let empty_len = len(empty)    // empty_len = 0

if len(input) > 0 {
    println("Input not empty")
}
```

**C Implementation:**

```c
strlen(string)
```


## File I/O Functions (Planned)

### `read_file(path)`

Reads entire file content into a string. (Coming in v1.0)

**Parameters:**

- `path` - File path string

**Returns:** File content as string, or error

**Example (future)**:

```
let content = read_file("data.txt")
if content != "" {
    println(content)
}
```

### 'write_file(path, content)'

Writes string content to a file. (Coming in v1.0)

**Parameters:**

- `path` - File path string

- `content` - String to write

**Returns:** Success/failure status

**Example (future):**

```
write_file("output.txt", "Hello, World!")
```

## Usage Examples

### Example 1: Basic Output

```
// Different print styles
println("Hello")      // With newline
print("World")        // Without newline
println()             // Just newline

// Output:
// Hello
// World
// (empty line)
```

### Example 2: Type Conversion

```
let number = 42
let text = "The answer is " + to_string(number)
println(text)         // "The answer is 42"

let user_input = "100"
let value = to_int(user_input)
let doubled = value * 2
println("Double is: " + to_string(doubled))  // "Double is: 200"
```

### Example 3: String Length

```
let password = "secret123"
if len(password) < 8 {
    println("Password too short")
} else {
    println("Password OK")
}

let name = "DragonTongue"
let greeting = "Hello, " + name
println("Message length: " + to_string(len(greeting)))
```

### Example 4: Combined Usage

```
fn greet(name: String) {
    let message = "Hello, " + name + "!"
    println(message)
    println("Your name has " + to_string(len(name)) + " letters")
}

fn main() {
    let user = "Alice"
    greet(user)
    
    let number = 42
    print("The number is: ")
    println(to_string(number))
}
```

## Implementation Details

### How Built-ins Are Recognized

The parser identifies built-in functions by name:

```c
bool is_builtin = (strcmp(name, "println") == 0 || 
                   strcmp(name, "print") == 0 ||
                   strcmp(name, "len") == 0 ||
                   strcmp(name, "to_string") == 0 ||
                   strcmp(name, "to_int") == 0 ||
                   strcmp(name, "read_file") == 0 ||
                   strcmp(name, "write_file") == 0);
```

### How Built-ins Are Generated

The code generator translates each built-in to C code:

| Built-in              | Generated C                                               |
|-----------------------|-----------------------------------------------------------|
| `println(x)` (int)    | `printf("%d\n", x)`                                       |
| `println(s)` (string) | `puts(s)`                                                 |
| `println()`           | `printf("\n")`                                            |
| `print(x)` (int)      | `printf("%d", x)`                                         |
| `print(s)` (string)   | `printf("%s", s)`                                         |
| `to_string(x)`        | `({ char buf[32]; sprintf(buf, "%d", x); strdup(buf); })` |
| `to_int(s)`           | `atoi(s)`                                                 |
| `len(s)`              | `strlen(s)`                                               |

## Error Handling

### Invalid Conversions

```
let result = to_int("not a number")
// result = 0 (silent failure)

let empty = to_int("")
// empty = 0
```

## Future Improvements

| Improvement       | Description                            | Target |
|-------------------|----------------------------------------|--------|
| Error returns     | Return Option type instead of 0        | v1.1   |
| Float conversion  | `to_float()` and `to_int()` for floats | v1.1   |
| More string funcs | `substr()`, `find()`, `replace()`      | v1.2   |
| File I/O          | `read_file()`, `write_file()`          | v1.0   |

## Quick Reference Card

| Function       | Input           | Output | Example                  |
|----------------|-----------------|--------|--------------------------|
| `println(x)`   | Int/String/Void | Void   | `println("Hi")`          |
| `print(x)`     | Int/String      | Void   | `print("Hi")`            |
| `to_string(x)` | Int             | String | `to_string(42)` → `"42"` |
| `to_int(s)`    | String          | Int    | `to_int("42")` → `42`    |
| `len(s)`       | String          | Int    | `len("hello")` → `5`     |

## Print Behavior

| Call               | Output    |
|--------------------|-----------|
| `println("Hello")` | `Hello\n` |
| `println(42)`      | `42\n`    |
| `println()`        | `\n`      |
| `print("Hello")`   | `Hello`   |
| `print(42)`        | `42`      |

Built-in functions make DragonTongue practical for everyday programming. 🐉
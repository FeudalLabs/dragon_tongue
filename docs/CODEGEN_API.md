# DragonTongue Code Generator API Reference

**Version:** 1.0  
**Status:** ✅ Implemented in Day 6


## Table of Contents

1. [Overview](#overview)
2. [Code Generator Structure](#code-generator-structure)
3. [API Functions](#api-functions)
4. [Internal Functions](#internal-functions)
5. [Expression Generation](#expression-generation)
6. [Statement Generation](#statement-generation)
7. [Runtime Library Integration](#runtime-library-integration)
8. [Usage Example](#usage-example)


## Overview

The Code Generator transforms the DragonTongue AST into executable C code. It handles:

- Variable declarations (integers and strings)
- Arithmetic and comparison operations
- If-else statements
- Function definitions and calls
- Built-in function calls
- String concatenation
- Block statements

### Design Philosophy

| Principle | Implementation |
|-----------|----------------|
| **Simple output** | Generate readable C code |
| **Type-aware** | Detect int vs string at generation time |
| **Minimal runtime** | Only essential helper functions |
| **Transparent** | Easy to debug generated code |


## Code Generator Structure

### Header: `src/codegen/cgen.h`

```c
typedef struct {
    FILE* output;           // Output C file stream
    SymbolTable* symbols;   // Symbol table for lookups
    Arena* arena;           // Memory arena
    int indent_level;       // Current indentation level
    int temp_counter;       // Counter for temporary variables
    char* current_function; // Name of function being generated
    bool had_error;         // Error flag
} CodeGenerator;
```

## State Variables

| Variable         | Purpose                                              |
|------------------|------------------------------------------------------|
| output           | File handle for generated C code                     |
| symbols          | Access to symbol table for type info                 |
| indent_level     | Tracks nesting depth for pretty printing             |
| temp_counter     | Creates unique temporary variable names              |
| current_function | Used for return type validation                      |
| had_error        | Signals generation failure                           |

## API Functions

### `CodeGenerator* cgen_new(const char* filename, SymbolTable* symbols, Arena* arena)`

Creates a new code generator and opens the output file.

**Parameters:**

- `filename` - Path to output .c file
- `symbols` - Symbol table from semantic analysis
- `arena` - Memory arena for allocations

**Returns:** New CodeGenerator instance

## Example:
```c
CodeGenerator* cg = cgen_new("output.c", analyzer->symbols, arena);
```

### `void cgen_generate(CodeGenerator* cg, ASTNode* program)`

Generates C code from the AST.

**Parameters:**

- cg - Code generator instance
- program - Root AST node (must be NODE_PROGRAM)

**What it does:**

1. Writes required headers (`#include <stdio.h>`, etc.)
2. Generates function declarations
3. Generates main() function
4. Generates function definitions

## Example:
```c
cgen_generate(cg, ast);
```

### `bool cgen_had_error(CodeGenerator* cg)`

Checks if any errors occurred during generation.

**Parameters:**

- `cg` - Code generator instance

**Returns**: `true` if errors occurred, `false` otherwise

**Example:**

```c
if (cgen_had_error(cg)) {
    fprintf(stderr, "Code generation failed\n");
}
```

### `void cgen_close(CodeGenerator* cg)`

Closes the output file and cleans up.

**Parameters:**

- `cg` - Code generator instance

**Example:**
```c
cgen_close(cg);
```

## Internal Functions

### `static void cgen_indent(CodeGenerator* cg)`

Prints indentation based on current indent_level.

**Output:** 2 spaces per indent level

### `static void cgen_writeln(CodeGenerator* cg, const char* fmt, ...)`

Prints an indented line with format string.

**Example:**
```c
cgen_writeln(cg, "int %s = %d;", var_name, value);
```

### `static int is_integer_node(CodeGenerator* cg, ASTNode* node)`

Determines if an expression evaluates to an integer.

**Logic:**

- `NODE_LITERAL_INT` → integer
- `NODE_VARIABLE` → check variable name (greeting = string, others = int)
- `NODE_BINARY_OP` → if both sides are integers
- `NODE_CALL` → function calls return int by default

## Expression Generation Functions

| Function                          | Node Type               | Output                   |
|-----------------------------------|-------------------------|--------------------------|
| `cgen_generate_literal_int`       | `NODE_LITERAL_INT`      | `42`                     |
| `cgen_generate_literal_string`    | `NODE_LITERAL_STRING`   | `"hello"`                |
| `cgen_generate_variable`          | `NODE_VARIABLE`         | `x`                      |
| `cgen_generate_binary_op`         | `NODE_BINARY_OP`        | `(a + b)`                |
| `cgen_generate_call`              | `NODE_CALL`             | `add(5, 3)`              |
| `cgen_generate_builtin`           | `NODE_BUILTIN_FUNC`     | `println(x)`             |

## Statement Generation Functions

| Function                         | Node Type            | Output                             |
|----------------------------------|----------------------|------------------------------------|
| `cgen_generate_let_statement`    | `NODE_LET_STMT`      | `int x = 42;`                      |
| `cgen_generate_if_statement`     | `NODE_IF_STMT`       | `if (cond) { ... }`                |
| `cgen_generate_return_statement` | `NODE_RETURN_STMT`   | `return value;`                    |
| `cgen_generate_block_statement`  | `NODE_BLOCK_STMT`    | `{ ... }`                          |
| `cgen_generate_function`         | `NODE_FUNCTION`      | `int add(int a, int b) { ... }`    |

## Expression Generation

### Integer Literals

**Input AST:**
```c
LiteralInt { value: 42 }
```

**Generated C:**
```c
42
```

### String Literals

**Input AST:**
```c
LiteralString { value: "hello" }
```

### Binary Operations

**Input AST:**
```c
BinaryOp { op: "+", left: LiteralInt(5), right: LiteralInt(3) }
```
**Generated C:**
```c
(5 + 3)
```

### Function Calls

**Input AST:**
```c
Call { name: "add", arguments: [LiteralInt(5), LiteralInt(3)] }
```

**Generated C:**
```c
add(5, 3)
```

## Built-in Functions

| Built-in          | Generated C                                                        |
|-------------------|--------------------------------------------------------------------|
| println(integer)  | printf("%d\n", value)                                              |
| println(string)   | puts(value)                                                        |
| println()         | printf("\n")                                                       |
| print(integer)    | printf("%d", value)                                                |
| print(string)     | printf(value)                                                      |
| to_string(integer)| ({ char buf[32]; sprintf(buf, "%d", value); strdup(buf); })        |
| to_int(string)    | atoi(value)                                                        |
| len(string)       | strlen(value)                                                      |

## Statement Generation

### Let Statement (Integer)

**Input:**

```c
let x = 42
```

**Generated C:**
```c
int x = 42;
```

### Let Statement (String)

**Input:**

```c
let name = "Alice"
```

**Generated C:**
```c
char* name = "Alice";
```

### If Statement

**Input:**
```c
if x > y {
    println("greater")
}
```

**Generated C:**
```c
if ((x > y)) {
    puts("greater");
}
```

### Function Definition

**Input:**
```
fn add(a, b) {
    return a + b
}
```

**Generated C:**
```c
int add(int a, int b) {
    return (a + b);
    return 0;
}
```

## Runtime Library Integration

### String Operations

The runtime library provides string handling that C doesn't have natively:

| DragonTongue    | Generated C                    |
|-----------------|--------------------------------|
| `"hello"`       | `"hello"` (direct)             |
| `str1 + str2`   | `dt_string_concat(str1, str2)` |
| `to_string(42)` | `dt_to_string(42)`             |
| `to_int("123")` | `dt_to_int("123")`             |
| `len("hello")`  | `dt_string_length("hello")`    |

## Runtime Headers

Generated C files include:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
```

The runtime library is compiled separately and linked:

```bash
gcc -o output runtime/runtime.c output.c
```
## Usage Example

Complete example of using the code generator:

```c
#include "dragontongue.h"
#include "lexer/source.h"
#include "lexer/scanner.h"
#include "parser/parser.h"
#include "semantic/analyzer.h"
#include "codegen/cgen.h"

int main(int argc, char** argv) {
    Arena* arena = arena_new(1024 * 1024);
    
    // Parse
    Source* source = source_from_file("program.dt", arena);
    Scanner* scanner = scanner_new(source, arena);
    Parser* parser = parser_new(scanner, arena);
    ASTNode* ast = parser_parse_program(parser);
    
    // Semantic analysis
    SemanticAnalyzer* sa = analyzer_new(arena);
    analyzer_analyze(sa, ast);
    
    if (analyzer_had_error(sa)) {
        return 1;
    }
    
    // Code generation
    CodeGenerator* cg = cgen_new("output.c", sa->symbols, arena);
    cgen_generate(cg, ast);
    
    if (cgen_had_error(cg)) {
        return 1;
    }
    
    cgen_close(cg);
    
    // Compile C code
    system("gcc -o output runtime/runtime.c output.c");
    system("./output");
    
    arena_free(arena);
    return 0;
}
```

## Generated Output Examples

### Simple Program

**DragonTongue:**

```
let x = 42
println(x)
```

**Generated C:**

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  int x = 42;
  printf("%d\n", x);
  return 0;
}
```

### Function with Return

**DragonTongue:**

```c
fn add(a, b) {
    return a + b
}

let sum = add(5, 3)
println(sum)
```

**Generated C:**

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add(int a, int b);
int add(int a, int b) {
    return (a + b);
    return 0;
}

int main() {
    int sum = add(5, 3);
    printf("%d\n", sum);
    return 0;
}
```

## String Concatenation

**DragonTongue:**

```
let greeting = "Hello, "
let name = "World"
let message = greeting + name
println(message)
```

**Generated C:**

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char* greeting = "Hello, ";
    char* name = "World";
    char* message = ({ char* _a = greeting; char* _b = name; char* _r = malloc(strlen(_a) + strlen(_b) + 1); strcpy(_r, _a); strcat(_r, _b); _r; });
    puts(message);
    return 0;
}
```

## Quick Reference Card

| Task                  | Function                                  |
|-----------------------|-------------------------------------------|
| Create generator      | `cgen_new(filename, symbols, arena)`      |
| Generate code         | `cgen_generate(cg, ast)`                  |
| Check errors          | `cgen_had_error(cg)`                      |
| Close generator       | `cgen_close(cg)`                          |
| Detect integer node   | `is_integer_node(cg, node)`               |
| Generate expression   | `cgen_generate_expression(cg, node)`      |
| Generate statement    | `cgen_generate_statement(cg, node)`       |

## Generated C Patterns

| DragonTongue          | C Output                  |
|-----------------------|---------------------------|
| `let x = 42`          | `int x = 42;`             |
| `let s = "hi"`        | `char* s = "hi";`         |
| `println(x) (int)`    | `printf("%d\n", x)`       |
| `println(s) (string)` | `puts(s)`                 |
| `if cond { }`         | `if (cond) { }`           |
| `fn f(a,b) { }`       | `int f(int a, int b) { }` |
| `a + b (int)`         | `(a + b)`                 |
| `a + b (string)`      | `dt_string_concat(a, b)`  |

The code generator turns DragonTongue into real executables. 🐉
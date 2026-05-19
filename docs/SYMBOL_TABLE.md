# DragonTongue Symbol Table Documentation

**Version:** 1.0  
**Status:** ✅ Implemented in Day 5

## Table of Contents

1. [Overview](#overview)
2. [Symbol Table Design](#symbol-table-design)
3. [Scope Management](#scope-management)
4. [Symbol Kinds](#symbol-kinds)
5. [API Reference](#api-reference)
6. [Examples](#examples)
7. [Memory Management](#memory-management)

## Overview

The Symbol Table is the compiler's memory of all named entities in the program: variables, functions, parameters, structs, and built-in functions.

### What the Symbol Table Tracks

| Entity | Kind | Example |
|--------|------|---------|
| Variables | `SYM_VARIABLE` | `let x = 42` |
| Functions | `SYM_FUNCTION` | `fn add(a,b) {...}` |
| Parameters | `SYM_PARAMETER` | `fn add(a: Int, b: Int)` |
| Structs | `SYM_STRUCT` | `struct User {...}` |
| Built-ins | `SYM_BUILTIN` | `println`, `len`, `to_string` |

### Why Symbol Tables Matter

| Problem | How Symbol Table Solves It |
|---------|---------------------------|
| Undefined variables | Lookup fails → error |
| Duplicate definitions | Check current scope before adding |
| Scope isolation | Different scopes have different tables |
| Type information | Store type with each symbol |

## Symbol Table Design

### Data Structure

```c
typedef struct SymbolTable {
    Symbol** entries;        // Hash table (1024 slots)
    int size;                // Table size
    struct SymbolTable* parent;  // Outer scope (for nesting)
    int scope_level;         // Current nesting depth
    Arena* arena;            // Memory arena for allocations
} SymbolTable;
```

### Hash Table Implementation
```c
static unsigned int hash(const char* name) {
    unsigned int h = 0;
    while (*name) {
        h = h * 31 + *name++;
    }
    return h % 1024;
}
```

### Symbol Structure
```c
typedef struct Symbol {
    char* name;              // Symbol name
    SymbolKind kind;         // What kind of symbol
    ASTNode* node;           // Pointer to AST node
    struct Symbol* next;     // For hash table chaining
    int scope_level;         // Which scope it belongs to
    int line;                // Declaration line (for errors)
    int column;              // Declaration column
} Symbol;
```

### Symbol Kinds
```c
typedef enum {
    SYM_VARIABLE,   // Variable binding
    SYM_FUNCTION,   // Function definition
    SYM_PARAMETER,  // Function parameter
    SYM_STRUCT,     // Type definition
    SYM_BUILTIN     // Built-in function
} SymbolKind;
```

### Scope Management

### Scope Hierarchy
DragonTongue has nested scopes:

```
Global Scope (level 0)
├── let global = 42
├── fn outer(x: Int)     ← Function scope (level 1)
│   ├── let y = 10       ← Same scope (level 1)
│   └── if x > y {       ← Block scope (level 2)
│       └── let z = 100  ← Inner scope (level 2)
│       }
└── fn main()            ← Another function scope (level 1)
```

### Scope Operations

| Operation      | Function                      | Description                          |
|----------------|-------------------------------|--------------------------------------|
| Enter scope    | `symtab_enter_scope()`        | Creates new nested scope             |
| Exit scope     | `symtab_exit_scope()`         | Returns to parent scope              |
| Define symbol  | `symtab_define()`             | Adds to current scope                |
| Lookup symbol  | `symtab_lookup()`             | Searches current + parents           |
| Lookup current | `symtab_lookup_current()`     | Searches only current scope          |

### Scope Example
```c
// Global scope
symtab_define(st, "x", SYM_VARIABLE, node, 1, 5);

// Enter function scope
symtab_enter_scope(st);

// Parameter
symtab_define(st, "a", SYM_PARAMETER, node, 2, 8);

// Local variable
symtab_define(st, "y", SYM_VARIABLE, node, 3, 9);

// Enter block scope
symtab_enter_scope(st);

// Inner variable (shadows outer 'y')
symtab_define(st, "y", SYM_VARIABLE, node, 4, 13);

// Exit block scope
symtab_exit_scope(st);

// Exit function scope
symtab_exit_scope(st);
```

### Symbol Kinds

### SYM_VARIABLE

Regular variables created with `let`.
```
let x = 42
let name = "DragonTongue"
```

**Symbol attributes:**

- Name: variable identifier
- Kind: `SYM_VARIABLE`
- Scope level: where defined
- Location: line/column

### SYM_FUNCTION

Functions defined with `fn`.

```
fn add(a, b) {
    return a + b
}
```
**Symbol attributes:**

- Name: function name
- Kind: `SYM_FUNCTION`
- Node: points to Function AST node
- Parameters: stored in function node

### SYM_PARAMETER

Function parameters.

```
fn greet(name: String, age: Int) {
    // name and age are parameters
}
```

**Symbol attributes:**

- Name: parameter name
- Kind: `SYM_PARAMETER`
- Scope level: function scope
- Type: from annotation or inferred

### SYM_STRUCT
User-defined struct types.
```
struct User {
    name: String
    age: Int
}
```

**Symbol attributes:**

- Name: struct name
- Kind: `SYM_STRUCT`
- Node: points to Struct AST node
- Fields: stored in struct node

### SYM_BUILTIN
Built-in functions provided by the language.

```
println("hello")
len("string")
to_string(42)
```

**Built-in symbols automatically added:**

- `println`, `print`
- `len`, `to_string`, `to_int`
- `read_file`, `write_file`

### API Reference
### Creation
```c
SymbolTable* symtab_new(Arena* arena);
```

Creates a new empty symbol table.

**Example:**

```c
Arena* arena = arena_new(1024 * 1024);
SymbolTable* st = symtab_new(arena);
```

### Scope Management

```c
void symtab_enter_scope(SymbolTable* st);
void symtab_exit_scope(SymbolTable* st);
```
Enter and exit nested scopes.

**Example:**

```c
symtab_enter_scope(st);  // Start function
// ... parse function body ...
symtab_exit_scope(st);   // End function
```

### Symbol Definition
```c
bool symtab_define(SymbolTable* st, const char* name, 
                   SymbolKind kind, ASTNode* node, 
                   int line, int col);
```

Adds a new symbol to the current scope.

**Returns:** `true` if successful, `false` if duplicate

**Example:**
```c
if (!symtab_define(st, "x", SYM_VARIABLE, node, line, col)) {
    error("Duplicate definition");
}
```

### Symbol Lookup
```c
Symbol* symtab_lookup(SymbolTable* st, const char* name);
Symbol* symtab_lookup_current(SymbolTable* st, const char* name);
```
Search for symbols. `lookup` searches all scopes, `lookup_current` searches only current scope.

**Example:**
```c
Symbol* sym = symtab_lookup(st, "x");
if (!sym) {
    error("Undefined variable 'x'");
}
```
### Debugging
```c
void symtab_print(SymbolTable* st);
```

Prints all symbols in the table.

**Output Example:**
```
Symbol Table (scope level 0):
  println [builtin] at 0:0
  x [variable] at 1:5
  y [variable] at 2:5
  add [function] at 3:1
```

### Examples
### Example 1: Basic Variable Tracking
```
let x = 42
let y = x + 1
```
**Symbol table after analysis:**
```
Symbol Table (scope level 0):
  x [variable] at 1:5
  y [variable] at 2:5
``` 

### Example 2: Function with Parameters
```
fn add(a: Int, b: Int) -> Int {
    let result = a + b
    return result
}
```
**Symbol table after analysis:**
```
Symbol Table (scope level 0):
  add [function] at 1:1

Symbol Table (scope level 1 - inside add):
  a [parameter] at 1:8
  b [parameter] at 1:16
  result [variable] at 2:9
``` 
### Example 3: Nested Scopes
```
let x = 10

fn outer() {
    let x = 20      // Shadows outer x
    
    if true {
        let x = 30  // Shadows again
        println(x)  // Prints 30
    }
    
    println(x)      // Prints 20
}

println(x)          // Prints 10
```
**Symbol table after analysis:**
```
Global Scope (level 0):
  x [variable] at 1:5
  outer [function] at 3:1

Function outer Scope (level 1):
  x [variable] at 4:9

If block Scope (level 2):
  x [variable] at 6:13
```
### Example 4: Duplicate Detection
```
let x = 1
let x = 2
```
**Error:**
``` 
Error at 2:5: Symbol 'x' already defined
```

### Memory Management

### Arena Allocation

All symbol table memory comes from a single arena:
```c
SymbolTable* symtab_new(Arena* arena) {
    SymbolTable* st = arena_alloc(arena, sizeof(SymbolTable));
    st->entries = arena_alloc(arena, TABLE_SIZE * sizeof(Symbol*));
    // ...
}
```

### Benefits

| Benefit   | Description                     |
|-----------|---------------------------------|
| No leaks  | All memory freed at once        |
| Fast      | No individual malloc/free       |
| Simple    | No manual memory tracking       |

### Cleanup
```c
// Arena freed at end of compilation
arena_free(arena);  // Frees entire symbol table
```

### Scope Rules Summary

| Rule                       | Description                                | Example                                    |
|----------------------------|--------------------------------------------|--------------------------------------------|
| Inner shadows outer        | Inner scope variable hides outer           | `let x = 1; { let x = 2; }`                |
| No duplicate in same scope | Cannot define same name twice              | `let x = 1; let x = 2;` ❌                 |
| Parent access              | Inner scope can access outer               | `let x = 1; { println(x); }` ✅            |
| No child access            | Outer cannot access inner                  | `{ let x = 1; } println(x);` ❌            |

### Quick Reference Card

| Task              | Function                                                     |
|-------------------|--------------------------------------------------------------|
| Create table      | `symtab_new(arena)`                                            |
| Enter scope       | `symtab_enter_scope(st)`                                      |
| Exit scope        | `symtab_exit_scope(st)`                                        |
| Define variable   | `symtab_define(st, name, SYM_VARIABLE, node, line, col)`       |
| Define function   | `symtab_define(st, name, SYM_FUNCTION, node, line, col)`       |
| Define parameter  | `symtab_define(st, name, SYM_PARAMETER, node, line, col)`      |
| Lookup symbol     | `symtab_lookup(st, name)`                                      |
| Lookup current    | `symtab_lookup_current(st, name)`                              |
| Print table       | `symtab_print(st)`                                             |

The symbol table is the compiler's memory of your code's structure. 🐉
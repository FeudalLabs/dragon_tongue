# DragonTongue Semantic Analyzer API Reference

**Version:** 1.0  
**Status:** ✅ Implemented in Day 5

## Table of Contents

1. [Overview](#overview)
2. [Symbol Table API](#symbol-table-api)
3. [Type System API](#type-system-api)
4. [Semantic Analyzer API](#semantic-analyzer-api)
5. [Usage Example](#usage-example)
6. [Error Codes](#error-codes)

## Overview

The Semantic Analyzer performs three main tasks:

1. **Symbol Resolution** - Tracks variables, functions, and types in scopes
2. **Type Checking** - Validates type compatibility in operations
3. **Scope Management** - Handles nested scopes and variable shadowing

## Symbol Table API

### Header: `src/semantic/symbol.h`

### Data Structures

```c
typedef enum {
    SYM_VARIABLE,   // Variable binding
    SYM_FUNCTION,   // Function definition
    SYM_PARAMETER,  // Function parameter
    SYM_STRUCT,     // Type definition
    SYM_BUILTIN     // Built-in function
} SymbolKind;

typedef struct Symbol {
    char* name;              // Symbol name
    SymbolKind kind;         // What kind of symbol
    ASTNode* node;           // Pointer to AST node
    struct Symbol* next;     // For hash table chaining
    int scope_level;         // Which scope it belongs to
    int line;                // Declaration line
    int column;              // Declaration column
} Symbol;

typedef struct SymbolTable {
    Symbol** entries;        // Hash table (1024 slots)
    int size;
    struct SymbolTable* parent;  // Outer scope
    int scope_level;
    Arena* arena;
} SymbolTable;
```
### Functions
`SymbolTable* symtab_new(Arena* arena)`

Creates a new empty symbol table.

**Parameters:**
- `arena` - Memory arena for allocations

**Returns:** New symbol table instance

**Example:**
```c
Arena* arena = arena_new(1024 * 1024);
SymbolTable* st = symtab_new(arena);
```

`void symtab_enter_scope(SymbolTable* st)`

Enters a new nested scope. All subsequent definitions go into this scope.

**Parameters:**

Enters a new nested scope. All subsequent definitions go into this scope.

**Parameters:**
- `st` - Symbol table

**Example:**
```c
symtab_enter_scope(st);  // Start of function body
// ... definitions ...
symtab_exit_scope(st);   // End of function body
```

`void symtab_exit_scope(SymbolTable* st)`

Exits the current scope and returns to the parent scope.

**Parameters:**

- `st` - Symbol table

`bool symtab_define(SymbolTable* st, const char* name, SymbolKind kind, ASTNode* node, int line, int col)`

Defines a new symbol in the current scope.

**Parameters:**
- `st` - Symbol table
- `name` - Symbol name
- `kind` - Type of symbol (SYM_VARIABLE, SYM_FUNCTION, etc.)
- `node` - AST node associated with this symbol
- `line` - Source code line number
- `col` - Source code column number

**Returns:** `true` if defined successfully, `false` if duplicate

**Example:**
```c
symtab_define(st, "x", SYM_VARIABLE, node, 5, 10);
```

`Symbol* symtab_lookup(SymbolTable* st, const char* name)`

Searches for a symbol in the current scope and all parent scopes.

**Parameters:**
- `st` - Symbol table

- `name` - Symbol name to find

**Returns:** Symbol pointer if found, NULL otherwise

**Example:**

```c
Symbol* sym = symtab_lookup(st, "x");
if (!sym) {
    error("Undefined variable 'x'");
}
```

`Symbol* symtab_lookup_current(SymbolTable* st, const char* name)`

Searches for a symbol in the current scope only (does not check parents).

**Parameters:**
- `st` - Symbol table
- `name` - Symbol name to find

**Returns:** Symbol pointer if found, NULL otherwise

**Example:**
```c
// Check for duplicate in current scope
if (symtab_lookup_current(st, "x")) {
    error("Duplicate definition of 'x'");
}
```

`void symtab_print(SymbolTable* st)`

Prints all symbols in the table for debugging.

**Parameters:**

- `st` - Symbol table

**Example Output:**

```
Symbol Table (scope level 0):
  println [builtin] at 0:0
  x [variable] at 1:5
  y [variable] at 2:5
```

## Type System API

### Header: `src/semantic/type.h`

### Data Structures
```c
typedef enum {
    TYPE_INT,       // 64-bit integer
    TYPE_BOOL,      // Boolean (true/false)
    TYPE_STRING,    // String type
    TYPE_VOID,      // No return value
    TYPE_ERROR,     // Error type (for reporting)
    TYPE_UNKNOWN    // Not yet inferred
} TypeKind;

typedef struct TypeInfo {
    TypeKind kind;
} TypeInfo;
```

### Functions
**`TypeInfo type_int(void)`**

Creates an integer type.

**Returns:** TypeInfo for Int

---

**`TypeInfo type_bool(void)`**

Creates a boolean type.

**Returns:** TypeInfo for Bool

---

**`TypeInfo type_string(void)`**

Creates a string type.

**Returns:** TypeInfo for String

---

**`TypeInfo type_void(void)`**

Creates a void type (for functions with no return).

**Returns:** TypeInfo for Void

---

**`TypeInfo type_error(void)`**

Creates an error type (for propagation).

**Returns:** TypeInfo for Error

---

**`TypeInfo type_unknown(void)`**

Creates an unknown type (for type inference).

**Returns:** TypeInfo for Unknown

---

`bool types_equal(TypeInfo a, TypeInfo b)`

Checks if two types are equal.

**Parameters:**

- `a` - First type
- `b` - Second type

**Returns:** `true` if types are compatible

**Example:**

```c
if (types_equal(type_int(), expr_type)) {
    // Type matches
}
```

`const char* type_to_string(TypeInfo t)`

Converts a type to human-readable string.

**Parameters:**

- `t` - Type to convert

**Returns:** String representation ("Int", "Bool", "String", etc.)

**Example:**

```c
printf("Expected %s, got %s\n", 
       type_to_string(expected), 
       type_to_string(actual));
```

### Semantic Analyzer API

### Header: `src/semantic/analyzer.h`

### Data Structures

```c
typedef struct {
    SymbolTable* symbols;
    Arena* arena;
    bool had_error;
    int error_count;
} SemanticAnalyzer;
```

**Functions**

`SemanticAnalyzer* analyzer_new(Arena* arena)`

Creates a new semantic analyzer.

**Parameters:**

- `arena` - Memory arena for allocations

**Returns:** New SemanticAnalyzer instance

**Example:**

```c
SemanticAnalyzer* sa = analyzer_new(arena);
```

`void analyzer_analyze(SemanticAnalyzer* sa, ASTNode* program)`

Performs semantic analysis on the entire AST.

**Parameters:**

- `sa` - Semantic analyzer
- `program` - Root AST node (must be NODE_PROGRAM)

**Example:**
```c
analyzer_analyze(sa, ast);
```

**Performs these checks:**

- Variable definitions before use
- Duplicate definitions
- Type compatibility
- Boolean conditions in if statements
- Binary operator type validation

`bool analyzer_had_error(SemanticAnalyzer* sa)`

Checks if any errors occurred during analysis.

**Parameters:**

- `sa` - Semantic analyzer

**Returns:** `true` if errors occurred, `false` otherwise

**Example:**
```c
if (analyzer_had_error(sa)) {
    printf("Fix errors before proceeding\n");
}
```

`void analyzer_print_errors(SemanticAnalyzer* sa)`

Prints all collected errors to stderr.

**Parameters:**

- `sa` - Semantic analyzer

### Usage Example

Complete example of using the semantic analyzer:

```c
#include "semantic/analyzer.h"

int main(int argc, char** argv) {
    // Setup
    Arena* arena = arena_new(1024 * 1024);
    Source* source = source_from_file("program.dt", arena);
    Scanner* scanner = scanner_new(source, arena);
    Parser* parser = parser_new(scanner, arena);
    
    // Parse
    ASTNode* ast = parser_parse_program(parser);
    if (parser_had_error(parser)) {
        return 1;
    }
    
    // Semantic Analysis
    SemanticAnalyzer* sa = analyzer_new(arena);
    analyzer_analyze(sa, ast);
    
    if (analyzer_had_error(sa)) {
        analyzer_print_errors(sa);
        return 1;
    }
    
    printf("Semantic analysis passed!\n");
    
    // Continue to code generation...
    
    arena_free(arena);
    return 0;
}
```

### Error Codes

| Code | Error                  | Description                              |
|------|------------------------|------------------------------------------|
| E001 | Undefined variable     | Variable used before definition          |
| E002 | Duplicate definition   | Variable defined twice in same scope     |
| E003 | Type mismatch          | Expected type doesn't match actual       |
| E004 | Non-boolean condition  | If condition not boolean                 |
| E005 | Invalid binary operation| Operator not applicable to types        |

### Error Example Output
```
Semantic error at 1:7: Undefined variable 'y'
Semantic error at 2:12: Type mismatch: expected Int, got String
Semantic error at 3:3: If condition must be boolean, got Int
```


### Quick Reference Card

| Task                | Function                                  |
|---------------------|-------------------------------------------|
| Create symbol table | symtab_new(arena)                         |
| Enter new scope     | symtab_enter_scope(st)                    |
| Exit scope          | symtab_exit_scope(st)                     |
| Define variable     | symtab_define(st, name, SYM_VARIABLE, node, line, col) |
| Lookup symbol       | symtab_lookup(st, name)                  |
| Create Int type     | type_int()                                |
| Create Bool type    | type_bool()                               |
| Create String type  | type_string()                             |
| Compare types       | types_equal(a, b)                         |
| Create analyzer     | analyzer_new(arena)                       |
| Run analysis        | analyzer_analyze(sa, ast)                 |
| Check for errors    | analyzer_had_error(sa)                   |

This API is stable and ready for use in code generation. 🐉
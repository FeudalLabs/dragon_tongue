# DragonTongue AST Reference Guide

**Version:** 1.0  
**Last Updated:** Day 4 (Parser Implementation)

## Table of Contents

1. [Overview](#overview)
2. [Node Structure](#node-structure)
3. [Statement Nodes](#statement-nodes)
   - [Program Node](#program-node)
   - [Let Statement](#let-statement)
   - [If Statement](#if-statement)
   - [Block Statement](#block-statement)
   - [Return Statement](#return-statement)
4. [Function Nodes](#function-nodes)
   - [Function Definition](#function-definition)
5. [Expression Nodes](#expression-nodes)
   - [Binary Operation](#binary-operation)
   - [Variable Reference](#variable-reference)
   - [Literal Integer](#literal-integer)
   - [Literal String](#literal-string)
6. [Built-in Function Nodes](#built-in-function-nodes)
   - [Builtin Function Call](#builtin-function-call)
7. [Concurrency Nodes](#concurrency-nodes)
   - [Parallel Block](#parallel-block)
8. [AST Printing](#ast-printing)
9. [Node Creation Summary](#node-creation-summary)

## Overview

The Abstract Syntax Tree (AST) represents the structure of DragonTongue source code after parsing. Each node in the tree corresponds to a language construct: variable declaration, if statement, function call, etc.

### Why AST?

| Representation | Pros | Cons |
|----------------|------|------|
| Source Code | Human readable | Hard to analyze |
| Tokens | Simple | Loses structure |
| **AST** | Preserves structure, easy to analyze | More complex to build |

## Node Structure

Every AST node contains a common header:

```c
struct ASTNode {
    NodeType type;      // What kind of node (NODE_LET_STMT, etc.)
    int line;           // Source code line number
    int column;         // Source code column number
    struct ASTNode* next;  // For linked lists (sibling nodes)
};
```
## Node Type Enumeration
```c
typedef enum {
    // Statements
    NODE_PROGRAM,
    NODE_LET_STMT,
    NODE_IF_STMT,
    NODE_BLOCK_STMT,
    NODE_RETURN_STMT,
    
    // Functions & Structs
    NODE_FUNCTION,
    NODE_STRUCT,
    
    // Expressions
    NODE_BINARY_OP,
    NODE_VARIABLE,
    NODE_LITERAL_INT,
    NODE_LITERAL_STRING,
    
    // Concurrency
    NODE_PARALLEL,
    NODE_SPAWN,
    NODE_AWAIT,
    
    // Built-ins
    NODE_BUILTIN_FUNC,
    
    // Pattern Matching
    NODE_MATCH,
    NODE_MATCH_ARM,
    
    // Types
    NODE_TYPE_ANNOTATION
} NodeType;
```
## Statement Nodes

### Program Node

**Purpose:** Root node of every AST. Contains all top-level statements.

**Structure:**

```c
// Program is stored in the base ASTNode
// The 'next' pointer points to the first statement
```

**DragonTongue Example:**

```
let x = 42
let y = 10

fn add(a, b) {
    return a + b
}
```

**AST Output:**
```
Program
  LetStmt: x
    LiteralInt: 42
  LetStmt: y
    LiteralInt: 10
  Function: add
    Block
      Return
        BinaryOp: +
          Variable: a
          Variable: b
```

**Creation Function:**
```c
ASTNode* new_program(ASTNode* statements, int line, int col);
```
### Let Statement

**Purpose:** Variable declaration with initialization.

**Structure:**

```c
typedef struct {
    ASTNode base;
    char* name;                 // Variable name
    ASTNode* value;             // Initialization expression
    ASTNode* type_annotation;   // Optional type (NULL if not specified)
} LetStmt;
```

**DragonTongue Examples:**
```
let x = 42                    // Integer variable
let name = "DragonTongue"     // String variable
let count: Int = 0            // With type annotation
```

**AST Output:**
```
LetStmt: x
  LiteralInt: 42
LetStmt: name
  LiteralString: "DragonTongue"
LetStmt: count
  LiteralInt: 0
```
**Creation Function:**

```c
ASTNode* new_let_stmt(char* name, ASTNode* value, ASTNode* type_ann, int line, int col);
```

### If Statement

**Purpose:** Conditional execution with optional else branch.

**Structure:**
```c
typedef struct {
    ASTNode base;
    ASTNode* condition;     // Expression that evaluates to boolean
    ASTNode* then_branch;   // Statements to execute if condition is true
    ASTNode* else_branch;   // Statements to execute if condition is false (NULL if no else)
} IfStmt;
```

**DragonTongue Examples:**
```
// Simple if
if x > 10 {
    println("Big")
}

// If-else
if x > y {
    let max = x
} else {
    let max = y
}

// If-else if-else
if x > 10 {
    println("Big")
} else if x > 5 {
    println("Medium")
} else {
    println("Small")
}
```
**AST Output:**
```
IfStmt
  Condition:
    BinaryOp: >
      Variable: x
      Variable: y
  Then:
    Block
      LetStmt: max
        Variable: x
  Else:
    Block
      LetStmt: max
        Variable: y
```

**Creation Function:**
```c
ASTNode* new_if_stmt(ASTNode* condition, ASTNode* then_branch, ASTNode* else_branch, int line, int col);
```

### Block Statement
**Purpose:** Groups multiple statements into a single unit. Used in function bodies, if branches, and parallel blocks.

**Structure:**
```c
typedef struct {
    ASTNode base;
    ASTNode* statements;    // Linked list of statements
} BlockStmt;
```

**DragonTongue Example:**
```
{
    let x = 10
    let y = 20
    let z = x + y
}
```

**AST Output:**
```
Block
  LetStmt: x
    LiteralInt: 10
  LetStmt: y
    LiteralInt: 20
  LetStmt: z
    BinaryOp: +
      Variable: x
      Variable: y
```
**Creation Function:**
```c
ASTNode* new_block_stmt(ASTNode* statements, int line, int col);
```

### Return Statement

**Purpose:** Returns a value from a function.

**Structure:**
```c
typedef struct {
    ASTNode base;
    ASTNode* value;     // Expression to return (NULL for void return)
} ReturnStmt;
```

**DragonTongue Examples:**
```
return 42           // Return a value
return              // Void return
return a + b        // Return expression result
```
**AST Output:**
```
Return
  LiteralInt: 42
Return
Return
  BinaryOp: +
    Variable: a
    Variable: b
```

**Creation Function:**
```c
ASTNode* new_return_stmt(ASTNode* value, int line, int col);
```

## Function Nodes

### Function Definition

**Purpose:** Defines a reusable function with parameters and a body.

**Structure:**
```c
typedef struct {
    ASTNode base;
    char* name;             // Function name
    ASTNode* params;        // Linked list of parameter variables
    ASTNode* return_type;   // Return type annotation (NULL if inferred)
    ASTNode* body;          // Block statement containing function body
} Function;
```
**DragonTongue Example:**
```
fn add(a: Int, b: Int) -> Int {
    return a + b
}

fn greet(name: String) {
    println("Hello, " + name)
}
```
**AST Output:**
```
Function: add
  Block
    Return
      BinaryOp: +
        Variable: a
        Variable: b
Function: greet
  Block
    BuiltinFunc: println
      BinaryOp: +
        LiteralString: "Hello, "
        Variable: name
```
**Creation Function:**
```c
ASTNode* new_function(char* name, ASTNode* params, ASTNode* return_type, ASTNode* body, int line, int col);
```
## Expression Nodes

### Binary Operation

**Purpose:** Represents operations between two expressions (+, -, *, /, >, <, ==, !=, etc.).

**Structure:**
```c
typedef struct {
    ASTNode base;
    char* op;           // Operator string: "+", "-", "*", "/", ">", "<", "==", "!=", etc.
    ASTNode* left;      // Left operand
    ASTNode* right;     // Right operand
} BinaryOp;
```

### Supported Operators:

| Operator | String | Precedence | Associativity |
|----------|--------|------------|---------------|
| *        | "*"    | 2          | Left          |
| /        | "/"    | 2          | Left          |
| +        | "+"    | 3          | Left          |
| -        | "-"    | 3          | Left          |
| >        | ">"    | 4          | Left          |
| <        | "<"    | 4          | Left          |
| >=       | ">="   | 4          | Left          |
| <=       | "<="   | 4          | Left          |
| ==       | "=="   | 5          | Left          |
| !=       | "!="   | 5          | Left          |

### DragonTongue Examples:
```
a + b
x * y + z
(2 + 3) * 4
a > b && c == d
```

### AST Output:

```
BinaryOp: +
  Variable: a
  Variable: b

BinaryOp: +
  BinaryOp: *
    Variable: x
    Variable: y
  Variable: z

BinaryOp: *
  BinaryOp: +
    LiteralInt: 2
    LiteralInt: 3
  LiteralInt: 4
```  
### Creation Function:

```c
ASTNode* new_binary_op(char* op, ASTNode* left, ASTNode* right, int line, int col);
```

### Variable Reference

**Purpose:** References a variable by name.

**Structure:**
```c
typedef struct {
    ASTNode base;
    char* name;     // Variable name
} Variable;
```

### DragonTongue Examples:

```
x
counter
user_name
result
```
### AST Output:

```
Variable: x
Variable: counter
Variable: user_name
Variable: result
```
### Creation Function:

```c
ASTNode* new_variable(char* name, int line, int col);
```

### Literal Integer

**Purpose:** Represents integer literal values.

**Structure:**
```c
typedef struct {
    ASTNode base;
    long long value;    // Integer value (64-bit)
} LiteralInt;
```

### DragonTongue Examples:

```
42
0
-17
1234567890
```

### AST Output:

```
LiteralInt: 42
LiteralInt: 0
LiteralInt: -17
LiteralInt: 1234567890
```

### Creation Function:

```c
ASTNode* new_literal_int(long long value, int line, int col);
```

### Literal String

**Purpose:** Represents string literal values.

**Structure:**
```c
typedef struct {
    ASTNode base;
    char* value;    // String content (without quotes)
} LiteralString;
```

### DragonTongue Examples:

```
"Hello"
""
"DragonTongue"
```

### AST Output:

```
LiteralString: "Hello"
LiteralString: ""
LiteralString: "DragonTongue"
```

### Creation Function:

```c
ASTNode* new_literal_string(char* value, int line, int col);
```

## Built-in Function Nodes

### Builtin Function Call

**Purpose:** Represents calls to built-in functions that the runtime provides.

**Structure:**
```c
typedef struct {
    ASTNode base;
    char* name;             // Function name: "println", "print", "len", etc.
    ASTNode* arguments;     // Linked list of argument expressions
} BuiltinFunc;
```
**Supported Built-in Functions:**

| Function                 | Purpose                  | Example                          |
|--------------------------|--------------------------|----------------------------------|
| println(x)               | Print with newline       | println("Hello")                 |
| print(x)                 | Print without newline    | print("Loading...")              |
| len(s)                   | Get string length        | let n = len("hello")             |
| to_string(x)             | Convert to string        | let s = to_string(42)            |
| to_int(s)                | Convert to integer       | let n = to_int("123")            |
| read_file(path)          | Read file content        | let content = read_file("data.txt") |
| write_file(path, content)| Write file               | write_file("out.txt", data)      |

### DragonTongue Examples:

```
println("Hello, World!")
print("No newline here")
let len = length("hello")
let s = to_string(42)
let n = to_int("123")
```

### AST Output:
```
BuiltinFunc: println
  LiteralString: "Hello, World!"
BuiltinFunc: print
  LiteralString: "No newline here"
LetStmt: len
  BuiltinFunc: len
    LiteralString: "hello"
LetStmt: s
  BuiltinFunc: to_string
    LiteralInt: 42
```

### Creation Function:

```c
ASTNode* new_builtin_func(char* name, ASTNode* arguments, int line, int col);
```

## Concurrency Nodes

### Parallel Block

**Purpose:** Executes multiple tasks concurrently.

**Structure:**
```c
typedef struct {
    ASTNode base;
    ASTNode* statements;    // Statements to execute in parallel
} Parallel;
```

### DragonTongue Example:

```
parallel {
    user = fetch_user(1)
    posts = fetch_posts(1)
}
```

### AST Output:

```
Parallel
  Assign: user
    Call: fetch_user
      LiteralInt: 1
  Assign: posts
    Call: fetch_posts
      LiteralInt: 1
```

### Creation Function:

```c
ASTNode* new_parallel(ASTNode* statements, int line, int col);
```

## AST Printing
The `ast_print()` function provides a human-readable representation of the AST for debugging.

**Print Format**

- Indentation: 2 spaces per level
- Node name: Followed by relevant details
- Children: Indented one level deeper

**Example Output Format**
```
Program
  LetStmt: x
    LiteralInt: 42
  IfStmt
    Condition:
      BinaryOp: >
        Variable: x
        LiteralInt: 10
    Then:
      Block
        BuiltinFunc: println
          LiteralString: "Big"
```
### Print Function
```c
void ast_print(ASTNode* node, int indent);
```
**Usage:**

```c
ASTNode* ast = parser_parse_program(parser);
ast_print(ast, 0);  // Start with indent level 0
```

## Node Creation Summary

| Node Type           | Creation Function        | Key Parameters                          |
|---------------------|--------------------------|------------------------------------------|
| Program             | new_program()            | statements list                          |
| Let Statement       | new_let_stmt()           | name, value, type_ann                    |
| If Statement        | new_if_stmt()            | condition, then, else                    |
| Block               | new_block_stmt()         | statements                               |
| Return              | new_return_stmt()        | value                                    |
| Function            | new_function()           | name, params, return_type, body          |
| Binary Operation    | new_binary_op()          | operator, left, right                    |
| Variable            | new_variable()           | name                                     |
| Integer Literal     | new_literal_int()        | value                                    |
| String Literal      | new_literal_string()     | value                                    |
| Parallel            | new_parallel()           | statements                               |
| Builtin Function    | new_builtin_func()       | name, arguments                          |

## Quick Reference Card

### Statement Nodes
```
Program        → Root container
LetStmt        → let x = 5
IfStmt         → if condition { ... } else { ... }
BlockStmt      → { ... }
ReturnStmt     → return value
```

### Expression Nodes
```
BinaryOp       → a + b, x > y, etc.
Variable       → variable name
LiteralInt     → 42
LiteralString  → "hello"
```

### Function Nodes

```
Function       → fn name(params) { ... }
BuiltinFunc    → println(), to_string(), etc.
```

### Concurrency Nodes

```
Parallel       → parallel { ... }
```
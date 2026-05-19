# DragonTongue Development Diary - Day 4

**Focus:** Parser & Abstract Syntax Tree (AST) Implementation  
**Status:** ✅ COMPLETE

## Overview

Day 4 marked a major milestone in DragonTongue development: the implementation of the **Parser** and **Abstract Syntax Tree (AST)**. This transforms a stream of tokens into a structured tree representation that the compiler can analyze and eventually generate code from.


## Key Design Decisions

### 1. AST Node Types

We defined 30+ node types covering:
- **Statements**: `NODE_LET_STMT`, `NODE_IF_STMT`, `NODE_BLOCK_STMT`, `NODE_RETURN_STMT`, `NODE_FUNCTION`
- **Expressions**: `NODE_BINARY_OP`, `NODE_VARIABLE`, `NODE_LITERAL_INT`, `NODE_LITERAL_STRING`
- **Concurrency**: `NODE_PARALLEL`, `NODE_SPAWN`, `NODE_AWAIT`
- **Built-ins**: `NODE_BUILTIN_FUNC`

### 2. If Statement Representation

Following best practices (Rust, Go, C approach), we created a dedicated `IfStmt` node:

```c
typedef struct {
    ASTNode base;
    ASTNode* condition;    // Expression that evaluates to bool
    ASTNode* then_branch;  // Statements to execute if true
    ASTNode* else_branch;  // Statements to execute if false (optional)
} IfStmt;
```

This allows:

- Clear separation from regular blocks  
- Easy code generation (conditional jumps)  
- Semantic analysis (type checking condition)  
- Dead code elimination  

### 3 Operator Precedence

Implemented Pratt-style parser with precedence levels:

| Level           | Operators        | Function            |
|------------------|------------------|---------------------|
| 1 (highest)      | () function call | parse_call()        |
| 2                | * /              | parse_factor()      |
| 3                | + -              | parse_term()        |
| 4                | > < >= <=        | parse_comparison()  |
| 5                | == !=            | parse_equality()    |
| 6 (lowest)       | assignment       | parse_expression()  |

### 4 Built-in Function Recognition

The parser recognizes these functions as built-ins:

- println()     - print with newline  
- print()       - print without newline  
- len()         - get string length  
- to_string()   - convert to string  
- to_int()      - convert to integer  
- read_file()   - read file content  
- write_file()  - write file content  

## Code Structure

### AST Header `(src/parser/ast.h)`
```c
// Node type enumeration
typedef enum {
    NODE_PROGRAM,
    NODE_LET_STMT,
    NODE_IF_STMT,
    NODE_BLOCK_STMT,
    // ... 30+ node types
} NodeType;

// Node creation functions
ASTNode* new_program(ASTNode* statements, int line, int col);
ASTNode* new_let_stmt(char* name, ASTNode* value, ...);
ASTNode* new_if_stmt(ASTNode* condition, ASTNode* then_branch, ...);
// ... 15+ creation functions
```

### Parser (src/parser/parser.c)

Key Functions:

- parse_expression()     - Entry point for expression parsing  
- parse_equality()       - Handles == and !=  
- parse_comparison()     - Handles >, <, >=, <=  
- parse_term()           - Handles + and -  
- parse_factor()         - Handles * and /  
- parse_call()           - Handles function calls  
- parse_primary()        - Handles literals, variables, parenthesized expressions  

### Statement Parsing

- parse_statement()      - Dispatches to appropriate statement parser  
- parse_let_statement()  - Variable declarations  
- parse_if_statement()   - Conditional branches  
- parse_function()       - Function definitions  

## Testing Results

### Test File: `examples/test_if_else.dt`
```
let x = 42
let y = 10

if x > y {
    let result = 100
} else {
    let other = 200
}

println("Done")
```

### AST Output
```
Program
  LetStmt: x
    LiteralInt: 42
  LetStmt: y
    LiteralInt: 10
  IfStmt
    Condition:
      BinaryOp: >
        Variable: x
        Variable: y
    Then:
      Block
        LetStmt: result
          LiteralInt: 100
    Else:
      Block
        LetStmt: other
          LiteralInt: 200
  BuiltinFunc: println
    LiteralString: "Done"
```

## All Test Cases Passing

| Test                                  | Status |
|---------------------------------------|--------|
| Variable declaration (let x = 42)     | ✅     |
| String literals ("hello")             | ✅     |
| Number literals (42)                  | ✅     |
| Binary operations (a + b, x > y)      | ✅     |
| If statement with else                | ✅     |
| Function definition                   | ✅     |
| Return statement                      | ✅     |
| Built-in function calls               | ✅     |
| Nested blocks                         | ✅     |

## Challenges Faced & Solutions

### Challenge 1: Operator Precedence
Problem: Expressions like `1 + 2 * 3` were parsing as `(1 + 2) * 3` instead of `1 + (2 * 3)`
Solution: Implemented Pratt parser with precedence levels

### Challenge 2: If Statement Without Condition in AST
Problem: The parser was throwing away the condition and only returning the then block
Solution: Created dedicated `IfStmt` node that stores condition, then, and else branches

### Challenge 3: Duplicate AST Output
Problem: The AST was printing twice due to recursive `node->next` traversal
Solution: Changed Program and Block nodes to use while loops instead of recursion, removed the final if `(node->next)` call

### Challenge 4: strdup Portability
Problem: `strdup()` not available on all systems
Solution: Implemented custom `my_strdup()` and `my_strndup()` functions

## Files Created/Modified Today

| File                       | Lines | Purpose                          |
|----------------------------|-------|----------------------------------|
| `src/parser/ast.h`         | 150   | AST node type definitions        |
| `src/parser/ast.c`         | 380   | AST node creation and printing   |
| `src/parser/parser.h`      | 40    | Parser interface                 |
| `src/parser/parser.c`      | 510   | Parser implementation            |
| `Makefile`                 | Updated | Added parser compilation       |
| `src/main.c`               | Updated | Integrated parser              |

## What's Working Now

✅ Lexer (Days 1-3)

- Tokenizes source code
- Recognizes keywords, operators, literals
- Handles comments and whitespace
- Reports errors with line/column

✅ Parser (Day 4)

- Builds AST from tokens
- Handles operator precedence
- Parses statements (let, if, return)
- Parses expressions
- Recognizes built-in functions
- Supports nested blocks

## Commands Used Today
```
# Build with parser
make clean && make

# Test if statement
./dragontongue examples/test_if_else.dt

# Test simple program
./dragontongue examples/simple.dt

# Run all tests
make test
```
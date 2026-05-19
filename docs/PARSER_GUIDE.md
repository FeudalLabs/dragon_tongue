# DragonTongue Parser Implementation Guide

**Version:** 1.0  
**Last Updated:** Day 4 (Parser Implementation)

## Table of Contents

1. [Overview](#overview)
2. [Parser Architecture](#parser-architecture)
3. [Pratt Parser (Operator Precedence)](#pratt-parser-operator-precedence)
4. [Parser Functions](#parser-functions)
   - [Core Helper Functions](#core-helper-functions)
   - [Expression Parsers](#expression-parsers)
   - [Statement Parsers](#statement-parsers)
5. [Operator Precedence Table](#operator-precedence-table)
6. [Parsing Walkthrough](#parsing-walkthrough)
7. [Error Handling](#error-handling)
8. [Complete API Reference](#complete-api-reference)

## Overview

The DragonTongue parser converts a stream of tokens from the lexer into an Abstract Syntax Tree (AST). It uses a **Pratt parser** (also called "top-down operator precedence parsing") to handle expressions with proper precedence and associativity.

### Parser Pipeline

Tokens → Pratt Parser → AST

### Why Pratt Parser?

| Approach | Complexity | Flexibility | Error Recovery |
|----------|------------|-------------|----------------|
| Recursive Descent | Low | Low | Poor |
| Shunting Yard | Medium | Medium | Medium |
| **Pratt Parser** | Medium | High | Excellent |

**Advantages of Pratt Parser:**
- Easy to add new operators
- Clear precedence handling
- Good error recovery
- Used in many production compilers (Rust, Swift, Go, Kotlin)

## Parser Architecture

### Core Data Structures

```c
typedef struct {
    Scanner* scanner;   // Source of tokens
    Token current;      // Current token being examined
    Token previous;     // Previously consumed token
    Arena* arena;       // Memory arena for allocations
    bool had_error;     // Error flag
} Parser;
```

### Parser Functions Hierarchy

```
parser_parse_program()
    └── parse_statement() (called repeatedly)
            ├── parse_let_statement()
            ├── parse_function()
            ├── parse_if_statement()
            ├── parse_return_statement()
            ├── parse_block()
            └── parse_expression() (default)
                    └── parse_equality()
                            └── parse_comparison()
                                    └── parse_term()
                                            └── parse_factor()
                                                    └── parse_call()
                                                            └── parse_primary()
```                                                            

## Pratt Parser (Operator Precedence)

### How Pratt Parsing Works

Pratt parsing uses two key concepts:

1. Null Denotation (nud) - Parses a token when it's at the start of an expression (like literals or variables)

2. Left Denotation (led) - Parses a token when it's in the middle of an expression (like binary operators)

### Implementation Pattern

```c
static ASTNode* parse_expression(Parser* p, int min_precedence) {
    // Parse the leftmost token (nud)
    ASTNode* left = parse_primary(p);
    
    // While the next token is an operator with precedence >= min_precedence
    while (p->current.type is operator && get_precedence(p->current) >= min_precedence) {
        Token op = p->current;
        advance(p);
        
        // Parse the right side with higher precedence (led)
        ASTNode* right = parse_expression(p, get_precedence(op) + 1);
        
        // Combine into a binary operation node
        left = new_binary_op(op_string, left, right);
    }
    
    return left;
}
```

### DragonTongue Implementation

We use a simplified approach with separate functions for each precedence level:

```c
// Lowest precedence first, calling higher precedence functions
static ASTNode* parse_equality(Parser* p) { ... }    // == !=
static ASTNode* parse_comparison(Parser* p) { ... }  // > < >= <=
static ASTNode* parse_term(Parser* p) { ... }        // + -
static ASTNode* parse_factor(Parser* p) { ... }      // * /
static ASTNode* parse_primary(Parser* p) { ... }     // literals, vars, ()
```

## Parser Functions

### Core Helper Functions

`advance(Parser* p)`

Moves to the next token and updates `current` and `previous`.

```c
static void advance(Parser* p) {
    p->previous = p->current;
    p->current = scanner_next_token(p->scanner);
}
```
`check(Parser* p, TokenType type)`

Returns true if the current token matches the given type.

```c
static bool check(Parser* p, TokenType type) {
    return p->current.type == type;
}
```

`consume(Parser* p, TokenType type, const char* message)`

Checks that the current token matches, advances, or reports an error.

```c
static bool consume(Parser* p, TokenType type, const char* message) {
    if (check(p, type)) {
        advance(p);
        return true;
    }
    fprintf(stderr, "Error at %d:%d: %s\n", 
            p->current.line, p->current.column, message);
    p->had_error = true;
    return false;
}
```

`match(Parser* p, TokenType type)`

Returns true and advances if current token matches, otherwise returns false.

```c
static bool match(Parser* p, TokenType type) {
    if (check(p, type)) {
        advance(p);
        return true;
    }
    return false;
}
```

### Expression Parsers

`parse_expression(Parser* p)`

Entry point for parsing any expression. Delegates to `parse_equality`.

```c
static ASTNode* parse_expression(Parser* p) {
    return parse_equality(p);
}
```

`parse_equality(Parser* p)`

Parses equality operators (`==`, `!=`). Lowest precedence.

```c
static ASTNode* parse_equality(Parser* p) {
    ASTNode* node = parse_comparison(p);
    
    while (match(p, TOKEN_EQ) || match(p, TOKEN_NE)) {
        TokenType op = p->previous.type;
        ASTNode* right = parse_comparison(p);
        const char* op_str = (op == TOKEN_EQ) ? "==" : "!=";
        node = new_binary_op((char*)op_str, node, right, 
                             p->previous.line, p->previous.column);
    }
    return node;
}
```

`parse_comparison(Parser* p)`

Parses comparison operators (`>`, `<`, `>=`, `<=`).

```c
static ASTNode* parse_comparison(Parser* p) {
    ASTNode* node = parse_term(p);
    
    while (match(p, TOKEN_GT) || match(p, TOKEN_LT) || 
           match(p, TOKEN_GE) || match(p, TOKEN_LE)) {
        TokenType op = p->previous.type;
        ASTNode* right = parse_term(p);
        const char* op_str = "";
        if (op == TOKEN_GT) op_str = ">";
        else if (op == TOKEN_LT) op_str = "<";
        else if (op == TOKEN_GE) op_str = ">=";
        else if (op == TOKEN_LE) op_str = "<=";
        node = new_binary_op((char*)op_str, node, right,
                             p->previous.line, p->previous.column);
    }
    return node;
}
```

`parse_term(Parser* p)`

Parses additive operators (`+`, `-`).

```c
static ASTNode* parse_term(Parser* p) {
    ASTNode* node = parse_factor(p);
    
    while (match(p, TOKEN_PLUS) || match(p, TOKEN_MINUS)) {
        TokenType op = p->previous.type;
        ASTNode* right = parse_factor(p);
        const char* op_str = (op == TOKEN_PLUS) ? "+" : "-";
        node = new_binary_op((char*)op_str, node, right,
                             p->previous.line, p->previous.column);
    }
    return node;
}
```

`parse_factor(Parser* p)`

Parses multiplicative operators (`*`, `/`).

```c
static ASTNode* parse_factor(Parser* p) {
    ASTNode* node = parse_call(p);
    
    while (match(p, TOKEN_STAR) || match(p, TOKEN_SLASH)) {
        TokenType op = p->previous.type;
        ASTNode* right = parse_call(p);
        const char* op_str = (op == TOKEN_STAR) ? "*" : "/";
        node = new_binary_op((char*)op_str, node, right,
                             p->previous.line, p->previous.column);
    }
    return node;
}
```

`parse_call(Parser* p)`

Parses function calls (after a primary expression).

```c
static ASTNode* parse_call(Parser* p) {
    ASTNode* node = parse_primary(p);
    
    if (match(p, TOKEN_LPAREN)) {
        // Parse arguments
        while (!check(p, TOKEN_RPAREN) && !check(p, TOKEN_EOF)) {
            parse_expression(p);
            if (!match(p, TOKEN_COMMA)) break;
        }
        consume(p, TOKEN_RPAREN, "Expected ')' after arguments");
    }
    return node;
}
```

`parse_primary(Parser* p)`

Parses primary expressions: literals, variables, and parenthesized expressions.

```c
static ASTNode* parse_primary(Parser* p) {
    ASTNode* node = NULL;
    
    switch (p->current.type) {
        case TOKEN_NUMBER:
            node = new_literal_int(p->current.int_value, 
                                   p->current.line, p->current.column);
            advance(p);
            break;
            
        case TOKEN_STRING_LITERAL:
            node = new_literal_string(p->current.string_value,
                                      p->current.line, p->current.column);
            advance(p);
            break;
            
        case TOKEN_IDENTIFIER:
            // Handle built-in functions and variables
            // ...
            break;
            
        case TOKEN_LPAREN:
            advance(p);
            node = parse_expression(p);
            consume(p, TOKEN_RPAREN, "Expected ')' after expression");
            break;
            
        default:
            // Error handling
            break;
    }
    
    return node;
}
```

### Statement Parsers

`parse_statement(Parser* p)`

Top-level statement dispatcher.

```c
static ASTNode* parse_statement(Parser* p) {
    switch (p->current.type) {
        case TOKEN_LET:    return parse_let_statement(p);
        case TOKEN_FN:     return parse_function(p);
        case TOKEN_IF:     return parse_if_statement(p);
        case TOKEN_RETURN: return parse_return_statement(p);
        case TOKEN_LBRACE: return parse_block(p);
        default:           return parse_expression(p);
    }
}
```

`parse_let_statement(Parser* p)`

Parses variable declarations.

```c
static ASTNode* parse_let_statement(Parser* p) {
    int line = p->current.line;
    int col = p->current.column;
    advance(p);  // consume 'let'
    
    if (!consume(p, TOKEN_IDENTIFIER, "Expected variable name")) {
        return NULL;
    }
    
    char* var_name = my_strndup(p->previous.start, p->previous.length);
    
    // Optional type annotation
    ASTNode* type_ann = NULL;
    if (match(p, TOKEN_COLON)) {
        if (check(p, TOKEN_IDENTIFIER)) {
            char* type_name = my_strndup(p->current.start, p->current.length);
            type_ann = new_variable(type_name, p->current.line, p->current.column);
            free(type_name);
            advance(p);
        }
    }
    
    consume(p, TOKEN_ASSIGN, "Expected '=' after variable name");
    
    ASTNode* value = parse_expression(p);
    
    return new_let_stmt(var_name, value, type_ann, line, col);
}
```

`parse_if_statement(Parser* p)`

Parses conditional statements.

```c
static ASTNode* parse_if_statement(Parser* p) {
    int line = p->current.line;
    int col = p->current.column;
    advance(p);  // consume 'if'
    
    ASTNode* condition = parse_expression(p);
    if (!condition) return NULL;
    
    // Then branch must be a block
    ASTNode* then_branch = parse_block(p);
    if (!then_branch) return NULL;
    
    // Optional else branch
    ASTNode* else_branch = NULL;
    if (match(p, TOKEN_ELSE)) {
        if (check(p, TOKEN_IF)) {
            // else if
            else_branch = parse_if_statement(p);
        } else {
            // else block
            else_branch = parse_block(p);
        }
    }
    
    return new_if_stmt(condition, then_branch, else_branch, line, col);
}
```

`parse_block(Parser* p)`

Parses a block of statements enclosed in braces.

```c
static ASTNode* parse_block(Parser* p) {
    int line = p->current.line;
    int col = p->current.column;
    
    if (!consume(p, TOKEN_LBRACE, "Expected '{'")) {
        return NULL;
    }
    
    ASTNode* statements = NULL;
    ASTNode* last = NULL;
    
    while (!check(p, TOKEN_RBRACE) && !check(p, TOKEN_EOF)) {
        ASTNode* stmt = parse_statement(p);
        if (stmt) {
            if (!statements) {
                statements = stmt;
                last = stmt;
            } else {
                last->next = stmt;
                last = stmt;
            }
        }
    }
    
    consume(p, TOKEN_RBRACE, "Expected '}'");
    return new_block_stmt(statements, line, col);
}
```

`parse_function(Parser* p)`

Parses function definitions.

```c
static ASTNode* parse_function(Parser* p) {
    int line = p->current.line;
    int col = p->current.column;
    advance(p);  // consume 'fn'
    
    if (!consume(p, TOKEN_IDENTIFIER, "Expected function name")) {
        return NULL;
    }
    
    char* func_name = my_strndup(p->previous.start, p->previous.length);
    
    consume(p, TOKEN_LPAREN, "Expected '(' after function name");
    
    // Parse parameters
    ASTNode* params = NULL;
    // ... parameter parsing logic ...
    
    consume(p, TOKEN_RPAREN, "Expected ')' after parameters");
    
    // Optional return type
    ASTNode* return_type = NULL;
    if (match(p, TOKEN_ARROW)) {
        // ... return type parsing ...
    }
    
    ASTNode* body = parse_block(p);
    
    return new_function(func_name, params, return_type, body, line, col);
}
```

## Operator Precedence Table

| Level        | Operators                | Function             | Associativity  | Example   |
|--------------|--------------------------|----------------------|----------------|-----------|
| 1 (Highest)  | `()` call                | `parse_call()`       | Left           | `foo()`   |
| 2            | `*` `/`                  | `parse_factor()`     | Left           | `a * b`   |
| 3            | `+` `-`                  | `parse_term()`       | Left           | `a + b`   |
| 4            | `>` `<` `>=` `<=`        | `parse_comparison()` | Left           | `a > b`   |
| 5            | `==` `!=`                | `parse_equality()`   | Left           | `a == b`  |
| 6 (Lowest)   | Assignment (future)      | `parse_expression()` | Right          | `a = b`   |


## Precedence Example

For expression: `1 + 2 * 3`

1. `parse_equality()` calls parse_comparison()
2. `parse_comparison()` calls parse_term()
3. `parse_term()` sees `1`, then `+`, but must parse `2 * 3` first
4. `parse_factor()` parses `2 * 3` as `BinaryOp(*, 2, 3)`
5. `parse_term()` combines: `BinaryOp(+, 1, BinaryOp(*, 2, 3))`

Result: `1 + (2 * 3)` ✅


## Parsing Walkthrough

Example: `let x = 10 + 20 * 3`

Step 1: `parse_statement()` sees `TOKEN_LET` → `parse_let_statement()`

Step 2: Advance past `let`, consume identifier `x`

Step 3: Consume `=` token

Step 4: Call `parse_expression()` to parse the value

Step 5: `parse_expression()` → `parse_equality()` → `parse_comparison()` → `parse_term()`

Step 6: `parse_term()` sees `10`, then `+`, calls `parse_factor()` for right side

Step 7: `parse_factor()` sees `20`, then `*`, calls `parse_call()` for right side

Step 8: `parse_call()` → `parse_primary()` → `3`

Step 9: Build nodes bottom-up:

  - `BinaryOp(*, 20, 3)`

  - `BinaryOp(+, 10, BinaryOp(*, 20, 3))`

Step 10: `Create LetStmt(x, BinaryOp(+, 10, *))`


## AST Result

```
LetStmt: x
  BinaryOp: +
    LiteralInt: 10
    BinaryOp: *
      LiteralInt: 20
      LiteralInt: 3
```


**Example:** `if x > 10 { println("Big") }`

Step 1: `parse_statement()` sees `TOKEN_IF` → `parse_if_statement()`

Step 2: Advance past `if`

Step 3: Call `parse_expression()` for condition

  - `parse_equality()` → `parse_comparison()`

  - `parse_comparison()` sees `x`, then `>`, parses `10`

Returns `BinaryOp(>, x, 10)`

Step 4: Call `parse_block()` for then branch

- Consumes `{`
- Parses statements inside
- Consumes `}`

Step 5: Create `IfStmt` with condition and then branch

### AST Result

```
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
## Error Handling

**Error Recovery Strategy**

When a parsing error occurs:

1. Report the error with line and column information
2. Set had_error flag to true
3. Synchronize to the next statement boundary
4. Continue parsing to find more errors

**Synchronization Points**
```c
while (!check(p, TOKEN_EOF) && 
       !check(p, TOKEN_LET) && 
       !check(p, TOKEN_FN) && 
       !check(p, TOKEN_IF) && 
       !check(p, TOKEN_RETURN) && 
       !check(p, TOKEN_LBRACE)) {
    advance(p);
}
```

**Error Example**
```
Error Example
```
**Parser output:**
```
Error at 1:9: Expected expression after '='
```

The parser recovers and continues parsing `let y = 42`.

## Complete API Reference

### Parser Lifecycle

```c
// Create parser
Parser* parser_new(Scanner* scanner, Arena* arena);

// Parse program
ASTNode* parser_parse_program(Parser* p);

// Check for errors
bool parser_had_error(Parser* p);

// Print errors (if any)
void parser_print_errors(Parser* p);
```

### Usage Example

```c
Arena* arena = arena_new(1024 * 1024);
Source* source = source_from_file("program.dt", arena);
Scanner* scanner = scanner_new(source, arena);
Parser* parser = parser_new(scanner, arena);

ASTNode* ast = parser_parse_program(parser);

if (!parser_had_error(parser)) {
    ast_print(ast, 0);
}

arena_free(arena);
```

### Quick Reference Card

### Expression Precedence (Highest to Lowest)

| Precedence | Operators             | Function             |
|------------|-----------------------|----------------------|
| 1          | `()`                  | `parse_call()`       |
| 2          | `*` `/`               | `parse_factor()`     |
| 3          | `+` `-`               | `parse_term()`       |
| 4          | `>` `<` `>=` `<=`     | `parse_comparison()` |
| 5          | `==` `!=`             | `parse_equality()`   |

### Statement Types

| Statement  | Handler                      | Example             |
|------------|------------------------------|---------------------|
| Variable   | `parse_let_statement()`      | `let x = 5`         |
| Function   | `parse_function()`           | `fn foo() {...}`    |
| If         | `parse_if_statement()`       | `if cond {...}`     |
| Return     | `parse_return_statement()`   | `return value`      |
| Block      | `parse_block()`              | `{...}`             |

### Key Tokens

| Token              | Purpose                    |
|--------------------|----------------------------|
| `TOKEN_LET`        | Variable declaration       |
| `TOKEN_FN`         | Function definition        |
| `TOKEN_IF`         | Conditional                |
| `TOKEN_RETURN`     | Return statement           |
| `TOKEN_LBRACE`     | Block start                |
| `TOKEN_RBRACE`     | Block end                  |
| `TOKEN_LPAREN`     | Grouping / function call   |
| `TOKEN_RPAREN`     | End grouping               |

# DragonTongue Language Reference

**Version:** 0.0.1 (Draft)

This is the complete specification of the DragonTongue programming language.

---

## Table of Contents

1. [Lexical Structure](#1-lexical-structure)
2. [Types](#2-types)
3. [Variables](#3-variables)
4. [Expressions](#4-expressions)
5. [Statements](#5-statements)
6. [Functions](#6-functions)
7. [Structs](#7-structs)
8. [Concurrency](#8-concurrency)
9. [Error Handling](#9-error-handling)
10. [Modules](#10-modules)

---

## 1. Lexical Structure

### 1.1 Character Set

DragonTongue source code is UTF-8 encoded. v1 supports ASCII identifiers.

### 1.2 Comments

```dragontongue
// Line comment until end of line

/* Block comment
   Can span multiple lines */
```   

## 1.3 Keywords

| Keyword | Purpose |
|--------|--------|
| `let` | Immutable variable |
| `var` | Mutable variable |
| `fn` | Function definition |
| `struct` | Type definition |
| `if`, `else` | Conditional |
| `for`, `while` | Loops |
| `spawn` | Create task |
| `await` | Wait for task |
| `return` | Return from function |
| `match` | Pattern matching |
| `catch`, `else` | Error handling |
| `parallel` | Concurrent block |

---

## 2. Types

### 2.1 Primitive Types

| Type | Literal examples | Size |
|------|----------------|------|
| `Int` | 42, -17, 0 | 64-bit |
| `Float` | 3.14, -0.5, 2.0 | 64-bit |
| `Bool` | true, false | 1 byte |
| `String` | "hello", "" | Variable |

### 2.2 Composite Types

```
// Array (fixed size, stack-allocated)
let numbers: [Int; 3] = [1, 2, 3]

// Slice (dynamic size)
let names: []String = ["Alice", "Bob"]

// Map
let ages: {String: Int} = {"Alice": 30, "Bob": 25}

// Option
let maybe: Option<Int> = Some(42)
```

### 2.3 Type Aliases
```
type UserId = Int
let id: UserId = 12345
```

## 3. Variables

### 3.1 Declaration
```
// Immutable (default)
let x = 5

// Mutable
var y = 10
y = y + 1

// With explicit type
let z: Int = 20
```

### 3.2 Scope
Variables are block-scoped:

```
{
    let inner = "only here"
}
// inner not accessible here
```

## 4. Expressions

## 4.1 Literals

| Type | Example |
|------|--------|
| Integer | 42 |
| Float | 3.14 |
| String | "hello" |
| Boolean | true |
| Array | [1, 2, 3] |

---

## 4.2 Operators

| Category | Operators | Precedence |
|----------|----------|------------|
| Unary | `-`, `!` | Highest |
| Multiplicative | `*`, `/` | High |
| Additive | `+`, `-` | Medium |
| Comparison | `<`, `>`, `<=`, `>=` | Medium |
| Equality | `==`, `!=` | Low |
| Logical AND | `and` | Lower |
| Logical OR | `or` | Lowest |
| Assignment | `=`, `+=`, `-=` | Lowest |

---

## 4.3 Function Calls

```
let result = add(5, 3)
let (q, r) = divide(10, 3)
```

## 5. Statements

### 5.1 If Statement
```
if condition {
    // then
} else if other {
    // else if
} else {
    // else
}

// As expression
let value = if x > 0 { x } else { -x }
```
### 5.2 For Loop
```
// Range
for i in 0..10 {
    print(i)
}

// Collection
for item in items {
    print(item)
}

// With index
for i, item in items {
    print(i, item)
}
```

### 5.3 While Loop
```
while condition {
    // ...
}
```
## 6. Functions

### 6.1 Definition
```
fn name(param1: Type1, param2: Type2) -> ReturnType {
    return value
}
```

### 6.2 Multiple Returns
```
fn swap(a: Int, b: Int) -> (Int, Int) {
    return (b, a)
}
```
### 6.3 Default Parameters
```
fn greet(name: String, prefix: String = "Hello") {
    print(prefix, name)
}
```

## 7. Structs

### 7.1 Definition

```
struct Point {
    x: Int
    y: Int
}
```

### 7.2 Instantiation
```
let p = Point{x: 10, y: 20}
let p2 = Point{10, 20}  // Positional if order matches
```
### 7.3 Methods

```
fn Point.distance(self, other: Point) -> Int {
    let dx = self.x - other.x
    let dy = self.y - other.y
    return (dx*dx + dy*dy).sqrt()
}
```

## 8. Concurrency

### 8.1 Spawning Tasks

```
let handle = spawn my_function(args)
```

### 8.2 Awaiting Tasks

```
let result = await handle
```

### 8.3 Parallel Blocks
```
parallel {
    a = fetch_a()
    b = fetch_b()
}
// Both complete here
```

### 8.4 Channels
```
let ch = channel(Int, 10)  // Buffered
let unbuffered = channel(Int)

send(ch, 42)
let value = recv(ch)
```

## 9. Error Handling

### 9.1 Errors as Return Values

```
fn may_fail() -> Int error MyError {
    if bad_condition {
        return MyError{message: "failed"}
    }
    return 42
}
```

### 9.2 Catching Errors

```
catch may_fail() as err {
    // Handle error
    return
} else value {
    // Use value
}
```

### 9.3 Bubbling Errors

```
fn caller() -> Int error MyError {
    let value = may_fail()?  // ? bubbles error
    return value
}
```

## 10. Modules

### 10.1 Importing
```
import http
import json as j
import "local/file.dt" as mylib
```
### 10.2 Exporting
```
pub fn public_function() { }
fn private_function() { }
```

## Appendix A: Operator Precedence

| Precedence | Operators | Associativity |
|------------|----------|----------------|
| 10 | `()`, `[]`, `.` | Left |
| 9 | `-`, `!` | Right |
| 8 | `*`, `/` | Left |
| 7 | `+`, `-` | Left |
| 6 | `<`, `>`, `<=`, `>=` | Left |
| 5 | `==`, `!=` | Left |
| 4 | `and` | Left |
| 3 | `or` | Left |
| 2 | `=`, `+=`, `-=` | Right |

## Appendix B: Grammar (EBNF)

```
program        = { statement } .
statement      = let_stmt | var_stmt | function | struct | if_stmt | for_stmt | while_stmt | return_stmt | expression_stmt | parallel_stmt | block .
let_stmt       = "let" identifier "=" expression .
var_stmt       = "var" identifier "=" expression .
function       = "fn" identifier "(" [ parameters ] ")" [ "->" type ] block .
struct         = "struct" identifier "{" { field } "}" .
if_stmt        = "if" expression block [ "else" ( if_stmt | block ) ] .
for_stmt       = "for" identifier "in" range block .
parallel_stmt  = "parallel" block .
expression     = assignment .
assignment     = logical_or [ "=" expression ] .
logical_or     = logical_and { "or" logical_and } .
logical_and    = equality { "and" equality } .
equality       = comparison { ("==" | "!=") comparison } .
comparison     = term { ("<" | ">" | "<=" | ">=") term } .
term           = factor { ("+" | "-") factor } .
factor         = unary { ("*" | "/") unary } .
unary          = ("-" | "!") unary | call .
call           = primary { "(" [ arguments ] ")" } .
primary        = number | string | "true" | "false" | identifier | "(" expression ")" .
```
> This reference is a work in progress. See [ROADMAP.md](ROADMAP.md) for upcoming additions.

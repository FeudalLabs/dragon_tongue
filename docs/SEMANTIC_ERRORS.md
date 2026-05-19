# DragonTongue Semantic Errors Reference

**Version:** 1.0  
**Status:** ✅ Implemented in Day 5

## Table of Contents

1. [Overview](#overview)
2. [Error Categories](#error-categories)
3. [Error Codes](#error-codes)
4. [Common Errors and Fixes](#common-errors-and-fixes)
5. [Troubleshooting Guide](#troubleshooting-guide)

## Overview

Semantic errors occur when code is syntactically correct but logically invalid. These errors are caught during the semantic analysis phase, before code generation.

### Error Format

Semantic error at <line>:<column>: <message>

```

### Example

```dragontongue
let x = y
```
**Output:**
```
Semantic error at 5:9: Undefined variable 'y'
```

### Visual Guide
```
let x = y
        ↑
    column 7
    line 1
```    

### Error Categories

| Category            | Error Codes           | Description                         |
|---------------------|-----------------------|-------------------------------------|
| Symbol Resolution   | E001, E002            | Undefined or duplicate symbols      |
| Type Mismatch       | E003, E004, E005      | Incompatible types                  |
| Control Flow        | E006, E007            | Invalid if/loop conditions          |
| Function            | E008, E009, E010      | Function declaration issues         |
| Scope               | E011, E012            | Scope access violations             |

### Error Codes

### E001: Undefined Variable

**Message:** `Undefined variable '<name>'`

**Cause:** Using a variable that hasn't been declared.

**Example:**
```
let x = y  // y is not defined
```
**Fix:**
```
let y = 10
let x = y  // ✅ y is defined
```

### E002: Duplicate Definition

**Message:** `Symbol '<name>' already defined`

**Cause:** Defining the same symbol twice in the same scope.

**Example:**
```
let x = 1
let x = 2  // Error: x already defined
```
**Fix:**
```
let x = 1
let y = 2  // ✅ different name
```

### E003: Type Mismatch - Assignment
**Message:** `Type mismatch: expected <expected>, got <actual>`

**Cause:** Assigning a value of the wrong type.

**Example:**
```
let x: Int = "hello"  // Error: expected Int, got String
```
**Fix:**
```
let x: Int = 42       // ✅ correct type
let x = "hello"       // ✅ let type be inferred
```

### E004: Type Mismatch - Binary Operation
**Message:** `Cannot apply <operator> to <type> and <type>`

**Cause:** Using an operator on incompatible types.

**Example:**
```
let x = true - false  // Error: Cannot apply - to Bool and Bool
```

**Fix:**
```
let x = true && false  // ✅ use logical operator
let x = 5 - 3          // ✅ use numeric types
```

### E005: Type Mismatch - Comparison
**Message:** `Cannot compare <type> and <type>`

**Cause:** Comparing incompatible types.

**Example:**
```
let result = 42 > "hello"  // Error: Cannot compare Int and String
```

**Fix:**
```
let result = 42 > 10        // ✅ compare Int with Int
let result = "hi" == "hi"   // ✅ compare String with String
```

### E006: Non-Boolean Condition
**Message:** `If condition must be boolean, got <type>`

**Cause:** Using a non-boolean value as an if condition.

**Example:**
```
if 42 {
    println("true")
}
```
**Fix:**
```
if 42 > 0 {           // ✅ comparison returns Bool
    println("true")
}

let flag = true
if flag {             // ✅ Bool variable
    println("true")
}
```

### E007: Non-Boolean Loop Condition
**Message:** `While condition must be boolean, got <type>`

**Cause:** Using a non-boolean value as a while condition.

**Example:**
```
while 42 {
    println("loop")
}
```
**Fix:**
```
let x = 0
while x < 10 {        // ✅ comparison returns Bool
    x = x + 1
}
```

### E008: Function Return Type Mismatch
**Message:** `Return type mismatch: expected <expected>, got <actual>`

**Cause:** Function returns wrong type.

**Example:**
```
fn add(a: Int, b: Int) -> Int {
    return "hello"    // Error: expected Int, got String
}
```
**Fix:**
```
fn add(a: Int, b: Int) -> Int {
    return a + b      // ✅ returns Int
}
```

### E009: Missing Return
**Message:** `Function '<name>' is missing a return value`

**Cause:** Function with non-void return type doesn't return.

**Example:**
```
fn add(a: Int, b: Int) -> Int {
    let x = a + b
    // No return statement
}
```
**Fix:**
```
fn add(a: Int, b: Int) -> Int {
    let x = a + b
    return x          // ✅ return the value
}
```

### E010: Void Return with Value
**Message:** `Void function cannot return a value`

**Cause:** Void function tries to return a value.

**Example:**
```
fn greet(name: String) -> Void {
    return "Hello"    // Error: Void function cannot return value
}
```
**Fix:**
```
fn greet(name: String) -> Void {
    println("Hello")  // ✅ just print, no return
    return            // ✅ or empty return
}
```
### E011: Variable Out of Scope
**Message:** `Variable '<name>' is not accessible here`

**Cause:** Accessing variable outside its scope.

**Example:**

```
{
    let x = 42
}
println(x)  // Error: x not accessible
```
**Fix:**
```
let x = 42          // ✅ define in outer scope
{
    // use x here
}
println(x)          // ✅ accessible
```
### E012: Parameter Name Conflict
**Message:** `Parameter '<name>' shadows outer variable`

**Cause:** Function parameter has same name as outer variable (warning level in future).

**Example:**
```
let x = 10

fn add(x: Int, y: Int) {
    // x parameter shadows outer x
}
```
**Fix:**
```
let outer_x = 10

fn add(x: Int, y: Int) {
    // use x as parameter
}
```

### Troubleshooting Guide

### Problem: "Undefined variable 'x'"

**Checklist:**

- Is `x` defined before use?
- Is `x` in the same scope?
- Is there a typo? (`x` vs `X`)

### Problem: "Type mismatch"

**Checklist:**

- Are both sides of the operator the same type?
- Did you use the correct operator? (`+` vs `&&`)
- Did you forget a conversion? (`to_string(42)`)

### Problem: "If condition must be boolean"

**Checklist:**

- Did you use a comparison operator? (`>`, `<`, `==`)
- Is the variable a Bool type?
- Did you mean to write a loop?

### Common Mistakes

### Mistake 1: Comparing Different Types
```
// ❌ Wrong
if "42" > 10 { ... }

// ✅ Correct
let num = to_int("42")
if num > 10 { ... }
```
### Mistake 2: Using = Instead of ==
```
// ❌ Wrong (assignment)
if x = 10 { ... }

// ✅ Correct (comparison)
if x == 10 { ... }
```
### Mistake 3: Forgetting to Convert
```
// ❌ Wrong
println("Value: " + 42)

// ✅ Correct
println("Value: " + to_string(42))
```
### Mistake 4: Shadowing Accidentally
```
// ❌ Confusing
let count = 10
if true {
    let count = 20  // Shadows outer
    println(count)  // Prints 20
}
println(count)      // Prints 10 (still confusing)

// ✅ Better
let total = 10
if true {
    let inner_count = 20
    println(inner_count)
}
println(total)
```
### Mistake 5: Missing Return in Non-Void Function
```
// ❌ Wrong
fn get_value() -> Int {
    let x = 42
    // No return
}

// ✅ Correct
fn get_value() -> Int {
    let x = 42
    return x
}
```
### Error Recovery
The semantic analyzer continues after errors to find more issues:

```
let x = y             // Error 1: undefined y
let a: Int = "hello"  // Error 2: type mismatch
if 42 { ... }         // Error 3: non-boolean condition
```
**Output:**
```
Semantic error at 1:7: Undefined variable 'y'
Semantic error at 2:12: Type mismatch: expected Int, got String
Semantic error at 3:3: If condition must be boolean, got Int
```

### Quick Reference Card

| Error                         | Code  | Common Fix                              |
|------------------------------|-------|------------------------------------------|
| Undefined variable           | E001  | Define variable before use               |
| Duplicate definition         | E002  | Use different name or different scope    |
| Type mismatch (assignment)   | E003  | Use correct type or conversion           |
| Invalid binary op            | E004  | Use compatible types                     |
| Invalid comparison           | E005  | Compare same types                       |
| Non-boolean condition        | E006  | Use comparison operator                  |
| Return type mismatch         | E008  | Return correct type                      |
| Missing return               | E009  | Add return statement                     |
| Out of scope                 | E011  | Define in accessible scope               |

Understanding these errors will make you a better DragonTongue programmer. 🐉
# DragonTongue Type System Documentation

**Version:** 1.0  
**Status:** ✅ Implemented in Day 5

## Table of Contents

1. [Overview](#overview)
2. [Primitive Types](#primitive-types)
3. [Type Rules](#type-rules)
4. [Type Checking Examples](#type-checking-examples)
5. [Type Inference](#type-inference)
6. [Error Messages](#error-messages)

## Overview

DragonTongue is a **statically typed** language with **type inference**. Types are checked at compile time, but you don't always need to write them explicitly.

### Type System Goals

| Goal | Description |
|------|-------------|
| **Safety** | Catch type errors before runtime |
| **Clarity** | Types are explicit when needed |
| **Inference** | Compiler deduces types when possible |
| **Performance** | No runtime type overhead |

## Primitive Types

### Int (Integer)

64-bit signed integer.

| Property | Value |
|----------|-------|
| Size | 64 bits (8 bytes) |
| Range | -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 |
| Literals | `42`, `-17`, `0`, `1234567890` |

**Examples:**
```dragontongue
let x = 42           // Type inferred as Int
let y: Int = 100     // Explicit type annotation
let z = -5           // Negative numbers
```

### Bool (Boolean)

True or false values.

| Property | Value                 |
|----------|-----------------------|
| Size     | 1 byte                |
| Values   | `true`, `false`       |

**Examples:**
```
let active = true
let is_valid = false
let result: Bool = x > y
```

## String

UTF-8 encoded text string.

| Property  | Value               |
|-----------|---------------------|
| Encoding  | UTF-8               |
| Mutable   | No (immutable)      |
| Literals  | Double quotes: "hello" |

**Examples:**
```
let name = "DragonTongue"
let empty = ""
let greeting: String = "Hello"
```

**Void**

Represents "no value" (used for functions that don't return).

**Examples:**
```
fn print_message(msg: String) -> Void {
    println(msg)    // No return statement needed
}
```

### Type Rules

### Variable Declaration

| Rule                          | Example                          |
|-------------------------------|----------------------------------|
| Type can be inferred          | `let x = 42 → Int`                |
| Type annotation optional      | `let x: Int = 42`                 |
| Type must match value         | `let x: Int = "hello"` ❌ Error   |

### Binary Operations

| Operator              | Left Type| Right Type | Result Type            |
|-----------------------|----------|------------|------------------------|
| `+`                   | Int      | Int        | Int                    |
| `+`                   | String   | String     | String                 |
| `+`                   | String   | Int        | String (concatenation) |
| `+`                   | Int      | String     | String (concatenation) |
| `-`                   | Int      | Int        | Int                    |
| `*`                   | Int      | Int        | Int                    |
| `/`                   | Int      | Int        | Int                    |
| `%`                   | Int      | Int        | Int                    |
| `==`, `!=`            | Any      | Same type  | Bool                   |
| `>`, `<`, `>=`, `<=`  | Int      | Int        | Bool                   |
| `&&`, `||`            | Bool     | Bool       | Bool                   |

### Type Compatibility Matrix

|       | Int | Bool | String | Void |
|-------|-----|------|--------|------|
| Int   | ✅  | ❌   | ❌     | ❌   |
| Bool  | ❌  | ✅   | ❌     | ❌   |
| String| ❌  | ❌   | ✅     | ❌   |
| Void  | ❌  | ❌   | ❌     | ✅   |

### Type Checking Examples

### Example 1: Valid Operations
```
let a = 10          // Int
let b = 20          // Int
let c = a + b       // Int (30)

let name = "Dragon" // String
let greeting = "Hello, " + name  // String ("Hello, Dragon")

let is_greater = a > b  // Bool (false)

let x: Int = 42     // Explicit type works
```

### Example 2: Invalid Operations
```
let x = 42
let y = "hello"

let z = x + y       // ❌ ERROR: Cannot add Int and String
                    // (Wait - DragonTongue allows this as concatenation!)

let result = x > y  // ❌ ERROR: Cannot compare Int and String

let flag = true + false  // ❌ ERROR: Cannot add Bools
```

### Example 3: If Condition Must Be Bool
```
let x = 42

if x {              // ❌ ERROR: Condition must be boolean
    println("true")
}

if x > 10 {         // ✅ OK: x > 10 returns Bool
    println("Greater")
}
```

### Example 4: Function Return Types
```
fn add(a: Int, b: Int) -> Int {
    return a + b    // ✅ Returns Int
}

fn bad_add(a: Int, b: Int) -> Int {
    return "hello"  // ❌ ERROR: Expected Int, got String
}

fn greet(name: String) {  // Implicit Void return
    println("Hello, " + name)
    // ✅ No return statement needed
}
```

### Type Inference

DragonTongue infers types when not explicitly specified.

### How Inference Works

| Code                   | Inferred Type | Why                         |
|------------------------|--------------|------------------------------|
| `let x = 42`           | Int          | Integer literal              |
| `let flag = true`      | Bool         | Boolean literal              |
| `let name = "Dragon"`  | String       | String literal               |
| `let sum = a + b`      | Int          | Addition of Ints             |
| `let result = x > y`   | Bool         | Comparison returns Bool      |
| `let message = greet()`| String       | Function returns String      |

### Inference Examples
```
// All types are inferred
let count = 10           // Int
let active = false       // Bool
let text = "Hello"       // String

// Type flows through expressions
let a = 5                // Int
let b = 10               // Int
let c = a + b            // Int (15)
let is_big = c > 20      // Bool (false)

// Function return types inferred when possible
fn add(a, b) {
    return a + b         // Return type inferred as Int
}

// But explicit types help with clarity
fn add(a: Int, b: Int) -> Int {
    return a + b
}
```

### Error Messages

### Error Format

```
Semantic error at <line>:<column>: <message>
```

### Common Errors
**E001: Undefined Variable**
```
let x = y
```
**Error**
```
Semantic error at 1:7: Undefined variable 'y'
```
**E002: Type Mismatch**
```
let x: Int = "hello"
```
**Error**
```
Semantic error at 1:12: Type mismatch: expected Int, got String
```
**E003: Non-boolean Condition**
```
if 42 {
    println("true")
}
```
**Error:**
```
Semantic error at 1:3: If condition must be boolean, got Int
```

**E004: Invalid Binary Operation**
```
let x = true + false
```
**Error:**
```
Semantic error at 1:9: Cannot apply + to Bool and Bool
```
**E005: Duplicate Definition**
```
let x = 1
let x = 2
```
**Error**
```
Error at 2:5: Symbol 'x' already defined
```

### Type Conversion (Built-in Functions)

DragonTongue provides built-in functions for type conversion:

| Function       | Input              | Output | Example                            |
|----------------|--------------------|--------|------------------------------------|
| `to_string()`  | Int, Bool, Float   | String | `to_string(42)` → `"42"`           |
| `to_int()`     | String             | Int    | `to_int("123")` → `123`            |
| `to_float()`   | String, Int        | Float  | `to_float("3.14")` → `3.14`        |
| `len()`        | String             | Int    | `len("hello")` → `5`               |

### Conversion Examples
```
let num = 42
let str = to_string(num)     // "42"

let text = "123"
let value = to_int(text)      // 123

let length = len("DragonTongue")  // 12

let combined = "Number: " + to_string(100)  // "Number: 100"
```

### Type Rules Quick Reference

### Valid Operations Table

| Operation        | Example                | Valid? | Result Type |
|------------------|------------------------|--------|-------------|
| Int + Int        | `5 + 3`                | ✅     | Int         |
| Int + String     | `5 + "hi"`             | ✅     | String      |
| String + Int     | `"hi" + 5`             | ✅     | String      |
| String + String  | `"hi" + "there"`       | ✅     | String      |
| Int - Int        | `5 - 3`                | ✅     | Int         |
| Int * Int        | `5 * 3`                | ✅     | Int         |
| Int / Int        | `6 / 3`                | ✅     | Int         |
| Int > Int        | `5 > 3`                | ✅     | Bool        |
| Int == Int       | `5 == 3`               | ✅     | Bool        |
| String == String | `"hi" == "hi"`         | ✅     | Bool        |
| Bool && Bool     | `true && false`        | ✅     | Bool        |
| Bool \|\| Bool   | `true \|\| false`      | ✅     | Bool        |

### Invalid Operations Table

| Operation        | Example        | Why Invalid                                |
|------------------|---------------|---------------------------------------------|
| Int - String     | `5 - "hi"`    | Subtraction not defined for String          |
| Int > String     | `5 > "hi"`    | Cannot compare Int and String               |
| Bool + Int       | `true + 5`    | Cannot add Bool and Int                     |
| String / String  | `"a" / "b"`   | Division not defined for String             |
| !Int             | `!42`         | Logical not only for Bool                   |

### Future Type Features (Planned)

| Feature            | Description                              | Target Day |
|--------------------|------------------------------------------|------------|
| Structs            | User-defined composite types             | Day 7      |
| Option Types       | `Option<T>` for optional values          | Day 7      |
| Generics           | Type parameters for functions/structs    | Day 10     |
| Type Aliases       | `type UserId = Int`                      | Day 10     |
| Union Types        | `Int \| String`                          | v2.0       |
| Type Classes/Traits| Interface-like behavior                  | v2.0       |

The type system ensures your DragonTongue code is safe and predictable. 🐉
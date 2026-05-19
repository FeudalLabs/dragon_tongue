# DragonTongue Scope Management Documentation

**Version:** 1.0  
**Status:** ✅ Implemented in Day 5

## Table of Contents

1. [Overview](#overview)
2. [What is Scope?](#what-is-scope)
3. [Scope Hierarchy](#scope-hierarchy)
4. [Scope Rules](#scope-rules)
5. [Shadowing](#shadowing)
6. [Examples](#examples)
7. [Best Practices](#best-practices)

## Overview

Scope defines where variables, functions, and types are visible and accessible in your code. DragonTongue uses **lexical scoping** (also called static scoping), meaning the structure of the code determines visibility.

### Why Scope Matters

| Reason | Description |
|--------|-------------|
| **Name Organization** | Same name can be used in different contexts |
| **Encapsulation** | Inner scopes don't pollute outer scopes |
| **Memory Management** | Variables can be freed when scope exits |
| **Code Clarity** | Limits where variables can be accessed |

## What is Scope?

### Simple Definition

A **scope** is a region of code where a variable is valid and accessible.

### Visual Representation
```
┌─────────────────────────────────────────────────────────┐
│ Global Scope                                            │
│ ┌─────────────────────────────────────────────────────┐ │
│ │ Function Scope                                      │ │
│ │ ┌─────────────────────────────────────────────────┐ │ │
│ │ │ Block Scope                                     │ │ │
│ │ │                                                 │ │ │
│ │ └─────────────────────────────────────────────────┘ │ │
│ └─────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────┘
```


### Scopes in DragonTongue

| Scope Type | Created By | Example |
|------------|------------|---------|
| **Global** | Program start | Variables outside functions |
| **Function** | Function definition | Parameters and local vars |
| **Block** | `{ }` braces | If statements, loops, parallel blocks |

---

## Scope Hierarchy

### Global Scope (Level 0)

Top-level scope containing:
- Global variables
- Function definitions
- Struct definitions

```dragontongue
let global_var = 42        // Global scope
let another = "hello"      // Global scope

fn my_function() {         // Global scope (function name)
    // Function body scope
}

struct User {              // Global scope (struct name)
    // Struct fields scope
}
```

### Function Scope (Level 1)
Created when a function is defined. Contains:

- Function parameters
- Local variables
```
fn add(x: Int, y: Int) -> Int {  // x and y in function scope
    let result = x + y            // result in function scope
    return result
}
```

### Block Scope (Level 2+)
Created by `{ }` braces. Contains:

- Block-local variables
- Variables only valid inside the block
```
fn main() {
    let outer = 10           // Function scope
    
    if true {
        let inner = 20       // Block scope (level 2)
        println(inner)       // ✅ Accessible
        println(outer)       // ✅ Can access outer scope
    }
    
    println(inner)           // ❌ NOT accessible (out of scope)
}
```

### Nested Scopes Table

| Level | Scope Type     | Can Access                                      |
|------|----------------|--------------------------------------------------|
| 0    | Global         | Global variables, functions, structs            |
| 1    | Function       | Global + Function parameters + Local variables  |
| 2    | Block          | Global + Function + Outer blocks                |
| 3+   | Deeper blocks  | All outer scopes                                |

### Scope Rules

### Rule 1: Variables Are Only Visible in Their Scope
```
{
    let x = 10
}
println(x)  // ❌ Error: x is not defined
```
### Rule 2: Inner Scopes Can Access Outer Scopes
```
let outer = 42

{
    let inner = 10
    println(outer)  // ✅ OK: can access outer
}
```
### Rule 3: Outer Scopes Cannot Access Inner Scopes
```
{
    let inner = 10
}
println(inner)  // ❌ Error: inner not visible
```
### Rule 4: Same Name in Different Scopes Is Allowed
```
let x = 10           // Global x

fn main() {
    let x = 20       // Different x (function scope)
    println(x)       // Prints 20
    
    {
        let x = 30   // Different x (block scope)
        println(x)   // Prints 30
    }
    
    println(x)       // Prints 20
}

println(x)           // Prints 10
```
### Rule 5: Cannot Duplicate in Same Scope
```
let x = 10
let x = 20  // ❌ Error: duplicate in same scope
```
### Rule 6: Parameters Are in Function Scope
```
fn process(x: Int) {  // x is parameter
    let x = 10        // ❌ Error: x already defined in this scope
}
```
### Shadowing

### What is Shadowing?
When an inner scope defines a variable with the same name as an outer scope variable, it shadows (hides) the outer variable.

### Shadowing Example
```
let x = 10          // Original x

fn main() {
    let x = 20      // Shadows global x inside function
    
    {
        let x = 30  // Shadows function x inside block
        println(x)  // Prints 30
    }
    
    println(x)      // Prints 20
}

println(x)          // Prints 10
```

### Shadowing Rules

| Rule                 | Explanation                                      |
|----------------------|--------------------------------------------------|
| Inner shadows outer  | Most inner variable is used                      |
| No error on shadow   | This is allowed (unlike duplication)             |
| Shadow ends with scope | When scope ends, outer becomes visible again   |

### Shadowing vs Duplication

| Aspect                    | Shadowing      | Duplication                 |
|---------------------------|----------------|-----------------------------|
| Same scope                | ❌ Not allowed | ❌ Not allowed              |
| Different scopes          | ✅ Allowed     | N/A                         |
| Inner variable hides outer| ✅ Yes         | N/A                         |
| Error message             | No error       | "Symbol already defined"   |

### Examples

### Example 1: Basic Scoping

```
// Global scope
let message = "Hello"

fn greet() {
    // Function scope
    let name = "Alice"
    println(message)  // ✅ Access global
    println(name)     // ✅ Access local
}

fn main() {
    // Different function scope
    let name = "Bob"
    println(message)  // ✅ Access global
    println(name)     // ✅ Access local
    
    if true {
        // Block scope
        let temp = 42
        println(message)  // ✅ Access global
        println(name)     // ✅ Access function variable
        println(temp)     // ✅ Access block variable
    }
    
    println(temp)     // ❌ temp not accessible
}
```

### Example 2: Nested Functions (Planned)
```
fn outer() {
    let x = 10
    
    fn inner() {
        // Nested function scope
        let y = 20
        println(x)  // ✅ Can access outer's x
    }
    
    inner()
    println(y)  // ❌ y not accessible
}
```
### Example 3: Loops and Scope
```
fn main() {
    for i in 0..10 {
        // Each iteration has its own scope
        let temp = i * 2
        println(temp)  // ✅ Accessible
    }
    
    println(i)     // ❌ i not accessible
    println(temp)  // ❌ temp not accessible
}
```
### Example 4: If-Else Blocks
```
fn main() {
    let x = 10
    
    if x > 5 {
        let y = 100
        println(x)  // ✅ Access outer
        println(y)  // ✅ Access block
    } else {
        let z = 200
        println(x)  // ✅ Access outer
        println(z)  // ✅ Access this block
    }
    
    println(y)  // ❌ Not accessible (different block)
    println(z)  // ❌ Not accessible (different block)
}
```
### Best Practices

1. Minimize Global Variables

**Avoid:**
```
let counter = 0      // Global mutable state

fn increment() {
    counter = counter + 1
}
```
**Prefer:**
```
fn increment(counter: Int) -> Int {
    return counter + 1
}
```
2. Use Descriptive Names

**Avoid:**

```
let x = 10
let y = 20
```

**Prefer:**
```
let user_count = 10
let max_attempts = 20
```

3. Limit Scope Duration

**Avoid:**
```
let temp = 0
for i in 0..100 {
    temp = i
}
```
**Prefer:**
```
for i in 0..100 {
    let temp = i  // Only exists in loop
}
```

4. Be Explicit About Shadowing

**Unclear:**

```
let x = 10
fn process() {
    let x = 20  // Shadows global - may confuse readers
}
```
**Clear:**
```
let global_x = 10
fn process() {
    let local_x = 20  // Different name, no confusion
}
```

5. Initialize Variables When Declared

**Avoid:**
```
let x: Int
if condition {
    x = 10
}
// x might be uninitialized here
```
**Prefer:**
```
let x = if condition { 10 } else { 0 }
```

### Scope Rules Quick Reference

| Rule               | Description                                        |
|--------------------|----------------------------------------------------|
| Visibility         | Variables visible from declaration to end of scope |
| Outer access       | Inner scopes can access outer variables            |
| Inner isolation    | Outer scopes cannot access inner variables         |
| Shadowing          | Inner scopes can reuse names from outer scopes     |
| No duplicate       | Cannot define same name twice in same scope        |
| Block scope        | `{ }` creates new scope                            |
| Function scope     | Parameters and locals in function scope            |
| Global scope       | Top-level variables and functions                  |

### Common Scope Errors

### Error: Variable Not in Scope
```
if true {
    let x = 10
}
println(x)  // ❌ x not in scope
```

Fix: Define variable before the block:

```
let x = 0
if true {
    x = 10
}
println(x)  // ✅ Works (if x is mutable)
```

### Error: Duplicate in Same Scope
```
fn main() {
    let x = 10
    let x = 20  // ❌ Duplicate
}
```
Fix: Use different names or separate scopes:
```
fn main() {
    let x = 10
    {
        let x = 20  // ✅ Different scope
    }
}
```
### Scope Diagram
```
┌─────────────────────────────────────────────────────────────────┐
│ GLOBAL SCOPE (Level 0)                                          │
│ ┌─────────────────────────────────────────────────────────────┐ │
│ │ let version = "1.0"                                         │ │
│ │                                                             │ │
│ │ ┌─────────────────────────────────────────────────────────┐ │ │
│ │ │ FUNCTION SCOPE: main() (Level 1)                        │ │ │
│ │ │ ┌─────────────────────────────────────────────────────┐ │ │ │
│ │ │ │ let x = 10                                          │ │ │ │
│ │ │ │                                                     │ │ │ │
│ │ │ │ ┌─────────────────────────────────────────────────┐ │ │ │ │
│ │ │ │ │ BLOCK SCOPE: if (Level 2)                       │ │ │ │ | 
│ │ │ │ │ let y = 20                                      │ │ │ │ | 
│ │ │ │ │ println(x)       // ✅ Access Level 1           │ │ │ │ | 
│ │ │ │ │ println(version) // ✅ Access Level 0           │ │ │ │ |
│ │ │ │ └─────────────────────────────────────────────────┘ │ │ │ |
│ │ │ │                                                     │ │ │ |
│ │ │ │ println(y)  // ❌ y not in Level 1                  │ │ │ |
│ │ │ └─────────────────────────────────────────────────────┘ │ │ |
│ │ │                                                         │ │ |
│ │ │ println(x)  // ✅ Access Level 1                        │ │ |
│ │ └─────────────────────────────────────────────────────────┘ │ |
│ │                                                             │ |
│ │ println(x)  // ❌ x not in Level 0                          │ |
│ └─────────────────────────────────────────────────────────────┘ |
└─────────────────────────────────────────────────────────────────┘
```

Understanding scope makes your code predictable and maintainable. 🐉
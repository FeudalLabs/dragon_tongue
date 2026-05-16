# The DragonTongue Book

*Learn DragonTongue in 2 hours*

## Table of Contents

1. [Why DragonTongue?](#1-why-dragontongue)
2. [Installation](#2-installation)
3. [Hello, World](#3-hello-world)
4. [Variables and Types](#4-variables-and-types)
5. [Control Flow](#5-control-flow)
6. [Functions](#6-functions)
7. [Structs](#7-structs)
8. [Concurrency](#8-concurrency)
9. [Error Handling](#9-error-handling)
10. [Standard Library](#10-standard-library)
11. [Putting It All Together](#11-putting-it-all-together)

---

## 1. Why DragonTongue?

**The problem:** Existing languages force you to choose:

- Python is easy but slow
- Rust is fast but hard
- Go is simple but has nil pointers

**The solution:** DragonTongue gives you all three.

### What makes DragonTongue different?

| Feature | DragonTongue | Python | Go | Rust |
|---------|-------------|--------|-----|------|
| No null crashes | ✅ | ❌ | ❌ | ✅ |
| Simple concurrency | ✅ | ❌ | ✅ | ❌ |
| Fast execution | ✅ | ❌ | ✅ | ✅ |
| Easy to learn | ✅ | ✅ | ✅ | ❌ |
| Single binary | ✅ | ❌ | ✅ | ✅ |

---

## 2. Installation

### Prerequisites
- GCC or Clang compiler
- Make

### From source
```bash
git clone https://github.com/codebyrashel/dragontongue
cd dragontongue
make
sudo make install
```

## Verify installation
```
dragontongue --version
# Output: DragonTongue Compiler v0.0.1
```

## Hello, World
Create a file called `hello.dt`:
```
fn main() {
    println("Hello, DragonTongue!")
}
```
Compile and run:

```
dragontongue hello.dt
./hello
```
Expected output: `Hello, DragonTongue!`

## What just happened?

1. `fn main()` - Every program starts here

2. `println()` - Built-in function that prints with newline

3. No semicolons needed (but they're allowed)

## Variables and Types

**Variables are immutable by default**
```
let x = 5          // Cannot change x
let name = "Alice" // Cannot change name
```

## Mutable variables use `var`
```
var counter = 0
counter = counter + 1  // OK
```

## Types are inferred
```
let age = 30        // Type: Int
let price = 19.99   // Type: Float
let active = true   // Type: Bool
let message = "Hi"  // Type: String
```

## Explicit types (optional)
```
let count: Int = 42
let name: String = "Bob"
```

## Basic Types Reference

| Type   | Example       | Size     |
|--------|---------------|----------|
| Int    | 42            | 64-bit   |
| Float  | 3.14          | 64-bit   |
| Bool   | true / false  | 1 byte   |
| String | "hello"       | Variable |

## No null! Use `Option` instead
```
let maybe_value: Option<Int> = Some(42)

match maybe_value {
    Some(x) -> print("Got: ", x)
    None -> print("Nothing here")
}
```

## Control Flow
**If statements**
```
let age = 18

if age >= 18 {
    println("Adult")
} else {
    println("Minor")
}

// If as expression (returns value)
let status = if age >= 18 { "adult" } else { "minor" }
```
**For loops**
```
// Range loop
for i in 0..10 {
    print(i)
}

// Over array
let names = ["Alice", "Bob", "Charlie"]
for name in names {
    println(name)
}

// While loop
var count = 0
while count < 5 {
    print(count)
    count = count + 1
}
```

**Match (pattern matching)**

```
let value = 42

match value {
    0 -> print("zero")
    1 -> print("one")
    42 -> print("the answer!")
    _ -> print("something else")
}
```

## Functions

**Basic function**
```
fn add(a: Int, b: Int) -> Int {
    return a + b
}

let result = add(5, 3)  // result = 8
```

**Implicit return (last expression)**
```
fn multiply(a: Int, b: Int) -> Int = a * b
```
**Multiple return values**

```
fn divide(a: Int, b: Int) -> (Int, Int) {
    return (a / b, a % b)
}

let (quotient, remainder) = divide(10, 3)
```

**Default parameters**
```
fn greet(name: String, prefix: String = "Hello") {
    println(prefix, " ", name)
}

greet("Alice")           // "Hello Alice"
greet("Bob", "Hi")       // "Hi Bob"
```

**Recursion**

```
fn factorial(n: Int) -> Int {
    if n <= 1 {
        return 1
    }
    return n * factorial(n - 1)
}
```

## Structs
```
struct User {
    id: Int
    name: String
    active: Bool
}
```

**Creating instances**
```
let alice = User{
    id: 1,
    name: "Alice",
    active: true
}
```
**Accessing fields**
```
println(alice.name)  // "Alice"
```
**Methods**
```
struct Rectangle {
    width: Float
    height: Float
}

fn Rectangle.area(self) -> Float {
    return self.width * self.height
}

let rect = Rectangle{width: 10, height: 5}
println(rect.area())  // 50
```
## Concurrency

**Spawning tasks**
```
fn slow_computation(x: Int) -> Int {
    sleep(1000)  // 1 second
    return x * 2
}

// Spawn as concurrent task
let handle = spawn slow_computation(42)

// Do other work while it runs
println("Working...")

// Wait for result
let result = await handle
println("Result: ", result)
```

**Parallel blocks (auto-wait)**
```
parallel {
    user = fetch_user(1)
    posts = fetch_posts(1)
}
// Both complete before continuing
println(user, posts)
```
**Channels (communication)**
```
let ch = channel(Int)

// Spawn a sender
spawn {
    send(ch, 42)
}

// Receive value
let value = recv(ch)
println(value)  // 42
```

## Error Handling
**Functions that can fail**
```
fn read_file(path: String) -> String error IOError {
    let content = open(path)?  // ? bubbles error
    return content
}
```
**Catching errors**
```
catch read_file("data.txt") as err {
    println("Failed:", err)
    return
} else content {
    println("Success:", content)
}
```
**Custom error types**
```
struct ValidationError {
    field: String
    message: String
}

fn validate_age(age: Int) -> Bool error ValidationError {
    if age < 0 {
        return ValidationError{field: "age", message: "Negative"}
    }
    if age > 150 {
        return ValidationError{field: "age", message: "Too old"}
    }
    return true
}
```

## Standard Library

**Built-in functions (no imports needed)**
```
// I/O
print("Hello")           // No newline
println("World")         // With newline
let input = read_line()  // Read from stdin

// Strings
let len = length("hello")           // 5
let upper = to_upper("hello")       // "HELLO"
let parts = split("a,b,c", ",")     // ["a","b","c"]

// Numbers
let n = to_int("42")                // 42
let s = to_string(3.14)             // "3.14"

// Time
sleep(1000)                         // Sleep 1 second
let now = time_ms()                 // Current timestamp

// Collections
let arr = [1, 2, 3]
push(arr, 4)                        // [1,2,3,4]
let last = pop(arr)                 // 4
let len = length(arr)               // 3
```
**HTTP client/server**
```
import http

// Server
fn handle(req: http.Request) -> http.Response {
    return http.html("<h1>Hello!</h1>")
}

let server = http.serve(":8080", handle)

// Client
let response = http.get("https://api.example.com/users")
println(response.body)
```

**JSON**
```
import json

let data = json.decode("{\"name\":\"Alice\"}")
println(data["name"])  // "Alice"

let output = json.encode({"name": "Bob", "age": 30})
```
## Putting It All Together
**Complete web server example**
```
import http
import json

struct User {
    id: Int
    name: String
}

let users = [
    User{id: 1, name: "Alice"},
    User{id: 2, name: "Bob"}
]

fn get_user(id: Int) -> Option<User> {
    for user in users {
        if user.id == id {
            return Some(user)
        }
    }
    return None
}

fn handle_request(req: http.Request) -> http.Response {
    match req.method + " " + req.path {
        "GET /" -> {
            return http.html("<h1>User API</h1>")
        }
        "GET /users" -> {
            let data = json.encode(users)
            return http.json(data)
        }
        "GET /users/" + id_str -> {
            let id = to_int(id_str)
            match get_user(id) {
                Some(user) -> {
                    return http.json(json.encode(user))
                }
                None -> {
                    return http.not_found("User not found")
                }
            }
        }
        _ -> return http.not_found()
    }
}

fn main() {
    println("Server running on http://localhost:8080")
    let server = http.serve(":8080", handle_request)
    await server
}
```

**Concurrent worker example**
```
fn worker(id: Int, jobs: channel<Int>, results: channel<Int>) {
    for job in recv(jobs) {
        println("Worker ", id, " processing job ", job)
        let result = job * 2
        send(results, result)
    }
}

fn main() {
    let jobs = channel(Int, 100)
    let results = channel(Int, 100)
    
    // Start 4 workers
    for i in 0..4 {
        spawn worker(i, jobs, results)
    }
    
    // Send 10 jobs
    for i in 0..10 {
        send(jobs, i)
    }
    
    // Collect results
    for i in 0..10 {
        let result = recv(results)
        println("Result: ", result)
    }
}
```

## Next Steps

To get started:

1. Read `LANGUAGE_REFERENCE.md` for full language details  
2. Run the examples in `EXAMPLES.md`  
3. Join the community (Discord coming soon)  
4. Start building with **DragonTongue**

Happy coding! 🐉
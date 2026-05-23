# DragonTongue Programming Language

![Status](https://img.shields.io/badge/status-beta-green)
![License](https://img.shields.io/badge/license-MIT-blue)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey)
![Build](https://img.shields.io/badge/build-passing-brightgreen)

**DragonTongue** is a systems programming language designed for:

- ⚡ **Rust-like speed** (stack-first memory, no GC pauses)
- 🐍 **Python-like simplicity** (familiar syntax, 2-hour learning curve)
- 🚀 **Go-like deployment** (single static binaries)
- 📦 **No ecosystem chaos** (batteries-included standard library)

> *"The language that breathes fire without burning your brain."*

## Quick Start

```bash
# Clone the repository
git clone https://github.com/FeudalLabs/dragontongue
cd dragontongue

# Build the compiler
make

# Run your first DragonTongue program
./dragontongue examples/hello.dt
./hello
```

## Example

```
// Hello World in DragonTongue
let greeting = "Hello, DragonTongue!"
println(greeting)

// Variables and conditionals
let x = 42
let y = 10

if x > y {
    println("x is greater")
} else {
    println("y is greater")
}

// Functions
fn add(a, b) {
    return a + b
}

let sum = add(5, 3)
println("Sum: " + to_string(sum))
```

### Output:

```
Hello, DragonTongue!
x is greater
Sum: 8
```

## Language Features

| Feature              | Status     | Example                                          |
|----------------------|------------|--------------------------------------------------|
| Variables            | ✅ Complete | `let x = 42`                                     |
| Integers             | ✅ Complete | `42`, `-17`, `0`                                 |
| Strings              | ✅ Complete | `"Hello"`                                        |
| If-Else              | ✅ Complete | `if x > y { ... } else { ... }`                  |
| Comparisons          | ✅ Complete | `>`, `<`, `>=`, `<=`, `==`, `!=`                 |
| Arithmetic           | ✅ Complete | `+`, `-`, `*`, `/`                               |
| Functions            | ✅ Complete | `fn add(a, b) { return a + b }`                  |
| Function Calls       | ✅ Complete | `add(5, 3)`                                      |
| Return Statements    | ✅ Complete | `return value`                                   |
| Built-in Functions   | ✅ Complete | `println`, `print`, `to_string`, `to_int`, `len` |
| String Concatenation | ✅ Complete | `"Hello, " + name`                               |
| Comments             | ✅ Complete | `// line comment`                                |
| Blocks               | ✅ Complete | `{ ... }`                                        |
| Nested Scopes        | ✅ Complete | Variables inside blocks                          |

## Coming Soon

| Feature                              | Status     |
|--------------------------------------|------------|
| Structs                              | 📝 Planned | 
| Loops (for, while)                   | 📝 Planned | 
| Arrays                               | 📝 Planned | 
| Concurrency (spawn, await, parallel) | 📝 Planned | 
| Pattern Matching (match)             | 📝 Planned | 
| File I/O                             | 📝 Planned | 
| Package Manager                      | 📝 Planned |

## Documentation

| Document              | Description                 |
|-----------------------|-----------------------------|
| BOOK.md               | Complete tutorial (2 hours) |
| LANGUAGE_REFERENCE.md | Full language specification |
| ARCHITECTURE.md       | Compiler internals          |
| CODEGEN_API.md        | Code generator API          |
| RUNTIME_API.md        | Runtime library             |
| BUILTINS.md           | Built-in functions          |
| FAQ.md                | Common questions            |

## Installation

### Prerequisites
- GCC or Clang
- Make

## From Source

```bash
git clone https://github.com/FeudalLabs/dragontongue
cd dragontongue
make
sudo make install
```

## Verify Installation
```bash
dragontongue --version
# DragonTongue Compiler v0.1.0
```

## Project Structure
```bash
dragontongue/
├── src/               # Compiler source (C)
│   ├── lexer/         # Tokenization
│   ├── parser/        # AST construction
│   ├── semantic/      # Type checking & symbols
│   └── codegen/       # C code generation
├── runtime/           # Runtime library
├── tests/             # Test suite
├── docs/              # Complete documentation
├── examples/          # Example programs
└── Makefile           # Build system
```

## Current Phase: Code Generation Complete

DragonTongue can now:

- ✅ Parse DragonTongue source code
- ✅ Perform semantic analysis
- ✅ Generate C code
- ✅ Compile to executable binary
- ✅ Run programs with variables, functions, if statements, and built-ins

## Next Up
- Structs (user-defined types)
- Loops (for, while)
- Arrays

## Testing

Run the test suite:

```bash
make test
```

Run example programs:

```bash
# Minimal example
./dragontongue examples/minimal.dt
./output          # Output: 42

# Simple program
./dragontongue examples/simple.dt
./output          # Output: Hello / 42

# Full feature test
./dragontongue examples/test.dt
./output          # Output: Hello, DragonTongue! / x is greater / Sum: 8
```

## Contributing

We welcome contributors of all skill levels!

- Read [CONTRIBUTING.md](/docs/CONTRIBUTING.md)
- Check `good-first-issue` labels
- Join discussions

## Areas Needing Help

| Area              | Difficulty   | Description                 |
|-------------------|--------------|-----------------------------|
| VS Code Extension | Beginner     | Improve syntax highlighting |
| Documentation     | Beginner     | Examples, tutorials         |
| Testing           | Beginner     | Write test cases            |
| Standard Library  | Intermediate | Add built-in functions      |
| Structs           | Intermediate | User-defined types          |
| Loops             | Intermediate | For and while loops         |

## License

MIT License - free for any use.

## Acknowledgments

Inspired by:

- Rust - Memory safety concepts
- Python - Syntax simplicity
- Go - Deployment simplicity
- Zig - Compilation model

## Contact

- Issues: GitHub Issues
- Discussions: GitHub Discussions
- Email: rothstaylor@yahoo.com

May your code be swift and your dragons fierce. 🐉
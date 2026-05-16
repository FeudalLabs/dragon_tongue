# DragonTongue Programming Language

![Status](https://img.shields.io/badge/status-alpha-orange)
![License](https://img.shields.io/badge/license-MIT-blue)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey)

**DragonTongue** is a new systems programming language designed for:

- ⚡ **Rust-like speed** (stack-first memory, no GC pauses)
- 🐍 **Python-like simplicity** (familiar syntax, 2-hour learning curve)
- 🚀 **Go-like deployment** (single static binaries)
- 📦 **No ecosystem chaos** (batteries-included standard library)

> *"The language that breathes fire without burning your brain."*

---

## Quick Start

```bash
# Clone the repository
git clone https://github.com/yourusername/dragontongue
cd dragontongue

# Build the compiler
make

# Run your first DragonTongue program
./dragontongue examples/hello.dt
./hello
```

## Example
```
// A concurrent web server in DragonTongue
import http

fn handle(req: http.Request) -> http.Response {
    match req.path {
        "/" -> return http.html("<h1>DragonTongue!</h1>")
        "/api/users" -> return http.json(get_users())
        _ -> return http.not_found()
    }
}

fn main() {
    println("Server running on http://localhost:8080")
    let server = http.serve(":8080", handle)
    await server
}
```

## 📚 Documentation

| Document | Description |
|----------|------------|
| Book | Complete tutorial (2 hours) |
| Language Reference | Full specification |
| Architecture Guide | Compiler internals |
| FAQ | Common questions |
| Development Diary | Day-by-day progress |

## Installation
**Prerequisites**
- GCC or Clang
- Make

**From source**
```bash
git clone https://github.com/yourusername/dragontongue
cd dragontongue
make
sudo make install
```
**Verify**
```bash
dragontongue --version
# DragonTongue Compiler v0.0.1
```

## Language Features

| Feature | Status | Documentation |
|--------|--------|--------------|
| Lexer | 🚧 In Progress | ARCHITECTURE.md |
| Parser | 📝 Planned | - |
| Type System | 📝 Planned | - |
| Concurrency | 📝 Planned | BOOK.md#concurrency |
| Error Handling | 📝 Planned | BOOK.md#error-handling |
| Standard Library | 📝 Planned | STANDARD_LIBRARY.md |

## Project Structure

```text
dragontongue/
├── src/           # Compiler source (C)
├── runtime/       # Runtime library
├── tests/         # Test suite
├── docs/          # Complete documentation
├── examples/      # Example programs
└── Makefile       # Build system
```

## Development Status

**Current Phase:** Lexer (Week 1–2)

- ✅ Project setup  
- ✅ Documentation (7+ files, 800+ lines)  
- ✅ Arena allocator  
- ✅ Token definitions  
- 🚧 Source reader (in progress)  
- 📝 Scanner / lexer  
- 📝 Parser  
- 📝 Semantic analysis  
- 📝 Code generation  

See **Development Diary** for daily progress.

## Contributing

We welcome contributors of all skill levels!

- Read `CONTRIBUTING.md`  
- Check `good-first-issue` labels  
- Join discussions  


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

*May your code be swift and your dragons fierce.*
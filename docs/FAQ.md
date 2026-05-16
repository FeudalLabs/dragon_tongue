# Frequently Asked Questions

## General

### Why create another programming language?

Existing languages force tradeoffs:
- Python: Easy but slow
- Rust: Fast but hard
- Go: Simple but has nil pointers

DragonTongue aims to eliminate these tradeoffs.

### Is DragonTongue production-ready?

Not yet. We're in early development (v0.0.1). The lexer is being built.

### What license does DragonTongue use?

MIT License - completely free for any use.

---

## Language Features

### Does DragonTongue have null?

**No.** Null is a billion-dollar mistake. Use `Option<T>` instead.

### Does DragonTongue have a garbage collector?

**No.** Memory is stack-based by default. Heap allocation is explicit with ownership transfer.

### How is concurrency different from Go?

DragonTongue tasks are similar to goroutines, but:
- No `nil` channels
- No deadlocks by design (simpler semantics)
- Explicit `spawn` and `await` (no hidden scheduling)

### Can I use DragonTongue for web frontend?

Not yet. v1 focuses on backend. WASM target is planned for v2.

### Does DragonTongue have generics?

v1 has simple templates (compile-time code generation). Full generics are planned for v2.

---

## Performance

### How fast is DragonTongue?

Target: Within 2x of Rust, 50x faster than Python, 5x faster than Go.

### Does DragonTongue have runtime pauses?

**No.** Stack allocation means no GC pauses. Heap allocation is explicit and rare.

### Can I write real-time systems?

Not in v1. Real-time requires more precise control over scheduling.

---

## Learning

### How long to learn DragonTongue?

About 2 hours for experienced developers. Read the [BOOK.md](BOOK.md).

### Do I need to know C?

To **use** DragonTongue: No.  
To **contribute** to the compiler: Yes, the compiler is written in C.

### What's the best way to start?

1. Read [BOOK.md](BOOK.md)
2. Try examples in [EXAMPLES.md](EXAMPLES.md)
3. Build a small project

---

## Tools

### What editor support exists?

v1: VSCode extension (syntax highlighting only).  
v2: Language Server Protocol (LSP) planned.

### Is there a package manager?

**No.** The standard library is designed to cover 95% of needs. External packages are discouraged in v1.

### Can I call C libraries?

Yes, via FFI (Foreign Function Interface).

---

## Community

### How can I contribute?

Read [CONTRIBUTING.md](CONTRIBUTING.md).

### Is there a code of conduct?

Yes. Be respectful, inclusive, and helpful.

### Where do I report bugs?

GitHub Issues (when repository is public).

---

## Future

### When will v1.0 be released?

Estimated: 6-12 months from start (depending on contributors).

### What's after v1.0?

- v1.5: Package manager (optional)
- v2.0: WASM target, self-hosting compiler
- v3.0: Distributed computing primitives

### Will DragonTongue ever have AI features?

Possibly in v3.0, but not in early versions. Focus is on core language first.

---

## Troubleshooting

### "command not found: dragontongue"

Run `make install` or add `~/dragontongue` to your PATH.

### Compiler crashes with segmentation fault

Run with `--debug` flag and open an issue with the output.

### Tests fail on my machine

Make sure GCC and Make are installed. Run `gcc --version` to verify.

---

## Still have questions?

Open an issue on GitHub or email rothstaylor@yahoo.com
# DragonTongue Documentation

**Version:** 0.0.1 (Development)  
**Status:** Lexer phase - Active development

## What is DragonTongue?

DragonTongue is a new programming language designed to be:

- **Fast as Rust** (stack-first memory, no GC pauses)
- **Simple as Python** (familiar syntax, 2-hour learning curve)
- **Deployable as Go** (single static binaries)
- **No ecosystem chaos** (batteries-included standard library)

## Quick Links

| Document | What you'll learn |
|----------|-------------------|
| [BOOK.md](BOOK.md) | **Start here** - Complete language tutorial |
| [LANGUAGE_REFERENCE.md](LANGUAGE_REFERENCE.md) | Full syntax and semantics reference |
| [ARCHITECTURE.md](ARCHITECTURE.md) | How the compiler works internally |
| [STANDARD_LIBRARY.md](STANDARD_LIBRARY.md) | All built-in functions documented |
| [EXAMPLES.md](EXAMPLES.md) | Real code examples |
| [CONTRIBUTING.md](CONTRIBUTING.md) | How to help build the language |

## For Different Audiences

### I want to LEARN DragonTongue
→ Read [BOOK.md](BOOK.md) from start to finish (30 minutes)

### I want to USE DragonTongue for a project
→ Read [LANGUAGE_REFERENCE.md](LANGUAGE_REFERENCE.md) + [STANDARD_LIBRARY.md](STANDARD_LIBRARY.md)

### I want to CONTRIBUTE to the compiler
→ Read [ARCHITECTURE.md](ARCHITECTURE.md) + [CONTRIBUTING.md](CONTRIBUTING.md)

### I want to FOLLOW development progress
→ Read the [diaries/](diaries/) folder - day-by-day development journal

## Quick Example

```dragontongue
// This is DragonTongue code
fn main() {
    let name = "DragonTongue"
    println("Hello, " + name)
    
    parallel {
        data = fetch_from_api()
        result = process_data()
    }
    
    print(result)
}
```
## Status Legend

| Symbol | Meaning |
|--------|---------|
| ✅ | Implemented |
| 🚧 | In Progress |
| 📝 | Planned     |
| ❌ | Not Started |


## Current Focus

| Component         | Status | Documentation               |
|-------------------|--------|------------------------------|
| Lexer             | 🚧     | ARCHITECTURE.md#lexer        |
| Parser            | 📝     | -                            |
| Semantic Analysis | 📝     | -                            |
| Code Generation   | 📝     | -                            |
| Runtime           | 📝     | -                            |


## Community

We’re building the community around this project.

- Issues: GitHub Issues (coming soon)  
- Discord: https://discord.gg/Bh9ApnKgWJ
- Email: rothstaylor@yahoo.com  

## License

This project is released under the **MIT License**, allowing free use, modification, and distribution.
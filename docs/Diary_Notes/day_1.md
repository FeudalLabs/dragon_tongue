Day 1 - Project Setup

What I learned:
- A simple arena allocator is enough for v1
- Token types are the foundation of everything
- Makefile saves hours of repeated typing

What worked:
- Directory structure is clean
- Compiler builds with no warnings
- Tests pass

What was tricky:
- Getting Makefile indentation right (tabs, not spaces)
- Understanding the difference between TOKEN_ASSIGN and TOKEN_EQ

Tomorrow's goal:
- Write the source reader (file loading + character iteration)
- Add line/column tracking
- Total lines: aim for 100 more

Questions I have:
- Should strings be null-terminated or length-prefixed?
- How to handle UTF-8? (v1: ASCII only, v2 later)

Next step documented in: NEXT_STEPS.md








# DragonTongue Development Diary - Day 1

**Date:** [Today]  
**Focus:** Project setup, documentation structure, lexer foundation  
**Status:** ✅ Complete

---

## What I Learned Today

### 1. Project structure matters
A clean directory layout makes it easier for others to understand:
- `src/` - Compiler source
- `runtime/` - Runtime library
- `tests/` - Test suite
- `docs/` - Documentation (today's focus)
- `examples/` - Example programs

### 2. Documentation is code
Without documentation, a language is just a personal experiment. With documentation, it becomes a tool others can use.

**Key insight:** Write documentation *as you write code*, not after.

### 3. Multiple documentation formats serve different audiences
- `BOOK.md` - Tutorial for learners
- `ARCHITECTURE.md` - Technical reference for contributors
- `LANGUAGE_REFERENCE.md` - Specification for power users
- `FAQ.md` - Quick answers
- `GLOSSARY.md` - Shared vocabulary

### 4. Arena allocator is elegant
The simple arena allocator we wrote (20 lines) will power the entire compiler's memory management. No fragmentation, no leaks.

---

## Code Written Today

| File | Lines | Purpose |
|------|-------|---------|
| `src/dragontongue.h` | 25 | Main header, arena API |
| `src/arena.c` | 20 | Arena implementation |
| `src/lexer/token.h` | 35 | Token definitions |
| `src/main.c` | 15 | Compiler entry point |
| `Makefile` | 25 | Build system |
| `tests/test_arena.c` | 20 | Arena tests |
| `docs/*.md` | ~800 | Complete documentation |

**Total code: ~140 lines**  
**Total documentation: ~800 lines**

---

## Documentation Created

| Document | Audience | Status |
|----------|----------|--------|
| `docs/README.md` | Everyone | ✅ |
| `docs/BOOK.md` | Learners | ✅ |
| `docs/ARCHITECTURE.md` | Contributors | ✅ |
| `docs/CONTRIBUTING.md` | Contributors | ✅ |
| `docs/GLOSSARY.md` | Everyone | ✅ |
| `docs/FAQ.md` | Everyone | ✅ |
| `docs/diaries/DAY_001.md` | Maintainers | ✅ |

---

## Commands Used Today

```bash
# Project setup
mkdir -p ~/dragontongue/{src/lexer,tests,runtime,examples,docs/diaries}
cd ~/dragontongue

# Build
make

# Test
make test

# Verify compiler works
./dragontongue
# Output: DragonTongue Compiler v0.0.1
# DragonTongue Development Diary - Day 2

**Focus:** Source Reader implementation  
**Status:** ✅ Complete

## What I Learned Today

### 1. Line/column tracking is subtle
Newlines (`\n`) reset column to 1, increment line. Other characters increment column. Windows `\r\n` would need special handling - decided to skip `\r` as whitespace for v1.

### 2. Arena allocation simplifies memory
No need to track individual allocations. Source content, filename, and struct all come from the same arena. Freed automatically.

### 3. Testing early prevents bugs
Wrote tests BEFORE implementing some functions. Found off-by-one errors in column tracking immediately.

### 4. Peek-ahead is useful for lexer
Being able to look at `source_peek_ahead(src, 1)` will help when tokenizing two-character operators like `==` and `!=`.

## Code Written Today

| File | Lines | Purpose |
|------|-------|---------|
| `src/lexer/source.h` | 44 | Source reader interface |
| `src/lexer/source.c` | 119 | Implementation |
| `tests/test_source.c` | 147 | Comprehensive tests |
| **TOTAL** | **310** | |


## Test Results
```
=== Testing Source Reader ===
✓ test_source_from_string passed
✓ test_source_peek passed
✓ test_source_advance passed
✓ test_line_column_tracking passed
✓ test_source_skip_whitespace passed
✓ test_source_eof passed
✓ test_position_to_line_column passed

✓ All source reader tests passed!
```

**Coverage:** 100% of source reader functions tested

## Challenges Faced

### Challenge 1: Off-by-one in column tracking
**Problem:** Column started at 0 instead of 1  
**Solution:** Initialize column = 1, increment BEFORE reading? No, increment AFTER reading current char  
**Fix:** Column tracks position AFTER advancing

### Challenge 2: EOF handling
**Problem:** `source_peek()` on empty file crashed  
**Solution:** Check `source_is_eof()` before accessing content array

### Challenge 3: File reading with arena
**Problem:** Need to know size before allocating  
**Solution:** Use `ftell()` to get size, allocate arena memory, then read

## 📚 Resources Used Today

- **C File I/O**  
  [fopen() Documentation](https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm)

- **Arena Allocator Pattern**  
  [Untangling Lifetimes: The Arena Allocator](https://www.dgtlgrove.com/p/untangling-lifetimes-the-arena-allocator)

- **Line Counting Algorithms**  
  [Newline - Wikipedia](https://en.wikipedia.org/wiki/Newline)


# End of Day Status

- ✅ Source reader complete
- ✅ All tests passing
- ✅ Documentation updated
- ✅ Ready for scanner implementation

# The dragon can now read files. 🐉

## Summary of Day 2 Achievements

| Area | Before | After |
|------|--------|-------|
| **Source loading** | ❌ None | ✅ File + string loading |
| **Character iteration** | ❌ None | ✅ Peek, advance, peek-ahead |
| **Line/column tracking** | ❌ None | ✅ Accurate position tracking |
| **Documentation** | Architecture outline | Detailed source reader section |

## Current Project Structure
```
~/dragontongue/
├── dragontongue # Compiler binary
├── Makefile # Updated with source tests
├── src/
│ ├── dragontongue.h
│ ├── main.c
│ ├── arena.c
│ └── lexer/
│ ├── token.h # Day 1
│ ├── source.h # Day 2 ✨ NEW
│ └── source.c # Day 2 ✨ NEW
├── tests/
│ ├── test_arena.c # Day 1
│ └── test_source.c # Day 2 ✨ NEW
├── docs/
│ ├── README.md
│ ├── BOOK.md
│ ├── ARCHITECTURE.md # Updated with source reader
│ ├── LANGUAGE_REFERENCE.md
│ ├── CONTRIBUTING.md
│ ├── GLOSSARY.md
│ ├── FAQ.md
│ └── diaries/
│ ├── DAY_001.md
│ └── DAY_002.md # Day 2 ✨ NEW
├── runtime/
├── examples/
└── build/
```

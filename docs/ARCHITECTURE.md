# DragonTongue Compiler Architecture

DragonTongue is a **transpiler** that converts `.dt` source code to C, then uses GCC/Clang to produce native binaries.

.dt Source → Lexer → Parser → Semantic Analysis → C Code → Binary


## Why this architecture?

| Decision | Reason |
|----------|--------|
| Transpile to C | Reuse mature C compiler optimizations |
| No custom backend | Save months of work |
| Simple pipeline | Easy to understand and debug |

---

## Component Breakdown

### Lexer (`src/lexer/`)

**Purpose:** Convert source text into tokens.

**Files:**
- `token.h` - Token type definitions
- `source.h/c` - Character reader with line/column tracking
- `scanner.h/c` - Token generation

**How it works:**
```c
Input: "let x = 42"
Tokens: [LET] [IDENTIFIER("x")] [ASSIGN] [NUMBER(42)]
```
**Key functions:**
```c
Token scanner_next(Scanner* s);
char source_peek(Source* s);
char source_advance(Source* s);
```
### Parser `(src/parser/)`
**Purpose:** Build Abstract Syntax Tree (AST) from tokens.

**Grammar (simplified):**
```
program     → statement*
statement   → let_stmt | fn_stmt | if_stmt | ...
let_stmt    → "let" IDENTIFIER "=" expression
expression  → term (("+" | "-") term)*
term        → factor (("*" | "/") factor)*
```
**AST Node Example:**

```c
// let x = 5 + 3
Node {
    type: NODE_LET_STMT,
    binding: {
        name: "x",
        value: {
            type: NODE_BINARY_OP,
            binary: {
                op: '+',
                left: {type: NODE_NUMBER, value: 5},
                right: {type: NODE_NUMBER, value: 3}
            }
        }
    }
}
```

### Semantic Analyzer `(src/semantic/)`
**Purpose:** Type checking, symbol resolution, validation.

**Checks performed:**

- Variables defined before use
- Function return types match
- Binary operators have compatible types
- No duplicate definitions
- Type inference for unannotated variables

**Symbol Table:**
```c
struct SymbolTable {
    Symbol** entries;     // hash table
    SymbolTable* parent;  // for nested scopes
    int scope_level;
};
```
**Type System:**
```c
typedef enum {
    TYPE_INT,
    TYPE_BOOL,
    TYPE_STRING,
    TYPE_FUNCTION,
    TYPE_STRUCT
} TypeKind;
```

## Code Generator `(src/codegen/)`
**Purpose:** Convert AST to C code.

**Mapping DragonTongue → C:**

| Source Code (DragonTongue) | Output (C) |
|----------------------------|------------|
| `let x = 5`               | `int x = 5;` |
| `let name = "hi"`         | `string_t name = string_new("hi");` |
| `fn add(a,b){...}`        | `int add(int a, int b) { ... }` |
| `spawn foo()`             | `task_t* t = task_spawn(foo, NULL);` |
| `await t`                 | `task_await(t);` |

---

## Runtime Library (`runtime/`)

- String handling
- Task system (M:N threading)  
- Channels  
- Hash maps  
- I/O

### Runtime `(runtime/)`
**Purpose:** Provide features not available in C.

**Components:**

**String**
```c
typedef struct {
    char* data;
    int64_t length;
    int64_t capacity;
} string_t;
```
**Task System**
```c
typedef struct task {
    void* (*fn)(void*);
    void* arg;
    void* result;
    int status;  // 0=ready, 1=running, 2=done
} task_t;

task_t* task_spawn(void* (*fn)(void*), void* arg);
void* task_await(task_t* task);
```
**Channel**
```c
typedef struct channel {
    void** buffer;
    int capacity;
    int head, tail;
    // ... synchronization primitives
} channel_t;

void channel_send(channel_t* ch, void* value);
void* channel_recv(channel_t* ch);
```

## Source Reader (`src/lexer/source.c`)

The Source Reader is responsible for loading source files and providing character-by-character access with line/column tracking.

### API Reference

| Function | Purpose |
|----------|---------|
| `source_from_file()` | Load a .dt file from disk |
| `source_from_string()` | Create source from string (testing) |
| `source_peek()` | Look at current character without consuming |
| `source_peek_ahead()` | Look ahead N characters |
| `source_advance()` | Consume and return current character |
| `source_is_eof()` | Check if end of file reached |
| `source_skip_whitespace()` | Skip spaces, tabs, newlines |
| `source_get_location()` | Get current line/column |

### Implementation Details

**Line/Column Tracking:**
- Lines start at 1
- Columns start at 1
- Newline (`\n`) increments line, resets column to 1
- All other characters increment column

**Memory Management:**
- All Source structs are allocated from the arena
- File content is copied into arena memory
- No manual free needed - arena freed at program exit

### Example Usage

```c
Source* src = source_from_file("program.dt", arena);

while (!source_is_eof(src)) {
    char c = source_advance(src);
    int line, col;
    source_get_location(src, &line, &col);
    printf("Char '%c' at %d:%d\n", c, line, col);
}
```



## Directory Structure
```
dragontongue/
├── src/                    # Compiler source
│   ├── dragontongue.h      # Main header
│   ├── main.c              # Entry point
│   ├── arena.c             # Memory allocator
│   ├── lexer/              # Lexical analysis
│   │   ├── token.h
│   │   ├── source.h
│   │   ├── source.c
│   │   ├── scanner.h
│   │   └── scanner.c
│   ├── parser/             # Syntax analysis
│   │   ├── ast.h
│   │   ├── parser.h
│   │   └── parser.c
│   ├── semantic/           # Type checking
│   │   ├── symbol.h
│   │   ├── type.h
│   │   └── checker.c
│   └── codegen/            # C output
│       ├── cgen.h
│       └── cgen.c
├── runtime/                # Runtime library
│   ├── runtime.h
│   ├── string.c
│   ├── task.c
│   ├── channel.c
│   ├── map.c
│   └── stdlib.c
├── tests/                  # Test suite
│   ├── test_lexer.c
│   ├── test_parser.c
│   └── test_runtime.c
├── docs/                   # Documentation
│   ├── BOOK.md
│   ├── ARCHITECTURE.md
│   └── ...
└── examples/               # Example programs
    ├── hello.dt
    ├── webserver.dt
    └── ...
```

## Data Flow

Source File  
→ Lexer → Token Stream  
→ Parser → AST  
→ Semantic Analyzer → Annotated AST + Symbol Table  
→ Code Generator → C Code  
→ GCC / Clang → Binary  
→ Executable

## Error Handling Strategy
**Compiler errors are user-friendly:** DragonTongue compiler provides clear, human-readable diagnostics with source context and hints.

```
Error at hello.dt:5:12: Expected Int but got String
  |
5 | let x: Int = "hello"
  |             ^^^^^^^
  |
  = Hint: Use to_int() to convert string to integer
```

Errors include:
- File name, line, and column location  
- Code excerpt with pointer marker  
- Explanatory message  
- Optional fix suggestion (hint system)

### Error Codes

- **E001** — Undefined variable  
- **E002** — Type mismatch  
- **E003** — Duplicate definition  
- **E004** — Invalid syntax  
- **E005** — Unhandled compiler error  

## Memory Management

**Compiler memory:** Arena allocator

- All allocations come from a single pool
- Freed all at once at program exit
- No leaks, no fragmentation

**Runtime memory:**

- Stack by default (no allocation)
- Heap only with explicit `heap` keyword
- Linear ownership for heap data

## Performance Considerations

| Component    | Target     | Strategy |
|--------------|------------|----------|
| Lexer        | Fast       | Single-pass, no backtracking |
| Parser       | Fast       | Recursive descent, minimal lookahead |
| Type Checking| Acceptable | Hash-based symbol lookup |
| Code Gen     | Acceptable | Direct AST to C translation |
| Runtime      | Fast       | M:N threads, work stealing |

## Extending the Compiler

### Adding a New Keyword

1. Add token to `token.h`  
2. Add keyword to `keywords.c`  
3. Add parsing rule in `parser.c`  
4. Add semantic check in `checker.c`  
5. Add code generation in `cgen.c`  

---

### Adding a New Type

1. Add `TypeKind` in `type.h`  
2. Add parsing support for type syntax  
3. Add type checking rules  
4. Add C type mapping in code generator  

## Testing Strategy
```bash
# Unit tests (per component)
make test-lexer
make test-parser
make test-semantic

# Integration tests
make test

# Runtime tests
make test-runtime

# Full suite with coverage
make test-coverage
```

## Future Architecture Plans

### Phase 2: Self-hosting
- Rewrite the compiler in DragonTongue itself  

### Phase 3: Direct LLVM Backend
- Remove C dependency  
- Better optimizations

### Phase 4: WebAssembly (WASM) Target
- Compile directly to WebAssembly  
- Run in browsers  

## Contributing Guidelines

See `CONTRIBUTING.md` for full details on:

- Code style  
- Commit conventions  
- Pull request process  
- Testing requirements  
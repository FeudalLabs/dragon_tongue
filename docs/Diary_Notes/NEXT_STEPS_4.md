## Next Steps (Day 6)

### Code Generation (C Backend)

Now that DragonTongue has a working parser and semantic analyzer, the next major phase is **Code Generation** - transforming the AST into executable C code.

### Goals:

- Generate C code from DragonTongue AST
- Implement runtime library (memory management, concurrency)
- Create compilation pipeline (DragonTongue → C → Binary)
- Support basic data types (Int, Bool, String)
- Support control flow (if, while, for, return)
- Support function calls
- Support built-in functions (println, print, len, to_string, to_int)

### What Will Be Generated

| DragonTongue Code | Generated C Code |
|-------------------|------------------|
| `let x = 42` | `int x = 42;` |
| `let name = "Dragon"` | `string_t name = string_new("Dragon");` |
| `println("Hello")` | `printf("%s\n", "Hello");` |
| `fn add(a, b) { return a + b; }` | `int add(int a, int b) { return a + b; }` |
| `if x > 10 { ... }` | `if (x > 10) { ... }` |

### Preview: Code Generation API

```c
typedef struct CodeGenerator {
    FILE* output;           // Output C file
    SymbolTable* symbols;   // For variable lookups
    Arena* arena;           // Memory arena
    int indent_level;       // For pretty printing
    int temp_counter;       // For temporary variables
} CodeGenerator;

// Initialize code generator
CodeGenerator* cgen_new(const char* output_filename, Arena* arena);

// Generate code from AST
void cgen_generate(CodeGenerator* cg, ASTNode* program);

// Generate specific constructs
void cgen_generate_statement(CodeGenerator* cg, ASTNode* node);
void cgen_generate_expression(CodeGenerator* cg, ASTNode* node);
void cgen_generate_function(CodeGenerator* cg, Function* func);
void cgen_generate_if_statement(CodeGenerator* cg, IfStmt* stmt);

// Helper functions
void cgen_indent(CodeGenerator* cg);
void cgen_writeln(CodeGenerator* cg, const char* fmt, ...);
```

# Contributing to DragonTongue

Thank you for your interest! DragonTongue is open source and welcomes contributors of all skill levels.

## Ways to Contribute

| Area | Difficulty | What to do |
|------|------------|------------|
| **Documentation** | Beginner | Fix typos, clarify explanations, add examples |
| **Testing** | Beginner | Write test cases, report bugs |
| **Examples** | Beginner | Create example programs |
| **Lexer** | Intermediate | Improve tokenization, error recovery |
| **Parser** | Intermediate | Fix grammar issues, better error messages |
| **Semantic analysis** | Advanced | Type inference, validation rules |
| **Runtime** | Advanced | Concurrency, performance optimization |
| **Code generation** | Advanced | Better C output, optimizations |

---

## Getting Started

### 1. Set up development environment

```bash
git clone https://github.com/yourname/dragontongue
cd dragontongue
make
make test
```

### Find an issue
Look for labels:

- `good-first-issue`
- `help-wanted`
- `documentation`

### Make changes

```bash
# Create a branch
git checkout -b feature/your-feature-name

# Make changes
# ...

# Run tests
make test

# Commit
git commit -m "Add: description of changes"
```

### Submit pull request
Push to GitHub and open a PR.

**Code Style**

**C code (compiler)**
```c
// Use snake_case for functions and variables
void parse_expression(Parser* parser);

// Use PascalCase for types
typedef struct SymbolTable SymbolTable;

// Braces on same line
if (condition) {
    do_something();
}

// 4 spaces, no tabs
// Maximum line length: 80 characters
// Comments explain WHY, not WHAT
```

## DragonTongue code (examples)
```
// Use snake_case
let user_name = "Alice"

// Space after commas
let numbers = [1, 2, 3]

// Space around operators
let sum = a + b
```

## Commit Conventions
```
type(scope): description

Types:
- feat: New feature
- fix: Bug fix
- docs: Documentation
- test: Testing
- refactor: Code change
- style: Formatting

Scope:
- lexer, parser, semantic, codegen, runtime, docs, tests

Example:
feat(lexer): Add support for hex numbers
fix(parser): Fix operator precedence for multiplication
docs(book): Add chapter on error handling
```

## Testing Requirements
**Every change must include tests**
```c
// tests/test_feature.c
TEST(test_new_feature) {
    const char* input = "...";
    ASTNode* result = parse_string(input);
    assert(result != NULL);
    assert(result->type == EXPECTED_TYPE);
}
```
## Run all tests before pushing
```bash
make test
make test-coverage  # Aim for >80% coverage
```

## Documentation Requirements

**New features need docs**

1. Update `LANGUAGE_REFERENCE.md`
2. Add example to `EXAMPLES.md`
3. Update `BOOK.md` if needed

**Changed behavior needs docs**

- Update relevant documentation
- Add note in `CHANGELOG.md`

## Code Review Process
1. Automated checks: Tests must pass
2. Human review: At least one maintainer approves
3. Documentation: Must be updated
4. Merge: Squash and merge to main

# Getting Help

- Read [ARCHITECTURE.md](ARCHITECTURE.md)
- Check [FAQ.md](FAQ.md)
- Open a discussion on GitHub
- Email maintainers

# Recognition

Contributors are recognized in:

- `CONTRIBUTORS.md` file
- Release notes
- Documentation credits

---

Thank you for helping build **DragonTongue! 🐉**
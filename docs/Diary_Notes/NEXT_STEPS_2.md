## Next Steps (Tomorrow - Day 3)

### Priority: Scanner (Token generation)
- [ ] Create `src/lexer/scanner.h`
- [ ] Implement `scanner_next_token()`
- [ ] Tokenize keywords (let, fn, if, etc.)
- [ ] Tokenize identifiers
- [ ] Tokenize numbers
- [ ] Tokenize strings
- [ ] Tokenize operators
- [ ] Write comprehensive tests

### Code Target
- [ ] Write ~200 lines of new code
- [ ] Pass all scanner tests

### Preview: Scanner API

```c
typedef struct Scanner {
    Source* source;
    Token current;
    Token previous;
    ErrorList* errors;
} Scanner;

Token scanner_next_token(Scanner* s);
```
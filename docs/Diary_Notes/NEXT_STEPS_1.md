# DragonTongue - Next Steps

## Immediate (Tomorrow)
1. Create `src/lexer/source.h` and `source.c`
   - Source reader that loads files
   - Track position, line, column
   - Peek/advance character by character

2. Create `tests/test_source.c`
   - Test file loading
   - Test line/column tracking
   - Test error for missing files

## Code to write (approx 100 lines):
- source_new(filename) - open and read file
- source_from_string(content) - for testing
- source_peek() - look at next char
- source_advance() - consume and return char
- source_skip_whitespace() - ignore spaces, tabs, newlines

## Commands to run:
```bash
touch src/lexer/source.h src/lexer/source.c
touch tests/test_source.c
```

## End of week goal:
Complete lexer that can tokenize a simple program:

```
let name = "DragonTongue"
print(name)
```
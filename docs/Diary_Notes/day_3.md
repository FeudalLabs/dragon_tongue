# DragonTongue Development Diary - Day 3 (Final)

Focus: Scanner/Lexer implementation - Complete tokenization system  
Status: ✅ COMPLETE - All tests passing  

## What Was Accomplished Today

Today we built the Scanner (Lexer) - the component that converts raw source code into meaningful tokens.  
This is the first stage of compilation where DragonTongue starts to "understand" code.

## Key Achievements

| Achievement              | Description                                                   |
|-------------------------|---------------------------------------------------------------|
| Complete token system   | 50+ token types covering all DragonTongue syntax              |
| Comment handling        | // line comments are properly skipped                         |
| String literals         | Quoted strings with proper memory management                  |
| Number parsing          | Integer literals converted to values                          |
| Identifier recognition  | Variable and function names                                  |
| Keyword detection       | 24 keywords (let, fn, struct, if, parallel, spawn, await...) |
| Operator detection      | 20+ operators including &&, ||, ->, .                        |
| Error reporting         | Line/column numbers for syntax errors                         |
| Peek functionality      | Look ahead at next token without consuming                    |


## Code Written Today

| File                      Purpose                    |
|------------------------------------------------------|
| src/lexer/token.h         Token type definitions     |
| src/lexer/token.c         Token helper functions     |
| src/lexer/scanner.h       Scanner interface          |
| src/lexer/scanner.c       Scanner implementation     |
| tests/test_scanner.c      Scanner test suite         |


## Token Types Implemented

Single-character tokens
```
( ) { } [ ] , ; : . + - * / =
```
## Two-character operators
```
== != <= >= && || -> += -=
```
## Keywords (24 total)
```
let var fn struct if else for while
return true false spawn await parallel
match catch type int bool string void
Option Some None
```
## Literals
```
IDENTIFIER   - variable/function names
NUMBER       - integer values
STRING_LITERAL - quoted strings
```
# Key Technical Decisions

## 1. Comment Skipping
```
// Skip // comments
if (source_peek(s->source) == '/' && source_peek_ahead(s->source, 1) == '/') {
    while (!source_is_eof(s->source) && source_peek(s->source) != '\n') {
        source_advance(s->source);
    }
    return scanner_next_token(s);  // Recursively get next token
}
```

## 2. String Literal Handling
```
// Strings are copied to arena memory
token.string_value = arena_alloc(s->arena, str_len + 1);
strncpy(token.string_value, str_start, str_len);
token.string_value[str_len] = '\0';
```

## 3. Keyword Recognition
```
// Read identifier first, then check against keyword table
token.type = TOKEN_IDENTIFIER;
for (int i = 0; keywords[i].word != NULL; i++) {
    if (strcmp(word, keywords[i].word) == 0) {
        token.type = keywords[i].type;
        break;
    }
}
```

## 4. Operator Lookahead
```
case '&':
    if (source_peek(s->source) == '&') {
        token.type = TOKEN_AND_AND;  // &&
        source_advance(s->source);
    } else {
        add_error(s, ...);
    }
    break;
```

## Test Results
```
=== Testing Scanner ===

✓ test_scanner_basic_tokens passed     ( ( ) { } [ ] , ; : )
✓ test_scanner_operators passed        ( + - * / = == != < > <= >= )
✓ test_scanner_keywords passed         ( let var fn if else for while return )
✓ test_scanner_identifiers passed      ( foo bar123 _camelCase )
✓ test_scanner_numbers passed          ( 42 0 12345 )
✓ test_scanner_strings passed          ( "hello" "world" )
✓ test_scanner_unterminated_string passed ( error detection )
✓ test_scanner_peek passed             ( lookahead without consuming )

✓ All scanner tests passed! (8 tests, 35+ assertions)
```

## Example Output

### Input: `examples/functions.dt`

```
struct Person {
    name: String
    age: Int
}

fn greet(person: Person) -> String {
    return "Hello, " + person.name
}
```

### Scanner Output:
```
Tokens:
  STRUCT
  IDENTIFIER(Person)
  LBRACE
  IDENTIFIER(name)
  COLON
  IDENTIFIER(String)
  IDENTIFIER(age)
  COLON
  IDENTIFIER(Int)
  RBRACE
  FN
  IDENTIFIER(greet)
  LPAREN
  IDENTIFIER(person)
  COLON
  IDENTIFIER(Person)
  RPAREN
  ARROW
  IDENTIFIER(String)
  LBRACE
  RETURN
  STRING_LITERAL("Hello, ")
  PLUS
  IDENTIFIER(person)
  DOT
  IDENTIFIER(name)
  RBRACE
  EOF
```

## Challenges Faced & Solved

### Challenge 1: Comment tokens appearing in output
Problem: // was being tokenized as two SLASH tokens  
Solution: Added comment detection BEFORE normal tokenization, skipping entire line  

### Challenge 2: String missing first character
Problem: "hello" became "ello" (off-by-one error)  
Solution: Adjusted pointer to account for already-consumed opening quote  

### Challenge 3: Keywords showing as UNKNOWN
Problem: struct, parallel, spawn, await appeared as UNKNOWN  
Solution: Added missing token names to token.c string mapping  

### Challenge 4: va_list compilation error
Problem: Missing #include <stdarg.h> in scanner.c  
Solution: Added required headers: stdarg.h, stdio.h  

### Challenge 5: Object files not linking
Problem: Makefile wasn't compiling all source files  
Solution: Updated Makefile to compile all .c files to .o before linking  

## Files Modified During Debugging

| File                    | Changes                                                       |
|-------------------------|---------------------------------------------------------------|
| src/lexer/scanner.c     | Added headers, fixed string reading, added comment skipping   |
| src/lexer/token.c       | Added complete token name mappings                            |
| src/lexer/token.h       | Added DOT, AND_AND, OR_OR, ARROW tokens                       |
| Makefile                | Fixed to compile all source files                             |
| tests/test_scanner.c    | Added string.h include                                        |

## Current Scanner Capabilities

### What DragonTongue Can Now Tokenize

```
// Variables
let name = "DragonTongue"
var count = 42

// Structs
struct User {
    id: Int
    name: String
}

// Functions
fn greet(user: User) -> String {
    return "Hello, " + user.name
}

// Control flow
if x == y {
    println("equal")
} else {
    println("not equal")
}

// Logical operators
if (a < b) && (c > d) {
    println("both true")
}

// Concurrency
parallel {
    user = fetch_user(1)
    posts = fetch_posts(1)
}

let handle = spawn background_task()
let result = await handle

// Pattern matching
match value {
    1 -> println("one")
    _ -> println("other")
}
```

## What's NOT Yet Implemented (Future)

| Feature                              | Planned For |
|--------------------------------------|-------------|
| Float literals (3.14)                | v1.1        |
| Hex/Octal numbers (0xFF, 0o755)      | v1.1        |
| Escape sequences in strings (\n, \t) | v1.1        |
| Character literals ('a')             | v1.1        |
| Multi-line comments (/* */)          | v1.2        |
| Raw strings (r"...")                 | v1.2        |

## Test Commands for Verification
```
# Run all scanner tests
make test

# Test specific features
./dragontongue examples/functions.dt | grep -E "(STRUCT|DOT)"
./dragontongue examples/concurrency.dt | grep -E "(PARALLEL|SPAWN|AWAIT)"
./dragontongue examples/all_features.dt

# Interactive REPL (if built)
./tools/repl
```

## Statistics

| Metric               | Value     |
|----------------------|-----------|
| Total token types    | 50+       |
| Keywords supported   | 24        |
| Operators supported  | 20+       |
| Test functions       | 8         |
| Test assertions      | 35+       |
| Scanner code size    | 420 lines |
| Memory arena size    | 1MB       |

## Lessons Learned

- Headers matter - Always include all required system headers  
- Test incrementally - Write tests for each feature as you build  
- Makefile simplicity - Keep build system clear and explicit  
- Pointer math is subtle - Off-by-one errors are common with string handling  
- Keyword tables work - Simple array lookup is efficient for small keyword sets  
- Error messages save time - Good error reporting helps debugging immensely  
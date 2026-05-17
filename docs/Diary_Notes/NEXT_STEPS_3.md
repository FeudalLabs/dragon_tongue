## Next Steps (Day 4)

### The Parser (Abstract Syntax Tree)

### Goals:

- Design AST node structures  
- Implement recursive descent parser  
- Parse expressions with precedence  
- Parse statements (let, if, while, return, etc.)  
- Parse functions and structs  
- Build complete AST from source  

## Preview: Parser API
```
typedef struct Parser {
    Scanner* scanner;
    Token current;
    Token previous;
    Arena* arena;
    ErrorList* errors;
} Parser;

ASTNode* parser_parse_program(Parser* p);
ASTNode* parse_expression(Parser* p, int precedence);
ASTNode* parse_statement(Parser* p);
```

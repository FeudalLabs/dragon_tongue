#ifndef SCANNER_H
#define SCANNER_H

#include "source.h"
#include "token.h"

// Error structure for lexer errors
typedef struct LexerError {
    int line;
    int column;
    char* message;
    struct LexerError* next;
} LexerError;

typedef struct Scanner {
    Source* source;
    Token current;
    Token previous;
    LexerError* errors;
    Arena* arena;
} Scanner;

// Create new scanner
Scanner* scanner_new(Source* source, Arena* arena);

// Get next token from source
Token scanner_next_token(Scanner* s);

// Peek at next token without consuming
Token scanner_peek_token(Scanner* s);

// Check if there are errors
bool scanner_has_errors(Scanner* s);

// Get error list
LexerError* scanner_get_errors(Scanner* s);

// Print all errors
void scanner_print_errors(Scanner* s);

#endif
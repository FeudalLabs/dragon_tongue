#ifndef PARSER_H
#define PARSER_H

#include "../lexer/scanner.h"
#include "ast.h"

typedef struct {
    Scanner* scanner;
    Token current;
    Token previous;
    Arena* arena;
    bool had_error;
} Parser;

// Create new parser
Parser* parser_new(Scanner* scanner, Arena* arena);

// Parse entire program
ASTNode* parser_parse_program(Parser* p);

// Check if parser had errors
bool parser_had_error(Parser* p);

// Print errors
void parser_print_errors(Parser* p);

#endif
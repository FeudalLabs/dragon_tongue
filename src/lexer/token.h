#ifndef TOKEN_H
#define TOKEN_H

#include "../dragontongue.h"

typedef enum {
    // Single-character tokens
    TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_LBRACE, TOKEN_RBRACE,
    TOKEN_LBRACKET, TOKEN_RBRACKET,
    TOKEN_COMMA, TOKEN_SEMICOLON, TOKEN_COLON,
    TOKEN_DOT,           // .
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH,
    TOKEN_ASSIGN,
    
    // One or two character tokens
    TOKEN_EQ, TOKEN_NE, TOKEN_LT, TOKEN_GT, TOKEN_LE, TOKEN_GE,
    TOKEN_PLUS_ASSIGN, TOKEN_MINUS_ASSIGN,
    TOKEN_AND_AND,      // &&
    TOKEN_OR_OR,        // ||
    TOKEN_ARROW,        // ->
    
    // Keywords
    TOKEN_LET, TOKEN_VAR, TOKEN_FN, TOKEN_STRUCT,
    TOKEN_IF, TOKEN_ELSE, TOKEN_FOR, TOKEN_WHILE,
    TOKEN_SPAWN, TOKEN_AWAIT, TOKEN_RETURN,
    TOKEN_TRUE, TOKEN_FALSE, TOKEN_PARALLEL,
    TOKEN_MATCH, TOKEN_CATCH, TOKEN_ELSE_ERROR,
    TOKEN_TYPE,
    
    // Types
    TOKEN_INT, TOKEN_BOOL, TOKEN_STRING, TOKEN_VOID,
    TOKEN_OPTION, TOKEN_SOME, TOKEN_NONE,
    
    // Values
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING_LITERAL,
    
    TOKEN_ERROR,
    TOKEN_EOF
} TokenType;

typedef struct Token {
    TokenType type;
    const char* start;
    int length;
    int line;
    int column;
    union {
        long long int_value;
        double float_value;
        char* string_value;
    };
} Token;

// Helper functions
const char* token_type_to_string(TokenType type);
const char* token_to_string(Token* token);
bool token_is_keyword(TokenType type);
bool token_is_operator(TokenType type);
bool token_is_literal(TokenType type);

#endif
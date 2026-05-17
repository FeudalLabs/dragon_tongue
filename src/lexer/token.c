#include "token.h"
#include <stdio.h>
#include <string.h>

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COLON: return "COLON";
        case TOKEN_DOT: return "DOT";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_STAR: return "STAR";
        case TOKEN_SLASH: return "SLASH";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_PLUS_ASSIGN: return "PLUS_ASSIGN";
        case TOKEN_MINUS_ASSIGN: return "MINUS_ASSIGN";
        case TOKEN_EQ: return "EQ";
        case TOKEN_NE: return "NE";
        case TOKEN_LT: return "LT";
        case TOKEN_GT: return "GT";
        case TOKEN_LE: return "LE";
        case TOKEN_GE: return "GE";
        case TOKEN_AND_AND: return "AND_AND";
        case TOKEN_OR_OR: return "OR_OR";
        case TOKEN_ARROW: return "ARROW";
        case TOKEN_LET: return "LET";
        case TOKEN_VAR: return "VAR";
        case TOKEN_FN: return "FN";
        case TOKEN_STRUCT: return "STRUCT";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_SPAWN: return "SPAWN";
        case TOKEN_AWAIT: return "AWAIT";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";
        case TOKEN_PARALLEL: return "PARALLEL";
        case TOKEN_MATCH: return "MATCH";
        case TOKEN_CATCH: return "CATCH";
        case TOKEN_ELSE_ERROR: return "ELSE_ERROR";
        case TOKEN_TYPE: return "TYPE";
        case TOKEN_INT: return "INT";
        case TOKEN_BOOL: return "BOOL";
        case TOKEN_STRING: return "STRING";
        case TOKEN_VOID: return "VOID";
        case TOKEN_OPTION: return "OPTION";
        case TOKEN_SOME: return "SOME";
        case TOKEN_NONE: return "NONE";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING_LITERAL: return "STRING_LITERAL";
        case TOKEN_ERROR: return "ERROR";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

const char* token_to_string(Token* token) {
    static char buffer[256];
    if (token->type == TOKEN_IDENTIFIER) {
        snprintf(buffer, sizeof(buffer), "%.*s", token->length, token->start);
    } else if (token->type == TOKEN_NUMBER) {
        snprintf(buffer, sizeof(buffer), "%lld", token->int_value);
    } else if (token->type == TOKEN_STRING_LITERAL) {
        snprintf(buffer, sizeof(buffer), "\"%s\"", token->string_value ? token->string_value : "");
    } else {
        snprintf(buffer, sizeof(buffer), "%s", token_type_to_string(token->type));
    }
    return buffer;
}

bool token_is_keyword(TokenType type) {
    return (type >= TOKEN_LET && type <= TOKEN_NONE) &&
           !(type == TOKEN_IDENTIFIER || type == TOKEN_NUMBER || 
             type == TOKEN_STRING_LITERAL || type == TOKEN_ERROR || type == TOKEN_EOF);
}

bool token_is_operator(TokenType type) {
    return (type >= TOKEN_PLUS && type <= TOKEN_SLASH) ||
           (type >= TOKEN_EQ && type <= TOKEN_GE) ||
           (type >= TOKEN_ASSIGN && type <= TOKEN_MINUS_ASSIGN) ||
           type == TOKEN_AND_AND || type == TOKEN_OR_OR || type == TOKEN_ARROW;
}

bool token_is_literal(TokenType type) {
    return type == TOKEN_NUMBER || 
           type == TOKEN_STRING_LITERAL ||
           type == TOKEN_TRUE ||
           type == TOKEN_FALSE;
}
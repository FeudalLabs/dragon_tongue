#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* my_strndup(const char* s, size_t n) {
    if (!s) return NULL;
    size_t len = strlen(s);
    if (len > n) len = n;
    char* new_str = malloc(len + 1);
    if (new_str) {
        memcpy(new_str, s, len);
        new_str[len] = '\0';
    }
    return new_str;
}

static char* my_strdup(const char* s) {
    if (!s) return NULL;
    return my_strndup(s, strlen(s));
}

static ASTNode* parse_statement(Parser* p);
static ASTNode* parse_expression(Parser* p);
static ASTNode* parse_equality(Parser* p);
static ASTNode* parse_comparison(Parser* p);
static ASTNode* parse_term(Parser* p);
static ASTNode* parse_factor(Parser* p);
static ASTNode* parse_primary(Parser* p);
static ASTNode* parse_block(Parser* p);

static void advance(Parser* p) {
    p->previous = p->current;
    p->current = scanner_next_token(p->scanner);
}

static bool check(Parser* p, TokenType type) {
    return p->current.type == type;
}

static bool consume(Parser* p, TokenType type, const char* message) {
    if (check(p, type)) {
        advance(p);
        return true;
    }
    fprintf(stderr, "Error at %d:%d: %s\n", p->current.line, p->current.column, message);
    p->had_error = true;
    return false;
}

static bool match(Parser* p, TokenType type) {
    if (check(p, type)) {
        advance(p);
        return true;
    }
    return false;
}

// Equality: == !=
static ASTNode* parse_equality(Parser* p) {
    ASTNode* node = parse_comparison(p);
    
    while (match(p, TOKEN_EQ) || match(p, TOKEN_NE)) {
        TokenType op = p->previous.type;
        ASTNode* right = parse_comparison(p);
        const char* op_str = (op == TOKEN_EQ) ? "==" : "!=";
        node = new_binary_op((char*)op_str, node, right, p->previous.line, p->previous.column);
    }
    return node;
}

// Comparison: > < >= <=
static ASTNode* parse_comparison(Parser* p) {
    ASTNode* node = parse_term(p);
    
    while (match(p, TOKEN_GT) || match(p, TOKEN_LT) || match(p, TOKEN_GE) || match(p, TOKEN_LE)) {
        TokenType op = p->previous.type;
        ASTNode* right = parse_term(p);
        const char* op_str = "";
        if (op == TOKEN_GT) op_str = ">";
        else if (op == TOKEN_LT) op_str = "<";
        else if (op == TOKEN_GE) op_str = ">=";
        else if (op == TOKEN_LE) op_str = "<=";
        node = new_binary_op((char*)op_str, node, right, p->previous.line, p->previous.column);
    }
    return node;
}

// Term: + -
static ASTNode* parse_term(Parser* p) {
    ASTNode* node = parse_factor(p);
    
    while (match(p, TOKEN_PLUS) || match(p, TOKEN_MINUS)) {
        TokenType op = p->previous.type;
        ASTNode* right = parse_factor(p);
        const char* op_str = (op == TOKEN_PLUS) ? "+" : "-";
        node = new_binary_op((char*)op_str, node, right, p->previous.line, p->previous.column);
    }
    return node;
}

// Factor: * /
static ASTNode* parse_factor(Parser* p) {
    ASTNode* node = parse_primary(p);
    
    while (match(p, TOKEN_STAR) || match(p, TOKEN_SLASH)) {
        TokenType op = p->previous.type;
        ASTNode* right = parse_primary(p);
        const char* op_str = (op == TOKEN_STAR) ? "*" : "/";
        node = new_binary_op((char*)op_str, node, right, p->previous.line, p->previous.column);
    }
    return node;
}

// Parse return statement
static ASTNode* parse_return_statement(Parser* p) {
    int line = p->current.line;
    int col = p->current.column;
    advance(p);
    ASTNode* value = NULL;
    if (!check(p, TOKEN_RBRACE) && !check(p, TOKEN_EOF)) {
        value = parse_expression(p);
    }
    return new_return_stmt(value, line, col);
}

// Parse variable declaration
static ASTNode* parse_let_statement(Parser* p) {
    int line = p->current.line;
    int col = p->current.column;
    advance(p);
    
    if (!consume(p, TOKEN_IDENTIFIER, "Expected variable name")) {
        return NULL;
    }
    
    char* var_name = my_strndup(p->previous.start, p->previous.length);
    
    ASTNode* type_ann = NULL;
    if (match(p, TOKEN_COLON)) {
        if (check(p, TOKEN_IDENTIFIER)) {
            char* type_name = my_strndup(p->current.start, p->current.length);
            type_ann = new_variable(type_name, p->current.line, p->current.column);
            free(type_name);
            advance(p);
        }
    }
    
    if (!consume(p, TOKEN_ASSIGN, "Expected '=' after variable name")) {
        free(var_name);
        return NULL;
    }
    
    ASTNode* value = parse_expression(p);
    if (!value) {
        free(var_name);
        return NULL;
    }
    
    ASTNode* result = new_let_stmt(var_name, value, type_ann, line, col);
    free(var_name);
    return result;
}

// Parse block statement
static ASTNode* parse_block(Parser* p) {
    int line = p->current.line;
    int col = p->current.column;
    
    if (!consume(p, TOKEN_LBRACE, "Expected '{'")) {
        return NULL;
    }
    
    ASTNode* statements = NULL;
    ASTNode* last = NULL;
    
    while (!check(p, TOKEN_RBRACE) && !check(p, TOKEN_EOF)) {
        ASTNode* stmt = parse_statement(p);
        if (stmt) {
            if (!statements) {
                statements = stmt;
                last = stmt;
            } else {
                last->next = stmt;
                last = stmt;
            }
        }
    }
    
    consume(p, TOKEN_RBRACE, "Expected '}'");
    return new_block_stmt(statements, line, col);
}

// Parse if statement
static ASTNode* parse_if_statement(Parser* p) {
    int line = p->current.line;
    int col = p->current.column;
    advance(p);
    
    ASTNode* condition = parse_expression(p);
    if (!condition) return NULL;
    
    ASTNode* then_branch = parse_block(p);
    if (!then_branch) return NULL;
    
    ASTNode* else_branch = NULL;
    if (match(p, TOKEN_ELSE)) {
        if (check(p, TOKEN_IF)) {
            else_branch = parse_if_statement(p);
        } else {
            else_branch = parse_block(p);
        }
    }
    
    return new_if_stmt(condition, then_branch, else_branch, line, col);
}

// Parse function
static ASTNode* parse_function(Parser* p) {
    int line = p->current.line;
    int col = p->current.column;
    advance(p);
    
    if (!consume(p, TOKEN_IDENTIFIER, "Expected function name")) {
        return NULL;
    }
    
    char* func_name = my_strndup(p->previous.start, p->previous.length);
    
    if (!consume(p, TOKEN_LPAREN, "Expected '(' after function name")) {
        free(func_name);
        return NULL;
    }
    
    ASTNode* params = NULL;
    ASTNode* last_param = NULL;
    
    if (!check(p, TOKEN_RPAREN)) {
        if (consume(p, TOKEN_IDENTIFIER, "Expected parameter name")) {
            char* param_name = my_strndup(p->previous.start, p->previous.length);
            ASTNode* param = new_variable(param_name, p->previous.line, p->previous.column);
            free(param_name);
            params = param;
            last_param = param;
        }
        
        while (match(p, TOKEN_COMMA)) {
            if (consume(p, TOKEN_IDENTIFIER, "Expected parameter name")) {
                char* param_name = my_strndup(p->previous.start, p->previous.length);
                ASTNode* param = new_variable(param_name, p->previous.line, p->previous.column);
                free(param_name);
                if (last_param) {
                    last_param->next = param;
                    last_param = param;
                }
            }
        }
    }
    
    consume(p, TOKEN_RPAREN, "Expected ')' after parameters");
    
    ASTNode* return_type = NULL;
    if (match(p, TOKEN_ARROW)) {
        if (consume(p, TOKEN_IDENTIFIER, "Expected return type")) {
            char* type_name = my_strndup(p->previous.start, p->previous.length);
            return_type = new_variable(type_name, p->previous.line, p->previous.column);
            free(type_name);
        }
    }
    
    ASTNode* body = parse_block(p);
    if (!body) {
        free(func_name);
        return NULL;
    }
    
    ASTNode* result = new_function(func_name, params, return_type, body, line, col);
    free(func_name);
    return result;
}

// Parse expression - entry point
static ASTNode* parse_expression(Parser* p) {
    return parse_equality(p);
}

// Parse primary expressions
static ASTNode* parse_primary(Parser* p) {
    ASTNode* node = NULL;
    
    switch (p->current.type) {
        case TOKEN_NUMBER:
            node = new_literal_int(p->current.int_value, p->current.line, p->current.column);
            advance(p);
            break;
            
        case TOKEN_STRING_LITERAL:
            node = new_literal_string(p->current.string_value, p->current.line, p->current.column);
            advance(p);
            break;
            
        case TOKEN_IDENTIFIER: {
            char* name = my_strndup(p->current.start, p->current.length);
            int line = p->current.line;
            int col = p->current.column;
            
            bool is_builtin = (strcmp(name, "println") == 0 || 
                               strcmp(name, "print") == 0 ||
                               strcmp(name, "len") == 0 ||
                               strcmp(name, "to_string") == 0 ||
                               strcmp(name, "to_int") == 0 ||
                               strcmp(name, "read_file") == 0 ||
                               strcmp(name, "write_file") == 0);
            
            advance(p);
            
            // Check if this is a function call
            if (match(p, TOKEN_LPAREN)) {
                ASTNode* args = NULL;
                ASTNode* last_arg = NULL;
                
                if (!check(p, TOKEN_RPAREN)) {
                    do {
                        ASTNode* arg = parse_expression(p);
                        if (arg) {
                            if (!args) {
                                args = arg;
                                last_arg = arg;
                            } else {
                                last_arg->next = arg;
                                last_arg = arg;
                            }
                        }
                    } while (match(p, TOKEN_COMMA));
                }
                
                consume(p, TOKEN_RPAREN, "Expected ')' after arguments");
                
                if (is_builtin) {
                    node = new_builtin_func(name, args, line, col);
                } else {
                    // Create a CALL node for user function calls
                    Call* call = (Call*)malloc(sizeof(Call));
                    call->base.type = NODE_CALL;
                    call->base.line = line;
                    call->base.column = col;
                    call->base.next = NULL;
                    call->name = my_strdup(name);
                    call->arguments = args;
                    node = (ASTNode*)call;
                }
                free(name);
                break;
            }
            
            // Regular variable reference
            node = new_variable(name, line, col);
            free(name);
            break;
        }
        
        case TOKEN_LPAREN:
            advance(p);
            node = parse_expression(p);
            consume(p, TOKEN_RPAREN, "Expected ')' after expression");
            break;
            
        default:
            fprintf(stderr, "Error at %d:%d: Unexpected token %s\n",
                    p->current.line, p->current.column,
                    token_type_to_string(p->current.type));
            p->had_error = true;
            return NULL;
    }
    
    return node;
}

// Parse a single statement
static ASTNode* parse_statement(Parser* p) {
    switch (p->current.type) {
        case TOKEN_LET:    return parse_let_statement(p);
        case TOKEN_FN:     return parse_function(p);
        case TOKEN_IF:     return parse_if_statement(p);
        case TOKEN_RETURN: return parse_return_statement(p);
        case TOKEN_LBRACE: return parse_block(p);
        default:           return parse_expression(p);
    }
}

// Parse entire program
ASTNode* parser_parse_program(Parser* p) {
    advance(p);
    
    ASTNode* program = NULL;
    ASTNode* last = NULL;
    
    while (!check(p, TOKEN_EOF)) {
        ASTNode* stmt = parse_statement(p);
        if (stmt) {
            if (!program) {
                program = stmt;
                last = stmt;
            } else {
                last->next = stmt;
                last = stmt;
            }
        }
        if (p->had_error) {
            while (!check(p, TOKEN_EOF) && !check(p, TOKEN_LET) && 
                   !check(p, TOKEN_FN) && !check(p, TOKEN_IF) && 
                   !check(p, TOKEN_RETURN) && !check(p, TOKEN_LBRACE)) {
                advance(p);
            }
            p->had_error = false;
        }
    }
    
    return new_program(program, 1, 1);
}

Parser* parser_new(Scanner* scanner, Arena* arena) {
    Parser* parser = arena_alloc(arena, sizeof(Parser));
    parser->scanner = scanner;
    parser->arena = arena;
    parser->had_error = false;
    return parser;
}

bool parser_had_error(Parser* p) {
    return p->had_error;
}

void parser_print_errors(Parser* p) {
    (void)p;
}
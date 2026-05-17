#include "scanner.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// Keyword mapping structure
typedef struct {
    const char* word;
    TokenType type;
} Keyword;

static Keyword keywords[] = {
    {"let", TOKEN_LET},
    {"var", TOKEN_VAR},
    {"fn", TOKEN_FN},
    {"struct", TOKEN_STRUCT},
    {"if", TOKEN_IF},
    {"else", TOKEN_ELSE},
    {"for", TOKEN_FOR},
    {"while", TOKEN_WHILE},
    {"spawn", TOKEN_SPAWN},
    {"await", TOKEN_AWAIT},
    {"return", TOKEN_RETURN},
    {"true", TOKEN_TRUE},
    {"false", TOKEN_FALSE},
    {"parallel", TOKEN_PARALLEL},
    {"match", TOKEN_MATCH},
    {"catch", TOKEN_CATCH},
    {"type", TOKEN_TYPE},
    {"int", TOKEN_INT},
    {"bool", TOKEN_BOOL},
    {"string", TOKEN_STRING},
    {"void", TOKEN_VOID},
    {"Option", TOKEN_OPTION},
    {"Some", TOKEN_SOME},
    {"None", TOKEN_NONE},
    {NULL, TOKEN_EOF}
};

// Helper: add error to scanner
static void add_error(Scanner* s, int line, int column, const char* format, ...) {
    LexerError* err = arena_alloc(s->arena, sizeof(LexerError));
    err->line = line;
    err->column = column;
    err->next = s->errors;
    
    // Format error message
    va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    err->message = arena_alloc(s->arena, strlen(buffer) + 1);
    strcpy(err->message, buffer);
    
    s->errors = err;
}

// Helper: check if character is valid identifier start
static bool is_identifier_start(char c) {
    return isalpha(c) || c == '_';
}

// Helper: check if character is valid identifier part
static bool is_identifier_part(char c) {
    return isalnum(c) || c == '_';
}

// Helper: read identifier or keyword
static Token read_identifier(Scanner* s) {
    Token token;
    token.start = &s->source->content[s->source->position];
    token.line = s->source->line;
    token.column = s->source->column;
    token.string_value = NULL;
    token.int_value = 0;
    
    // Read whole identifier
    while (!source_is_eof(s->source) && is_identifier_part(source_peek(s->source))) {
        source_advance(s->source);
    }
    
    token.length = &s->source->content[s->source->position] - token.start;
    
    // Check if it's a keyword
    char word[token.length + 1];
    strncpy(word, token.start, token.length);
    word[token.length] = '\0';
    
    token.type = TOKEN_IDENTIFIER;
    for (int i = 0; keywords[i].word != NULL; i++) {
        if (strcmp(word, keywords[i].word) == 0) {
            token.type = keywords[i].type;
            break;
        }
    }
    
    return token;
}

// Helper: read number (integer only for v1)
static Token read_number(Scanner* s) {
    Token token;
    token.start = &s->source->content[s->source->position];
    token.line = s->source->line;
    token.column = s->source->column;
    token.type = TOKEN_NUMBER;
    token.string_value = NULL;
    
    // Read digits
    while (!source_is_eof(s->source) && isdigit(source_peek(s->source))) {
        source_advance(s->source);
    }
    
    token.length = &s->source->content[s->source->position] - token.start;
    
    // Convert to integer
    char num_str[token.length + 1];
    strncpy(num_str, token.start, token.length);
    num_str[token.length] = '\0';
    token.int_value = atoll(num_str);
    
    return token;
}

// Helper: read string literal
static Token read_string(Scanner* s) {
    Token token;
    token.start = &s->source->content[s->source->position - 1];  // Point to opening quote
    token.line = s->source->line;
    token.column = s->source->column - 1;  // Column before consuming quote
    token.type = TOKEN_STRING_LITERAL;
    token.int_value = 0;
    
    // The opening quote has already been consumed by scanner_next_token
    // So s->source->position is now at the first character AFTER the quote
    
    // Save the start position of the string content
    int content_start = s->source->position;
    
    // Read until closing quote
    int str_len = 0;
    while (!source_is_eof(s->source) && source_peek(s->source) != '"') {
        source_advance(s->source);
        str_len++;
    }
    
    // Check for unterminated string
    if (source_is_eof(s->source)) {
        add_error(s, token.line, token.column, "Unterminated string literal");
        token.type = TOKEN_ERROR;
        return token;
    }
    
    // Extract string content from source
    char* str_start = &s->source->content[content_start];
    
    // Copy string value (allocate from arena)
    token.string_value = arena_alloc(s->arena, str_len + 1);
    strncpy(token.string_value, str_start, str_len);
    token.string_value[str_len] = '\0';
    
    // Set token length (including both quotes)
    token.length = str_len + 2;
    
    // Consume closing quote
    source_advance(s->source);
    
    return token;
}

// Main tokenization function
Token scanner_next_token(Scanner* s) {
    // Skip whitespace
    source_skip_whitespace(s->source);
    
    // Skip // comments
    if (source_peek(s->source) == '/' && source_peek_ahead(s->source, 1) == '/') {
        // Skip until end of line
        while (!source_is_eof(s->source) && source_peek(s->source) != '\n') {
            source_advance(s->source);
        }
        // Skip the newline as well
        if (source_peek(s->source) == '\n') {
            source_advance(s->source);
        }
        // Restart tokenization
        return scanner_next_token(s);
    }
    
    // Record token position
    Token token;
    token.start = &s->source->content[s->source->position];
    token.line = s->source->line;
    token.column = s->source->column;
    token.length = 1;
    token.string_value = NULL;
    token.int_value = 0;
    
    // Check for EOF
    if (source_is_eof(s->source)) {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char c = source_advance(s->source);
    
    // Single character tokens
    switch (c) {
        case '(': token.type = TOKEN_LPAREN; break;
        case ')': token.type = TOKEN_RPAREN; break;
        case '{': token.type = TOKEN_LBRACE; break;
        case '}': token.type = TOKEN_RBRACE; break;
        case '[': token.type = TOKEN_LBRACKET; break;
        case ']': token.type = TOKEN_RBRACKET; break;
        case ',': token.type = TOKEN_COMMA; break;
        case ';': token.type = TOKEN_SEMICOLON; break;
        case ':': token.type = TOKEN_COLON; break;
        case '.': token.type = TOKEN_DOT; break;
        case '+': 
            if (source_peek(s->source) == '=') {
                token.type = TOKEN_PLUS_ASSIGN;
                token.length = 2;
                source_advance(s->source);
            } else {
                token.type = TOKEN_PLUS;
            }
            break;
        case '-':
            if (source_peek(s->source) == '=') {
                token.type = TOKEN_MINUS_ASSIGN;
                token.length = 2;
                source_advance(s->source);
            } else if (source_peek(s->source) == '>') {
                token.type = TOKEN_ARROW;
                token.length = 2;
                source_advance(s->source);
            } else {
                token.type = TOKEN_MINUS;
            }
            break;
        case '*': token.type = TOKEN_STAR; break;
        case '/': token.type = TOKEN_SLASH; break;
        case '=':
            if (source_peek(s->source) == '=') {
                token.type = TOKEN_EQ;
                token.length = 2;
                source_advance(s->source);
            } else {
                token.type = TOKEN_ASSIGN;
            }
            break;
        case '!':
            if (source_peek(s->source) == '=') {
                token.type = TOKEN_NE;
                token.length = 2;
                source_advance(s->source);
            } else {
                add_error(s, token.line, token.column, "Unexpected '!' character");
                token.type = TOKEN_ERROR;
            }
            break;
        case '<':
            if (source_peek(s->source) == '=') {
                token.type = TOKEN_LE;
                token.length = 2;
                source_advance(s->source);
            } else {
                token.type = TOKEN_LT;
            }
            break;
        case '>':
            if (source_peek(s->source) == '=') {
                token.type = TOKEN_GE;
                token.length = 2;
                source_advance(s->source);
            } else {
                token.type = TOKEN_GT;
            }
            break;
        case '&':
            if (source_peek(s->source) == '&') {
                token.type = TOKEN_AND_AND;
                token.length = 2;
                source_advance(s->source);
            } else {
                add_error(s, token.line, token.column, "Unexpected '&' character (did you mean '&&'?)");
                token.type = TOKEN_ERROR;
            }
            break;
        case '|':
            if (source_peek(s->source) == '|') {
                token.type = TOKEN_OR_OR;
                token.length = 2;
                source_advance(s->source);
            } else {
                add_error(s, token.line, token.column, "Unexpected '|' character (did you mean '||'?)");
                token.type = TOKEN_ERROR;
            }
            break;
        case '"':
            return read_string(s);
        default:
            if (isdigit(c)) {
                // Put back the character and read number properly
                s->source->position--;
                s->source->column--;
                return read_number(s);
            } else if (is_identifier_start(c)) {
                // Put back the character and read identifier
                s->source->position--;
                s->source->column--;
                return read_identifier(s);
            } else {
                add_error(s, token.line, token.column, "Unexpected character: '%c' (ASCII %d)", c, c);
                token.type = TOKEN_ERROR;
            }
            break;
    }
    
    s->previous = s->current;
    s->current = token;
    return token;
}

// Peek at next token without consuming
Token scanner_peek_token(Scanner* s) {
    // Save position
    int saved_pos = s->source->position;
    int saved_line = s->source->line;
    int saved_col = s->source->column;
    
    Token next = scanner_next_token(s);
    
    // Restore position
    s->source->position = saved_pos;
    s->source->line = saved_line;
    s->source->column = saved_col;
    
    return next;
}

Scanner* scanner_new(Source* source, Arena* arena) {
    Scanner* scanner = arena_alloc(arena, sizeof(Scanner));
    scanner->source = source;
    scanner->errors = NULL;
    scanner->arena = arena;
    scanner->current.type = TOKEN_EOF;
    scanner->previous.type = TOKEN_EOF;
    return scanner;
}

bool scanner_has_errors(Scanner* s) {
    return s->errors != NULL;
}

LexerError* scanner_get_errors(Scanner* s) {
    return s->errors;
}

void scanner_print_errors(Scanner* s) {
    LexerError* err = s->errors;
    while (err) {
        fprintf(stderr, "Lexer error at %d:%d: %s\n", err->line, err->column, err->message);
        err = err->next;
    }
}
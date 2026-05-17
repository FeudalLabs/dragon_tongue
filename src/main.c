#include "dragontongue.h"
#include "lexer/source.h"
#include "lexer/scanner.h"

int main(int argc, char** argv) {
    printf("DragonTongue Compiler v%s\n", DRAGONTONGUE_VERSION);
    
    if (argc < 2) {
        fprintf(stderr, "Usage: dragontongue <source.dt>\n");
        return 1;
    }
    
    printf("Compiling: %s\n", argv[1]);
    
    Arena* arena = arena_new(1024 * 1024);  // 1MB arena
    
    Source* source = source_from_file(argv[1], arena);
    if (!source) {
        fprintf(stderr, "Failed to load source file\n");
        arena_free(arena);
        return 1;
    }
    
    Scanner* scanner = scanner_new(source, arena);
    
    printf("\nTokens:\n");
    Token token;
    do {
        token = scanner_next_token(scanner);
        printf("  %s", token_type_to_string(token.type));
        if (token.type == TOKEN_IDENTIFIER) {
            printf("(%.*s)", token.length, token.start);
        } else if (token.type == TOKEN_NUMBER) {
            printf("(%lld)", token.int_value);
        } else if (token.type == TOKEN_STRING_LITERAL) {
            printf("(\"%s\")", token.string_value);
        }
        printf("\n");
    } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);
    
    if (scanner_has_errors(scanner)) {
        printf("\nErrors:\n");
        scanner_print_errors(scanner);
    }
    
    arena_free(arena);
    return 0;
}
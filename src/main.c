#include "dragontongue.h"
#include "lexer/source.h"
#include "lexer/scanner.h"
#include "parser/parser.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: dragontongue <source.dt>\n");
        return 1;
    }
    
    Arena* arena = arena_new(1024 * 1024);
    
    Source* source = source_from_file(argv[1], arena);
    if (!source) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
        arena_free(arena);
        return 1;
    }
    
    Scanner* scanner = scanner_new(source, arena);
    Parser* parser = parser_new(scanner, arena);
    
    ASTNode* ast = parser_parse_program(parser);
    
    if (parser_had_error(parser)) {
        fprintf(stderr, "Parsing failed with errors.\n");
        arena_free(arena);
        return 1;
    }
    
    // Only print ONCE
    ast_print(ast, 0);
    
    arena_free(arena);
    return 0;
}
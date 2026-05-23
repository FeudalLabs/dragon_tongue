#include "dragontongue.h"
#include "lexer/source.h"
#include "lexer/scanner.h"
#include "parser/parser.h"
#include "semantic/analyzer.h"
#include "codegen/cgen.h"

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
    
    printf("Parsing...\n");
    ASTNode* ast = parser_parse_program(parser);
    
    if (parser_had_error(parser)) {
        fprintf(stderr, "Parsing failed with errors.\n");
        arena_free(arena);
        return 1;
    }
    
    printf("Semantic Analysis...\n");
    SemanticAnalyzer* analyzer = analyzer_new(arena);
    analyzer_analyze(analyzer, ast);
    
    if (analyzer_had_error(analyzer)) {
        fprintf(stderr, "Semantic analysis failed.\n");
        arena_free(arena);
        return 1;
    }
    
    printf("Generating C code...\n");
    CodeGenerator* cgen = cgen_new("output.c", analyzer->symbols, arena);
    cgen_generate(cgen, ast);
    
    if (cgen_had_error(cgen)) {
        fprintf(stderr, "Code generation failed.\n");
        cgen_close(cgen);
        arena_free(arena);
        return 1;
    }
    
    cgen_close(cgen);
    
    printf("Compiling C code...\n");
    int compile_result = system("gcc -o output runtime/runtime.c output.c -I./runtime 2>&1");
    
    if (compile_result != 0) {
        fprintf(stderr, "C compilation failed. See errors above.\n");
        arena_free(arena);
        return 1;
    }
    
    printf("\nRunning program:\n");
    system("./output");
    printf("\n");
    
    arena_free(arena);
    return 0;
}
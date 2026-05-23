#ifndef CGEN_H
#define CGEN_H

#include "../parser/ast.h"
#include "../semantic/symbol.h"

typedef struct {
    FILE* output;
    SymbolTable* symbols;
    Arena* arena;
    int indent_level;
    int temp_counter;
    char* current_function;
    bool had_error;
} CodeGenerator;

// Create new code generator
CodeGenerator* cgen_new(const char* filename, SymbolTable* symbols, Arena* arena);

// Generate code from AST
void cgen_generate(CodeGenerator* cg, ASTNode* program);

// Check for errors
bool cgen_had_error(CodeGenerator* cg);

// Close the generator
void cgen_close(CodeGenerator* cg);

#endif
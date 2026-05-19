#ifndef ANALYZER_H
#define ANALYZER_H

#include "../parser/ast.h"
#include "symbol.h"
#include "type.h"

typedef struct {
    SymbolTable* symbols;
    Arena* arena;
    bool had_error;
    int error_count;
} SemanticAnalyzer;

// Create semantic analyzer
SemanticAnalyzer* analyzer_new(Arena* arena);

// Analyze entire program
void analyzer_analyze(SemanticAnalyzer* sa, ASTNode* program);

// Check if errors occurred
bool analyzer_had_error(SemanticAnalyzer* sa);

// Print all errors
void analyzer_print_errors(SemanticAnalyzer* sa);

#endif
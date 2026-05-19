#ifndef SYMBOL_H
#define SYMBOL_H

#include "../dragontongue.h"
#include "../parser/ast.h"

typedef enum {
    SYM_VARIABLE,
    SYM_FUNCTION,
    SYM_PARAMETER,
    SYM_STRUCT,
    SYM_BUILTIN
} SymbolKind;

typedef struct Symbol {
    char* name;
    SymbolKind kind;
    ASTNode* node;          // Pointer to AST node (for location info)
    struct Symbol* next;    // For hash table chaining
    int scope_level;
    int line;
    int column;
} Symbol;

typedef struct SymbolTable {
    Symbol** entries;       // Hash table (1024 slots)
    int size;
    struct SymbolTable* parent;
    int scope_level;
    Arena* arena;
} SymbolTable;

// Create new symbol table
SymbolTable* symtab_new(Arena* arena);

// Scope management
void symtab_enter_scope(SymbolTable* st);
void symtab_exit_scope(SymbolTable* st);

// Symbol management
bool symtab_define(SymbolTable* st, const char* name, SymbolKind kind, 
                   ASTNode* node, int line, int col);
Symbol* symtab_lookup(SymbolTable* st, const char* name);
Symbol* symtab_lookup_current(SymbolTable* st, const char* name);

// Debug
void symtab_print(SymbolTable* st);

#endif
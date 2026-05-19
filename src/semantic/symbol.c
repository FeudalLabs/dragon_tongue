#include "symbol.h"
#include <stdio.h>
#include <string.h>

#define TABLE_SIZE 1024

static unsigned int hash(const char* name) {
    unsigned int h = 0;
    while (*name) {
        h = h * 31 + *name++;
    }
    return h % TABLE_SIZE;
}

SymbolTable* symtab_new(Arena* arena) {
    SymbolTable* st = arena_alloc(arena, sizeof(SymbolTable));
    st->entries = arena_alloc(arena, TABLE_SIZE * sizeof(Symbol*));
    memset(st->entries, 0, TABLE_SIZE * sizeof(Symbol*));
    st->size = TABLE_SIZE;
    st->parent = NULL;
    st->scope_level = 0;
    st->arena = arena;
    return st;
}

void symtab_enter_scope(SymbolTable* st) {
    SymbolTable* new_scope = symtab_new(st->arena);
    new_scope->parent = st;
    new_scope->scope_level = st->scope_level + 1;
    
    // Copy the pointer - we're replacing the current table with a new one
    // This is simplified; in production you'd manage scope stack
    st->scope_level++;
}

void symtab_exit_scope(SymbolTable* st) {
    if (st->scope_level > 0) {
        st->scope_level--;
    }
}

bool symtab_define(SymbolTable* st, const char* name, SymbolKind kind,
                   ASTNode* node, int line, int col) {
    unsigned int idx = hash(name);
    
    // Check if already defined in current scope
    Symbol* existing = symtab_lookup_current(st, name);
    if (existing) {
        fprintf(stderr, "Error at %d:%d: Symbol '%s' already defined\n", 
                line, col, name);
        return false;
    }
    
    Symbol* sym = arena_alloc(st->arena, sizeof(Symbol));
    sym->name = arena_alloc(st->arena, strlen(name) + 1);
    strcpy(sym->name, name);
    sym->kind = kind;
    sym->node = node;
    sym->next = st->entries[idx];
    sym->scope_level = st->scope_level;
    sym->line = line;
    sym->column = col;
    
    st->entries[idx] = sym;
    return true;
}

Symbol* symtab_lookup(SymbolTable* st, const char* name) {
    unsigned int idx = hash(name);
    Symbol* sym = st->entries[idx];
    
    while (sym) {
        if (strcmp(sym->name, name) == 0) {
            return sym;
        }
        sym = sym->next;
    }
    
    // Check parent scope
    if (st->parent) {
        return symtab_lookup(st->parent, name);
    }
    
    return NULL;
}

Symbol* symtab_lookup_current(SymbolTable* st, const char* name) {
    unsigned int idx = hash(name);
    Symbol* sym = st->entries[idx];
    
    while (sym) {
        if (strcmp(sym->name, name) == 0) {
            return sym;
        }
        sym = sym->next;
    }
    
    return NULL;
}

void symtab_print(SymbolTable* st) {
    printf("Symbol Table (scope level %d):\n", st->scope_level);
    for (int i = 0; i < TABLE_SIZE; i++) {
        Symbol* sym = st->entries[i];
        while (sym) {
            const char* kind_str = "";
            switch (sym->kind) {
                case SYM_VARIABLE: kind_str = "variable"; break;
                case SYM_FUNCTION: kind_str = "function"; break;
                case SYM_PARAMETER: kind_str = "parameter"; break;
                case SYM_STRUCT: kind_str = "struct"; break;
                case SYM_BUILTIN: kind_str = "builtin"; break;
            }
            printf("  %s [%s] at %d:%d\n", sym->name, kind_str, sym->line, sym->column);
            sym = sym->next;
        }
    }
}
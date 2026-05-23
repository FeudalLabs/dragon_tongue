#include "analyzer.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// Forward declarations
static void analyze_statement(SemanticAnalyzer* sa, ASTNode* node);
static TypeInfo analyze_expression(SemanticAnalyzer* sa, ASTNode* node);
static TypeInfo analyze_function_call(SemanticAnalyzer* sa, const char* name, ASTNode* args, int line, int col);

static void error(SemanticAnalyzer* sa, int line, int col, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "Semantic error at %d:%d: ", line, col);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    sa->had_error = true;
    sa->error_count++;
}

// Add built-in functions to symbol table
static void add_builtins(SemanticAnalyzer* sa) {
    const char* builtins[] = {"println", "print", "len", "to_string", 
                               "to_int", "read_file", "write_file", NULL};
    
    for (int i = 0; builtins[i] != NULL; i++) {
        symtab_define(sa->symbols, builtins[i], SYM_BUILTIN, NULL, 0, 0);
    }
}

// Get type from type annotation node
static TypeInfo get_type_from_annotation(ASTNode* type_ann) {
    if (!type_ann || type_ann->type != NODE_VARIABLE) {
        return type_unknown();
    }
    
    Variable* var = (Variable*)type_ann;
    if (strcmp(var->name, "Int") == 0) return type_int();
    if (strcmp(var->name, "Bool") == 0) return type_bool();
    if (strcmp(var->name, "String") == 0) return type_string();
    if (strcmp(var->name, "Void") == 0) return type_void();
    
    return type_unknown();
}

// Analyze function call
static TypeInfo analyze_function_call(SemanticAnalyzer* sa, const char* name, ASTNode* args, int line, int col) {
    Symbol* sym = symtab_lookup(sa->symbols, name);
    if (!sym) {
        error(sa, line, col, "Undefined function '%s'", name);
        return type_error();
    }
    
    // Analyze arguments
    ASTNode* arg = args;
    while (arg) {
        analyze_expression(sa, arg);
        arg = arg->next;
    }
    
    if (sym->kind == SYM_BUILTIN) {
        if (strcmp(name, "println") == 0 || strcmp(name, "print") == 0) {
            return type_void();
        }
        if (strcmp(name, "len") == 0) {
            return type_int();
        }
        if (strcmp(name, "to_string") == 0) {
            return type_string();
        }
        if (strcmp(name, "to_int") == 0) {
            return type_int();
        }
        return type_void();
    }
    
    if (sym->kind == SYM_FUNCTION) {
        Function* func = (Function*)sym->node;
        if (func->return_type) {
            return get_type_from_annotation(func->return_type);
        }
        return type_int();
    }
    
    return type_unknown();
}

// Analyze binary operation
static TypeInfo analyze_binary_op(SemanticAnalyzer* sa, BinaryOp* binop, int line, int col) {
    TypeInfo left = analyze_expression(sa, binop->left);
    TypeInfo right = analyze_expression(sa, binop->right);
    
    // Handle unknown types - allow them to pass through
    if (left.kind == TYPE_UNKNOWN || right.kind == TYPE_UNKNOWN) {
        // For string concatenation, assume string result
        if (strcmp(binop->op, "+") == 0) {
            return type_string();
        }
        return type_unknown();
    }
    
    // Comparison operators return Bool
    if (strcmp(binop->op, "==") == 0 || strcmp(binop->op, "!=") == 0 ||
        strcmp(binop->op, ">") == 0 || strcmp(binop->op, "<") == 0 ||
        strcmp(binop->op, ">=") == 0 || strcmp(binop->op, "<=") == 0) {
        return type_bool();
    }
    
    // String concatenation with +
    if (strcmp(binop->op, "+") == 0) {
        // If either operand is string or unknown, result is string
        if (left.kind == TYPE_STRING || right.kind == TYPE_STRING ||
            left.kind == TYPE_UNKNOWN || right.kind == TYPE_UNKNOWN) {
            return type_string();
        }
        // Integer addition
        if (left.kind == TYPE_INT && right.kind == TYPE_INT) {
            return type_int();
        }
        error(sa, line, col, "Cannot apply + to %s and %s", 
              type_to_string(left), type_to_string(right));
        return type_error();
    }
    
    // Arithmetic operators
    if (strcmp(binop->op, "-") == 0 || strcmp(binop->op, "*") == 0 || strcmp(binop->op, "/") == 0) {
        if (left.kind == TYPE_INT && right.kind == TYPE_INT) {
            return type_int();
        }
        error(sa, line, col, "Cannot apply %s to %s and %s", 
              binop->op, type_to_string(left), type_to_string(right));
        return type_error();
    }
    
    return type_unknown();
}

// Analyze expression
static TypeInfo analyze_expression(SemanticAnalyzer* sa, ASTNode* node) {
    if (!node) return type_void();
    
    switch (node->type) {
        case NODE_LITERAL_INT:
            return type_int();
            
        case NODE_LITERAL_STRING:
            return type_string();
            
        case NODE_VARIABLE: {
            Variable* var = (Variable*)node;
            Symbol* sym = symtab_lookup(sa->symbols, var->name);
            if (!sym) {
                error(sa, node->line, node->column, "Undefined variable '%s'", var->name);
                return type_error();
            }
            // For now, assume variables are integers
            return type_int();
        }
        
        case NODE_CALL: {
            Call* call = (Call*)node;
            return analyze_function_call(sa, call->name, call->arguments, node->line, node->column);
        }
        
        case NODE_BINARY_OP: {
            BinaryOp* binop = (BinaryOp*)node;
            return analyze_binary_op(sa, binop, node->line, node->column);
        }
        
        case NODE_BUILTIN_FUNC: {
            BuiltinFunc* func = (BuiltinFunc*)node;
            return analyze_function_call(sa, func->name, func->arguments, node->line, node->column);
        }
        
        default:
            return type_unknown();
    }
}

// Analyze let statement
static void analyze_let_statement(SemanticAnalyzer* sa, LetStmt* stmt) {
    TypeInfo value_type = analyze_expression(sa, stmt->value);
    symtab_define(sa->symbols, stmt->name, SYM_VARIABLE, (ASTNode*)stmt,
                  stmt->base.line, stmt->base.column);
    
    // Store type info somewhere for code generation
    (void)value_type;
}

// Analyze if statement
static void analyze_if_statement(SemanticAnalyzer* sa, IfStmt* stmt) {
    TypeInfo cond_type = analyze_expression(sa, stmt->condition);
    
    if (cond_type.kind != TYPE_BOOL && cond_type.kind != TYPE_UNKNOWN && cond_type.kind != TYPE_ERROR) {
        error(sa, stmt->base.line, stmt->base.column, 
              "If condition must be boolean, got %s", type_to_string(cond_type));
    }
    
    if (stmt->then_branch) analyze_statement(sa, stmt->then_branch);
    if (stmt->else_branch) analyze_statement(sa, stmt->else_branch);
}

// Analyze block statement
static void analyze_block_statement(SemanticAnalyzer* sa, BlockStmt* block) {
    symtab_enter_scope(sa->symbols);
    
    ASTNode* stmt = block->statements;
    while (stmt) {
        analyze_statement(sa, stmt);
        stmt = stmt->next;
    }
    
    symtab_exit_scope(sa->symbols);
}

// Analyze function
static void analyze_function(SemanticAnalyzer* sa, Function* func) {
    symtab_define(sa->symbols, func->name, SYM_FUNCTION, (ASTNode*)func,
                  func->base.line, func->base.column);
    
    symtab_enter_scope(sa->symbols);
    
    ASTNode* param = func->params;
    while (param) {
        if (param->type == NODE_VARIABLE) {
            Variable* var = (Variable*)param;
            symtab_define(sa->symbols, var->name, SYM_PARAMETER, param,
                          param->line, param->column);
        }
        param = param->next;
    }
    
    if (func->body) analyze_statement(sa, func->body);
    symtab_exit_scope(sa->symbols);
}

// Analyze return statement
static void analyze_return_statement(SemanticAnalyzer* sa, ReturnStmt* stmt) {
    if (stmt->value) analyze_expression(sa, stmt->value);
}

// Analyze a single statement
static void analyze_statement(SemanticAnalyzer* sa, ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_LET_STMT: analyze_let_statement(sa, (LetStmt*)node); break;
        case NODE_IF_STMT: analyze_if_statement(sa, (IfStmt*)node); break;
        case NODE_BLOCK_STMT: analyze_block_statement(sa, (BlockStmt*)node); break;
        case NODE_FUNCTION: analyze_function(sa, (Function*)node); break;
        case NODE_RETURN_STMT: analyze_return_statement(sa, (ReturnStmt*)node); break;
        default: analyze_expression(sa, node); break;
    }
}

// Analyze entire program
void analyzer_analyze(SemanticAnalyzer* sa, ASTNode* program) {
    if (!program || program->type != NODE_PROGRAM) {
        error(sa, 1, 1, "Invalid program AST");
        return;
    }
    
    add_builtins(sa);
    
    // First pass: collect functions
    ASTNode* stmt = program->next;
    while (stmt) {
        if (stmt->type == NODE_FUNCTION) {
            analyze_statement(sa, stmt);
        }
        stmt = stmt->next;
    }
    
    // Second pass: analyze everything else
    stmt = program->next;
    while (stmt) {
        if (stmt->type != NODE_FUNCTION) {
            analyze_statement(sa, stmt);
        }
        stmt = stmt->next;
    }
    
    if (!sa->had_error) {
        printf("Semantic analysis completed successfully.\n");
        symtab_print(sa->symbols);
    }
}

SemanticAnalyzer* analyzer_new(Arena* arena) {
    SemanticAnalyzer* sa = arena_alloc(arena, sizeof(SemanticAnalyzer));
    sa->symbols = symtab_new(arena);
    sa->arena = arena;
    sa->had_error = false;
    sa->error_count = 0;
    return sa;
}

bool analyzer_had_error(SemanticAnalyzer* sa) {
    return sa->had_error;
}

void analyzer_print_errors(SemanticAnalyzer* sa) {
    printf("Total errors: %d\n", sa->error_count);
}
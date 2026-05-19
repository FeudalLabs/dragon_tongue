#include "analyzer.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


// Forward declarations
static void analyze_statement(SemanticAnalyzer* sa, ASTNode* node);
static TypeInfo analyze_expression(SemanticAnalyzer* sa, ASTNode* node);

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
    // Built-in functions are recognized but not type-checked heavily yet
    const char* builtins[] = {"println", "print", "len", "to_string", 
                               "to_int", "read_file", "write_file", NULL};
    
    for (int i = 0; builtins[i] != NULL; i++) {
        symtab_define(sa->symbols, builtins[i], SYM_BUILTIN, NULL, 0, 0);
    }
}

// Analyze binary operation
static TypeInfo analyze_binary_op(SemanticAnalyzer* sa, BinaryOp* binop, int line, int col) {
    TypeInfo left = analyze_expression(sa, binop->left);
    TypeInfo right = analyze_expression(sa, binop->right);
    
    // Comparison operators return Bool
    if (strcmp(binop->op, "==") == 0 || strcmp(binop->op, "!=") == 0 ||
        strcmp(binop->op, ">") == 0 || strcmp(binop->op, "<") == 0 ||
        strcmp(binop->op, ">=") == 0 || strcmp(binop->op, "<=") == 0) {
        if (types_equal(left, right) || left.kind == TYPE_UNKNOWN || right.kind == TYPE_UNKNOWN) {
            return type_bool();
        }
        error(sa, line, col, "Cannot compare %s and %s", 
              type_to_string(left), type_to_string(right));
        return type_error();
    }
    
    // Arithmetic operators require Int
    if (strcmp(binop->op, "+") == 0 || strcmp(binop->op, "-") == 0 ||
        strcmp(binop->op, "*") == 0 || strcmp(binop->op, "/") == 0) {
        
        // String concatenation with +
        if (strcmp(binop->op, "+") == 0) {
            if ((left.kind == TYPE_STRING && right.kind == TYPE_STRING) ||
                (left.kind == TYPE_STRING && right.kind == TYPE_INT) ||
                (left.kind == TYPE_INT && right.kind == TYPE_STRING)) {
                return type_string();
            }
        }
        
        if (left.kind == TYPE_INT && right.kind == TYPE_INT) {
            return type_int();
        }
        
        error(sa, line, col, "Cannot apply %s to %s and %s", 
              binop->op, type_to_string(left), type_to_string(right));
        return type_error();
    }
    
    return type_unknown();
}

// Analyze variable reference
static TypeInfo analyze_variable(SemanticAnalyzer* sa, Variable* var, int line, int col) {
    Symbol* sym = symtab_lookup(sa->symbols, var->name);
    if (!sym) {
        error(sa, line, col, "Undefined variable '%s'", var->name);
        return type_error();
    }
    // Return unknown for now - type would come from symbol
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
            return analyze_variable(sa, var, node->line, node->column);
        }
        
        case NODE_BINARY_OP: {
            BinaryOp* binop = (BinaryOp*)node;
            return analyze_binary_op(sa, binop, node->line, node->column);
        }
        
        case NODE_BUILTIN_FUNC: {
            BuiltinFunc* func = (BuiltinFunc*)node;
            // Built-in functions - simplified type checking
            if (func->arguments) {
                analyze_expression(sa, func->arguments);
            }
            return type_unknown();
        }
        
        default:
            return type_unknown();
    }
}

// Analyze let statement
static void analyze_let_statement(SemanticAnalyzer* sa, LetStmt* stmt) {
    // Analyze the value expression first
    analyze_expression(sa, stmt->value);  // Don't store return value
    
    // Define the variable in symbol table
    symtab_define(sa->symbols, stmt->name, SYM_VARIABLE, (ASTNode*)stmt,
                  stmt->base.line, stmt->base.column);
    
    // Check type annotation if present
    if (stmt->type_annotation) {
        // Would check annotation matches value_type in future
    }
}

// Analyze if statement
static void analyze_if_statement(SemanticAnalyzer* sa, IfStmt* stmt) {
    TypeInfo cond_type = analyze_expression(sa, stmt->condition);
    
    if (cond_type.kind != TYPE_BOOL && cond_type.kind != TYPE_UNKNOWN) {
        error(sa, stmt->base.line, stmt->base.column, 
              "If condition must be boolean, got %s", type_to_string(cond_type));
    }
    
    // Analyze then branch
    if (stmt->then_branch) {
        analyze_statement(sa, stmt->then_branch);
    }
    
    // Analyze else branch
    if (stmt->else_branch) {
        analyze_statement(sa, stmt->else_branch);
    }
}

// Analyze block statement
static void analyze_block_statement(SemanticAnalyzer* sa, BlockStmt* block) {
    // Enter new scope for block
    symtab_enter_scope(sa->symbols);
    
    ASTNode* stmt = block->statements;
    while (stmt) {
        analyze_statement(sa, stmt);
        stmt = stmt->next;
    }
    
    // Exit scope
    symtab_exit_scope(sa->symbols);
}

// Analyze function
static void analyze_function(SemanticAnalyzer* sa, Function* func) {
    // Define function in symbol table
    symtab_define(sa->symbols, func->name, SYM_FUNCTION, (ASTNode*)func,
                  func->base.line, func->base.column);
    
    // Enter function scope
    symtab_enter_scope(sa->symbols);
    
    // Add parameters to symbol table
    ASTNode* param = func->params;
    while (param) {
        if (param->type == NODE_VARIABLE) {
            Variable* var = (Variable*)param;
            symtab_define(sa->symbols, var->name, SYM_PARAMETER, param,
                          param->line, param->column);
        }
        param = param->next;
    }
    
    // Analyze function body
    if (func->body) {
        analyze_statement(sa, func->body);
    }
    
    // Exit function scope
    symtab_exit_scope(sa->symbols);
}

// Analyze return statement
static void analyze_return_statement(SemanticAnalyzer* sa, ReturnStmt* stmt) {
    if (stmt->value) {
        analyze_expression(sa, stmt->value);
    }
}

// Analyze a single statement
static void analyze_statement(SemanticAnalyzer* sa, ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_LET_STMT:
            analyze_let_statement(sa, (LetStmt*)node);
            break;
            
        case NODE_IF_STMT:
            analyze_if_statement(sa, (IfStmt*)node);
            break;
            
        case NODE_BLOCK_STMT:
            analyze_block_statement(sa, (BlockStmt*)node);
            break;
            
        case NODE_FUNCTION:
            analyze_function(sa, (Function*)node);
            break;
            
        case NODE_RETURN_STMT:
            analyze_return_statement(sa, (ReturnStmt*)node);
            break;
            
        default:
            analyze_expression(sa, node);
            break;
    }
}

// Analyze entire program
void analyzer_analyze(SemanticAnalyzer* sa, ASTNode* program) {
    if (!program || program->type != NODE_PROGRAM) {
        error(sa, 1, 1, "Invalid program AST");
        return;
    }
    
    // Add built-in functions
    add_builtins(sa);
    
    // Analyze each statement in the program
    ASTNode* stmt = program->next;
    while (stmt) {
        analyze_statement(sa, stmt);
        stmt = stmt->next;
    }
    
    if (!sa->had_error) {
        printf("Semantic analysis completed successfully.\n");
        printf("Symbols found: ");
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
    // Errors are printed as they occur
    printf("Total errors: %d\n", sa->error_count);
}
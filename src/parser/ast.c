#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* my_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char* new_str = malloc(len);
    if (new_str) memcpy(new_str, s, len);
    return new_str;
}

// Node creation functions
ASTNode* new_program(ASTNode* statements, int line, int col) {
    ASTNode* node = calloc(1, sizeof(ASTNode));
    node->type = NODE_PROGRAM;
    node->line = line;
    node->column = col;
    node->next = statements;
    return node;
}

ASTNode* new_let_stmt(char* name, ASTNode* value, ASTNode* type_ann, int line, int col) {
    LetStmt* stmt = calloc(1, sizeof(LetStmt));
    stmt->base.type = NODE_LET_STMT;
    stmt->base.line = line;
    stmt->base.column = col;
    stmt->name = my_strdup(name);
    stmt->value = value;
    stmt->type_annotation = type_ann;
    return (ASTNode*)stmt;
}

ASTNode* new_if_stmt(ASTNode* condition, ASTNode* then_branch, ASTNode* else_branch, int line, int col) {
    IfStmt* stmt = calloc(1, sizeof(IfStmt));
    stmt->base.type = NODE_IF_STMT;
    stmt->base.line = line;
    stmt->base.column = col;
    stmt->condition = condition;
    stmt->then_branch = then_branch;
    stmt->else_branch = else_branch;
    return (ASTNode*)stmt;
}

ASTNode* new_block_stmt(ASTNode* statements, int line, int col) {
    BlockStmt* block = calloc(1, sizeof(BlockStmt));
    block->base.type = NODE_BLOCK_STMT;
    block->base.line = line;
    block->base.column = col;
    block->statements = statements;
    return (ASTNode*)block;
}

ASTNode* new_function(char* name, ASTNode* params, ASTNode* return_type, ASTNode* body, int line, int col) {
    Function* func = calloc(1, sizeof(Function));
    func->base.type = NODE_FUNCTION;
    func->base.line = line;
    func->base.column = col;
    func->name = my_strdup(name);
    func->params = params;
    func->return_type = return_type;
    func->body = body;
    return (ASTNode*)func;
}

ASTNode* new_struct(char* name, ASTNode* fields, int line, int col) {
    Struct* st = calloc(1, sizeof(Struct));
    st->base.type = NODE_STRUCT;
    st->base.line = line;
    st->base.column = col;
    st->name = my_strdup(name);
    st->fields = fields;
    return (ASTNode*)st;
}

ASTNode* new_binary_op(char* op, ASTNode* left, ASTNode* right, int line, int col) {
    BinaryOp* binop = calloc(1, sizeof(BinaryOp));
    binop->base.type = NODE_BINARY_OP;
    binop->base.line = line;
    binop->base.column = col;
    binop->op = my_strdup(op);
    binop->left = left;
    binop->right = right;
    return (ASTNode*)binop;
}

ASTNode* new_variable(char* name, int line, int col) {
    Variable* var = calloc(1, sizeof(Variable));
    var->base.type = NODE_VARIABLE;
    var->base.line = line;
    var->base.column = col;
    var->name = my_strdup(name);
    return (ASTNode*)var;
}

ASTNode* new_literal_int(long long value, int line, int col) {
    LiteralInt* lit = calloc(1, sizeof(LiteralInt));
    lit->base.type = NODE_LITERAL_INT;
    lit->base.line = line;
    lit->base.column = col;
    lit->value = value;
    return (ASTNode*)lit;
}

ASTNode* new_literal_string(char* value, int line, int col) {
    LiteralString* lit = calloc(1, sizeof(LiteralString));
    lit->base.type = NODE_LITERAL_STRING;
    lit->base.line = line;
    lit->base.column = col;
    lit->value = my_strdup(value);
    return (ASTNode*)lit;
}

ASTNode* new_parallel(ASTNode* statements, int line, int col) {
    Parallel* par = calloc(1, sizeof(Parallel));
    par->base.type = NODE_PARALLEL;
    par->base.line = line;
    par->base.column = col;
    par->statements = statements;
    return (ASTNode*)par;
}

ASTNode* new_builtin_func(char* name, ASTNode* arguments, int line, int col) {
    BuiltinFunc* func = calloc(1, sizeof(BuiltinFunc));
    func->base.type = NODE_BUILTIN_FUNC;
    func->base.line = line;
    func->base.column = col;
    func->name = my_strdup(name);
    func->arguments = arguments;
    return (ASTNode*)func;
}

ASTNode* new_return_stmt(ASTNode* value, int line, int col) {
    ReturnStmt* stmt = calloc(1, sizeof(ReturnStmt));
    stmt->base.type = NODE_RETURN_STMT;
    stmt->base.line = line;
    stmt->base.column = col;
    stmt->value = value;
    return (ASTNode*)stmt;
}

// ADDED: new_call function
ASTNode* new_call(char* name, ASTNode* arguments, int line, int col) {
    Call* call = calloc(1, sizeof(Call));
    call->base.type = NODE_CALL;
    call->base.line = line;
    call->base.column = col;
    call->name = my_strdup(name);
    call->arguments = arguments;
    return (ASTNode*)call;
}

static void print_indent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}

void ast_print(ASTNode* node, int indent) {
    if (!node) return;
    
    // Handle PROGRAM node specially - iterate through statements
    if (node->type == NODE_PROGRAM) {
        printf("Program\n");
        ASTNode* stmt = node->next;
        while (stmt) {
            ast_print(stmt, indent + 1);
            stmt = stmt->next;
        }
        return;
    }
    
    // Handle BLOCK node - iterate through statements
    if (node->type == NODE_BLOCK_STMT) {
        BlockStmt* block = (BlockStmt*)node;
        print_indent(indent);
        printf("Block\n");
        ASTNode* stmt = block->statements;
        while (stmt) {
            ast_print(stmt, indent + 1);
            stmt = stmt->next;
        }
        return;
    }
    
    // Handle other node types
    switch (node->type) {
        case NODE_LET_STMT: {
            LetStmt* stmt = (LetStmt*)node;
            print_indent(indent);
            printf("LetStmt: %s\n", stmt->name);
            if (stmt->value) ast_print(stmt->value, indent + 1);
            break;
        }
        
        case NODE_IF_STMT: {
            IfStmt* stmt = (IfStmt*)node;
            print_indent(indent);
            printf("IfStmt\n");
            print_indent(indent + 1);
            printf("Condition:\n");
            ast_print(stmt->condition, indent + 2);
            print_indent(indent + 1);
            printf("Then:\n");
            ast_print(stmt->then_branch, indent + 2);
            if (stmt->else_branch) {
                print_indent(indent + 1);
                printf("Else:\n");
                ast_print(stmt->else_branch, indent + 2);
            }
            break;
        }
        
        case NODE_FUNCTION: {
            Function* func = (Function*)node;
            print_indent(indent);
            printf("Function: %s\n", func->name);
            if (func->body) ast_print(func->body, indent + 1);
            break;
        }
        
        case NODE_BINARY_OP: {
            BinaryOp* binop = (BinaryOp*)node;
            print_indent(indent);
            printf("BinaryOp: %s\n", binop->op);
            ast_print(binop->left, indent + 1);
            ast_print(binop->right, indent + 1);
            break;
        }
        
        case NODE_VARIABLE: {
            Variable* var = (Variable*)node;
            print_indent(indent);
            printf("Variable: %s\n", var->name);
            break;
        }
        
        case NODE_LITERAL_INT: {
            LiteralInt* lit = (LiteralInt*)node;
            print_indent(indent);
            printf("LiteralInt: %lld\n", lit->value);
            break;
        }
        
        case NODE_LITERAL_STRING: {
            LiteralString* lit = (LiteralString*)node;
            print_indent(indent);
            printf("LiteralString: \"%s\"\n", lit->value);
            break;
        }
        
        case NODE_BUILTIN_FUNC: {
            BuiltinFunc* func = (BuiltinFunc*)node;
            print_indent(indent);
            printf("BuiltinFunc: %s\n", func->name);
            if (func->arguments) {
                ASTNode* arg = func->arguments;
                while (arg) {
                    ast_print(arg, indent + 1);
                    arg = arg->next;
                }
            }
            break;
        }
        
        // ADDED: NODE_CALL handling
        case NODE_CALL: {
            Call* call = (Call*)node;
            print_indent(indent);
            printf("Call: %s\n", call->name);
            if (call->arguments) {
                ASTNode* arg = call->arguments;
                while (arg) {
                    ast_print(arg, indent + 1);
                    arg = arg->next;
                }
            }
            break;
        }
        
        case NODE_RETURN_STMT: {
            ReturnStmt* stmt = (ReturnStmt*)node;
            print_indent(indent);
            printf("Return\n");
            if (stmt->value) ast_print(stmt->value, indent + 1);
            break;
        }
        
        case NODE_PARALLEL: {
            Parallel* par = (Parallel*)node;
            print_indent(indent);
            printf("Parallel\n");
            ASTNode* stmt = par->statements;
            while (stmt) {
                ast_print(stmt, indent + 1);
                stmt = stmt->next;
            }
            break;
        }
        
        default:
            print_indent(indent);
            printf("Unknown(%d)\n", node->type);
            break;
    }
}

void ast_free(ASTNode* node) {
    if (!node) return;
    
    // Free children based on node type (simplified for now)
    if (node->type == NODE_PROGRAM) {
        ast_free(node->next);
    } else if (node->type == NODE_BLOCK_STMT) {
        BlockStmt* block = (BlockStmt*)node;
        ast_free(block->statements);
    } else if (node->type == NODE_LET_STMT) {
        LetStmt* stmt = (LetStmt*)node;
        if (stmt->value) ast_free(stmt->value);
        free(stmt->name);
    } else if (node->type == NODE_IF_STMT) {
        IfStmt* stmt = (IfStmt*)node;
        if (stmt->condition) ast_free(stmt->condition);
        if (stmt->then_branch) ast_free(stmt->then_branch);
        if (stmt->else_branch) ast_free(stmt->else_branch);
    } else if (node->type == NODE_BINARY_OP) {
        BinaryOp* binop = (BinaryOp*)node;
        if (binop->left) ast_free(binop->left);
        if (binop->right) ast_free(binop->right);
        free(binop->op);
    } else if (node->type == NODE_VARIABLE) {
        Variable* var = (Variable*)node;
        free(var->name);
    } else if (node->type == NODE_LITERAL_STRING) {
        LiteralString* lit = (LiteralString*)node;
        free(lit->value);
    } else if (node->type == NODE_BUILTIN_FUNC) {
        BuiltinFunc* func = (BuiltinFunc*)node;
        if (func->arguments) ast_free(func->arguments);
        free(func->name);
    } else if (node->type == NODE_FUNCTION) {
        Function* func = (Function*)node;
        if (func->body) ast_free(func->body);
        free(func->name);
    // ADDED: NODE_CALL freeing
    } else if (node->type == NODE_CALL) {
        Call* call = (Call*)node;
        if (call->arguments) ast_free(call->arguments);
        free(call->name);
    }
    
    free(node);
}
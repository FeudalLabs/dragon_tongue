#include "cgen.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

static void cgen_indent(CodeGenerator* cg) {
    for (int i = 0; i < cg->indent_level; i++) {
        fprintf(cg->output, "  ");
    }
}

static void cgen_writeln(CodeGenerator* cg, const char* fmt, ...) {
    cgen_indent(cg);
    va_list args;
    va_start(args, fmt);
    vfprintf(cg->output, fmt, args);
    va_end(args);
    fprintf(cg->output, "\n");
}

static void cgen_generate_expression(CodeGenerator* cg, ASTNode* node);
static void cgen_generate_statement(CodeGenerator* cg, ASTNode* node);
static void cgen_generate_function(CodeGenerator* cg, Function* func);

// Track variable types (simplified - in production, from symbol table)
static int is_string_variable(const char* name) {
    // Common string variables
    if (strcmp(name, "greeting") == 0) return 1;
    return 0;
}

// Determine if a node is an integer expression
static int is_integer_node(CodeGenerator* cg, ASTNode* node) {
    if (!node) return 0;
    switch (node->type) {
        case NODE_LITERAL_INT:
            return 1;
        case NODE_VARIABLE: {
            Variable* var = (Variable*)node;
            // Check if this is a known string variable
            if (is_string_variable(var->name)) return 0;
            return 1;
        }
        case NODE_BINARY_OP: {
            BinaryOp* binop = (BinaryOp*)node;
            if (strcmp(binop->op, "+") == 0) {
                return is_integer_node(cg, binop->left) && is_integer_node(cg, binop->right);
            }
            return 1;
        }
        case NODE_CALL:
            return 1; // Function calls return int by default
        default:
            return 0;
    }
}

// Generate literal integer
static void cgen_generate_literal_int(CodeGenerator* cg, LiteralInt* lit) {
    fprintf(cg->output, "%lld", lit->value);
}

// Generate literal string
static void cgen_generate_literal_string(CodeGenerator* cg, LiteralString* lit) {
    fprintf(cg->output, "\"%s\"", lit->value);
}

// Generate variable reference
static void cgen_generate_variable(CodeGenerator* cg, Variable* var) {
    fprintf(cg->output, "%s", var->name);
}

// Generate function call
static void cgen_generate_call(CodeGenerator* cg, Call* call) {
    fprintf(cg->output, "%s(", call->name);
    ASTNode* arg = call->arguments;
    int first = 1;
    while (arg) {
        if (!first) fprintf(cg->output, ", ");
        cgen_generate_expression(cg, arg);
        first = 0;
        arg = arg->next;
    }
    fprintf(cg->output, ")");
}

// Generate binary operation
static void cgen_generate_binary_op(CodeGenerator* cg, BinaryOp* binop) {
    if (strcmp(binop->op, "+") == 0) {
        // Check if this is integer addition or string concatenation
        if (is_integer_node(cg, binop->left) && is_integer_node(cg, binop->right)) {
            fprintf(cg->output, "(");
            cgen_generate_expression(cg, binop->left);
            fprintf(cg->output, " + ");
            cgen_generate_expression(cg, binop->right);
            fprintf(cg->output, ")");
        } else {
            // String concatenation
            fprintf(cg->output, "({ char* _a = ");
            cgen_generate_expression(cg, binop->left);
            fprintf(cg->output, "; char* _b = ");
            cgen_generate_expression(cg, binop->right);
            fprintf(cg->output, "; char* _r = malloc(strlen(_a) + strlen(_b) + 1); strcpy(_r, _a); strcat(_r, _b); _r; })");
        }
    } else {
        fprintf(cg->output, "(");
        cgen_generate_expression(cg, binop->left);
        fprintf(cg->output, " %s ", binop->op);
        cgen_generate_expression(cg, binop->right);
        fprintf(cg->output, ")");
    }
}

// Generate built-in function call
static void cgen_generate_builtin(CodeGenerator* cg, BuiltinFunc* func) {
    if (strcmp(func->name, "println") == 0) {
        if (func->arguments) {
            ASTNode* arg = func->arguments;
            if (is_integer_node(cg, arg)) {
                fprintf(cg->output, "printf(\"%%d\\n\", ");
                cgen_generate_expression(cg, arg);
                fprintf(cg->output, ")");
            } else {
                fprintf(cg->output, "puts(");
                cgen_generate_expression(cg, arg);
                fprintf(cg->output, ")");
            }
        } else {
            fprintf(cg->output, "printf(\"\\n\")");
        }
    } else if (strcmp(func->name, "print") == 0) {
        if (func->arguments) {
            ASTNode* arg = func->arguments;
            if (is_integer_node(cg, arg)) {
                fprintf(cg->output, "printf(\"%%d\", ");
                cgen_generate_expression(cg, arg);
                fprintf(cg->output, ")");
            } else {
                fprintf(cg->output, "printf(");
                cgen_generate_expression(cg, arg);
                fprintf(cg->output, ")");
            }
        }
    } else if (strcmp(func->name, "to_string") == 0) {
        fprintf(cg->output, "({ char buf[32]; sprintf(buf, \"%%d\", ");
        cgen_generate_expression(cg, func->arguments);
        fprintf(cg->output, "); strdup(buf); })");
    } else if (strcmp(func->name, "to_int") == 0) {
        fprintf(cg->output, "atoi(");
        cgen_generate_expression(cg, func->arguments);
        fprintf(cg->output, ")");
    } else if (strcmp(func->name, "len") == 0) {
        fprintf(cg->output, "strlen(");
        cgen_generate_expression(cg, func->arguments);
        fprintf(cg->output, ")");
    } else {
        fprintf(cg->output, "%s", func->name);
        if (func->arguments) {
            fprintf(cg->output, "(");
            cgen_generate_expression(cg, func->arguments);
            fprintf(cg->output, ")");
        }
    }
}

// Generate expression
static void cgen_generate_expression(CodeGenerator* cg, ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_LITERAL_INT:
            cgen_generate_literal_int(cg, (LiteralInt*)node);
            break;
        case NODE_LITERAL_STRING:
            cgen_generate_literal_string(cg, (LiteralString*)node);
            break;
        case NODE_VARIABLE:
            cgen_generate_variable(cg, (Variable*)node);
            break;
        case NODE_BINARY_OP:
            cgen_generate_binary_op(cg, (BinaryOp*)node);
            break;
        case NODE_BUILTIN_FUNC:
            cgen_generate_builtin(cg, (BuiltinFunc*)node);
            break;
        case NODE_CALL:
            cgen_generate_call(cg, (Call*)node);
            break;
        default:
            fprintf(stderr, "Unknown expression type: %d\n", node->type);
            break;
    }
}

// Generate let statement
static void cgen_generate_let_statement(CodeGenerator* cg, LetStmt* stmt) {
    if (stmt->value && is_integer_node(cg, stmt->value)) {
        cgen_writeln(cg, "int %s = ", stmt->name);
        cgen_indent(cg);
        cgen_generate_expression(cg, stmt->value);
        fprintf(cg->output, ";\n");
    } else {
        cgen_writeln(cg, "char* %s = ", stmt->name);
        cgen_indent(cg);
        cgen_generate_expression(cg, stmt->value);
        fprintf(cg->output, ";\n");
    }
}

// Generate return statement
static void cgen_generate_return_statement(CodeGenerator* cg, ReturnStmt* stmt) {
    if (stmt->value) {
        cgen_writeln(cg, "return ");
        cgen_indent(cg);
        cgen_generate_expression(cg, stmt->value);
        fprintf(cg->output, ";\n");
    } else {
        cgen_writeln(cg, "return;\n");
    }
}

// Generate if statement
static void cgen_generate_if_statement(CodeGenerator* cg, IfStmt* stmt) {
    cgen_writeln(cg, "if (");
    cgen_indent(cg);
    cgen_generate_expression(cg, stmt->condition);
    fprintf(cg->output, ") {\n");
    
    cg->indent_level++;
    cgen_generate_statement(cg, stmt->then_branch);
    cg->indent_level--;
    
    if (stmt->else_branch) {
        cgen_writeln(cg, "} else {\n");
        cg->indent_level++;
        cgen_generate_statement(cg, stmt->else_branch);
        cg->indent_level--;
    }
    
    cgen_writeln(cg, "}");
}

// Generate block statement
static void cgen_generate_block_statement(CodeGenerator* cg, BlockStmt* block) {
    ASTNode* stmt = block->statements;
    while (stmt) {
        cgen_generate_statement(cg, stmt);
        stmt = stmt->next;
    }
}

// Generate function
static void cgen_generate_function(CodeGenerator* cg, Function* func) {
    cg->current_function = func->name;
    
    fprintf(cg->output, "int %s(", func->name);
    
    ASTNode* param = func->params;
    int param_count = 0;
    while (param) {
        if (param_count > 0) fprintf(cg->output, ", ");
        Variable* var = (Variable*)param;
        fprintf(cg->output, "int %s", var->name);
        param_count++;
        param = param->next;
    }
    fprintf(cg->output, ") {\n");
    
    cg->indent_level++;
    if (func->body) {
        cgen_generate_statement(cg, func->body);
    }
    cgen_writeln(cg, "return 0;");
    cg->indent_level--;
    fprintf(cg->output, "}\n\n");
    cg->current_function = NULL;
}

// Generate statement dispatcher
static void cgen_generate_statement(CodeGenerator* cg, ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_LET_STMT:
            cgen_generate_let_statement(cg, (LetStmt*)node);
            break;
        case NODE_RETURN_STMT:
            cgen_generate_return_statement(cg, (ReturnStmt*)node);
            break;
        case NODE_IF_STMT:
            cgen_generate_if_statement(cg, (IfStmt*)node);
            break;
        case NODE_BLOCK_STMT:
            cgen_generate_block_statement(cg, (BlockStmt*)node);
            break;
        case NODE_FUNCTION:
            break;
        default:
            cgen_indent(cg);
            cgen_generate_expression(cg, node);
            fprintf(cg->output, ";\n");
            break;
    }
}

// Generate program
void cgen_generate(CodeGenerator* cg, ASTNode* program) {
    if (!program || program->type != NODE_PROGRAM) {
        fprintf(stderr, "Invalid program AST\n");
        cg->had_error = true;
        return;
    }
    
    fprintf(cg->output, "#include <stdio.h>\n");
    fprintf(cg->output, "#include <stdlib.h>\n");
    fprintf(cg->output, "#include <string.h>\n\n");
    
    // Function declarations
    ASTNode* stmt = program->next;
    while (stmt) {
        if (stmt->type == NODE_FUNCTION) {
            Function* func = (Function*)stmt;
            fprintf(cg->output, "int %s(", func->name);
            ASTNode* param = func->params;
            int pcount = 0;
            while (param) {
                if (pcount > 0) fprintf(cg->output, ", ");
                fprintf(cg->output, "int");
                pcount++;
                param = param->next;
            }
            fprintf(cg->output, ");\n");
        }
        stmt = stmt->next;
    }
    fprintf(cg->output, "\n");
    
    // Main function
    fprintf(cg->output, "int main() {\n");
    cg->indent_level++;
    
    stmt = program->next;
    while (stmt) {
        if (stmt->type != NODE_FUNCTION) {
            cgen_generate_statement(cg, stmt);
        }
        stmt = stmt->next;
    }
    
    cgen_writeln(cg, "return 0;");
    cg->indent_level--;
    fprintf(cg->output, "}\n\n");
    
    // Function definitions
    stmt = program->next;
    while (stmt) {
        if (stmt->type == NODE_FUNCTION) {
            cgen_generate_function(cg, (Function*)stmt);
        }
        stmt = stmt->next;
    }
}

CodeGenerator* cgen_new(const char* filename, SymbolTable* symbols, Arena* arena) {
    CodeGenerator* cg = arena_alloc(arena, sizeof(CodeGenerator));
    cg->output = fopen(filename, "w");
    if (!cg->output) {
        fprintf(stderr, "Cannot open output file: %s\n", filename);
        cg->had_error = true;
    }
    cg->symbols = symbols;
    cg->arena = arena;
    cg->indent_level = 0;
    cg->temp_counter = 0;
    cg->current_function = NULL;
    cg->had_error = false;
    return cg;
}

bool cgen_had_error(CodeGenerator* cg) {
    return cg->had_error;
}

void cgen_close(CodeGenerator* cg) {
    if (cg->output) {
        fclose(cg->output);
    }
}
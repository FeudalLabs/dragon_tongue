#ifndef AST_H
#define AST_H

#include "../dragontongue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Types of AST nodes
typedef enum {
    NODE_PROGRAM,
    NODE_LET_STMT,
    NODE_VAR_STMT,
    NODE_RETURN_STMT,
    NODE_IF_STMT,
    NODE_FOR_STMT,
    NODE_WHILE_STMT,
    NODE_BLOCK_STMT,
    NODE_FUNCTION,
    NODE_STRUCT,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_CALL,
    NODE_VARIABLE,
    NODE_ASSIGN,
    NODE_LITERAL_INT,
    NODE_LITERAL_FLOAT,
    NODE_LITERAL_STRING,
    NODE_LITERAL_BOOL,
    NODE_PARALLEL,
    NODE_SPAWN,
    NODE_AWAIT,
    NODE_MATCH,
    NODE_MATCH_ARM,
    NODE_TYPE_ANNOTATION,
    NODE_BUILTIN_FUNC
} NodeType;

// Forward declaration
typedef struct ASTNode ASTNode;

// Base AST node
struct ASTNode {
    NodeType type;
    int line;
    int column;
    struct ASTNode* next;
};

// Statement nodes
typedef struct {
    ASTNode base;
    char* name;
    ASTNode* value;
    ASTNode* type_annotation;
} LetStmt;

typedef struct {
    ASTNode base;
    ASTNode* condition;
    ASTNode* then_branch;
    ASTNode* else_branch;
} IfStmt;

typedef struct {
    ASTNode base;
    ASTNode* statements;
} BlockStmt;

typedef struct {
    ASTNode base;
    ASTNode* condition;
    ASTNode* body;
} WhileStmt;

typedef struct {
    ASTNode base;
    ASTNode* value;
} ReturnStmt;

// Function and struct nodes
typedef struct {
    ASTNode base;
    char* name;
    ASTNode* params;
    ASTNode* return_type;
    ASTNode* body;
} Function;

typedef struct {
    ASTNode base;
    char* name;
    ASTNode* fields;
} Struct;

typedef struct {
    ASTNode base;
    char* name;
    ASTNode* type;
} Parameter;

typedef struct {
    ASTNode base;
    char* name;
    ASTNode* type;
} Field;

// Expression nodes
typedef struct {
    ASTNode base;
    char* op;
    ASTNode* left;
    ASTNode* right;
} BinaryOp;

typedef struct {
    ASTNode base;
    char* op;
    ASTNode* operand;
} UnaryOp;

typedef struct {
    ASTNode base;
    char* name;
    ASTNode* arguments;
} Call;

typedef struct {
    ASTNode base;
    char* name;
} Variable;

typedef struct {
    ASTNode base;
    char* name;
    ASTNode* target;
    ASTNode* value;
} Assign;

// Literal nodes
typedef struct {
    ASTNode base;
    long long value;
} LiteralInt;

typedef struct {
    ASTNode base;
    double value;
} LiteralFloat;

typedef struct {
    ASTNode base;
    char* value;
} LiteralString;

typedef struct {
    ASTNode base;
    int value;  // 0 = false, 1 = true
} LiteralBool;

// Concurrency nodes
typedef struct {
    ASTNode base;
    ASTNode* statements;
} Parallel;

typedef struct {
    ASTNode base;
    ASTNode* call;
} Spawn;

typedef struct {
    ASTNode base;
    ASTNode* handle;
} Await;

// Pattern matching nodes
typedef struct {
    ASTNode base;
    ASTNode* value;
    ASTNode* arms;
} Match;

typedef struct {
    ASTNode base;
    ASTNode* pattern;
    ASTNode* body;
} MatchArm;

// Built-in function node
typedef struct {
    ASTNode base;
    char* name;
    ASTNode* arguments;
} BuiltinFunc;

// Node creation functions
ASTNode* new_program(ASTNode* statements, int line, int col);
ASTNode* new_let_stmt(char* name, ASTNode* value, ASTNode* type_ann, int line, int col);
ASTNode* new_if_stmt(ASTNode* condition, ASTNode* then_branch, ASTNode* else_branch, int line, int col);
ASTNode* new_block_stmt(ASTNode* statements, int line, int col);
ASTNode* new_function(char* name, ASTNode* params, ASTNode* return_type, ASTNode* body, int line, int col);
ASTNode* new_struct(char* name, ASTNode* fields, int line, int col);
ASTNode* new_binary_op(char* op, ASTNode* left, ASTNode* right, int line, int col);
ASTNode* new_variable(char* name, int line, int col);
ASTNode* new_literal_int(long long value, int line, int col);
ASTNode* new_literal_string(char* value, int line, int col);
ASTNode* new_parallel(ASTNode* statements, int line, int col);
ASTNode* new_builtin_func(char* name, ASTNode* arguments, int line, int col);
ASTNode* new_return_stmt(ASTNode* value, int line, int col);

// Helper functions
void ast_print(ASTNode* node, int indent);
void ast_free(ASTNode* node);

#endif
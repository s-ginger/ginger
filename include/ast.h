#ifndef AST_H
#define AST_H
#include "arena.h"
#include "core.h"

typedef enum { AST_IDENT, AST_INT, AST_BINOP, AST_STR } AstType;

typedef struct Ast {
    AstType type;
    union {
        i32 value;
        struct {
            char *value;
            usize length;
        } string_literal;
        struct {
            char *name;
            usize length;
        } ident;
        struct {
            struct Ast *l;
            struct Ast *r;
            char op;
        } bin_op;
    };
} Ast;

Ast *new_bin_op_ast(Arena* a, Ast *left, Ast *right, char op);
Ast *new_int_ast(Arena* a, i32 value);
Ast *new_ident_ast(Arena* a,const char *name, usize lenght);
Ast *new_string_ast( Arena* a, const char *name, usize length);

typedef enum {
    STMT_VAR,    // var x = expr;
    STMT_CONST,  // const x = expr;
    STMT_EXPR,   // expr;
    STMT_ASSIGN, // x = expr;
    STMT_BLOCK,  // { stmt* }

    STMT_IMPORT,
    STMT_PACKAGE,

} StmtType;

typedef struct Stmt {
    StmtType type;
    union {
        struct {
            const char *name;
            usize length;
            Ast *value;
        } var_decl;

        struct {
            const char *name;
            usize length;
            Ast *value;
        } const_decl;

        struct {
            const char *name;
            usize length;
            Ast *value;
        } assign;

        struct {
            Ast *expr;
        } expr_stmt;

        struct {
            const char *name;
            usize length;
        } package_decl;

        struct {
            const char *name;
            usize length;
        } import_decl;

        struct {
            struct Stmt **stmts;
            usize count;
        } block;
    };
} Stmt;

Stmt *new_var_stmt(Arena *a, const char *name, usize len, Ast *value);
Stmt *new_const_stmt(Arena *a, const char *name, usize len, Ast *value);
Stmt *new_package_stmt(Arena *a, const char *name, usize len);
Stmt *new_import_stmt(Arena *a, const char *name, usize len);
Stmt *new_assign_stmt(Arena *a, const char *name, usize len, Ast *value);
Stmt *new_expr_stmt(Arena *a, Ast *expr);
Stmt *new_block_stmt(Arena *a, Stmt **stmts, usize count);

void print_ast(Ast *ast, i32 indent);
void print_stmt(Stmt *stmt, i32 indent);
void print_program(Stmt **program);

#endif
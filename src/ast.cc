#include "ast.h"
#include "core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Ast *new_bin_op_ast(Arena *a, Ast *left, Ast *right, char op) {
    Ast *ast_node = (Ast *)arena_alloc(a, sizeof(Ast));
    if (!ast_node)
        return NULL;

    ast_node->type = AST_BINOP;
    ast_node->bin_op.l = left;
    ast_node->bin_op.r = right;
    ast_node->bin_op.op = op;

    return ast_node;
}

Ast *new_int_ast(Arena *a, i32 value) {
    Ast *ast_node = (Ast *)arena_alloc(a, sizeof(Ast));
    if (!ast_node)
        return NULL;

    ast_node->type = AST_INT;
    ast_node->value = value;

    return ast_node;
}

Ast *new_ident_ast(Arena *a, const char *name, usize length) {
    Ast *node = (Ast *)arena_alloc(a, sizeof(Ast));
    if (!node)
        return NULL;

    node->type = AST_IDENT;
    node->ident.name = (char *)arena_alloc(a, length + 1); // память из арены
    if (!node->ident.name)
        return NULL;

    memcpy(node->ident.name, name, length);
    node->ident.name[length] = '\0';
    node->ident.length = length;

    return node;
}

Ast *new_string_ast(Arena *a, const char *value, usize length) {
    Ast *node = (Ast *)arena_alloc(a, sizeof(Ast));
    if (!node)
        return NULL;

    node->type = AST_STR;
    node->string_literal.value = (char *)arena_alloc(a, length + 1);
    if (!node->string_literal.value)
        return NULL;

    memcpy(node->string_literal.value, value, length);
    node->string_literal.value[length] = '\0';
    node->string_literal.length = length;

    return node;
}

Stmt *new_var_stmt(Arena *a, const char *name, usize len, Ast *value) {
    Stmt *stmt = (Stmt *)arena_alloc(a, sizeof(Stmt));
    if (!stmt)
        return NULL;

    stmt->type = STMT_VAR;
    stmt->var_decl.name = (char *)arena_alloc(a, len + 1);
    if (!stmt->var_decl.name)
        return NULL;
    memcpy((char *)stmt->var_decl.name, name, len);
    ((char *)stmt->var_decl.name)[len] = '\0';
    stmt->var_decl.length = len;
    stmt->var_decl.value = value;

    return stmt;
}

Stmt *new_const_stmt(Arena *a, const char *name, usize len, Ast *value) {
    Stmt *stmt = (Stmt *)arena_alloc(a, sizeof(Stmt));
    if (!stmt)
        return NULL;

    stmt->type = STMT_CONST;
    stmt->const_decl.name = (char *)arena_alloc(a, len + 1);
    if (!stmt->const_decl.name)
        return NULL;
    memcpy((char *)stmt->const_decl.name, name, len);
    ((char *)stmt->const_decl.name)[len] = '\0';
    stmt->const_decl.length = len;
    stmt->const_decl.value = value;

    return stmt;
}

Stmt *new_package_stmt(Arena *a, const char *name, usize len) {
    Stmt *stmt = (Stmt *)arena_alloc(a, sizeof(Stmt));
    if (!stmt)
        return NULL;

    stmt->type = STMT_PACKAGE;
    stmt->package_decl.name = (char *)arena_alloc(a, len + 1);
    if (!stmt->package_decl.name)
        return NULL;
    memcpy((char *)stmt->package_decl.name, name, len);
    ((char *)stmt->package_decl.name)[len] = '\0';
    stmt->package_decl.length = len;

    return stmt;
}

Stmt *new_import_stmt(Arena *a, const char *name, usize len) {
    Stmt *stmt = (Stmt *)arena_alloc(a, sizeof(Stmt));
    if (!stmt)
        return NULL;

    stmt->type = STMT_IMPORT;
    stmt->import_decl.name = (char *)arena_alloc(a, len + 1);
    if (!stmt->import_decl.name)
        return NULL;
    memcpy((char *)stmt->import_decl.name, name, len);
    ((char *)stmt->import_decl.name)[len] = '\0';
    stmt->import_decl.length = len;

    return stmt;
}

Stmt *new_assign_stmt(Arena *a, const char *name, usize len, Ast *value) {
    Stmt *stmt = (Stmt *)arena_alloc(a, sizeof(Stmt));
    if (!stmt)
        return NULL;

    stmt->type = STMT_ASSIGN;
    stmt->assign.name = (char *)arena_alloc(a, len + 1);
    if (!stmt->assign.name)
        return NULL;
    memcpy((char *)stmt->assign.name, name, len);
    ((char *)stmt->assign.name)[len] = '\0';
    stmt->assign.length = len;
    stmt->assign.value = value;

    return stmt;
}

Stmt *new_expr_stmt(Arena *a, Ast *expr) {
    Stmt *s = (Stmt *)arena_alloc(a, sizeof(Stmt));
    if (!s)
        return NULL;

    s->type = STMT_EXPR;
    s->expr_stmt.expr = expr;
    return s;
}

Stmt *new_block_stmt(Arena *a, Stmt **stmts, usize count) {
    Stmt *s = (Stmt *)arena_alloc(a, sizeof(Stmt));
    if (!s)
        return NULL;

    s->type = STMT_BLOCK;
    s->block.stmts = stmts;
    s->block.count = count;
    return s;
}

static void print_indent(i32 indent) {
    for (int i = 0; i < indent; i++)
        printf("  ");
}

void print_ast(Ast *ast, i32 indent) {
    if (!ast)
        return;

    print_indent(indent);

    switch (ast->type) {
    case AST_INT:
        printf("Int(%d)\n", ast->value);
        break;

    case AST_STR:
        printf("String(%.*s)\n", (i32)ast->string_literal.length,
               ast->string_literal.value);
        break;
    case AST_IDENT:
        printf("Ident(%.*s)\n", (i32)ast->ident.length, ast->ident.name);
        break;

    case AST_BINOP:
        printf("BinOp('%c')\n", ast->bin_op.op);

        print_ast(ast->bin_op.l, indent + 1);
        print_ast(ast->bin_op.r, indent + 1);
        break;
    }
}

void print_stmt(Stmt *stmt, i32 indent) {
    if (!stmt)
        return;

    print_indent(indent);

    switch (stmt->type) {
    case STMT_VAR:
        printf("VarDecl(%.*s)\n", (i32)stmt->var_decl.length,
               stmt->var_decl.name);
        print_ast(stmt->var_decl.value, indent + 1);
        break;

    case STMT_CONST:
        printf("ConstDecl(%.*s)\n", (i32)stmt->const_decl.length,
               stmt->const_decl.name);
        print_ast(stmt->const_decl.value, indent + 1);
        break;

    case STMT_ASSIGN:
        printf("Assign(%.*s)\n", (i32)stmt->assign.length, stmt->assign.name);
        print_ast(stmt->assign.value, indent + 1);
        break;

    case STMT_PACKAGE:
        printf("Package(%.*s)\n", (i32)stmt->package_decl.length,
               stmt->package_decl.name);
        break;

    case STMT_IMPORT:
        printf("Import(%.*s)\n", (i32)stmt->import_decl.length,
               stmt->import_decl.name);
        break;

    case STMT_EXPR:
        printf("ExprStmt\n");
        print_ast(stmt->expr_stmt.expr, indent + 1);
        break;

    case STMT_BLOCK:
        printf("Block\n");
        for (usize i = 0; i < stmt->block.count; i++) {
            print_stmt(stmt->block.stmts[i], indent + 1);
        }
        break;
    }
}

void print_program(Stmt **program) {
    for (size_t i = 0; program[i] != NULL; i++) {
        print_stmt(program[i], 0);
    }
}
#include "ast.h"
#include <stdlib.h>
#include <string.h> 

Ast *new_bin_op_ast(Ast* left, Ast* right, char op)
{
    Ast *ast_node = malloc(sizeof(Ast));
    if (!ast_node) return NULL;

    ast_node->type = AST_BINOP;
    ast_node->l = left;
    ast_node->r = right;
    ast_node->op = op;

    return ast_node;
}

Ast *new_int_ast(int value)
{
    Ast *ast_node = malloc(sizeof(Ast));
    if (!ast_node) return NULL;

    ast_node->type = AST_INT;
    ast_node->value = value;

    return ast_node;
}

Ast* new_ident_ast(const char* name, size_t length) {
    Ast* node = malloc(sizeof(Ast));
    if (!node) return NULL;

    node->type = AST_IDENT;
    node->ident.name = malloc(length + 1);  // выделяем память
    if (!node->ident.name) {
        free(node);
        return NULL;
    }
    memcpy(node->ident.name, name, length); // копируем содержимое
    node->ident.name[length] = '\0';       // добавляем нулевой терминатор
    node->ident.length = length;
    return node;
}



void free_ast(Ast* ast) {
    if (!ast) return;

    switch (ast->type) {
        case AST_INT:
            // ничего дополнительного
            break;

        case AST_IDENT:
            // name обычно указывает в исходный код → НЕ free
            break;

        case AST_BINOP:
            free_ast(ast->l);
            free_ast(ast->r);
            break;
    }

    free(ast);
}


Stmt* new_var_stmt(const char* name, size_t len, Ast* value) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_VAR;
    stmt->var_decl.name = name;
    stmt->var_decl.length = len;
    stmt->var_decl.value = value;
    return stmt;
}

Stmt* new_const_stmt(const char* name, size_t len, Ast* value) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_CONST;
    stmt->const_decl.name = name;
    stmt->const_decl.length = len;
    stmt->const_decl.value = value;
    return stmt;
}

Stmt* new_assign_stmt(const char* name, size_t len, Ast* value) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_ASSIGN;
    stmt->assign.name = name;
    stmt->assign.length = len;
    stmt->assign.value = value;
    return stmt;
}

Stmt* new_expr_stmt(Ast* expr) {
    Stmt* s = malloc(sizeof(Stmt));
    s->type = STMT_EXPR;
    s->expr_stmt.expr = expr;
    return s;
}

Stmt* new_block_stmt(Stmt** stmts, size_t count) {
    Stmt* s = malloc(sizeof(Stmt));
    s->type = STMT_BLOCK;
    s->block.stmts = stmts;
    s->block.count = count;
    return s;
}

void free_stmt(Stmt* stmt) {
    if (!stmt) return;

    switch (stmt->type) {
        case STMT_VAR:
            free_ast(stmt->var_decl.value);
            break;

        case STMT_CONST:
            free_ast(stmt->const_decl.value);
            break;

        case STMT_ASSIGN:
            free_ast(stmt->assign.value);
            break;

        case STMT_EXPR:
            free_ast(stmt->expr_stmt.expr);
            break;

        case STMT_BLOCK:
            for (size_t i = 0; i < stmt->block.count; i++) {
                free_stmt(stmt->block.stmts[i]);
            }
            free(stmt->block.stmts);
            break;
    }

    free(stmt);
}

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

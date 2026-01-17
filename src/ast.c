#include "ast.h"
#include <stdlib.h>


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

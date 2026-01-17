#ifndef AST_H
#define AST_H

typedef enum {
    AST_INT,
    AST_BINOP

} AstType;


typedef struct Ast
{
    AstType type;
    union 
    {
        int value;
        struct
        {
            struct Ast* l;
            struct Ast* r;
            char op;
        };  
    }; 
} Ast;

Ast *new_bin_op_ast(Ast* left, Ast* right, char op);
Ast *new_int_ast(int value);

#endif
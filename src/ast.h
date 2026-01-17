#ifndef AST_H
#define AST_H

typedef enum {
    AST_IDENT,
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
            char* name;
            size_t length;
        } ident;
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
Ast *new_ident_ast(const char* name, size_t lenght);

#endif
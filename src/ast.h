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


typedef enum {
    STMT_VAR,      // var x = expr;
    STMT_CONST,    // const x = expr;
    STMT_EXPR,     // expr;
    STMT_ASSIGN,   // x = expr;
    STMT_BLOCK     // { stmt* }
} StmtType;


typedef struct Stmt
{
    StmtType type;
    union {
        struct {
            const char* name;
            size_t length;
            Ast* value;
        } var_decl;

        struct {
            const char* name;
            size_t length;
            Ast* value;
        } const_decl;

        struct {
            const char* name;
            size_t length;
            Ast* value;
        } assign;

        struct {
            Ast* expr;
        } expr_stmt;

        struct {
            Stmt** stmts;
            size_t count;
        } block;
    };
} Stmt;

Stmt* new_var_stmt(const char* name, size_t len, Ast* value);
Stmt* new_const_stmt(const char* name, size_t len, Ast* value);
Stmt* new_assign_stmt(const char* name, size_t len, Ast* value);
Stmt* new_expr_stmt(Ast* expr);
Stmt* new_block_stmt(Stmt** stmts, size_t count);

void free_ast(Ast* ast);
void free_stmt(Stmt* stmt);

#endif
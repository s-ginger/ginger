#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "ast.h"

typedef struct Parser
{
    Lexer* lexer;
    Token current;
} Parser;

void parser_init(Parser* p, Lexer* lexer);
Token parser_advance(Parser *p);

Ast* parse_factor(Parser* p);
Ast* parse_term(Parser* p);
Ast* parse_expr(Parser* p);
Stmt* parse_stmt(Parser* p);

#endif
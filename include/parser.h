#ifndef PARSER_H
#define PARSER_H
#include "ast.h"
#include "lexer.h"

typedef struct Parser {
  Lexer *lexer;
  Token current;
  Token next;
  Context *ctx;
} Parser;

void parser_init(Parser *p, Lexer *lexer);
Token parser_advance(Parser *p);

Ast *parse_factor(Parser *p);
Ast *parse_term(Parser *p);
Ast *parse_expr(Parser *p);
Stmt *parse_stmt(Parser *p);
Stmt **parse_program(Parser *p);

#endif
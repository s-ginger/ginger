#include "parser.h"
#include "tokens.h"
#include "ast.h"
#include <stdlib.h>

void parser_init(Parser* p, Lexer* lexer) {
    p->lexer = lexer;
    p->current = lexer_next(lexer);
}

Token parser_advance(Parser *p) {
    p->current = lexer_next(p->lexer);
    return p->current;
}


Ast* parse_factor(Parser* p) {
    if (p->current.type == TOK_LPAREN) {
        parser_advance(p);
        Ast* ast = parse_expr(p);
        if (p->current.type != TOK_RPAREN) return NULL;
        parser_advance(p);
        return ast;
    }

    if (p->current.type == TOK_IDENT) {
        Ast* ident_ast = new_ident_ast(p->current.start, p->current.length);
        parser_advance(p);
        return ident_ast;
    }

    if (p->current.type != TOK_INT) {
        return NULL; // ошибка
    }

    Ast* node = new_int_ast(atoi(p->current.start));
    parser_advance(p); // следующий токен
    return node;
}

Ast* parse_term(Parser* p) {
    Ast* left = parse_factor(p);
    if (!left) return NULL;

    while (p->current.type == TOK_STAR || p->current.type == TOK_SLASH) {
        char op = (p->current.type == TOK_STAR) ? '*' : '/';
        parser_advance(p);

        Ast* right = parse_factor(p);
        if (!right) return NULL; 

        left = new_bin_op_ast(left, right, op);
    }
    
    return left;
}

Ast* parse_expr(Parser* p) {
    Ast* left = parse_term(p);
    if (!left) return NULL;

    while (p->current.type == TOK_PLUS || p->current.type == TOK_MINUS) {
        char op = (p->current.type == TOK_PLUS) ? '+' : '-';
        parser_advance(p);

        Ast* right = parse_term(p);
        if (!right) return NULL; // ошибка синтаксиса

        left = new_bin_op_ast(left, right, op);
    }
    
    return left;
}



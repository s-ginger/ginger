#include "core.h"
#include "parser.h"
#include "ast.h"
#include "tokens.h"
#include <stdlib.h>

void parser_init(Parser *p, Lexer *lexer) {
    p->lexer = lexer;
    p->current = lexer_next(lexer);
    p->next = lexer_next(lexer);
}

Token parser_advance(Parser *p) {
    p->current = p->next;
    p->next = lexer_next(p->lexer);
    return p->current;
}

Ast *parse_factor(Parser *p) {
    if (p->current.type == TOK_LPAREN) {
        parser_advance(p);
        Ast *ast = parse_expr(p);
        if (p->current.type != TOK_RPAREN)
            return NULL;
        parser_advance(p);
        return ast;
    }

    if (p->current.type == TOK_IDENT) {
        Ast *ident_ast = new_ident_ast(p->current.start, p->current.length);
        parser_advance(p);
        return ident_ast;
    }

    if (p->current.type != TOK_INT) {
        return NULL; // ошибка
    }

    Ast *node = new_int_ast(atoi(p->current.start));
    parser_advance(p); // следующий токен
    return node;
}

Ast *parse_term(Parser *p) {
    Ast *left = parse_factor(p);
    if (!left)
        return NULL;

    while (p->current.type == TOK_STAR || p->current.type == TOK_SLASH) {
        char op = (p->current.type == TOK_STAR) ? '*' : '/';
        parser_advance(p);

        Ast *right = parse_factor(p);
        if (!right)
            return NULL;

        left = new_bin_op_ast(left, right, op);
    }

    return left;
}

Ast *parse_expr(Parser *p) {
    Ast *left = parse_term(p);
    if (!left)
        return NULL;

    while (p->current.type == TOK_PLUS || p->current.type == TOK_MINUS) {
        char op = (p->current.type == TOK_PLUS) ? '+' : '-';
        parser_advance(p);

        Ast *right = parse_term(p);
        if (!right)
            return NULL; // ошибка синтаксиса

        left = new_bin_op_ast(left, right, op);
    }

    return left;
}

static Stmt *parse_var_stmt(Parser *p) {
    parser_advance(p);

    if (p->current.type != TOK_IDENT)
        return NULL;

    const char *name = p->current.start;
    size_t len = p->current.length;
    parser_advance(p);

    Ast *value = NULL;

    if (p->current.type == TOK_ASSIGN) {
        parser_advance(p);
        value = parse_expr(p);
        if (!value)
            return NULL;
    }

    return new_var_stmt(name, len, value);
}

static Stmt *parse_const_stmt(Parser *p) {
    parser_advance(p);

    if (p->current.type != TOK_IDENT)
        return NULL;

    const char *name = p->current.start;
    size_t len = p->current.length;
    parser_advance(p);

    Ast *value = NULL;

    if (p->current.type == TOK_ASSIGN) {
        parser_advance(p);
        value = parse_expr(p);
        if (!value)
            return NULL;
    }

    return new_const_stmt(name, len, value);
}

static Stmt *parse_assign(Parser *p) {
    Ast *ident = parse_factor(p);

    if (p->current.type != TOK_ASSIGN)
        return NULL;
    else
        parser_advance(p);

    Ast *expr = parse_expr(p);

    return new_assign_stmt(ident->ident.name, ident->ident.length, expr);
}

Stmt *parse_stmt(Parser *p) {
    Stmt *stmt = NULL;

    if (p->current.type == TOK_VAR) {
        stmt = parse_var_stmt(p);
    } else if (p->current.type == TOK_CONST) {
        stmt = parse_const_stmt(p);
    } else if (p->current.type == TOK_IDENT) {
        if (p->next.type == TOK_ASSIGN)
            stmt = parse_assign(p);
        
        

    } else {
        stmt = new_expr_stmt(parse_expr(p));
    }

    if (!stmt)
        return NULL;

    if (p->current.type != TOK_NEWLINE)
        return NULL; // ошибка: ожидался ;

    parser_advance(p); // съели ;

    return stmt;
}

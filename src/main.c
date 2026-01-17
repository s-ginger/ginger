#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "lexer.h"
#include "parser.h"


int main(int argc, char const *argv[])
{
    const char *src = "123 + 456";

    // токен числа "123"
    Token t1 = new_token(TOK_INT, src, 3);

    // токен плюса "+"
    Token t2 = new_token(TOK_PLUS, src + 4, 1);

    // печатаем токены
    printf("t1: %.*s\n", (int)t1.length, t1.start); // 123
    printf("t2: %.*s\n", (int)t2.length, t2.start); // +

    Lexer l = {"123 + (123 - x)", 0};

    Parser* parser = malloc(sizeof(Parser));
    parser_init(parser, &l);

    Ast *ast = parse_expr(parser);

    if (ast)
    {
        printf("AST is completed\n");
    }
    else
    {
        printf("Error.\n");
    }

    return 0;
}

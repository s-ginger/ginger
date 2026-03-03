#include "lexer.h"
#include "parser.h"
#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *src = "123 + 456";

    // токен числа "123"
    Token t1 = new_token(TOK_INT, src, 3);

    // токен плюса "+"
    Token t2 = new_token(TOK_PLUS, src + 4, 1);

    // печатаем токены
    printf("t1: %.*s\n", (i32)t1.length, t1.start); // 123
    printf("t2: %.*s\n", (i32)t2.length, t2.start); // +

    Lexer l = {"x = 12 + 23\n", 0};

    Parser *parser = cast<Parser *>(malloc(sizeof(Parser)));
    parser_init(parser, &l);

    Stmt *stmt = parse_stmt(parser);

    
    

    if (stmt) {
        printf("AST is completed\n");
        print_stmt(stmt, 0);
    } else {
        printf("Error.\n");
    }

    free_stmt(stmt);

    return 0;
}

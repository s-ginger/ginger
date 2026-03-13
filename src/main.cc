#include "lexer.h"
#include "parser.h"
#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Создаём арену для AST/Stmt
    Arena *arena = arena_create(1024 * 1024); // 1 MB, можно больше

    Lexer lexer = {"package main \n import \"base\" \n var a = 10 + 12 * 6 \n const p = 10", 0};
    Parser parser;

    parser_init(&parser, &lexer, arena); // передаём арену в парсер

    Stmt **program = parse_program(&parser);

    print_program(program);

    arena_destroy(arena); // освобождаем память в конце
    return 0;
}
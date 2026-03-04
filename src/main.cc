#include "lexer.h"
#include "parser.h"
#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  Lexer l = {"package main\n", 0};

  Parser parser;
  parser_init(&parser, &l);

  Stmt *stmt = parse_stmt(&parser);

  if (stmt) {
    printf("AST is completed\n");
    print_stmt(stmt, 0);
  } else {
    printf("Error.\n");
  }

  free_stmt(stmt);

  return 0;
}

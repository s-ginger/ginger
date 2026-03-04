#ifndef LEXER_H
#define LEXER_H
#include "core.h"
#include "tokens.h"

typedef struct Lexer {
  const char *src;
  i32 pos;
} Lexer;

char lexer_advance(Lexer *l);
char lexer_peek(Lexer *l);
Token lexer_next(Lexer *l);

#endif
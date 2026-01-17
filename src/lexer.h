#ifndef LEXER_H
#define LEXER_H
#include "tokens.h"

typedef struct Lexer
{
    const char* src;
    int pos; 
} Lexer;

char lexer_advance(Lexer* l);
char lexer_peek(Lexer* l);
Token lexer_next(Lexer* l);

#endif
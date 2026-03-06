#include "tokens.h"
#include <stdio.h>

Token new_token_at(TokenType type, const char *start, usize len, usize line, usize column) {
    Token t;
    t.type = type;
    t.start = start;
    t.length = len;
    t.line = line;
    t.column = column;
    return t;
}
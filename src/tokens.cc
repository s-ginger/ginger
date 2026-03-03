#include "tokens.h"
#include <stdio.h>


Token new_token(TokenType type, const char* start, usize length) {
    Token t;
    t.type = type;
    t.start = start;
    t.length = length;
    return t;
}

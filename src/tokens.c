#include "tokens.h"
#include <stdio.h>


Token new_token(int type, const char* start, size_t length) {
    Token t;
    t.type = type;
    t.start = start;
    t.length = length;
    return t;
}

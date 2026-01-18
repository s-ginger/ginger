#ifndef TOKENS_H
#define TOKENS_H

typedef enum {

    TOK_IDENT,
    TOK_INT,        // 12

    TOK_PLUS,       // +
    TOK_MINUS,      // -
    TOK_STAR,       // *
    TOK_SLASH,      // /
    TOK_ASSIGN,
    
    TOK_LPAREN,     // (
    TOK_RPAREN,     // )

    TOK_LBRACE,     // {
    TOK_RBRACE,     // }
    
    TOK_COMMA,      // ,
    TOK_SEMICOLON,  // ;

    TOK_VAR,        // var
    TOK_CONST,      // const

    TOK_EOF,
    TOK_UNKNOWN

} TokenType;

typedef struct Token
{
    TokenType type;
    const char* start;
    size_t length;
} Token;

Token new_token(int type, const char* start, size_t length);


#endif 
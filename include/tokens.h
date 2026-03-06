#ifndef TOKENS_H
#define TOKENS_H
#include "core.h"

typedef enum {

  TOK_IDENT,
  TOK_INT,    // 12
  TOK_FLOAT,  // 12.12
  TOK_STRING, // "Hello world"

  TOK_PLUS,   // +
  TOK_MINUS,  // -
  TOK_STAR,   // *
  TOK_SLASH,  // /
  TOK_ASSIGN, // =

  TOK_LPAREN, // (
  TOK_RPAREN, // )

  TOK_LBRACE, // {
  TOK_RBRACE, // }

  TOK_COMMA,       // ,
  TOK_COLON,       // :
  TOK_COLONCOLON,  // ::
  TOK_SEMICOLON,   // ;
  TOK_SHORTASSIGN, // :=

  TOK_VAR,
  TOK_CONST,
  TOK_PACKAGE,
  TOK_IMPORT,
  TOK_PROC,

  TOK_NEWLINE,
  TOK_EOF,
  TOK_UNKNOWN,
  TOK_ILLEGAL

} TokenType;

typedef struct Token {
  TokenType type;
  const char *start;
  usize length;
  usize line;
  usize column;
} Token;

Token new_token_at(TokenType type, const char *start, usize len, usize line, usize column);
#endif
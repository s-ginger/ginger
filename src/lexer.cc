#include "lexer.h"
#include "core.h"
#include <ctype.h> // для isspace
#include <string.h>

// advance на 1 символ
char lexer_advance(Lexer *l) { return l->src[l->pos++]; }

// посмотреть следующий символ
char lexer_peek(Lexer *l) {
  if (l->src[l->pos] == '\0')
    return '\0';
  return l->src[l->pos];
}

static b8 is_digit(char c) { return c >= '0' && c <= '9'; }

static b8 is_letter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static b8 is_space(char c) { return (c != '\n') && isspace(cast<u32>(c)); }

// вернуть следующий токен
Token lexer_next(Lexer *l) {
  // Пропускаем пробелы
  while (lexer_peek(l) != '\0' && is_space(cast<i8>(lexer_peek(l)))) {
    lexer_advance(l);
  }

  usize start = l->pos;
  char c = lexer_advance(l);

  if (c == '\0') {
    return new_token(TOK_EOF, l->src + start, 0); // длина 0 для EOF
  }

  if (is_digit(c)) {
    while (is_digit(lexer_peek(l)))
      lexer_advance(l);
    usize len = l->pos - start;
    return new_token(TOK_INT, l->src + start, len);
  }

  // Идентификаторы
  if (is_letter(c)) {
    while (is_letter(lexer_peek(l)) || is_digit(lexer_peek(l)))
      lexer_advance(l);

    usize len = l->pos - start;
    const char *text = l->src + start;

    if (len == 3 && strncmp(text, "var", 3) == 0)
      return new_token(TOK_VAR, text, len);

    if (len == 5 && strncmp(text, "const", 5) == 0)
      return new_token(TOK_CONST, text, len);

    if (len == 7 && strncmp(text, "package", 7) == 0)
      return new_token(TOK_PACKAGE, text, len);

    return new_token(TOK_IDENT, text, len);
  }

  // Операторы и символы
  switch (c) {
  case '+':
    return new_token(TOK_PLUS, l->src + start, 1);
  case '-':
    return new_token(TOK_MINUS, l->src + start, 1);
  case '*':
    return new_token(TOK_STAR, l->src + start, 1);
  case '/':
    return new_token(TOK_SLASH, l->src + start, 1);

  case '(':
    return new_token(TOK_LPAREN, l->src + start, 1);
  case ')':
    return new_token(TOK_RPAREN, l->src + start, 1);

  case '{':
    return new_token(TOK_LBRACE, l->src + start, 1);
  case '}':
    return new_token(TOK_RBRACE, l->src + start, 1);
  case ':': {
    char next = lexer_peek(l);

    if (next == '=') {
      lexer_advance(l);
      return new_token(TOK_SHORTASSIGN, l->src + start, 2); // :=
    }

    if (next == ':') {
      lexer_advance(l);
      return new_token(TOK_COLONCOLON, l->src + start, 2); // ::
    }

    return new_token(TOK_COLON, l->src + start, 1); // :
  }
  case '=':
    return new_token(TOK_ASSIGN, l->src + start, 1);
  case ';':
    return new_token(TOK_SEMICOLON, l->src + start, 1);
  case '\n':
    return new_token(TOK_NEWLINE, l->src + start, 1);
  default:
    return new_token(TOK_UNKNOWN, l->src + start, 1);
  }
}

#include "lexer.h"
#include <ctype.h> // для isspace
#include <string.h>

// advance на 1 символ
char lexer_advance(Lexer *l)
{
    return l->src[l->pos++];
}

// посмотреть следующий символ
char lexer_peek(Lexer *l)
{
    if (l->src[l->pos] == '\0')
        return '\0';
    return l->src[l->pos];
}

static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static int is_letter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

// вернуть следующий токен
Token lexer_next(Lexer *l)
{
    // Пропускаем пробелы
    while (lexer_peek(l) != '\0' && isspace((unsigned char)lexer_peek(l)))
    {
        lexer_advance(l);
    }

    size_t start = l->pos;
    char c = lexer_advance(l);

    if (c == '\0')
    {
        return new_token(TOK_EOF, l->src + start, 0); // длина 0 для EOF
    }

    if (is_digit(c))
    {
        while (is_digit(lexer_peek(l)))
            lexer_advance(l);
        size_t len = l->pos - start;
        return new_token(TOK_INT, l->src + start, len);
    }

    // Идентификаторы
    if (is_letter(c))
    {
        while (is_letter(lexer_peek(l)) || is_digit(lexer_peek(l)))
            lexer_advance(l);

        size_t len = l->pos - start;
        const char *text = l->src + start;

        if (len == 3 && strncmp(text, "var", 3) == 0)
            return new_token(TOK_VAR, text, len);

        if (len == 5 && strncmp(text, "const", 5) == 0)
            return new_token(TOK_CONST, text, len);

        return new_token(TOK_IDENT, text, len);
    }

    // Операторы и символы
    switch (c)
    {
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

    case '=':
        return new_token(TOK_ASSIGN, l->src + start, 1);
    case ';':
        return new_token(TOK_SEMICOLON, l->src + start, 1);

    default:
        return new_token(TOK_UNKNOWN, l->src + start, 1);
    }
}

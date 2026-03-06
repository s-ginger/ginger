#include "lexer.h"
#include "core.h"
#include <ctype.h> // для isspace
#include <string.h>

// advance на 1 символ
char lexer_advance(Lexer *l) {
    char c = l->src[l->pos++];
    if (c == '\n') {
        l->line++;
        l->column = 1;
    } else {
        l->column++;
    }
    return c;
}

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
    // пропуск пробелов
    while (lexer_peek(l) != '\0' && is_space(cast<i8>(lexer_peek(l)))) {
        lexer_advance(l);
    }

    usize start = l->pos;
    usize token_line = l->line;     // сохраняем линию начала токена
    usize token_column = l->column; // сохраняем колонку начала токена

    char c = lexer_advance(l);

    if (c == '\0') {
        return new_token_at(TOK_EOF, l->src + start, 0, token_line,
                            token_column);
    }

    // строки
    if (c == '\"') {
        while (lexer_peek(l) != '\0') {
            char ch = lexer_advance(l);
            if (ch == '\\') {
                lexer_advance(l); // пропускаем экранированный символ
            } else if (ch == '\"') {
                usize len = l->pos - start;
                return new_token_at(TOK_STRING, l->src + start, len, token_line,
                                    token_column);
            }
        }
        usize len = l->pos - start;
        return new_token_at(TOK_ILLEGAL, l->src + start, len, token_line,
                            token_column);
    }

    // числа
    if (is_digit(c)) {
        while (is_digit(lexer_peek(l)))
            lexer_advance(l);
        usize len = l->pos - start;
        return new_token_at(TOK_INT, l->src + start, len, token_line,
                            token_column);
    }

    // идентификаторы
    if (is_letter(c)) {
        while (is_letter(lexer_peek(l)) || is_digit(lexer_peek(l)))
            lexer_advance(l);

        usize len = l->pos - start;
        const char *text = l->src + start;

        if (len == 3 && strncmp(text, "var", 3) == 0)
            return new_token_at(TOK_VAR, text, len, token_line, token_column);
        if (len == 5 && strncmp(text, "const", 5) == 0)
            return new_token_at(TOK_CONST, text, len, token_line, token_column);
        if (len == 6 && strncmp(text, "import", 6) == 0)
            return new_token_at(TOK_IMPORT, text, len, token_line,
                                token_column);
        if (len == 7 && strncmp(text, "package", 7) == 0)
            return new_token_at(TOK_PACKAGE, text, len, token_line,
                                token_column);

        return new_token_at(TOK_IDENT, text, len, token_line, token_column);
    }

    // операторы и символы
    switch (c) {
    case '+':
        return new_token_at(TOK_PLUS, l->src + start, 1, token_line,
                            token_column);
    case '-':
        return new_token_at(TOK_MINUS, l->src + start, 1, token_line,
                            token_column);
    case '*':
        return new_token_at(TOK_STAR, l->src + start, 1, token_line,
                            token_column);
    case '/':
        return new_token_at(TOK_SLASH, l->src + start, 1, token_line,
                            token_column);

    case '(':
        return new_token_at(TOK_LPAREN, l->src + start, 1, token_line,
                            token_column);
    case ')':
        return new_token_at(TOK_RPAREN, l->src + start, 1, token_line,
                            token_column);
    case '{':
        return new_token_at(TOK_LBRACE, l->src + start, 1, token_line,
                            token_column);
    case '}':
        return new_token_at(TOK_RBRACE, l->src + start, 1, token_line,
                            token_column);

    case ':': {
        char next = lexer_peek(l);
        if (next == '=') {
            lexer_advance(l);
            return new_token_at(TOK_SHORTASSIGN, l->src + start, 2, token_line,
                                token_column);
        }
        if (next == ':') {
            lexer_advance(l);
            return new_token_at(TOK_COLONCOLON, l->src + start, 2, token_line,
                                token_column);
        }
        return new_token_at(TOK_COLON, l->src + start, 1, token_line,
                            token_column);
    }

    case '=':
        return new_token_at(TOK_ASSIGN, l->src + start, 1, token_line,
                            token_column);
    case ';':
        return new_token_at(TOK_SEMICOLON, l->src + start, 1, token_line,
                            token_column);
    case '\n':
        return new_token_at(TOK_NEWLINE, l->src + start, 1, token_line,
                            token_column);

    default:
        return new_token_at(TOK_UNKNOWN, l->src + start, 1, token_line,
                            token_column);
    }
}
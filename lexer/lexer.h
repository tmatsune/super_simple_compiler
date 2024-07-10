#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>
#include "utils.h"

struct lexer {
    char *buffer;
    int buffer_size;
    int pos;
    char chr;
};

typedef enum {
    IDENT,
    LABEL,
    INT,
    INPUT,
    OUTPUT,
    GOTO,
    IF,
    THEN,
    EQUAL,
    PLUS,
    LESS_THAN,
    INVALID,
    END
} token_type;

typedef struct {
    token_type tok_type;
    char *literal;
} token;


void lexer_init(struct lexer *l, char *buffer, int buffer_size);
void lexer_tokenize(struct lexer *l, dynamic_array *array);
void display_token(const token tok);
void display_token_ptr(const token *tok);
char *token_type_str(token_type tok);

#endif
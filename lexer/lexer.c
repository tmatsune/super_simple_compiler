#include "lexer.h"
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --------------- helper functions -------------- // 
static bool in_bounds(struct lexer *l){ return l->pos < l->buffer_size; }
static void advc_pos(struct lexer *l){ l->pos++; }
static void read_char(struct lexer *l){ l->chr = l->buffer[l->pos]; }

static void skip_whitespace(struct lexer *l){
    while(in_bounds(l) && isspace(l->chr)){
        advc_pos(l);
        read_char(l);
    }
}

// peek funcs
/*
static bool peek_in_bounds(struct lexer *l) { return l->pos + 1 < l->buffer_size; }
static char lexer_peek_chr(struct lexer *l){
    if(peek_in_bounds(l)) return l->buffer[l->pos + 1];
    return '\0';
}
*/

static void lexer_get_num(struct lexer *l, string_slice *slice){
    while(in_bounds(l) && isdigit(l->chr)){
        slice_append(slice, l->chr);
        advc_pos(l);
        read_char(l);
    }
}

static void lexer_get_word(struct lexer *l, string_slice *slice){
    while (in_bounds(l) && (isalnum(l->chr) || l->chr == '_')){
        slice_append(slice, l->chr);
        advc_pos(l);
        read_char(l);
    }
}

static token literal_to_token(char *literal){
    if (strcmp(literal, "input") == 0){
        return (token){.tok_type = INPUT, .literal = NULL};
    }else if(strcmp(literal, "output") == 0){
        return (token){.tok_type = OUTPUT, .literal = NULL};
    }else if(strcmp(literal, "goto") == 0){
        return (token){.tok_type = GOTO, .literal = NULL};
    }else if(strcmp(literal, "if") == 0){
        return (token){.tok_type = IF, .literal = NULL};
    }else if(strcmp(literal, "then") == 0){
        return (token){.tok_type = THEN, .literal = NULL};
    }else {
        return (token){.tok_type = IDENT, .literal = literal};
    }
}


// --------------- lexer main functions -------------- //

static token get_token(struct lexer *l){
    read_char(l);
    skip_whitespace(l);
    read_char(l);
    if(l->chr == '\0'){
        advc_pos(l);
        return (token){.tok_type=END, .literal=NULL};
    }else if(l->chr == '='){
        advc_pos(l);
        return (token){.tok_type=EQUAL, .literal=NULL};
    }else if(l->chr == '+'){
        advc_pos(l);
        return (token){.tok_type=PLUS, .literal=NULL};
    }else if(l->chr == '<'){
        advc_pos(l);
        return (token){.tok_type=LESS_THAN, .literal=NULL};
    }else if(l->chr == ':'){
        advc_pos(l);
        read_char(l);
        string_slice slice;
        slice_init(&slice);
        lexer_get_word(l, &slice);
        char *literal = slice_get(&slice);
        return (token){.tok_type=LABEL, .literal=literal};

    }else if(isdigit(l->chr)){

        string_slice slice;
        slice_init(&slice);
        lexer_get_num(l, &slice);
        char *literal = slice_get(&slice);
        return (token){.tok_type=INT, .literal=literal};
        
    }else if(isalpha(l->chr) || l->chr == '_'){

        string_slice slice;
        slice_init(&slice);
        lexer_get_word(l, &slice);
        char *literal = slice_get(&slice);
        token tok = literal_to_token(literal);
        return tok;

    }else{
        return (token){.tok_type=INVALID, .literal=NULL};
    }
}

void lexer_tokenize(struct lexer *l, dynamic_array *array){
    int count = 100;
    while(l->chr != '\0'){
        token tok = get_token(l);
        array_append(array, &tok, sizeof(token));
        count--;
        if(count < 0){
            printf("FAILED");
            exit(EXIT_FAILURE);
        }
    }
}

void lexer_init(struct lexer *l, char *buffer, int buffer_size){
    l->buffer = buffer;
    l->buffer_size = buffer_size;
    l->pos = 0;
    read_char(l);
};

void display_token(const token tok){
    printf("{ %s ", token_type_str(tok.tok_type));
    printf(" ( %s )  }\n", tok.literal ? tok.literal : "NULL");
}
void display_token_ptr(const token *tok){
    printf("{ %s ", token_type_str(tok->tok_type));
    printf(" ( %s )  }\n", tok->literal ? tok->literal : "NULL");
}

char *token_type_str(token_type tok){
    switch(tok){
        case IDENT: return "identify";
        case LABEL: return "label";
        case INT: return "int";
        case INPUT: return "input";
        case OUTPUT: return "output";
        case GOTO: return "goto";
        case IF: return "if";
        case THEN: return "then";
        case EQUAL: return "equal";
        case PLUS: return "plus";
        case LESS_THAN: return "less than";
        case INVALID: return "invalid";
        case END: return "end";
    }
    return "error";
}

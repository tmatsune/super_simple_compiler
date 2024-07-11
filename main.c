#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "lexer.h"
#include "parser.h"
#include "assembler.h"


int main(int argc, char **argv){
    if(argc < 2){
        printf("not rnough arguments, no file given");
        exit(EXIT_FAILURE);
    }
    int file_size = 0;
    char *buffer = get_file(argv[1], &file_size);
    
    struct lexer l;
    dynamic_array tokens;
    array_init(&tokens);
    lexer_init(&l, buffer, file_size);
    
    lexer_tokenize(&l, &tokens);
    
    /*
    token *last_token = (token*)array_get(&tokens, tokens.count - 2);
    display_token_ptr(last_token);
    */
    for(int i = 0; i < tokens.count; i++){
        token *tok = (token*)array_get(&tokens, i);
        display_token_ptr(tok);
    }

    struct parser p;
    parser_init(&p, tokens);
    struct program_node program;
    parse_program(&p, &program);

    program_asm(&program);

    return 0;
}

/*

    token *tok;
    for (int i = 0; i < tokens.count; i++){
        tok = (token *)array_get(&tokens, i);
        printf("index: %d ", i);
        display_token_ptr(tok);
    }
    printf("\n");


struct instruction_node *instruct;
for(int i = 0; i < program.instructions.count; i++){
    instruct = (struct instruction_node *)array_get(&program.instructions, i);
    switch(instruct->type){
        case INSTR_ASSIGN: {
            printf("ASSIGN {");
            printf("Identier: %s", instruct->assign.identifier);
            printf("}");
            endl();
        }
            break;
        case INSTR_GOTO:
            printf("GOTO");
            endl();
            break;
        case INSTR_IF:
            printf("IF");
            endl();
            break;
        case INSTR_LABEL:
            printf("LABEL");
            endl();
            break;
        case INSTR_OUTPUT:
            printf("OUTPUT");
            endl();
            break;
    }
}
*/
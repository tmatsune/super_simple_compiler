#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"

static void advance_parser(struct parser *p){ p->index++; };
static void parser_current_token(struct parser *p, token **tok){ *tok = (token*)array_get(&p->tokens, p->index); }
static void *parser_get_current(struct parser *p) { return (token *)array_get(&p->tokens, p->index); }
/*

static bool parser_in_bounds(struct parser *p){ return p->index < p->tokens.count; }
*/

// ---------------- creating AST functions ------------------ //

static void parse_term(struct parser *p, struct term_node *term){
    token *token;
    parser_current_token(p, &token);
    if(token->tok_type == INPUT){
        term->type = TERM_INPUT;
    }else if(token->tok_type == INT){
        term->type = TERM_INT;
        term->value = token->literal;
    }else if(token->tok_type == IDENT){
        term->type = TERM_IDENT;
        term->value = token->literal;
    }else{
        printf("error getting term, not valid term\n");
        exit(EXIT_FAILURE);
    }
    advance_parser(p);

}

static void parse_expression(struct parser *p, struct expression_node *expression){
    token *token;
    struct term_node left, right;
    parse_term(p, &left); // gets left the left side of the expression and advaneces the parser 

    parser_current_token(p, &token);

    if(token->tok_type == PLUS){
        advance_parser(p);
        parse_term(p, &right);

        expression->type = EXPRESSION_PLUS;
        expression->add.left = left;
        expression->add.right = right;
    }else{
        expression->type = EXPRESSION_TERM;
        expression->term = left;
    }
}

static void parse_relation(struct parser *p, struct relation_node *relate){
    token *tok;
    struct term_node left, right;
    parse_term(p, &left);

    parser_current_token(p, &tok);

    if(tok->tok_type == LESS_THAN){
        advance_parser(p);
        parse_term(p, &right);

        relate->type = REL_LESS_THAN;
        relate->less_than.left = left;
        relate->less_than.right = right;
    }else{
        printf("error no compare operator found \n");
        exit(EXIT_FAILURE);
    }

}

//Expects an identifier followed by an EQUAL sign and an expression. Sets up the instruction node with the parsed identifier and expression.
static void parse_assign(struct parser *p, struct instruction_node *instruct){
    token *tok;
    instruct->type = INSTR_ASSIGN;

    parser_current_token(p, &tok);
    instruct->assign.identifier = tok->literal;
    advance_parser(p);

    parser_current_token(p, &tok);
    if(tok->tok_type != EQUAL){
        printf("expected EQUAL\n");
        exit(EXIT_FAILURE);
    }
    advance_parser(p);

    parse_expression(p, &instruct->assign.expression);

    printf("token type: %d, parser index: %d \n", tok->tok_type, p->index);
}

static void parse_instruction(struct parser *p, struct instruction_node *instruct);

// Sets up the instruction node with the parsed relational expression and nested instruction.
static void parse_if(struct parser *p, struct instruction_node *instruct){
    token *tok;
    instruct->type = INSTR_IF;
    advance_parser(p);

    parse_relation(p, &instruct->if_.relation); // TODO 
    parser_current_token(p, &tok);
    if(tok->tok_type != THEN){
        printf("did not get then token\n");
        exit(EXIT_FAILURE);
    }
    advance_parser(p);

    instruct->if_.intsruct = malloc(sizeof(struct instruction_node));
    parse_instruction(p, instruct->if_.intsruct);
}

// Expects a LABEL token and sets up the instruction node with the parsed label 
static void parse_goto(struct parser *p, struct instruction_node *instruct){
    token *tok;
    
    instruct->type = INSTR_GOTO;
    advance_parser(p);

    parser_current_token(p, &tok);
    if(tok->tok_type != LABEL){
        printf("expected label\n");
        exit(EXIT_FAILURE);
    }

    advance_parser(p);
    instruct->goto_.label = tok->literal;
}

// parses an output statement, Expects a term and sets up the instruction node with the parsed term
static void parse_output(struct parser *p, struct instruction_node *instruct){
    token *tok;
    struct term_node left;

    instruct->type = INSTR_OUTPUT;
    advance_parser(p);

    parse_term(p, &left);
    instruct->output.term = left;
}

// Expects a label and sets up the instruction node with the parsed label.
static void parse_label(struct parser *p, struct instruction_node *instruct){
    token *tok;

    instruct->type = INSTR_LABEL;

    parser_current_token(p, &tok);
    instruct->label.label = tok->literal;

    advance_parser(p);
}

static void parse_instruction(struct parser *p, struct instruction_node *instruct){
    token *tok;
    parser_current_token(p, &tok);

    printf("insutrction token: ");
    display_token_ptr(tok);

    if (tok->tok_type == IDENT) {
        printf("IDENTIFY INSTRUCTION\n");
        parse_assign(p, instruct);
    } else if (tok->tok_type == IF) {
        printf("IF INSTRUCTION\n");
        parse_if(p, instruct);
    } else if (tok->tok_type == GOTO) {
        printf("GOTO INSTRUCTION\n");
        parse_goto(p, instruct);
    } else if (tok->tok_type == OUTPUT) {
        printf("OUTPUT INSTRUCTION\n");
        parse_output(p, instruct);
    } else if (tok->tok_type == LABEL) {
        printf("LABEL INSTRUCTION\n");
        parse_label(p, instruct);
    } else {
        printf("ERR parse intruction not found: %d\n", tok->tok_type);
        exit(EXIT_FAILURE);
    }
    endl();

}

void parse_program(struct parser *p, struct program_node *program){

    array_init(&program->instructions);
    token *tok;
    parser_current_token(p, &tok);
    while(tok->tok_type != END){

        // get instruction from tokens 
        struct instruction_node instruct;
        parse_instruction(p, &instruct);
        array_append(&program->instructions, &instruct, sizeof(struct instruction_node));
        parser_current_token(p, &tok);

    }

}

void parser_init(struct parser *p, dynamic_array tokens){
    p->tokens = tokens;
    p->index = 0;
}


/*

    token *tok;
    parser_current_token(p, &tok);
    int count = 50;
    while(tok->tok_type != END){
        display_token_ptr(tok);
        advance_parser(p);
        parser_current_token(p, &tok);
        count--;
        if(count < 0) break;
    }


    token *tok = NULL;
    parser_current(p, &tok);
    if(tok){
        display_token_ptr(tok);
    }else printf("error getting token \n");
*/
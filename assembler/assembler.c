#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "utils.h"
#include "parser.h"

// ----------------------------- declare variables ------------------------------ //

static void term_declare_variable(struct term_node *term, dynamic_array *variables){
    switch(term->type){
        case TERM_INPUT: {
            break;
        }
        case TERM_INT: {
            break;
        }
        case TERM_IDENT: {
            for(int i = 0; i < variables->count; i++){
                char *variable = (char *)array_get(variables, i);
                if (strcmp(term->value, variable) == 0){
                    return;
                }
            }
            break;
        }
    }
}

static void expression_declare_variable(struct expression_node *expression, dynamic_array *variables){
    switch(expression->type){
        case EXPRESSION_TERM: {
            term_declare_variable(&expression->term, variables);
            break;
        }
        case EXPRESSION_PLUS: {
            term_declare_variable(&expression->add.left, variables);
            term_declare_variable(&expression->add.right, variables);
            break;
        }
    }
}

static void relation_declare_variable(struct relation_node *relation, dynamic_array *variables){
    switch(relation->type){
        case REL_LESS_THAN: {
            term_declare_variable(&relation->less_than.left, variables);
            term_declare_variable(&relation->less_than.right, variables);
            break;
        }
    }
}

static void instruction_declare_variable(struct instruction_node *instruction, dynamic_array *variables){
    switch(instruction->type){
        case INSTR_ASSIGN: {   
            // This ensures that all variables used in the expression being assigned are declared
            expression_declare_variable(&instruction->assign.expression, variables);
            for (unsigned int i = 0; i < variables->count; i++){
                char *variable = NULL;
                array_get(variables, i);
                if (strcmp(instruction->assign.identifier, variable) == 0) {
                    return;
                }
            }
            array_append(variables, &instruction->assign.identifier, sizeof(char *));
            break;
        }
        case INSTR_IF: {

            break;
        }
        case INSTR_GOTO: {
            
            break;
        }
        case INSTR_OUTPUT: {

            break;
        }
        case INSTR_LABEL: {

            break;
        }
        default:
            printf("INSTRUCT not found\n");
            exit(EXIT_FAILURE);
    
    }
}

// ------------------------------ main assemblerfuncion ------------------------------ //

void program_asm(struct program_node *program){

    dynamic_array variables;
    array_init(&variables);

    struct instruction_node *instruction;
    for(int i = 0; i < program->instructions.count; i++){
        instruction = (struct instruction_node *)array_get(&program->instructions, i);
        instruction_declare_variable(instruction, &variables);
    }

}

void test_assembler(void){ printf("assember lib \n"); }
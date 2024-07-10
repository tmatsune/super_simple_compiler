#ifndef PARSER_H
#define PARSER_H 
#include "utils.h"
#include "lexer.h"

// ------------- term node ------------ //

enum term_type{
    TERM_INPUT, 
    TERM_INT, 
    TERM_IDENT
};

struct term_node {
    enum term_type type;
    union {
        char *value;
    };
};

struct term_binary_node{
    struct term_node left;
    struct term_node right;
};

// ----------- expression node ---------- //

enum expression_type{
    EXPRESSION_PLUS,
    EXPRESSION_TERM,
};

struct expression_node {
    enum expression_type type;
    union {
        struct term_node term;
        struct term_binary_node add;
    };
};

// ----------- relation node ---------- //

enum relation_type {
    REL_LESS_THAN,
};

struct relation_node{
    enum relation_type type;
    union {
        struct term_binary_node less_than;
    };
};

// ----------- instruction node ---------- //

enum instruction_type{
    INSTR_ASSIGN,
    INSTR_IF,
    INSTR_GOTO,
    INSTR_OUTPUT,
    INSTR_LABEL
} ;

struct assign_node{
    char *identifier;
    struct expression_node expression;
};

struct if_node {
    struct relation_node relation;
    struct instruct_node *intsruct; // needs to be ptr since it is defined before the intruction node
};

struct goto_node {
    char *label;
};

struct output_node {
    struct term_node term; // can only output a term for now 
};

struct label_node {
    char *label;
};

struct instruction_node{
    enum instruction_type type;
    union { // available instructions
        struct assign_node assign;
        struct if_node if_;
        struct goto_node goto_;
        struct output_node output;
        struct label_node label;
    };
    
} ;


// ----------- program node ---------- // 

struct program_node{
    dynamic_array instructions;
};

struct parser {
    dynamic_array tokens;
    int index;
};

void parser_init(struct parser *p, dynamic_array tokens);

void parse_program(struct parser *p, struct program_node *program);


#endif
#ifndef B6024EEC_47B1_43D8_A5E2_7E0CE27C1E65
#define B6024EEC_47B1_43D8_A5E2_7E0CE27C1E65

#define STACK_LEN 12   // *check that this is right length

#ifndef DECODE_H
#define DECODE_H

typedef struct Stack {
    int characters[STACK_LEN];    // stack array has int datatypes inside
    int top;
} Stack;

typedef struct string_table_el {
    int pair_code;
    int prefix_index;
    int k;       // making this int instead of a char
} string_table_el;

string_table_el *initialize_string_table();

Stack *initialize_stack();

void push(Stack *stack, int character);

char pop(Stack *stack);

int code_unknown(int c, string_table_el *string_table);

int stack_non_empty (Stack *stack);

int pref(string_table_el *string_table, int c);

int get_char(string_table_el *string_table, int c);

int get_code();

void insert_string_pair(string_table_el *string_table, int pair_code, int k);

void decode();



#endif

#endif /* B6024EEC_47B1_43D8_A5E2_7E0CE27C1E65 */
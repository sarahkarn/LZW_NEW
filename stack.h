#ifndef STACK_H
#define STACK_H

#define STACK_LEN 8


typedef struct Stack {
    int *characters;    // characters has pointers to int datatypes inside
    int top;
    int space;
} Stack;

Stack *initialize_stack();

void push(Stack *stack, int character);

char pop(Stack *stack);

int stack_non_empty (Stack *stack);



//void push(Stack *stack, char val);
//void initialize_stack(Stack *stack);
//int check_empty(Stack *stack);

#endif
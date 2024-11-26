#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


Stack *initialize_stack() {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  if (!stack) {
    fprintf(stderr, "Memory allocation failed inside new_stack");
    exit(1);
  }
  stack->characters = (int *)malloc(STACK_LEN * sizeof(int));
  if (!stack->characters) {
    fprintf(stderr, "Memory allocation failed when creating characters in "
                    "initialize_stack");
    free(stack);
    exit(1);
  }
  stack->top = -1; // initially the stack is empty
  stack->space = STACK_LEN;
  return stack;
}

void push(Stack *stack, int character) {
  if (stack->top == stack->space - 1) {
    stack->space *= 2; // get double the space
    stack->characters =
        (int *)realloc(stack->characters, stack->space * sizeof(int));
    if (!stack->characters) {
      fprintf(stderr, "Memory realloc failed inside push, space is %d/", stack->space);
      exit(1);
    }
  }

  if( 0 == stack->top % 100  ) {
    //fprintf(stderr,"DEPTH = %d\n", stack->top );
  }
  stack->top += 1;
  stack->characters[stack->top] = character;
  return;
}


char pop(Stack *stack) {
  if (stack->top == -1) {
    fprintf(stderr, "Stack underflow, cannot pop");
    assert(0); // return null terminator to indicate the stack was empty
  } else {
    char rval = stack->characters[stack->top];
    stack->top -= 1;
    // fprintf(stderr, "Popped %d from stack\n", (int)rval);
    return rval;
  }
}

int stack_non_empty(Stack *stack) {
  if (stack->top != -1) {
    return 1;
  }
  return 0;
}






/*void push(Stack *stack, char val) {
  if (stack->top >= (STACK_LEN - 1)) {
    fprintf(stderr,"Stack overflow - cannot push the value");
    exit(1);
  } else {
    int insert_index = stack->top++;
    stack->characters[insert_index] = val;
  }
}

char pop(Stack *stack) {
  if (check_empty(stack)) {
    fprintf(stderr,"Stack underflow - cannot pop any values");
    exit(1);
  } else {
    int pop_index = stack->top;
    stack->top--;
    return stack->characters[pop_index];
  }
}

void initialize_stack(Stack *stack) {
  stack->top = -1; // initially, empty stack has top value -1
}

int check_empty(Stack *stack) {
  return stack->top == -1; // check if the top of the stack has value -1
} */

#include "decode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRING_TABLE_LEN 4096 // 2 ^ 12 since max bit length is 12
#define EMPTY -1
#define UNKNOWN -2
#include <assert.h>

int last_code_assigned = 256;

string_table_el *initialize_string_table() {
  string_table_el *string_table =
      (string_table_el *)malloc(STRING_TABLE_LEN * sizeof(string_table_el));
  if (!string_table) {
    fprintf(stderr, "Memory allocation failed inside initialize_string_table");
    exit(1);
  }
  // first dealing with ASCII characters
  for (int i = 0; i < 128; i++) {
    string_table[i].prefix_index = EMPTY; // -1 will denote an empty prefix_index
    string_table[i].k = (char)i;
  }

  for (int i = 128; i < STRING_TABLE_LEN; i++) {
    string_table[i].prefix_index = EMPTY;
    string_table[i].k = UNKNOWN; // k will be initially set to -1
                            // if element is not within first 128
  }
  return string_table;
}

Stack *initialize_stack() {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  if (!stack) {
    fprintf(stderr, "Memory allocation failed inside new_stack");
    exit(1);
  }
  stack->top = -1; // initially the stack is empty
  return stack;
}

void push(Stack *stack, int character) {
  if (stack->top == STACK_LEN - 1) {
    fprintf(stderr, "Stack overflow, cannot push");
    assert(0);
  } else {
    stack->top += 1;
    stack->characters[stack->top] = character;
    fprintf(stderr, "Pushed %d onto stack\n", (int)character);
  }
  return;
}

char pop(Stack *stack) {
  if (stack->top == -1) {
    fprintf(stderr, "Stack underflow, cannot pop");
    assert(0); // return null terminator to indicate the stack was empty
  } else {
    char rval = stack->characters[stack->top];
    stack->top -= 1;
    fprintf(stderr, "Popped %d from stack\n", (int)rval);
    return rval;
  }
}


int stack_non_empty(Stack *stack) {
  if (stack->top != -1) {
    return 1;
  }
  return 0;
}

int pref(string_table_el *string_table, int c) {
  assert(c >= 0 && c < STRING_TABLE_LEN);
  int rval = string_table[c].prefix_index;
  assert(rval >= -1 && rval < STRING_TABLE_LEN);
  fprintf(stderr, "Calling pref: rval is: %d\n", rval);
  return rval;
}

int get_char(string_table_el *string_table, int c) {
  assert(c >= 0 && c < STRING_TABLE_LEN);
  return string_table[c].k;
}

int get_code() {
  int code;
  int scan_val = scanf("%d", &code);
  if (scan_val != 1) {
    return EOF;
  }
  return code;
}

void insert_string_pair(string_table_el *string_table, int pair_code, int k) {
  assert(pair_code >= 0 && pair_code < STRING_TABLE_LEN);
  assert(last_code_assigned >= 0 && last_code_assigned < STRING_TABLE_LEN);
  string_table[last_code_assigned].pair_code = pair_code;
  string_table[last_code_assigned].k = k;
  last_code_assigned++;
}


void decode() { // using left hand side implementation on page 7 of project
                // spec.
  // "Version where strings are inserted in table with UNKNOWN CHAR fields"
  string_table_el *string_table = initialize_string_table();
  fprintf(stderr, "string table initialized\n");
  Stack *stack = initialize_stack();
  fprintf(stderr, "stack initialized\n");


  int c;
  int newC;
  int final_k = EMPTY;

  while ((newC = c = get_code()) != EOF) {
    fprintf(stderr, "Reading code: %d\n", c);
    if (get_char(string_table, c) == UNKNOWN) {
      fprintf(stderr, "Code %d is unknown, pushing final_k: %d onto stack\n", c,
              final_k);
      push(stack, final_k);
      fprintf(stderr, "Location c: c = %d\n", c);
      c = pref(string_table, c);
      //ADDING HERE lines 126-128:
      //if (c == EMPTY) {
        //fprintf(stderr, "Found an invalid code: %d\n", c);
        //continue;
     // }
      fprintf(stderr, "Location d: c = %d\n", c);
    }
    while (pref(string_table, c) != EMPTY) { // issue here
      int s;
      s = get_char(string_table, c);
      fprintf(stderr, "Pushing char %d onto stack\n", s);
      push(stack, s);
      // push(stack, get_char(string_table, c)); // should be char(c)
      fprintf(stderr, "Location a: c = %d\n", c);
      c = pref(string_table, c);
      // ADDING HERE lines 141-143
      //if (c == EMPTY) {
        //fprintf(stderr, "Found an invalid code: %d\n", c);
        //break;
      //}
      fprintf(stderr, "Location b: c = %d\n", c);
    }
    fprintf(stderr, "about to get_char of %d\n", c);
    final_k = get_char(string_table, c);
    fprintf(stderr, "Final char (new char to output): %d\n", final_k);
    putchar(final_k);

    while (stack_non_empty(stack)) {
      int k;
      k = pop(stack);
      putchar(k);
    }

    if (get_char(string_table, last_code_assigned) == UNKNOWN) {
        string_table[last_code_assigned].k = final_k;
    }
    // insert
    //last_code_assigned++;
    insert_string_pair(string_table, newC, UNKNOWN);
  }

  free(stack);
  free(string_table);
} 

/*void decode() {   // using left hand side implementation on page 7 of project
spec.
// "Version where strings are inserted in table with UNKNOWN CHAR fields"
  char *decoded = malloc(512);
  if (!decoded) {
    fprintf(stderr, "Memory allocation failure\n");
    exit(1);
  }

  string_table_el *string_table = initialize_string_table();
  Stack *stack = initialize_stack();

  int ascii_letters; // ascii is a variable to store the numbers read by scanf
  int decoded_len = 0;

  while (decoded_len < 511 && scanf("%d", &ascii_letters) != EOF) {
    decoded[decoded_len] = (char)ascii_letters;
    decoded_len++;
  }

  decoded[decoded_len] = '\0';
  printf("%s\n", decoded);
  free(decoded);
} */
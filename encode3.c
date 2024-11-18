#include "encode.h"
#include <stdio.h>
#include <stdlib.h>
#define CHAR_BIT 8
#define TABLE_LEN 4091

int hash_function(int prefix_index, char last_char) {
  return ((unsigned long)(prefix_index) << CHAR_BIT | (unsigned)(last_char)) %
         TABLE_LEN;
}

void initialize_hash(hash_table *table) {
  for (int i = 0; i < TABLE_LEN; i++) { // initialize all the dict elements to
                                        // NULL to prevent undefined behavior
    table->dict[i] = NULL;
  }

  for (int i = 0; i < 128;
       i++) { // set up the first 128 elements of the hash table
    table->dict[i] = malloc(sizeof(hash_element));
    if (!table->dict[i]) {
      fprintf(stderr, "Memory allocation failed inside initialize_hash");
      exit(1);
    }
    table->dict[i]->pair_code = i;
    table->dict[i]->prefix_index = -1;
    table->dict[i]->last_char = (char)i;
    table->dict[i]->next = NULL; // added this
  }
  table->next_pair_code = 256;
}

int check_pair_in_table(int c, int k, hash_table *table) {
  char last_char = (char)k;
  int hash_index = hash_function(c, k);
  hash_element *current_node = table->dict[hash_index];
  while (current_node) {
    if ((current_node->prefix_index == c) &&
        (current_node->last_char == last_char)) {
      return current_node
          ->pair_code; // return the pair code if the pair exists in the table
    }
    current_node =
        current_node
            ->next; // need to traverse linked list in case there are collsions
  }
  return -1; // case where pair is not found
}

void insert_pair_in_table(int c, int k, int pair_code, hash_table *table) {
  int hash_index = hash_function(c, k);
  hash_element *insert_pair = malloc(sizeof(hash_element));
  if (!insert_pair) {
    fprintf(stderr, "Memory allocation failed inside insert_pair_in_table");
    exit(1);
  }
  insert_pair->pair_code = pair_code;
  insert_pair->prefix_index = c;
  insert_pair->last_char = k;
  insert_pair->next = table->dict[hash_index]; // need to set the pair at the
                                               // front of the linked list
  table->dict[hash_index] = insert_pair;
}

void free_hash(hash_table *table) {
  // free elements of the hash table
  for (int i = 0; i < TABLE_LEN; i++) {
    hash_element *current_el = table->dict[i];
    while (current_el) {
      hash_element *garbage = current_el;
      current_el = current_el->next;
      free(garbage);
    }
    table->dict[i] = NULL;
  }
}

void encode(int p, int max_bit_length) {
  hash_table *table = malloc(sizeof(hash_table));
  if (!table) {
    fprintf(stderr, "Memory allocation for hash table failed inside encode");
    exit(1);
  }
  initialize_hash(table);
  //int next_pair_code = 256;

  int c = -1;
  int k;

  while ((k = getchar()) != EOF) {
    int pair_code = check_pair_in_table(c, k, table);
    if (pair_code !=
        -1) { // if pair code is not -1, then it exists in the table
      c = pair_code;
    } else {
      if (c != -1) {
        printf("%d\n", c);
      }
      insert_pair_in_table(c, k, table->next_pair_code, table);
      table->next_pair_code++;
      //int hash_index = hash_function(c, k);
      //table->dict[hash_index]->pair_code = next_pair_code;
      //next_pair_code++;

      c = k;
    }
  }
  if (c != -1) {
    printf("%d\n", c);
  }
  free_hash(table);
  free(table); // free the overall hash_table struct
}
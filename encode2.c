#include <stdlib.h>
#include "encode.h"
//#include "dict.h"
#include <stdio.h>
#define CHAR_BIT 8
#define TABLE_LEN 4091


int hash_function(int prefix_index, char last_char) {
  return ((unsigned long)(prefix_index) << CHAR_BIT | (unsigned)(last_char)) %
         TABLE_LEN;
}

void initialize_hash(hash_table *table) {
  for (int i = 0; i < TABLE_LEN; i++) {
    table->dict[i] = NULL;
  }
}

void hash_insert(hash_table *table, int pair_code, int prefix_index,
                 char last_char) {
  int new_index = hash_function(prefix_index, last_char);
  hash_element *new = (hash_element *)malloc(sizeof(hash_element));
  new->pair_code = pair_code;
  new->prefix_index = prefix_index;
  new->last_char = last_char;
  new->next = table->dict[new_index];
  table->dict[new_index] = new;
}

int get_code(hash_table *table, int prefix_index, char last_char) {
  int index = hash_function(prefix_index, last_char);
  hash_element *current_el = table->dict[index];

  while (current_el) {
    if (current_el->prefix_index == prefix_index &&
        current_el->last_char == last_char) {
      return current_el->pair_code;
    } else {
      current_el = current_el->next;
    }
  }
  return -1; // In case that the pair was not found
}

void free_hash(hash_table *table) {
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

void initialize_dict_encode(hash_table *table) {
  for (int i = 0; i < 256; i++) {
    hash_insert(table, i, -1, (char)i);
  }
}


void encode(int p, int max_bit_length) {
  hash_table table;
  initialize_hash(&table);
  initialize_dict_encode(&table);

  int prefix_index = -1;
  int last_char;
  int next_code = 256;

  while ((last_char = getchar()) != EOF) {
    int code = get_code(&table, prefix_index, (char)last_char);
    if (code != -1) {
        prefix_index = code;
    } else {
        if (prefix_index != -1) {
            printf("%d\n", prefix_index);
        }

        if (next_code < (1 << max_bit_length)) {
            hash_insert(&table, next_code, prefix_index, (char)last_char);
        next_code++;
        }
        prefix_index = last_char;

    }
  }

    if (prefix_index != -1) {
        printf("%d\n", prefix_index);
    }
    free_hash(&table);
  }
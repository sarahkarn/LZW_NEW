#ifndef AB1D5576_F986_4B13_AE08_ABCF50CDF907
#define AB1D5576_F986_4B13_AE08_ABCF50CDF907
#include <stdio.h>
#include "dict.h"
#define CHAR_BIT 8
#define TABLE_LEN 4091

typedef struct hash_element {
    int pair_code;
    int prefix_index;
    char last_char;
    struct hash_element *next;
} hash_element;

typedef struct hash_table {    // implementing the string table
    hash_element *dict[TABLE_LEN];    // dict is an array of pointers to hash_elements
    int next_pair_code;
} hash_table;


void initialize_hash(hash_table *table);

int hash_function(int prefix_index, char last_char);

//int check_pair_in_table(int c, int k, int pair_code, hash_table *table);

int check_pair_in_table(hash_table *table, int c, int k);

void insert_pair_in_table(hash_table *table, int c, int k, int pair_code);

void free_hash(hash_table *table);

void encode(int p, int max_bit_length);



#endif // AB1D5576_F986_4B13_AE08_ABCF50CDF907
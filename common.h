#ifndef COMMON_H
#define COMMON_H

#define USE_SPECIAL_CODES

#include <stddef.h> 
#include <stdlib.h>
#include <stdio.h>
#define TABLE_LEN 4096
#define EMPTY -1
#define NOT_FOUND -2
#define CHAR_BIT 8
#define PRUNE_CODE 10000

// #define FIXED_BITS  13

extern int DBG;


typedef struct hash_element {
    int pair_code;
    int prefix_index;
    int last_char;
    int mark_pref_bit;
    struct hash_element *next;
} hash_element;

typedef struct hash_table {    // implementing the string table
  hash_element *dict[TABLE_LEN];    // dict is an array of pointers to hash_elements
  hash_element *codes[TABLE_LEN];
  int next_pair_code;
} hash_table;


hash_table* initialize_hash();

int table_sz( hash_table* table );

int table_full(hash_table *table);

int hash_function(int prefix_index, char last_char);

int table_find(hash_table *table, int c, int k);

int insert_pair_in_table(hash_table *table, int c, int k);   

void free_hash(hash_table *table);

int CHAR(hash_table *table, int pair_code );

// Takes a pair_code and uses the codes array in the table to get a hash_element. Then returns the prefix_index
int PREF(hash_table *table, int pair_code );

int code_unknown(hash_table *table, int c);

int is_special_code(int c);

int next_special(int num_bits);

void table_dump(hash_table *table, const char* filename );

int get_code();


//delete/free the old table inside the function. After prune is run, the
// run using format: table = prune(table)
// mape code to reassigend code
hash_table *prune(hash_table *original_table, int* code );   


void mark_pref_bit(hash_table *table, int code);


#endif // COMMON_H

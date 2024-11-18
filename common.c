#include "common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int get_code() {
  int code;
  int scan_val = scanf("%d", &code);
  if (scan_val != 1) {
    return EOF;
  }
  return code;
}


int table_full(hash_table *table) {
  if (table->next_pair_code >= TABLE_LEN) {
    return 1;
  }
  return 0;
}

hash_table *initialize_hash() {

  hash_table *table = malloc(sizeof(hash_table));
  if (!table) {
    fprintf(stderr, "Memory allocation for hash table failed inside encode");
    exit(1);
  }

  for (int i = 0; i < TABLE_LEN; i++) { // initialize all the dict elements to
                                        // NULL to prevent undefined behavior
    table->dict[i] = NULL;
    table->codes[i] = NULL;
  }

  table->next_pair_code = 0;

  for (int k = 0; k < 256; k++) {
    int code_insert = insert_pair_in_table(table, EMPTY, k);
    int code_find = table_find(table, EMPTY, k);
    if (code_find < 0) {
      fprintf(stderr, "Failure k:%d code_insert:%d code_find%d\n", k,
              code_insert, code_find);
    }
    assert(code_find >= 0);
  }

  table->next_pair_code = 256;

  return table;
}

int hash_function(int prefix_index, char last_char) {
  return ((unsigned long)(prefix_index) << CHAR_BIT | (unsigned)(last_char)) %
         TABLE_LEN;
}

int table_find(hash_table *table, int c, int k) {
  int last_char = k;
  int hash_index = hash_function(c, k);
  hash_element *current_node = table->dict[hash_index];
  while (current_node) {
    if ((current_node->prefix_index == c) &&
        (current_node->last_char == last_char)) {

      // Make sure codes are synced up
      int pair_code = current_node->pair_code;
      assert(table->codes[pair_code] == current_node);
      //

      return current_node
          ->pair_code; // return the pair code if the pair exists in the table
    }
    current_node =
        current_node
            ->next; // need to traverse linked list in case there are collsions
  }
  // fprintf(stderr, "Pair not found\n");

  return NOT_FOUND; // case where pair is not found
}

int insert_pair_in_table(hash_table *table, int c, int k) {

  if (table->next_pair_code >= TABLE_LEN) {
    fprintf(stderr, "insert_pair_in_table: next_pair_code: %d|n",
            table->next_pair_code);
  }
  assert(table->next_pair_code < TABLE_LEN);
  if (c >= table->next_pair_code) {
    fprintf(stderr,
            "codes can only refer to earlier codes: next_pair_code:%d c:%d\n",
            table->next_pair_code, c);
  }
  assert(c < table->next_pair_code);

  int hash_index = hash_function(c, k);
  hash_element *insert_pair = malloc(sizeof(hash_element));
  if (!insert_pair) {
    fprintf(stderr, "Memory allocation failed inside insert_pair_in_table");
    exit(1);
  }

  
  insert_pair->pair_code = table->next_pair_code;
  insert_pair->prefix_index = c;
  insert_pair->mark_pref_bit = 0;  // new codes are not marked
  insert_pair->last_char = k;
  insert_pair->next = table->dict[hash_index]; // need to set the pair at the
                                               // front of the linked list
  table->dict[hash_index] = insert_pair;
  table->codes[table->next_pair_code] = insert_pair;

  int rval = table->next_pair_code;
  table->next_pair_code++;
  return rval;
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

  for (int i = 0; i < TABLE_LEN; i++) {
    table->codes[i] = NULL;
  }
}

int CHAR(hash_table *table, int pair_code) {
  if (pair_code < 0 || pair_code >= TABLE_LEN ||
      table->codes[pair_code] == NULL) {
    fprintf(stderr, "pair_code passed to CHAR function is not valid %d\n",
            pair_code);
    exit(-1);
  }
  return table->codes[pair_code]->last_char;
}

int PREF(hash_table *table, int pair_code) {
  if (pair_code < 0 || pair_code >= TABLE_LEN ||
      table->codes[pair_code] == NULL) {
    fprintf(stderr, "pair_code passed to PREF function is not valid %d\n",
            pair_code);
    exit(-1);
  }
  return table->codes[pair_code]->prefix_index;
}

int code_unknown(hash_table *table, int c) {
  assert(c >= 0 && c < TABLE_LEN);
  if (table->codes[c] == NULL) {
    return 1; // code is unknown
  }
  return 0;
}

int is_special_code(int c) { // assuming special codes start at 512
  if (c >= 512) {
    int comp_val = 512;
    int power = 10;
    while (comp_val <= c) {
      if (comp_val == c) {
        return 1;
      }
      comp_val = (1 << power);
      power++;
    }
  }
  return 0;
}

int next_special(int num_bits) {
    return ((1 << num_bits) - 1);
}

void table_dump(hash_table *table, const char *filename) {
  fprintf(stderr, "table_dump %s:: has %d entries\n", filename,
          table->next_pair_code);
  FILE *f = fopen(filename, "w");
  if (f == NULL) {
    fprintf(stderr, "Error opening file %s for writing\n", filename);
    return;
  }

  for (int i = 0; i < table->next_pair_code; i++) {
    hash_element *e = table->codes[i];
    assert(i == e->pair_code);
    fprintf(f, "%d\t%d\t%d\n", i, e->prefix_index, e->last_char);
  }

  fclose(f);
}

void mark_pref_bit(hash_table *table, int code) {
  assert(table->codes[code] != NULL);
  table->codes[code]->mark_pref_bit = 1;
}

int is_marked(hash_table *table, int code) {
  assert(table->codes[code] != NULL);
  return table->codes[code]->mark_pref_bit;
}


int table_sz( hash_table* table ) {
  return table->next_pair_code;
}

hash_table *prune(hash_table *original_table, int* code ) {
  
  int original_len = table_sz(original_table);
  for (int i = 0; i < 256; i++) {
    mark_pref_bit(
        original_table,
        i); // marking the pref bit for first 256 single char pairs to 1
  }         // note that while these are not all prefixes to other pairs,
            // we want to include them in the pruned table
  for (int i = (original_len - 1); i >= 256; i--) {
    assert(original_table->codes[i] !=
           NULL); // don't want to attempt to access NULL data

    int current_pref_index = original_table->codes[i]->prefix_index;
    while (current_pref_index != EMPTY) {

      assert(original_table->codes[current_pref_index] != NULL);
      if( is_marked( original_table, current_pref_index )) {
	break;
      }
      mark_pref_bit(original_table, current_pref_index);
      current_pref_index =
          original_table->codes[current_pref_index]->prefix_index;
    }
  }
  // now finished identifying which pairs should be kept for the pruned table
  // by setting the mark_pref_bits. below, pruned table will be constructed

  int map[TABLE_LEN];
  int count = 0;   // count the number of pairs with marked bits
  for (int i = 0; i < TABLE_LEN; i++) {
    map[i] = count;
    count += original_table->codes[i]->mark_pref_bit;
  }

  // map code
  *code = map[*code];

  hash_table *pruned_table = initialize_hash();
  for (int i = 256; i < original_len; i++) {
    if (original_table->codes[i]->mark_pref_bit == 1) {
        int c = original_table->codes[i]->prefix_index;
        int k = original_table->codes[i]->last_char;
        int map_c = map[c];
        int map_k = map[k];
	assert(map_k == k );
	if( i < 256 ) {
	  assert( map_c == c );
	}
        insert_pair_in_table(pruned_table, map_c, map_k);
    }
  }

  free_hash(original_table);
  return pruned_table;
}

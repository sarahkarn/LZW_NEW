#include "encode.h"
#include "codec.h"
#include <stdio.h>
#include <stdlib.h>
//#define CHAR_BIT 8
// define EMPTY -1
//#define NOT_FOUND -2
#include <assert.h>

// WORKING VERSION OF ENCODE
/* void encode(int p, int max_bit_length) {
  hash_table *table = malloc(sizeof(hash_table));
  if (!table) {
    fprintf(stderr, "Memory allocation for hash table failed inside encode");
    exit(1);
  }
  initialize_hash(table);
  int next_pair_code = 256;
  int prefix_index = EMPTY;
  int k;

  while ((k = getchar()) != EOF) {
    int pair_code = check_pair_in_table(prefix_index, k, table);
    //fprintf(stderr, "Pair code from check_pair is %d\n", pair_code);
    if (pair_code != NOT_FOUND) {
      // Found the pair, update prefix_index to the found pair code
      //fprintf(stderr, "A: pair_code (%d,%d) found\n", prefix_index, k);
      prefix_index = pair_code;
      //fprintf(stderr, "Prefix index has been set to %d\n", pair_code);
    } else {
      // Pair not found, output current prefix_index
      if (prefix_index != EMPTY) {
        printf("%d\n", prefix_index);
        fflush(stdout); // Ensure immediate output
      }
      // Insert the new pair into the table
      //fprintf(stderr, "B: pair_code (%d,%d) not found\n", prefix_index, k);
      assert(prefix_index >= 0); // Ensure prefix_index is valid before
inserting
      //fprintf(stderr, "adding (%d,%d,%d)\n", next_pair_code, prefix_index, k);
      insert_pair_in_table(table, next_pair_code, prefix_index, k);

      // Debugging print statements
      assert(next_pair_code >= 256);
      //fprintf(stderr, "Pair code is: %d\n", next_pair_code);

      next_pair_code++;
      prefix_index = k; // Update prefix_index to k for the next iteration
      //fprintf(stderr, "Inside else block, prefix_index has been set to %d\n",
k);
    }
  }

  // Print the last prefix_index if it's not EMPTY
  if (prefix_index != EMPTY) {
    printf("%d\n", prefix_index);
    fflush(stdout); // Ensure immediate output
  }

  free_hash(table);
  free(table); // Free the overall hash_table struct
} */

// my latest version of encode BELOW
void encode(int p, int max_bit_length) {

  codec *codec = new_codec();

  hash_table *table = initialize_hash();

  // fprintf(stderr,"Initialized hash table\n");

  // first line is MAXBIT as specified in bullet (E)

  codec_put(codec, max_bit_length, FIXED_BITS);
  // printf("%d\n", max_bit_length);
  // fflush(stdout);

  codec_put(codec, p, FIXED_BITS);
  // second line is p as specified on page 2
  // printf("%d\n", p);
  // fflush(stdout);

  int C = EMPTY;
  int K;
  int num_bits = 9;
  int next_special_code = next_special(9);

  while ((K = getchar()) != EOF) {

    int code = table_find(table, C, K);

    // If the pair (C,K) is in the table
    if (code != NOT_FOUND) {

      // Set C = code associated witht the pair (C,K) in the table
      C = code;

    } else {

      assert(C >= 0);

      codec_put(codec, C, FIXED_BITS);
      // printf("%d\n", C);
      // fflush(stdout);

      if (p && table_full(table)) { // Prune table and modify C
        fprintf(stderr, "ENCODE: TABLE FULL\n");

        int orig_C = C;
        int orig_sz = table_sz(table);
        table = prune(table, &C);
        int sz = table_sz(table);
        fprintf(stderr, "ENCODE PRUNE: %d->%d  %d->%d\n", orig_sz, sz, orig_C,
                C);
      }


      if (!table_full(table)) {

        assert(C >= 0);
        assert(K >= 0);
        int new_code = insert_pair_in_table(table, C, K);

        // dealing with special codes below

#ifdef USE_SPECIAL_CODES
	
	if (num_bits < max_bit_length) {	
	  if (new_code == next_special_code) { // ADDED THIS
            printf("%d\n", new_code);
            fflush(stdout);
            num_bits++;
            fprintf(stderr, "ENCODE: num_bits has been incremented to %d\n", num_bits);
            assert(num_bits <= max_bit_length);
            next_special_code = next_special(num_bits);
            //fprintf(stderr, "new special code is %d\n", next_special_code);
          }
        }
#endif

	int sz = table_sz(table);

	if (sz >= 4090) {
	  fprintf(stderr, "ENCODE: insert %d %d %d\n", new_code, C, K);
	}
      
      }

    C = table_find(table, EMPTY, K);

    assert(C >= 0);
  }
}

// fprintf(stderr,"B: Table has %d entries\n", table->next_pair_code );   -
// DEBUG STATEMENT

// Output code C ( if c != EMPTY )
if (C != EMPTY) {
  assert(C >= 0);
  codec_put(codec, C, FIXED_BITS);
}

// fprintf(stderr,"C: Table has %d entries\n", table->next_pair_code );  DEBUG
// STATEMENT
if (DBG) {
  table_dump(table, "./DBG.encode");
}

free_hash(table);

free_codec(codec);
}

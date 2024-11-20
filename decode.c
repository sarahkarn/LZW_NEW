#include "decode.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHAR_BIT 8
#define UNKNOWN -2
#include "codec.h"
#include "common.h"
#include <assert.h>

// int last_code_assigned = 256;

// NEW DECODE USING RIGHT SIDE PSEUDO
void decode() {

  hash_table *table = initialize_hash(table);
  Stack *stack = initialize_stack();

  // TESTING NEW IMP FOR NOW codec *codec = new_codec();

#ifdef USE_CODEC
  codec *codec = initialize_codec();  
#else
  coder_dec *coder_dec = initialize_coder_dec();
#endif  

  int current_num_bits = 9;
  int max_bits;
  int p = 0; // pruning off by default

  int check_first_code = 1;
  int check_second_code = 0;

  int final_k = EMPTY;
  int c;
  int oldC = EMPTY;
  int newC;

  int next_special_code = 511;

  //  while ((newC = c = get_code()) != EOF) {

#ifdef USE_CODEC
  while ((newC = c = codec_get(codec, FIXED_BITS)) != EOF) {  
#else
  while ((newC = c = coder_dec_get(coder_dec, FIXED_BITS)) != EOF) {
#endif    


    //  fprintf(stderr,"c:%d\n", c );
  
  // TESTING NEW IMP FOR NOW while ((newC = c = codec_get(codec, FIXED_BITS)) != EOF) {

    assert(c >= 0);
    if (check_first_code) { // treating the first code differently since it
                            // outputs MAXBITS.
      max_bits = c;
      check_first_code = 0; // set first code flag to 0 once MAXBITS has been identified

      check_second_code++;
      continue;
    }

    if (check_second_code) {
      p = c;
      check_second_code = 0;
      continue;
    }

#ifdef USE_SPECIAL_CODES
    if (c == next_special_code) {
      current_num_bits++;
      fprintf(stderr, "DECODE: current_num_bits: %d\n", current_num_bits);
      assert(current_num_bits <= max_bits);
      next_special_code = next_special(current_num_bits);
      continue;
    }
#endif

    if (p && table_full(table)) { // Prune table and modify oldC

      fprintf(stderr, "DECODE: TABLE FULL\n");

      int old_sz = table_sz(table);
      int orig_oldC = oldC;
      table = prune(table, &oldC);
      int sz = table_sz(table);
      fprintf(stderr, "DECODE: PRUNE %d->%d  %d->%d\n", old_sz, sz, orig_oldC,
              oldC);
      ;
    }

    if (table->codes[c] == NULL) { // Unknonw code
      push(stack, final_k);
      c = oldC;
    }

    while (PREF(table, c) != EMPTY) {
      push(stack, CHAR(table, c));
      c = PREF(table, c);
    }

    final_k = CHAR(table, c);
    putchar(final_k);

    while (stack_non_empty(stack)) {
      int k;
      k = pop(stack);
      putchar(k);
    }

    if (oldC != EMPTY) {

      if (!table_full(table)) {
        int new_code = insert_pair_in_table(table, oldC, final_k);
        int sz = table_sz(table);
        if (sz >= 4090) {
          fprintf(stderr, "DECODE: insert %d %d %d\n", new_code, oldC, final_k);
        }
      }
    }

    oldC = newC;
  }

  if (DBG) {
    table_dump(table, "./DBG.decode");
  }

  free(stack->characters);
  free(stack);
  free_hash(table);

#ifdef USE_CODEC
  codec_free(codec);  
#else
  coder_dec_free(coder_dec);
#endif  
  // TESTING NEW IMP FOR NOW free_codec(codec);
}

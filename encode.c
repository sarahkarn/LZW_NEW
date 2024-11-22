#include "encode.h"
#include "codec.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int count = 0;

int put_(coder_dec *c, int code, int num_bits) {
  
#ifdef FIXED_BITS  
  coder_dec_put(c, code, FIXED_BITS);
#else
  coder_dec_put(c, code, num_bits );  
#endif

}

void encode(int p, int max_bit_length) {

  int num_bits = 9;

  coder_dec *coder_dec = initialize_coder_dec();

  hash_table *table = initialize_hash();

  // fprintf(stderr,"Initialized hash table\n");

  // first line is MAXBIT as specified in bullet (E)


  put_(coder_dec, max_bit_length, num_bits);
  put_(coder_dec, p, num_bits);


  //coder_dec_put(coder_dec, max_bit_length, FIXED_BITS); 

  //coder_dec_put(coder_dec, p, FIXED_BITS);

  int C = EMPTY;
  int K;
  int next_special_code = next_special(9);

  while ((K = getchar()) != EOF) {

    int code = table_find(table, C, K);

    // If the pair (C,K) is in the table
    if (code != NOT_FOUND) {

      // Set C = code associated with the pair (C,K) in the table
      C = code;

    } else {

      assert(C >= 0);

       put_(coder_dec, C, num_bits);
      // coder_dec_put(coder_dec, C, FIXED_BITS);
     
      // printf("%d\n", C);
      // fflush(stdout);

      if (p && table_full(table)) { // Prune table and modify C
        //fprintf(stderr, "ENCODE: TABLE FULL\n");

        int orig_C = C;
        int orig_sz = table_sz(table);
        table = prune(table, &C);
        int sz = table_sz(table);
        //fprintf(stderr, "ENCODE PRUNE: %d->%d  %d->%d\n", orig_sz, sz, orig_C,
                //C);
      }


      if (!table_full(table)) {

        assert(C >= 0);
        assert(K >= 0);
        int new_code = insert_pair_in_table(table, C, K);

        // dealing with special codes below

#ifdef USE_SPECIAL_CODES
	
	if (num_bits < max_bit_length) {	
	  if (new_code == next_special_code) {

	    put_(coder_dec, new_code, num_bits);
	    //coder_dec_put(coder_dec, new_code, FIXED_BITS);
	
            num_bits++;
            //fprintf(stderr, "ENCODE: num_bits has been incremented to %d\n", num_bits);
            assert(num_bits <= max_bit_length);
            next_special_code = next_special(num_bits);
            //fprintf(stderr, "new special code is %d\n", next_special_code);
          }
        }

    #endif

	int sz = table_sz(table);

	if (sz >= 4090) {
	  //fprintf(stderr, "ENCODE: insert %d %d %d\n", new_code, C, K);
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
 
  put_(coder_dec, C, num_bits);
  //coder_dec_put(coder_dec, C, FIXED_BITS);

}

// fprintf(stderr,"C: Table has %d entries\n", table->next_pair_code );  DEBUG
// STATEMENT
if (DBG) {
  table_dump(table, "./DBG.encode");
}

free_hash(table);

coder_dec_free(coder_dec);

}

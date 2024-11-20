#include "codec.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef USE_CODEC

void coder_dec_write_bit(coder_dec *c, int bit) {
    assert(bit == 0 || bit == 1);

    if (bit) {
        c->buf |= (1 << (c->idx - 1)); 
    }
    c->idx--; 

    if (c->idx == 0) {
        int r = putchar(c->buf);
        if (r == EOF) {
            fprintf(stderr, "putchar failed\n");
            exit(-1);
        }
        fflush(stdout);
        c->buf = 0; // Reset buffer
        c->idx = 8; // Reset index
    }
}


/*
// write a bit starting with most significant
void coder_dec_write_bit(coder_dec *c, int bit) {  
  assert(bit == 0 || bit == 1);

  if( bit ) {  
    c->buf |= ( 1 << ( c->idx -1 ) );
  }
  c->idx--;
  if (c->idx == 0) {
    int r = putchar(c->buf);
    if( r == EOF ) {
      fprintf(stderr,"putchar failed\n");
      exit(-1);
    }
    fflush(stdout);
    c->buf = 0;
    c->idx = 8;
  }
  return;
}
*/


void coder_dec_put(coder_dec *c, int code, int num_bits) {
  if (STAGE != 3) {
    printf("%d\n", code);
    fflush(stdout);
    return;
  }

  for (int i = num_bits - 1; i >= 0; i--) {
    int bit = (code >> i) & 1; // Extract each bit from code
    coder_dec_write_bit(c, bit);
  }
  return;
}


int coder_dec_get(coder_dec *c, int num_bits) {
  if (STAGE != 3) {
    int code = get_code();
    return code;
  }

  int rval = 0;
  for (int i = num_bits-1; i >= 0; i--) {
    int bit = coder_dec_next_bit(c);
    if (bit == EOF) {
      return EOF;
    }
    rval |= (  bit << i );
  }
  return rval;
}

int coder_dec_next_bit(coder_dec *c) {
    if (c->idx == 8) { // If buffer is empty, read a new byte
        c->buf = getchar();
        if (c->buf == EOF) {
            return EOF;
        }
    }
    c->idx--;
    int bit = (c->buf >> c->idx) & 1; // Get the current bit
    if( c->idx == 0 ) {
      c->idx = 8;
    }
    return bit;
}

coder_dec *initialize_coder_dec() {
  coder_dec *c = (coder_dec *)malloc(sizeof(coder_dec));
  if (!c) {
    fprintf(stderr, "Memory allocation failed inside initialize_coder_dec");
    exit(-1);
  }
  c->buf = 0;
  c->idx = 8;
  return c;
}

void coder_dec_free(coder_dec *c) {
  // Flush the last bits
  if (c->idx != 8 )  {
    putchar( c->buf);
    fflush(stdout);
  }
  free(c);
}

// DONT SUBMIT FOR STAGE 2
void test_coder_dec() {

  int codes[TEST_SIZE] = {
      511,  1023, 2047, 4095, // Max values for 9, 10, 11, 12 bits
      256,  512,  1024, 2048, // Powers of 2
      123,  456,  789,  1234, // Random values
      0,    1,    2,    3,    // Small values
      4093, 4094, 4095, 4095  // Large values (last two are max 12-bit)
  };

  int bits[TEST_SIZE] = {9,  10, 11, 12, 9,  10, 11, 12, 9,  10,
                         11, 12, 9,  9,  10, 11, 12, 12, 12, 12};

  coder_dec *c = initialize_coder_dec();
  FILE *temp_file = tmpfile();

  // Redirect stdout to our temporary file
  FILE *old_stdout = stdout;
  stdout = temp_file;

  // Write values

  int N = TEST_SIZE;
  for (int i = 0; i < N; i++) {
    coder_dec_put(c, codes[i], bits[i]);
  }

  // Flush any remaining bits
  coder_dec_free(c); // Use max bits to ensure all data is flushed




  

  // Reset file position and redirect stdin
  fflush(temp_file);
  fseek(temp_file, 0, SEEK_SET);
  FILE *old_stdin = stdin;
  stdin = temp_file;

  // Read and verify values
  c = initialize_coder_dec();
  for (int i = 0; i < N; i++) {
    int read_value = coder_dec_get(c, bits[i]);
    if (read_value != codes[i]) {
      fprintf(stderr,"Mismatch at index %d: expected %d, got %d (bits: %d)\n", i,
             codes[i], read_value, bits[i]);
      assert(read_value == codes[i]);
    }
  }

  // Verify EOF
  assert(coder_dec_get(c, 12) == EOF);

  // Clean up
  coder_dec_free(c);
  fclose(temp_file);
  stdin = old_stdin;
  stdout = old_stdout;

  fprintf(stderr,"All tests passed successfully!\n");
}

#endif

#ifdef USE_CODEC

codec *initialize_codec() {
  codec *c = (codec *)malloc(sizeof(codec));
  if (c) {
    c->current_char = 0;
    c->bit_position = 0;
  }
  return c;
}

void codec_free(codec *c) {
  if (c) {
    // Flush any remaining bits
    if (c->bit_position > 0) {
      putchar(c->current_char);
    }
    free(c);
  }
}

void codec_put(codec *c, int code, int mbits) {

  
  if (STAGE != 3) {
    printf("%d\n", code);
    fflush(stdout);
    return;
  }

  if( code > (1 << mbits ) -1 ) {
    fprintf(stderr,"codec_put ERROR code:%d mbits:%d\n", code, mbits );
    exit(-1);
  }

  // DELETE THIS FOR STAGE 2 SUBMISSION         -
  for (int i = mbits - 1; i >= 0; i--) {
    int bit = (code >> i) & 1;
    c->current_char |= (bit << (7 - c->bit_position));
    c->bit_position++;

    if (c->bit_position == 8) {
      putchar(c->current_char);
      if( ferror(stdout) ) {
	fprintf(stderr,"ERROR putchar:%d to stdout\n", c->current_char );
	exit(-1);
      }
      c->current_char = 0;
      c->bit_position = 0;
    }
  }
}

int codec_get(codec *c, int mbits) {

  if (STAGE != 3) {
    return get_code();
  }

  int code = 0;
  for (int i = 0; i < mbits; i++) {
    if (c->bit_position == 0) {
      c->current_char = getchar();
      if (c->current_char == EOF) {
        return EOF;
      }
      c->bit_position = 8;
    }

    c->bit_position--;
    int bit = (c->current_char >> c->bit_position) & 1;
    code = (code << 1) | bit;
  }
  return code; 
}




// DONT SUBMIT FOR STAGE 2
void test_codec() {


  int codes[TEST_SIZE] = {
      511,  1023, 2047, 4095, // Max values for 9, 10, 11, 12 bits
      256,  512,  1024, 2048, // Powers of 2
      123,  456,  789,  1234, // Random values
      0,    1,    2,    3,    // Small values
      4093, 4094, 4095, 4095  // Large values (last two are max 12-bit)
  };

  int bits[TEST_SIZE] = {9,  10, 11, 12, 9,  10, 11, 12, 9,  10,
                         11, 12, 9,  9,  10, 11, 12, 12, 12, 12};

  codec *c = initialize_codec();
  FILE *temp_file = tmpfile();

  // Redirect stdout to our temporary file
  FILE *old_stdout = stdout;
  stdout = temp_file;

  // Write values
  for (int i = 0; i < TEST_SIZE; i++) {
    codec_put(c, codes[i], bits[i]);
  }

  // Flush any remaining bits
  codec_free(c); // Use max bits to ensure all data is flushed

  // Reset file position and redirect stdin
  fflush(temp_file);
  fseek(temp_file, 0, SEEK_SET);
  FILE *old_stdin = stdin;
  stdin = temp_file;

  // Read and verify values
  c = initialize_codec();
  for (int i = 0; i < TEST_SIZE; i++) {
    int read_value = codec_get(c, bits[i]);
    if (read_value != codes[i]) {
      fprintf(stderr,"Mismatch at index %d: expected %d, got %d (bits: %d)\n", i,
             codes[i], read_value, bits[i]);
      assert(read_value == codes[i]);
    }
  }

  // Verify EOF
  assert(codec_get(c, 12) == EOF);

  // Clean up
  codec_free(c);
  fclose(temp_file);
  stdin = old_stdin;
  stdout = old_stdout;

  printf("All tests passed successfully!\n");
}

#endif

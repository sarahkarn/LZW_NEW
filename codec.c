#include <stdlib.h>
#include <stdio.h>
#include "codec.h"
#include "common.h"

codec* new_codec() {
    codec* c = (codec*)malloc(sizeof(codec));
    if (c) {
        c->current_char = 0;
        c->bit_position = 0;
    }
    return c;
}

void free_codec(codec* c) {
    if (c) {
        // Flush any remaining bits
        if (c->bit_position > 0) {
            putchar(c->current_char);
        }
        free(c);
    }
}

void codec_put(codec* c, int code, int mbits) {
  if( STAGE != 3 ) {
    printf("%d\n", code );
    fflush(stdout);
    return;
  }

  fprintf(stderr, "Stage 3 not implemented");
  exit(-1);

// DELETE THIS FOR STAGE 2 SUBMISSION         - 
    /*for (int i = mbits - 1; i >= 0; i--) {
        int bit = (code >> i) & 1;
        c->current_char |= (bit << (7 - c->bit_position));
        c->bit_position++;
        
        if (c->bit_position == 8) {
            putchar(c->current_char);
            c->current_char = 0;
            c->bit_position = 0;
        }
    } */
}

int codec_get(codec* c, int mbits) {
  
  if( STAGE != 3 ) {
    return get_code();
  }


  fprintf(stderr, "Stage 3 not implemented");
  exit(-1);

  /* int code = 0;
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
  return code; */
}

// DONT SUBMIT FOR STAGE 2
void test_codec( int stage ) {

  STAGE = stage;
  
    int codes[TEST_SIZE] = {
        511, 1023, 2047, 4095,  // Max values for 9, 10, 11, 12 bits
        256, 512, 1024, 2048,   // Powers of 2
        123, 456, 789, 1234,    // Random values
        0, 1, 2, 3,             // Small values
        4093, 4094, 4095, 4095  // Large values (last two are max 12-bit)
    };
    
    int bits[TEST_SIZE] = {
        9, 10, 11, 12,
        9, 10, 11, 12,
        9, 10, 11, 12,
        9, 9, 10, 11,
        12, 12, 12, 12
    };

    codec* c = new_codec();
    FILE* temp_file = tmpfile();

    // Redirect stdout to our temporary file
    FILE* old_stdout = stdout;
    stdout = temp_file;

    // Write values
    for (int i = 0; i < TEST_SIZE; i++) {
        codec_put(c, codes[i], bits[i]);
    }

    // Flush any remaining bits
    free_codec(c);  // Use max bits to ensure all data is flushed

    // Reset file position and redirect stdin
    fflush(temp_file);
    fseek(temp_file, 0, SEEK_SET);
    FILE* old_stdin = stdin;
    stdin = temp_file;

    // Read and verify values
    c = new_codec();
    for (int i = 0; i < TEST_SIZE; i++) {
        int read_value = codec_get(c, bits[i]);
        if (read_value != codes[i]) {
            printf("Mismatch at index %d: expected %d, got %d (bits: %d)\n", 
                   i, codes[i], read_value, bits[i]);
            assert(read_value == codes[i]);
        }
    }

    // Verify EOF
    assert(codec_get(c, 12) == EOF);

    // Clean up
    free_codec(c);
    fclose(temp_file);
    stdin = old_stdin;
    stdout = old_stdout;

    printf("All tests passed successfully!\n");
}



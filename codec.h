#ifndef CODEC_H
#define CODEC_H    

//#define USE_CODEC  // IF YOU COMMENT THIS OUT, IT WILL SWITCH OUT TO USING CODER_DEC

extern int STAGE;

#define TEST_SIZE 20

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define TEST_SIZE 20

#ifndef USE_CODEC
typedef struct coder_dec {
    int buf;     // holds the bits
    int idx;      // number of bits held in buffer
} coder_dec;

void coder_dec_write_bit(coder_dec *c, int bit);

void coder_dec_put(coder_dec *c, int code, int num_bits);

int coder_dec_get(coder_dec *c, int num_bits);

int coder_dec_next_bit(coder_dec *c);

coder_dec *initialize_coder_dec();

void coder_dec_free(coder_dec *c);

#endif



#ifdef USE_CODEC
typedef struct codec {
  int current_char;
  int bit_position;
} codec;
 
codec* initialize_codec();

// flush any remaining codes
// then free the data structure
void   codec_free( codec* codec );

// interpret code as a binary number length 'mbits' bits
// put the bits on the standard output one bit at a time
// if we don't have a complete byte to put, then buffer the
// data until a subsequent code_put fills up a complete byte
void   codec_put( codec* codec, int code, int mbits );

// get the next mbits interpreted as an integer
// retuns EOF when there is no more data to read
int codec_get( codec* codec, int mbits );

void test_coder_dec();

void test_codec();

#endif

#endif

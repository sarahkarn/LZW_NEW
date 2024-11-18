#ifndef CODEC_H
#define CODEC_H

#define TEST_SIZE 20

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define TEST_SIZE 20


typedef struct codec {
  int current_char;
  int bit_position;
} codec;
 
codec* new_codec();

// flush any remaining codes
// then free the data structure
void   free_codec( codec* codec );

// interpret code as a binary number length 'mbits' bits
// put the bits on the standard output one bit at a time
// if we don't have a complete byte to put, then buffer the
// data until a subsequent code_put fills up a complete byte
void   codec_put( codec* codec, int code, int mbits );

// get the next mbits interpreted as an integer
// retuns EOF when there is no more data to read
int    codec_get( codec* codec, int mbits );

void  test_codec( int stage );

#endif

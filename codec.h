
extern int STAGE;

#define TEST_SIZE 20

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define TEST_SIZE 20

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
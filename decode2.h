#ifndef B6024EEC_47B1_43D8_A5E2_7E0CE27C1E65
#define B6024EEC_47B1_43D8_A5E2_7E0CE27C1E65

#define DICT_LEN 4096

#ifndef DECODE_H
#define DECODE_H

typedef struct Dict {
    char *string;
} Dict;

void initialize_dict_decode(Dict *dict);

void free_dict(Dict *dict);

void decode();

#endif

#endif /* B6024EEC_47B1_43D8_A5E2_7E0CE27C1E65 */
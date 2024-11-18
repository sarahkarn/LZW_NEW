#define EMPTY -1

struct dict_struct {
    int pair_code;
    int prefix_index;
    char last_char;
};

extern struct dict_struct dict[4096];

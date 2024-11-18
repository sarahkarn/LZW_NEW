#include "decode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// make sure that you free the stack too

// free_dict can only be called exactly once

void initialize_dict_decode(Dict *dict) {
  for (int i = 0; i < DICT_LEN; i++) {
    dict[i].string = NULL;
  }
  for (int i = 0; i < 256; i++) {
    dict[i].string =
        (char *)malloc(2 * sizeof(char)); // need space for null terminator
    dict[i].string[0] = (char)i;          // casting i from an int to a char
    dict[i].string[1] = '\0';
  }
}

void free_dict(Dict *dict) {
  fprintf(stderr, "Calling free_dict\n");
  for (int i = 0; i < DICT_LEN; i++) {
    //fprintf(stderr, "Loop index i = %d\n", i);
    if (dict[i].string != NULL) { // check that the string has been allocated
      //fprintf(stderr, "Freeing dict[%d].string\n", i);
      free(dict[i].string);
      //fprintf(stderr, "Freed dict[%d].string\n", i);
      dict[i].string = NULL;
    }
  }
  fprintf(stderr, "Exiting free_dict");
}

void decoded_append(char **decoded, char *string, int *decoded_len) {
  int str_len = strlen(string);
  char *new_decoded = realloc(*decoded, *decoded_len + str_len + 1);
  if (!new_decoded) {
    fprintf(stderr,
            "Memory allocation failed for new_decoded in decoded_append");
    exit(1);
  }
  *decoded = new_decoded;
  strcat(*decoded, string);
  *decoded_len += str_len;
}

void dict_add(Dict *dict, int prev_code, char first_char, int new_code) {
  int new_len = strlen(dict[prev_code].string) + 2;
  dict[new_code].string = (char *)malloc(new_len * sizeof(char));
  strcpy(dict[new_code].string, dict[prev_code].string);
  dict[new_code].string[new_len - 2] = first_char;
  dict[new_code].string[new_len - 1] = '\0'; // set null terminator
}

char *handle_equal_codes(Dict *dict, int prev_code) {
  int len = strlen(dict[prev_code].string) + 2;
  char *new_str = (char *)malloc(len * sizeof(char));
  if (!new_str) {
    fprintf(stderr, "Memory allocation failed");
    exit(1);
  }
  strcpy(new_str, dict[prev_code].string);
  new_str[len - 2] = dict[prev_code].string[0];
  new_str[len - 1] = '\0';
  return new_str;
}

void decode() {
  Dict dict[DICT_LEN];
  initialize_dict_decode(dict);

  char *decoded = malloc(512); // decoded holds the string that will be printed
  if (!decoded) {
    fprintf(stderr, "Memory allocation failed for decoded");
    exit(1);
  }

  int decoded_len = 0;

  int next_code = 256;

  int prev_code;
  if (scanf("%d", &prev_code) == EOF) {
    free(decoded);
    free_dict(dict);
    return;
  }

  strcpy(decoded, dict[prev_code].string);
  decoded_len = strlen(decoded);

  int new_code;
  while (scanf("%d", &new_code) != EOF) {
    char *string;
    if (new_code < next_code) {
      string = dict[new_code].string;
    } else if (new_code == next_code) {
      string = handle_equal_codes(dict, prev_code);
      if (next_code < DICT_LEN) {
        dict[next_code].string = string;
      }
    } else {
      fprintf(stderr, "Code invalid");
      exit(1);
    }

    decoded_append(&decoded, string, &decoded_len);

    if (next_code < DICT_LEN && new_code != next_code) {
      dict_add(dict, prev_code, string[0], next_code);
      next_code++;
    }

    prev_code = new_code;
  }

  printf("%s\n", decoded);
  free(decoded);
  free_dict(dict);
}

/* void decode() {
  char *decoded = malloc(512);
  if (!decoded) {
    fprintf(stderr, "Memory allocation failure\n");
    exit(1);
  }
  int ascii_letters; // ascii is a variable to store the numbers read by scanf
  int decoded_len = 0;

  while (decoded_len < 511 && scanf("%d", &ascii_letters) != EOF) {
    decoded[decoded_len] = (char)ascii_letters;
    decoded_len++;
  }

  decoded[decoded_len] = '\0';
  printf("%s\n", decoded);
  free(decoded);
} */
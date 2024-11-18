#include <libgen.h> // Include for basename
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decode.h"
#include "encode.h"
#include "codec.h"

#include "common.h"

#include <unistd.h>

int DBG = 0;   // DBG is a global variable
int STAGE = 0;

//#include "dict.h"

// struct dict_struct dict[4096];

int main(int argc, char *argv[]) {

  //test_codec(3); // test stage 3 codec
  //test_codec(2); // test stage 2 codec  
  //exit(0);


  
  static char bin[64], bout[64];
  setvbuf(stdin, bin, _IOFBF, 64);
  setvbuf(stdout, bout, _IOFBF, 64);

  char *exec_name = basename(argv[0]); // Get the executable name

  // Check the STAGE environment variable
  char *stage_env = getenv("STAGE");
  STAGE = stage_env ? atoi(stage_env) : 0;

  // Check the DBG environment variable
  char *dbg_env = getenv("DBG");
  DBG = dbg_env ? atoi(dbg_env) : 0;


  if (strcmp(exec_name, "encode") == 0) {
    //hash_table dict;
    //initialize_hash(&dict);
    int maxBits = 12;
    int pruning = 0;

    // TODO: Parsing logic
    

    // Only parse -p and -m flags if STAGE is 2 or 3
    if ( STAGE == 2 || STAGE == 3) {
      for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
          pruning = 1; // Enable prune mode
        } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
          maxBits = atoi(argv[++i]); // Set max bits from argument
        } else {
          fprintf(stderr, "Usage: %s [-m MAXBITS] [-p] < input > output\n",
                  argv[0]);
          exit(1);
        }
      }
    }


    encode(pruning, maxBits);


    //free_hash(&dict);

  } else if (strcmp(exec_name, "decode") == 0) {
    if (argc > 1) {
      fprintf(stderr, "decode: invalid option '%s'\n", argv[1]);
      exit(1);
    }
    decode();


  } else {
    fprintf(stderr, "Usage: %s [-m MAXBITS] [-p] < input > output\n", argv[0]);
    fprintf(stderr, "       %s < input > output\n", argv[0]);
    exit(1);
  }
  return 0;
}

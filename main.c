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
int STAGE = 0; // global

int main(int argc, char *argv[]) {

  static char bin[64], bout[64];
  setvbuf(stdin, bin, _IOFBF, 64);
  setvbuf(stdout, bout, _IOFBF, 64);

  char *exec_name = basename(argv[0]);

  // Checking the STAGE environment variable
  char *stage_env = getenv("STAGE");
  STAGE = stage_env ? atoi(stage_env) : 0;

  fprintf(stderr,"STAGE:%d\n", STAGE );

  //test_coder_dec();
  //return 0;


  // Checking the DBG environment variable
  char *dbg_env = getenv("DBG");
  DBG = dbg_env ? atoi(dbg_env) : 0;


  if (strcmp(exec_name, "encode") == 0) {   // default settings
    int maxBits = 12;
    int pruning = 0;

    // TODO: Parsing logic
    

    // Parsing of -p and -m flags only applies to STAGES 2 and 3
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

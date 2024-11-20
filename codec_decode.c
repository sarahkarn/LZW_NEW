#include <stdio.h>
#include "common.h"
#include "codec.h"

int STAGE = 3;

int main() {

#ifdef USE_CODEC
  codec *c = initialize_codec();  
#else  
  coder_dec *c = initialize_coder_dec();
#endif

  int code = 0;
  // Read each line and extract decimal numbers
#ifdef USE_CODEC
  while ( (code= codec_get(c, 13) ) != EOF ) {  
#else  
    while ( (code= coder_dec_get(c, 13 )) != EOF ) {
#endif    
    //fprintf(stderr,"%d\n", code );
    printf( "%d\n", code );
  }
  
#ifdef USE_CODEC
  codec_free( c );  
#else  
  coder_dec_free( c );
#endif  
}

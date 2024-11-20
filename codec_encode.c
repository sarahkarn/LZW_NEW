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

  int number = 0;
  // Read each line and extract decimal numbers
  while (scanf("%d", &number) == 1) {

#ifdef USE_CODEC
    codec_put( c, number, 13 );    
#else
    coder_dec_put( c, number, 13 );
#endif    
    
  }

#ifdef USE_CODEC
  codec_free( c );  
#else  
  coder_dec_free( c );
#endif  
}

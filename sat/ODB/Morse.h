
#ifndef _QB50_CW_MORSE_H
#define _QB50_CW_MORSE_H

#include <cstddef>

#include "device/GPIO.h"
#include "Baseband.h"


namespace qb50 {

   class Morse
   {

      public:

         Morse( Baseband::Power pow );
         ~Morse();

         void writeChar ( char c );
         void write     ( const char *x );
         void write     ( const char *x, size_t len );


      private:

         Baseband::Power _pow;

   };

}


#endif /*_QB50_CW_MORSE_H*/

/*EoF*/

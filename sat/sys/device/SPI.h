
#ifndef _QB50_SYS_DEVICE_SPI_H
#define _QB50_SYS_DEVICE_SPI_H

#include <cstddef>


namespace qb50 {

   class SPI
   {

      public:

         #include "SPI_Stream.h"

         SPI()
         { ; }

         virtual ~SPI()
         { ; }

         virtual SPI& lock    ( void ) = 0;
         virtual SPI& unlock  ( void ) = 0;
         virtual SPI& enable  ( bool debug = false ) = 0;
         virtual SPI& disable ( bool debug = false ) = 0;

         /* synchronous transfer */

         virtual SPI& xfer     ( const void *src, void *dst, size_t len ) = 0;
         virtual SPI& pollXfer ( const void *src, void *dst, size_t len ) = 0;
         virtual SPI& write    ( const void *src,            size_t len ) = 0;
         virtual SPI& read     (                  void *dst, size_t len ) = 0;

   };

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_SPI_H*/

/*EoF*/

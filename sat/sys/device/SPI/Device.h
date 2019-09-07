
#ifndef _QB50_SYSTEM_SPI_DEVICE_H
#define _QB50_SYSTEM_SPI_DEVICE_H

#include <Device.h>
#include <SPI.h>
#include <GPIO.h>


namespace qb50 {

   class SPI_Device : public Device
   {

      public:

         enum SelectMode {
            ActiveLow  = 0,
            ActiveHigh = 1
         };

         SPI_Device( const char *name, SPI& spi, GPIO::Pin& csPin, SelectMode csMode );
         virtual ~SPI_Device();

         SPI_Device& init   ( void );
         SPI_Device& lock   ( void );
         SPI_Device& unlock ( void );

      protected:

         SPI&       _spi;
         GPIO::Pin& _csPin;
         SelectMode _csMode;

         void _select   ( void );
         void _deselect ( void );

   };

} /* qb50 */


#endif /*_QB50_SYSTEM_SPI_DEVICE_H*/

/*EoF*/


#ifndef _QB50_SYS_DEVICE_SPI_L3GD20_H
#define _QB50_SYS_DEVICE_SPI_L3GD20_H

#include "SPI/Device.h"
#include "Gyro.h"


namespace qb50 {

   class L3GD20 : public SPI_Device, public Gyro
   {

      public:

         enum Range {
            R250DPS  = 0,
            R500DPS  = 1,
            R2000DPS = 2
         };

         L3GD20( SPI&        spi,
                 const char *name,
                 GPIO::Pin&  csPin,
                 Range       r = R250DPS );
         ~L3GD20();

         L3GD20& init      ( void );
         L3GD20& reset     ( void );
         L3GD20& enable    ( bool debug = false );
         L3GD20& disable   ( bool debug = false );

         L3GD20& omega     ( Vec3D& v );
         L3GD20& range     ( Range  r );
         L3GD20& calibrate ( void );


      protected:

         int16_t _calX;
         int16_t _calY;
         int16_t _calZ;
         Range   _range;

         void _WHO_AM_I ( uint8_t& id  );

   };

} /*qb50*/


#endif /*_QB50_SYS_DEVICE_SPI_L3GD20_H*/

/*EoF*/

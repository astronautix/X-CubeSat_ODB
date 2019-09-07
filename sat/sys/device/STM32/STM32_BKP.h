
#ifndef _QB50_SYS_DEVICE_STM32_BKP_H
#define _QB50_SYS_DEVICE_STM32_BKP_H

#include "STM32/STM32_Device.h"


namespace qb50 {

   class STM32_BKP : public STM32_Device
   {

      public:

         STM32_BKP( Bus& bus,
                 const uint32_t iobase,
                 const uint32_t periph,
                 const char    *name
         );

         ~STM32_BKP();

         STM32_BKP& init    ( void );
         STM32_BKP& enable  ( bool debug = false );
         STM32_BKP& disable ( bool debug = false );

   };

   extern qb50::STM32_BKP BKP;

}


#endif /*_QB50_SYS_DEVICE_STM32_BKP_H*/

/*EoF*/

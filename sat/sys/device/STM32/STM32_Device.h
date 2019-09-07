
#ifndef _QB50_SYS_DEVICE_STM32_DEVICE_H
#define _QB50_SYS_DEVICE_STM32_DEVICE_H

#include "Bus.h"
#include "Device.h"


namespace qb50 {

   class STM32_Device : public Device
   {

      public:

         STM32_Device( const char *name, Bus& bus, const uint32_t iobase, const uint32_t periph )
            : Device( name ), bus( bus ), iobase( iobase ), periph( periph )
         { ; }

         virtual ~STM32_Device()
         { ; }

         Bus&           bus;
         const uint32_t iobase;
         const uint32_t periph;

   };

} /* qb50 */


#endif /* _QB50_SYS_DEVICE_STM32_DEVICE_H */

/*EoF*/

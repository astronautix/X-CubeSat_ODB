
#ifndef _QB50_SYS_DEVICE_STM32_RTC_H
#define _QB50_SYS_DEVICE_STM32_RTC_H

#include "STM32/STM32_Device.h"
#include "STM32/STM32_PWR.h"
#include "device/RTC.h"


namespace qb50 {

   class STM32_RTC : public STM32_Device, public RTC
   {

      public:

         STM32_RTC( Bus& bus,
                    const uint32_t iobase,
                    const uint32_t periph,
                    const char    *name
         );

         ~STM32_RTC();

         STM32_RTC& init    ( void );
         STM32_RTC& enable  ( bool debug = false );
         STM32_RTC& disable ( bool debug = false );

         STM32_RTC& getTime ( Time &t );
         STM32_RTC& setTime ( Time &t );

      private:

         int  _enterInit ( void );
         void _exitInit  ( void );

   };

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_STM32_RTC_H*/

/*EoF*/

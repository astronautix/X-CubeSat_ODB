
#ifndef _QB50_SYS_DEVICE_STM32_TIMER_H
#define _QB50_SYS_DEVICE_STM32_TIMER_H

#include "STM32/STM32_Device.h"

#undef TIM


namespace qb50 {

   class STM32_Timer : public STM32_Device
   {

      public:

         /* Timer Channel Definitions */

         enum ChId {
            CH0  = 0x00, CH1  = 0x01,
            CH2  = 0x02, CH3  = 0x03
         };

         #include "STM32_Timer_Channel.h"

         STM32_Timer( Bus&        bus,
                      uint32_t    iobase,
                      uint32_t    periph,
                      const char *name,
                      uint32_t    presc,
                      uint32_t    period
         );

         ~STM32_Timer();

         STM32_Timer& init    ( void );
         STM32_Timer& enable  ( bool debug = false );
         STM32_Timer& disable ( bool debug = false );


      private:

         uint32_t _psc;
         uint32_t _arr;

    };

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_STM32_TIMER_H*/

/*EoF*/

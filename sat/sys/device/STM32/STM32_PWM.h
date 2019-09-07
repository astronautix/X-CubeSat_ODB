
#ifndef _QB50_SYS_DEVICE_STM32_PWM_H
#define _QB50_SYS_DEVICE_STM32_PWM_H

#include "STM32/STM32_Timer.h"
#include "STM32/STM32_GPIO.h"
#include "STM32/STM32_NVIC.h"

#undef TIM


namespace qb50 {

   class STM32_PWM : public STM32_Timer::Channel
   {

      public:

         enum Polarity
         {
            POS = 0,
            NEG = 1
         };

         STM32_PWM( STM32_Timer &tmr, const char *name, STM32_Timer::ChId id );
         ~STM32_PWM();

         STM32_PWM& init    ( void );
         STM32_PWM& enable  ( bool debug = false );
         STM32_PWM& disable ( bool debug = false );

         STM32_PWM& duty    ( unsigned dty );

    };

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_STM32_PWM_H*/

/*EoF*/

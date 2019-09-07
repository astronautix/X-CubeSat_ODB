
#ifndef _QB50_SYS_DEVICE_STM32_NVIC_H
#define _QB50_SYS_DEVICE_STM32_NVIC_H

#include <cstdint>


namespace qb50 {

   class STM32_NVIC
   {
      public:

         STM32_NVIC();
         ~STM32_NVIC();

         enum Priority {
            IPR0 = 0, IPR1,  IPR2,  IPR3,
            IPR4,     IPR5,  IPR6,  IPR7,
            IPR8,     IPR9,  IPR10, IPR11,
            IPR12,    IPR13, IPR14, IPR15,
         };

         STM32_NVIC& init    ( void );

         STM32_NVIC& enable  ( uint32_t IRQn, Priority sel = IPR15 );
         STM32_NVIC& clear   ( uint32_t IRQn );
         STM32_NVIC& disable ( uint32_t IRQn );
   };

   extern qb50::STM32_NVIC NVIC;

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_STM32_NVIC_H*/

/*EoF*/

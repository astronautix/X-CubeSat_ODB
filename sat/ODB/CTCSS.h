
#ifndef _QB50_CTCSS_H
#define _QB50_CTCSS_H

#include "devices.h"
#include "device/STM32/STM32_EXTI.h"


namespace qb50 {

   class CTCSS : public Device, public STM32_EXTIHandler
   {

      public:

         CTCSS( const char *name, STM32_GPIO::Pin& enPin, STM32_GPIO::Pin& dtPin );
         virtual ~CTCSS();

         CTCSS& init    ( void );
         CTCSS& enable  ( bool debug = false );
         CTCSS& disable ( bool debug = false );

         bool   wait    ( unsigned ms );
         bool   decTone ( void );

         void handle( STM32_EXTI::EXTIn );


      private:

         STM32_GPIO::Pin& _enPin;
         STM32_GPIO::Pin& _dtPin;

         xSemaphoreHandle _semDecTone;

   };

   extern qb50::CTCSS CTCS0;

} /* qb50 */


#endif /*_QB50_CTCSS_H*/

/*EoF*/

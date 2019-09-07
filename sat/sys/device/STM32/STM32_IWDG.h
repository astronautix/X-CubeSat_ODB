
#ifndef _QB50_SYS_DEVICE_STM32_IWDG_H
#define _QB50_SYS_DEVICE_STM32_IWDG_H

#include "Device.h"
#include "device/WDG.h"


namespace qb50{

   class STM32_IWDG : public Device, public WDG
   {

      public:

         STM32_IWDG( const uint32_t iobase, const char *name );
         ~STM32_IWDG();

         STM32_IWDG& init    ( void );
         STM32_IWDG& enable  ( bool debug = false );
         STM32_IWDG& disable ( bool debug = false );

         STM32_IWDG& refresh ( void );

      protected:

         const uint32_t _iobase;

   };

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_STM32_IWDG_H*/

/*EoF*/

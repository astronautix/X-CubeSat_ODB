
#ifndef _QB50_SYS_DEVICE_STM32_GPIO_H
#define _QB50_SYS_DEVICE_STM32_GPIO_H

#include "STM32/STM32_Device.h"
#include "GPIO.h"


namespace qb50 {

   class STM32_GPIO : public STM32_Device, public GPIO
   {
      public:

         /* modes */

         enum Mode {
            INPUT     = 0x00,
            OUTPUT    = 0x01,
            ALTERNATE = 0x02,
            ANALOG    = 0x03
         };

         /* alternate function mapping */

         enum Alt {
            AF0  = 0x00, RTC_50Hz = AF0,
                         MCO      = AF0,
                         TAMPER   = AF0,
                         SWJ      = AF0,
                         TRACE    = AF0,
            AF1  = 0x01, TIM1     = AF1,
                         TIM2     = AF1,
            AF2  = 0x02, TIM3     = AF2,
                         TIM4     = AF2,
                         TIM5     = AF2,
            AF3  = 0x03, TIM8     = AF3,
                         TIM9     = AF3,
                         TIM10    = AF3,
                         TIM11    = AF3,
            AF4  = 0x04, I2C1     = AF4,
                         I2C2     = AF4,
                         I2C3     = AF4,
            AF5  = 0x05, SPI1     = AF5,
                         SPI2     = AF5,
            AF6  = 0x06, SPI3     = AF6,
            AF7  = 0x07, UART1    = AF7,
                         UART2    = AF7,
                         UART3    = AF7,
            AF8  = 0x08, UART4    = AF8,
                         UART5    = AF8,
                         UART6    = AF8,
            AF9  = 0x09, CAN1     = AF9,
                         CAN2     = AF9,
                         TIM12    = AF9,
                         TIM13    = AF9,
                         TIM14    = AF9,
            AF10 = 0x0a, OTG_FS   = AF10,
                         OTG_HS   = AF10,
            AF11 = 0x0b, ETH      = AF11,
            AF12 = 0x0c, FSMC     = AF12,
                         OTG_HSFS = AF12,
                         OTG_SDIO = AF12,
            AF13 = 0x0d, DCMI     = AF13,
            AF14 = 0x0e,
            AF15 = 0x0f, EVENTOUT = AF15
         };

         /* output speed */

         enum OSpeed {
            SLOW         = 0x00, /*   2MHz */
            MEDIUM       = 0x01, /*  25MHz */
            FAST         = 0x02, /*  50MHz */
            HIGH         = 0x03  /* 100MHz */
         };

         /* output type */

         enum OType {
            PUSHPULL     = 0x00,
            OPENDRAIN    = 0x01
         };

         /* pull-up / pull-down */

         enum PullUpDn {
            NOPULL       = 0x00,
            PULLUP       = 0x01,
            PULLDN       = 0x02
         };

         #include "STM32/STM32_GPIO_Pin.h"

         STM32_GPIO( Bus& bus,
                     const uint32_t iobase,
                     const uint32_t periph,
                     const unsigned id,
                     const char *name,
                     const Pin *pins );
         ~STM32_GPIO();

         STM32_GPIO& init    ( void );
         STM32_GPIO& enable  ( bool debug = false );
         STM32_GPIO& disable ( bool debug = false );

         unsigned id ( void ) const { return _id;   }

      private:

         const unsigned _id;

   };

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_STM32_GPIO_H*/

/*EoF*/

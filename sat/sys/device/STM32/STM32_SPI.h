
#ifndef _QB50_SYS_DEVICE_STM32_SPI_H
#define _QB50_SYS_DEVICE_STM32_SPI_H

#include "STM32/STM32_Device.h"
#include "STM32/STM32_GPIO.h"
#include "STM32/STM32_DMA.h"
#include "SPI.h"


namespace qb50 {

   class STM32_SPI : public STM32_Device, public SPI
   {

      public:

         #include "STM32_SPI_Stream.h"

         enum ClkDiv {
            DIV2   = 0,
            DIV4   = 1,
            DIV8   = 2,
            DIV16  = 3,
            DIV32  = 4,
            DIV64  = 5,
            DIV128 = 6,
            DIV256 = 7
         };

         STM32_SPI( Bus&             bus,
                    const uint32_t   iobase,
                    const uint32_t   periph,
                    const char*      name,
                    Stream&          stMISO,
                    Stream&          stMOSI,
                    STM32_GPIO::Pin& clkPin,
                    STM32_GPIO::Alt  alt,
                    ClkDiv           div
         );

         ~STM32_SPI();

         STM32_SPI& init     ( void );
         STM32_SPI& enable   ( bool debug = false );
         STM32_SPI& disable  ( bool debug = false );

         STM32_SPI& master   ( void );
         STM32_SPI& slave    ( void );

         STM32_SPI& lock     ( void );
         STM32_SPI& unlock   ( void );

         /* synchronous transfer */

         STM32_SPI& xfer     ( const void *src, void *dst, size_t len );
         STM32_SPI& pollXfer ( const void *src, void *dst, size_t len );
         STM32_SPI& write    ( const void *src, size_t len );
         STM32_SPI& read     ( void *dst, size_t len );

         void isr ( void );


      private:

         STM32_SPI& _xfer   ( void );

         Stream&          _stMISO;
         Stream&          _stMOSI;
         STM32_GPIO::Pin& _clkPin;
         STM32_GPIO::Alt  _alt;
         ClkDiv           _div;

   };

} /* qb50 */


extern "C" {
   void SPI1_IRQHandler ( void );
   void SPI2_IRQHandler ( void );
   void SPI3_IRQHandler ( void );
}


#endif /*_QB50_SYS_DEVICE_STM32_SPI_H*/

/*EoF*/

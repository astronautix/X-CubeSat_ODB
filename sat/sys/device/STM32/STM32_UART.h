
#ifndef _QB50_SYS_DEVICE_STM32_UART_H
#define _QB50_SYS_DEVICE_STM32_UART_H

#include "STM32/STM32_Device.h"
#include "STM32/STM32_GPIO.h"
#include "system/FIFO.hpp"
#include "device/UART.h"

#include <task.h>


namespace qb50{

   class STM32_UART : public STM32_Device, public UART
   {

      public:

         STM32_UART( Bus& bus,
                     const uint32_t    iobase,
                     const uint32_t    periph,
                     const char       *name,
                     STM32_GPIO::Pin&  rxPin,
                     STM32_GPIO::Pin&  txPin,
                     const uint32_t    IRQn,
                     STM32_GPIO::Alt   alt
         );

         ~STM32_UART();

         STM32_UART& init     ( void );
         STM32_UART& enable   ( bool debug = false );
         STM32_UART& disable  ( bool debug = false );

         STM32_UART& baudRate ( unsigned rate );

         size_t      read     (       uint8_t *x, size_t len, int toms = -1 );
         size_t      readLine (       uint8_t *x, size_t len, int toms = -1 );
         size_t      write    ( const uint8_t *x, size_t len, int toms = -1 );

         void isr( void );

      private:

         xSemaphoreHandle _isrRXNE; /**< ISR semaphore bound to RXNE  */
         xSemaphoreHandle _isrTXE;  /**< ISR semaphore bound to TXE   */

         FIFO<uint8_t,64> _rxFIFO;  /**< receiver FIFO (input)        */
         FIFO<uint8_t,64> _txFIFO;  /**< transmitter FIFO (output)    */

         STM32_GPIO::Pin& _rxPin;
         STM32_GPIO::Pin& _txPin;
         const uint32_t   _IRQn;
         STM32_GPIO::Alt  _alt;

   };

} /* qb50 */


extern "C" {
   void USART1_IRQHandler ( void );
   void USART2_IRQHandler ( void );
   void USART3_IRQHandler ( void );
   void UART4_IRQHandler  ( void );
   void UART5_IRQHandler  ( void );
   void USART6_IRQHandler ( void );
}


#endif /*_QB50_SYS_DEVICE_STM32_UART_H*/

/*EoF*/

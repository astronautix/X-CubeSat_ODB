
#include "STM32/STM32_RCC.h"
#include "STM32/STM32_NVIC.h"
#include "STM32/STM32_UART.h"
#include "system/Logger.h"

#include <safe_stm32f4xx.h>

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_UART::STM32_UART( Bus&              bus,
                        const uint32_t    iobase,
                        const uint32_t    periph,
                        const char       *name,
                        STM32_GPIO::Pin&  rxPin,
                        STM32_GPIO::Pin&  txPin,
                        const uint32_t    IRQn,
                        STM32_GPIO::Alt   alt )
   : STM32_Device( name, bus, iobase, periph ),
     _rxPin ( rxPin ),
     _txPin ( txPin ),
     _IRQn  ( IRQn  ),
     _alt   ( alt   )
{
   _isrRXNE = xSemaphoreCreateBinary();
   _isrTXE  = xSemaphoreCreateBinary();
}


STM32_UART::~STM32_UART()
{
   vSemaphoreDelete( _isrTXE );
   vSemaphoreDelete( _isrRXNE );
}


//  - - - - - - - - - - - - - - - -  //
//  P U B L I C   I N T E R F A C E  //
//  - - - - - - - - - - - - - - - -  //

STM32_UART& STM32_UART::init( void )
{
   kprintf( "%s: STM32F4xx UART controller at %s, rx: %s, tx: %s\r\n",
            _name, bus.name, _rxPin.name(), _txPin.name() );

   return *this;
}


STM32_UART& STM32_UART::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   _rxPin.pullUp().alt( _alt );
   _txPin.pullUp().alt( _alt );

   USART_TypeDef *USARTx = (USART_TypeDef*)iobase;

   RCC.enable( this, debug );

   USARTx->CR1 = USART_CR1_TE | USART_CR1_RE;
   USARTx->CR2 = 0;
   USARTx->CR3 = 0;

   baudRate( 9600 );

   USARTx->CR1 |= ( USART_CR1_UE | USART_CR1_RXNEIE | USART_CR1_TXEIE );
   NVIC.enable( _IRQn );

   return *this;
}


STM32_UART& STM32_UART::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   USART_TypeDef *USARTx = (USART_TypeDef*)iobase;

   NVIC.disable( _IRQn );
   USARTx->CR1 &= ~( USART_CR1_UE | USART_CR1_RXNEIE | USART_CR1_TXEIE );
   RCC.disable( this, debug );

   return *this;
}


size_t STM32_UART::read( uint8_t *x, size_t len, int toms )
{
   size_t n = 0;

   TickType_t tk = toms < 0 ? portMAX_DELAY : ( toms / portTICK_RATE_MS );

   while( n < len ) {
      if( _rxFIFO.isEmpty() ) {
         if( xSemaphoreTake( _isrRXNE, tk ) == pdFALSE ) {
            return 0; /* timeout */
         }
         continue;
      }

      x[ n++ ] = _rxFIFO.pull();
   }

   return n;
}


size_t STM32_UART::readLine( uint8_t *x, size_t len, int toms )
{
   uint8_t ch = 0x00;
   size_t   n = 0;

   TickType_t tk = toms < 0 ? portMAX_DELAY : ( toms / portTICK_RATE_MS );

   while( n < len ) {
      if( _rxFIFO.isEmpty() ) {
         if( xSemaphoreTake( _isrRXNE, tk ) == pdFALSE ) {
            return 0; /* timeout */
         }
         continue;
      }

      ch = _rxFIFO.pull();

      if( ch == 0x0a ) continue;
      if( ch == 0x0d ) break;

      x[ n++ ] = ch;
   }

   while( ch != 0x0d ) {
      if( _rxFIFO.isEmpty() ) {
         if( xSemaphoreTake( _isrRXNE, tk ) == pdFALSE ) {
            return 0; /* timeout */
         }
         continue;
      }

      ch = _rxFIFO.pull();
   }

   return n;
}


size_t STM32_UART::write( const uint8_t *x, size_t len, int toms )
{
   USART_TypeDef *USARTx = (USART_TypeDef*)iobase;

   size_t n = 0;

   TickType_t tk = toms < 0 ? portMAX_DELAY : ( toms / portTICK_RATE_MS );

   while( n < len ) {
      if( _txFIFO.isFull() ) {
         if( xSemaphoreTake( _isrTXE, tk ) == pdFALSE ) {
            return 0; /* timeout */
         }
         continue;
      }
      (void)_txFIFO.push( x[ n++ ] );
      USARTx->CR1 |= USART_CR1_TXEIE;
   }

   return n;
}


STM32_UART& STM32_UART::baudRate( unsigned rate )
{
   USART_TypeDef *USARTx = (USART_TypeDef*)iobase;

   uint32_t idiv, fdiv;
   uint32_t tmp32;

   /* assuming oversampling by 16 (CR1.OVER8 = 0) */

   idiv   = ( 25 * RCC.freq( bus )) / ( 4 * rate );
   tmp32  = idiv / 100;
   fdiv   = idiv - ( 100 * tmp32 );
   tmp32  = tmp32 << 4;
   tmp32 |= ((( fdiv << 4 ) + 50 ) / 100 ) & 0x0f;

   USARTx->BRR = (uint16_t)tmp32;

   kprintf( "%s: Baud rate set to %u\r\n", _name, rate );

   return *this;
}


//  - - - - - - - - - - - -  //
//  I S R   H A N D L E R S  //
//  - - - - - - - - - - - -  //

void STM32_UART::isr( void )
{
   USART_TypeDef *USARTx = (USART_TypeDef*)iobase;
   portBASE_TYPE hpTask  = pdFALSE;

   uint16_t SR = USARTx->SR;
   uint16_t DR = USARTx->DR;

   if( SR & USART_SR_RXNE ) {
      USARTx->SR &= ~USART_SR_RXNE;
      if( !_rxFIFO.isFull() ) {
         (void)_rxFIFO.push( DR & 0xff );
         xSemaphoreGiveFromISR( _isrRXNE, &hpTask );
      }
   }

   if( SR & USART_SR_TXE ) {
      USARTx->SR &= ~USART_SR_TXE;
      if( _txFIFO.isEmpty() ) {
         USARTx->CR1 &= ~USART_CR1_TXEIE;
      } else {
         if( _txFIFO.isFull() ) {
            xSemaphoreGiveFromISR( _isrTXE, &hpTask );
         }
         USARTx->DR = _txFIFO.pull();
      }
   }

   if( hpTask == pdTRUE )
      portEND_SWITCHING_ISR( hpTask );
}

/*EoF*/

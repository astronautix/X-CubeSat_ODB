
#include "STM32/STM32_RCC.h"
#include "STM32/STM32_SPI.h"
#include "system/Logger.h"

#include <safe_stm32f4xx.h>

/*
 * STM32_SPI_HARD_LIMIT - hard limit for busy-wait loops
 * 4000000 = about 7076ms @ 16MHz
 *   10000 = about 18ms @ 16Mhz
 */

#define STM32_SPI_HARD_LIMIT 10000

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_SPI::STM32_SPI( Bus&              bus,
                      const uint32_t    iobase,
                      const uint32_t    periph,
                      const char       *name,
                      Stream&           stMISO,
                      Stream&           stMOSI,
                      STM32_GPIO::Pin&  clkPin,
                      STM32_GPIO::Alt   alt,
                      STM32_SPI::ClkDiv div )
   : STM32_Device( name, bus, iobase, periph ),
     _stMISO( stMISO ),
     _stMOSI( stMOSI ),
     _clkPin( clkPin ),
     _alt( alt ),
     _div( div )
{ ; }


STM32_SPI::~STM32_SPI()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

STM32_SPI& STM32_SPI::init( void )
{
   _clkPin.pullDn().alt( _alt );

   _stMISO.init();
   _stMOSI.init();

   kprintf( "%s: STM32F4xx SPI controller at %s, clk: %s, miso: %s, mosi: %s\r\n",
            _name, bus.name, _clkPin.name(), _stMISO._pin.name(), _stMOSI._pin.name() );

   return *this;
}


STM32_SPI& STM32_SPI::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   SPI_TypeDef *SPIx = (SPI_TypeDef*)iobase;

   _stMISO.enable( debug );
   _stMOSI.enable( debug );

   RCC.enable( this, debug );

   SPIx->CR1 &= ~SPI_CR1_SPE;

   SPIx->CR1 = SPI_CR1_MSTR  /* device is master      */
             | _div << 3     /* baud rate control     */
             | SPI_CR1_SSI   /* internal slave select */
             | SPI_CR1_SSM   /* software slave mgmt.  */
             ;

   SPIx->CR2 |= ( SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN );
   SPIx->CR1 |= SPI_CR1_SPE;

   return *this;
}


STM32_SPI& STM32_SPI::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   SPI_TypeDef *SPIx = (SPI_TypeDef*)iobase;

   SPIx->CR1 &= ~SPI_CR1_SPE;

   RCC.disable( this, debug );

   _stMOSI.disable( debug );
   _stMISO.disable( debug );

   return *this;
}


STM32_SPI& STM32_SPI::master( void )
{
   SPI_TypeDef *SPIx = (SPI_TypeDef*)iobase;
   SPIx->CR1 |= SPI_CR1_MSTR;

   return *this;
}


STM32_SPI& STM32_SPI::slave( void )
{
   SPI_TypeDef *SPIx = (SPI_TypeDef*)iobase;
   SPIx->CR1 &= ~SPI_CR1_MSTR;

   return *this;
}


STM32_SPI& STM32_SPI::lock( void )
{
   (void)Device::lock();
   return *this;
}


STM32_SPI& STM32_SPI::unlock( void )
{
   (void)Device::unlock();
   return *this;
}


STM32_SPI& STM32_SPI::xfer( const void *src, void *dst, size_t len )
{
   SPI_TypeDef *SPIx = (SPI_TypeDef*)iobase;

   _stMISO.dmaStream.pAddr     ((uint32_t)&( SPIx->DR ))
                    .m0Addr    ((uint32_t)     dst     )
                    .counter   ((uint32_t)     len     )
                    .pIncMode  ( STM32_DMA::FIXED      )
                    .mIncMode  ( STM32_DMA::INCR       )
                    .direction ( STM32_DMA::P2M        );

   _stMOSI.dmaStream.pAddr     ((uint32_t)&( SPIx->DR ))
                    .m0Addr    ((uint32_t)     src     )
                    .counter   ((uint32_t)     len     )
                    .pIncMode  ( STM32_DMA::FIXED      )
                    .mIncMode  ( STM32_DMA::INCR       )
                    .direction ( STM32_DMA::M2P        );

   return _xfer();
}


STM32_SPI& STM32_SPI::write( const void *src, size_t len )
{
   SPI_TypeDef *SPIx = (SPI_TypeDef*)iobase;
   uint8_t dummy;

   _stMISO.dmaStream.pAddr     ((uint32_t)&( SPIx->DR ))
                    .m0Addr    ((uint32_t)   &dummy    )
                    .counter   ((uint32_t)     len     )
                    .pIncMode  ( STM32_DMA::FIXED      )
                    .mIncMode  ( STM32_DMA::FIXED      )
                    .direction ( STM32_DMA::P2M        );

   _stMOSI.dmaStream.pAddr     ((uint32_t)&( SPIx->DR ))
                    .m0Addr    ((uint32_t)     src     )
                    .counter   ((uint32_t)     len     )
                    .pIncMode  ( STM32_DMA::FIXED      )
                    .mIncMode  ( STM32_DMA::INCR       )
                    .direction ( STM32_DMA::M2P        );

   return _xfer();
}


STM32_SPI& STM32_SPI::read( void *dst, size_t len )
{
   SPI_TypeDef *SPIx = (SPI_TypeDef*)iobase;
   uint8_t dummy = 0xff;

   _stMISO.dmaStream.pAddr     ((uint32_t)&( SPIx->DR ))
                    .m0Addr    ((uint32_t)     dst     )
                    .counter   ((uint32_t)     len     )
                    .pIncMode  ( STM32_DMA::FIXED      )
                    .mIncMode  ( STM32_DMA::INCR       )
                    .direction ( STM32_DMA::P2M        );

   _stMOSI.dmaStream.pAddr     ((uint32_t)&( SPIx->DR ))
                    .m0Addr    ((uint32_t)   &dummy    )
                    .counter   ((uint32_t)     len     )
                    .pIncMode  ( STM32_DMA::FIXED      )
                    .mIncMode  ( STM32_DMA::FIXED      )
                    .direction ( STM32_DMA::M2P        );

   return _xfer();
}

/* polling methods */

STM32_SPI& STM32_SPI::pollXfer( const void *src, void *dst, size_t len )
{
   SPI_TypeDef *SPIx = (SPI_TypeDef*)iobase;
   size_t i;
   int n;
   uint8_t rx;

   for( i = 0 ; i < len ; ++i ) {

      for( n = 0 ; n < STM32_SPI_HARD_LIMIT ; ++n ) {
         if( SPIx->SR & SPI_SR_TXE ) break;
      }

      if( n == STM32_SPI_HARD_LIMIT ) {
         kprintf( RED( "%s: timeout in STM32_SPI::pollXfer() - SPI_SR_TXE" ) "\r\n", _name );
         break;
      }

      if( src == NULL ) {
         SPIx->DR = 0xff;
      } else {
         SPIx->DR = ((uint8_t*)src)[ i ];
      }

      for( n = 0 ; n < STM32_SPI_HARD_LIMIT ; ++n ) {
         if( SPIx->SR & SPI_SR_RXNE ) break;
      }

      if( n == STM32_SPI_HARD_LIMIT ) {
         kprintf( RED( "%s: timeout in STM32_SPI::pollXfer() - SPI_SR_RXNE" ) "\r\n", _name );
         break;
      }

      rx = SPIx->DR & 0xff;
      if( dst != NULL ) {
         ((uint8_t*)dst)[ i ] = rx;
      }
   }

   if( i < len ) {
      kprintf( RED( "%s: got %u/%u bytes" ) "\r\n", _name, i, len );
   }

   return *this;
}


//  - - - - - - - - - - - - - - -  //
//  P R I V A T E   M E T H O D S  //
//  - - - - - - - - - - - - - - -  //

STM32_SPI& STM32_SPI::_xfer( void )
{
   /* enable transmission for both RX and TX streams */

   _stMISO.dmaStream.start();
   _stMOSI.dmaStream.start();

   /*
    * block until the transmission is complete,
    * FreeRTOS will let other threads run in the meantime.
    */

   _stMOSI.dmaStream.wait();
   _stMISO.dmaStream.wait();

   /* we're done: disable both streams and return */

   _stMOSI.dmaStream.stop();
   _stMISO.dmaStream.stop();

   return *this;
}


//  - - - - - - - - - - - -  //
//  I S R   H A N D L E R S  //
//  - - - - - - - - - - - -  //

void STM32_SPI::isr( void )
{ ; }

/*EoF*/

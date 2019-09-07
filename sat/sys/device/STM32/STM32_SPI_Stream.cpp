
#include "STM32/STM32_SPI.h"
#include "system/Logger.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_SPI::Stream::Stream( STM32_DMA::Stream& dmaStream,
                           STM32_DMA::Channel dmaChannel,
                           const char      *name,
                           STM32_GPIO::Pin& pin,
                           STM32_GPIO::Alt  alt )
   : Device( name ),
     dmaStream( dmaStream ),
     dmaChannel( dmaChannel ),
     _pin( pin ),
     _alt( alt )
{ ; }


STM32_SPI::Stream::~Stream()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

STM32_SPI::Stream& STM32_SPI::Stream::init( void )
{
   _pin.pullUp().alt( _alt );
   return *this;
}


STM32_SPI::Stream& STM32_SPI::Stream::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   dmaStream.enable( debug )
            .channel( dmaChannel );

   return *this;
}


STM32_SPI::Stream& STM32_SPI::Stream::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   dmaStream.disable( debug );

   return *this;
}

/*EoF*/

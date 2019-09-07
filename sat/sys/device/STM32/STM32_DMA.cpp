
#include "STM32/STM32_DMA.h"
#include "STM32/STM32_RCC.h"
#include "system/Logger.h"

#include <safe_stm32f4xx.h>

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_DMA::STM32_DMA( Bus& bus,
          const uint32_t iobase,
          const uint32_t periph,
          const char *name )
   : STM32_Device( name, bus, iobase, periph )
{ ; }


STM32_DMA::~STM32_DMA()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

STM32_DMA& STM32_DMA::init( void )
{
   kprintf( "%s: STM32F4xx DMA controller at %s\r\n", _name, bus.name );
   return *this;
}


STM32_DMA& STM32_DMA::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   RCC.enable( this, debug );

   return *this;
}


STM32_DMA& STM32_DMA::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   RCC.disable( this, debug );

   return *this;
}

/*EoF*/

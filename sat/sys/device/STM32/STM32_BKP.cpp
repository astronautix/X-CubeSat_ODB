
#include "STM32/STM32_RCC.h"
#include "STM32/STM32_PWR.h"
#include "STM32/STM32_BKP.h"
#include "system/Logger.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_BKP::STM32_BKP( Bus& bus,
                const uint32_t iobase,
                const uint32_t periph,
                const char    *name )
   : STM32_Device( name, bus, iobase, periph )
{ ; }


STM32_BKP::~STM32_BKP()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

STM32_BKP& STM32_BKP::init( void )
{
   kprintf( "%s: STM32F4xx Backup Domain controller at %s\r\n", _name, bus.name );

   return *this;
}


STM32_BKP& STM32_BKP::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   PWR.enable( debug );
   RCC.enable( this, debug );
   PWR.enableBRE( debug );

   return *this;
}


STM32_BKP& STM32_BKP::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   PWR.disableBRE( debug );
   RCC.disable( this, debug );
   PWR.disable( debug );

   return *this;
}


/*EoF*/

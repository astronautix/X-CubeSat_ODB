
#include "STM32/STM32_IWDG.h"
#include "system/Logger.h"

#include <safe_stm32f4xx.h>


using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_IWDG::STM32_IWDG( const uint32_t iobase, const char *name )
   : Device( name ), _iobase( iobase )
{ ; }


STM32_IWDG::~STM32_IWDG()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

// XXX lock/unlock

STM32_IWDG& STM32_IWDG::init( void )
{
   IWDG_TypeDef *WDGx = (IWDG_TypeDef*)_iobase;

   kprintf( "%s: STM32F4xx Independent Watchdog\r\n", _name );

   WDGx->KR  = 0x5555;
   WDGx->PR  = 0x0003; // prescaler: /32
   WDGx->RLR = 0x0fff; // reload

   return *this;
}

// XXX lock/unlock

STM32_IWDG& STM32_IWDG::enable( bool debug )
{
   IWDG_TypeDef *WDGx = (IWDG_TypeDef*)_iobase;

   WDGx->KR  = 0x5555;
   WDGx->KR  = 0xcccc;

   if( debug ) {
      kprintf( "%s: enabled\r\n", _name );
   }

   return *this;
}

// XXX lock/unlock

STM32_IWDG& STM32_IWDG::disable( bool debug )
{
   (void)debug;

   kprintf( RED( "%s: disable(): not supported" ) "\r\n", _name );

   return *this;
}

// XXX lock/unlock

STM32_IWDG& STM32_IWDG::refresh( void )
{
   IWDG_TypeDef *WDGx = (IWDG_TypeDef*)_iobase;

   WDGx->KR  = 0x5555;
   WDGx->KR  = 0xaaaa;

   return *this;
}

/*EoF*/

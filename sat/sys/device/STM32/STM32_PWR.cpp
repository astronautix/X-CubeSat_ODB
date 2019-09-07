
#include "STM32/STM32_RCC.h"
#include "STM32/STM32_PWR.h"
#include "system/Logger.h"

#include <safe_stm32f4xx.h>

#define PWR_BRR_HARD_LIMIT 100000


using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_PWR::STM32_PWR( Bus& bus,
                      const uint32_t iobase,
                      const uint32_t periph,
                      const char    *name )
   : STM32_Device( name, bus, iobase, periph )
{ ; }


STM32_PWR::~STM32_PWR()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

STM32_PWR& STM32_PWR::init( void )
{
   kprintf( "%s: STM32F4xx Power controller at %s\r\n", _name, bus.name );
   return *this;
}


STM32_PWR& STM32_PWR::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   RCC.enable( this, debug );

   return *this;
}


STM32_PWR& STM32_PWR::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   RCC.disable( this, debug );

   return *this;
}


STM32_PWR& STM32_PWR::enableBRE( bool debug )
{
   PWR_TypeDef *PWRx = (PWR_TypeDef*)iobase;

/*
   if(( PWRx->CSR & PWR_CSR_BRR ) != 0 )
      return *this;
*/

   PWRx->CR  |= PWR_CR_DBP;
   PWRx->CSR |= PWR_CSR_BRE;

   for( int n = 0 ; n < PWR_BRR_HARD_LIMIT ; ++n ) {
      if(( PWRx->CSR & PWR_CSR_BRR ) != 0 )
         break;
   }

   if(( PWRx->CSR & PWR_CSR_BRR ) == 0 ) {
      kprintf( RED( "%s: timeout in STM32_PWR::enableBRE()" ) "\r\n", _name );
      return *this;
   }

   if( debug ) {
      kprintf( "%s: Backup regulator enabled\r\n", _name );
   }

   return *this;
}


STM32_PWR& STM32_PWR::disableBRE( bool debug )
{
   PWR_TypeDef *PWRx = (PWR_TypeDef*)iobase;

/*
   if(( PWRx->CSR & PWR_CSR_BRR ) == 0 )
      return *this;
*/

   PWRx->CSR &= ~PWR_CSR_BRE;
   PWRx->CR  &= ~PWR_CR_DBP;

   if( debug ) {
      kprintf( "%s: Backup regulator disabled\r\n", _name );
   }


   return *this;
}

/*EoF*/

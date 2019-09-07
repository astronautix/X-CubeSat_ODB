
#include "STM32/STM32_RCC.h"
#include "STM32/STM32_Timer.h"
#include "system/Logger.h"
#include "stm32f4xx_tim.h"
//#include "safe_stm32f4xx.h"

#undef RCC

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_Timer::STM32_Timer( Bus& bus,
              uint32_t    iobase,
              uint32_t    periph,
              const char *name,
              uint32_t    presc,
              uint32_t    period )
   : STM32_Device( name, bus, iobase, periph ), _psc( presc ), _arr( period )
{ ; }


STM32_Timer::~STM32_Timer()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

STM32_Timer& STM32_Timer::init( void )
{
   kprintf( "%s: System timer controller at %s\r\n", _name, bus.name );
   return *this;
}


STM32_Timer& STM32_Timer::enable( bool debug )
{
   uint32_t tmp32;

   if( _incRef() > 0 )
      return *this;

   TIM_TypeDef *TIMx = (TIM_TypeDef*)iobase;

   RCC.enable( this, debug );

   /* set counter mode */

   tmp32 = TIMx->CR1;
   tmp32 &= ~( TIM_CR1_DIR | TIM_CR1_CMS );
   tmp32 |= TIM_CounterMode_Up;
   TIMx->CR1 = tmp32;

   /* set autoreload (period) */

   TIMx->ARR = _arr;

   /* set prescaler */

   TIMx->PSC = _psc;

   /* enable timer */

   TIMx->CR1 |= TIM_CR1_CEN;

   return *this;
}


STM32_Timer& STM32_Timer::disable( bool debug )
{
   uint32_t tmp32;

   if( _decRef() > 0 )
      return *this;

   TIM_TypeDef *TIMx = (TIM_TypeDef*)iobase;

   /* disable timer */

   tmp32 = TIMx->CR1;
   tmp32 &= ~TIM_CR1_CEN;
   TIMx->CR1 = tmp32;

   RCC.disable( this, debug );

   return *this;
}

/*EoF*/

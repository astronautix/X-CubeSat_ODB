
#include "STM32/STM32_Timer.h"
#include "system/Logger.h"
#include "safe_stm32f4xx.h"

using namespace qb50;


#define _CCER_CCE  ((uint16_t)( 0x0001 << ( 4 * _cid )))  /* Capture/Compare output enable                 */
#define _CCER_CCP  ((uint16_t)( 0x0002 << ( 4 * _cid )))  /* Capture/Compare output Polarity               */
#define _CCER_CCNE ((uint16_t)( 0x0004 << ( 4 * _cid )))  /* Capture/Compare Complementary output enable   */
#define _CCER_CCNP ((uint16_t)( 0x0008 << ( 4 * _cid )))  /* Capture/Compare Complementary output Polarity */

#define _CCER_MASK ((uint16_t)( 0x000f << ( _cid  <<  2 )))
#define _CCMR_MASK ((uint16_t)( 0x00ff << ( _cid & 0x01 )))


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_Timer::Channel::Channel( STM32_Timer& tmr, const char *name, ChId cid )
: Device( name ), _tmr( tmr ), _cid( cid )
{ ; }


STM32_Timer::Channel::~Channel()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

STM32_Timer::Channel&
STM32_Timer::Channel::init( void )
{
   uint16_t ccer, ccmr /*, cr2*/ ;

   (void)enable();

   TIM_TypeDef *TIMx = (TIM_TypeDef*)_tmr.iobase;

   /* read c/c enable register */

   ccer = TIMx->CCER;
   ccer &= ~_CCER_MASK;
   TIMx->CCER = ccer;  // disable c/c

   /* read o/c mode register */

   switch( _cid ) {
      case CH0:
      case CH1:
         ccmr = TIMx->CCMR1;
         ccmr &= ~_CCMR_MASK;
         TIMx->CCMR1 = ccmr; // reset o/c mode
         break;

      case CH2:
      case CH3:
         ccmr = TIMx->CCMR2;
         ccmr &= ~_CCMR_MASK;
         TIMx->CCMR2 = ccmr; // reset o/c mode
         break;
   }

   /* read control register */

 //cr2 = TIMx->CR2;

   (void)disable();

   return *this;
}


STM32_Timer::Channel&
STM32_Timer::Channel::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   _tmr.enable( debug );

   return *this;
}


STM32_Timer::Channel&
STM32_Timer::Channel::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   _tmr.disable( debug );

   return *this;
}

/*EoF*/

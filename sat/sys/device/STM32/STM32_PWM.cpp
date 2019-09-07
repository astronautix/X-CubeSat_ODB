
#include "STM32/STM32_PWM.h"
#include "system/Logger.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_PWM::STM32_PWM( STM32_Timer &tmr, const char *name, STM32_Timer::ChId id )
   : STM32_Timer::Channel( tmr, name, id )
{ ; }


STM32_PWM::~STM32_PWM()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

STM32_PWM& STM32_PWM::init( void )
{
   STM32_Timer::Channel::init();

   return *this;
}


STM32_PWM& STM32_PWM::enable( bool debug )
{
   STM32_Timer::Channel::enable( debug );

   return *this;
}


STM32_PWM& STM32_PWM::disable( bool debug )
{
   STM32_Timer::Channel::disable( debug );

   return *this;
}


STM32_PWM& STM32_PWM::duty( unsigned dty )
{
   (void)dty;

   return *this;
}

/*EoF*/

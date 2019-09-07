
#include "STM32/STM32_NVIC.h"
#include "system/Logger.h"

#include <safe_stm32f4xx.h>


using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_NVIC::STM32_NVIC()
{ ; }


STM32_NVIC::~STM32_NVIC()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

STM32_NVIC& STM32_NVIC::init( void )
{
   /*
    * See the STM32F4 Programming Manual sec. 4.4.5,
    * "Application interrupt and reset control register" (pp. 212-213)
    *
    * "To write to this register, you must write 0x5FA to the
    *  VECTKEY Field, otherwise the processor ignores the write."
    */

   kprintf( "NVIC: STM32F4xx Nested Vectored Interrupt Controller\r\n" );

   SCB->AIRCR = ( 0x05fa << 16 )  /* VECTKEY  */
              | (   0x03 <<  8 ); /* PRIGROUP */

   return *this;
}


STM32_NVIC& STM32_NVIC::enable( uint32_t IRQn, Priority sel )
{
   NVIC_Type *STM32_NVICx = (NVIC_Type*)NVIC_BASE;

   STM32_NVICx->IP[ IRQn ] = sel << 4;
   STM32_NVICx->ISER[ IRQn >> 0x05 ] = 0x01 << ( IRQn & 0x1f );
   clear( IRQn );

   return *this;
}


STM32_NVIC& STM32_NVIC::disable( uint32_t IRQn )
{
   (void)IRQn; /* XXX */
   return *this;
}


STM32_NVIC& STM32_NVIC::clear( uint32_t IRQn )
{
   NVIC_Type *STM32_NVICx = (NVIC_Type*)NVIC_BASE;
   STM32_NVICx->ICPR[ IRQn >> 0x05 ] = 0x01 << ( IRQn & 0x1f );

   return *this;
}

/*EoF*/

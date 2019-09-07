
#include "STM32/STM32_EXTI.h"
#include "STM32/STM32_NVIC.h"
#include "STM32/STM32_GPIO.h"
#include "system/Logger.h"

#include <safe_stm32f4xx.h>

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_EXTI::STM32_EXTI()
{
   ;
}


STM32_EXTI::~STM32_EXTI()
{
   ;
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

STM32_EXTI& STM32_EXTI::init( void )
{
   for( int i = 0; i < 16; i++ )
      _extiHandler[i] = (STM32_EXTIHandler*)0;

   kprintf( "EXTI: STM32F4xx External Interrupt Controller\r\n" );

   return *this;
}


void STM32_EXTI::registerHandler( STM32_GPIO::Pin& pin, STM32_EXTIHandler *handler, Edge edge )
{
    EXTI_TypeDef *EXTIx = (EXTI_TypeDef*)EXTI_BASE;

    const unsigned pinId  = pin.id();
    const unsigned portId = pin.portId();

    const unsigned pinHi  = ( pinId >> 2 ) & 0x03;
    const unsigned pinLo  =   pinId        & 0x03;

    register uint32_t tmp32;

    /* register handler */

    _extiHandler[ pinId ] = handler;

    /* set pin as input */

    pin.in().noPull();
/*
    if( edge == FALLING ) {
        pin.pullUp();
    } else {
        pin.pullDn();
    }
*/

    /* XXX enable clock for SYSCFG */

    RCC_TypeDef    *RCCx    = (    RCC_TypeDef* )RCC_BASE;
    SYSCFG_TypeDef *SYSCFGx = ( SYSCFG_TypeDef* )SYSCFG_BASE;

    RCCx->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    /* setup the external interrupt */

    tmp32  = SYSCFGx->EXTICR[ pinHi ];
    tmp32 &= ~( 0x000f << ( 4 * pinLo ));
    tmp32 |=    portId << ( 4 * pinLo );
    SYSCFGx->EXTICR[ pinHi ] = tmp32;

    uint32_t exti_mask = 0x0001 << pinId;

    EXTIx->IMR |= exti_mask;

    switch( edge ) {
        case RISING:
            EXTIx->RTSR |=  exti_mask;
            EXTIx->FTSR &= ~exti_mask;
            break;

        case FALLING:
            EXTIx->RTSR &= ~exti_mask;
            EXTIx->FTSR |=  exti_mask;
            break;

        case BOTH:
        default:
            EXTIx->RTSR |=  exti_mask;
            EXTIx->FTSR |=  exti_mask;
            break;
    }

#if 0
    EXTIx->SWIER |= exti_mask; // XXX
#endif
    EXTIx->PR    |= exti_mask;

    /* XXX - NVIC IRQ channel configuration*/

    NVIC.enable( EXTI0_IRQn     );
    NVIC.enable( EXTI1_IRQn     );
    NVIC.enable( EXTI2_IRQn     );
    NVIC.enable( EXTI3_IRQn     );
    NVIC.enable( EXTI4_IRQn     );
    NVIC.enable( EXTI9_5_IRQn   );
    NVIC.enable( EXTI15_10_IRQn );

}


//  - - - - - - - - - - - -  //
//  I S R   H A N D L E R S  //
//  - - - - - - - - - - - -  //

void STM32_EXTI::isr( EXTIn i )
{
    STM32_EXTIHandler *handler = _extiHandler[ i ];

    if( handler != 0 )
        handler->handle( i );
}


//  - - - - - - - - - - - - - - - - -  //
//  E X T I _ I R Q   H A N D L E R S  //
//  - - - - - - - - - - - - - - - - -  //

/* EXTI0_IRQHandler */

extern "C" void EXTI0_IRQHandler( void )
{
    EXTI_TypeDef *EXTIx = (EXTI_TypeDef*)EXTI_BASE;

    EXTIx->PR |= EXTI_PR_PR0;
    qb50::EXTI.isr( STM32_EXTI::EXTI0 );
}

/* EXTI1_IRQHandler */

extern "C" void EXTI1_IRQHandler( void )
{
    EXTI_TypeDef *EXTIx = (EXTI_TypeDef*)EXTI_BASE;

    EXTIx->PR |= EXTI_PR_PR1;
    qb50::EXTI.isr( STM32_EXTI::EXTI1 );
}

/* EXTI2_IRQHandler */

extern "C" void EXTI2_IRQHandler( void )
{
    EXTI_TypeDef *EXTIx = (EXTI_TypeDef*)EXTI_BASE;

    EXTIx->PR |= EXTI_PR_PR2;
    qb50::EXTI.isr( STM32_EXTI::EXTI2 );
}

/* EXTI3_IRQHandler */

extern "C" void EXTI3_IRQHandler( void )
{
    EXTI_TypeDef *EXTIx = (EXTI_TypeDef*)EXTI_BASE;

    EXTIx->PR |= EXTI_PR_PR3;
    qb50::EXTI.isr( STM32_EXTI::EXTI3 );
}

/* EXTI4_IRQHandler */

extern "C" void EXTI4_IRQHandler( void )
{
    EXTI_TypeDef *EXTIx = (EXTI_TypeDef*)EXTI_BASE;

    EXTIx->PR |= EXTI_PR_PR4;
    qb50::EXTI.isr( STM32_EXTI::EXTI4 );
}

/* EXTI9_5_IRQHandler */

extern "C" void EXTI9_5_IRQHandler( void )
{
    EXTI_TypeDef *EXTIx = (EXTI_TypeDef*)EXTI_BASE;

    if( EXTIx->PR & EXTI_PR_PR5 )
    {
         EXTIx->PR |= EXTI_PR_PR5;
         qb50::EXTI.isr( STM32_EXTI::EXTI5 );
    }

    if( EXTIx->PR & EXTI_PR_PR6 )
    {
         EXTIx->PR |= EXTI_PR_PR6;
         qb50::EXTI.isr( STM32_EXTI::EXTI6 );
    }

    if( EXTIx->PR & EXTI_PR_PR7 )
    {
         EXTIx->PR |= EXTI_PR_PR7;
         qb50::EXTI.isr( STM32_EXTI::EXTI7 );
    }

    if( EXTIx->PR & EXTI_PR_PR8 )
    {
         EXTIx->PR |= EXTI_PR_PR8;
         qb50::EXTI.isr( STM32_EXTI::EXTI8 );
    }

    if( EXTIx->PR & EXTI_PR_PR9 )
    {
         EXTIx->PR |= EXTI_PR_PR9;
         qb50::EXTI.isr( STM32_EXTI::EXTI9 );
    }

}

/* EXTI15_10_IRQHandler */

extern "C" void EXTI15_10_IRQHandler( void )
{
    EXTI_TypeDef *EXTIx = (EXTI_TypeDef*)EXTI_BASE;

    if( EXTIx->PR & EXTI_PR_PR10 )
    {
         EXTIx->PR |= EXTI_PR_PR10;
         qb50::EXTI.isr( STM32_EXTI::EXTI10 );
    }

    if( EXTIx->PR & EXTI_PR_PR11 )
    {
         EXTIx->PR |= EXTI_PR_PR11;
         qb50::EXTI.isr( STM32_EXTI::EXTI11 );
    }

    if( EXTIx->PR & EXTI_PR_PR12 )
    {
         EXTIx->PR |= EXTI_PR_PR12;
         qb50::EXTI.isr( STM32_EXTI::EXTI12 );
    }

    if( EXTIx->PR & EXTI_PR_PR13 )
    {
         EXTIx->PR |= EXTI_PR_PR13;
         qb50::EXTI.isr( STM32_EXTI::EXTI13 );
    }

    if( EXTIx->PR & EXTI_PR_PR14 )
    {
         EXTIx->PR |= EXTI_PR_PR14;
         qb50::EXTI.isr( STM32_EXTI::EXTI14 );
    }

    if( EXTIx->PR & EXTI_PR_PR15 )
    {
         EXTIx->PR |= EXTI_PR_PR15;
         qb50::EXTI.isr( STM32_EXTI::EXTI15 );
    }

}

/*EoF*/

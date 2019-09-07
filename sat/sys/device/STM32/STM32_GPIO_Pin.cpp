
#include "STM32/STM32_GPIO.h"
#include "STM32/STM32_EXTI.h"
#include <safe_stm32f4xx.h>

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_GPIO::Pin::Pin( STM32_GPIO& gpio, const unsigned id, const char* name, const uint16_t mask )
   : GPIO::Pin( name ), _gpio( gpio ), _id( id ), _mask( mask )
{ ; }


STM32_GPIO::Pin::~Pin()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

STM32_GPIO::Pin& STM32_GPIO::Pin::on( void )
{
   GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)_gpio.iobase;
   GPIOx->BSRRL = (uint16_t)( _mask & 0xffff );

   return *this;
}


STM32_GPIO::Pin& STM32_GPIO::Pin::off( void )
{
   GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)_gpio.iobase;
   GPIOx->BSRRH = (uint16_t)( _mask & 0xffff );

   return *this;
}


STM32_GPIO::Pin& STM32_GPIO::Pin::toggle( void )
{
   GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)_gpio.iobase;
   GPIOx->ODR ^= (uint16_t)( _mask & 0xffff );

   return *this;
}


bool STM32_GPIO::Pin::read( void )
{
   GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)_gpio.iobase;
   return ( GPIOx->IDR & _mask ) != 0;
}


STM32_GPIO::Pin& STM32_GPIO::Pin::mode( Mode mode )
{
   GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)_gpio.iobase;
   register uint32_t tmp32;

   tmp32  = GPIOx->MODER;
   tmp32 &= ~( 0x03 << ( _id * 2 ));
   tmp32 |=  ( mode << ( _id * 2 ));

   GPIOx->MODER = tmp32;

   return *this;
}


STM32_GPIO::Pin& STM32_GPIO::Pin::alt( Alt alt )
{
   GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)_gpio.iobase;
   register uint32_t tmp32;

   mode( ALTERNATE );

   tmp32  = GPIOx->AFR[ _id >> 3 ];
   tmp32 &= ~( 0x0f << (( _id & 0x07 ) * 4 ));
   tmp32 |=  ( alt  << (( _id & 0x07 ) * 4 ));

   GPIOx->AFR[ _id >> 3 ] = tmp32;

   return *this;
}


STM32_GPIO::Pin& STM32_GPIO::Pin::oSpeed( OSpeed speed )
{
   GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)_gpio.iobase;
   register uint32_t tmp32;

   tmp32  = GPIOx->OSPEEDR;
   tmp32 &= ~( 0x03  << ( _id * 2 ));
   tmp32 |=  ( speed << ( _id * 2 ));

   GPIOx->OSPEEDR = tmp32;

   return *this;
}


STM32_GPIO::Pin& STM32_GPIO::Pin::oType( OType type )
{
   GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)_gpio.iobase;
   register uint32_t tmp32;

   tmp32  = GPIOx->OTYPER;
   tmp32 &= ~( 0x01 << _id );
   tmp32 |=  ( type << _id );

   GPIOx->OTYPER = tmp32;

   return *this;
}


STM32_GPIO::Pin& STM32_GPIO::Pin::PuPd( PullUpDn pud )
{
   GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)_gpio.iobase;
   register uint32_t tmp32;

   tmp32  = GPIOx->PUPDR;
   tmp32 &= ~( 0x03 << ( _id * 2 ));
   tmp32 |=  ( pud  << ( _id * 2 ));

   GPIOx->PUPDR = tmp32;

   return *this;
}


STM32_GPIO::Pin& STM32_GPIO::Pin::setHandler( GPIO::Handler *handler, GPIO::Pin::Edge edge )
{
   (void)handler;
   (void)edge;

   return *this;
}

/*EoF*/

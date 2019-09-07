
#include "STM32/STM32_RCC.h"
#include "STM32/STM32_ADC.h"
#include "system/Logger.h"

#include <stm32f4xx_adc.h>

#undef ADC
#undef RCC

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_ADC::STM32_ADC( Bus& bus,
                      const uint32_t iobase,
                      const uint32_t periph,
                      const char    *name )
   : STM32_Device( name, bus, iobase, periph )
{
   _numConv = 5;
}


STM32_ADC::~STM32_ADC()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

STM32_ADC& STM32_ADC::init( void )
{
   kprintf( "%s: STM32F4xx ADC at %s\r\n", _name, bus.name );

   return *this;
}


STM32_ADC& STM32_ADC::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   RCC.enable( this, debug );

   ADC_TypeDef *ADCx = (ADC_TypeDef*)iobase;

   ADCx->CR1 = 0; //Set resolution for the ADC
   uint32_t tmpreg = 0;
   tmpreg = ADCx->CR2;
   tmpreg &= ((uint32_t)0xC0FFF7FD);  //CR2_CLEAR_MASK
   tmpreg |= 0 << 1;
   ADCx->CR2 = tmpreg; //Sets the continuous conversion mode

   tmpreg = ADCx->SQR1;

   tmpreg &= ((uint32_t)0xFF0FFFFF); //SQR1_L_RESET

   //This sets the number of conversions to perform
   //Corresponds to the number of sun sensors on board
   //
   tmpreg |= (uint32_t)(_numConv-1) << 20;

   ADCx->SQR1 = tmpreg;
   ADCx->CR2 |= ADC_CR2_ADON;

   if( debug ) {
      kprintf( "%s: enabled\r\n", _name );
   }

   return *this;
}


STM32_ADC& STM32_ADC::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   ADC_TypeDef *ADCx = (ADC_TypeDef*)iobase;
   ADCx->CR2 &= ~ADC_CR2_ADON;

   RCC.disable( this, debug );

   if( debug ) {
      kprintf( "%s: disabled\r\n", _name );
   }

   return *this;
}


STM32_ADC& STM32_ADC::read( ADC::Channel& ch, SensorSample<uint16_t> *sample )
{
   STM32_ADC::Channel& stmCh = static_cast<STM32_ADC::Channel&>( ch );

   ADC_TypeDef *ADCx = (ADC_TypeDef*)iobase;
   ADC_RegularChannelConfig(ADCx, stmCh._id, 1, ADC_SampleTime_15Cycles);
   ADC_SoftwareStartConv(ADCx);
   //(void)ADC_ClearFlag(ADCx, ADC_FLAG_EOC);

   while( ADC_GetFlagStatus( ADCx, ADC_FLAG_EOC ) == RESET ) /* XXX meh... use interrupts */
      ;

   sample->stdev = 0;
   sample->value = ADC_GetConversionValue( ADCx );

   return *this;
}

/*EoF*/

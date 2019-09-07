
#include "STM32/STM32_RCC.h"
#include "system/Logger.h"

#include <safe_stm32f4xx.h>

#define RCC_LSE_HARD_LIMIT 100000

using namespace qb50;


uint8_t STM32_RCC::pv[ 16 ] = { 0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9 };

//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_RCC::STM32_RCC( const uint32_t iobase, const char *name )
   : _iobase( iobase ), _name( name ), _csr( 0 )
{ ; }


STM32_RCC::~STM32_RCC()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

STM32_RCC& STM32_RCC::init( void )
{
   RCC_TypeDef *RCCx = (RCC_TypeDef*)_iobase;

   Clocks clk;

   kprintf( "%s: STM32F4xx Reset and Clock controller\r\n", _name );

   clocks( &clk );

   kprintf( "%s: SYSCLK: %luMHz, HCLK: %luMHz, PCLK1: %luMHz, PCLK2: %luMHz\r\n",
            _name,
            clk.SYSCLKFreq / ( 1000 * 1000 ),
            clk.HCLKFreq   / ( 1000 * 1000 ),
            clk.PCLK1Freq  / ( 1000 * 1000 ),
            clk.PCLK2Freq  / ( 1000 * 1000 )
   );

   _csr = RCCx->CSR;
   RCCx->CSR |= 0x01000000; // clear reset flag

   kprintf( "%s: Reset flags: 0x%02x\r\n", _name, (( _csr & 0xfe000000 ) >> 25 ));

   return *this;
}


STM32_RCC& STM32_RCC::enable( STM32_Device *dev, bool debug )
{
   RCC_TypeDef *RCCx = (RCC_TypeDef*)_iobase;

   (void)debug;

   switch( dev->bus.id ) {

      case Bus::AHB1: RCCx->AHB1ENR |= dev->periph; break;
      case Bus::AHB2: RCCx->AHB2ENR |= dev->periph; break;
      case Bus::AHB3: RCCx->AHB3ENR |= dev->periph; break;
      case Bus::APB1: RCCx->APB1ENR |= dev->periph; break;
      case Bus::APB2: RCCx->APB2ENR |= dev->periph; break;

   };

   if( debug ) {
      kprintf( "%s: %s enabled at %s\r\n", _name, dev->name(), dev->bus.name );
   }

   return *this;
}


STM32_RCC& STM32_RCC::disable( STM32_Device *dev, bool debug )
{
   RCC_TypeDef *RCCx = (RCC_TypeDef*)_iobase;

   (void)debug;

   switch( dev->bus.id ) {

      case Bus::AHB1: RCCx->AHB1ENR &= ~dev->periph; break;
      case Bus::AHB2: RCCx->AHB2ENR &= ~dev->periph; break;
      case Bus::AHB3: RCCx->AHB3ENR &= ~dev->periph; break;
      case Bus::APB1: RCCx->APB1ENR &= ~dev->periph; break;
      case Bus::APB2: RCCx->APB2ENR &= ~dev->periph; break;

   };

   if( debug ) {
      kprintf( "%s: %s disabled at %s\r\n", _name, dev->name(), dev->bus.name );
   }

   return *this;
}


STM32_RCC& STM32_RCC::enableLSE( bool debug )
{
   RCC_TypeDef *RCCx = (RCC_TypeDef*)_iobase;

   if(( RCCx->BDCR & RCC_BDCR_LSERDY ) == 0 ) {

      if( debug ) {
         kprintf( "%s: enabling LSE...\r\n", _name );
      }

      RCCx->BDCR |= RCC_BDCR_LSEON;

      for( int n = 0 ; n < RCC_LSE_HARD_LIMIT ; ++n ) {
         if(( RCCx->BDCR & RCC_BDCR_LSERDY ) != 0 )
            break;
      }
   }

   if(( RCCx->BDCR & RCC_BDCR_LSERDY ) == 0 ) {
      kprintf( RED( "%s: timeout while waiting for RCC_BDCR_LSERDY" ) "\r\n", _name );
   } else {
      if( debug ) {
         kprintf( "%s: LSE enabled\r\n", _name );
      }
   }

   return *this;
}


STM32_RCC& STM32_RCC::enableRTC( uint32_t clkSrc, bool debug )
{
   RCC_TypeDef *RCCx = (RCC_TypeDef*)_iobase;

   RCCx->BDCR |= ( clkSrc & 0x00000300 );
   RCCx->BDCR |=  (uint32_t)0x00008000;

   if( debug ) {
      kprintf( "%s: RTC enabled\r\n", _name );
   }

   return *this;
}


STM32_RCC& STM32_RCC::disableRTC( bool debug )
{
   RCC_TypeDef *RCCx = (RCC_TypeDef*)_iobase;

   RCCx->BDCR &= ~(uint32_t)0x00008000;

   if( debug ) {
      kprintf( "%s: RTC disabled\r\n", _name );
   }

   return *this;
}


STM32_RCC& STM32_RCC::clocks( Clocks *clk )
{
   RCC_TypeDef *RCCx = (RCC_TypeDef*)_iobase;

   uint32_t SWS = ( RCCx->CFGR & RCC_CFGR_SWS ) >> 2;

   switch( SWS ) {

      case ClockSource::HSI:

         clk->SYSCLKFreq = HSI_VALUE;
         break;

      case ClockSource::HSE:

         clk->SYSCLKFreq = HSE_VALUE;
         break;

      case ClockSource::PLL:
         {
            /* see STM32 ref. manual sec. 6.3.2 "RCC PLL configuration register" */

            uint32_t pllsrc = ( RCCx->PLLCFGR & RCC_PLLCFGR_PLLSRC ) >> 22;
            uint32_t pllp   = ( RCCx->PLLCFGR & RCC_PLLCFGR_PLLP   ) >> 16;
            uint32_t plln   = ( RCCx->PLLCFGR & RCC_PLLCFGR_PLLN   ) >>  6;
            uint32_t pllm   = ( RCCx->PLLCFGR & RCC_PLLCFGR_PLLM   );

            uint32_t pllvco = ( pllsrc == 0 ) ? HSI_VALUE : HSE_VALUE;
                     pllvco = ( pllvco / pllm ) * plln;

            clk->SYSCLKFreq = pllvco / (( pllp + 1 ) << 1 );
         }
         break;

      default:

         clk->SYSCLKFreq = HSI_VALUE;
         break;

   }

   uint32_t HPRE  = ( RCCx->CFGR & RCC_CFGR_HPRE  ) >>  4;
   uint32_t PPRE1 = ( RCCx->CFGR & RCC_CFGR_PPRE1 ) >> 10;
   uint32_t PPRE2 = ( RCCx->CFGR & RCC_CFGR_PPRE1 ) >> 13;

   clk->HCLKFreq  = clk->SYSCLKFreq >> pv[ HPRE  ];
   clk->PCLK1Freq = clk->HCLKFreq   >> pv[ PPRE1 ];
   clk->PCLK2Freq = clk->HCLKFreq   >> pv[ PPRE2 ];

   return *this;
};


uint32_t STM32_RCC::freq( Bus &bus )
{
   Clocks clk;
   clocks( &clk );

   uint32_t rv = 0;

   switch( bus.id ) {

      case Bus::AHB1: rv = clk.HCLKFreq;  break;
      case Bus::AHB2: rv = clk.HCLKFreq;  break;
      case Bus::AHB3: rv = clk.HCLKFreq;  break;
      case Bus::APB1: rv = clk.PCLK1Freq; break;
      case Bus::APB2: rv = clk.PCLK2Freq; break;

   }

   return rv;
}


bool STM32_RCC::isPwrOn( void )
{
   return
      (( _csr & ( RCC_CSR_PORRSTF | RCC_CSR_BORRSTF )) != 0 );
}

/*EoF*/

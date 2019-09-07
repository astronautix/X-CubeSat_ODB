
#include "STM32/STM32_PWR.h"
#include "STM32/STM32_RCC.h"
#include "STM32/STM32_RTC.h"
#include "STM32/STM32_BKP.h"
#include "system/Logger.h"

#include <safe_stm32f4xx.h>

#define RTC_HARD_LIMIT 100000

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

STM32_RTC::STM32_RTC( Bus& bus,
                      const uint32_t iobase,
                      const uint32_t periph,
                      const char    *name )
   : STM32_Device( name, bus, iobase, periph )
{ ; }


STM32_RTC::~STM32_RTC()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

STM32_RTC& STM32_RTC::init( void )
{
   kprintf( "%s: STM32F4xx Real-Time Clock controller at %s\r\n", _name, bus.name );

   return *this;
}


STM32_RTC& STM32_RTC::enable( bool debug )
{
   Time tm;
   time_t ts;

   if( _incRef() > 0 )
      return *this;

   lock();

   PWR.enable( debug );         // enable the power controller
   PWR.enableBRE();              // enable access to backup domain
   BKP.enable( debug );

   RCC.enableLSE( debug );

   // select the RTC source (LSE) and enable RTC
   RCC.enableRTC( 0x00000100, debug );

   RTC_TypeDef *RTCx = (RTC_TypeDef*)iobase;

   if(( RTCx->ISR & RTC_ISR_INITS ) == 0 ) {
      kprintf( "%s: calendar not initialized\r\n", _name );
   } else {
      kprintf( "%s: calendar already initialized\r\n", _name );
   }

   if( _enterInit() ) {
      /* set prescaler to 32678 */
      RTCx->PRER |= 0x000000ff; /*  synchronous prescaler */
      RTCx->PRER |= 0x007f0000; /* asynchronous prescaler */
   }

   /* exit init mode */

   _exitInit();

   (void)getTime( tm );
   ts = RTC::conv( tm );

   kprintf( "%s: date is %04d-%02d-%02d (YYYY-MM-DD)\r\n", _name, tm.year, tm.mon, tm.day );
   kprintf( "%s: time is %02d:%02d:%02d (%lu)\r\n", _name, tm.hour, tm.min, tm.sec, ts );

   unlock();

   return *this;
}


STM32_RTC& STM32_RTC::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   RTC_TypeDef *RTCx = (RTC_TypeDef*)iobase;

   lock();

   RTCx->WPR = 0x00;

   RCC.disableRTC( debug );

   unlock();

   return *this;
}


STM32_RTC& STM32_RTC::getTime( Time &t )
{
   int n;

   RTC_TypeDef *RTCx = (RTC_TypeDef*)iobase;

   for( n = 0 ; n < RTC_HARD_LIMIT ; ++n ) {
      if(( RTCx->ISR & RTC_ISR_RSF ) != 0 )
         break;
   }

   if(( RTCx->ISR & RTC_ISR_RSF ) == 0 ) {
      kprintf( RED( "%s: timeout in STM32_RTC::getTime()" ) "\r\n", _name );

      t.year = t.mon = t.day = 0;
      t.hour = t.min = t.sec = 0;

      return *this;
   }

   /*
    * "To ensure consistency between the 3 values, reading either RTC_SSR or
    *  RTC_TR locks the values in the higher-order calendar shadow registers
    *  until RTC_DR is read." -- see Sec. 26.3.6 - "Reading the Calendar"
    */

   uint32_t TR = RTCx->TR;
   uint32_t DR = RTCx->DR;

   t.year = 10 * (( DR >> 20 ) & 0x0f )
               + (( DR >> 16 ) & 0x0f )
               + 2000;

   t.mon  = 10 * (( DR >> 12 ) & 0x01 )
               + (( DR >>  8 ) & 0x0f );

   t.day  = 10 * (( DR >>  4 ) & 0x03 )
               + (  DR         & 0x0f );

   t.hour = 10 * (( TR >> 20 ) & 0x03 )
               + (( TR >> 16 ) & 0x0f );

   t.min  = 10 * (( TR >> 12 ) & 0x07 )
               + (( TR >>  8 ) & 0x0f );

   t.sec  = 10 * (( TR >>  4 ) & 0x07 )
               + (  TR         & 0x0f );

   return *this;
}


STM32_RTC& STM32_RTC::setTime( Time &t )
{
   RTC_TypeDef *RTCx = (RTC_TypeDef*)iobase;

   /* set calendar */

   uint32_t year = t.year - 2000;

   uint32_t DR = (   year / 10 ) << 20
               | (   year % 10 ) << 16
               | ( t.mon  / 10 ) << 12
               | ( t.mon  % 10 ) <<  8
               | ( t.day  / 10 ) <<  4
               | ( t.day  % 10 )
               ;

   uint32_t TR = ( t.hour / 10 ) << 20
               | ( t.hour % 10 ) << 16
               | ( t.min  / 10 ) << 12
               | ( t.min  % 10 ) <<  8
               | ( t.sec  / 10 ) <<  4
               | ( t.sec  % 10 )
               ;

   if( !_enterInit() )
      return *this;

   RTCx->TR = TR;
   RTCx->DR = DR;

   _exitInit();

   Time t1;
   time_t ts;
   (void)getTime( t1 );
   ts = RTC::conv( t1 );

   kprintf( "%s: date is %04d-%02d-%02d (YYYY-MM-DD)\r\n", _name, t1.year, t1.mon, t1.day );
   kprintf( "%s: time is %02d:%02d:%02d (%lu)\r\n", _name, t1.hour, t1.min, t1.sec, ts );

   return *this;
}


//  - - - - - - - - - - - - - - -  //
//  P R I V A T E   M E T H O D S  //
//  - - - - - - - - - - - - - - -  //

int STM32_RTC::_enterInit( void )
{
   RTC_TypeDef *RTCx = (RTC_TypeDef*)iobase;

   /* see the STM32F4 Ref. Manual sec. 26.3.5 */

   RTCx->WPR = 0xca;
   RTCx->WPR = 0x53;

   /* init sequence */

   RTCx->ISR |= (uint32_t)RTC_ISR_INIT;

   for( int n = 0 ; n < RTC_HARD_LIMIT ; ++n ) {
      if(( RTCx->ISR & RTC_ISR_INITF ) != 0 )
         break;
   }

   if(( RTCx->ISR & RTC_ISR_INITF ) == 0 ) {
      kprintf( RED( "%s: timeout while waiting for RTC_ISR_INITF" ) "\r\n", _name );
      return 0;
   }

   return 1;
}


void STM32_RTC::_exitInit( void )
{
   RTC_TypeDef *RTCx = (RTC_TypeDef*)iobase;

   /* exit init mode */

   RTCx->ISR &= (uint32_t)~RTC_ISR_INIT;

   /* lock RTC registers */

   RTCx->WPR = 0xff;
}


/*EoF*/

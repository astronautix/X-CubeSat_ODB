
#include "CTCSS.h"

using namespace qb50;


CTCSS qb50::CTCS0( "CTCS0", PB12, PA8 );


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

CTCSS::CTCSS( const char *name, STM32_GPIO::Pin& enPin, STM32_GPIO::Pin& dtPin )
   : Device( name ), _enPin( enPin ), _dtPin( dtPin )
{
   _semDecTone = xSemaphoreCreateBinary();
}


CTCSS::~CTCSS()
{
   vSemaphoreDelete( _semDecTone );
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

CTCSS& CTCSS::init()
{
   _dtPin . in()  . noPull();
   _enPin . out() . off();

   EXTI.registerHandler( _dtPin, this, STM32_EXTI::BOTH );

   kprintf( "%s: CTCSS handler, enPin: %s, dtPin: %s\r\n",
            _name, _enPin.name(), _dtPin.name() );

   return *this;
}


CTCSS& CTCSS::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   _enPin.on();

   if( debug ) {
      kprintf( "%s: enabled\r\n", _name );
   }

   return *this;
}


CTCSS& CTCSS::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   _enPin.off();

   if( debug ) {
      kprintf( "%s: disabled\r\n", _name );
   }

   return *this;
}


bool CTCSS::wait( unsigned ms )
{
   TickType_t dt;

   if( ms == 0 ) {
      dt = portMAX_DELAY;
   } else {
      dt = (TickType_t)ms / portTICK_RATE_MS;
   }

 //bool tone = decTone();
 //int  rv   = xSemaphoreTake( _semDecTone, dt );

   return
      ( xSemaphoreTake( _semDecTone, dt ) == pdTRUE );
}


bool CTCSS::decTone( void )
{
   return
      _dtPin.read() == false;
}


//  - - - - - - - - - - - -  //
//  E X T I   H A N D L E R  //
//  - - - - - - - - - - - -  //

void CTCSS::handle( STM32_EXTI::EXTIn /* ignored */ )
{
   portBASE_TYPE hpTask = pdFALSE;

   (void)xSemaphoreGiveFromISR( _semDecTone, &hpTask );

   if( hpTask == pdTRUE )
      portEND_SWITCHING_ISR( hpTask );
}


/*EoF*/

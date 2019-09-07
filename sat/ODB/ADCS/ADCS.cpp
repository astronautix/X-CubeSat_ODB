
#include "Config.h"
#include "ADCS.h"

#include <cstring>


using namespace qb50;


ADCS qb50::ADCS0( "ADCS0", UART1, PC13 );


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

ADCS::ADCS( const char *name, STM32_UART& uart, STM32_GPIO::Pin& enPin )
   : Device( name ), _uart( uart ), _enPin( enPin ), _state( ADCS::MEASURE )
{
   _mp = new ADCSMeas;
}


ADCS::~ADCS()
{
   delete _mp;
}


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

ADCS& ADCS::init()
{
   _enPin.out().off();

   kprintf( "%s: Attitude Determination & Control System on %s, enPin: %s\r\n", _name, _uart.name(), _enPin.name() );

   return *this;
}


ADCS& ADCS::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   _uart.enable( debug );
   _enPin.on();

   if( debug ) {
      kprintf( "%s: enabled\r\n", _name );
   }

   return *this;
}


ADCS& ADCS::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   _enPin.off();
   _uart.disable( debug );

   if( debug ) {
      kprintf( "%s: disabled\r\n", _name );
   }

   return *this;
}


ADCS& ADCS::setMeas( const ADCSMeas *mp )
{
   lock();
   (void)memcpy( _mp, mp, sizeof( ADCSMeas ));
   unlock();

   return *this;
}


ADCS& ADCS::getMeas( ADCSMeas *mp )
{
   lock();
   (void)memcpy( mp, _mp, sizeof( ADCSMeas ));
   unlock();

   return *this;
}


ADCS::State ADCS::state( void ) const
{
   return _state;
}


ADCS::State ADCS::state( ADCS::State st, bool debug )
{
   ADCS::State old = _state;

   if( st != old ) {
      _state = st;
      if( debug ) {
         kprintf( YELLOW( "%s: switching modes (%ld -> %ld)" ) "\r\n", _name, old, st );
      }
   }

   return old;
}


unsigned ADCS::read( uint8_t *x, unsigned len, int toms )
{
   return
      _uart.read( x, len, toms );
}


unsigned ADCS::write( const uint8_t *x, unsigned len, int toms )
{
   return
      _uart.write( x, len, toms );
}

/*EoF*/

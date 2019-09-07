
#include "Config.h"
#include "Fipex.h"

#include <safe_stm32f4xx.h>
#include <cstring>


using namespace qb50;


Fipex::ScriptFormatException  qb50::FipexScriptFormatException;
Fipex::ResponseException      qb50::FipexResponseException;
Fipex::TimeoutException       qb50::FipexTimeoutException;

Fipex qb50::SU( "SU", UART2, PB14 );


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

Fipex::Fipex( const char *name, STM32_UART& uart, STM32_GPIO::Pin& enPin )
   : Device( name ), _uart( uart ), _enPin( enPin )
{
   _st = (uint8_t*)BKPSRAM_BASE + 2048; // XXX avoid magic numbers
   _sc = _st;
   _sn = 0;
}


Fipex::~Fipex()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

Fipex& Fipex::init()
{
   Config::pval_t sn;

   _enPin.out().on();

   kprintf( "%s: FIPEX Probe on %s, enPin: %s\r\n", _name, _uart.name(), _enPin.name() );

   sn = CONF.getParam( Config::PARAM_FIPEX_SCRIPT_N );
   (void)activeScript( sn );

   return *this;
}


Fipex& Fipex::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   _enPin.off();

   // Req: FPX-SW-0240 - "The OBC shall wait 500ms before initializing the UART interface [...]"

   delay( 500 );
   _uart.enable( debug );

   if( debug ) {
      kprintf( "%s: enabled\r\n", _name );
   }

   return *this;
}


Fipex& Fipex::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   _uart.disable( debug );
   _enPin.on();

   if( debug ) {
      kprintf( "%s: disabled\r\n", _name );
   }

   return *this;
}


Fipex& Fipex::activeScript( unsigned sn )
{
   kprintf( "%s: script #%lu active\r\n", _name, sn );

   _sn = sn;
   _sc = _st + _sn * 256;

   return *this;
}


Fipex& Fipex::storeScript( unsigned sn, Script::Header *sh )
{
   Fipex::Script sc;

   if( sn > 7 ) {

      kprintf( RED( "%s: invalid script number #%lu" ) "\r\n", _name, sn );

   } else {

      hexdump( sh, sh->len + 1 );

      try {
         sc.load( sh );
         uint8_t *base = _st + sn * 256;
         (void)memcpy( base, sh, sh->len + 1 );
         kprintf( GREEN( "%s: storing script #%lu" ) "\r\n", _name, sn );
      } catch( Fipex::Exception& e ) {
         kprintf( RED( "%s: %s" ) "\r\n", _name, e.what() );
      }

   }

   return *this;
}


Fipex::Script::Header *Fipex::loadScript( unsigned sn )
{
   uint8_t *base = _st + sn * 256;

   return (Script::Header*)base;
}


Fipex& Fipex::abort( void )
{
   return *this;
}


size_t Fipex::send( CmdHeader *ch, RspHeader *rh, int toms )
{
   size_t n = _uart.write( (const uint8_t*)ch, ch->len + 4U, toms );

hexdump( (const uint8_t*)ch, ch->len + 4U );

   if( n != ( ch->len + 4U )) {
      kprintf( RED( "%s: timeout in _send()" ) "\r\n", _name );
      return 0;
   }

   return
      recv( rh, toms );
}


size_t Fipex::recv( RspHeader *rh, int toms )
{
   (void)memset( rh, 0, 205 );

   size_t n = _uart.read( (uint8_t*)rh, 205, toms );
kprintf( YELLOW( "%s: got %ld bytes" ) "\r\n", _name, n );

   if( n == 0 ) {
      kprintf( "%s: timeout in _recv()\r\n", _name );
      return 0;
   }

   if( n != 205 ) {
      kprintf( RED( "%s: got %ld bytes (expected 205)" ) "\r\n", _name, n );
      return 0;
   }

   if( rh->sb != 0x7e ) {
      kprintf( RED( "%s: start byte not found in SU response" ) "\r\n", _name );
      return 0;
   }

   uint8_t sum = rh->id ^ rh->len ^ rh->seq;
   for( size_t i = 0 ; i < rh->len ; ++i )
      sum ^= rh->x[i];

   const uint8_t *x = (const uint8_t *)rh;
   if( sum != x[ rh->len + 4U ] ) {
      kprintf( RED( "%s: checksum mismatch in SU response" ) "\r\n", _name );
      return 0;
   }

   return n;
}


/*EoF*/

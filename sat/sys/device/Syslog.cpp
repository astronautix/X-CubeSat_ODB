
#include "device/Syslog.h"
#include "system/Application.h"

using namespace qb50;


static void _trampoline( void *x );


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

Syslog::Syslog( const char *name, UART& uart )
   : Device( name ), _uart( uart )
{
   _line = new uint8_t[ 128 ];
   _sem  = xSemaphoreCreateBinary();
}


Syslog::~Syslog()
{
   vSemaphoreDelete( _sem );
   delete[] _line;
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

Syslog& Syslog::init( void )
{
   (void)xTaskCreate( _trampoline, _name, /*128*/ 256, this, 1, &_ioTask );
   return *this;
}


Syslog& Syslog::printf( const char *fmt, ... )
{
   va_list ap;
   va_start( ap, fmt );

   lock();

   do {
      unsigned ts = ticks();

      int len = snprintf( (char*)_line, 16, "[% 9.3f] ", (float)ts / configTICK_RATE_HZ );
      if( len <= 0 ) break;

      int n = vsnprintf( (char*)_line + len, 128 - len - 1, fmt, ap );
      if( n <= 0 ) break;

      len += n;

      if( _ring.avail() >= (unsigned)len )
         _ring.write( _line, len );

   } while( 0 );

   unlock();

   (void)xSemaphoreGive( _sem );

   va_end( ap );

   return *this;
}


Syslog& Syslog::enable( bool debug )
{
   (void)debug;
   return *this;
}


Syslog& Syslog::disable( bool debug )
{
   (void)debug;
   return *this;
}


//  - - - - - - - - - - - - - - -  //
//  P R I V A T E   M E T H O D S  //
//  - - - - - - - - - - - - - - -  //

static void _trampoline( void *x )
{
   Syslog *self = (Syslog*)x;
   self->run();
   vTaskDelete( NULL );
}


void Syslog::run( void )
{
   uint8_t *buf = new uint8_t[ 64 ];
   unsigned n;

   for( ;; ) {
      (void)xSemaphoreTake( _sem, 500 / portTICK_RATE_MS );

      n = _ring.read( buf, 64 );
      while( n > 0 ) {
         (void)_uart.write( buf, n );
         n = _ring.read( buf, 64 );
      }
   }

   delete[] buf;
}

/*EoF*/

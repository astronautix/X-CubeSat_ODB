
#include "CTCSSThread.h"
#include "Config.h"
#include "devices.h"
#include "Baseband.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

CTCSSThread::CTCSSThread()
   : Thread( "CTCSS", 1, SUSPENDED )
{
   _sec   = 0;
   _state = OFF;
}


CTCSSThread::~CTCSSThread()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

void CTCSSThread::onSuspend( void )
{
   CTCS0.disable();

   _sec   = 0;
   _state = OFF;

   Thread::onSuspend();
}


void CTCSSThread::onResume( void )
{
/*
   int i, vp;
*/

   Thread::onResume();

   _sec   = 0;
   _state = OFF;

   CTCS0.enable();

/*
   vp = xSemaphoreTake( _sem, 10 / portTICK_RATE_MS );
   for( i = 0 ; i < 10 ; ++i ) {
      if( vp == pdFAIL ) break;
      kprintf( YELLOW( "FLUSHING CTCSS" ) "\r\n" );
      vp = xSemaphoreTake( _sem, 10 / portTICK_RATE_MS );
   }

   if( i > 2 ) {
      kprintf( RED( "%s: too many pending CTCSS events" ) "\r\n", name );
   }
*/
}


void CTCSSThread::run( void )
{
#if 0
   int cnt = 0;
   int state = 0;

   for( ;; ) {
      _wait();

      switch( state ) {

         case 0: // attente DEC_Tone
            vp = xSemaphoreTake( _sem, 1000 / portTICK_RATE_MS );
            if( vp == pdPASS ) {
               if( !PA8.read() ) {
                  kprintf( RED( "CTCSS ON" ) "\r\n" );
                  PB12.off(); // disable DEC_Tone
                  BB.enable();
                  state = 1;
                  cnt   = 0;
               }
            } else {
               kprintf( "waiting for CTCSS...\r\n" );
            }
            break;

         case 1:

            if( cnt < 20 ) {
               delay( 1000 );
            } else {
               kprintf( RED( "CTCSS OFF" ) "\r\n" );
               BB.disable();
               PB12.on(); // enable DEC_Tone
               state = 0;
            }

            ++cnt;
            break;

         default:
            break;

      }
   }

#else

   for( ;; ) {
      _wait();

      if( CTCS0.wait( 1000 )) {

         if( CTCS0.decTone() ) {

            kprintf( GREEN( "CTCSS ON" ) "\r\n" );
            BB.enable();
            Baseband::Power p = (Baseband::Power)CONF.getParam( Config::PARAM_WODEX_POWER );
            BB.power( p );
            _sec   = 20;
            _state = ON;

         } else {

            if( _state != OFF ) {
               kprintf( GREEN( "CTCSS OFF" ) "\r\n" );
               BB.disable();
               _sec   = 0;
               _state = OFF;
            }

         }

      } else {

         if( _state == ON ) {
            if( _sec > 0 ) {
               --_sec;
            } else {
               kprintf( YELLOW( "CTCSS OFF (TOT)" ) "\r\n" );
               BB.disable();
               _state = OFF;
            }
         }

         kprintf( "waiting for CTCSS...\r\n" );

      }
   }

#endif
}

/*EoF*/

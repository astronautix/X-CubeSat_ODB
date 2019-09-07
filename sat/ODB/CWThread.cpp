
#include "devices.h"
#include "CWThread.h"
#include "Morse.h"
#include "Config.h"
#include "system/Application.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

CWThread::CWThread()
   : Thread( "CW Transmitter", 1, SUSPENDED )
{ ; }


CWThread::~CWThread()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

static const char *alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


static char mvCode( unsigned mv )
{
   if( mv < 6000 ) mv = 5900;
   if( mv > 8400 ) mv = 8400;

   return
      alpha[ ( mv - 5900 ) / 100 ];
}


static char maCode( unsigned ma )
{
   if( ma > 2000 ) ma = 2000;

   return
      alpha[ ma / 100 ];
}


static char dcCode( int dc )
{
   if( dc < -20 ) dc = -25;
   if( dc >  60 ) dc =  60;

   return
      alpha[ ( dc + 25 ) / 5 ];
}


void CWThread::onSuspend( void )
{
   BB.disable();
   Thread::onSuspend();
}


void CWThread::onResume( void )
{
   Thread::onResume();
   BB.enable();
}


void CWThread::run( void )
{
   char x[16];
   unsigned dt;

   Baseband::Power p = (Baseband::Power)CONF.getParam( Config::PARAM_CW_POWER );
   Morse cw( p );

   for( unsigned i = 0 ;; ++i ) {

      dt = CONF.getParam( Config::PARAM_CW_CYCLE_TX );
      dt = ( 10 + 5 * dt ) * 1000;

    //pw = CONF.getParam( Config::PARAM_CW_POWER );

      delay( dt );

      _wait();  // wait if suspended

      x[  0 ] = 'O';
      x[  1 ] = 'N';
      x[  2 ] = '0';
      x[  3 ] = 'F';
      x[  4 ] = 'R';
      x[  5 ] = SAT.id() == ODB::FR01 ? '1' : '5';
      x[  6 ] = ' ';
      x[  7 ] = mvCode( SAT.mvBat );
      x[  8 ] = maCode( SAT.maIRx + SAT.maITx );
      x[  9 ] = maCode( SAT.maI[0] + SAT.maI[1] + SAT.maI[2] + SAT.maI[3] );
      x[ 10 ] = dcCode( SAT.dcBat );
      x[ 11 ] = '\0';

#if 1
      kprintf( "  vBat: %4d mv (%c)\r\n", SAT.mvBat,                                         x[  7 ] );
      kprintf( " I_out: %4d mA (%c)\r\n", SAT.maIRx + SAT.maITx,                             x[  8 ] );
      kprintf( "  I_in: %4d mA (%c)\r\n", SAT.maI[0] + SAT.maI[1] + SAT.maI[2] + SAT.maI[3], x[  9 ] );
      kprintf( "  tBat: %4d dC (%c)\r\n", SAT.dcBat,                                         x[ 10 ] );
#endif

      PB15.on(); // TX
      delay( 20 * 1000 );
      PB13.on(); // PA

      if(( i % 5 ) == 0 ) {
         kprintf( "CW - %s\r\n", x );
         cw.write( x, 11 );
      } else {
         kprintf( "CW - %s\r\n", x + 3 );
         cw.write( x + 3, 8 );
      }

      PB13.off(); // PA
      PB15.off(); // TX
   }
}

/*EoF*/

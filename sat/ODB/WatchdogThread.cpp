
#include "WatchdogThread.h"
#include "devices.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

WatchdogThread::WatchdogThread()
   : Thread( "Watchdog Thread", 2, RUNNING )
{ ; }


WatchdogThread::~WatchdogThread()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

void WatchdogThread::run( void )
{
   for( ;; ) {
      WDG.refresh();
      delay( 1000 );
   }
}

/*EoF*/


#include "devices.h"
#include "InitThread.h"
#include "Event.h"
#include "Config.h"
#include "system/Application.h"

using namespace qb50;


extern QueueHandle_t evQueue;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

InitThread::InitThread()
   : Thread( "Initialization", 1, SUSPENDED )
{ ; }


InitThread::~InitThread()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

void InitThread::run( void )
{
   Event *ev;

   for( ;; ) {
      _wait();

      kprintf( "Waiting for 30mn...\r\n" );
      delay( 30 * 60 * 1000 );
      kprintf( "Done waiting\r\n" );

      if( SAT.aDeploy() == ODB::DEPLOYED ) {
         ev = new Event( Event::AD_SUCCESS );
      } else {
         ev = new Event( Event::AD_FAILURE );
      }

      xQueueSendToBack( evQueue, &ev, portMAX_DELAY );

      delay( 1000 );
   }
}

/*EoF*/

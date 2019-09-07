
#include "devices.h"
#include "TestThread.h"
#include "Config.h"
#include "Event.h"
#include "system/Application.h"

using namespace qb50;


extern QueueHandle_t evQueue;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

TestThread::TestThread()
   : Thread( "Test Thread", 1, SUSPENDED, 256 ),
     _mode( ON )
{ ; }


TestThread::~TestThread()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

void TestThread::onSuspend( void )
{
 //MAG0.disable();
   Thread::onSuspend();
}


void TestThread::onResume( void )
{
   Thread::onResume();
 //MAG0.enable();
}


void TestThread::run( void )
{
   for( ;; ) {

      _wait();

      delay( 1000 );

   }
}

/*EoF*/

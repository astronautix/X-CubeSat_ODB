
#include "CommandThread.h"
#include "common/Message.h"
#include "config.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

CommandThread::CommandThread()
   : Thread( "Command Thread", 2, RUNNING, 384 )
{
   _x = new uint8_t[ 256 ];
}


CommandThread::~CommandThread()
{
   delete[] _x;
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

void CommandThread::onSuspend( void )
{
   Thread::onSuspend();
}


void CommandThread::onResume( void )
{
   Thread::onResume();
}


void CommandThread::run( void )
{
   size_t n;
   Timer tm;

   const ADCSCtrl *m = (const ADCSCtrl*)_x;

   for( ;; ) {

      n = UART1.read( _x, sizeof( ADCSCtrl ), 5000 );

      if( n == 0 ) {
         kprintf( YELLOW( "waiting for ODB..." ) "\r\n" );
         continue;
      }

      kprintf( GREEN( "D: %ld, X: %ld, Y: %ld, Z: %ld" ) "\r\n", m->d, m->x, m->y, m->z );

      AST0.n = m->n;
      AST0.d = m->d;
      AST0.x = m->x;
      AST0.y = m->y;
      AST0.z = m->z;

   }
}


/*EoF*/

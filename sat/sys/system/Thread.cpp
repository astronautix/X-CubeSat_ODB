
#include <FreeRTOS.h>
#include <task.h>
#include <stddef.h>

#include "system/Thread.h"
#include "system/Logger.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

Thread::Thread( const char *name, int prio, ThreadState state, unsigned stackDepth )
   : _thst( state )
{
   this->name       = name == NULL ? "(generic thread)" : name;
   this->priority   = prio;
   this->stackDepth = stackDepth;

   onInit();
}


Thread::~Thread()
{
   onExit();
}


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

void Thread::suspend( void )
{
   switch( _thst ) {

      case RUNNING:
      case RESUMING:
         _thst = SUSPENDING;
         break;

      case SUSPENDED:
         /* already suspended */
         break;

      case SUSPENDING:
         /* already about to be suspended */
         break;

   }
}


void Thread::resume( void )
{
   switch( _thst ) {

      case SUSPENDED:
         _thst = RESUMING;
         (void)xTaskNotifyGive( (TaskHandle_t)handle );
         break;

      case SUSPENDING:
         _thst = RESUMING;
         break;

      case RUNNING:
         /* already running */
         break;

      case RESUMING:
         /* already about to be resumed */
         break;

   }

   if( _thst == SUSPENDED ) {
      _thst = RUNNING;
      (void)xTaskNotifyGive( (TaskHandle_t)handle );
   }
}

/* handlers */

void Thread::onInit( void )
{ ; }


void Thread::onExit( void )
{ ; }


void Thread::onSuspend( void )
{
   kprintf( "[%s] suspended\r\n", name );
/*
   unsigned su = _stackHWM();
   kprintf( "[%s] suspended (stack usage: %u/%u words)\r\n", name, su, stackDepth );
*/
}


void Thread::onResume( void )
{
   kprintf( "[%s] resuming\r\n", name );
}


#if 0
void *Thread::operator new( size_t size )
{
   void *p = pvPortMalloc( size );

   kprintf( "\033[42;1mNEW( %u ): %p\033[0m\r\n", size, p );

   return p;
}


void Thread::operator delete( void *p )
{
   kprintf( "\033[42;1mDELETE( %p )\033[0m\r\n", p );
   vPortFree( p );
}
#endif


//  - - - - - - - - - - - - - - -  //
//  P R I V A T E   M E T H O D S  //
//  - - - - - - - - - - - - - - -  //

void Thread::_wait( void )
{

   while( _thst != RUNNING ) {

      switch( _thst ) {

         case RUNNING:
            /* can't happen - keep gcc happy */
            break;

         case RESUMING:
            onResume();
            _thst = RUNNING;
            break;

         case SUSPENDING:
            onSuspend();
            _thst = SUSPENDED;
            break;

         case SUSPENDED:
            ::ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
            break;

      }
   }

/*
   while( _thst == SUSPENDED) {
      onSuspend();
      ::ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
      onResume();
   }
*/
}


unsigned Thread::_stackHWM( void )
{
   uint32_t hwm = uxTaskGetStackHighWaterMark( handle );
   return( stackDepth - hwm );
}

/*EoF*/

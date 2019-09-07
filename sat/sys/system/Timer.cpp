
#include "system/qb50.h"
#include "system/Timer.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

Timer::Timer()
{
   prev = 0;
}


Timer::~Timer()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

Timer& Timer::reset( void )
{
   prev = ::xTaskGetTickCount();

   return *this;
}


Timer& Timer::every( unsigned ms )
{
   if( prev == 0 ) {
      reset();
   }

   TickType_t incr = (TickType_t)ms / portTICK_RATE_MS;
   (void)::vTaskDelayUntil( &prev, incr );

   return *this;
}


Timer& Timer::until( TickType_t tick )
{
   (void)::vTaskDelayUntil( &prev, tick );
   return *this;
}


/*EoF*/


#include "CommandBacklog.h"

using namespace qb50;

CommandBacklog qb50::CBL( 16 );


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

CommandBacklog::CommandBacklog( size_t max )
{
   _forms = new Form[ max ];
   _lock  = xSemaphoreCreateMutex();
   _max   = max;
   _cur   = 0;
}


CommandBacklog::~CommandBacklog()
{
   vSemaphoreDelete( _lock );
   delete[] _forms;
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

bool CommandBacklog::push( Form& form )
{
   (void)xSemaphoreTake( _lock, portMAX_DELAY );

   if( _cur < _max ) {
      _forms[ _cur++ ] = form;
      (void)xSemaphoreGive( _lock );
      return true;
   }

   (void)xSemaphoreGive( _lock );
   return false;
}


bool CommandBacklog::pop( Form& form )
{
   (void)xSemaphoreTake( _lock, portMAX_DELAY );

   if( _cur > 0 ) {
      form = _forms[ --_cur ];
      (void)xSemaphoreGive( _lock );
      return true;
   }

   (void)xSemaphoreGive( _lock );
   return false;
}


/*EoF*/

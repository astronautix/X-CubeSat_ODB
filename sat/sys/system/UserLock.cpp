
#include "system/UserLock.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

UserLock::UserLock()
{
	_lock = xSemaphoreCreateBinary();
	if( _lock == NULL )
		throw( "xSemaphoreCreateBinary() failed" ); /* XXX exception */

	xSemaphoreGive( _lock );
}


UserLock::~UserLock()
{
	vSemaphoreDelete( _lock );
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

void UserLock::P( void )
{
	xSemaphoreTake( _lock, portMAX_DELAY );
}


void UserLock::V( void )
{
	xSemaphoreGive( _lock );
}

/*EoF*/

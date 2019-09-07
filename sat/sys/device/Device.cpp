
#include "Device.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

Device::Device( const char *name )
   : _name( name ), _refCount( 0 )
{
   _devLock = xSemaphoreCreateMutex();
}


Device::~Device()
{
   vSemaphoreDelete( _devLock );
}


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

Device& Device::lock( void )
{
   (void)xSemaphoreTake( _devLock, portMAX_DELAY );
   return *this;
}


Device& Device::unlock( void )
{
   (void)xSemaphoreGive( _devLock );
   return *this;
}


//  - - - - - - - - - - - - - - -  //
//  P R I V A T E   M E T H O D S  //
//  - - - - - - - - - - - - - - -  //

unsigned Device::_incRef( void )
{ return _refCount++; }


unsigned Device::_decRef( void )
{
   if( _refCount > 0 ) _refCount--;
      return _refCount;
}

/*EoF*/

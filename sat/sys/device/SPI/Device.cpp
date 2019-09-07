
#include "SPI/Device.h"
#include "system/Logger.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

SPI_Device::SPI_Device( const char *name, SPI& spi, GPIO::Pin& csPin, SelectMode csMode )
   : Device( name ), _spi( spi ), _csPin( csPin ), _csMode( csMode )
{ ; }


SPI_Device::~SPI_Device()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

SPI_Device& SPI_Device::init( void )
{
   if( _csMode == ActiveLow ) {
      _csPin.out().on();
   } else {
      _csPin.out().off();
   }

   return *this;
}


SPI_Device& SPI_Device::lock( void )
{
   _spi.lock();
   Device::lock();

   return *this;
}


SPI_Device& SPI_Device::unlock( void )
{
   Device::unlock();
   _spi.unlock();

   return *this;
}


//  - - - - - - - - - - - - - - -  //
//  P R I V A T E   M E T H O D S  //
//  - - - - - - - - - - - - - - -  //

void SPI_Device::_select( void )
{
   if( _csMode == ActiveLow ) {
      _csPin.off();
   } else {
      _csPin.on();
   }
}


void SPI_Device::_deselect( void )
{
   if( _csMode == ActiveLow ) {
      _csPin.on();
   } else {
      _csPin.off();
   }
}

/*EoF*/

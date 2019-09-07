
#include "SPI/MAX111x.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

MAX111x::Channel::Channel( MAX111x& adc, const char *name, ChId id )
: Device( name ), ADC::Channel( adc ), _id( id )
{ ; }


MAX111x::Channel::~Channel()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

MAX111x::Channel&
MAX111x::Channel::init( void )
{
 //_adc.init();
   return *this;
}


MAX111x::Channel&
MAX111x::Channel::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   MAX111x& adc = static_cast<MAX111x&>( _adc );
   adc.enable( debug );

   return *this;
}


MAX111x::Channel&
MAX111x::Channel::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   MAX111x& adc = static_cast<MAX111x&>( _adc );
   adc.disable( debug );

   return *this;
}

/*EoF*/

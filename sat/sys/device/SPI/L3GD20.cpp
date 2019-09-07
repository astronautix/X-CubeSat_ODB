
#include "SPI/L3GD20.h"
#include "system/Application.h"
#include "system/Logger.h"

using namespace qb50;


/* SPI commands */

static const uint8_t WHO_AM_I_Cmd[ 2 ]     = { 0x8f, 0xff };
static const uint8_t ENABLE_XYZ_Cmd[ 2 ]   = { 0x20, 0x0f };
static const uint8_t DISABLE_XYZ_Cmd[ 2 ]  = { 0x20, 0x00 };
static const uint8_t STATUS_REG_Cmd[ 2 ]   = { 0xa7, 0xff };
static const uint8_t OMEGA_Cmd[ 7 ]        = { 0xe8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
static const uint8_t RATE_250DPS_Cmd[ 2 ]  = { 0x23, 0x00 };
static const uint8_t RATE_500DPS_Cmd[ 2 ]  = { 0x23, 0x01 };
static const uint8_t RATE_2000DPS_Cmd[ 2 ] = { 0x23, 0x02 };

static const float m250DPS  = 0.00875F;
static const float m500DPS  = 0.01750F;
static const float m2000DPS = 0.07000F;

//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

L3GD20::L3GD20( SPI&        spi,
                const char *name,
                GPIO::Pin&  csPin,
                Range       r )
   : SPI_Device( name, spi, csPin, SPI_Device::ActiveLow ),
     _range( r )
{
   _calX = _calY = _calZ = 0;
}


L3GD20::~L3GD20()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

L3GD20& L3GD20::init( void )
{
   (void)SPI_Device::init();

   (void)reset();

   kprintf( "%s: Onboard L3GD20 MEMS motion sensor, cs: %s\r\n",
            _name, _csPin.name() );

   return *this;
}


L3GD20& L3GD20::reset( void )
{
   uint8_t id;

   (void)enable();

   _WHO_AM_I( id );
   kprintf( "%s: id: 0x%02x\r\n", _name, id );

   range( R250DPS );

   (void)disable();

   return *this;
}


L3GD20& L3GD20::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   _spi.enable( debug );

   _spi.lock();
   _select();
   _spi.pollXfer( ENABLE_XYZ_Cmd, NULL, 2 );
   _deselect();
   _spi.unlock();

   if( debug ) {
      kprintf( "%s: enabled\r\n", _name );
   }

   return *this;
}


L3GD20& L3GD20::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   _spi.lock();
   _select();
   _spi.pollXfer( DISABLE_XYZ_Cmd, NULL, 2 );
   _deselect();
   _spi.unlock();

   _spi.disable( debug );

   if( debug ) {
      kprintf( "%s: disabled\r\n", _name );
   }

   return *this;
}


L3GD20& L3GD20::omega( Vec3D& v )
{
   uint8_t res[ 8 ];
   float coef;

   _spi.lock();
   _select();
   _spi.pollXfer( OMEGA_Cmd, res, 7 /*sizeof( OMEGA_Cmd )*/);
   _deselect();
   _spi.unlock();

   int16_t xr = (uint16_t)res[1] | ( (uint16_t)res[2] << 8 );
   int16_t yr = (uint16_t)res[3] | ( (uint16_t)res[4] << 8 );
   int16_t zr = (uint16_t)res[5] | ( (uint16_t)res[6] << 8 );

   switch( _range ) {
      case R250DPS:  coef = m250DPS;  break;
      case R500DPS:  coef = m500DPS;  break;
      case R2000DPS: coef = m2000DPS; break;
   }

   v.x = coef * ( xr - _calX );
   v.y = coef * ( yr - _calY );
   v.z = coef * ( zr - _calZ );

   return *this;
}


L3GD20& L3GD20::range( Range r )
{
   uint8_t res[ 2 ];

   _spi.lock();
   _select();

   switch( r ) {

      case R250DPS:
         _spi.pollXfer( RATE_250DPS_Cmd, res, 2 );
         break;

      case R500DPS:
         _spi.pollXfer( RATE_500DPS_Cmd, res, 2 );
         break;

      case R2000DPS:
         _spi.pollXfer( RATE_2000DPS_Cmd, res, 2 );
         break;

   }

   _deselect();
   _spi.unlock();

   return *this;
}


L3GD20& L3GD20::calibrate( void )
{
   int32_t sum[ 3 ];
   uint8_t res[ 8 ];

   sum[ 0 ] = sum[ 1 ] = sum[ 2 ] = 0;

   _spi.lock();

   for( int i = 0 ; i < 110 ; ++i ) {

      delay( 10 );

      _select();
      _spi.pollXfer( OMEGA_Cmd, res, 7 );
      _deselect();

      if( i >= 10 ) { /* ignore the first 10 readouts */
         sum[ 0 ] += (int16_t)((uint16_t)res[1] | ((uint16_t)res[2] << 8 ));
         sum[ 1 ] += (int16_t)((uint16_t)res[3] | ((uint16_t)res[4] << 8 ));
         sum[ 2 ] += (int16_t)((uint16_t)res[5] | ((uint16_t)res[6] << 8 ));
      }
   }

   _spi.unlock();

   _calX = sum[ 0 ] / 100;
   _calY = sum[ 1 ] / 100;
   _calZ = sum[ 2 ] / 100;

   kprintf( "%s: CAL [ %d %d %d ]\r\n", _name, _calX, _calY, _calZ );

   return *this;
}


//  - - - - - - - - - - - - - - -  //
//  P R I V A T E   M E T H O D S  //
//  - - - - - - - - - - - - - - -  //

void L3GD20::_WHO_AM_I( uint8_t& id )
{
   uint8_t res[ 2 ];

   _spi.lock();
   _select();
   _spi.pollXfer( WHO_AM_I_Cmd, res, 2 /*sizeof( res )*/);
   _deselect();
   _spi.unlock();

   id = res[1];
}

/*EoF*/

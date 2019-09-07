
#include "SPI/LSM303.h"
#include "system/Application.h"
#include "system/Logger.h"

using namespace qb50;


/* SPI commands */

static const uint8_t WHO_AM_I_Cmd[ 2 ]     = { 0x8f, 0xff };
static const uint8_t ENABLE_XYZ_Cmd[ 2 ]   = { 0x20, 0x0f };
static const uint8_t DISABLE_XYZ_Cmd[ 2 ]  = { 0x20, 0x00 };
static const uint8_t STATUS_REG_Cmd[ 2 ]   = { 0xa7, 0xff };
static const uint8_t OMEGA_Cmd[ 7 ]        = { 0xc8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
static const uint8_t RATE_250DPS_Cmd[ 2 ]  = { 0x23, 0x00 };
static const uint8_t RATE_500DPS_Cmd[ 2 ]  = { 0x23, 0x01 };
static const uint8_t RATE_2000DPS_Cmd[ 2 ] = { 0x23, 0x02 };

/*
          +-------- write
          |+------- increment address
          ||+------ CTRL5 (0x24)
          |||_____
   0x64 = 01100100

          +-------- TEMP_EN (temperature sensor) disabled
          |+------- M_RES (resolution): high
          ||
          || +----- M_ODR (data rate): 6.25Hz
          || |
          || |
          || |  +-- LIR2 (interrupt not latched)
          || |  |+- LIR1 (interrupt not latched)
          ||_|__||
   0x64 = 01100100  <- CTRL5

           +------- MFS (mag. full scale): +/- 2 gauss
           |_
   0x00 = 00000000  <- CTRL6

          +-------- AHPM (HP filter): normal mode
          |
          | +------ AFDS (filter): disabled
          | |+----- T_ONLY: disabled
          | ||
          | || +--- MLP (mag. low power): mag. data rate set by CRTL5
          | || |+-- MD (mag. sensor mode): continuous conversion
          | || ||
          |_|| ||_
   0x00 = 00000000  <- CTRL7

 */

static const uint8_t MAG_INIT_Cmd[ 4 ]  = { 0x64, 0x64, 0x00, 0x00 };


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

LSM303::LSM303( SPI&        spi,
                const char *name,
                GPIO::Pin&  csPin,
                Range       r )
   : SPI_Device( name, spi, csPin, SPI_Device::ActiveLow ),
     _range( r )
{
   _calX = _calY = _calZ = 0;
}


LSM303::~LSM303()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

LSM303& LSM303::init( void )
{
   (void)SPI_Device::init();

   (void)reset();

   kprintf( "%s: Onboard LSM303 MEMS accelerometer/magnetometer, cs: %s\r\n",
            _name, _csPin.name() );

   return *this;
}



LSM303& LSM303::reset( void )
{
   uint8_t res[ 4 ];
   uint8_t id;

   (void)enable();

   _WHO_AM_I( id );
   kprintf( "%s: id: 0x%02x\r\n", _name, id );

   _spi.lock();
   _select();
   _spi.pollXfer( MAG_INIT_Cmd, res, 4 );
   _deselect();
   _spi.unlock();

 //range( R250DPS );

   (void)disable();

   return *this;
}


LSM303& LSM303::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   _spi.enable( debug );

/*
   _spi.lock();
   _select();
   _spi.pollXfer( ENABLE_XYZ_Cmd, NULL, 2 );
   _deselect();
   _spi.unlock();
*/

   if( debug ) {
      kprintf( "%s: enabled\r\n", _name );
   }

   return *this;
}


LSM303& LSM303::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

/*
   _spi.lock();
   _select();
   _spi.pollXfer( DISABLE_XYZ_Cmd, NULL, 2 );
   _deselect();
   _spi.unlock();
*/

   _spi.disable( debug );

   if( debug ) {
      kprintf( "%s: disabled\r\n", _name );
   }

   return *this;
}



LSM303& LSM303::omega( Vec3D& v )
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

   coef = 3.0;

   v.x = coef * ( xr - _calX );
   v.y = coef * ( yr - _calY );
   v.z = coef * ( zr - _calZ );

   return *this;
}


LSM303& LSM303::range( Range r )
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


LSM303& LSM303::calibrate( void )
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

void LSM303::_WHO_AM_I( uint8_t& id )
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

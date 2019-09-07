
#include "SPI/A25Lxxx.h"
#include "system/Application.h"
#include "system/Logger.h"

#define A25Lxxx_HARD_LIMIT 100

using namespace qb50;

/*
 * 64 blocks
 * 64 blocks * 16 sectors/block = 1024 sectors
 * 64 blocks * 16 sectors/block * 16 pages/sector = 16384 pages
 * 64 blocks * 16 sectors/block * 16 pages/sector * 256 bytes/page = 4194304 bytes
 */

/* typical Sector Erase Cycle Time (ms) */
#define tSE 40

/* typical Block Erase Cycle Time (ms) */
#define tBE 500

/* typical Chip Erase Cycle Time (ms) */
#define tCE 32000

/* typical Page Program Cycle Time (ms) */
#define tPP 2

/* supported A25Lxxx chips */

A25Lxxx::A25LChip A25Lxxx::chips[] = {
  /* sig.    mask     part #            bpc  spb  pps  bpp */
   { 0x3010, 0xffff, "AMIC A25L512",      1,  16,  16, 256 }, /* 512 Kbit */
   { 0x3011, 0xffff, "AMIC A25L010",      2,  16,  16, 256 }, /*   1 Mbit */
   { 0x3012, 0xffff, "AMIC A25L020",      4,  16,  16, 256 }, /*   2 Mbit */
   { 0x3013, 0xffff, "AMIC A25L040",      8,  16,  16, 256 }, /*   4 Mbit */
   { 0x3014, 0xffff, "AMIC A25L080",     16,  16,  16, 256 }, /*   8 Mbit */
   { 0x3015, 0xffff, "AMIC A25L016",     32,  16,  16, 256 }, /*  16 Mbit */
   { 0x4015, 0xffff, "AMIC A25LQ16",     32,  16,  16, 256 }, /*  16 Mbit */
   { 0x3016, 0xffff, "AMIC A25L032",     64,  16,  16, 256 }, /*  32 Mbit */
   { 0x4016, 0xffff, "AMIC A25LQ32A",    64,  16,  16, 256 }, /*  32 Mbit */
   { 0x4017, 0xffff, "AMIC A25LQ64",    128,  16,  16, 256 }, /*  64 Mbit */
   { 0xba17, 0xffff, "MICRON N25Q064A", 128,  16,  16, 256 }, /*  64 Mbit */
   {      0,      0,  NULL,               0,   0,   0,   0 }
};

/* SPI commands */

static const uint8_t RDIDCmd[]  = { 0x9f, 0xff, 0xff, 0xff };
static const uint8_t REMSCmd[]  = { 0x90, 0xff, 0xff, 0x00, 0xff, 0xff };
static const uint8_t RDSR1Cmd[] = { 0x05, 0xff };
static const uint8_t RDSR2Cmd[] = { 0x35, 0xff };
static const uint8_t WRENCmd[]  = { 0x06 };
static const uint8_t WRDICmd[]  = { 0x04 };


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

A25Lxxx::A25Lxxx( SPI& spi, const char *name, GPIO::Pin& csPin )
   : SPI_Device( name, spi, csPin, SPI_Device::ActiveLow )
{ ; }


A25Lxxx::~A25Lxxx()
{ ; }


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

A25Lxxx& A25Lxxx::init( void )
{
   RDIDResp rdid;

   (void)SPI_Device::init();

   /* temporarily (debugly) enable the device
    * in order to read its identification ID */

   lock();
   enable();
   _RDID( &rdid );
   disable();
   unlock();

   /* display ID infos */

   uint16_t  sig  = ( rdid.memType << 8 ) | rdid.memCap;
   A25LChip *chip = chips;

   while(( chip->mask != 0 ) && ( chip->sig != ( sig & chip->mask )))
      ++chip;

   if( chip->mask == 0 ) {
      kprintf( RED( "%s: unknown chip - using default" ) "\r\n", _name );
      kprintf( "%s:   manID: 0x%04x\r\n", _name, rdid.manId   );
      kprintf( "%s: memType: 0x%04x\r\n", _name, rdid.memType );
      kprintf( "%s:  memCap: 0x%04x\r\n", _name, rdid.memCap  );
      chip = &chips[7];
   }

   _geo.bpc = chip->bpc;
   _geo.spb = chip->spb;
   _geo.pps = chip->pps;
   _geo.bpp = chip->bpp;

   kprintf( "%s: Onboard %s serial flash (%luMbit), cs: %s\r\n",
            _name, chip->name, ( chipSize() >> 17 ), _csPin.name() );

   kprintf( "%s: %lu blocks * %lu sectors * %lu pages * %lu bytes\r\n",
            _name, _geo.bpc, _geo.spb, _geo.pps, _geo.bpp );

   return *this;
}


A25Lxxx& A25Lxxx::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   _spi.enable( debug );

   if( debug ) {
      kprintf( "%s: enabled\r\n", _name );
   }

   return *this;
}


A25Lxxx& A25Lxxx::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   _spi.disable( debug );

   if( debug ) {
      kprintf( "%s: disabled\r\n", _name );
   }

   return *this;
}


A25Lxxx& A25Lxxx::pageRead( uint32_t addr, void *x )
{
   lock();
   _READ( addr, x );
   unlock();

   return *this;
}


A25Lxxx& A25Lxxx::pageWrite( uint32_t addr, const void *x )
{
   lock();
   _WREN();
   _PP( addr, x );
   unlock();

   return *this;
}


A25Lxxx& A25Lxxx::sectorErase( uint32_t addr )
{
   lock();
   _WREN();
   _SE( addr );
   unlock();

   return *this;
}


A25Lxxx& A25Lxxx::sectorRead( uint32_t addr, void *x )
{
   uint8_t *dst = (uint8_t*)x;

   lock();
   for( int i = 0 ; i < _geo.pps ; ++i ) {
      (void)_READ( addr, dst );
      dst  += _geo.bpp;
      addr += _geo.bpp;
   }
   unlock();

   return *this;
}


A25Lxxx& A25Lxxx::sectorWrite( uint32_t addr, const void *x )
{
   const uint8_t *src = (const uint8_t*)x;

   lock();
   _WREN();
   _SE( addr );

   for( int i = 0 ; i < _geo.pps ; ++i ) {
      _WREN();
      _PP( addr, src );
      src  += _geo.bpp;
      addr += _geo.bpp;
   }
   unlock();

   return *this;
}


A25Lxxx& A25Lxxx::blockErase( uint32_t addr )
{
   lock();
   _WREN();
   _BE( addr );
   unlock();

   return *this;
}


//  - - - - - - - - - - - - - - -  //
//  P R I V A T E   M E T H O D S  //
//  - - - - - - - - - - - - - - -  //

void A25Lxxx::_RDID( RDIDResp *rdid )
{
   _select();
   _spi.pollXfer( RDIDCmd, rdid, sizeof( RDIDCmd ));
   _deselect();
}


void A25Lxxx::_READ( uint32_t addr, void *x )
{
   uint8_t cmd[ 4 ];

   cmd[ 0 ] = 0x03;
   cmd[ 1 ] = ( addr >> 16 ) & 0x3f;
   cmd[ 2 ] = ( addr >>  8 ) & 0xff;
   cmd[ 3 ] =   addr         & 0xff;

   _select();
   _spi.pollXfer( cmd, NULL, sizeof( cmd ));
   _spi.read( x, 256 /*req->_len*/ );
   _deselect();
}


void A25Lxxx::_SE( uint32_t addr )
{
   uint8_t cmd[ 4 ];

   cmd[ 0 ] = 0x20;
   cmd[ 1 ] = ( addr >> 16 ) & 0x3f;
   cmd[ 2 ] = ( addr >>  8 ) & 0xf0;
   cmd[ 3 ] =   addr         & 0x00;

   _select();
   _spi.pollXfer( cmd, NULL, sizeof( cmd ));
   _deselect();

   _WIPWait( tSE );
}


void A25Lxxx::_BE( uint32_t addr )
{
   uint8_t cmd[ 4 ];

   cmd[ 0 ] = 0x20;
   cmd[ 1 ] = ( addr >> 16 ) & 0x3f;
   cmd[ 2 ] = ( addr >>  8 ) & 0x00;
   cmd[ 3 ] =   addr         & 0x00;

   _select();
   _spi.pollXfer( cmd, NULL, sizeof( cmd ));
   _deselect();

   _WIPWait( tBE );
}


void A25Lxxx::_PP( uint32_t addr, const void *x )
{
   uint8_t cmd[ 4 ];

   cmd[ 0 ] = 0x02;
   cmd[ 1 ] = ( addr >> 16 ) & 0x3f;
   cmd[ 2 ] = ( addr >>  8 ) & 0xff;
   cmd[ 3 ] =   addr         & 0xff;

   _select();
   _spi.pollXfer( cmd, NULL, sizeof( cmd ));
   _spi.write( x, 256 /*req->_len*/ );
   _deselect();

   _WIPWait( tPP );
}


void A25Lxxx::_WREN( void )
{
   _select();
   _spi.pollXfer( WRENCmd, NULL, sizeof( WRENCmd ));
   _deselect();
}


void A25Lxxx::_WRDI( void )
{
   _select();
   _spi.pollXfer( WRDICmd, NULL, sizeof( WRDICmd ));
   _deselect();
}


void A25Lxxx::_REMS( REMSResp *rems )
{
   _select();
   _spi.pollXfer( REMSCmd, rems, sizeof( REMSCmd ));
   _deselect();
}


void A25Lxxx::_WIPWait( unsigned ms )
{
   RDSRResp rdsr;

   int n;
   uint8_t rx;
   uint8_t tx = 0xff;

   if( ms > 0 ) {
      delay( ms );
      ms >>= 3;
   }

   _select();
   _spi.pollXfer( RDSR1Cmd, &rdsr, sizeof( RDSR1Cmd ));

   if( rdsr.sr & 0x01 ) {
      for( n = 0 ; n < A25Lxxx_HARD_LIMIT ; ++n ) {
         if( ms > 0 ) delay( ms );
         _spi.pollXfer( &tx, &rx, 1 );
         if( !( rx & 0x01 )) break;
      }

      if( n == A25Lxxx_HARD_LIMIT ) {
         kprintf( RED( "%s: timeout in A25Lxxx::_WIPWait()" ) "\r\n", _name );
      }
   }
   _deselect();
}

/*EoF*/

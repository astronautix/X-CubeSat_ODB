
#include "device/FlashArray.h"
#include "system/Logger.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

FlashArray::FlashArray( const char *name, const int nSlaves, FlashMemory *slaves[] )
   : Device( name ), _nSlaves( nSlaves ), _slaves( slaves )
{ ; }


FlashArray::~FlashArray()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

/* Device interface */

FlashArray& FlashArray::init( void )
{
   FlashMemory::Geometry geo;

   // XXX check that _nSlaves > 0
   // XXX check that ( _nSlaves & ( _nSlaves - 1 )) == 0

   _slaves[0]->init();
   _slaves[0]->geometry( &_geo );

   for( unsigned i = 1 ; i < _nSlaves ; ++i ) {
      _slaves[i]->init();
      _slaves[i]->geometry( &geo );
      // XXX check that geo.ppb == _geo.ppb
      // XXX check that geo.bpp == _geo.bpp
      _geo.bpc += geo.bpc;
   }

   uint32_t sz = _slaves[0]->chipSize();
   _mask = sz - 1;
   _shft = log2( sz );

   kprintf( "%s: Virtual flash memory device (%lu Mbit)\r\n",
            _name, ( chipSize() >> 17 ));
   kprintf( "%s: %d blocks * %d sectors * %d pages * %d bytes\r\n",
            _name, _geo.bpc, _geo.spb, _geo.pps, _geo.bpp );

   return *this;
}


FlashArray& FlashArray::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   for( unsigned i = 0 ; i < _nSlaves ; ++i )
      _slaves[i]->enable( debug );

   if( debug )
      kprintf( "%s: enabled\r\n", _name );

   return *this;
}


FlashArray& FlashArray::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   for( unsigned i = 0 ; i < _nSlaves ; ++i )
      _slaves[i]->disable( debug );

   if( debug )
      kprintf( "%s: disabled\r\n", _name );

   return *this;
}


/* FlashMemory interface */

FlashArray& FlashArray::pageRead( uint32_t addr, void *x )
{
   unsigned sn = getSn( addr );
   _slaves[ sn ]->pageRead( addr & _mask, x );

   return *this;
}


FlashArray& FlashArray::pageWrite( uint32_t addr, const void *x )
{
   unsigned sn = getSn( addr );
   _slaves[ sn ]->pageWrite( addr & _mask, x );

   return *this;
}


FlashArray& FlashArray::sectorErase( uint32_t addr )
{
   unsigned sn = getSn( addr );
   _slaves[ sn ]->sectorErase( addr & _mask );

   return *this;
}


FlashArray& FlashArray::sectorRead( uint32_t addr, void *x )
{
   unsigned sn = getSn( addr );
   _slaves[ sn ]->sectorRead( addr & _mask, x );

   return *this;
}


FlashArray& FlashArray::sectorWrite( uint32_t addr, const void *x )
{
   unsigned sn = getSn( addr );
   _slaves[ sn ]->sectorWrite( addr & _mask, x );

   return *this;
}


FlashArray& FlashArray::blockErase( uint32_t addr )
{
   unsigned sn = getSn( addr );
   _slaves[ sn ]->blockErase( addr & _mask );

   return *this;
}


//  - - - - - - - - - - - - - - -  //
//  P R I V A T E   M E T H O D S  //
//  - - - - - - - - - - - - - - -  //

unsigned FlashArray::log2( uint32_t u )
{
   unsigned r = 0;

   if( u & 0xffff0000 /* 11111111111111110000000000000000 */ ) { u >>= 16; r |= 0x10; }
   if( u & 0x0000ff00 /* 00000000000000001111111100000000 */ ) { u >>=  8; r |= 0x08; }
   if( u & 0x000000f0 /* 00000000000000000000000011110000 */ ) { u >>=  4; r |= 0x04; }
   if( u & 0x0000000c /* 00000000000000000000000000001100 */ ) { u >>=  2; r |= 0x02; }
   if( u & 0x00000002 /* 00000000000000000000000000000010 */ ) { u >>=  1; r |= 0x01; }

   return r;
}


unsigned FlashArray::getSn( uint32_t addr )
{
   unsigned sn = ( addr >> _shft );

   if( sn >= _nSlaves ) {
      kprintf( RED( "%s: invalid slave %u/%u requested" ) "\r\n", _name, sn, _nSlaves );
      sn %= _nSlaves;
   }

   return sn;
}


/*EoF*/

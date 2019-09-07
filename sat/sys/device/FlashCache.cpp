
#include "FlashCache.h"
#include "system/Logger.h"
#include "system/CRC32.h"

#include <cstring>

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

FlashCache::FlashCache( const char *name, FlashMemory& mem )
   : Device( name ), _mem( mem )
{
   _nhit = 0;
   _nmis = 0;
   _drty = false;
   _base = (uint32_t)-1;
   _psiz = 0;
   _vsiz = 0;
   _vcap = 0;
   _buf  = (uint8_t*)0;
   _tmp  = (uint8_t*)0;
}


FlashCache::~FlashCache()
{
   if( _tmp != (uint8_t*)0 )
      delete[] _tmp;

   if( _buf != (uint8_t*)0 )
      delete[] _buf;
}


//  - - - - - - - - - - - - - -  //
//  P U B L I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

FlashCache& FlashCache::init( void )
{
   (void)_mem.init();

   FlashMemory::Geometry geo;
   _mem.geometry( &geo );

   _psiz = _mem.sectorSize();
   _vsiz = _psiz - sizeof( Header );
   _vcap = _vsiz * geo.bpc * geo.spb;
   _buf  = new uint8_t[ _psiz ];
   _tmp  = new uint8_t[ _psiz ];

   kprintf( "%s: Flash Cache (capacity: %u/%uKbit)\r\n",
            _name,
            _vcap >> 7,
            _mem.chipSize() >> 7
   );

 //kprintf( "%s: Cache size: %u\r\n", _name, _psiz );

   return *this;
}


FlashCache& FlashCache::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   (void)_mem.enable( debug );

   if( debug )
      kprintf( "%s: enabled\r\n", _name );

   return *this;
}


FlashCache& FlashCache::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   (void)_mem.disable( debug );

   if( debug )
      kprintf( "%s: disabled\r\n", _name );

   return *this;
}


FlashCache& FlashCache::clear( void )
{
   FlashMemory::Geometry geo;
   _mem.geometry( &geo );

   unsigned n = geo.bpc * geo.spb;
   uint32_t paddr = 0;

   for( unsigned i = 0 ; i < n ; ++i ) {
      kprintf( "%s: Formatting sector %u/%u...\r\n", _name, i, n );
      _clear( paddr );
      paddr += _psiz;
   }

   return *this;
}


FlashCache& FlashCache::read( uint32_t vaddr, void *x, size_t len )
{
   if( len == 0 ) {
      kprintf( "%s: FlashCache::read(): 0 bytes requested\r\n", _name );
      return *this;
   }

   Header *hdr = (Header*)_buf;

   uint8_t* dst = (uint8_t*)x;
   uint32_t off = vaddr % _vsiz;
   uint32_t nb  = _vsiz - off;

   if( nb > len ) nb = len;

if( len > 1024 ) {
   kprintf( "FlashCache::read( %u, %p, %u )\r\n", vaddr, x, len );
   for( ;; );
}

   (void)lock();

   /* read sectors */

   while( nb > 0 ) {

      _load( vaddr );
      (void)memcpy( dst, hdr->x + off, nb );

      len   -= nb;
      dst   += nb;
      vaddr += nb;
      off    = 0;
      nb     = ( len > _vsiz ) ? _vsiz : len;

   }

   (void)unlock();

 //kprintf( "%s: hits: %u, misses: %u\r\n", _name, _nhit, _nmis );

   return *this;
}


FlashCache& FlashCache::write( uint32_t vaddr, const void *x, size_t len, bool sync )
{
   if( len == 0 ) {
      kprintf( "%s: FlashCache::write(): nothing to write\r\n", _name );
      if( sync ) {
         /* let the user a chance to flush the cache */
         (void)lock();
         _flush();
         (void)unlock();
      }
      return *this;
   }

   Header *hdr = (Header*)_buf;

   const uint8_t* src = (const uint8_t*)x;
   uint32_t       off = vaddr % _vsiz;
   uint32_t       nb  = _vsiz - off;

   if( nb > len ) nb = len;

 //kprintf( "FlashCache::write( %u, %p, %u )\r\n", vaddr, x, len );

   /* lock the device */

   (void)lock();

   /* write sectors */

   while( nb > 0 ) {

      _load( vaddr );
      (void)memcpy( hdr->x + off, src, nb );
      _drty = true;

      len   -= nb;
      src   += nb;
      vaddr += nb;
      off    = 0;
      nb     = ( len > _vsiz ) ? _vsiz : len;

   }

   /**/

   if( sync ) {
      /* synchronous operation requested: force a sector flush */
      _flush();
   }

   (void)unlock();

 //kprintf( "%s: hits: %u, misses: %u\r\n", _name, _nhit, _nmis );

   return *this;
}


//  - - - - - - - - - - - - - - -  //
//  P R I V A T E   M E T H O D S  //
//  - - - - - - - - - - - - - - -  //

/* get physical sector address from logical memory address */

uint32_t FlashCache::_v2p( uint32_t vaddr )
{
   return _psiz * ( vaddr / _vsiz );
}


void FlashCache::_load( uint32_t vaddr )
{
   Header  *hdr   = (Header*)_buf;
   uint32_t paddr = _v2p( vaddr );

 //kprintf( "FlashCache::_load( %u )\r\n", vaddr );

   if( paddr == _base ) {
      /* sector is already in cache: this is a hit */
      ++_nhit;
      return;
   }

   /* sector not in cache */

   ++_nmis;

   if( _drty ) {
      /* flush the sector if cache is dirty */
      _flush();
      /* cache is now clean */
   }

   /* load the requested sector */

   _mem.sectorRead( paddr, _buf );

   uint32_t crc = crc32( hdr->x, _vsiz );
   if( crc != hdr->crc32 ) {
      kprintf( RED( "%s: (load) CRC mismatch at physical address 0x%08x" ) "\r\n", _name, paddr );
      kprintf( RED( "%s: (load) got 0x%08x, expected 0x%08x" ) "\r\n", _name, crc, hdr->crc32 );
   } else {
      kprintf( GREEN( "%s: (load) CRC verified at physical address 0x%08x" ) "\r\n", _name, paddr );
      kprintf( GREEN( "%s: (load) got 0x%08x" ) "\r\n", _name, crc );
   }

   _base = paddr;
}


void FlashCache::_clear( uint32_t paddr )
{
   Header *hdr = (Header*)_tmp;

   _mem.sectorErase ( paddr       );
   _mem.sectorRead  ( paddr, _tmp );

   hdr->crc32 = crc32( hdr->x, _vsiz );

   _mem.sectorWrite ( paddr, _tmp );
   _mem.sectorRead  ( paddr, _tmp );

   uint32_t crc = crc32( hdr->x, _vsiz );
   if( crc != hdr->crc32 ) {
      kprintf( RED( "%s: (clear) CRC mismatch at physical address 0x%08x" ) "\r\n", _name, paddr );
      kprintf( RED( "%s: (clear) got 0x%08x, expected 0x%08x" ) "\r\n", _name, crc, hdr->crc32 );
   }
}


void FlashCache::_flush( void )
{
   Header *hdr;

   hdr = (Header*)_buf;
   hdr->crc32 = crc32( hdr->x, _vsiz );

   _mem.sectorErase( _base );
   _mem.sectorWrite( _base, _buf );

   /* read back and check CRC */

   hdr = (Header*)_tmp;
   _mem.sectorRead( _base, _tmp );

   uint32_t crc = crc32( hdr->x, _vsiz );
   if( crc != hdr->crc32 ) {
      kprintf( RED( "%s: (flush) CRC mismatch at physical address 0x%08x" ) "\r\n", _name, _base );
      kprintf( RED( "%s: (flush) got 0x%08x, expected 0x%08x" ) "\r\n", _name, crc, hdr->crc32 );
   }

   _drty = false;
}

/*EoF*/

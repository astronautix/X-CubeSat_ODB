
#include "E64.h"
#include "device/Syslog.h"

using namespace qb50;

#define ECHR 0x40
#define CHEQ 0x3d


static const char e64v[ 64 ] = {
   'A', 'B', 'C' ,'D', 'E', 'F', 'G', 'H',
   'I', 'J', 'K' ,'L', 'M', 'N', 'O', 'P',
   'Q', 'R', 'S' ,'T', 'U', 'V', 'W', 'X',
   'Y', 'Z', 'a', 'b', 'c' ,'d', 'e', 'f',
   'g', 'h', 'i', 'j', 'k' ,'l', 'm', 'n',
   'o', 'p', 'q', 'r', 's' ,'t', 'u', 'v',
   'w', 'x', 'y', 'z', '0', '1', '2', '3',
   '4', '5', '6', '7', '8', '9', '+', '/'
};


static const uint8_t d64v[ 128 ] = {
   ECHR, ECHR, ECHR, ECHR, ECHR, ECHR, ECHR, ECHR,  ECHR, ECHR, ECHR, ECHR, ECHR, ECHR, ECHR, ECHR,
   ECHR, ECHR, ECHR, ECHR, ECHR, ECHR, ECHR, ECHR,  ECHR, ECHR, ECHR, ECHR, ECHR, ECHR, ECHR, ECHR,
   ECHR, ECHR, ECHR, ECHR, ECHR, ECHR, ECHR, ECHR,  ECHR, ECHR, ECHR, 0x3e, ECHR, ECHR, ECHR, 0x3f,
   0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,  0x3c, 0x3d, ECHR, ECHR, ECHR, 0x00, ECHR, ECHR,

   ECHR, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,  0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
   0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,  0x17, 0x18, 0x19, ECHR, ECHR, ECHR, ECHR, ECHR,
   ECHR, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,  0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
   0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,  0x31, 0x32, 0x33, ECHR, ECHR, ECHR, ECHR, ECHR
};


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //


E64::E64( unsigned len )
{
   _x   = new char[ len ];
   _off = 0;
   _len = len;
}


E64::~E64()
{
   delete[] _x;
}


E64& E64::push( const void *src, unsigned len )
{
   const uint8_t *_src = (const uint8_t*)src;
   uint8_t a, b, c;

   while(( _off < _len ) && ( len > 0 )) {
      a = _src[ 0 ];
      _x[ _off ] = e64v[ a >> 2 ];
      if( len > 1 ) {
         b = _src[ 1 ];
         _x[ _off + 1 ] = e64v[ (( a & 0x03 ) << 4 ) | (( b & 0xf0 ) >> 4 ) ];
         if( len > 2 ) {
            c = _src[ 2 ];
            _x[ _off + 2 ] = e64v[ (( b & 0x0f ) << 2 ) | (( c & 0xc0 ) >> 6 ) ];
            _x[ _off + 3 ] = e64v[  ( c & 0x3f ) ];
            len -= 3;
         } else {
            _x[ _off + 2 ] = e64v[ ( b & 0x0f ) << 2 ];
            _x[ _off + 3 ] = '=';
            len -= 2;
         }
      } else {
         _x[ _off + 1 ] = e64v[ (( a & 0x03 ) << 4 ) ];
         _x[ _off + 2 ] = '=';
         _x[ _off + 3 ] = '=';
         len -= 1;
      }

      _src += 3;
      _off += 4;
   }

   return *this;
}


E64& E64::dump( void )
{
   unsigned len = _off;
   unsigned off = 0;

   while( len > 0 ) {
      unsigned blen = len > 64 ? 64 : len;

      len -= blen;
      (void)kprintf( "%c%.*s\r\n", ( len > 0 ? '+' : '=' ), blen, _x + off );
      off += blen;
   }

   return *this;
}


E64& E64::dump( Dumper *dumper )
{
   unsigned len = _off;
   unsigned off = 0;

   while( len > 0 ) {
      bool     mf   = ( len > 64 );
      unsigned blen = mf ? 64 : len;

      len -= blen;
      dumper->dump( _x + off, blen, mf );
      off += blen;
   }

   return *this;
}

/*EoF*/


#include "Morse.h"
#include "Baseband.h"
#include "system/qb50.h"

using namespace qb50;


#define ZERO 0x000000

#define DOTMS 75


static const uint32_t bits[ 128 ] = {

  /* 0x00 */     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,
  /* 0x08 */     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,
  /* 0x10 */     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,
  /* 0x18 */     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,

  /* 0x20 */     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,
  /* 0x28 */     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,
  /* 0x30 */   0x077777, 0x01dddd, 0x007775, 0x001dd5, 0x000755, 0x000155, 0x000557, 0x001577,
  /* 0x38 */   0x005777, 0x017777,   ZERO,     ZERO,     ZERO,     ZERO,     ZERO,     ZERO,

  /* 0x40 */     ZERO,   0x00001d, 0x000157, 0x0005d7, 0x000057, 0x000001, 0x000175, 0x000177,
  /* 0x48 */   0x000055, 0x000005, 0x001ddd, 0x0001d7, 0x00015d, 0x000077, 0x000017, 0x000777,
  /* 0x50 */   0x0005dd, 0x001d77, 0x00005d, 0x000015, 0x000007, 0x000075, 0x0001d5, 0x0001dd,
  /* 0x58 */   0x000757, 0x001dd7, 0x000577,   ZERO,     ZERO,     ZERO,     ZERO,     ZERO,

  /* 0x60 */     ZERO,   0x00001d, 0x000157, 0x0005d7, 0x000057, 0x000001, 0x000175, 0x000177,
  /* 0x68 */   0x000055, 0x000005, 0x001ddd, 0x0001d7, 0x00015d, 0x000077, 0x000017, 0x000777,
  /* 0x70 */   0x0005dd, 0x001d77, 0x00005d, 0x000015, 0x000007, 0x000075, 0x0001d5, 0x0001dd,
  /* 0x78 */   0x000757, 0x001dd7, 0x000577,   ZERO,     ZERO,     ZERO,     ZERO,     ZERO

};


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

Morse::Morse( Baseband::Power pow )
   : _pow( pow )
{ ; }


Morse::~Morse()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

void Morse::writeChar( char c )
{
   uint32_t x = bits[ (int)c & 0x7f ];

   while( x ) {
      if( x & 1 ) {
         BB.power( _pow );
      } else {
         BB.power( Baseband::P0 );
      }
      delay( DOTMS );

      x >>= 1;
   }

   BB.power( Baseband::P0 );

   delay( 3 * DOTMS );
}


void Morse::write( const char *x )
{
   while( *x ) {
      writeChar( *x );
      ++x;
   }
}


void Morse::write( const char *x, size_t len )
{
   for( size_t i = 0 ; i < len ; ++i )
      writeChar( x[i] );
}

/*EoF*/


#ifndef _QB50_SYSTEM_RING_HPP
#define _QB50_SYSTEM_RING_HPP

#include <cstddef>
#include <cstdint>


namespace qb50 {

   template<unsigned size>
   class Ring
   {

      public:

         Ring<size>()
         {
            static_assert(
               (( size > 2 ) && (( size & ( size - 1 )) == 0 )),
               "size must be a power of 2"
            );

            _x = new uint8_t[ size ];
            _r = 0;
            _w = 0;
         }


         ~Ring<size>()
         {
            delete[] _x;
         }


         /* remaining capacity of the buffer */

         inline unsigned avail( void ) __attribute__(( always_inline ))
         { return( size - ( _w - _r )); }


         /* occupancy of the buffer */

         inline unsigned count( void ) __attribute__(( always_inline ))
         { return( _w - _r ); }


         unsigned write( const uint8_t *src, unsigned cnt )
         {
            unsigned n = avail();
            if( cnt > n ) cnt = n;

            for( unsigned i = 0 ; i < cnt ; ++i ) {
               _x[ _w & ( size - 1 ) ] = src[ i ];
               _w++;
            }

            return cnt;
         }


         unsigned read( uint8_t *dst, unsigned cnt )
         {
            unsigned n = count();
            if( cnt > n ) cnt = n;

            for( unsigned i = 0 ; i < cnt ; ++i ) {
               dst[ i ] = _x[ _r & ( size - 1 ) ];
               _r++;
            }

            return cnt;
         }


      protected:

         uint8_t *_x;
         unsigned _r;
         unsigned _w;

   };

} /* qb50 */


#endif /*_QB50_SYSTEM_RING_HPP*/

/*EoF*/

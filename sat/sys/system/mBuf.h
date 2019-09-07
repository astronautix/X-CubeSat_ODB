
#ifndef _QB50_SYSTEM_MBUF_H
#define _QB50_SYSTEM_MBUF_H

#include <cstddef>
#include <cstdint>


namespace qb50 {

   template<typename T>
   struct mBuf
   {

      //  - - - - - - - - -  //
      //  S T R U C T O R S  //
      //  - - - - - - - - -  //

      mBuf( T *x, std::size_t len )
         : _x( x ), _off( 0 ), _len( len )
      { ; }

      mBuf() : _x( 0 ), _off( 0 ), _len( 0 )
      { ; }

      virtual ~mBuf()
      { ; }


      //  - - - - - - - - -  //
      //  O P E R A T O R S  //
      //  - - - - - - - - -  //

      T& operator* ()
      { return _x[ _off % _len ]; }

      T& operator[] ( const unsigned i )
      { return _x[ ( _off + i ) % _len ]; }

      const T& operator* () const
      { return _x[ _off % _len ]; }

      const T& operator[] ( const unsigned i ) const
      { return _x[ ( _off + i ) % _len ]; }

      /* ++obj (prefix) */
      mBuf& operator++ ()
      {
         ++_off;
         return *this;
      }

      /* obj++ (postfix) */
      mBuf operator++ ( int )
      {
         mBuf tmp( *this );
         ++_off;
         return tmp;
      }

      mBuf& operator+= ( const unsigned n )
      {
         _off += n;
         return *this;
      }

      /* --obj (prefix) */
      mBuf& operator-- ()
      {
         --_off;
         return *this;
      }

      /* obj-- (postfix) */
      mBuf operator-- ( int )
      {
         mBuf tmp( *this );
         --_off;
         return tmp;
      }

      mBuf& operator-= ( const unsigned n )
      {
         _off -= n;
         return *this;
      }


      //  - - - - - - -  //
      //  M E T H O D S  //
      //  - - - - - - -  //

      mBuf& reset( T *x, std::size_t len )
      {
         _x   = x;
         _off = 0;
         _len = len;

         return *this;
      }

      std::size_t avail( void )
      {
         size_t av = _len - _off;
         return ( av <= _len ) ? av : 0;
      }


      //  - - - - - - -  //
      //  M E M B E R S  //
      //  - - - - - - -  //

      T *_x;
      std::size_t _off;
      std::size_t _len;

   };

   template<typename T>
   inline mBuf<T> operator+ ( mBuf<T> lhs, const unsigned rhs )
   {
      lhs += rhs;
      return lhs;
   }

   template<typename T>
   inline mBuf<T> operator- ( mBuf<T> lhs, const unsigned rhs )
   {
      lhs -= rhs;
      return lhs;
   }

} /* qb50 */


#endif /*_QB50_SYSTEM_MBUF_H*/

/*EoF*/

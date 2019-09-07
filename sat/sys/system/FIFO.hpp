
#ifndef _QB50_SYSTEM_FIFO_HPP
#define _QB50_SYSTEM_FIFO_HPP


namespace qb50 {

template <class T, unsigned depth>
class FIFO
{

   public:

      FIFO<T,depth>()
      {
         static_assert(
            (( depth > 2 ) && (( depth & ( depth - 1 )) == 0 )),
            "depth must be a power of 2"
         );

         _x = new T[ depth ];
         _m = depth - 1;
         _r = 0;
         _w = 0;
      }


      virtual ~FIFO<T,depth>()
      { delete[] _x; }


      bool isEmpty( void )
      { return( _r == _w ); }


      bool isFull( void )
      { return((( _r & _m ) == ( _w & _m )) && ( _r != _w )); }


      unsigned count( void )
      { return( _w - _r ); }


      FIFO<T,depth>& push( T val )
      {
         if( isFull() ) {
            _triggerFullCondition();
         } else {
            _x[ _w & _m ] = val;
            _w++;
         }

         return *this;
      }


      T pull( void )
      {
         T rv = _x[ _r & _m ];

         if( isEmpty() ) {
            _triggerEmptyCondition();
         } else {
            _r++;
         }

         return rv;
      }


      FIFO<T,depth>& flush( void )
      {
         _r = _w = 0;
         return *this;
      }


      virtual void lock( void )
      { ; }


      virtual void unlock( void )
      { ; }


   protected:

      unsigned _m;  /* counter mask (= depth - 1) */
      unsigned _r;  /* read offset in units of T  */
      unsigned _w;  /* write offset in units of T */
      T*       _x;

      void _triggerFullCondition( void )
      { ; }

      void _triggerEmptyCondition( void )
      { ; }

};

} /* qb50 */

#endif /*_QB50_SYSTEM_FIFO_HPP*/

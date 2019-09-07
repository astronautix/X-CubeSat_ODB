
#ifndef _QB50_SYSTEM_E64_H
#define _QB50_SYSTEM_E64_H

#include <cstdlib>
#include <cstdint>


namespace qb50 {

   class E64
   {

      public:

         class Dumper
         {

            public:

               virtual ~Dumper() { ; }
               virtual void dump( const char *x, unsigned len, bool mf ) = 0;

         };

         E64( unsigned len = 384 );
         virtual ~E64();

         E64& push( const void *src, unsigned len );
         E64& dump( Dumper *dumper );
         E64& dump( void );


      private:

         char    *_x;
         unsigned _len;
         unsigned _off;

   };

} /* qb50 */


#endif /*_QB50_SYSTEM_E64_H*/

/*EoF*/

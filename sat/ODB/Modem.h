
#ifndef _QB50_ODB_MODEM_H
#define _QB50_ODB_MODEM_H

#include "Device.h"
#include "WodStore.h"


namespace qb50 {

   class Modem : public Device
   {

      public:

         Modem( const char *name ) : Device( name )
         { ; }

         virtual ~Modem()
         { ; }

         virtual Modem& init    ( void )                = 0;
         virtual Modem& enable  ( bool debug = false ) = 0;
         virtual Modem& disable ( bool debug = false ) = 0;

         virtual size_t send ( WodStore::WEnt *wod, const void *x,    int toms ) = 0;
         virtual size_t send ( const uint8_t  *x,   unsigned len,     int toms ) = 0;

   };

} /* qb50 */


#endif /*_QB50_ODB_MODEM_H*/

/*EoF*/

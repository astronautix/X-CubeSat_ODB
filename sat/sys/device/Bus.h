
#ifndef _QB50_SYSTEM_BUS_H
#define _QB50_SYSTEM_BUS_H

#include <stdint.h>


namespace qb50 {

   struct Bus
   {

      enum BusId {
         AHB1 = 0,
         AHB2 = 1,
         AHB3 = 2,
       //AHB4 = 3,
         APB1 = 4,
         APB2 = 5
      };

      Bus( const BusId busId, const char *busName ) : id( busId ), name( busName )
      { ; }

      const BusId id;
      const char *name;

   };

} /* qb50 */


#endif /*_QB50_SYSTEM_BUS_H*/

/*EoF*/

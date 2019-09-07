
#ifndef _QB50_SYS_DEVICE_COMPASS_H
#define _QB50_SYS_DEVICE_COMPASS_H

#include "common/Vec3D.h"


namespace qb50 {

   class Compass
   {

      public:

         Compass()
         { ; }

         virtual ~Compass()
         { ; }

         virtual Compass& omega( Vec3D& v ) = 0;

   };

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_COMPASS_H*/

/*EoF*/

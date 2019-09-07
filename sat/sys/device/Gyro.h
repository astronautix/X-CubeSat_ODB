
#ifndef _QB50_SYS_DEVICE_GYRO_H
#define _QB50_SYS_DEVICE_GYRO_H

#include "common/Vec3D.h"


namespace qb50 {

   class Gyro
   {

      public:

         Gyro()
         { ; }

         virtual ~Gyro()
         { ; }

         virtual Gyro& omega( Vec3D& v ) = 0;

   };

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_GYRO_H*/

/*EoF*/

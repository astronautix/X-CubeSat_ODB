
#ifndef _QB50_SYS_DEVICE_SENSOR_FILTER_H
#define _QB50_SYS_DEVICE_SENSOR_FILTER_H

#include "SensorSample.h"

#include <cstdint>


namespace qb50 {

   template<class T>
   class SensorFilter
   {

      public:

         SensorFilter()
         { ; }

         virtual ~SensorFilter()
         { ; }


      protected:

         virtual SensorFilter& _filter( SensorSample<T> *sample ) = 0;

   };

} /*qb50*/


#endif /*_QB50_SYS_DEVICE_SENSOR_FILTER_H*/

/*EoF*/

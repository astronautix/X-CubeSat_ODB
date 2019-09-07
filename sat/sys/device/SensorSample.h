
#ifndef _QB50_SYS_DEVICE_SENSOR_SAMPLE_H
#define _QB50_SYS_DEVICE_SENSOR_SAMPLE_H

#include <cstdint>


namespace qb50 {

   template<class T>
   struct SensorSample
   {
      float stdev;
      T     value;
   };

} /*qb50*/


#endif /*_QB50_SYS_DEVICE_SENSOR_SAMPLE_H*/

/*EoF*/

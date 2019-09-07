
#ifndef _QB50_SYS_DEVICE_ADC_H
#define _QB50_SYS_DEVICE_ADC_H

#include "Device.h"
#include "Sensor.h"


namespace qb50 {

   template<class T>
   class ADC
   {

      protected:

         struct IOReq;

      public:

         #include "ADC_Channel.h"

         ADC()
         { ; }

         virtual ~ADC()
         { ; }

         virtual ADC& read ( Channel& ch, SensorSample<T> *sample ) = 0;

   };

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_ADC_H*/

/*EoF*/

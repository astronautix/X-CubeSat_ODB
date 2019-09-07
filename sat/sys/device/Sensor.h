
#ifndef _QB50_SYS_DEVICE_SENSOR_H
#define _QB50_SYS_DEVICE_SENSOR_H

#include "SensorFilter.h"
#include "SensorSample.h"

#include <cstdint>


namespace qb50 {

   template <class T>
   class Sensor
   {

      public:

         Sensor();
         virtual ~Sensor();

         Sensor& addFilter ( SensorFilter<T> *filter );
         Sensor& read      ( SensorSample<T> *sample );


      protected:

         virtual Sensor& _readRaw( SensorSample<T> *sample ) = 0;

   };


   //  - - - - - - - - -  //
   //  S T R U C T O R S  //
   //  - - - - - - - - -  //

   template <class T>
   Sensor<T>::Sensor()
   { ; }


   template <class T>
   Sensor<T>::~Sensor()
   { ; }


   //  - - - - - - -  //
   //  M E T H O D S  //
   //  - - - - - - -  //

   template <class T>
   Sensor<T>& Sensor<T>::addFilter( SensorFilter<T> *filter )
   {
      (void)filter;
      return *this;
   }


   template <class T>
   Sensor<T>& Sensor<T>::read( SensorSample<T> *sample )
   {
      return
         _readRaw( sample );
   }

} /*qb50*/


#endif /*_QB50_SYS_DEVICE_SENSOR_H*/

/*EoF*/

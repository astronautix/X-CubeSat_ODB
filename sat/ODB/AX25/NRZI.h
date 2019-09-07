
#ifndef _QB50_ODB_NRZI_H
#define _QB50_ODB_NRZI_H

#include <FreeRTOS.h>


namespace qb50 {

   class NRZI
   {

      public:

         NRZI();
         ~NRZI();

         bool push( bool cur );

      private:

         bool _prv;

    };

}

#endif /*_QB50_ODB_NRZI_H*/

/*EoF*/

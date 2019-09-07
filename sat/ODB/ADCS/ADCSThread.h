
#ifndef _QB50_ODB_ADCS_THREAD_H
#define _QB50_ODB_ADCS_THREAD_H

#include "system/qb50.h"
#include "ADCS.h"


namespace qb50 {

   class ADCSThread : public Thread
   {

      public:

         ADCSThread();
         ~ADCSThread();

         void run( void );

         void onSuspend ( void );
         void onResume  ( void );

      private:

         uint8_t *_x;

   };

} /* qb50 */


#endif /*_QB50_ODB_ADCS_THREAD_H*/

/*EoF*/

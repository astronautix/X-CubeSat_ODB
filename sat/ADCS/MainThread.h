
#ifndef _QB50_ADCS_MAIN_THREAD_H
#define _QB50_ADCS_MAIN_THREAD_H

#include "system/qb50.h"


namespace qb50 {

   class MainThread : public Thread
   {

      public:

         MainThread();
         ~MainThread();

         void run       ( void );
         void onSuspend ( void );
         void onResume  ( void );

         void setCoils  ( int x, int y, int z );

      private:

         void initTimers   ( void );
         void initPWMChans ( void );

   };

} /* qb50 */


#endif /*_QB50_ADCS_MAIN_THREAD_H*/

/*EoF*/

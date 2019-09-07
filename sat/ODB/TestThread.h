
#ifndef _QB50_ODB_TEST_THREAD_H
#define _QB50_ODB_TEST_THREAD_H

#include "system/qb50.h"


namespace qb50 {

   class TestThread : public Thread
   {

      public:

         TestThread();
         ~TestThread();

         void run       ( void );
         void onSuspend ( void );
         void onResume  ( void );

      private:

         enum Mode { OFF = 0, ON = 1 };
         Mode _mode;

   };

} /* qb50 */


#endif /*_QB50_ODB_TEST_THREAD_H*/

/*EoF*/


#ifndef _QB50_WATCHDOG_THREAD_H
#define _QB50_WATCHDOG_THREAD_H

#include "system/qb50.h"


namespace qb50 {

   class WatchdogThread : public Thread
   {

      public:

         WatchdogThread();
         ~WatchdogThread();

         void run( void );

   };

} /* qb50 */


#endif /*_QB50_WATCHDOG_THREAD_H*/

/*EoF*/


#ifndef _QB50_CW_THREAD_H
#define _QB50_CW_THREAD_H

#include "system/qb50.h"


namespace qb50 {

   class CWThread : public Thread
   {

      public:

         CWThread();
         ~CWThread();

         void run       ( void );
         void onSuspend ( void );
         void onResume  ( void );

   };

} /* qb50 */


#endif /*_QB50_CW_THREAD_H*/

/*EoF*/


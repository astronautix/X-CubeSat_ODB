
#ifndef _QB50_ODB_INIT_THREAD_H
#define _QB50_ODB_INIT_THREAD_H

#include "system/qb50.h"


namespace qb50 {

   class InitThread : public Thread
   {

      public:

         InitThread();
         ~InitThread();

         void run( void );

   };

} /* qb50 */


#endif /*_QB50_ODB_INIT_THREAD_H*/

/*EoF*/


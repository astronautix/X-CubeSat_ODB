
#ifndef _QB50_SYSTEM_SIMPLE_THREAD_H
#define _QB50_SYSTEM_SIMPLE_THREAD_H

#include "system/Thread.h"


namespace qb50 {

   typedef void (*ThreadWorker)( Thread *self );

   class SimpleThread : public Thread
   {

      public:

         SimpleThread( const char *name, ThreadWorker worker );
         virtual ~SimpleThread();

         virtual void run( void );


      protected:

         ThreadWorker _worker;
   };

}


#endif /*_QB50_SYSTEM_SIMPLE_THREAD_H*/

/*EoF*/

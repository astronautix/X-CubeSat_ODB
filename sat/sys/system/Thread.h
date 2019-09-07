
#ifndef _QB50_SYSTEM_THREAD_H
#define _QB50_SYSTEM_THREAD_H

#include <cstddef>


namespace qb50 {

   class Thread
   {

      public:

         enum ThreadState {
            SUSPENDED  = 0,
            RUNNING    = 1,
            SUSPENDING = 2,
            RESUMING   = 3
         };

         Thread( const char *name, int prio, ThreadState state = RUNNING, unsigned stackDepth = 256 );
         ~Thread();

                 void suspend   ( void );
                 void resume    ( void );

         virtual void run       ( void ) = 0;

         virtual void onInit    ( void );
         virtual void onExit    ( void );
         virtual void onSuspend ( void );
         virtual void onResume  ( void );

         const char *name;
         void       *handle;
         unsigned    priority;
         unsigned    stackDepth;

      protected:

         ThreadState _thst;

         void     _wait     ( void );
         unsigned _stackHWM ( void );

   };

} /* qb50 */


#endif /*_QB50_SYSTEM_THREAD_H*/

/*EoF*/

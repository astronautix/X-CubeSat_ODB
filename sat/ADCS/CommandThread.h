
#ifndef _QB50_ADCS_COMMAND_THREAD_H
#define _QB50_ADCS_COMMAND_THREAD_H

#include "system/qb50.h"


namespace qb50 {

   class CommandThread : public Thread
   {

      public:

         CommandThread();
         ~CommandThread();

         void run       ( void );
         void onSuspend ( void );
         void onResume  ( void );

      private:

         uint8_t *_x;

   };

} /* qb50 */


#endif /*_QB50_ADCS_COMMAND_THREAD_H*/

/*EoF*/

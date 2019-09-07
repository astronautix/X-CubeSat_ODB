
#ifndef _QB50_TELEM_THREAD_H
#define _QB50_TELEM_THREAD_H

#include "system/qb50.h"
#include "Modem.h"


namespace qb50 {

   class TelemThread : public Thread
   {

      public:

         TelemThread();
         ~TelemThread();

         void     run       ( void );
         void     onSuspend ( void );
         void     onResume  ( void );


      private:

         Modem   *_modem;
         unsigned _tsMax;
         uint8_t *_x;

   };

} /* qb50 */


#endif /*_QB50_TELEM_THREAD_H*/

/*EoF*/

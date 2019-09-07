
#ifndef _QB50_CTCSS_THREAD_H
#define _QB50_CTCSS_THREAD_H

#include "system/qb50.h"
#include "CTCSS.h"


namespace qb50 {

   class CTCSSThread : public Thread
   {

      public:

         CTCSSThread();
         ~CTCSSThread();

         void onSuspend ( void );
         void onResume  ( void );
         void run       ( void );


      private:

         enum State { OFF = 0, ON = 1 };

         State    _state;
         unsigned _sec;

   };

} /* qb50 */


#endif /*_QB50_CTCSS_THREAD_H*/

/*EoF*/

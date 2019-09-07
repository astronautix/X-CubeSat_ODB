
#ifndef _QB50_SYSTEM_TIMER_H
#define _QB50_SYSTEM_TIMER_H

#include <FreeRTOS.h>
#include <task.h>


namespace qb50 {

   class Timer
   {

      public:

         Timer();
         virtual ~Timer();

         Timer& reset( void );
         Timer& every( unsigned ms     );
         Timer& until( TickType_t tick );

         TickType_t prev;

   };

} /* qb50 */


#endif /*_QB50_SYSTEM_TIMER_H*/

/*EoF*/

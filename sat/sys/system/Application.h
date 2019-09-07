
#ifndef _QB50_SYSTEM_APPLICATION_H
#define _QB50_SYSTEM_APPLICATION_H

//#include "system/Ring.h"
//#include "system/printf.h"
#include "system/SimpleThread.h"
#include "system/E64.h"


namespace qb50 {

   extern Thread  *createThread( const char *name, ThreadWorker worker );
   extern Thread  *registerThread( Thread *t );
   extern void     run( void );
   extern void     delay( unsigned ms );
   extern uint32_t ticks( void );
   extern void     hexdump( const void *addr, unsigned len );

} /* qb50 */


#endif /*_QB50_SYSTEM_APPLICATION_H*/

/*EoF*/

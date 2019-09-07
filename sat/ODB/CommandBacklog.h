
#ifndef _QB50_ODB_COMMAND_BACKLOG_H
#define _QB50_ODB_COMMAND_BACKLOG_H

#include <FreeRTOS.h>
#include <semphr.h>

#include "Form.h"


namespace qb50 {

   class CommandBacklog
   {

      public:

         CommandBacklog( size_t max );
         virtual ~CommandBacklog();

         bool push ( Form &f );
         bool pop  ( Form &f );


      private:

         size_t _max;
         size_t _cur;

         xSemaphoreHandle _lock;

         Form *_forms;

   };

   extern qb50::CommandBacklog CBL;

} /*qb50*/


#endif /*_QB50_ODB_COMMAND_BACKLOG_H*/

/*EoF*/

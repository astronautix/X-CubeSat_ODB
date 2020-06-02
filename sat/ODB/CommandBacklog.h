/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:22:17+02:00
 * @License: This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * @Copyright: Copyright (C) 2027 Olivier Piras  
 */
 
 
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

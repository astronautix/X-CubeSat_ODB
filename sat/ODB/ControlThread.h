/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:21:59+02:00
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
 
 
#ifndef _QB50_ODB_CONTROL_THREAD_H
#define _QB50_ODB_CONTROL_THREAD_H

#include "system/qb50.h"
#include "Config.h"
#include "Form.h"


namespace qb50 {

   class ControlThread : public Thread
   {

      public:

         ControlThread();
         ~ControlThread();

         void run( void );


      private:

         Thread         *_tv[ _QB50_NTHREADS ];
         static uint32_t _mt[ _QB50_NMODES   ];

         uint32_t _ctb; // current thread bitmask

         void _handleForm  ( Form  *fp );
         void _handleCForm ( CForm *fp );

         void _switchModes ( Config::mode_t target );

   };

} /* qb50 */


#endif /*_QB50_ODB_CONTROL_THREAD_H*/

/*EoF*/

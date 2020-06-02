/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:21:35+02:00
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
 
 
#ifndef _QB50_ODB_EVENT_H
#define _QB50_ODB_EVENT_H

#include "Form.h"


namespace qb50 {

   struct Event
   {

      static const char *events[];

      enum event_t {

         GENERIC     = 0x00,  /* generic event                     */

         AD_SUCCESS  = 0x01,  /* antenna deployment (success)      */
         AD_FAILURE  = 0x02,  /* antenna deployment (failure)      */
         VBAT_HIGH   = 0x03,  /* V_Bat crossed the upper threshold */
         VBAT_LOW    = 0x04,  /* V_Bat crossed the lower threshold */
         TPA_HIGH    = 0x05,  /* T_PA crossed the upper threshold  */
         TPA_LOW     = 0x06,  /* T_PA crossed the lower threshold  */
         FORM        = 0x07,  /* command from the ground           */
         WOD_EMPTY   = 0x08,  /* WOD store is empty                */

      };

      Event( event_t type, Form *form = (Form*)0 );
      ~Event();

      const char *name( void ) const
      { return events[ type ]; }

      event_t type;
      Form   *form;

   };

} /* qb50 */


#endif /*_QB50_ODB_EVENT_H*/

/*EoF*/

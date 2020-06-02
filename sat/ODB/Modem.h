/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:20:53+02:00
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
 
 
#ifndef _QB50_ODB_MODEM_H
#define _QB50_ODB_MODEM_H

#include "Device.h"
#include "WodStore.h"


namespace qb50 {

   class Modem : public Device
   {

      public:

         Modem( const char *name ) : Device( name )
         { ; }

         virtual ~Modem()
         { ; }

         virtual Modem& init    ( void )                = 0;
         virtual Modem& enable  ( bool debug = false ) = 0;
         virtual Modem& disable ( bool debug = false ) = 0;

         virtual size_t send ( WodStore::WEnt *wod, const void *x,    int toms ) = 0;
         virtual size_t send ( const uint8_t  *x,   unsigned len,     int toms ) = 0;

   };

} /* qb50 */


#endif /*_QB50_ODB_MODEM_H*/

/*EoF*/

/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:22:23+02:00
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
 
 
#ifndef _QB50_SYS_DEVICE_BASEBAND_H
#define _QB50_SYS_DEVICE_BASEBAND_H

#include "Device.h"
#include "GPIO.h"


namespace qb50 {

   class Baseband : public Device
   {

      public:

         enum Power { P0, P1, P2, P3, P4 };

         Baseband( const char *name, GPIO::Pin& enTXPin, GPIO::Pin& enPAPin );
         ~Baseband();

         Baseband& init    ( void );
         Baseband& enable  (          bool debug = false );
         Baseband& enable  ( Power p, bool debug = false );
         Baseband& disable (          bool debug = false );

         Baseband& power   ( Power p );
         uint8_t   pMask   ( void ) const;


      private:

         GPIO::Pin& _enTXPin;
         GPIO::Pin& _enPAPin;

         uint8_t _pMask;

   };

   extern qb50::Baseband BB;

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_BASEBAND_H*/

/*EoF*/

/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:21:54+02:00
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
 
 
#ifndef _QB50_CTCSS_H
#define _QB50_CTCSS_H

#include "devices.h"
#include "device/STM32/STM32_EXTI.h"


namespace qb50 {

   class CTCSS : public Device, public STM32_EXTIHandler
   {

      public:

         CTCSS( const char *name, STM32_GPIO::Pin& enPin, STM32_GPIO::Pin& dtPin );
         virtual ~CTCSS();

         CTCSS& init    ( void );
         CTCSS& enable  ( bool debug = false );
         CTCSS& disable ( bool debug = false );

         bool   wait    ( unsigned ms );
         bool   decTone ( void );

         void handle( STM32_EXTI::EXTIn );


      private:

         STM32_GPIO::Pin& _enPin;
         STM32_GPIO::Pin& _dtPin;

         xSemaphoreHandle _semDecTone;

   };

   extern qb50::CTCSS CTCS0;

} /* qb50 */


#endif /*_QB50_CTCSS_H*/

/*EoF*/

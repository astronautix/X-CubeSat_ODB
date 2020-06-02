/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:20:40+02:00
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
 
 
#ifndef _QB50_ODB_H
#define _QB50_ODB_H

#include "STM32/STM32_GPIO.h"


namespace qb50 {

   class ODB
   {

      public:

         ODB( const char *name,
              STM32_GPIO::Pin& selPin,
              STM32_GPIO::Pin& inhPin,
              STM32_GPIO::Pin& asPin,
              STM32_GPIO::Pin& adPin );
         virtual ~ODB();

         ODB& init( void );

         enum SatSel {
            FR01 = 0, /* X-CubeSat (Polytechnique)   */
            FR05 = 1  /* SpaceCube (Mines ParisTech) */
         };

         SatSel id        ( void );
         bool   isInhibit ( void );

         enum AntState {
            RETRACTED = 0,
            DEPLOYED  = 1
         };

         AntState aState   ( void );
         AntState aDeploy  ( void );

         unsigned maI[4];
         unsigned mvV[4];
         unsigned maIRx;
         unsigned maITx;
         unsigned mvBat;
         float    dcBat;
         float    dcPA;


      private:

         const char       *_name;
         STM32_GPIO::Pin&  _selPin; /* satellite selection (in) */
         STM32_GPIO::Pin&  _inhPin; /* ihnibit pin         (in) */
         STM32_GPIO::Pin&  _asPin;  /* antenna status      (in) */
         STM32_GPIO::Pin&  _adPin;  /* antenna deploy     (out) */

   };

} /* qb50 */


#endif /*_QB50_ODB_H*/

/*EoF*/

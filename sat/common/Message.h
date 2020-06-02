/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:19:36+02:00
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
 

#ifndef _QB50_COMMON_MESSAGE_H
#define _QB50_COMMON_MESSAGE_H

#include "Vec3D.h"


namespace qb50{

   enum CmdID {
      PING         = 'P',
      MODE_ATT_DET = 'D',
      MODE_ATT_CON = 'C', //A Confirmer
      ASK_ATT      = 'Q',
      ASK_MAG_RAW  = 'M',
      ASK_GYR_RAW  = 'G',
      ASK_SUN_RAW  = 'J',
      ASK_SUN_TRUE = 'F',
      SET_TIME     = 'K',
      READ_TIME    = 'H',
      STOP         = 'S'

      //to Add and confirm with Gerard

      /* ASK_MAG_TRUE  True Magnetic field
         ASK_GYR_TRUE  True Gyrometer Readings */
   };


   struct ADCSMeas {

      int8_t gxyz[3];
      int8_t mxyz[3];

      uint8_t xf;   // X (front) - GS1_1
      uint8_t xr;   // X (rear)  - GS4_1

      uint8_t yf;   // Y (front) - GS3_2
      uint8_t yr;   // Y (rear)  - GS2_1

      uint8_t zf;   // Z (front) - GS5
      uint8_t zr;   // Z (rear)  - GS3_1

   } __attribute__(( packed ));


   struct ADCSCtrl {

      long n;
      long d;
      long x;
      long y;
      long z;

   } __attribute__(( packed ));


   struct ADCSBeat {

      ADCSCtrl ctrl;
      ADCSMeas meas;

   } __attribute__(( packed ));


} /* qb50 */


#endif

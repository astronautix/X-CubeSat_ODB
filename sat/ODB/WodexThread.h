/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:20:19+02:00
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
 
 
#ifndef _QB50_WODEX_THREAD_H
#define _QB50_WODEX_THREAD_H

#include "Modem.h"
#include "system/qb50.h"


namespace qb50 {

   class WodexThread : public Thread
   {

      public:

         enum {
            SAMPLE_X0,
            SAMPLE_X1,
            SAMPLE_X2,
            SAMPLE_V4,
            SAMPLE_I4,
            SAMPLE_T4,
            SAMPLE_V1,
            SAMPLE_T1,

            SAMPLE_I1,
            SAMPLE_T_Bat,
            SAMPLE_V_Bat,
            SAMPLE_V2,
            SAMPLE_T2,
            SAMPLE_I2,
            SAMPLE_V3,
            SAMPLE_T3,

            SAMPLE_I3,
            SAMPLE_I_Surt,
            SAMPLE_I_ADCS,
            SAMPLE_T_ARM,
            SAMPLE_I_RX,
            SAMPLE_RSSI,
            SAMPLE_I_TX,
            SAMPLE_P_TX,

            SAMPLE_P_PA,
            SAMPLE_T_PA,
            SAMPLE_I_GPS,
            SAMPLE_I_3V3_FIPEX,
            SAMPLE_V_3V3_FIPEX,
            SAMPLE_I_5V_FIPEX,
            SAMPLE_V_5V_FIPEX,
            SAMPLE_SU_TH_G0,
         };

         WodexThread();
         ~WodexThread();

         void run       ( void );
         void onSuspend ( void );
         void onResume  ( void );


      private:

         MAX111x::Sample *_raw;
         uint8_t         *_raw8;

         enum Mode { LOW = 0, HIGH = 1 };

         Mode   _modeBat;
         Mode   _modePA;

         Timer  _timer;

         Modem *_modem;

         void _updateSamples ( void );
         void _sendBacklog   ( Modem *modem );

   };

} /* qb50 */


#endif /*_QB50_WODEX_THREAD_H*/

/*EoF*/

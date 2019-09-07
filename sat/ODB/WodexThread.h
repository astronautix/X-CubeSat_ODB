
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

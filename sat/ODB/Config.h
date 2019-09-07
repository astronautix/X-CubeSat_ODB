
#ifndef _QB50_ODB_CONFIG_H
#define _QB50_ODB_CONFIG_H

#include "TLE.h"
#include <cstdint>

#include <FreeRTOS.h>
#include <semphr.h>


#define _QB50_NMODES    16
#define _QB50_NTHREADS  8
#define _QB50_NPARAMS   64
#define _QB50_PVAL_MASK 0xffff


namespace qb50 {

   class Config
   {

      public:

         //  - - - - - - - - - -  //
         //  P A R A M E T E R S  //
         //  - - - - - - - - - -  //

         enum pid_t {
            PARAM_NONE              = 0,
            PARAM_CW_CYCLE_TX       = 1,
            PARAM_WODEX_CYCLE_TX    = 2,
            PARAM_FIPEX_SCRIPT_N    = 3,
            PARAM_MODEM             = 4,
            PARAM_ADCS_CYCLE_MEAS   = 5,
            PARAM_TELEM_TIMER       = 6,
            PARAM_VBAT_LOW          = 7,
            PARAM_ADCS_CYCLE_DTMB   = 8,
            PARAM_FM_WODEX_CYCLE_TX = 9,
            PARAM_ADCS_CYCLE_CTRL   = 10,
            PARAM_FM_CYCLE_ON       = 11,
            PARAM_VBAT_HIGH         = 12,
            PARAM_TELEM_FORMAT      = 13,
            PARAM_PA_TEMP_HIGH      = 14,
            PARAM_PA_TEMP_LOW       = 15,

            PARAM_CW_POWER          = 32,
            PARAM_WODEX_POWER       = 33,
            PARAM_FM_POWER          = 34,
            PARAM_ADCS_PWM_D        = 35,
            PARAM_ADCS_PWM_X        = 36,
            PARAM_ADCS_PWM_Y        = 37,
            PARAM_ADCS_PWM_Z        = 38,
            PARAM_ADCS_PWM_N        = 39
         };

         typedef int16_t pval_t;

         struct definition {
            const pid_t  pid;  /* parameter ID  */
            const pval_t min;  /* min value     */
            const pval_t max;  /* max value     */
            const pval_t def;  /* default value */
         };

         static const definition defs[ _QB50_NPARAMS ];

         pid_t    chkParam ( long p, long v );
         pval_t   getParam ( pid_t pid );
         pval_t   setParam ( pid_t pid, pval_t pval );

         //  - - - - -  //
         //  M O D E S  //
         //  - - - - -  //

         enum Mode {
            INIT   = 0x00,
            CW     = 0x01,
            WODEX  = 0x02,
            AMEAS  = 0x03,
            ACTRL  = 0x04,
            FIPEX  = 0x05,
            TELEM  = 0x06,
            FM     = 0x07,
            MODE8  = 0x08,
            MODE9  = 0x09,
            MODE10 = 0x0a,
            MODE11 = 0x0b,
            MODE12 = 0x0c,
            MODE13 = 0x0d,
            POWER  = 0x0e,
            STDBY  = 0x0f
         };

         static const char *modes[ _QB50_NMODES ];

         typedef uint32_t mode_t;

         mode_t mode ( void );
         mode_t mode ( mode_t mode );

         //  - - - - -  //
         //  W O D E X  //
         //  - - - - -  //

         uint32_t wHead    ( unsigned idx );
         uint32_t wHead    ( unsigned idx, uint32_t addr );

         uint32_t wTail    ( unsigned idx );
         uint32_t wTail    ( unsigned idx, uint32_t addr );

         uint32_t wNext    ( unsigned idx );
         uint32_t wNext    ( unsigned idx, uint32_t addr );

         //  - - - - - - -  //
         //  M E T H O D S  //
         //  - - - - - - -  //

         Config();
         virtual ~Config();

         Config&  lock     ( void );
         Config&  unlock   ( void );

         void     clear    ( void );
         uint16_t reset    ( void );
         uint16_t nrst     ( void );


      private:

         xSemaphoreHandle _lock;

         struct _Store
         {
            pval_t   pv[ _QB50_NPARAMS ];  /* parameters                        */
            uint16_t nReset;               /* reset counter                     */
            mode_t   mode;                 /* current mode                      */
            TLE      tle;                  /* latest TLE known to the satellite */
            uint32_t wHead[4];             /* head of the WOD chain (first)     */
            uint32_t wTail[4];             /* tail of the WOD chain (last)      */
         };

   };

   extern qb50::Config CONF;

} /* qb50 */


#endif /*_QB50_ODB_CONFIG_H*/

/*EoF*/

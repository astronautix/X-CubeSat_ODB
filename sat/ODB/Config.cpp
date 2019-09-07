
#include "Config.h"
#include "Syslog.h"
#include "WodStore.h"

#include <safe_stm32f4xx.h>

using namespace qb50;


Config qb50::CONF; // global config object

#define INF 0xffff

const Config::definition Config::defs[ _QB50_NPARAMS ] = {

   /* params[ 0 .. 15 ]       min  max  def                               min  max  def */
   { PARAM_NONE,               0,   0,   0 },  { PARAM_CW_CYCLE_TX,        0,  10,   4 },
   { PARAM_WODEX_CYCLE_TX,     1,   3,   3 },  { PARAM_FIPEX_SCRIPT_N,     0,   7,   1 },
   { PARAM_MODEM,              1,   2,   1 },  { PARAM_ADCS_CYCLE_MEAS,    1,   4,   4 },
   { PARAM_TELEM_TIMER,        0,  15,   5 },  { PARAM_VBAT_LOW,           1,   7,   7 },
   { PARAM_ADCS_CYCLE_DTMB,    0,  24,   0 },  { PARAM_FM_WODEX_CYCLE_TX,  1,   4,   2 },
   { PARAM_ADCS_CYCLE_CTRL,    0,  24,   0 },  { PARAM_FM_CYCLE_ON,        1,  96,  16 },
   { PARAM_VBAT_HIGH,          1,   5,   1 },  { PARAM_TELEM_FORMAT,       0,   1,   0 },
   { PARAM_PA_TEMP_HIGH,       0,   7,   1 },  { PARAM_PA_TEMP_LOW,        0,   5,   4 },

   /* params[ 16 .. 31 ]      min  max  def                               min  max  def */
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },

   /* params[ 32 .. 47 ]      min  max  def                               min  max  def */
   { PARAM_CW_POWER,           1,   4,   1 },  { PARAM_WODEX_POWER,        1,   4,   1 },
   { PARAM_FM_POWER,           1,   4,   1 },  { PARAM_ADCS_PWM_D,         0, 60*45, 1 },
   { PARAM_ADCS_PWM_X,      -100, 100,   0 },  { PARAM_ADCS_PWM_Y,      -100, 100,   0 },
   { PARAM_ADCS_PWM_Z,      -100, 100,   0 },  { PARAM_ADCS_PWM_N,         0,  -1,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },

   /* params[ 48 .. 63 ]      min  max  def                               min  max  def */
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 },
   { PARAM_NONE,               0,   0,   0 },  { PARAM_NONE,               0,   0,   0 }

};


const char *Config::modes[ _QB50_NMODES ] = {
   "INIT",         "CW",           "WODEX",        "AMEAS",
   "ACTRL",        "FIPEX",        "TELEM",        "FM",
   "MODE8",        "MODE9",        "MODE10",       "MODE11",
   "MODE12",       "MODE13",       "POWER",        "STDBY"
};


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

Config::Config()
{
   _lock = xSemaphoreCreateMutex();
}


Config::~Config()
{
   vSemaphoreDelete( _lock );
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //


Config& Config::lock( void )
{
   (void)xSemaphoreTake( _lock, portMAX_DELAY );
   return *this;
}


Config& Config::unlock( void )
{
   (void)xSemaphoreGive( _lock );
   return *this;
}


uint16_t Config::reset( void )
{
   _Store *st = (_Store*)BKPSRAM_BASE;
   return ++st->nReset;
}


uint16_t Config::nrst( void )
{
   _Store *st = (_Store*)BKPSRAM_BASE;
   return st->nReset;
}


void Config::clear( void )
{
   lock();

   _Store *st = (_Store*)BKPSRAM_BASE;

   for( int i = 0 ; i < _QB50_NPARAMS ; ++i )
      st->pv[ i ] = defs[ i ].def;

   st->nReset = 0;
   st->mode   = Config::INIT;

   unlock();
}


//  - - - - - - - - - -  //
//  P A R A M E T E R S  //
//  - - - - - - - - - -  //

Config::pid_t Config::chkParam( long p, long v )
{
   const Config::definition *def;

   if(( p < 0 ) || ( p >= _QB50_NPARAMS ))
      return PARAM_NONE;

   def = &defs[ p ];

   if( def->pid == PARAM_NONE )
      return PARAM_NONE;

   if(( def->max > def->min ) && (( v < def->min ) || ( v > def->max )))
      return PARAM_NONE;

   return def->pid;
}


Config::pval_t Config::setParam( Config::pid_t pid, Config::pval_t pval )
{
   lock();

   _Store *st = (_Store*)BKPSRAM_BASE;

   Config::pval_t old = st->pv[ pid ];

   if( chkParam( pid, pval ) == Config::PARAM_NONE ) {

      kprintf( RED( "- P%d: parameter out of bounds" ) "\r\n", pid );

   } else {

      st->pv[ pid ] = pval;

      if( pval == old ) {
         kprintf( "+ P%d,%d (unchanged)\r\n", pid, pval );
      } else {
         kprintf( "+ P%d,%d (was: %d)\r\n", pid, pval, old );
      }
   }

   unlock();

   return old;
}


Config::pval_t Config::getParam( Config::pid_t pid )
{
   _Store *st = (_Store*)BKPSRAM_BASE;
   return st->pv[ pid ];
}


//  - - - - -  //
//  W O D E X  //
//  - - - - -  //

uint32_t Config::wHead( unsigned idx )
{
   _Store *st = (_Store*)BKPSRAM_BASE;
   return st->wHead[ idx ];
}


uint32_t Config::wHead( unsigned idx, uint32_t addr )
{
   lock();

   _Store *st = (_Store*)BKPSRAM_BASE;

   uint32_t old = st->wHead[ idx ];
   st->wHead[ idx ] = addr;

   unlock();

   return old;
}


uint32_t Config::wTail( unsigned idx )
{
   _Store *st = (_Store*)BKPSRAM_BASE;
   return st->wTail[ idx ];
}


uint32_t Config::wTail( unsigned idx, uint32_t addr )
{
   lock();

   _Store *st = (_Store*)BKPSRAM_BASE;

   uint32_t old = st->wTail[ idx ];
   st->wTail[ idx ] = addr;

   unlock();

   return old;
}


//  - - - - -  //
//  M O D E S  //
//  - - - - -  //

Config::mode_t Config::mode( void )
{
   _Store *st = (_Store*)BKPSRAM_BASE;
   return st->mode;
}


Config::mode_t Config::mode( Config::mode_t mode )
{
   lock();

   _Store *st = (_Store*)BKPSRAM_BASE;

   Config::mode_t old = st->mode;
   st->mode = mode;

   unlock();

   return old;
}

/*EoF*/

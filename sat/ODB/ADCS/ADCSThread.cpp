
#include "devices.h"
#include "common/Message.h"
#include "ADCSThread.h"
#include "WodStore.h"
#include "Config.h"


using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

ADCSThread::ADCSThread()
   : Thread( "ADCS Thread", 1, SUSPENDED, 384 )
{
   _x = new uint8_t[ 256 ];
}


ADCSThread::~ADCSThread()
{
   delete[] _x;
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

void ADCSThread::run( void )
{
   unsigned i, n;
   Config::pval_t dt;
   WodStore::WEnt wod;
   ADCSCtrl m;

   ADCSBeat *bp;
   ADCSCtrl *cp;
   ADCSMeas *mp;

   for( i = 0 ; i < 10 ; ++i ) {
      n = ADCS0.read( _x, 256, 500 );

      if( n == 0 )
         break;

      kprintf( YELLOW( "%s: flushing %lu bytes from ADCS" ) "\r\n", name, n );
   }

   if( n > 0 ) {
      kprintf( RED( "%s: garbage received from ADCS (tried %lu times)" ) "\r\n", name, i );
   }

   for( i = 0 ;; ++i ) {
      _wait();

      n = ADCS0.read( _x, sizeof( ADCSBeat ), 5000 );
      if( n == 0 ) {
         (void)kprintf( "%s: waiting...\r\n", name );
         continue;
      }

      bp = (ADCSBeat*)_x;
      cp = &bp->ctrl;
      mp = &bp->meas;

      m.n = CONF.getParam( Config::PARAM_ADCS_PWM_N );
      m.d = CONF.getParam( Config::PARAM_ADCS_PWM_D );
      m.x = CONF.getParam( Config::PARAM_ADCS_PWM_X );
      m.y = CONF.getParam( Config::PARAM_ADCS_PWM_Y );
      m.z = CONF.getParam( Config::PARAM_ADCS_PWM_Z );

      if( m.n > cp->n ) {
         kprintf( YELLOW( "%s: configuring ADCS" ) "\r\n", name );
         (void)ADCS0.write( (const uint8_t*)&m, sizeof( ADCSCtrl ), 1000 );
      }

      (void)ADCS0.setMeas( mp );

      dt = 15 * CONF.getParam( Config::PARAM_ADCS_CYCLE_MEAS );

      if(( i % dt ) == 0 ) {
         kprintf( YELLOW( "%s: ADCS MEASURE" ) "\r\n", name );
         wod.type = WodStore::ADCS;
         wod.len  = sizeof( ADCSMeas );
         (void)WOD0.write( &wod, mp );
      }

/*
      (void)kprintf( "GOT CTRL: [ %d %d %d %d ]\r\n", cp->d, cp->x, cp->y, cp->z );
      (void)kprintf( "GOT GYR0: [ %d %d %d ]\r\n", mp->gxyz[0], mp->gxyz[1], mp->gxyz[2] );
      (void)kprintf( "GOT MAG0: [ %d %d %d ]\r\n", mp->mxyz[0], mp->mxyz[1], mp->mxyz[2] );
      (void)kprintf( "GOT SUNV: [ %u %u %u %u %u %u ]\r\n", mp->xf, mp->xr, mp->yf, mp->yr, mp->zf, mp->zr );
*/
   }
}


void ADCSThread::onSuspend()
{
   ADCS0.disable();
   Thread::onSuspend();
}


void ADCSThread::onResume()
{
   Thread::onResume();
   ADCS0.enable();
}


/*EoF*/

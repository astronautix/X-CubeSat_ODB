
#include "devices.h"

#include "ControlThread.h"
#include "CommandThread.h"
#include "InitThread.h"
#include "CWThread.h"
#include "WodexThread.h"
#include "FiPEX/FipexThread.h"
#include "TelemThread.h"
#include "CTCSSThread.h"
#include "ADCS/ADCSThread.h"
#include "TestThread.h"

#include "Event.h"
#include "WodStore.h"

using namespace qb50;

QueueHandle_t evQueue;


uint32_t ControlThread::_mt[ _QB50_NMODES ] = {
  /* +----------- TestThread
     |+---------- ADCSThread
     ||+--------- CTCSSThread
     |||+-------- TelemThread
     ||||+------- CWThread
     |||||+------ FipexThread
     ||||||+----- WodexThread
     |||||||+---- InitThread
     |||||||| */
   0b00000001, /* mode INIT      */
   0b00001010, /* mode CW        */
   0b00000010, /* mode WODEX     */
   0b01000010, /* mode AMEAS     */
   0b01000010, /* mode ACTRL     */
   0b01000110, /* mode FIPEX     */
   0b00010010, /* mode TELEM     */
   0b00100010, /* mode FM        */
   0b00000010, /* mode (0x08)    */
   0b00000010, /* mode (0x09)    */
   0b00000010, /* mode (0x0a)    */
   0b00000010, /* mode (0x0b)    */
   0b00000010, /* mode (0x0c)    */
   0b00000010, /* mode (0x0d)    */
   0b00000010, /* mode POWER     */
   0b00000000  /* mode STDBY     */
};


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

ControlThread::ControlThread()
   : Thread( "Event Manager", 2, RUNNING, 384 )
{
   evQueue = xQueueCreate( 16, sizeof( Event* ));

   _tv[ 0 ] = new InitThread();
   _tv[ 1 ] = new WodexThread();
   _tv[ 2 ] = new FipexThread();
   _tv[ 3 ] = new CWThread();
   _tv[ 4 ] = new TelemThread();
   _tv[ 5 ] = new CTCSSThread();
   _tv[ 6 ] = new ADCSThread();
   _tv[ 7 ] = new TestThread();

   _ctb = 0x00;
}


ControlThread::~ControlThread()
{
   vQueueDelete( evQueue );
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

void ControlThread::run( void )
{
   Event *ev;

      SAT.init();

      WDG.enable();
    GPIOA.enable();
    GPIOB.enable();
    GPIOC.enable();
    UART6.enable();
   SYSLOG.enable();
     RTC0.enable();
      BKP.enable();
     WOD0.enable();
     WOD1.enable();

   /*
    * Hardware reset ?
    * -> load the default configuration
    */

   if( RCC.isPwrOn() ) {
      kprintf( "POWER ON - loading default configuration\r\n" );
      CONF.clear();
      CONF.reset();
      WOD0.clear();
      WOD1.clear();
   }

   /* increment the reset counter */

   kprintf( "Reset count: %d\r\n", CONF.nrst() );

   /* display satellite ID */

   switch( SAT.id() ) {

      case ODB::FR01:
         kprintf( "ON01FR (X-CubeSat - Ecole Polytechnique)\r\n" );
         break;

      case ODB::FR05:
         kprintf( "ON05FR (SpaceCube - Mines ParisTech)\r\n" );
         break;

      default:
         ;
   }

   /* register worker threads */

   (void)registerThread( new CommandThread() );

   for( int i = 0 ; i < _QB50_NTHREADS ; ++i ) {
      (void)registerThread( _tv[i] );
   }

   /* get the last known mode */

   Config::mode_t mode = CONF.mode();

   if( mode != Config::STDBY ) {
      if( SAT.aState() != ODB::DEPLOYED ) {
         mode = Config::INIT;
      } else {
         kprintf( "Antenna already deployed, resuming WODEX mode\r\n" );
         mode = Config::WODEX;
      }
   }

   _switchModes( mode );

   /* event loop */

   for( ;; ) {
      xQueueReceive( evQueue, &ev, portMAX_DELAY );
      if( !ev ) continue;

      kprintf( "\033[1mEVENT TYPE #%d - %s\033[0m\r\n", ev->type, ev->name() );

      switch( ev->type ) {

         case Event::AD_SUCCESS:
         case Event::AD_FAILURE:

            if( mode == Config::INIT ) {
               _switchModes( Config::WODEX );
            }

            break;

         case Event::VBAT_HIGH:

            if( mode == Config::POWER ) {
               _switchModes( Config::WODEX );
            }

            break;

         case Event::VBAT_LOW:

            if( mode != Config::POWER ) {
               _switchModes( Config::POWER );
            }

            break;

         case Event::FORM:

            _handleForm( ev->form );
            break;

         case Event::WOD_EMPTY:

            if(( mode == Config::TELEM ) || ( mode == Config::TELEM )) {
               _switchModes( Config::WODEX );
            }

            break;

         default:
            break;

      }

      mode = CONF.mode();

      delete ev;
   }
}


void ControlThread::_handleForm( Form *fp )
{
   switch( fp->type ) {

      case Form::FORM_TYPE_C:
         _handleCForm( &fp->C );
         break;

      default:
         break;
   }
}


void ControlThread::_handleCForm( CForm *fp )
{
   Config::mode_t mode = CONF.mode();
   Config::pval_t n;

   switch( fp->argv[0] ) {

      /* C1 - passage en mode CW */

      case 1:

         if( fp->argc > 1 ) {
            (void)CONF.setParam( Config::PARAM_CW_CYCLE_TX, fp->argv[1] );
            if( fp->argc > 2 ) {
               (void)CONF.setParam( Config::PARAM_CW_POWER, fp->argv[2] );
            }
         }

         if( mode != Config::CW ) {
            _switchModes( Config::CW );
         }

         break;

      /* C2 - passage en mode WODEX (1200) */

      case 2:

         (void)CONF.setParam( Config::PARAM_MODEM, 1 );

         if( fp->argc > 1 ) {
            (void)CONF.setParam( Config::PARAM_WODEX_CYCLE_TX, fp->argv[1] );
            if( fp->argc > 2 ) {
               (void)CONF.setParam( Config::PARAM_WODEX_POWER, fp->argv[2] );
            }
         }

         if( mode != Config::WODEX ) {
            _switchModes( Config::WODEX );
         }

         break;

      /* C3 - passage en mode WODEX (9600) */

      case 3:

         (void)CONF.setParam( Config::PARAM_MODEM, 2 );

         if( fp->argc > 1 ) {
            (void)CONF.setParam( Config::PARAM_WODEX_CYCLE_TX, fp->argv[1] );
            if( fp->argc > 2 ) {
               (void)CONF.setParam( Config::PARAM_WODEX_POWER, fp->argv[2] );
            }
         }

         if( mode != Config::WODEX ) {
            _switchModes( Config::WODEX );
         }

         break;

      /* C4 - passage en mode mesure d'attitude */

      case 4:

         if( fp->argc > 1 ) {
            (void)CONF.setParam( Config::PARAM_ADCS_CYCLE_MEAS, fp->argv[1] );
         }

         (void)CONF.setParam( Config::PARAM_ADCS_PWM_D, 0 );
         (void)CONF.setParam( Config::PARAM_ADCS_PWM_X, 0 );
         (void)CONF.setParam( Config::PARAM_ADCS_PWM_Y, 0 );
         (void)CONF.setParam( Config::PARAM_ADCS_PWM_Z, 0 );

         n = CONF.getParam( Config::PARAM_ADCS_PWM_N );
         (void)CONF.setParam( Config::PARAM_ADCS_PWM_N, n+1 );

         if( mode != Config::AMEAS ) {
            _switchModes( Config::AMEAS );
         }

         break;

      /* C5 - passage en mode contrôle d'attitude */

      case 5:

         if( fp->argc > 1 ) {
            (void)CONF.setParam( Config::PARAM_ADCS_PWM_D, 60 * fp->argv[1] );
            if( fp->argc > 2 ) {
               (void)CONF.setParam( Config::PARAM_ADCS_PWM_X, fp->argv[2] );
               if( fp->argc > 3 ) {
                  (void)CONF.setParam( Config::PARAM_ADCS_PWM_Y, fp->argv[3] );
                  if( fp->argc > 4 ) {
                     (void)CONF.setParam( Config::PARAM_ADCS_PWM_Z, fp->argv[4] );
                  }
               }
            }
         }

         n = CONF.getParam( Config::PARAM_ADCS_PWM_N );
         (void)CONF.setParam( Config::PARAM_ADCS_PWM_N, n+1 );

         if( mode != Config::ACTRL ) {
            _switchModes( Config::ACTRL );
         }

         break;

      /* C6 - passage en mode FIPEX */

      case 6:

         if( fp->argc > 1 ) {
            (void)CONF.setParam( Config::PARAM_FIPEX_SCRIPT_N, fp->argv[1] );
         }

         if( mode != Config::FIPEX ) {
            _switchModes( Config::FIPEX );
         }

         break;

      /* C7 - passage en mode relais FM */

      case 7:

         if( fp->argc > 1 ) {
            (void)CONF.setParam( Config::PARAM_FM_CYCLE_ON, fp->argv[1] );
            if( fp->argc > 2 ) {
               (void)CONF.setParam( Config::PARAM_FM_POWER, fp->argv[2] );
            }
         }

         if( mode != Config::FM ) {
            _switchModes( Config::FM );
         }

         break;

      /* C8 - passage en mode standby */

      case 8:

         if( mode != Config::STDBY ) {
            _switchModes( Config::STDBY );
         }

         break;

      /* C9 - passage en mode télémétrie PSK9600 */

      case 9:

         (void)CONF.setParam( Config::PARAM_MODEM, 2 );

         if( fp->argc > 1 ) {
            (void)CONF.setParam( Config::PARAM_WODEX_POWER, fp->argv[1] );
            if( fp->argc > 2 ) {
               (void)CONF.setParam( Config::PARAM_TELEM_TIMER, fp->argv[2] );
               if( fp->argc > 3 ) {
                  (void)CONF.setParam( Config::PARAM_TELEM_FORMAT, fp->argv[3] );
               }
            }
         }

         if( mode != Config::TELEM ) {
            _switchModes( Config::TELEM );
         }

         break;

      /* C10 - passage en mode télémétrie AFSK1200 */

      case 10:

         (void)CONF.setParam( Config::PARAM_MODEM, 1 );

         if( fp->argc > 1 ) {
            (void)CONF.setParam( Config::PARAM_WODEX_POWER, fp->argv[1] );
            if( fp->argc > 2 ) {
               (void)CONF.setParam( Config::PARAM_TELEM_TIMER, fp->argv[2] );
               if( fp->argc > 3 ) {
                  (void)CONF.setParam( Config::PARAM_TELEM_FORMAT, fp->argv[3] );
               }
            }
         }

         if( mode != Config::TELEM ) {
            _switchModes( Config::TELEM );
         }

         break;

      /* C11 - ping */

      case 11:
         break;

      /* C12 - arrêt télémétries */

      case 12:

         if( mode != Config::TELEM ) {
            _switchModes( Config::WODEX );
         }

         break;

      /* C13 - effacement données FIPEX (SU) */

      case 13:
         break;

      /* C14 - mode test mesure d'attitude */

      case 14:
         break;

      /* C15 - mode test contrôle d'attitude */

      case 15:
         break;

      default:
         break;

      /* C1001 - formattage de la flash */

      case 1001:

       //if( SAT.isInhibit() && ( mode == Config::STDBY )) {
         if( mode == Config::STDBY ) {
            WOD0.clear();
            WOD1.clear();
         }

         break;

   }
}


void ControlThread::_switchModes( Config::mode_t target )
{
   uint32_t delta = _ctb ^ _mt[ target & 0x0f ];
   uint32_t tmp = 0x01;

   for( unsigned i = 0 ; i < _QB50_NTHREADS ; ++i ) {
      if(( delta & tmp ) != 0 ) {
         if(( _ctb & tmp ) != 0 ) {
            _tv[ i ]->suspend();
         } else {
            _tv[ i ]->resume();
         }
         delay( 10 );
      }
      tmp <<= 1;
   }

   _ctb = _mt[ target & 0x0f ];
   CONF.mode( target & 0x0f );

   kprintf( "\033[7m-------- [ %s ] --------\033[0m\r\n", Config::modes[ target & 0x0f ] );
}

/*EoF*/

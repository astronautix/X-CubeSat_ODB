
#include "FipexThread.h"
#include "Fipex.h"
#include "WodStore.h"
#include "Config.h"
#include "AX25Modem.h"
#include "ADCS/ADCS.h"
#include "devices.h"

#define VKI_EPOCH 946684800

using namespace qb50;


extern QueueHandle_t evQueue;


static uint8_t cmd_rsp[ 4 ] = { 0x7e, 0x10, 0x00, 0x10 };
static uint8_t cmd_hk [ 4 ] = { 0x7e, 0x20, 0x00, 0x20 };
static uint8_t cmd_dp [ 4 ] = { 0x7e, 0x21, 0x00, 0x21 };


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

FipexThread::FipexThread()
   : Thread( "Fipex", 1, SUSPENDED, 1024 )
{
   _st  = ST_INIT;
   _rx  = new uint8_t[ 256 ];
   _mp  = new ADCSMeas;
}


FipexThread::~FipexThread()
{
   delete   _mp;
   delete[] _rx;
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

#if 0
static uint8_t cmd_ping[]    = { 0x7e, 0x00, 0x00, 0x00 };
static uint8_t cmd_hk[]      = { 0x7e, 0x20, 0x00, 0x20 };
static uint8_t cmd_sp_mt10[] = { 0x7e, 0x11, 0x03, 0x02, 0x0a, 0x00, 0x1a };
static uint8_t cmd_sm[]      = { 0x7e, 0x0c, 0x00, 0x0c };
static uint8_t cmd_sp_si1[]  = { 0x7e, 0x11, 0x03, 0x08, 0x01, 0x00, 0x1b };
static uint8_t cmd_stdby[]   = { 0x7e, 0x0a, 0x00, 0x0a };
#endif


#if 0
static const uint8_t FPX_GERARD[] = {
   0x1d, 0x6c, 0x06, 0x42, 0x1e, 0x00, 0x00, 0x04,  0x7e, 0x0f, 0x00, 0x0f, 0x0a, 0x00, 0x7e, 0x20,
   0x00, 0x20, 0x1e, 0x00, 0x7e, 0xf0, 0x00, 0xf0,  0x0a, 0x00, 0x7e, 0xff, 0x01, 0xfe
};

static const uint8_t FPX_ICD[] = {
   0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b,  0x7e, 0x0f, 0x00, 0x0f, 0x3c, 0x00, 0x7e, 0x0b,
   0x00, 0x0b, 0x3c, 0x00, 0x7e, 0x11, 0x03, 0x04,  0x01, 0x00, 0x17, 0xff, 0xff, 0x7e, 0x11, 0x03,
   0x00, 0x7b, 0x00, 0x69, 0xff, 0xff, 0x7e, 0x11,  0x03, 0x05, 0x10, 0x0a, 0x0d, 0xff, 0xff, 0x7e,
   0x11, 0x03, 0x02, 0xc8, 0x00, 0xd8, 0xff, 0xff,  0x7e, 0x0c, 0x00, 0x0c, 0x2c, 0x01, 0x7e, 0x20,
   0x00, 0x20, 0xff, 0xff, 0x7e, 0x21, 0x00, 0x21,  0xff, 0xff, 0x7e, 0xf0, 0x00, 0xf0, 0xff, 0xff,
   0x7e, 0xff, 0x01, 0xfe
/*
   0x43, 0xc0, 0xbf, 0x56, 0x1a, 0x10, 0x0e, 0x0a,  0x7e, 0x0f, 0x00, 0x0f, 0x3c, 0x00, 0x7e, 0x0b,
   0x00, 0x0b, 0x3c, 0x00, 0x7e, 0x11, 0x03, 0x04,  0x01, 0x00, 0x17, 0xff, 0xff, 0x7e, 0x11, 0x03,
   0x05, 0x10, 0x0a, 0x0d, 0xff, 0xff, 0x7e, 0x11,  0x03, 0x02, 0xc8, 0x00, 0xd8, 0xff, 0xff, 0x7e,
   0x0c, 0x00, 0x0c, 0x2c, 0x01, 0x7e, 0x20, 0x00,  0x20, 0xff, 0xff, 0x7e, 0x21, 0x00, 0x21, 0xff,
   0xff, 0x7e, 0xf0, 0x00, 0xf0, 0xff, 0xff, 0x7e,  0xff, 0x01, 0xfe
*/
};

static const uint8_t FPX_E2E_1_1[] = {
   0x23, 0x6c, 0x06, 0x42, 0x1e, 0x00, 0x00, 0x05,  0x7e, 0x0f, 0x00, 0x0f, 0x0a, 0x00, 0x7e, 0x20,
   0x00, 0x20, 0xd2, 0x00, 0x7e, 0x20, 0x00, 0x20,  0x0a, 0x00, 0x7e, 0xf0, 0x00, 0xf0, 0x0a, 0x00,
   0x7e, 0xff, 0x01, 0xfe
};

static const uint8_t FPX_E2E_1_2[] = {
   0x23, 0x98, 0x07, 0x42, 0x1e, 0x00, 0x00, 0x05,  0x7e, 0x0f, 0x00, 0x0f, 0x0a, 0x00, 0x7e, 0x20,
   0x00, 0x20, 0xd2, 0x00, 0x7e, 0x20, 0x00, 0x20,  0x0a, 0x00, 0x7e, 0xf0, 0x00, 0xf0, 0x0a, 0x00,
   0x7e, 0xff, 0x01, 0xfe
};

static const uint8_t FPX_E2E_1_3[] = {
   0x23, 0xc4, 0x08, 0x42, 0x1e, 0x00, 0x00, 0x05,  0x7e, 0x0f, 0x00, 0x0f, 0x0a, 0x00, 0x7e, 0x20,
   0x00, 0x20, 0xd2, 0x00, 0x7e, 0x20, 0x00, 0x20,  0x0a, 0x00, 0x7e, 0xf0, 0x00, 0xf0, 0x0a, 0x00,
   0x7e, 0xff, 0x01, 0xfe
};

static const uint8_t FPX_E2E_1_4[] = {
   0x23, 0xf0, 0x09, 0x42, 0x1e, 0x00, 0x00, 0x05,  0x7e, 0x0f, 0x00, 0x0f, 0x0a, 0x00, 0x7e, 0x20,
   0x00, 0x20, 0xd2, 0x00, 0x7e, 0x20, 0x00, 0x20,  0x0a, 0x00, 0x7e, 0xf0, 0x00, 0xf0, 0x0a, 0x00,
   0x7e, 0xff, 0x01, 0xfe
};

static const uint8_t FPX_E2E_1_5[] = {
   0x23, 0x1c, 0x0b, 0x42, 0x1e, 0x00, 0x00, 0x05,  0x7e, 0x0f, 0x00, 0x0f, 0x0a, 0x00, 0x7e, 0x20,
   0x00, 0x20, 0xd2, 0x00, 0x7e, 0x20, 0x00, 0x20,  0x0a, 0x00, 0x7e, 0xf0, 0x00, 0xf0, 0x0a, 0x00,
   0x7e, 0xff, 0x01, 0xfe
};

static const uint8_t FPX_E2E_1_6[] = {
   0x23, 0x48, 0x0c, 0x42, 0x1e, 0x00, 0x00, 0x05,  0x7e, 0x0f, 0x00, 0x0f, 0x0a, 0x00, 0x7e, 0x20,
   0x00, 0x20, 0xd2, 0x00, 0x7e, 0x20, 0x00, 0x20,  0x0a, 0x00, 0x7e, 0xf0, 0x00, 0xf0, 0x0a, 0x00,
   0x7e, 0xff, 0x01, 0xfe
};

static const uint8_t FPX_E2E_1_7[] = {
   0x23, 0x74, 0x0d, 0x42, 0x1e, 0x00, 0x00, 0x05,  0x7e, 0x0f, 0x00, 0x0f, 0x0a, 0x00, 0x7e, 0x20,
   0x00, 0x20, 0xd2, 0x00, 0x7e, 0x20, 0x00, 0x20,  0x0a, 0x00, 0x7e, 0xf0, 0x00, 0xf0, 0x0a, 0x00,
   0x7e, 0xff, 0x01, 0xfe
};

static const uint8_t FPX_E2E_2[] = {
   0x29, 0xf8, 0xa6, 0x42, 0x1e, 0x00, 0x00, 0x06,  0x7e, 0x0f, 0x00, 0x0f, 0x0a, 0x00, 0x7e, 0x20,
   0x00, 0x20, 0xfe, 0x06, 0x7e, 0x20, 0x00, 0x20,  0xf4, 0x06, 0x7e, 0x20, 0x00, 0x20, 0x0a, 0x00,
   0x7e, 0xf0, 0x00, 0xf0, 0x0a, 0x00, 0x7e, 0xff,  0x01, 0xfe
};

static const uint8_t FPX_E2E_5[] = {
   0x29, 0x40, 0x05, 0x42, 0x1e, 0x00, 0x00, 0x06,  0x7e, 0x0f, 0x00, 0x0f, 0x0a, 0x00, 0x7e, 0x20,
   0x00, 0x20, 0xd2, 0x00, 0x7e, 0x00, 0x00, 0x01,  0x0a, 0x00, 0x7e, 0x20, 0x00, 0x20, 0x0a, 0x00,
   0x7e, 0xf0, 0x00, 0xf0, 0x0a, 0x00, 0x7e, 0xff,  0x01, 0xfe
};

static const uint8_t FPX_E2E_6[] = {
   0x23, 0x40, 0x05, 0x42, 0x1e, 0x00, 0x00, 0x05,  0x7e, 0x0f, 0x00, 0x0f, 0x0a, 0x00, 0x7e, 0x20,
   0x00, 0x20, 0xd2, 0x00, 0x7e, 0x20, 0x00, 0x20,  0x0a, 0x00, 0x7e, 0xf0, 0x00, 0xf0, 0x0a, 0x00,
   0x7e, 0xff, 0x01, 0xfe
};

static const uint8_t FPX_E2E_7_1[] = {
   0x29, 0x80, 0x5c, 0x41, 0x1e, 0x00, 0x00, 0x06,  0x7e, 0x0f, 0x00, 0x0f, 0x0a, 0x00, 0x7e, 0x20,
   0x00, 0x20, 0xd2, 0x00, 0x7e, 0x20, 0x00, 0x20,  0xd2, 0x00, 0x7e, 0x20, 0x00, 0x20, 0xd2, 0x00,
   0x7e, 0xf0, 0x00, 0xf0, 0x0a, 0x00, 0x7e, 0xff,  0x01, 0xfe
};

static const uint8_t FPX_E2E_7_2[] = {
   0x0b, 0x00, 0xae, 0x42, 0x1e, 0x00, 0x00, 0x01,  0x7e, 0xff, 0x01, 0xfe
};

static const uint8_t FPX_E2E_7_3[] = {
   0x29, 0x80, 0xff, 0x43, 0x1e, 0x00, 0x00, 0x06,  0x7e, 0x0f, 0x00, 0x0f, 0x0a, 0x00, 0x7e, 0x20,
   0x00, 0x20, 0xd2, 0x00, 0x7e, 0x20, 0x00, 0x20,  0xd2, 0x00, 0x7e, 0x20, 0x00, 0x20, 0xd2, 0x00,
   0x7e, 0xf0, 0x00, 0xf0, 0x0a, 0x00, 0x7e, 0xff,  0x01, 0xfe
};

static const uint8_t FPX_E2E_8[] = {
   0x29, 0xc0, 0x56, 0x43, 0x1e, 0x58, 0x02, 0x06,  0x7e, 0x0f, 0x00, 0x0f, 0x0a, 0x00, 0x7e, 0x20,
   0x00, 0x20, 0x1e, 0x00, 0x7e, 0x20, 0x00, 0x20,  0x1e, 0x00, 0x7e, 0x20, 0x00, 0x20, 0x1e, 0x00,
   0x7e, 0xf0, 0x00, 0xf0, 0x0a, 0x00, 0x7e, 0xff,  0x01, 0xfe
};


static const uint8_t *sv[] = {
//FPX_E2E_5,
   FPX_GERARD,
   FPX_ICD,
   FPX_E2E_1_1,
   FPX_E2E_1_2,
   FPX_E2E_1_3,
   FPX_E2E_1_4,
   FPX_E2E_1_5,
   FPX_E2E_1_6,
   FPX_E2E_1_7,
   FPX_E2E_2,
   FPX_E2E_5,
   FPX_E2E_6,
   FPX_E2E_7_1,
   FPX_E2E_7_2,
   FPX_E2E_7_3,
   FPX_E2E_8
};
#endif


static const char *unk = "unknown";
#define UNKNOWN unk

static const char *rsp_ids[ 64 ] = {

   /* 0x00 - 0x0f */

   UNKNOWN,    UNKNOWN,    "SU_R_ACK", "SU_R_NACK",
   "SU_R_ID",  UNKNOWN,    UNKNOWN,    UNKNOWN,
   UNKNOWN,    UNKNOWN,    UNKNOWN,    UNKNOWN,
   UNKNOWN,    UNKNOWN,    UNKNOWN,    UNKNOWN,

   /* 0x10 - 0x1f */

   UNKNOWN,    UNKNOWN,    UNKNOWN,    UNKNOWN,
   UNKNOWN,    UNKNOWN,    UNKNOWN,    UNKNOWN,
   UNKNOWN,    UNKNOWN,    UNKNOWN,    UNKNOWN,
   UNKNOWN,    UNKNOWN,    UNKNOWN,    UNKNOWN,

   /* 0x20 - 0x2f */

   "SU_R_HK",  UNKNOWN,    UNKNOWN,    UNKNOWN,
   UNKNOWN,    UNKNOWN,    UNKNOWN,    UNKNOWN,
   UNKNOWN,    UNKNOWN,    UNKNOWN,    UNKNOWN,
   UNKNOWN,    UNKNOWN,    UNKNOWN,    UNKNOWN,

   /* 0x30 - 0x3f */

   "SU_R_SDP", UNKNOWN,    UNKNOWN,    "SU_R_CAL",
   UNKNOWN,    UNKNOWN,    UNKNOWN,    UNKNOWN,
   UNKNOWN,    UNKNOWN,    UNKNOWN,    UNKNOWN,
   UNKNOWN,    UNKNOWN,    UNKNOWN,    UNKNOWN
};


#if 0
void FipexThread::cmd( const uint8_t *cmd, size_t len )
{
   size_t n, dlen;
   Modem *modem;
   WodStore::WEH hdr;
   RTC::Time tm;
   time_t ts;

   UART2.write( cmd, len );
   n = UART2.read( _rx, 205, 1000 ); // Req: FPX-SW-0190 - "The response packet size shall be 205 bytes."

   if( n == 0 ) {
      kprintf( RED( "%s: n: %lu" ) "\r\n", name, n );
      kprintf( RED( "%s: tiemout in cmd()" ) "\r\n", name );
   } else {
      kprintf( GREEN( "%s: n: %lu" ) "\r\n", name, n );

      dlen = _rx[2];

      kprintf( "%s: len: %lu\r\n", name, dlen );
      kprintf( "%s: seq: %d\r\n", name, _rx[3] );
    //hexdump( _rx, n );

      switch( _rx[1] ) {

         case 0x20: /* SU_R_HK */
         case 0x30: /* SU_R_SDP */

            RTC0.getTime( tm );
            ts = RTC::conv( tm ) - VKI_EPOCH;
            _rx[ dlen +  5 ] = ( ts       ) & 0xff;
            _rx[ dlen +  6 ] = ( ts >>  8 ) & 0xff;
            _rx[ dlen +  7 ] = ( ts >> 16 ) & 0xff;
            _rx[ dlen +  8 ] = ( ts >> 24 ) & 0xff;

            _rx[ dlen +  9 ] = 0x00;  // Q1L
            _rx[ dlen + 10 ] = 0x00;  // Q1H
            _rx[ dlen + 11 ] = 0x00;  // Q2L
            _rx[ dlen + 12 ] = 0x00;  // Q2H
            _rx[ dlen + 13 ] = 0x00;  // Q3L
            _rx[ dlen + 14 ] = 0x00;  // Q3H
            _rx[ dlen + 15 ] = 0x00;  // Q4L
            _rx[ dlen + 16 ] = 0x00;  // Q4H

            _rx[ dlen + 17 ] = 0x00;  // XDL
            _rx[ dlen + 18 ] = 0x00;  // XDH
            _rx[ dlen + 19 ] = 0x00;  // YDL
            _rx[ dlen + 20 ] = 0x00;  // YDH
            _rx[ dlen + 21 ] = 0x00;  // ZDL
            _rx[ dlen + 22 ] = 0x00;  // ZDH

            _rx[ dlen + 23 ] = 0x00;  // XPL
            _rx[ dlen + 24 ] = 0x00;  // XPH
            _rx[ dlen + 25 ] = 0x00;  // YPL
            _rx[ dlen + 26 ] = 0x00;  // YPH
            _rx[ dlen + 27 ] = 0x00;  // ZPL
            _rx[ dlen + 28 ] = 0x00;  // ZPH

            (void)WOD1.write( WodStore::FIPEX, _rx + 1, dlen + 28, &hdr );

            if( CONF.getParam( Config::PARAM_MODEM ) == 1 ) {
               modem = &M1K2;
            } else {
               modem = &M9K6;
            }

            modem->enable();
            modem->send( &hdr, _rx + 1, -1 );
            modem->disable();

            break;

         default:
            break;

      }
   }
}
#endif

void FipexThread::dump( uint8_t *x, size_t len )
{
   size_t dlen = x[2];
   const char *name = rsp_ids[ x[1] % 0x3f ];

//hexdump( x, len );

   kprintf( "         SB: 0x%02x\r\n",      x[0] );
   kprintf( "     RSP_ID: 0x%02x (%s)\r\n", x[1], name );
   kprintf( "        LEN: %d\r\n",         dlen );
   kprintf( "    SEQ_CNT: %d\r\n",         x[3] );

   switch( x[1] ) {
      case 0x30: dump_sdp( x + 4, len - 4 ); break;
      default:
         ;
   }
}


void FipexThread::dump_sdp( uint8_t *x, size_t len )
{
   uint32_t tf = x[0] | ( x[1] << 8 ) | ( x[2] << 16 ) | ( x[3] << 24 );
   uint32_t ts = x[4] | ( x[5] << 8 ) | ( x[6] << 16 ) | ( x[7] << 24 );

   (void)len;

   kprintf( " TIME_FIPEX: %f\r\n",     0.1 * tf );
   kprintf( "   TIME_STM: %f\r\n",     0.1 * ts );
   kprintf( "         ID: 0x%02x\r\n", x[8] );

   int i = 1;

   size_t off = 9;

   for( ;; ) {
      uint8_t sh = x[ off ];
      uint8_t tp = ( sh >> 6 ) & 0x01;
      kprintf( " - SAMPLE #%d:\r\n", i++ );
      kprintf( " Header byte: 0x%02x\r\n", sh );
      kprintf( "        Gain: %d\r\n", (  sh        & 0x03 ));
      kprintf( "      Sensor: %d\r\n", (( sh >> 3 ) & 0x03 ));

      if( tp == 1 ) {
         /* FIPEX */
         kprintf( "        Type: FIPEX\r\n" );
         hexdump( x + off + 1, 7 );
         off += ( 7 + 1 );
      } else {
         /* STM */
         kprintf( "        Type: STM\r\n" );
         hexdump( x + off + 1, 9 );
         off += ( 9 + 1 );
      }

      if( sh & 0x80 ) break;
   }
}


void FipexThread::run( void )
{
   unsigned now;
   unsigned tNextStart;
   unsigned tNextCmd;

   unsigned dt, rt;
   unsigned n;

   bool debug = true;

   RTC::Time tm;

   Config::pval_t sn;

   Fipex::Script sc;
   Fipex::Script::Header *sh;

   Fipex::CmdHeader *ch;
   Fipex::RspHeader *rh = (Fipex::RspHeader*)_rx;

   for( ;; ) {
      _wait();

      kdebug( true, YELLOW( "%s: --- state #%d ---" ) "\r\n", name, _st );

      RTC0.getTime( tm );
      now = RTC::conv( tm );

      switch( _st ) {

         case ST_INIT:

            kdebug( debug, RED( "%s: ST_INIT" ) "\r\n", name );

            sn = CONF.getParam( Config::PARAM_FIPEX_SCRIPT_N );
            if( sn == 0 ) {
               sh = _whatNext( true );
            } else {
               sh = SU.loadScript( sn );
            }

            if( sh == 0 ) {
               delay( 5000 );
               break;
            }

            try {
               sc.load( sh );
            }

            catch( Fipex::Exception& e ) {
               kdebug( debug, RED( "%s: *** EXCEPTION CAUGHT ***" ) "\r\n", name );
               kdebug( debug, RED( "%s: %s" ) "\r\n", name, e.what() );
               delay( 5000 );
               break;
            }

            tNextStart = Fipex::Script::startTime( sh );

            if( tNextStart == VKI_EPOCH ) {
               // start time == EPOCH ?
               // -> set tNextStart to now and run the script right away
               tNextStart = now;
               _st = ST_START_SCRIPT;
            } else {
               _st = ST_WAIT_SCRIPT;
            }

            break;


         case ST_IDLE:

            kdebug( debug, RED( "%s: ST_IDLE" ) "\r\n", name );

            sh = _whatNext( debug );
            if( sh ) {
               kdebug( debug, "%s: next script found\r\n", name );
            } else {
               kdebug( debug, "%s: no script to run\r\n", name );
            }

            delay( 5000 );

            break;


         case ST_WAIT_SCRIPT:

            sc.reset();

            kdebug( debug, RED( "%s: ST_WAIT_SCRIPT" ) "\r\n", name );

            if( now > tNextStart ) {

               rt = Fipex::Script::repeatTime( sh );

               if( rt == 0 ) {
                  kdebug( debug, "%s: start time in the past and no repeat time\r\n", name );
                  _st = ST_IDLE;
                  break;
               }

               while( now > tNextStart ) {
                  tNextStart += rt;
               }
            }

            dt = tNextStart - now;

            kdebug( debug, YELLOW( "%s: script due to start in %lu sec" ) "\r\n", name, dt );

            if( dt > 0 ) {
               delay( 1000 );
            } else {
               _st = ST_START_SCRIPT;
            }

            break;


         case ST_START_SCRIPT:

            kdebug( debug, RED( "%s: ST_START_SCRIPT" ) "\r\n", name );

            tNextCmd = tNextStart;
            _st = ST_SYNC_WAIT;

            break;


         case ST_EXEC_CMD:

            kdebug( debug, RED( "%s: ST_EXEC_CMD" ) "\r\n", name );

            try {
               ch = sc.next();
               dt = sc.delay( ch );
               tNextCmd += dt;
               _runCmd( ch, rh );
            }

            catch( Fipex::Exception& e ) {
               kprintf( RED( "%s: *** EXCEPTION CAUGHT ***" ) "\r\n", name );
               kprintf( RED( "%s: %s" ) "\r\n", name, e.what() );
               _st = ST_ERROR;
            }


            break;


         case ST_SYNC_WAIT:

            kdebug( debug, RED( "%s: ST_SYNC_WAIT" ) "\r\n", name );

            if( now < tNextCmd ) {
               kdebug( debug, YELLOW( "%s: sync wait: %lu sec" ) "\r\n", name, tNextCmd - now );
               delay( 1000 );
            } else {
               _st = ST_EXEC_CMD;
            }

            break;


         case ST_ASYNC_WAIT:

            kdebug( debug, RED( "%s: ST_ASYNC_WAIT" ) "\r\n", name );

            if( now < tNextCmd ) {
               n = SU.recv( rh, 1000 );

               if( n == 0 ) {
                  kdebug( debug, YELLOW( "%s: async wait: %lu sec" ) "\r\n", name, tNextCmd - now );
               } else {
                  kdebug( debug, YELLOW( "%s: *** SCIENCE DATA ***" ) "\r\n", name );
                  _handleRsp( rh );
               }

               break;
            }

            _st = ST_EXEC_CMD;

            break;


         case ST_ERROR:

            kdebug( debug, RED( "%s: ST_ERROR" ) "\r\n", name );

            n = SU.send( (Fipex::CmdHeader*)cmd_dp, rh, 500 );
            if( n > 0 ) {
               kdebug( debug, YELLOW( "%s: ERROR RECOVERY - got science data packet" ) "\r\n", name );
               _handleRsp( rh );
            }

            n = SU.send( (Fipex::CmdHeader*)cmd_hk, rh, 500 );
            if( n > 0 ) {
               kdebug( debug, YELLOW( "%s: ERROR RECOVERY - got housekeeping packet" ) "\r\n", name );
               _handleRsp( rh );
            }

            SU.disable();
            _st = ST_INIT;

            break;

      }

   }
}


void FipexThread::onSuspend( void )
{
   SU.disable();
   Thread::onSuspend();
}


void FipexThread::onResume( void )
{
   Thread::onResume();
   _st = ST_INIT;
}


//  - - - - - - - - - - - - - - -  //
//  P R I V A T E   M E T H O D S  //
//  - - - - - - - - - - - - - - -  //

Fipex::Script::Header*
FipexThread::_whatNext( bool debug )
{
   RTC::Time tm;

   Fipex::Script sc;
   Fipex::Script::Header *sh;
   Fipex::Script::Header *nh;

   unsigned now;  // current time
   unsigned sn;   // script number
   unsigned ts;   // timestamp
   unsigned rt;   // repeat time
   unsigned nt;   // next time

   RTC0.getTime( tm );
   now = RTC::conv( tm );

   kdebug( debug, "%s: now is @%lu\r\n", name, now );

   nh = (Fipex::Script::Header*)0;
   nt = VKI_NEVER;

   for( sn = 0 ; sn < 8 ; ++sn ) {

      kdebug( debug, "%s: considering script #%u...\r\n", name, sn );
      sh = SU.loadScript( sn );

      try {
         sc.load( sh );
      }

      catch( Fipex::Exception& e ) {
         kdebug( debug, "%s: skipping script #%u (can't load)\r\n", name );
         continue;
      }

      ts = Fipex::Script::startTime( sh );

      if(( now <= ts ) && ( ts < nt )) {

         kdebug( debug, "%s: best candidate: script #%u (start) @%lu\r\n", name, sn, ts );
         nt = ts;
         nh = sh;

      } else {

         rt = Fipex::Script::repeatTime( sh );

         if( rt == 0 ) {
            kdebug( debug, "%s: skipping script #%u (not scheduled)\r\n", name, sn );
            continue;
         }

         while( now > ts ) {
            ts += rt;
         }

         if( ts < nt ) {
            kdebug( debug, "%s: best candidate: script #%u (repeat) @%lu\r\n", name, sn, ts );
            nt = ts;
            nh = sh;
         }

      }
   }

   return nh;
}


void FipexThread::_runCmd( Fipex::CmdHeader *ch, Fipex::RspHeader *rh, bool debug )
{
   switch( ch->id ) {

      case Fipex::OBC_SU_ON:

         kdebug( debug, "%s: > OBC_SU_ON\r\n", name );
         SU.enable( debug );
         _st = ST_SYNC_WAIT;

         break;


      case Fipex::OBC_SU_OFF:

         kdebug( debug, "%s: > OBC_SU_OFF\r\n", name );
         SU.disable( debug );
         _st = ST_SYNC_WAIT;

         break;


      case Fipex::OBC_SU_END:

         kdebug( debug, "%s: > OBC_SU_END\r\n", name );
         _st = ST_INIT;

         break;


      default:

         if( !SU.send( ch, rh, 500 )) {
            kprintf( RED( "%s: SU.send() failed, retrying with SU_RSP" ) "\r\n", name );
            if( !SU.send( (Fipex::CmdHeader*)cmd_rsp, rh, 500 )) {
               kprintf( RED( "%s: SU.send() failed again, aborting" ) "\r\n", name );
               _st = ST_ERROR;
               break;
            }
         }

         _handleRsp( rh );

         break;
   }
}


void FipexThread::_handleRsp( Fipex::RspHeader *rh, bool debug )
{
   WodStore::WEnt wod;

   (void)debug;
   kdebug( true, "%s: < %s\r\n", name, Fipex::Script::rspName( (Fipex::RspId)rh->id ));

   switch( rh->id ) {

      case Fipex::SU_R_ACK:

         _st = ST_ASYNC_WAIT;

         break;


      case Fipex::SU_R_ID:

         _st = ST_ASYNC_WAIT;

         break;


      case Fipex::SU_R_NACK:

         wod.type = WodStore::FIPEX;
         wod.len  = sizeof( Fipex::RspHeader ) + rh->len;
         (void)WOD1.write( &wod, rh, true ); // <- SYNC WOD1

         /* XXX resend command */
         _st = ST_ERROR;

         break;


      default:

         (void)ADCS0.getMeas( _mp );

         wod.type = WodStore::ADCS;
         wod.len  = sizeof( ADCSMeas );
         (void)WOD1.write( &wod, _mp );

         wod.type = WodStore::FIPEX;
         wod.len  = sizeof( Fipex::RspHeader ) + rh->len;
         (void)WOD1.write( &wod, rh, true ); // <- SYNC WOD1

         _st = ST_ASYNC_WAIT;

         break;

   }
}


/*EoF*/

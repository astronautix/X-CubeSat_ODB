
#ifndef _QB50_FIPEX_THREAD_H
#define _QB50_FIPEX_THREAD_H

#include "system/qb50.h"
#include "common/Message.h"
#include "Fipex.h"


namespace qb50 {

   class FipexThread : public Thread
   {

      public:

         FipexThread();
         ~FipexThread();

         enum State {
            ST_INIT         = 0,  // initialization
            ST_IDLE         = 1,  // idle
            ST_WAIT_SCRIPT  = 2,  // wait for script to start
            ST_START_SCRIPT = 3,  // script starting
            ST_EXEC_CMD     = 4,  // sending command
            ST_SYNC_WAIT    = 5,  // command sent, wait for response (synchronous)
            ST_ASYNC_WAIT   = 6,  // command sent, wait for response (asynchronous)
            ST_ERROR        = 7   // error condition
         };

         void run       ( void );
         void cmd       ( const uint8_t *x, size_t len );

         void dump      ( uint8_t *x, size_t len );
         void dump_sdp  ( uint8_t *x, size_t len );

         void onSuspend ( void );
         void onResume  ( void );

      private:

#if 0
         void _runScript( const uint8_t *x, bool debug = false );
#endif

         State     _st;  // current state
         uint8_t  *_rx;  // receive buffer
         ADCSMeas *_mp;  // last known attitude

         void _runCmd    ( Fipex::CmdHeader *ch, Fipex::RspHeader *rh, bool debug = false );
         void _handleRsp (                       Fipex::RspHeader *rh, bool debug = false );

         Fipex::Script::Header* _whatNext( bool debug = false );

   };

} /* qb50 */


#endif /*_QB50_FIPEX_THREAD_H*/

/*EoF*/


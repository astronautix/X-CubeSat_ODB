/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:22:42+02:00
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


/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:20:35+02:00
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
 
 
#include "TelemThread.h"
#include "AX25Modem.h"
#include "Event.h"
#include "WodStore.h"
#include "Config.h"
#include "devices.h"

using namespace qb50;

extern QueueHandle_t evQueue;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

TelemThread::TelemThread()
   : Thread( "Telem", 1, SUSPENDED, 512 )
{
   _modem = &M1K2;
   _tsMax = 0xffffffff;
   _x     = new uint8_t[ 256 ];
}


TelemThread::~TelemThread()
{
   delete[] _x;
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

void TelemThread::onSuspend( void )
{
   _modem->disable();
   Thread::onSuspend();
}


void TelemThread::onResume( void )
{
   Thread::onResume();

   if( CONF.getParam( Config::PARAM_MODEM ) == 1 ) {
      _modem = &M1K2;
   } else {
      _modem = &M9K6;
   }

   _modem->enable();

   unsigned tmn = CONF.getParam( Config::PARAM_TELEM_TIMER );

   if( tmn == 0 ) {
      _tsMax = 0xffffffff;
   } else {
      unsigned ts  = ticks();
      _tsMax = ts + ( tmn * 60 * 1000 );
      kprintf( YELLOW( "%s: ts: %lu, tsMax: %lu " ) "\r\n", name, ts, _tsMax );
   }
}


void TelemThread::run( void )
{
 //WodStore::WEH hdr;
   WodStore::WEnt wod;
   unsigned ts;

   for( ;; ) {

      _wait();

      ts = ticks();

      /* science data is priority #1 */

      (void)WOD1.read( &wod, _x );

      kprintf( YELLOW( "%s: ts: %lu, tsMax: %lu " ) "\r\n", name, ts, _tsMax );

      if(( wod.type != WodStore::NONE ) && ( ts < _tsMax )) {

         kprintf( "FIPEX WOD - type: %d, len: %d, prev: 0x%08x\r\n", wod.type, wod.len, wod.prev );
         _modem->send( &wod, _x, -1);

         continue;  // <- YES, CONTINUE (SCI data first)
      }

      /* transmit regular telemetry if no science data is available */

      (void)WOD0.read( &wod, _x );

      if(( wod.type != WodStore::NONE ) && ( ts < _tsMax )) {

         kprintf( "WODEX WOD - type: %d, len: %d, prev: 0x%08x\r\n", wod.type, wod.len, wod.prev );
         _modem->send( &wod, _x, -1 );

      } else {

         Event *ev = new Event( Event::WOD_EMPTY );
         xQueueSendToBack( evQueue, &ev, portMAX_DELAY );

         delay( 1000 );

      }
   }
}

/*EoF*/

/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:18:05+02:00
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



#include "CommandThread.h"
#include "common/Message.h"
#include "config.h"

using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

CommandThread::CommandThread()
   : Thread( "Command Thread", 2, RUNNING, 384 )
{
   _x = new uint8_t[ 256 ];
}


CommandThread::~CommandThread()
{
   delete[] _x;
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

void CommandThread::onSuspend( void )
{
   Thread::onSuspend();
}


void CommandThread::onResume( void )
{
   Thread::onResume();
}


void CommandThread::run( void )
{
   size_t n;
   Timer tm;

   const ADCSCtrl *m = (const ADCSCtrl*)_x;

   for( ;; ) {

      n = UART1.read( _x, sizeof( ADCSCtrl ), 5000 );

      if( n == 0 ) {
         kprintf( YELLOW( "waiting for ODB..." ) "\r\n" );
         continue;
      }

      kprintf( GREEN( "D: %ld, X: %ld, Y: %ld, Z: %ld" ) "\r\n", m->d, m->x, m->y, m->z );

      AST0.n = m->n;
      AST0.d = m->d;
      AST0.x = m->x;
      AST0.y = m->y;
      AST0.z = m->z;

   }
}


/*EoF*/

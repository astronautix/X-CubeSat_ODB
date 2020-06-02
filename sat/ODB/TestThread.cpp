/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:20:30+02:00
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
 
 
#include "devices.h"
#include "TestThread.h"
#include "Config.h"
#include "Event.h"
#include "system/Application.h"

using namespace qb50;


extern QueueHandle_t evQueue;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

TestThread::TestThread()
   : Thread( "Test Thread", 1, SUSPENDED, 256 ),
     _mode( ON )
{ ; }


TestThread::~TestThread()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

void TestThread::onSuspend( void )
{
 //MAG0.disable();
   Thread::onSuspend();
}


void TestThread::onResume( void )
{
   Thread::onResume();
 //MAG0.enable();
}


void TestThread::run( void )
{
   for( ;; ) {

      _wait();

      delay( 1000 );

   }
}

/*EoF*/

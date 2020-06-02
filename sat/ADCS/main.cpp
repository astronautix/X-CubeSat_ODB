/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:18:18+02:00
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
 
 
#include "config.h"
#include "MainThread.h"

using namespace qb50;

extern void AttitudeControlThread           ( Thread *self );  //Thread for Attitude Control
extern void AttitudeDeterThread             ( Thread *self );  //Thread for Attitude Determination
extern void ODBCommUpThread                 ( Thread *self );  //Thread for Communication with ODB (Sending Data)
extern void ODBCommDownThread               ( Thread *self );  //Thread for Communication with ODB (Receiving Data)
extern void TestThreads                     ( Thread *self );  //Thread for testing purposes only

static void initDevices( void );


void initDevices( void )
{
   SYSLOG.init();
      RCC.init();
      PWR.init();
      BKP.init();
    GPIOA.init().enable();
    GPIOB.init().enable();
    GPIOC.init().enable();
    UART1.init();
    UART6.init();
     SPI3.init();
     ADC1.init();
   FLASH0.init();
     GYR0.init();
     MAG0.init();
}


int main( void )
{
   AST0.d = 0;
   AST0.x = 0;
   AST0.y = 0;
   AST0.z = 0;

   /* initialize devices */

   initDevices();

   R1.out().off();
   R2.out().off();
   R3.out().off();
   F1.out().off();
   F2.out().off();
   F3.out().off();

   PB15.out().on();

   UART6.enable();
   SYSLOG.enable();
   BKP.enable();

   /* create worker threads */

   (void)registerThread( new MainThread() );

   run();

}

/*EoF*/

/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:21:56+02:00
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
 
 
#include "CTCSS.h"

using namespace qb50;


CTCSS qb50::CTCS0( "CTCS0", PB12, PA8 );


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

CTCSS::CTCSS( const char *name, STM32_GPIO::Pin& enPin, STM32_GPIO::Pin& dtPin )
   : Device( name ), _enPin( enPin ), _dtPin( dtPin )
{
   _semDecTone = xSemaphoreCreateBinary();
}


CTCSS::~CTCSS()
{
   vSemaphoreDelete( _semDecTone );
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

CTCSS& CTCSS::init()
{
   _dtPin . in()  . noPull();
   _enPin . out() . off();

   EXTI.registerHandler( _dtPin, this, STM32_EXTI::BOTH );

   kprintf( "%s: CTCSS handler, enPin: %s, dtPin: %s\r\n",
            _name, _enPin.name(), _dtPin.name() );

   return *this;
}


CTCSS& CTCSS::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   _enPin.on();

   if( debug ) {
      kprintf( "%s: enabled\r\n", _name );
   }

   return *this;
}


CTCSS& CTCSS::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   _enPin.off();

   if( debug ) {
      kprintf( "%s: disabled\r\n", _name );
   }

   return *this;
}


bool CTCSS::wait( unsigned ms )
{
   TickType_t dt;

   if( ms == 0 ) {
      dt = portMAX_DELAY;
   } else {
      dt = (TickType_t)ms / portTICK_RATE_MS;
   }

 //bool tone = decTone();
 //int  rv   = xSemaphoreTake( _semDecTone, dt );

   return
      ( xSemaphoreTake( _semDecTone, dt ) == pdTRUE );
}


bool CTCSS::decTone( void )
{
   return
      _dtPin.read() == false;
}


//  - - - - - - - - - - - -  //
//  E X T I   H A N D L E R  //
//  - - - - - - - - - - - -  //

void CTCSS::handle( STM32_EXTI::EXTIn /* ignored */ )
{
   portBASE_TYPE hpTask = pdFALSE;

   (void)xSemaphoreGiveFromISR( _semDecTone, &hpTask );

   if( hpTask == pdTRUE )
      portEND_SWITCHING_ISR( hpTask );
}


/*EoF*/

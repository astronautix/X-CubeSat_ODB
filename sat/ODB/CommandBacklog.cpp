/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:22:20+02:00
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
 
 
#include "CommandBacklog.h"

using namespace qb50;

CommandBacklog qb50::CBL( 16 );


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

CommandBacklog::CommandBacklog( size_t max )
{
   _forms = new Form[ max ];
   _lock  = xSemaphoreCreateMutex();
   _max   = max;
   _cur   = 0;
}


CommandBacklog::~CommandBacklog()
{
   vSemaphoreDelete( _lock );
   delete[] _forms;
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

bool CommandBacklog::push( Form& form )
{
   (void)xSemaphoreTake( _lock, portMAX_DELAY );

   if( _cur < _max ) {
      _forms[ _cur++ ] = form;
      (void)xSemaphoreGive( _lock );
      return true;
   }

   (void)xSemaphoreGive( _lock );
   return false;
}


bool CommandBacklog::pop( Form& form )
{
   (void)xSemaphoreTake( _lock, portMAX_DELAY );

   if( _cur > 0 ) {
      form = _forms[ --_cur ];
      (void)xSemaphoreGive( _lock );
      return true;
   }

   (void)xSemaphoreGive( _lock );
   return false;
}


/*EoF*/

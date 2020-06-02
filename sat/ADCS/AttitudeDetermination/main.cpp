/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:19:00+02:00
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
 
 #include <stdio.h>
#include "system/qb50.h"
#include "common/State.h"

using namespace qb50;

void AttitudeDeterThread( Thread *self)
{
    (void)self;

    for(;;){
            delay( 2000 );
            (void)printf(" ------- Attitude Determination Thread -------\r\n");
            //Modification for demonstrations purposes only
            //For soutenance de PSC 01/06/2015
            Current_state.PWM.duty_cycle_x = 30;
            Current_state.PWM.x_dir = 1;
            Current_state.PWM.duty_cycle_y = 30;
            Current_state.PWM.y_dir = 1;
            Current_state.PWM.duty_cycle_z = 30;
            Current_state.PWM.z_dir = 1;

            Current_state.POS.pos_x += 0.2;
            Current_state.POS.pos_y +=  -0.3;
            Current_state.POS.pos_z -= 0.2;
    }
}

/*EoF*/

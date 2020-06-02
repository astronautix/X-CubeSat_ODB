/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:19:04+02:00
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

void AttitudeControlThread( Thread *self)
{
    (void)self;

    for(;;){
            delay( 2000 );
            (void)printf(" ------- Attitude Control Thread -------\r\n");
            //Modification for demonstrations purposes only
            //For soutenance de PSC 01/06/2015

            (void)printf( "PWM Duty Cycle x: %lu\r \n", Current_state.PWM.duty_cycle_x);
            (void)printf( "PWM X Dir: %d\r \n", Current_state.PWM.x_dir);

            (void)printf( "PWM Duty Cycle x: %lu\r \n", Current_state.PWM.duty_cycle_y);
            (void)printf( "PWM X Dir: %d\r \n", Current_state.PWM.y_dir);

            (void)printf( "PWM Duty Cycle x: %lu\r \n", Current_state.PWM.duty_cycle_z);
            (void)printf( "PWM X Dir: %d\r \n", Current_state.PWM.z_dir);

    }

}

/*EoF*/

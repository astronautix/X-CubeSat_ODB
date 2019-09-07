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

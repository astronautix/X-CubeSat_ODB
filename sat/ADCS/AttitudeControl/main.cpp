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

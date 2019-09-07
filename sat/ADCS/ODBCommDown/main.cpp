
#include "config.h"
#include "common/State.h"

using namespace qb50;



void ODBCommDownThread( Thread *self)
{
    (void)self;

    Current_state.LCR = STOP;

    for(;;){
            delay( 2000 );
            kprintf( "------- ODB Communication (DOWN) Thread -------\r\n" );
    }
}

/*EoF*/

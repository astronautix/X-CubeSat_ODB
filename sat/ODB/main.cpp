
#include "system/qb50.h"

#include "WatchdogThread.h"
#include "ControlThread.h"

using namespace qb50;


#define LED1 PC5
#define LED2 PB0
#define LED3 PB1
#define LED4 PA1


int main( void )
{
   (void)registerThread( new ControlThread() );
   run(); // FreeRTOS RUN.
}

/*EoF*/

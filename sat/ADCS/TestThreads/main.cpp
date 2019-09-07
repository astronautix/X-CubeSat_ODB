
#include "config.h"
#include "common/State.h"

using namespace qb50;

void testThreadADC()
{
    int i;

    STM32_ADC::Channel* ch[6] = { &SUN1, &SUN2, &SUN3, &SUN4, &SUN5, &SUN6 };

    for( i = 0 ; i < 6 ; ++i )
        ch[i]->enable();

    for( ;; ) {
        kprintf( "------- ADC Test Thread -------\r\n" );
        kprintf( "[DATA ADC]\r\n" );

        for( i = 0 ; i < 6 ; ++i )
            (void)ch[i]->read( &Current_state.TADC.adc_temp[i] );

        kprintf( "SUN sensors: [ %d, %d, %d, %d, %d, %d ]\r\n",
                  Current_state.TADC.adc_temp[0],
                  Current_state.TADC.adc_temp[1],
                  Current_state.TADC.adc_temp[2],
                  Current_state.TADC.adc_temp[3],
                  Current_state.TADC.adc_temp[4],
                  Current_state.TADC.adc_temp[5]
        );

        delay(5000);
    }
}

void TestThreads( Thread *self)
{
    (void)self;
    testThreadADC();
}

/*EoF*/

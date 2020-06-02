/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:18:41+02:00
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

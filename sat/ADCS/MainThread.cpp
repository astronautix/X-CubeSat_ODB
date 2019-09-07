
#include "MainThread.h"
#include "CommandThread.h"
#include "common/Message.h"
#include "config.h"

#include <cstring>

#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>

#undef ADC1

/* _T_PERIOD = 640 */
#define _T_PERIOD ( 16000000 / 25000 )
#define _T_PULSE  ( _T_PERIOD / 10 )


using namespace qb50;


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

MainThread::MainThread()
   : Thread( "Main Thread", 2, RUNNING, 384 )
{
   ;
}


MainThread::~MainThread()
{
   ;
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

void MainThread::initTimers( void )
{
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE );
   RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM8, ENABLE );

   TIM_TimeBaseInitTypeDef timerInitStructure;

   /* initialize timer 1 */

   TIM_TimeBaseStructInit( &timerInitStructure );

   timerInitStructure.TIM_Prescaler         = 0;
   timerInitStructure.TIM_Period            = _T_PERIOD;
   timerInitStructure.TIM_CounterMode       = TIM_CounterMode_Up;
   timerInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
   timerInitStructure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInit( TIM1, &timerInitStructure );

   TIM_Cmd( TIM1, ENABLE );

   /* initialize timer 2 */

   TIM_TimeBaseStructInit( &timerInitStructure );

   timerInitStructure.TIM_Prescaler         = 0;
   timerInitStructure.TIM_Period            = _T_PERIOD;
   timerInitStructure.TIM_CounterMode       = TIM_CounterMode_Up;
   timerInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
   timerInitStructure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInit( TIM2, &timerInitStructure );

   TIM_Cmd( TIM2, ENABLE );

   /* initialize timer 8 */

   TIM_TimeBaseStructInit( &timerInitStructure );

   timerInitStructure.TIM_Prescaler         = 0;
   timerInitStructure.TIM_Period            = _T_PERIOD;
   timerInitStructure.TIM_CounterMode       = TIM_CounterMode_Up;
   timerInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
   timerInitStructure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInit( TIM8, &timerInitStructure );

   TIM_Cmd( TIM8, ENABLE );
}


void MainThread::initPWMChans( void )
{
   TIM_OCInitTypeDef outputChannelInit;

   /* initialize PWM for TIM1_CH1N */

   TIM_OCStructInit( &outputChannelInit );

   outputChannelInit.TIM_OCMode       = TIM_OCMode_PWM1;
   outputChannelInit.TIM_Pulse        = _T_PULSE;
   outputChannelInit.TIM_OutputState  = TIM_OutputState_Disable;
   outputChannelInit.TIM_OutputNState = TIM_OutputNState_Enable;
   outputChannelInit.TIM_OCNPolarity  = TIM_OCNPolarity_High;
   outputChannelInit.TIM_OCNIdleState = TIM_OCIdleState_Reset;

   TIM_OC1Init( TIM1, &outputChannelInit );
   TIM_OC1PreloadConfig( TIM1, TIM_OCPreload_Enable );

   TIM_CtrlPWMOutputs( TIM1, ENABLE );

   /* initialize PWM for TIM2_CH4 */

   TIM_OCStructInit( &outputChannelInit );

   outputChannelInit.TIM_OCMode       = TIM_OCMode_PWM1;
   outputChannelInit.TIM_Pulse        = _T_PULSE;
   outputChannelInit.TIM_OutputState  = TIM_OutputState_Enable;
   outputChannelInit.TIM_OCPolarity   = TIM_OCPolarity_High;
   outputChannelInit.TIM_OCIdleState  = TIM_OCIdleState_Reset;

   TIM_OC4Init( TIM2, &outputChannelInit );
   TIM_OC4PreloadConfig( TIM2, TIM_OCPreload_Enable );

   /* initialize PWM for TIM8_CH1N */

   TIM_OCStructInit( &outputChannelInit );

   outputChannelInit.TIM_OCMode       = TIM_OCMode_PWM1;
   outputChannelInit.TIM_Pulse        = _T_PULSE;
   outputChannelInit.TIM_OutputState  = TIM_OutputState_Disable;
   outputChannelInit.TIM_OutputNState = TIM_OutputNState_Enable;
   outputChannelInit.TIM_OCNPolarity  = TIM_OCNPolarity_High;
   outputChannelInit.TIM_OCNIdleState = TIM_OCIdleState_Reset;

   TIM_OC1Init( TIM8, &outputChannelInit );
   TIM_OC1PreloadConfig( TIM8, TIM_OCPreload_Enable );

   TIM_CtrlPWMOutputs( TIM8, ENABLE );
}


void MainThread::setCoils( int x, int y, int z )
{
   if( x < 0 ) {
      F2.on();
      TIM2->CCR4 = ( 100 + x ) * _T_PERIOD / 100;
   } else {
      F2.off();
      TIM2->CCR4 =         x   * _T_PERIOD / 100;
   }

   if( y < 0 ) {
      F3.on();
      TIM8->CCR1 = ( 100 + y ) * _T_PERIOD / 100;
   } else {
      F3.off();
      TIM8->CCR1 =         y   * _T_PERIOD / 100;
   }

   if( z < 0 ) {
      F1.on();
      TIM1->CCR1 = ( 100 + z ) * _T_PERIOD / 100;
   } else {
      F1.off();
      TIM1->CCR1 =         z   * _T_PERIOD / 100;
   }
}


void MainThread::onSuspend( void )
{
   Thread::onSuspend();

   UART1.disable();
    GYR0.disable();
    MAG0.disable();
}


void MainThread::onResume( void )
{
   GYR0.enable();
   MAG0.enable();
   UART1.enable();

   Thread::onResume();
}


void MainThread::run( void )
{
   SensorSample<uint16_t> sunv[ 6 ];
   Vec3D gyr, mag;
   Timer tm;

   SUN1.enable();
   SUN2.enable();
   SUN3.enable();
   SUN4.enable();
   SUN5.enable();
   SUN6.enable();

   GYR0.enable();
   MAG0.enable();

   UART1.enable();

#undef TIM1
#undef TIM2
#undef TIM8

   R1.oType( STM32_GPIO::PUSHPULL ).noPull().alt( STM32_GPIO::TIM1 ); /* PB13 */
   R2.oType( STM32_GPIO::PUSHPULL ).noPull().alt( STM32_GPIO::TIM2 ); /* PB11 */
   R3.oType( STM32_GPIO::PUSHPULL ).noPull().alt( STM32_GPIO::TIM8 ); /* PA5  */

   initTimers();
   initPWMChans();

   setCoils( 0, 0, 0 );

   (void)registerThread( new CommandThread() );

   ADCSBeat *bp = new ADCSBeat();

   for( ;; ) {

      tm.every( 1000 );

/*
F1.toggle();
F2.toggle();
F3.toggle();
*/
      setCoils( 0, 0, 0 );
      delay( 200 );

      GYR0.omega( gyr );
      kprintf( "GYR0: [ %0.2f %0.2f %0.2f ]\r\n", gyr.x, gyr.y, gyr.z );

      MAG0.omega( mag );
      kprintf( "MAG0: [ %0.2f %0.2f %0.2f ]\r\n", mag.x, mag.y, mag.z );

      ADC1.read( SUN1, &sunv[ 0 ]);
      ADC1.read( SUN2, &sunv[ 1 ]);
      ADC1.read( SUN3, &sunv[ 2 ]);
      ADC1.read( SUN4, &sunv[ 3 ]);
      ADC1.read( SUN5, &sunv[ 4 ]);
      ADC1.read( SUN6, &sunv[ 5 ]);

      kprintf(
         "SUNV: [ %u %u %u %u %u %u ]\r\n",
         sunv[0].value, sunv[1].value, sunv[2].value,
         sunv[3].value, sunv[4].value, sunv[5].value
      );

      if( AST0.d == 0 ) {
         setCoils( 0, 0, 0 );
      } else {
         setCoils( AST0.x, AST0.y, AST0.z );
         --AST0.d;
      }

      bp->ctrl.n = AST0.n;
      bp->ctrl.d = AST0.d;
      bp->ctrl.x = AST0.x;
      bp->ctrl.y = AST0.y;
      bp->ctrl.z = AST0.z;

      bp->meas.gxyz[0] = (int)( gyr.x / 16.0 );
      bp->meas.gxyz[1] = (int)( gyr.y / 16.0 );
      bp->meas.gxyz[2] = (int)( gyr.z / 16.0 );

      bp->meas.mxyz[0] = (int)( mag.x / 16.0 );
      bp->meas.mxyz[1] = (int)( mag.y / 16.0 );
      bp->meas.mxyz[2] = (int)( mag.z / 16.0 );

      bp->meas.xf = sunv[0].value >> 4;
      bp->meas.xr = sunv[1].value >> 4;
      bp->meas.yf = sunv[2].value >> 4;
      bp->meas.yr = sunv[3].value >> 4;
      bp->meas.zf = sunv[4].value >> 4;
      bp->meas.zr = sunv[5].value >> 4;

      (void)UART1.write( (const uint8_t*)bp, sizeof( ADCSBeat ), 100 );

      if( AST0.d > 0 ) {
         /* attitude control */
      }

   }

   delete bp;
}


/*EoF*/

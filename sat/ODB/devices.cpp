/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:21:42+02:00
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
 
 
#include "devices.h"
#include <safe_stm32f4xx.h>


namespace qb50 {

//  - - - - - - - - - - - - -  //
//  S Y S T E M   L O G G E R  //
//  - - - - - - - - - - - - -  //

   Syslog SYSLOG( "SYSLOG", UART6 );

//  - - - - - - - - - -  //
//  C O R E   B U S E S  //
//  - - - - - - - - - -  //

   /*        id         name */
   Bus AHB1( Bus::AHB1, "AHB1" );
   Bus AHB2( Bus::AHB2, "AHB2" );
   Bus AHB3( Bus::AHB3, "AHB3" );

   /*        id         name */
   Bus APB1( Bus::APB1, "APB1" );
   Bus APB2( Bus::APB2, "APB2" );

//  - - - - - - - - - - - - - - - - - - - - - -  //
//  R E S E T / C L O C K   C O N T R O L L E R  //
//  - - - - - - - - - - - - - - - - - - - - - -  //

   STM32_RCC RCC( RCC_BASE, "RCC" );

//  - - - - - - - -  //
//  W A T C H D O G  //
//  - - - - - - - -  //

   STM32_IWDG WDG( IWDG_BASE, "WDG" );

//  - - - - - - - - - - - - - -  //
//  R T C   C O N T R O L L E R  //
//  - - - - - - - - - - - - - -  //

   STM32_RTC RTC0( APB1, RTC_BASE, STM32_RCC::APB1Periph_RTC, "RTC0" );

//  - - - - - - - - - - - - - - - - - - - -  //
//  I N T E R R U P T   C O N T R O L L E R  //
//  - - - - - - - - - - - - - - - - - - - -  //

   STM32_NVIC NVIC;
   STM32_EXTI EXTI;

//  - - - - - - - - - - - - - - - -  //
//  P O W E R   C O N T R O L L E R  //
//  - - - - - - - - - - - - - - - -  //

   STM32_PWR PWR( APB1, PWR_BASE, STM32_RCC::APB1Periph_PWR, "PWR" );

//  - - - - - - - - - - -  //
//  B A C K U P   S R A M  //
//  - - - - - - - - - - -  //

   STM32_BKP BKP( AHB1, BKPSRAM_BASE, STM32_RCC::AHB1Periph_BKP, "BKP" );

//  - - - - - - - - - - - - - - -  //
//  D M A   C O N T R O L L E R S  //
//  - - - - - - - - - - - - - - -  //

   /*              bus   iobase     periph                      name */
   STM32_DMA DMA1( AHB1, DMA1_BASE, STM32_RCC::AHB1Periph_DMA1, "DMA1" );
   STM32_DMA DMA2( AHB1, DMA2_BASE, STM32_RCC::AHB1Periph_DMA2, "DMA2" );

//  - - - - - - - - - - -  //
//  D M A   S T R E A M S  //
//  - - - - - - - - - - -  //

   STM32_DMA::Stream DMA1ST2( DMA1, DMA1_Stream2_BASE, DMA1_Stream2_IRQn, "DMA1ST2", 0x10 );
   STM32_DMA::Stream DMA1ST5( DMA1, DMA1_Stream5_BASE, DMA1_Stream5_IRQn, "DMA1ST5", 0x26 );

   STM32_DMA::Stream DMA2ST5( DMA2, DMA2_Stream5_BASE, DMA1_Stream5_IRQn, "DMA2ST5", 0x26 );
   STM32_DMA::Stream DMA2ST7( DMA2, DMA2_Stream7_BASE, DMA1_Stream7_IRQn, "DMA2ST7", 0x36 );

//  - - - - - - - - -  //
//  G P I O   P I N S  //
//  - - - - - - - - -  //

   static STM32_GPIO::Pin GPIOA_Pins[ 16 ] = {
      STM32_GPIO::Pin( GPIOA,  0, "PA0",  0x0001 ),  STM32_GPIO::Pin( GPIOA,  1, "PA1",  0x0002 ),
      STM32_GPIO::Pin( GPIOA,  2, "PA2",  0x0004 ),  STM32_GPIO::Pin( GPIOA,  3, "PA3",  0x0008 ),
      STM32_GPIO::Pin( GPIOA,  4, "PA4",  0x0010 ),  STM32_GPIO::Pin( GPIOA,  5, "PA5",  0x0020 ),
      STM32_GPIO::Pin( GPIOA,  6, "PA6",  0x0040 ),  STM32_GPIO::Pin( GPIOA,  7, "PA7",  0x0080 ),
      STM32_GPIO::Pin( GPIOA,  8, "PA8",  0x0100 ),  STM32_GPIO::Pin( GPIOA,  9, "PA9",  0x0200 ),
      STM32_GPIO::Pin( GPIOA, 10, "PA10", 0x0400 ),  STM32_GPIO::Pin( GPIOA, 11, "PA11", 0x0800 ),
      STM32_GPIO::Pin( GPIOA, 12, "PA12", 0x1000 ),  STM32_GPIO::Pin( GPIOA, 13, "PA13", 0x2000 ),
      STM32_GPIO::Pin( GPIOA, 14, "PA14", 0x4000 ),  STM32_GPIO::Pin( GPIOA, 15, "PA15", 0x8000 )
   };

   static STM32_GPIO::Pin GPIOB_Pins[ 16 ] = {
      STM32_GPIO::Pin( GPIOB,  0, "PB0",  0x0001 ),  STM32_GPIO::Pin( GPIOB,  1, "PB1",  0x0002 ),
      STM32_GPIO::Pin( GPIOB,  2, "PB2",  0x0004 ),  STM32_GPIO::Pin( GPIOB,  3, "PB3",  0x0008 ),
      STM32_GPIO::Pin( GPIOB,  4, "PB4",  0x0010 ),  STM32_GPIO::Pin( GPIOB,  5, "PB5",  0x0020 ),
      STM32_GPIO::Pin( GPIOB,  6, "PB6",  0x0040 ),  STM32_GPIO::Pin( GPIOB,  7, "PB7",  0x0080 ),
      STM32_GPIO::Pin( GPIOB,  8, "PB8",  0x0100 ),  STM32_GPIO::Pin( GPIOB,  9, "PB9",  0x0200 ),
      STM32_GPIO::Pin( GPIOB, 10, "PB10", 0x0400 ),  STM32_GPIO::Pin( GPIOB, 11, "PB11", 0x0800 ),
      STM32_GPIO::Pin( GPIOB, 12, "PB12", 0x1000 ),  STM32_GPIO::Pin( GPIOB, 13, "PB13", 0x2000 ),
      STM32_GPIO::Pin( GPIOB, 14, "PB14", 0x4000 ),  STM32_GPIO::Pin( GPIOB, 15, "PB15", 0x8000 )
   };

   static STM32_GPIO::Pin GPIOC_Pins[ 16 ] = {
      STM32_GPIO::Pin( GPIOC,  0, "PC0",  0x0001 ),  STM32_GPIO::Pin( GPIOC,  1, "PC1",  0x0002 ),
      STM32_GPIO::Pin( GPIOC,  2, "PC2",  0x0004 ),  STM32_GPIO::Pin( GPIOC,  3, "PC3",  0x0008 ),
      STM32_GPIO::Pin( GPIOC,  4, "PC4",  0x0010 ),  STM32_GPIO::Pin( GPIOC,  5, "PC5",  0x0020 ),
      STM32_GPIO::Pin( GPIOC,  6, "PC6",  0x0040 ),  STM32_GPIO::Pin( GPIOC,  7, "PC7",  0x0080 ),
      STM32_GPIO::Pin( GPIOC,  8, "PC8",  0x0100 ),  STM32_GPIO::Pin( GPIOC,  9, "PC9",  0x0200 ),
      STM32_GPIO::Pin( GPIOC, 10, "PC10", 0x0400 ),  STM32_GPIO::Pin( GPIOC, 11, "PC11", 0x0800 ),
      STM32_GPIO::Pin( GPIOC, 12, "PC12", 0x1000 ),  STM32_GPIO::Pin( GPIOC, 13, "PC13", 0x2000 ),
      STM32_GPIO::Pin( GPIOC, 14, "PC14", 0x4000 ),  STM32_GPIO::Pin( GPIOC, 15, "PC15", 0x8000 )
   };

   /* aliases */

   STM32_GPIO::Pin& PA0  = GPIOA_Pins[  0 ];  STM32_GPIO::Pin& PA1  = GPIOA_Pins[  1 ];
   STM32_GPIO::Pin& PA2  = GPIOA_Pins[  2 ];  STM32_GPIO::Pin& PA3  = GPIOA_Pins[  3 ];
   STM32_GPIO::Pin& PA4  = GPIOA_Pins[  4 ];  STM32_GPIO::Pin& PA5  = GPIOA_Pins[  5 ];
   STM32_GPIO::Pin& PA6  = GPIOA_Pins[  6 ];  STM32_GPIO::Pin& PA7  = GPIOA_Pins[  7 ];
   STM32_GPIO::Pin& PA8  = GPIOA_Pins[  8 ];  STM32_GPIO::Pin& PA9  = GPIOA_Pins[  9 ];
   STM32_GPIO::Pin& PA10 = GPIOA_Pins[ 10 ];  STM32_GPIO::Pin& PA11 = GPIOA_Pins[ 11 ];
   STM32_GPIO::Pin& PA12 = GPIOA_Pins[ 12 ];  STM32_GPIO::Pin& PA13 = GPIOA_Pins[ 13 ];
   STM32_GPIO::Pin& PA14 = GPIOA_Pins[ 14 ];  STM32_GPIO::Pin& PA15 = GPIOA_Pins[ 15 ];

   STM32_GPIO::Pin& PB0  = GPIOB_Pins[  0 ];  STM32_GPIO::Pin& PB1  = GPIOB_Pins[  1 ];
   STM32_GPIO::Pin& PB2  = GPIOB_Pins[  2 ];  STM32_GPIO::Pin& PB3  = GPIOB_Pins[  3 ];
   STM32_GPIO::Pin& PB4  = GPIOB_Pins[  4 ];  STM32_GPIO::Pin& PB5  = GPIOB_Pins[  5 ];
   STM32_GPIO::Pin& PB6  = GPIOB_Pins[  6 ];  STM32_GPIO::Pin& PB7  = GPIOB_Pins[  7 ];
   STM32_GPIO::Pin& PB8  = GPIOB_Pins[  8 ];  STM32_GPIO::Pin& PB9  = GPIOB_Pins[  9 ];
   STM32_GPIO::Pin& PB10 = GPIOB_Pins[ 10 ];  STM32_GPIO::Pin& PB11 = GPIOB_Pins[ 11 ];
   STM32_GPIO::Pin& PB12 = GPIOB_Pins[ 12 ];  STM32_GPIO::Pin& PB13 = GPIOB_Pins[ 13 ];
   STM32_GPIO::Pin& PB14 = GPIOB_Pins[ 14 ];  STM32_GPIO::Pin& PB15 = GPIOB_Pins[ 15 ];

   STM32_GPIO::Pin& PC0  = GPIOC_Pins[  0 ];  STM32_GPIO::Pin& PC1  = GPIOC_Pins[  1 ];
   STM32_GPIO::Pin& PC2  = GPIOC_Pins[  2 ];  STM32_GPIO::Pin& PC3  = GPIOC_Pins[  3 ];
   STM32_GPIO::Pin& PC4  = GPIOC_Pins[  4 ];  STM32_GPIO::Pin& PC5  = GPIOC_Pins[  5 ];
   STM32_GPIO::Pin& PC6  = GPIOC_Pins[  6 ];  STM32_GPIO::Pin& PC7  = GPIOC_Pins[  7 ];
   STM32_GPIO::Pin& PC8  = GPIOC_Pins[  8 ];  STM32_GPIO::Pin& PC9  = GPIOC_Pins[  9 ];
   STM32_GPIO::Pin& PC10 = GPIOC_Pins[ 10 ];  STM32_GPIO::Pin& PC11 = GPIOC_Pins[ 11 ];
   STM32_GPIO::Pin& PC12 = GPIOC_Pins[ 12 ];  STM32_GPIO::Pin& PC13 = GPIOC_Pins[ 13 ];
   STM32_GPIO::Pin& PC14 = GPIOC_Pins[ 14 ];  STM32_GPIO::Pin& PC15 = GPIOC_Pins[ 15 ];

//  - - - - - - - - - - - - - - - -  //
//  G P I O   C O N T R O L L E R S  //
//  - - - - - - - - - - - - - - - -  //

   /*                bus   iobase      periph                      id  name     pins */
   STM32_GPIO GPIOA( AHB1, GPIOA_BASE, STM32_RCC::AHB1Periph_GPIOA, 0, "GPIOA", GPIOA_Pins );
   STM32_GPIO GPIOB( AHB1, GPIOB_BASE, STM32_RCC::AHB1Periph_GPIOB, 1, "GPIOB", GPIOB_Pins );
   STM32_GPIO GPIOC( AHB1, GPIOC_BASE, STM32_RCC::AHB1Periph_GPIOC, 2, "GPIOC", GPIOC_Pins );

//  - - - - - - - - - - - - - - - -  //
//  U A R T   C O N T R O L L E R S  //
//  - - - - - - - - - - - - - - - -  //

                  /* bus   iobase       periph                        name */
   STM32_UART UART1( APB2, USART1_BASE, STM32_RCC::APB2Periph_USART1, "UART1",
                     PA10,              /* rx pin        */
                     PA9,               /* tx pin        */
                     USART1_IRQn,       /* IRQ number    */
                     STM32_GPIO::UART1  /* alt. function */
   );

                  /* bus   iobase       periph                        name */
   STM32_UART UART2( APB1, USART2_BASE, STM32_RCC::APB1Periph_USART2, "UART2",
                     PA3,               /* rx pin        */
                     PA2,               /* tx pin        */
                     USART2_IRQn,       /* IRQ number    */
                     STM32_GPIO::UART2  /* alt. function */
   );

                   /* bus   iobase       periph                        name */
   STM32_UART UART6( APB2, USART6_BASE, STM32_RCC::APB2Periph_USART6, "UART6",
                     PC7,               /* rx pin        */
                     PC6,               /* tx pin        */
                     USART6_IRQn,       /* IRQ number    */
                     STM32_GPIO::UART6  /* alt. function */
   );

//  - - - - - - - - - - -  //
//  S P I   S T R E A M S  //
//  - - - - - - - - - - -  //

   /*
    *  See the STM32F4 reference manual for DMA stream/channel mappings,
    *  sec. 10.3.3 "Channel Selection", pp. 306-307
    */

   STM32_SPI::Stream SPI3_MISO( DMA1ST2, STM32_DMA::CH0, "SPI3MISO", PB4, STM32_GPIO::SPI3 );
   STM32_SPI::Stream SPI3_MOSI( DMA1ST5, STM32_DMA::CH0, "SPI3MOSI", PB5, STM32_GPIO::SPI3 );

//  - - - - - - - - - - - - - - -  //
//  S P I   C O N T R O L L E R S  //
//  - - - - - - - - - - - - - - -  //

   /*              bus   iobase     periph                      name */
   STM32_SPI SPI3( APB1, SPI3_BASE, STM32_RCC::APB1Periph_SPI3, "SPI3",
                   SPI3_MISO,         /* rx stream      */
                   SPI3_MOSI,         /* tx stream      */
                   PB3,               /* clock pin      */
                   STM32_GPIO::SPI3,  /* alt. function  */
                   STM32_SPI::DIV32   /* clock division */
   );

//  - - - - - - - - - - - - - -  //
//  O N B O A R D   M E M O R Y  //
//  - - - - - - - - - - - - - -  //

   /*              SPI    name    csPin */
   A25Lxxx FLASH0( SPI3, "FLASH0", PA0 /*CS6*/ );  /* U9 - "mémoire soft"    */
   A25Lxxx FLASH1( SPI3, "FLASH1", PA7 /*CS4*/ );  /* U5 - "mémoire données" */

   FlashCache FCH0( "FCH0", FLASH0 );
   FlashCache FCH1( "FCH1", FLASH1 );

//  - - - - - - - - - - - - - - -  //
//  A D C   C O N T R O L L E R S  //
//  - - - - - - - - - - - - - - -  //

   /*            SPI    name  csPin */
   MAX111x ADC1( SPI3, "ADC1", PA4 /*CS1*/ ); /* ADC carte energie (U5@PMU)   */
   MAX111x ADC2( SPI3, "ADC2", PA5 /*CS2*/ ); /* ADC carte energie (U6@PMU)   */
   MAX111x ADC3( SPI3, "ADC3", PA6 /*CS3*/ ); /* ADC carte ODB     (U7@ODB)   */
   MAX111x ADC4( SPI3, "ADC4", PC4 /*CS5*/ ); /* ADC carte FiPEX   (U2@FiPEX) */

//  - - - - - - - - - - - -  //
//  A D C   C H A N N E L S  //
//  - - - - - - - - - - - -  //

   static MAX111x::Channel ADC1_Channels[ 8 ] = {
      MAX111x::Channel( ADC1, "ADC1CH0", MAX111x::CH0 /*, 4.404494*/ ), /* V4 */
      MAX111x::Channel( ADC1, "ADC1CH1", MAX111x::CH1 /*, 0.266667*/ ), /* I4 */
      MAX111x::Channel( ADC1, "ADC1CH2", MAX111x::CH2 /*, 2.0     */ ), /* T4 */
      MAX111x::Channel( ADC1, "ADC1CH3", MAX111x::CH3 /*, 4.404494*/ ), /* V1 */
      MAX111x::Channel( ADC1, "ADC1CH4", MAX111x::CH4 /*, 2.0     */ ), /* T1 */
      MAX111x::Channel( ADC1, "ADC1CH5", MAX111x::CH5 /*, 0.266667*/ ), /* I1 */
      MAX111x::Channel( ADC1, "ADC1CH6", MAX111x::CH6 /*, 0.2     */ ), /* T_Bat */
      MAX111x::Channel( ADC1, "ADC1CH7", MAX111x::CH7 /*, 4.404494*/ )  /* V_Bat */
   };

   static MAX111x::Channel ADC2_Channels[ 8 ] = {
      MAX111x::Channel( ADC2, "ADC2CH0", MAX111x::CH0 /*, 4.404494*/ ), /* V2 */
      MAX111x::Channel( ADC2, "ADC2CH1", MAX111x::CH1 /*, 2.0     */ ), /* T2 */
      MAX111x::Channel( ADC2, "ADC2CH2", MAX111x::CH2 /*, 0.266667*/ ), /* I2 */
      MAX111x::Channel( ADC2, "ADC2CH3", MAX111x::CH3 /*, 4.404494*/ ), /* V3 */
      MAX111x::Channel( ADC2, "ADC2CH4", MAX111x::CH4 /*, 2.0     */ ), /* T3 */
      MAX111x::Channel( ADC2, "ADC2CH5", MAX111x::CH5 /*, 0.266667*/ ), /* I3 */
      MAX111x::Channel( ADC2, "ADC2CH6", MAX111x::CH6 /*, 0.2     */ ), /* I_surt */
      MAX111x::Channel( ADC2, "ADC2CH7", MAX111x::CH7 /*, 0.0     */ )  /* not used */
   };

   static MAX111x::Channel ADC3_Channels[ 8 ] = {
      MAX111x::Channel( ADC3, "ADC3CH0", MAX111x::CH0 /*, 0.17    */ ), /* I_ADCS */
      MAX111x::Channel( ADC3, "ADC3CH1", MAX111x::CH1 /*, 1.0     */ ), /* T_ARM_ODB */
      MAX111x::Channel( ADC3, "ADC3CH2", MAX111x::CH2 /*, 0.053333*/ ), /* I_RX */
      MAX111x::Channel( ADC3, "ADC3CH3", MAX111x::CH3 /*, 1.0     */ ), /* RSSI */
      MAX111x::Channel( ADC3, "ADC3CH4", MAX111x::CH4 /*, 0.4     */ ), /* I_TX */
      MAX111x::Channel( ADC3, "ADC3CH5", MAX111x::CH5 /*, 1.0     */ ), /* P_TX (mW) */
      MAX111x::Channel( ADC3, "ADC3CH6", MAX111x::CH6 /*, 1.0     */ ), /* P_PA (mW) */
      MAX111x::Channel( ADC3, "ADC3CH7", MAX111x::CH7 /*, 0.2     */ )  /* T_PA */
   };

   static MAX111x::Channel ADC4_Channels[ 8 ] = {
      MAX111x::Channel( ADC4, "ADC4CH0", MAX111x::CH0 /*, 0.0     */ ), /* not used (I_ADCS) */
      MAX111x::Channel( ADC4, "ADC4CH1", MAX111x::CH1 /*, 0.24    */ ), /* I_GPS */
      MAX111x::Channel( ADC4, "ADC4CH2", MAX111x::CH2 /*, 0.0     */ ), /* not used */
      MAX111x::Channel( ADC4, "ADC4CH3", MAX111x::CH3 /*, 0.026667*/ ), /* I_3V3_FIPEX */
      MAX111x::Channel( ADC4, "ADC4CH4", MAX111x::CH4 /*, 2.0     */ ), /* V_3V3_FIPEX */
      MAX111x::Channel( ADC4, "ADC4CH5", MAX111x::CH5 /*, 0.2424  */ ), /* I_5V_FIPEX */
      MAX111x::Channel( ADC4, "ADC4CH6", MAX111x::CH6 /*, 4.404494*/ ), /* V_5V_FIPEX */
      MAX111x::Channel( ADC4, "ADC4CH7", MAX111x::CH7 /*, 1.0     */ )  /* SU_TH_G0 */
   };

   /* aliases */

   MAX111x::Channel& ADC1CH0 = ADC1_Channels[ 0 ];  MAX111x::Channel& ADC1CH1 = ADC1_Channels[ 1 ];
   MAX111x::Channel& ADC1CH2 = ADC1_Channels[ 2 ];  MAX111x::Channel& ADC1CH3 = ADC1_Channels[ 3 ];
   MAX111x::Channel& ADC1CH4 = ADC1_Channels[ 4 ];  MAX111x::Channel& ADC1CH5 = ADC1_Channels[ 5 ];
   MAX111x::Channel& ADC1CH6 = ADC1_Channels[ 6 ];  MAX111x::Channel& ADC1CH7 = ADC1_Channels[ 7 ];

   MAX111x::Channel& ADC2CH0 = ADC2_Channels[ 0 ];  MAX111x::Channel& ADC2CH1 = ADC2_Channels[ 1 ];
   MAX111x::Channel& ADC2CH2 = ADC2_Channels[ 2 ];  MAX111x::Channel& ADC2CH3 = ADC2_Channels[ 3 ];
   MAX111x::Channel& ADC2CH4 = ADC2_Channels[ 4 ];  MAX111x::Channel& ADC2CH5 = ADC2_Channels[ 5 ];
   MAX111x::Channel& ADC2CH6 = ADC2_Channels[ 6 ];  MAX111x::Channel& ADC2CH7 = ADC2_Channels[ 7 ];

   MAX111x::Channel& ADC3CH0 = ADC3_Channels[ 0 ];  MAX111x::Channel& ADC3CH1 = ADC3_Channels[ 1 ];
   MAX111x::Channel& ADC3CH2 = ADC3_Channels[ 2 ];  MAX111x::Channel& ADC3CH3 = ADC3_Channels[ 3 ];
   MAX111x::Channel& ADC3CH4 = ADC3_Channels[ 4 ];  MAX111x::Channel& ADC3CH5 = ADC3_Channels[ 5 ];
   MAX111x::Channel& ADC3CH6 = ADC3_Channels[ 6 ];  MAX111x::Channel& ADC3CH7 = ADC3_Channels[ 7 ];

   MAX111x::Channel& ADC4CH0 = ADC4_Channels[ 0 ];  MAX111x::Channel& ADC4CH1 = ADC4_Channels[ 1 ];
   MAX111x::Channel& ADC4CH2 = ADC4_Channels[ 2 ];  MAX111x::Channel& ADC4CH3 = ADC4_Channels[ 3 ];
   MAX111x::Channel& ADC4CH4 = ADC4_Channels[ 4 ];  MAX111x::Channel& ADC4CH5 = ADC4_Channels[ 5 ];
   MAX111x::Channel& ADC4CH6 = ADC4_Channels[ 6 ];  MAX111x::Channel& ADC4CH7 = ADC4_Channels[ 7 ];

//  - - - - -  //
//  A X . 2 5  //
//  - - - - -  //

   //AX25Out ax25( PC9, PC8 );

//  - - -  //
//  O D B  //
//  - - -  //

   ODB SAT( "QB50", PC11, PC1, PA11, PA15 );

} /* qb50 */


extern "C" void DMA1_Stream2_IRQHandler( void )
{ qb50::DMA1ST2.isr(); }

extern "C" void DMA1_Stream5_IRQHandler( void )
{ qb50::DMA1ST5.isr(); }

extern "C" void DMA2_Stream5_IRQHandler( void )
{ qb50::DMA2ST5.isr(); }

extern "C" void DMA2_Stream7_IRQHandler( void )
{ qb50::DMA2ST7.isr(); }

extern "C" void SPI3_IRQHandler( void )
{ qb50::SPI3.isr(); }

extern "C" void USART1_IRQHandler( void )
{ qb50::UART1.isr(); }

extern "C" void USART2_IRQHandler( void )
{ qb50::UART2.isr(); }

extern "C" void USART6_IRQHandler( void )
{ qb50::UART6.isr(); }


/*EoF*/

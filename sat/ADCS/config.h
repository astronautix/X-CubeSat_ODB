
#ifndef _QB50_ADCS_CONFIG_H
#define _QB50_ADCS_CONFIG_H

#include "system/qb50.h"
#include "ADCSState.h"


namespace qb50 {

//  - - - - - - - - - - - - -  //
//  S Y S T E M   L O G G E R  //
//  - - - - - - - - - - - - -  //

   extern qb50::Syslog SYSLOG;

//  - - - - - - - - - - - - - - - - - - - - - -  //
//  R E S E T / C L O C K   C O N T R O L L E R  //
//  - - - - - - - - - - - - - - - - - - - - - -  //

   extern qb50::STM32_RCC RCC;

//  - - - - - - - - - - - - - - - - - - - -  //
//  I N T E R R U P T   C O N T R O L L E R  //
//  - - - - - - - - - - - - - - - - - - - -  //

   extern qb50::STM32_NVIC NVIC;

//  - - - - - - - - - - -  //
//  B A C K U P   S R A M  //
//  - - - - - - - - - - -  //

   extern qb50::STM32_BKP BPK;

//  - - - - - - - - - - - - - - -  //
//  D M A   C O N T R O L L E R S  //
//  - - - - - - - - - - - - - - -  //

   extern qb50::STM32_DMA DMA1;
   extern qb50::STM32_DMA DMA2;

//  - - - - - - - - - - -  //
//  D M A   S T R E A M S  //
//  - - - - - - - - - - -  //

   extern qb50::STM32_DMA::Stream DMA1ST0;
   extern qb50::STM32_DMA::Stream DMA1ST1;
   extern qb50::STM32_DMA::Stream DMA1ST2;
   extern qb50::STM32_DMA::Stream DMA1ST3;
   extern qb50::STM32_DMA::Stream DMA1ST4;
   extern qb50::STM32_DMA::Stream DMA1ST5;
   extern qb50::STM32_DMA::Stream DMA1ST6;
   extern qb50::STM32_DMA::Stream DMA1ST7;

   extern qb50::STM32_DMA::Stream DMA2ST0;
   extern qb50::STM32_DMA::Stream DMA2ST1;
   extern qb50::STM32_DMA::Stream DMA2ST2;
   extern qb50::STM32_DMA::Stream DMA2ST3;
   extern qb50::STM32_DMA::Stream DMA2ST4;
   extern qb50::STM32_DMA::Stream DMA2ST5;
   extern qb50::STM32_DMA::Stream DMA2ST6;
   extern qb50::STM32_DMA::Stream DMA2ST7;

//  - - - - - - - - -  //
//  G P I O   P I N S  //
//  - - - - - - - - -  //

   extern qb50::STM32_GPIO::Pin& PA0;   extern qb50::STM32_GPIO::Pin& PA1;
   extern qb50::STM32_GPIO::Pin& PA2;   extern qb50::STM32_GPIO::Pin& PA3;
   extern qb50::STM32_GPIO::Pin& PA4;   extern qb50::STM32_GPIO::Pin& PA5;
   extern qb50::STM32_GPIO::Pin& PA6;   extern qb50::STM32_GPIO::Pin& PA7;
   extern qb50::STM32_GPIO::Pin& PA8;   extern qb50::STM32_GPIO::Pin& PA9;
   extern qb50::STM32_GPIO::Pin& PA10;  extern qb50::STM32_GPIO::Pin& PA11;
   extern qb50::STM32_GPIO::Pin& PA12;  extern qb50::STM32_GPIO::Pin& PA13;
   extern qb50::STM32_GPIO::Pin& PA14;  extern qb50::STM32_GPIO::Pin& PA15;

   extern qb50::STM32_GPIO::Pin& PB0;   extern qb50::STM32_GPIO::Pin& PB1;
   extern qb50::STM32_GPIO::Pin& PB2;   extern qb50::STM32_GPIO::Pin& PB3;
   extern qb50::STM32_GPIO::Pin& PB4;   extern qb50::STM32_GPIO::Pin& PB5;
   extern qb50::STM32_GPIO::Pin& PB6;   extern qb50::STM32_GPIO::Pin& PB7;
   extern qb50::STM32_GPIO::Pin& PB8;   extern qb50::STM32_GPIO::Pin& PB9;
   extern qb50::STM32_GPIO::Pin& PB10;  extern qb50::STM32_GPIO::Pin& PB11;
   extern qb50::STM32_GPIO::Pin& PB12;  extern qb50::STM32_GPIO::Pin& PB13;
   extern qb50::STM32_GPIO::Pin& PB14;  extern qb50::STM32_GPIO::Pin& PB15;

   extern qb50::STM32_GPIO::Pin& PC0;   extern qb50::STM32_GPIO::Pin& PC1;
   extern qb50::STM32_GPIO::Pin& PC2;   extern qb50::STM32_GPIO::Pin& PC3;
   extern qb50::STM32_GPIO::Pin& PC4;   extern qb50::STM32_GPIO::Pin& PC5;
   extern qb50::STM32_GPIO::Pin& PC6;   extern qb50::STM32_GPIO::Pin& PC7;
   extern qb50::STM32_GPIO::Pin& PC8;   extern qb50::STM32_GPIO::Pin& PC9;
   extern qb50::STM32_GPIO::Pin& PC10;  extern qb50::STM32_GPIO::Pin& PC11;
   extern qb50::STM32_GPIO::Pin& PC12;  extern qb50::STM32_GPIO::Pin& PC13;
   extern qb50::STM32_GPIO::Pin& PC14;  extern qb50::STM32_GPIO::Pin& PC15;

   extern qb50::STM32_GPIO::Pin& F1;
   extern qb50::STM32_GPIO::Pin& R1;
   extern qb50::STM32_GPIO::Pin& F2;
   extern qb50::STM32_GPIO::Pin& R2;
   extern qb50::STM32_GPIO::Pin& F3;
   extern qb50::STM32_GPIO::Pin& R3;

//  - - - - - - - - - - - - - - - -  //
//  G P I O   C O N T R O L L E R S  //
//  - - - - - - - - - - - - - - - -  //

   extern qb50::STM32_GPIO GPIOA;
   extern qb50::STM32_GPIO GPIOB;
   extern qb50::STM32_GPIO GPIOC;

//  - - - - - - - - - - - - - - - -  //
//  U A R T   C O N T R O L L E R S  //
//  - - - - - - - - - - - - - - - -  //

   extern qb50::STM32_UART UART1;
   extern qb50::STM32_UART UART2;
   extern qb50::STM32_UART UART3;
   extern qb50::STM32_UART UART4;
   extern qb50::STM32_UART UART5;
   extern qb50::STM32_UART UART6;

//  - - - - - - - - - - - - - - -  //
//  S P I   C O N T R O L L E R S  //
//  - - - - - - - - - - - - - - -  //

   extern qb50::STM32_SPI SPI1;
   extern qb50::STM32_SPI SPI2;
   extern qb50::STM32_SPI SPI3;

//  - - - - - - - - - - - - - - -  //
//  A D C   C O N T R O L L E R S  //
//  - - - - - - - - - - - - - - -  //

   extern qb50::STM32_ADC ADC1;

//  - - - - - - - - - - - -  //
//  A D C   C H A N N E L S  //
//  - - - - - - - - - - - -  //

   extern qb50::STM32_ADC::Channel SUN1;
   extern qb50::STM32_ADC::Channel SUN2;
   extern qb50::STM32_ADC::Channel SUN3;
   extern qb50::STM32_ADC::Channel SUN4;
   extern qb50::STM32_ADC::Channel SUN5;
   extern qb50::STM32_ADC::Channel SUN6;

   /* aliases */

   extern qb50::STM32_ADC::Channel& GS1_1; // X front
   extern qb50::STM32_ADC::Channel& GS4_1; // X rear
   extern qb50::STM32_ADC::Channel& GS3_2; // Y front
   extern qb50::STM32_ADC::Channel& GS2_1; // Y rear
   extern qb50::STM32_ADC::Channel& GS5;   // Z front
   extern qb50::STM32_ADC::Channel& GS3_1; // Z rear

//  - - - - - - - - - - - - - -  //
//  O N B O A R D   M E M O R Y  //
//  - - - - - - - - - - - - - -  //

   extern qb50::A25Lxxx FLASH0;


//  - - - - - - - - - - - -  //
//  M A G N E T O M E T E R  //
//  - - - - - - - - - - - -  //

   extern qb50::LSM303 MAG0;


//  - - - - - - - - -  //
//  G Y R O S C O P E  //
//  - - - - - - - - -  //

   extern qb50::L3GD20 GYR0;


//  - - - - - - - - - - -  //
//        S T A T E        //
//  - - - - - - - - - - -  //

   extern qb50::ADCSState AST0;

} /* qb50 */


#endif /*_QB50_ADCS_CONFIG_H*/

/*EoF*/

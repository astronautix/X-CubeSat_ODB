
#ifndef _QB50_ODB_DEVICES_H
#define _QB50_ODB_DEVICES_H

#include "system/qb50.h"
#include "ODB.h"


namespace qb50 {

//  - - - - - - - - - - - - -  //
//  S Y S T E M   L O G G E R  //
//  - - - - - - - - - - - - -  //

   extern qb50::Syslog SYSLOG;

//  - - - - - - - - - - - - - - - - - - - - - -  //
//  R E S E T / C L O C K   C O N T R O L L E R  //
//  - - - - - - - - - - - - - - - - - - - - - -  //

   extern qb50::STM32_RCC RCC;

//  - - - - - - - -  //
//  W A T C H D O G  //
//  - - - - - - - -  //

   extern qb50::STM32_IWDG WDG;

//  - - - - - - - - - - - - - -  //
//  R T C   C O N T R O L L E R  //
//  - - - - - - - - - - - - - -  //

   extern qb50::STM32_RTC RTC0;

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

   extern qb50::STM32_DMA::Stream DMA1ST2;
   extern qb50::STM32_DMA::Stream DMA1ST5;

   extern qb50::STM32_DMA::Stream DMA2ST5;
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
   extern qb50::STM32_UART UART6;

//  - - - - - - - - - - - - - - -  //
//  S P I   C O N T R O L L E R S  //
//  - - - - - - - - - - - - - - -  //

   extern qb50::STM32_SPI SPI3;

//  - - - - - - - - - - - - - - -  //
//  A D C   C O N T R O L L E R S  //
//  - - - - - - - - - - - - - - -  //

   extern qb50::MAX111x ADC1;
   extern qb50::MAX111x ADC2;
   extern qb50::MAX111x ADC3;
   extern qb50::MAX111x ADC4;

//  - - - - - - - - - - - -  //
//  A D C   C H A N N E L S  //
//  - - - - - - - - - - - -  //

   extern qb50::MAX111x::Channel& ADC1CH0;  extern qb50::MAX111x::Channel& ADC1CH1;
   extern qb50::MAX111x::Channel& ADC1CH2;  extern qb50::MAX111x::Channel& ADC1CH3;
   extern qb50::MAX111x::Channel& ADC1CH4;  extern qb50::MAX111x::Channel& ADC1CH5;
   extern qb50::MAX111x::Channel& ADC1CH6;  extern qb50::MAX111x::Channel& ADC1CH7;

   extern qb50::MAX111x::Channel& ADC2CH0;  extern qb50::MAX111x::Channel& ADC2CH1;
   extern qb50::MAX111x::Channel& ADC2CH2;  extern qb50::MAX111x::Channel& ADC2CH3;
   extern qb50::MAX111x::Channel& ADC2CH4;  extern qb50::MAX111x::Channel& ADC2CH5;
   extern qb50::MAX111x::Channel& ADC2CH6;  extern qb50::MAX111x::Channel& ADC2CH7;

   extern qb50::MAX111x::Channel& ADC3CH0;  extern qb50::MAX111x::Channel& ADC3CH1;
   extern qb50::MAX111x::Channel& ADC3CH2;  extern qb50::MAX111x::Channel& ADC3CH3;
   extern qb50::MAX111x::Channel& ADC3CH4;  extern qb50::MAX111x::Channel& ADC3CH5;
   extern qb50::MAX111x::Channel& ADC3CH6;  extern qb50::MAX111x::Channel& ADC3CH7;

   extern qb50::MAX111x::Channel& ADC4CH0;  extern qb50::MAX111x::Channel& ADC4CH1;
   extern qb50::MAX111x::Channel& ADC4CH2;  extern qb50::MAX111x::Channel& ADC4CH3;
   extern qb50::MAX111x::Channel& ADC4CH4;  extern qb50::MAX111x::Channel& ADC4CH5;
   extern qb50::MAX111x::Channel& ADC4CH6;  extern qb50::MAX111x::Channel& ADC4CH7;

//  - - - - - - - - - - - - - -  //
//  O N B O A R D   M E M O R Y  //
//  - - - - - - - - - - - - - -  //

   extern qb50::A25Lxxx FLASH0;
   extern qb50::A25Lxxx FLASH1;

   extern qb50::FlashCache FCH0; // flash cache #0
   extern qb50::FlashCache FCH1; // flash cache #1

//  - - -  //
//  O D B  //
//  - - -  //

   extern qb50::ODB SAT;

} /* qb50 */


#endif /*_QB50_ODB_DEVICES_H*/

/*EoF*/

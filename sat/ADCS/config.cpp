
#include "config.h"
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

//  - - - - - - - - - - - - - - - - - - - -  //
//  I N T E R R U P T   C O N T R O L L E R  //
//  - - - - - - - - - - - - - - - - - - - -  //

   STM32_NVIC NVIC;

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
   STM32_DMA DMA2( AHB1, DMA1_BASE, STM32_RCC::AHB1Periph_DMA2, "DAM2" );

//  - - - - - - - - - - -  //
//  D M A   S T R E A M S  //
//  - - - - - - - - - - -  //

   STM32_DMA::Stream DMA1ST0( DMA1, DMA1_Stream0_BASE, DMA1_Stream0_IRQn, "DMA1ST0", 0x00 );
   STM32_DMA::Stream DMA1ST1( DMA1, DMA1_Stream1_BASE, DMA1_Stream1_IRQn, "DMA1ST1", 0x06 );
   STM32_DMA::Stream DMA1ST2( DMA1, DMA1_Stream2_BASE, DMA1_Stream2_IRQn, "DMA1ST2", 0x10 );
   STM32_DMA::Stream DMA1ST3( DMA1, DMA1_Stream3_BASE, DMA1_Stream3_IRQn, "DMA1ST3", 0x16 );
   STM32_DMA::Stream DMA1ST4( DMA1, DMA1_Stream4_BASE, DMA1_Stream4_IRQn, "DMA1ST4", 0x20 );
   STM32_DMA::Stream DMA1ST5( DMA1, DMA1_Stream5_BASE, DMA1_Stream5_IRQn, "DMA1ST5", 0x26 );
   STM32_DMA::Stream DMA1ST6( DMA1, DMA1_Stream6_BASE, DMA1_Stream6_IRQn, "DMA1ST6", 0x30 );
   STM32_DMA::Stream DMA1ST7( DMA1, DMA1_Stream7_BASE, DMA1_Stream7_IRQn, "DMA1ST7", 0x36 );

   STM32_DMA::Stream DMA2ST0( DMA2, DMA2_Stream0_BASE, DMA2_Stream0_IRQn, "DMA2ST0", 0x00 );
   STM32_DMA::Stream DMA2ST1( DMA2, DMA2_Stream1_BASE, DMA2_Stream1_IRQn, "DMA2ST1", 0x06 );
   STM32_DMA::Stream DMA2ST2( DMA2, DMA2_Stream2_BASE, DMA2_Stream2_IRQn, "DMA2ST2", 0x10 );
   STM32_DMA::Stream DMA2ST3( DMA2, DMA2_Stream3_BASE, DMA2_Stream3_IRQn, "DMA2ST3", 0x16 );
   STM32_DMA::Stream DMA2ST4( DMA2, DMA2_Stream4_BASE, DMA2_Stream4_IRQn, "DMA2ST4", 0x20 );
   STM32_DMA::Stream DMA2ST5( DMA2, DMA2_Stream5_BASE, DMA2_Stream5_IRQn, "DMA2ST5", 0x26 );
   STM32_DMA::Stream DMA2ST6( DMA2, DMA2_Stream6_BASE, DMA2_Stream6_IRQn, "DMA2ST6", 0x30 );
   STM32_DMA::Stream DMA2ST7( DMA2, DMA2_Stream7_BASE, DMA2_Stream7_IRQn, "DMA2ST7", 0x36 );

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

   /* aliases for coils */

   STM32_GPIO::Pin& F1 = PB14;
   STM32_GPIO::Pin& R1 = PB13;
   STM32_GPIO::Pin& F2 = PB12;
   STM32_GPIO::Pin& R2 = PB11;
   STM32_GPIO::Pin& F3 = PB10;
   STM32_GPIO::Pin& R3 = PA5;

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

   /*                bus   iobase       periph                        name    rxPin txPin  IRQ number   alt. function */
   STM32_UART UART1( APB2, USART1_BASE, STM32_RCC::APB2Periph_USART1, "UART1", PA10, PA9,  USART1_IRQn, STM32_GPIO::UART1 );
   STM32_UART UART6( APB2, USART6_BASE, STM32_RCC::APB2Periph_USART6, "UART6", PC7,  PC6,  USART6_IRQn, STM32_GPIO::UART6 );

//  - - - - - - - - - - -  //
//  S P I   S T R E A M S  //
//  - - - - - - - - - - -  //

   /*
    *  See the STM32F4 reference manual for STM32_DMA stream/channel mappings,
    *  sec. 10.3.3 "Channel Selection", pp. 306-307
    */

   STM32_SPI::Stream SPI3_MISO( DMA1ST2, STM32_DMA::CH0, "SPI3MISO", PB4, STM32_GPIO::SPI3 );
   STM32_SPI::Stream SPI3_MOSI( DMA1ST5, STM32_DMA::CH0, "SPI3MOSI", PB5, STM32_GPIO::SPI3 );

//  - - - - - - - - - - - - - - -  //
//  S P I   C O N T R O L L E R S  //
//  - - - - - - - - - - - - - - -  //

   /*        bus   iobase           periph                      name    rx stream  tx stream  clk   alt. func.        clk. div */
   STM32_SPI SPI3( APB1, SPI3_BASE, STM32_RCC::APB1Periph_SPI3, "SPI3", SPI3_MISO, SPI3_MOSI, PB3,  STM32_GPIO::SPI3, STM32_SPI::DIV16 );

//  - - - - - - - - - - - - - - -  //
//  A D C   C O N T R O L L E R S  //
//  - - - - - - - - - - - - - - -  //

   STM32_ADC ADC1( APB2, ADC1_BASE, STM32_RCC::APB2Periph_ADC1, "ADC1" );

//  - - - - - - - - - - - -  //
//  A D C   C H A N N E L S  //
//  - - - - - - - - - - - -  //

   /*                       adc   name    channel#         pin */
   STM32_ADC::Channel SUN1( ADC1, "SUN1", STM32_ADC::CH9,  PB1 );
   STM32_ADC::Channel SUN2( ADC1, "SUN2", STM32_ADC::CH10, PC0 );
   STM32_ADC::Channel SUN3( ADC1, "SUN3", STM32_ADC::CH13, PC3 );
   STM32_ADC::Channel SUN4( ADC1, "SUN4", STM32_ADC::CH15, PC5 );
   STM32_ADC::Channel SUN5( ADC1, "SUN5", STM32_ADC::CH12, PC2 );
   STM32_ADC::Channel SUN6( ADC1, "SUN6", STM32_ADC::CH4,  PA4 );

   /* aliases */

   STM32_ADC::Channel& GS1_1 = SUN1;
   STM32_ADC::Channel& GS4_1 = SUN2;
   STM32_ADC::Channel& GS3_2 = SUN3;
   STM32_ADC::Channel& GS2_1 = SUN4;
   STM32_ADC::Channel& GS5   = SUN5;
   STM32_ADC::Channel& GS3_1 = SUN6;

/*

   R1: PB13
   R2: PB11
   R3: PA5

 */


//  - - - - - - - - - - - - - -  //
//  O N B O A R D   M E M O R Y  //
//  - - - - - - - - - - - - - -  //

   /*              SPI   name     csPin */
   A25Lxxx FLASH0( SPI3, "FLASH0", PA15 );


//  - - - - - - - - - - - -  //
//  M A G N E T O M E T E R  //
//  - - - - - - - - - - - -  //

   LSM303 MAG0( SPI3, "MAG0", PC8 );


//  - - - - - - - - -  //
//  G Y R O S C O P E  //
//  - - - - - - - - -  //

   L3GD20 GYR0( SPI3, "GYR0", PC9 );


//  - - - - - - - - - - -  //
//        S T A T E        //
//  - - - - - - - - - - -  //

   ADCSState AST0;

} /* qb50 */


extern "C" void USART1_IRQHandler( void )
{ qb50::UART1.isr(); }

extern "C" void USART6_IRQHandler( void )
{ qb50::UART6.isr(); }


/*EoF*/

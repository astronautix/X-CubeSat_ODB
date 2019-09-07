
#ifndef _QB50_SYS_DEVICE_STM32_RCC_H
#define _QB50_SYS_DEVICE_STM32_RCC_H

#include "STM32/STM32_Device.h"


namespace qb50 {

   class STM32_RCC
   {

      public:

         static const uint32_t AHB1Periph_GPIOA        = 0x00000001;
         static const uint32_t AHB1Periph_GPIOB        = 0x00000002;
         static const uint32_t AHB1Periph_GPIOC        = 0x00000004;
         static const uint32_t AHB1Periph_GPIOD        = 0x00000008;
         static const uint32_t AHB1Periph_GPIOE        = 0x00000010;
         static const uint32_t AHB1Periph_GPIOF        = 0x00000020;
         static const uint32_t AHB1Periph_GPIOG        = 0x00000040;
         static const uint32_t AHB1Periph_GPIOH        = 0x00000080;
         static const uint32_t AHB1Periph_GPIOI        = 0x00000100;
         static const uint32_t AHB1Periph_CRC          = 0x00001000;
         static const uint32_t AHB1Periph_FLITF        = 0x00008000;
         static const uint32_t AHB1Periph_SRAM1        = 0x00010000;
         static const uint32_t AHB1Periph_SRAM2        = 0x00020000;
         static const uint32_t AHB1Periph_BKP          = 0x00040000;
         static const uint32_t AHB1Periph_CCMDATARAMEN = 0x00100000;
         static const uint32_t AHB1Periph_DMA1         = 0x00200000;
         static const uint32_t AHB1Periph_DMA2         = 0x00400000;
         static const uint32_t AHB1Periph_ETH_MAC      = 0x02000000;
         static const uint32_t AHB1Periph_ETH_MAC_Tx   = 0x04000000;
         static const uint32_t AHB1Periph_ETH_MAC_Rx   = 0x08000000;
         static const uint32_t AHB1Periph_ETH_MAC_PTP  = 0x10000000;
         static const uint32_t AHB1Periph_OTG_HS       = 0x20000000;
         static const uint32_t AHB1Periph_OTG_HS_ULPI  = 0x40000000;

         static const uint32_t AHB2Periph_DCMI         = 0x00000001;
         static const uint32_t AHB2Periph_CRYP         = 0x00000010;
         static const uint32_t AHB2Periph_HASH         = 0x00000020;
         static const uint32_t AHB2Periph_RNG          = 0x00000040;
         static const uint32_t AHB2Periph_OTG_FS       = 0x00000080;

         static const uint32_t AHB3Periph_FSMC         = 0x00000001;

         static const uint32_t APB1Periph_TIM2         = 0x00000001;
         static const uint32_t APB1Periph_TIM3         = 0x00000002;
         static const uint32_t APB1Periph_TIM4         = 0x00000004;
         static const uint32_t APB1Periph_TIM5         = 0x00000008;
         static const uint32_t APB1Periph_TIM6         = 0x00000010;
         static const uint32_t APB1Periph_TIM7         = 0x00000020;
         static const uint32_t APB1Periph_TIM12        = 0x00000040;
         static const uint32_t APB1Periph_TIM13        = 0x00000080;
         static const uint32_t APB1Periph_TIM14        = 0x00000100;
         static const uint32_t APB1Periph_WWDG         = 0x00000800;
         static const uint32_t APB1Periph_SPI2         = 0x00004000;
         static const uint32_t APB1Periph_SPI3         = 0x00008000;
         static const uint32_t APB1Periph_USART2       = 0x00020000;
         static const uint32_t APB1Periph_USART3       = 0x00040000;
         static const uint32_t APB1Periph_UART4        = 0x00080000;
         static const uint32_t APB1Periph_UART5        = 0x00100000;
         static const uint32_t APB1Periph_I2C1         = 0x00200000;
         static const uint32_t APB1Periph_I2C2         = 0x00400000;
         static const uint32_t APB1Periph_I2C3         = 0x00800000;
         static const uint32_t APB1Periph_CAN1         = 0x02000000;
         static const uint32_t APB1Periph_CAN2         = 0x04000000;
         static const uint32_t APB1Periph_PWR          = 0x10000000;
         static const uint32_t APB1Periph_DAC          = 0x20000000;
         // only for consistency (RTC has no peripheral bits)
         static const uint32_t APB1Periph_RTC          = 0x00000000;

         static const uint32_t APB2Periph_TIM1         = 0x00000001;
         static const uint32_t APB2Periph_TIM8         = 0x00000002;
         static const uint32_t APB2Periph_USART1       = 0x00000010;
         static const uint32_t APB2Periph_USART6       = 0x00000020;
         static const uint32_t APB2Periph_ADC          = 0x00000100;
         static const uint32_t APB2Periph_ADC1         = 0x00000100;
         static const uint32_t APB2Periph_ADC2         = 0x00000200;
         static const uint32_t APB2Periph_ADC3         = 0x00000400;
         static const uint32_t APB2Periph_SDIO         = 0x00000800;
         static const uint32_t APB2Periph_SPI1         = 0x00001000;
         static const uint32_t APB2Periph_SYSCFG       = 0x00004000;
         static const uint32_t APB2Periph_TIM9         = 0x00010000;
         static const uint32_t APB2Periph_TIM10        = 0x00020000;
         static const uint32_t APB2Periph_TIM11        = 0x00040000;

         struct Clocks
         {
            uint32_t SYSCLKFreq; /*!< SYSCLK clock frequency expressed in Hz */
            uint32_t   HCLKFreq; /*!<   HCLK clock frequency expressed in Hz */
            uint32_t  PCLK1Freq; /*!<  PCLK1 clock frequency expressed in Hz */
            uint32_t  PCLK2Freq; /*!<  PCLK2 clock frequency expressed in Hz */
         };

         static uint8_t pv[ 16 ];

         STM32_RCC( const uint32_t iobase, const char *name );
         ~STM32_RCC();

         STM32_RCC&  init    ( void );
         STM32_RCC&  enable  ( STM32_Device *dev, bool debug = false );
         STM32_RCC&  disable ( STM32_Device *dev, bool debug = false );

         STM32_RCC&  enableLSE  ( bool debug = false );

         STM32_RCC&  enableRTC  ( uint32_t clkSrc, bool debug = false );
         STM32_RCC&  disableRTC (                  bool debug = false );

         STM32_RCC&  clocks  ( Clocks *clk );
         uint32_t    freq    ( Bus& bus );
         bool        isPwrOn ( void );

      protected:

         enum ClockSource {
            HSI = 0,
            HSE = 1,
            PLL = 2
         };

         const uint32_t _iobase;
         const char    *_name;

         uint32_t       _csr;

   };

   extern qb50::STM32_RCC RCC;

}


#endif /*_QB50_SYS_DEVICE_STM32_RCC_H*/

/*EoF*/

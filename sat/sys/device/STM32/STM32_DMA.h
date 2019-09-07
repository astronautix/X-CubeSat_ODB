
#ifndef _QB50_SYS_DEVICE_STM32_DMA_H
#define _QB50_SYS_DEVICE_STM32_DMA_H

#include "STM32/STM32_Device.h"


namespace qb50 {

   class STM32_DMA : public STM32_Device
   {

      public:

         /* channel selection */

         enum Channel {
            CH0      = 0, /*!< channel 0 */
            CH1      = 1, /*!< channel 1 */
            CH2      = 2, /*!< channel 2 */
            CH3      = 3, /*!< channel 3 */
            CH4      = 4, /*!< channel 4 */
            CH5      = 5, /*!< channel 5 */
            CH6      = 6, /*!< channel 6 */
            CH7      = 7  /*!< channel 7 */
         };

         /* burst transfer configuration */

         enum Burst {
            SINGLE   = 0, /*!< single transfer               */
            INCR4    = 1, /*!< incremental burst of  4 beats */
            INCR8    = 2, /*!< incremental burst of  8 beats */
            INCR16   = 3  /*!< incremental burst of 16 beats */
         };

         /* current target */

         enum Target {
            MEM0 = 0, /*!< target memory is memory 0 */
            MEM1 = 1  /*!< target memory is memory 1 */
         };

        /* priority level */

         enum Priority {
            LOW      = 0, /*!< low priority       */
            MEDIUM   = 1, /*!< medium priority    */
            HIGH     = 2, /*!< high priority      */
            VERYHIGH = 3  /*!< very high priority */
         };

         /* memory/peripheral data size */

         enum DataSize {
            BYTE     = 0, /*!< byte (8-bit)   */
            SHORT    = 1, /*!< short (16-bit) */
            WORD     = 2  /*!< word (32-bit)  */
         };

         /* address increment mode */

         enum IncMode {
            FIXED    = 0, /*!< address pointer is fixed */
            INCR     = 1  /*!< address pointer is incremented after each transfer */
         };

         /* circular mode */

         enum Mode {
            NORMAL   = 0, /*!< circular mode disabled */
            CIRCULAR = 1  /*!< circular mode enabled  */
         };

         /* transfer direction */

         enum Direction {
            P2M      = 0, /*!< peripheral to memory */
            M2P      = 1, /*!< memory to peripheral */
            M2M      = 2  /*!< memory to memory     */
         };

         #include "STM32_DMA_Stream.h"

         STM32_DMA( Bus& bus, const uint32_t iobase, const uint32_t periph, const char *name );
         ~STM32_DMA();

         STM32_DMA& init    ( void );
         STM32_DMA& enable  ( bool debug = false );
         STM32_DMA& disable ( bool debug = false );

   };

} /* qb50 */


extern "C" {
   void DMA1_Stream0_IRQHandler( void );
   void DMA1_Stream1_IRQHandler( void );
   void DMA1_Stream2_IRQHandler( void );
   void DMA1_Stream3_IRQHandler( void );
   void DMA1_Stream4_IRQHandler( void );
   void DMA1_Stream5_IRQHandler( void );
   void DMA1_Stream6_IRQHandler( void );
   void DMA1_Stream7_IRQHandler( void );

   void DMA2_Stream0_IRQHandler( void );
   void DMA2_Stream1_IRQHandler( void );
   void DMA2_Stream2_IRQHandler( void );
   void DMA2_Stream3_IRQHandler( void );
   void DMA2_Stream4_IRQHandler( void );
   void DMA2_Stream5_IRQHandler( void );
   void DMA2_Stream6_IRQHandler( void );
   void DMA2_Stream7_IRQHandler( void );
}


#endif /*_QB50_SYS_DEVICE_STM32_DMA_H*/

/*EoF*/

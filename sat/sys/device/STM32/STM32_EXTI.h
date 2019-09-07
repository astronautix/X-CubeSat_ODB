
#ifndef _QB50_SYS_STM32_EXTI_H
#define _QB50_SYS_STM32_EXTI_H

#include "STM32/STM32_GPIO.h"
#include "system/FIFO.hpp"
#include "Device.h"


namespace qb50 {

    class STM32_EXTIHandler;

    class STM32_EXTI
    {
        public:

            STM32_EXTI();
            ~STM32_EXTI();

            enum EXTIn {
                EXTI0  = 0,  EXTI1  = 1,
                EXTI2  = 2,  EXTI3  = 3,
                EXTI4  = 4,  EXTI5  = 5,
                EXTI6  = 6,  EXTI7  = 7,
                EXTI8  = 8,  EXTI9  = 9,
                EXTI10 = 10, EXTI11 = 11,
                EXTI12 = 12, EXTI13 = 13,
                EXTI14 = 14, EXTI15 = 15
            };

            enum Edge {
                RISING = 0, FALLING = 1, BOTH = 2
            };

            STM32_EXTI& init( void );

            void registerHandler( STM32_GPIO::Pin &Pin, STM32_EXTIHandler *handler, Edge edge = RISING );
            void isr( EXTIn i );


        private:

            STM32_EXTIHandler* _extiHandler[16];
    };

    class STM32_EXTIHandler
    {
        public:

            STM32_EXTIHandler()
            { ; }

            virtual ~STM32_EXTIHandler()
            { ; }

            virtual void handle( STM32_EXTI::EXTIn ) = 0;
    };

    extern qb50::STM32_EXTI EXTI;

} /* qb50 */


extern "C" {
    void     STM32_EXTI0_IRQHandler( void );
    void     STM32_EXTI1_IRQHandler( void );
    void     STM32_EXTI2_IRQHandler( void );
    void     STM32_EXTI3_IRQHandler( void );
    void     STM32_EXTI4_IRQHandler( void );
    void   STM32_EXTI9_5_IRQHandler( void );
    void STM32_EXTI15_10_IRQHandler( void );
};

#endif /*_QB50_SYS_STM32_EXTI_H*/

/*EoF*/

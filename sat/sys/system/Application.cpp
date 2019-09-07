
#include <FreeRTOS.h>
#include <task.h>
#include <cstdio>

#include "system/Application.h"


#ifdef __cplusplus
extern "C" {
#endif

void vApplicationTickHook( void );
void vApplicationIdleHook( void );
void vApplicationMallocFailedHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vAssertCalled( const char *file, unsigned line );
void HardFault_Handler( void ) __attribute__(( naked ));
void hard_fault_handler_c( uint32_t *argv );

#ifdef __cplusplus
}
#endif

void *operator new( size_t size )
{
   void *p;

   p = pvPortMalloc( size );
   return p;
}

void operator delete( void *p )
{
   vPortFree( p );
   p = NULL;
}

void vApplicationTickHook( void )
{ ; }

void vApplicationIdleHook( void )
{ ; }

void vApplicationMallocFailedHook( void )
{
   printf( "\r\n\r\n\033[31;1mvApplicationMallocFailedHook()\033[0m\r\n\r\n" );
   for( ;; );
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
   (void)pxTask;
   (void)pcTaskName;

   if( pcTaskName != 0 ) {
      printf( "\r\n\r\n\033[31;1mSTACK OVERVLOW [%s]\033[0m\r\n\r\n", pcTaskName );
   } else {
      printf( "\r\n\r\n\033[31;1mSTACK OVERVLOW\033[0m\r\n\r\n" );
   }

   for( ;; );
}

void vAssertCalled( const char *file, unsigned line )
{
   printf( "\r\n\r\n\033[31;1mASSERTION FAILED - %s:%d\033[0m\r\n\r\n", file, line );
   for( ;; );
}


/*
void HardFault_Handler( void )
{ for( ;; ); }
*/

// From Joseph Yiu, minor edits by FVH
// hard fault handler in C,
// with stack frame location as input parameter
// called from HardFault_Handler in file xxx.s
void hard_fault_handler_c( uint32_t *argv )
{
 volatile uint32_t r0;
 volatile uint32_t r1;
 volatile uint32_t r2;
 volatile uint32_t r3;
 volatile uint32_t r12;
 volatile uint32_t r14;
 volatile uint32_t r15;
 volatile uint32_t psr;

 r0  = argv[0];
 r1  = argv[1];
 r2  = argv[2];
 r3  = argv[3];
 r12 = argv[4];
 r14 = argv[5];
 r15 = argv[6];
 psr = argv[7];

 printf( "\r\n\r\n*** HARD FAULT HANDLER ***\r\n\r\n" );

 printf( "   R0: 0x%08lx\r\n", r0 );
 printf( "   R1: 0x%08lx\r\n", r1 );
 printf( "   R2: 0x%08lx\r\n", r2 );
 printf( "   R3: 0x%08lx\r\n", r3 );
 printf( "  R12: 0x%08lx\r\n", r12 );
 printf( "  R14: 0x%08lx (LR - return address)\r\n", r14 );
 printf( "  R15: 0x%08lx (PC - program counter)\r\n", r15 );
 printf( "  PSR: 0x%08lx\r\n", psr );
 printf( " BFAR: 0x%08lx\r\n", (*((volatile unsigned long *)(0xe000ed38))));
 printf( " CFSR: 0x%08lx\r\n", (*((volatile unsigned long *)(0xe000ed28))));
 printf( " HFSR: 0x%08lx\r\n", (*((volatile unsigned long *)(0xe000ed2c))));
 printf( " DFSR: 0x%08lx\r\n", (*((volatile unsigned long *)(0xe000ed30))));
 printf( " AFSR: 0x%08lx\r\n", (*((volatile unsigned long *)(0xe000ed3c))));
//printf( " SCB_SHCSR = %x\r\n", SCB->SHCSR );

 printf( "\r\nSYSTEM HALTED\r\n" );

 for( ;; );
}

void HardFault_Handler( void )
{
   __asm volatile
   (
       " tst lr, #4                                         \n"
       " ite eq                                             \n"
       " mrseq r0, msp                                      \n"
       " mrsne r0, psp                                      \n"
       " ldr r1, [r0, #24]                                  \n"
       " ldr r2, handler2_address_const                     \n"
       " bx r2                                              \n"
       " handler2_address_const: .word hard_fault_handler_c \n"
   );
}


namespace qb50 {


static void trampoline( void *x )
{
   Thread *thread = (Thread*)x;
   thread->run();
   vTaskDelete( NULL );
}


Thread *createThread( const char *name, ThreadWorker worker )
{
   Thread *thread = new SimpleThread( name, worker );
   registerThread( thread );

   return thread;
}


Thread* registerThread( Thread *thread )
{
   int rv = xTaskCreate(
      trampoline,
      thread->name,
      thread->stackDepth,
      thread,
      thread->priority,
      &thread->handle
   );

   if( rv != pdTRUE ) {
      printf( "\033[31;1mThread::registerThread( %p ) failed [%s]\033[0m\r\n", thread, thread->name );
      #if 0
      { throw 42; /* XXX */ }
      #endif
   }

   return thread;
}


void run( void )
{ ::vTaskStartScheduler(); }


void delay( unsigned ms )
{
   if( ms > portMAX_DELAY )
      ms = portMAX_DELAY;

   ::vTaskDelay( (const TickType_t)ms / portTICK_RATE_MS );
}


uint32_t ticks( void )
{
   uint32_t t = (uint32_t)::xTaskGetTickCount();
   return( t / portTICK_RATE_MS );
}


} /* qb50 */

/*EoF*/

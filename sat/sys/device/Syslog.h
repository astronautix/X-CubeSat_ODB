
#ifndef _QB50_SYS_SYSLOG_H
#define _QB50_SYS_SYSLOG_H

#include "system/Ring.hpp"
#include "device/UART.h"

#include <iostream>
#include <sstream>
#include <task.h>

#include <cstdio>
#include <cstdarg>


#define RED( MSG )    "\033[31;1m" MSG "\033[0m"
#define GREEN( MSG )  "\033[32;1m" MSG "\033[0m"
#define YELLOW( MSG ) "\033[33;1m" MSG "\033[0m"
#define WHITE( MSG )  "\033[1m" MSG "\033[0m"

#define kprintf SYSLOG.printf

#define kdebug( debug, ... ) \
 if( debug ) { SYSLOG.printf( __VA_ARGS__ ); }


namespace qb50 {

   class Syslog : public Device
   {

      public:

         Syslog( const char *name, UART& uart );
         virtual ~Syslog();

         Syslog& init    ( void );
         Syslog& enable  ( bool debug = false );
         Syslog& disable ( bool debug = false );

         Syslog& printf  ( const char *fmt, ... );

         void run( void );

      protected:

         Ring<4096>  _ring;
         uint8_t    *_line;
         UART&       _uart;

         xSemaphoreHandle _sem;
         TaskHandle_t     _ioTask;

   };

   extern qb50::Syslog SYSLOG;

} /*qb50*/


#endif /*_QB50_SYS_SYSLOG_H*/

/*EoF*/

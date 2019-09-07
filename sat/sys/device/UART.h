
#ifndef _QB50_SYS_DEVICE_UART_H
#define _QB50_SYS_DEVICE_UART_H

#include "system/DataSource.hpp"
#include "system/DataSink.hpp"
#include "Device.h"


namespace qb50 {

   class UART : public DataSource<uint8_t>, public DataSink<uint8_t>
   {

      public:

         UART()
         { ; }

         virtual ~UART()
         { ; }

         virtual size_t readLine ( uint8_t *x, size_t len, int toms = -1 ) = 0;

         virtual UART&  baudRate ( unsigned rate ) = 0;

   };

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_UART_H*/

/*EoF*/

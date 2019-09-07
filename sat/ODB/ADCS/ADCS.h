
#ifndef _QB50_ADCS_H
#define _QB50_ADCS_H

#include "system/DataSource.hpp"
#include "system/DataSink.hpp"
#include "common/Message.h"
#include "devices.h"


namespace qb50 {

   class ADCS : public Device, public DataSource<uint8_t>, public DataSink<uint8_t>
   {

      public:

         enum State {
            MEASURE = 0,
            CONTROL = 1
         };

         ADCS( const char *name, STM32_UART& uart, STM32_GPIO::Pin& enPin );
         virtual ~ADCS();

         ADCS& init    ( void );
         ADCS& enable  ( bool debug = false );
         ADCS& disable ( bool debug = false );

         ADCS& setMeas( const ADCSMeas *mp );
         ADCS& getMeas(       ADCSMeas *mp );

         State state   ( void     ) const;                 // get state
         State state   ( State st, bool debug = false );   // set state

         unsigned read  (       uint8_t *x, unsigned len, int toms = 0 );
         unsigned write ( const uint8_t *x, unsigned len, int toms = 0 );


      private:

         STM32_UART&      _uart;
         STM32_GPIO::Pin& _enPin;
         State            _state;

         ADCSMeas        *_mp;

   };

   extern qb50::ADCS ADCS0;

} /* qb50 */


#endif /*_QB50_ADCS_H*/

/*EoF*/

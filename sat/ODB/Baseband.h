
#ifndef _QB50_SYS_DEVICE_BASEBAND_H
#define _QB50_SYS_DEVICE_BASEBAND_H

#include "Device.h"
#include "GPIO.h"


namespace qb50 {

   class Baseband : public Device
   {

      public:

         enum Power { P0, P1, P2, P3, P4 };

         Baseband( const char *name, GPIO::Pin& enTXPin, GPIO::Pin& enPAPin );
         ~Baseband();

         Baseband& init    ( void );
         Baseband& enable  (          bool debug = false );
         Baseband& enable  ( Power p, bool debug = false );
         Baseband& disable (          bool debug = false );

         Baseband& power   ( Power p );
         uint8_t   pMask   ( void ) const;


      private:

         GPIO::Pin& _enTXPin;
         GPIO::Pin& _enPAPin;

         uint8_t _pMask;

   };

   extern qb50::Baseband BB;

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_BASEBAND_H*/

/*EoF*/

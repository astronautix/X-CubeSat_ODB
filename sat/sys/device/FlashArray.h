
#ifndef _QB50_SYS_DEVICE_FLASH_ARRAY_H
#define _QB50_SYS_DEVICE_FLASH_ARRAY_H

#include "Device.h"
#include "device/FlashMemory.h"


namespace qb50 {

   class FlashArray : public Device, public FlashMemory
   {

      public:

         FlashArray( const char *name, const int nSlaves, FlashMemory *slaves[] );
         ~FlashArray();

         /* FlashMemory interface */

         FlashArray& init        ( void );
         FlashArray& enable      ( bool debug = false );
         FlashArray& disable     ( bool debug = false );

         FlashArray& pageRead    ( uint32_t addr, void *x       );
         FlashArray& pageWrite   ( uint32_t addr, const void *x );
         FlashArray& sectorErase ( uint32_t addr                );
         FlashArray& sectorRead  ( uint32_t addr, void *x       );
         FlashArray& sectorWrite ( uint32_t addr, const void *x );
         FlashArray& blockErase  ( uint32_t addr                );

      protected:

         const unsigned _nSlaves;
         FlashMemory  **_slaves;

         uint32_t       _mask; /* mask for the address part    */
         unsigned       _shft; /* bits to shift for the chip # */

         unsigned log2  ( uint32_t u    );
         unsigned getSn ( uint32_t addr );

   };

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_FLASH_ARRAY_H*/

/*EoF*/

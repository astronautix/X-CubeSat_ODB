
#ifndef _QB50_SYS_DEVICE_FLASH_CACHE_H
#define _QB50_SYS_DEVICE_FLASH_CACHE_H

#include "Device.h"
#include "FlashMemory.h"


namespace qb50 {

   class FlashCache : public Device
   {

      public:

         FlashCache( const char *name, FlashMemory& mem );
         ~FlashCache();

         FlashCache& init    ( void );
         FlashCache& enable  ( bool debug = false );
         FlashCache& disable ( bool debug = false );

         FlashCache& clear   ( void );
         FlashCache& read    ( uint32_t addr,       void *x, size_t len );
         FlashCache& write   ( uint32_t addr, const void *x, size_t len, bool sync = false );


      protected:

         struct Header {
            uint32_t crc32;
            uint8_t  x[];
         };

         FlashMemory& _mem;   // slave FlashMemory

         uint32_t     _nhit;  // number of cache hits
         uint32_t     _nmis;  // number of cache misses
         bool         _drty;  // cache is dirty

         uint32_t     _base;  // address (slave) of the sector currently in cache
         uint32_t     _psiz;  // physical sector size
         uint32_t     _vsiz;  // virtual sector size (excl. headers)
         uint32_t     _vcap;  // storage capacity (excl. headers)
         uint8_t     *_buf;   // buffer (header + data)
         uint8_t     *_tmp;   // temporary buffer (write verification)

         void         _clear ( uint32_t paddr );

         uint32_t     _v2p   ( uint32_t vaddr );
         void         _load  ( uint32_t vaddr );
         void         _flush ( void );

   };

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_FLASH_CACHE_H*/

/*EoF*/

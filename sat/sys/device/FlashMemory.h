
#ifndef _QB50_SYS_DEVICE_FLASH_MEMORY_H
#define _QB50_SYS_DEVICE_FLASH_MEMORY_H

#include <cstdint>


namespace qb50 {

   class FlashMemory
   {

      public:

         FlashMemory()
         { ; }

         virtual ~FlashMemory()
         { ; }

         struct Geometry {
            uint16_t bpc;  /* blocks per chip   */
            uint16_t spb;  /* sectors per block */
            uint16_t pps;  /* pages per sector  */
            uint16_t bpp;  /* bytes per page    */

            Geometry() : bpc( 0 ), spb( 0 ), pps( 0 ), bpp( 0 )
            { ; }

            virtual ~Geometry()
            { ; }
         };

         virtual FlashMemory& init        ( void ) = 0;
         virtual FlashMemory& enable      ( bool debug = false ) = 0;
         virtual FlashMemory& disable     ( bool debug = false ) = 0;

         virtual FlashMemory& pageRead    ( uint32_t addr, void *x       ) = 0;
         virtual FlashMemory& pageWrite   ( uint32_t addr, const void *x ) = 0;
         virtual FlashMemory& sectorErase ( uint32_t addr                ) = 0;
         virtual FlashMemory& sectorRead  ( uint32_t addr, void *x       ) = 0;
         virtual FlashMemory& sectorWrite ( uint32_t addr, const void *x ) = 0;
         virtual FlashMemory& blockErase  ( uint32_t addr                ) = 0;

         FlashMemory& geometry( Geometry *geo )
         {
            geo->bpc = _geo.bpc;
            geo->spb = _geo.spb;
            geo->pps = _geo.pps;
            geo->bpp = _geo.bpp;

            return *this;
         }

         uint32_t chipSize( void )
         { return (uint32_t)_geo.bpc * blockSize(); }

         uint32_t blockSize( void )
         { return (uint32_t)_geo.spb * sectorSize(); }

         uint32_t sectorSize( void )
         { return (uint32_t)_geo.pps * pageSize(); }

         uint32_t pageSize( void )
         { return (uint32_t)_geo.bpp; }

      protected:

         Geometry _geo;

   };

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_FLASH_MEMORY_H*/

/*EoF*/

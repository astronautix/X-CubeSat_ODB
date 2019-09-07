
#ifndef _QB50_SYS_DEVICE_SPI_A25LXXX_H
#define _QB50_SYS_DEVICE_SPI_A25LXXX_H

#include "SPI/Device.h"
#include "FlashMemory.h"

#include <FreeRTOS.h>
#include <task.h>


namespace qb50 {

   class A25Lxxx : public SPI_Device, public FlashMemory
   {

      private:

         struct IOReq;

      public:

         A25Lxxx( SPI& spi, const char *name, GPIO::Pin& csPin );
         ~A25Lxxx();

         void run( void );

         /* FlashMemory interface */

         A25Lxxx& init        ( void );
         A25Lxxx& enable      ( bool debug = false );
         A25Lxxx& disable     ( bool debug = false );

         A25Lxxx& pageRead    ( uint32_t addr, void *x       );
         A25Lxxx& pageWrite   ( uint32_t addr, const void *x );
         A25Lxxx& sectorErase ( uint32_t addr                );
         A25Lxxx& sectorRead  ( uint32_t addr, void *x       );
         A25Lxxx& sectorWrite ( uint32_t addr, const void *x );
         A25Lxxx& blockErase  ( uint32_t addr                );

      private:

         /* type definition for A25Lxxx chips */

         struct A25LChip {
            const uint16_t sig;  /* JEDEC signature   */
            const uint16_t mask; /* signature mask    */
            const char    *name; /* chip name         */
            uint16_t       bpc;  /* blocks per chip   */
            uint16_t       spb;  /* sectors per block */
            uint16_t       pps;  /* pages per sector  */
            uint16_t       bpp;  /* bytes per page    */
         };

         static A25LChip chips[];

         /* read device id */

         struct RDIDResp {
            uint8_t z0;      /*!< dummy byte      */
            uint8_t manId;   /*!< Manufacturer ID */
            uint8_t memType; /*!< Memory Type     */
            uint8_t memCap;  /*!< Memory Capacity */
         } __attribute__(( packed ));

         /* read electronic manufacturer id */

         struct REMSResp {
            uint8_t z0;      /*!< dummy byte      */
            uint8_t z1;      /*!< dummy byte      */
            uint8_t z2;      /*!< dummy byte      */
            uint8_t z3;      /*!< dummy byte      */
            uint8_t manId;   /*!< Manufacturer ID */
            uint8_t devId;   /*!< Device ID       */
         } __attribute__(( packed ));

         /* read status register */

         struct RDSRResp {
            uint8_t z0;      /*!< dummy byte      */
            uint8_t sr;      /*!< status register */
         } __attribute__(( packed ));

         /* operations */

         void _SE   ( uint32_t addr );
         void _BE   ( uint32_t addr  );
         void _RDID ( RDIDResp *rdid );
         void _READ ( uint32_t addr, void * );
         void _PP   ( uint32_t addr, const void *x );
         void _WREN ( void );
         void _WRDI ( void );
         void _REMS ( REMSResp *rems );

         void _WIPWait( unsigned ms = 10 );

   };

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_SPI_A25LXXX_H*/

/*EoF*/

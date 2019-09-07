
#ifndef _QB50_SYSTEM_DEVICE_H
#define _QB50_SYSTEM_DEVICE_H

#include <FreeRTOS.h>
#include <semphr.h>


namespace qb50 {

   class Device
   {

      public:

         Device( const char *name );
         virtual ~Device();

         inline const char *name ( void ) const
         { return _name; }

         virtual Device& init    ( void )                = 0;
         virtual Device& enable  ( bool debug = false ) = 0;
         virtual Device& disable ( bool debug = false ) = 0;

         virtual Device& lock    ( void );
         virtual Device& unlock  ( void );

      protected:

         const char      *_name;     /**< device name               */
         unsigned         _refCount; /**< reference counter         */
         xSemaphoreHandle _devLock;  /**< global lock on the device */

         unsigned _incRef ( void );
         unsigned _decRef ( void );

   };

} /* qb50 */


#endif /*_QB50_SYSTEM_DEVICE_H*/

/*EoF*/

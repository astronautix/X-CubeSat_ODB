
#ifndef _QB50_ODB_H
#define _QB50_ODB_H

#include "STM32/STM32_GPIO.h"


namespace qb50 {

   class ODB
   {

      public:

         ODB( const char *name,
              STM32_GPIO::Pin& selPin,
              STM32_GPIO::Pin& inhPin,
              STM32_GPIO::Pin& asPin,
              STM32_GPIO::Pin& adPin );
         virtual ~ODB();

         ODB& init( void );

         enum SatSel {
            FR01 = 0, /* X-CubeSat (Polytechnique)   */
            FR05 = 1  /* SpaceCube (Mines ParisTech) */
         };

         SatSel id        ( void );
         bool   isInhibit ( void );

         enum AntState {
            RETRACTED = 0,
            DEPLOYED  = 1
         };

         AntState aState   ( void );
         AntState aDeploy  ( void );

         unsigned maI[4];
         unsigned mvV[4];
         unsigned maIRx;
         unsigned maITx;
         unsigned mvBat;
         float    dcBat;
         float    dcPA;


      private:

         const char       *_name;
         STM32_GPIO::Pin&  _selPin; /* satellite selection (in) */
         STM32_GPIO::Pin&  _inhPin; /* ihnibit pin         (in) */
         STM32_GPIO::Pin&  _asPin;  /* antenna status      (in) */
         STM32_GPIO::Pin&  _adPin;  /* antenna deploy     (out) */

   };

} /* qb50 */


#endif /*_QB50_ODB_H*/

/*EoF*/

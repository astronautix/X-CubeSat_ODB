
#ifndef _QB50_SYS_DEVICE_WDG_H
#define _QB50_SYS_DEVICE_WDG_H


namespace qb50 {

   class WDG
   {

      public:

         WDG()
         { ; }

         virtual ~WDG()
         { ; }

         virtual WDG& refresh( void ) = 0;

   };

} /* qb50 */


#endif /*_QB50_SYS_DEVICE_WDG_H*/

/*EoF*/

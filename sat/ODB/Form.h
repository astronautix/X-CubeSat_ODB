
#ifndef _QB50_ODB_FORM_H
#define _QB50_ODB_FORM_H

#include "Config.h"
#include "devices.h"

#include <cstdint>
#include <ctime>


namespace qb50 {

   struct CForm {
      int  argc;
      long argv[8];
   };

   struct HForm {
      RTC::Time tm;
   };

   struct PForm {
      Config::pid_t  pid;
      Config::pval_t pval;
   };

   struct FForm {
      int sn;
   };

   struct T1Form {
      long   sat;  /* satellite number                    */
      long   eyr;  /* epoch year                          */
      double edy;  /* epoch day of the year               */
      double d1m;  /* 1st. time derivative of mean motion */
      double d2m;  /* 2nd. time derivative of mean motion */
      double bdt;  /* BSTAR drag term                     */
   };

   struct T2Form {
      long   sat;  /* satellite number                    */
      long   rev;  /* revolution number                   */
      double inc;  /* inclination (degrees)               */
      double ran;  /* R.A.A.N (degrees)                   */
      double ecc;  /* eccentricity                        */
      double aop;  /* argument of perigee (degrees)       */
      double man;  /* mean anomaly (degrees)              */
      double mmo;  /* mean motion (revolutions per day)   */
   };

   struct Form
   {

      enum FormType {
         FORM_TYPE_0  = 0,
         FORM_TYPE_C  = 1,
         FORM_TYPE_H  = 2,
         FORM_TYPE_P  = 3,
         FORM_TYPE_F  = 4,
         FORM_TYPE_T  = 5,
         FORM_TYPE_T1 = 6,
         FORM_TYPE_T2 = 7
      };

      FormType type;

      union {
          CForm C;
          HForm H;
          PForm P;
          FForm F;
         T1Form T1;
         T2Form T2;
      };

   };

}


#endif /*_QB50_ODB_FORM_H*/

/*EoF*/

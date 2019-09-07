
#ifndef _QB50_SYS_TLE_H
#define _QB50_SYS_TLE_H


namespace qb50 {

   /* ripped & adapted from GPredict (sgp4sdp4.h) */

   struct TLE {
      double       epoch;       //!< Epoch Time in NORAD TLE format YYDDD.FFFFFFFF
      unsigned int epoch_year;  //!< Epoch: year
      unsigned int epoch_day;   //!< Epoch: day of year
      double       epoch_fod;   //!< Epoch: Fraction of day
      double       xndt2o;      //!< 1. time derivative of mean motion
      double       xndd6o;      //!< 2. time derivative of mean motion
      double       bstar;       //!< Bstar drag coefficient
      double       xincl;       //!< Inclination
      double       xnodeo;      //!< R.A.A.N.
      double       eo;          //!< Eccentricity
      double       omegao;      //!< argument of perigee
      double       xmo;         //!< mean anomaly
      double       xno;         //!< mean motion

      int          catnr;       //!< Catalogue Number
      int          elset;       //!< Element Set number
      int          revnum;      //!< Revolution Number at epoch

      /* values needed for squint calculations */
      double       xincl1;
      double       xnodeo1;
      double       omegao1;
   };

} /* qb50 */


#endif /*_QB50_SYS_TLE_H*/

/*EoF*/


#include "RTC.h"

using namespace qb50;


static const int _days[ 12 ] = {
   0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
};


//  - - - - - - - - - - - - - -  //
//  S T A T I C   M E T H O D S  //
//  - - - - - - - - - - - - - -  //

time_t RTC::conv( const Time &tm )
{
   unsigned dd =   tm.day;
   unsigned mm = ( tm.mon  - 1    ) % 12;
   unsigned yy = ( tm.year - 1970 ) % 130;

   if(( mm < 2 ) || (( yy + 2 ) % 4 ))
      --dd;

   time_t ts = ( yy * 365 + ( yy + 1 ) / 4 + _days[ mm ] + dd ) * 86400
             + tm.hour * 3600
             + tm.min  * 60
             + tm.sec;

   return ts;
}

/*EoF*/

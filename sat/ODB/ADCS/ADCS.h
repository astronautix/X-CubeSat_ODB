/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:23:10+02:00
 * @License: This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * @Copyright: Copyright (C) 2027 Olivier Piras  
 */
 
 
#ifndef _QB50_ADCS_H
#define _QB50_ADCS_H

#include "system/DataSource.hpp"
#include "system/DataSink.hpp"
#include "common/Message.h"
#include "devices.h"


namespace qb50 {

   class ADCS : public Device, public DataSource<uint8_t>, public DataSink<uint8_t>
   {

      public:

         enum State {
            MEASURE = 0,
            CONTROL = 1
         };

         ADCS( const char *name, STM32_UART& uart, STM32_GPIO::Pin& enPin );
         virtual ~ADCS();

         ADCS& init    ( void );
         ADCS& enable  ( bool debug = false );
         ADCS& disable ( bool debug = false );

         ADCS& setMeas( const ADCSMeas *mp );
         ADCS& getMeas(       ADCSMeas *mp );

         State state   ( void     ) const;                 // get state
         State state   ( State st, bool debug = false );   // set state

         unsigned read  (       uint8_t *x, unsigned len, int toms = 0 );
         unsigned write ( const uint8_t *x, unsigned len, int toms = 0 );


      private:

         STM32_UART&      _uart;
         STM32_GPIO::Pin& _enPin;
         State            _state;

         ADCSMeas        *_mp;

   };

   extern qb50::ADCS ADCS0;

} /* qb50 */


#endif /*_QB50_ADCS_H*/

/*EoF*/

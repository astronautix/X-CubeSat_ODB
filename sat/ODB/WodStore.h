/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:20:15+02:00
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
 
 
#ifndef _QB50_ODB_WOD_STORE_H
#define _QB50_ODB_WOD_STORE_H

#include "Device.h"
#include "device/FlashCache.h"


namespace qb50 {


   class WodStore : public Device
   {

      public:

         /*
            +-------------------------------+
            |              prev             |
            +-------+-------+---------------+
            | type  |  xxx  |      len      |
            +-------+-------+---------------+
            |             ticks             |
            +-------------------------------+
            |            VKI time           |
            +-------------------------------+
         */

         struct WEnt /* WOD Entry */
         {
            uint32_t prev;  // previous entry
            uint8_t  type;  // entry type
            uint8_t  xxx;   // reserved (zero)
            uint16_t len;   // payload length in bytes
            uint32_t ticks; // 1ms ticks since poweron
            uint32_t time;  // VKI time
         } __attribute__(( packed ));

         enum : uint32_t { NIL = (uint32_t)-1 };

         /*
            In an effort to make telemetry frames easier to identify for
            humans (visual inspection), entry types were defined in such
            a way that their base64-encoded representation yield a plain
            type prefix (i.e. ADCS -> 'A', Fipex -> 'F', Wodex -> 'W').
          */

         enum EntryType {
            ADCS  = 0x00, // 'A'
            FIPEX = 0x14, // 'F'
            WODEX = 0x58, // 'W'
            NONE  = 0xff
         };

         WodStore( const char *name, FlashCache &mem, unsigned idx );
         ~WodStore();

         WodStore& init    ( void );
         WodStore& enable  ( bool debug = false );
         WodStore& disable ( bool debug = false );

         WodStore& clear   ( void );

         WodStore& read    ( WEnt *wod, void *x );
         WodStore& write   ( WEnt *wod, const void *x, bool sync = false );

      private:

         const char  *_name;
         FlashCache&  _mem;
         unsigned     _idx;

         void _read  ( WEnt *wod, void *x );
         void _write ( WEnt *wod, const void *x, bool sync );

   };

   extern qb50::WodStore WOD0;
   extern qb50::WodStore WOD1;

} /*qb50*/

#endif /*_QB50_ODB_WOD_STORE_H*/

/*EoF*/

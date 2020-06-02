/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:22:27+02:00
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
 
 
#ifndef _QB50_ODB_AX25_MODEM_H
#define _QB50_ODB_AX25_MODEM_H

#include "Modem.h"
#include "system/E64.h"
#include "device/STM32/STM32_EXTI.h"


namespace qb50 {

   class AX25Modem : public Modem, public STM32_EXTIHandler, public E64::Dumper
   {

      public:

         AX25Modem( const char *name, STM32_GPIO::Pin& enPin, STM32_GPIO::Pin& ckPin, STM32_GPIO::Pin& txPin );
         virtual ~AX25Modem();

         AX25Modem&  init    ( void );
         AX25Modem&  enable  ( bool debug = false );
         AX25Modem&  disable ( bool debug = false );

         size_t      send    ( WodStore::WEnt *wod, const void *x, int toms = -1 );
         size_t      sendHex ( WodStore::WEnt *wod, const void *x, int toms = -1 );
         size_t      sendB64 ( WodStore::WEnt *wod, const void *x, int toms = -1 );
         size_t      send    ( const uint8_t  *x,   unsigned len,  int toms = -1 );

         AX25Modem&  mycall  ( const char *x, int ssid );
         AX25Modem&  unproto ( const char *x, int ssid );

         void handle( STM32_EXTI::EXTIn );

         void dump( const char *x, unsigned len, bool mf );


      private:

         uint16_t _crc16   ( const uint8_t *x, unsigned len, uint16_t crc = 0xffff );
         void     _push    ( const uint16_t w, int toms );

         void     _sendUI  ( const uint8_t *x, unsigned len,              int toms = -1 );
         void     _sendUIX ( const uint8_t *x, unsigned len, uint8_t pfx, int toms = -1 );
         void     _sendUIH ( const uint8_t *x, unsigned len,              int toms = -1 );
         void     _flush   ( void );

         STM32_GPIO::Pin& _enPin;   // enable pin
         STM32_GPIO::Pin& _ckPin;   // clock pin
         STM32_GPIO::Pin& _txPin;   // TX pin

         xSemaphoreHandle    _semTX;
         FIFO<uint16_t,1024> _fifo;

         uint8_t  _ohdr[ 16 ];
         uint8_t *_obuf;

         bool     _flag;
         uint8_t  _byte;
         uint8_t  _mask;
         uint8_t  _ones;
   };

   extern qb50::AX25Modem M9K6;
   extern qb50::AX25Modem M1K2;

} /* qb50 */


#endif /*_QB50_ODB_AX25_MODEM_H*/

/*EoF*/

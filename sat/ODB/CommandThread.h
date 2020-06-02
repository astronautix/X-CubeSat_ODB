/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:22:12+02:00
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
 
 
#ifndef _QB50_ODB_COMMAND_THREAD_H
#define _QB50_ODB_COMMAND_THREAD_H

#include "system/qb50.h"
#include "Form.h"
#include "Event.h"


namespace qb50 {

   class CommandThread : public Thread
   {

      public:

         CommandThread();
         ~CommandThread();

         void run( void );

      protected:

         int _parseForm     ( void );

         int _parseCForm    ( void );
         int _parseHForm    ( void );
         int _parsePForm    ( void );
         int _parseFForm    ( void );
#if 0
         int _parseTForm    ( void );
         int _parseT1Form   ( void );
         int _parseT2Form   ( void );
#endif

         int _parseHex      ( uint8_t *x, size_t len );
         int _parseInteger  ( long    &p );
         int _parseDecimal  ( double  &p );
         int _parseDouble   ( double  &p );
         int _parseSign     ( int     &p );
#if 0
         int _parseNoradSci ( double &p, size_t off, size_t len );
         int _parseNoradExp ( double &p, size_t off, size_t len );
#endif
         int _checkTLESum   ( const uint8_t *x, size_t& off, size_t n );

         uint8_t       *_c;
         uint8_t       *_x;
         mBuf<uint8_t>  _m;

         Form _form;

   };

} /* qb50 */


#endif /*_QB50_ODB_COMMAND_THREAD_H*/

/*EoF*/

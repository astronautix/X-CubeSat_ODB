/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:22:25+02:00
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
 
 
#include "devices.h"
#include "Baseband.h"

using namespace qb50;


Baseband qb50::BB( "BB", PB15, PB13 ); // global Baseband object


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

Baseband::Baseband( const char *name, GPIO::Pin& enTXPin, GPIO::Pin& enPAPin )
   : Device( name ), _enTXPin( enTXPin ), _enPAPin( enPAPin ), _pMask( 0x00 )
{ ; }


Baseband::~Baseband()
{ ; }


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

Baseband& Baseband::init( void )
{
   kprintf( "%s: AMSAT-F Baseband Interface (F6FAO), enTX: %s, enPA: %s\r\n",
            _name, _enTXPin.name(), _enPAPin.name() );

   _enTXPin . out() . off();  // TX
   _enPAPin . out() . off();  // PA
   PC3      . out() . off();  // ON/OFF_GPS
   PC5      . out() . off();  // P1
   PB0      . out() . off();  // P2
   PB1      . out() . off();  // P3
   PA1      . out() . off();  // P4

   return *this;
}

#define POWPIN PC5

Baseband& Baseband::enable( bool debug )
{
   if( _incRef() > 0 )
      return *this;

   //unsigned p = CONF.getParam( Config::PARAM_WODEX_POWER );
   //POWPIN.on();

   _enTXPin.on();

   _enPAPin.on();
   if( debug ) {
      kprintf( "%s: enabled\r\n", _name );
   }

   return *this;
}


Baseband& Baseband::enable( Power p, bool debug )
{
   enable( debug );
   power( p );

   return *this;
}


Baseband& Baseband::disable( bool debug )
{
   if( _decRef() > 0 )
      return *this;

   MAX111x::Sample tpa;
   (void)ADC3CH7.read( &tpa );

   _enPAPin.off();
   _enTXPin.off();

   power( P0 );

   if( debug ) {
      if( tpa.stdev > 2.0 ) {
         kprintf( RED( "%s: T_PA not steady" ) "\r\n", _name );
      } else {
         float dK = 1.6 * tpa.value;
         float dC = dK - 273.15;
         kprintf( YELLOW( "%s: T_PA: %.2fdK - %.2fdC (raw: %u)" ) "\r\n", _name, dK, dC, tpa.value );
      }
      kprintf( "%s: disabled\r\n", _name );
   }

   return *this;
}


Baseband& Baseband::power( Power p )
{
   switch( p ) {

      case P0:
         PC5.off(); // P1
         PB0.off(); // P2
         PB1.off(); // P3
         PA1.off(); // P4
         _pMask = 0x00;
         break;

      case P1:
         PB0.off(); // P2
         PB1.off(); // P3
         PA1.off(); // P4
         PC5.on();  // P1 <-
         _pMask = 0x01;
         break;

      case P2:
         PC5.off(); // P1
         PB1.off(); // P3
         PA1.off(); // P4
         PB0.on();  // P2 <-
         _pMask = 0x02;
         break;

      case P3:
         PC5.off(); // P1
         PB0.off(); // P2
         PA1.off(); // P4
         PB1.on();  // P3 <-
         _pMask = 0x04;
         break;

      case P4:
         PC5.off(); // P1
         PB0.off(); // P2
         PB1.off(); // P3
         PA1.on();  // P4 <-
         _pMask = 0x08;
         break;

   }

   return *this;
}


uint8_t Baseband::pMask( void ) const
{
   return _pMask;
}


/*EoF*/

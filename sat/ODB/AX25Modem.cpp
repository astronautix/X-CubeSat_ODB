/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:22:29+02:00
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
#include "AX25Modem.h"
#include "Baseband.h"
#include "Config.h"

using namespace qb50;


AX25Modem qb50::M9K6( "M9K6", PC10, PC9,  PC8  ); // global Modem9600 object
AX25Modem qb50::M1K2( "M1K2", PC3,  PB11, PB10 ); // global Modem1200 object

#define BCHUNK 64


static const uint8_t _hexv[ 16 ] = {
   '0', '1', '2', '3', '4', '5', '6', '7',
   '8' ,'9', 'a', 'b', 'c', 'd', 'e', 'f'
};


static const uint16_t _crc16v[ 256 ] = {
   0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
   0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
   0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
   0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
   0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
   0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
   0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
   0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
   0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
   0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
   0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
   0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
   0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
   0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
   0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
   0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
   0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
   0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
   0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
   0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
   0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
   0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
   0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
   0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
   0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
   0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
   0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
   0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
   0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
   0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
   0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
   0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

AX25Modem::AX25Modem( const char *name, STM32_GPIO::Pin& enPin, STM32_GPIO::Pin& ckPin, STM32_GPIO::Pin& txPin )
   : Modem( name ), _enPin( enPin ), _ckPin( ckPin ), _txPin( txPin )
{
   _semTX = xSemaphoreCreateBinary();
   _flag  = false;
   _byte  = 0x00;
   _mask  = 0x00;
   _ones  = 0;
   _obuf  = new uint8_t[ 512 ];

   for( unsigned i = 0 ; i < sizeof( _ohdr ) ; ++i )
      _ohdr[ i ] = 0x00;
}


AX25Modem::~AX25Modem()
{
   delete[] _obuf;
   vSemaphoreDelete( _semTX );
}


//  - - - - - - -  //
//  M E T H O D S  //
//  - - - - - - -  //

AX25Modem& AX25Modem::init( void )
{
   _ckPin . in()  . noPull();   // clock
   _txPin . out() . off();      // data
   _enPin . out() . off();      // enable

   EXTI.registerHandler( _ckPin, this, STM32_EXTI::RISING );

   unproto( "TLM", 0 );

   switch( SAT.id() ) {
      case ODB::FR01:
         mycall( "ON01FR", 0 );
         break;

      case ODB::FR05:
         mycall( "ON05FR", 0 );
         break;
   }

   _ohdr[ 14 ] = 0x03; // Control
   _ohdr[ 15 ] = 0xf0; // PID

   kprintf( "%s: AX.25 Transmitter, enPin: %s, ckPin: %s, txPin: %s\r\n",
            _name, _enPin.name(), _ckPin.name(), _txPin.name() );

   return *this;
}


AX25Modem& AX25Modem::enable( bool debug )
{
   (void)debug;

   if( _incRef() > 0 )
      return *this;

   BB.enable();
   delay( 6500 );
   Baseband::Power p = (Baseband::Power)CONF.getParam( Config::PARAM_WODEX_POWER );
   BB.power( p );
   _enPin.on(); // ON_OFF_9600
   delay( 100 );

 //if( debug )
      kprintf( "%s: enabled\r\n", _name );

   return *this;
}


AX25Modem& AX25Modem::disable( bool debug )
{
   (void)debug;

   if( _decRef() > 0 )
      return *this;

   delay( 100 );
   _enPin.off(); // CLK_9600
   BB.disable();

 //if( debug )
      kprintf( "%s: disabled\r\n", _name );

   return *this;
}


AX25Modem& AX25Modem::mycall( const char *addr, int ssid )
{
   int i;
   uint8_t c;

   for( i = 0 ; i < 6 ; ++i ) {
      c = (uint8_t)addr[ i ];
      if( c == 0 ) break;
      _ohdr[ i + 7 ] = ( c << 1 );
   }

   for( ; i < 6 ; ++i )
      _ohdr[ i + 7 ] = 0x20 << 1;

   _ohdr[ 13 ] = (( ssid & 0x7f ) << 1 ) | 1;

   return *this;
}


AX25Modem& AX25Modem::unproto( const char *addr, int ssid )
{
   int i;
   uint8_t c;

   for( i = 0 ; i < 6 ; ++i ) {
      c = (uint8_t)addr[ i ];
      if( c == 0 ) break;
      _ohdr[ i ] = ( c << 1 );
   }

   for( ; i < 6 ; ++i )
      _ohdr[ i ] = 0x20 << 1;

   _ohdr[ 6 ] = ( ssid & 0x7f ) << 1;

   return *this;
}


size_t AX25Modem::send( WodStore::WEnt *wod, const void *x, int toms )
{
   Config::pval_t fmt = CONF.getParam( Config::PARAM_TELEM_FORMAT );

   if( fmt == 0 ) {
      return sendHex( wod, x, toms );
   } else {
      return sendB64( wod, x, toms );
   }
}


size_t AX25Modem::sendHex( WodStore::WEnt *wod, const void *x, int toms )
{
   struct tm stm;

   unsigned blen, off, on, i, n;
   uint8_t *o;
   char wt;

   switch( wod->type ) {
      case WodStore::WODEX:
         wt = '!';
         break;

      case WodStore::FIPEX:
         wt = '#';
         break;

      case WodStore::ADCS:
         wt = '%';
         break;

      default:
         wt = '?';
         break;
   }

   (void)gmtime_r( (const time_t*)&wod->time, &stm );

   off = 0;
   on  = 0;

   while( off < wod->len ) {

      blen = wod->len - off;
      if( blen > BCHUNK )
         blen = BCHUNK;

      n  = snprintf ( (char*)_obuf,     32,     "%c%02x", wt, CONF.nrst() & 0xff );
      n += strftime ( (char*)_obuf + n, 32 - n, "%y%m%d@%H%M%S;", &stm );

      if(( wod->type == WodStore::FIPEX ) || ( wod->len >= BCHUNK )) {
         n += snprintf( (char*)_obuf + n, 32 - n, "%01x%01x;", ( off >> 6 ) + 1, ( wod->len >> 6 ) + 1 );
      }

      o = _obuf + n;

      for( i = 0 ; i < blen ; ++i ) {
         *(o++) = _hexv[ ((const uint8_t*)x)[ off + i ] >> 4    ];
         *(o++) = _hexv[ ((const uint8_t*)x)[ off + i ]  & 0x0f ];
      }

      on  += send( _obuf, n + 2*i, toms );
      off += blen;
   }

   return on;
}


size_t AX25Modem::sendB64( WodStore::WEnt *wod, const void *x, int toms )
{
   (void)wod;
   (void)x;
   (void)toms;

   E64 e64;

   e64.push( (const uint8_t*)wod + 4, sizeof( WodStore::WEnt ) - 4 );
   e64.push( x, wod->len );
   e64.dump( this );

   return 0;
}


void AX25Modem::dump( const char *x, unsigned len, bool mf )
{
   if( mf ) {
      _sendUIX( (const uint8_t*)x, len, '+', 900 * 1000 /* = 15mn */);
   } else {
      _sendUIX( (const uint8_t*)x, len, '=', 900 * 1000 /* = 15mn */);
   }
}


size_t AX25Modem::send( const uint8_t *x, size_t len, int toms )
{
   _sendUI( x, len, toms );
   return len;
}


//  - - - - - - - - - - - - - - -  //
//  P R I V A T E   M E T H O D S  //
//  - - - - - - - - - - - - - - -  //

uint16_t AX25Modem::_crc16( const uint8_t *x, unsigned len, uint16_t crc )
{
   for( unsigned i = 0 ; i < len ; ++i )
      crc = ( crc >> 8 ) ^ _crc16v[ ( crc ^ x[i] ) & 0x00ff ];

   return crc;
}


void AX25Modem::_push( const uint16_t w, int toms )
{
   TickType_t tk = toms < 0 ? portMAX_DELAY : ( toms / portTICK_RATE_MS );

   while( _fifo.isFull() ) {
      xSemaphoreTake( _semTX, tk );
   }

   (void)_fifo.push( w );
}


void AX25Modem::_sendUI( const uint8_t *x, unsigned len, int toms )
{
   unsigned i;
   uint16_t fcs, w;

   if( len > 96 ) {
      kprintf( "%s: [%.*s ...]\r\n", _name, 96, x );
   } else {
      kprintf( "%s: [%.*s]\r\n", _name, len, x );
   }

   _ones = 0;

   /* start flags */

   for( i = 0 ; i < 10 ; ++i )
      _push( 0x017e, toms );

   /* header */

   for( i = 0 ; i < 16 ; ++i )
      _push( _ohdr[ i ], toms );

   /* data */

   for( i = 0 ; i < len ; ++i )
      _push( x[ i ], toms );

   /* FCS */

   fcs = _crc16( _ohdr, 16       );
   fcs = _crc16( x,     len, fcs );
   fcs ^= 0xffff;

   w = fcs & 0xff;
   _push( w, toms );

   w = ( fcs >> 8 ) & 0xff;
   _push( w, toms );

   /* end flag */

   _push( 0x017e, toms );

   _flush();
}


void AX25Modem::_sendUIX( const uint8_t *x, unsigned len, uint8_t pfx, int toms )
{
   unsigned i;
   uint16_t fcs, w;

   if( len > 96 ) {
      kprintf( "%s: [%.*s ...]\r\n", _name, 96, x );
   } else {
      kprintf( "%s: [%.*s]\r\n", _name, len, x );
   }

   _ones = 0;

   /* start flags */

   for( i = 0 ; i < 10 ; ++i )
      _push( 0x017e, toms );

   /* header */

   for( i = 0 ; i < 16 ; ++i )
      _push( _ohdr[ i ], toms );

   /* prefix */

   _push( pfx, toms );

   /* data */

   for( i = 0 ; i < len ; ++i )
      _push( x[ i ], toms );

   /* FCS */

   fcs = _crc16( _ohdr, 16       );
   fcs = _crc16( &pfx,  1,   fcs );
   fcs = _crc16( x,     len, fcs );
   fcs ^= 0xffff;

   w = fcs & 0xff;
   _push( w, toms );

   w = ( fcs >> 8 ) & 0xff;
   _push( w, toms );

   /* end flag */

   _push( 0x017e, toms );

   _flush();
}


void AX25Modem::_sendUIH( const uint8_t *x, unsigned len, int toms )
{
   uint8_t *h = new uint8_t[ 2 * len ];

   for( unsigned i = 0 ; i < len ; ++i ) {
      h[ 2*i     ] = _hexv[ x[ i ] >> 4    ];
      h[ 2*i + 1 ] = _hexv[ x[ i ]  & 0x0f ];
   }

   _sendUI( h, 2 * len, toms );

   delete[] h;
}


void AX25Modem::_flush( void )
{
   while( !_fifo.isEmpty() ) {
      delay( 100 );
   }
}


//  - - - - - - - - - - - -  //
//  E X T I   H A N D L E R  //
//  - - - - - - - - - - - -  //

void AX25Modem::handle( STM32_EXTI::EXTIn /* ignored */ )
{
   portBASE_TYPE hpTask = pdFALSE;
   uint16_t w;

   if( !_flag && ( _ones == 5 )) {
      _ones = 0;
      _txPin.toggle();
      return;
   }

   if( _mask == 0 ) {
      if( _fifo.isEmpty() ) {
         /* no data ? transmit flags */
         w = 0x017e;
      } else {
         w = _fifo.pull();
         (void)xSemaphoreGiveFromISR( _semTX, &hpTask );
      }

      /* check if flag */

      if(( w & 0x0100 ) != 0 ) {
         _flag = true;
         _ones = 0;
      } else {
         _flag = false;
      }

      _byte = w & 0xff;
      _mask = 0x01;
   }

   if(( _byte & _mask ) != 0 ) {
      ++_ones;
   } else {
      _ones = 0;
      _txPin.toggle();
   }

   _mask <<= 1;

   if( hpTask == pdTRUE )
      portEND_SWITCHING_ISR( hpTask );
}

/*EoF*/

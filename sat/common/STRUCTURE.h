/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:19:54+02:00
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
 
#ifndef STRUCTURE_H_INCLUDED
#define STRUCTURE_H_INCLUDED



typedef double real;

enum MsgTyp {
   T_QUAT   = 0,
   T_ROT    = 1,
   T_GPS=3,
   T_POS=2,
   T_control_attitude = 4,
   T_PING   = 5,
   T_TEMPS=6,
   T_MESURE=7,
   T_STOP=8,
} ;

struct ping{
MsgTyp typ=T_PING;
int p;
};

struct temps
{MsgTyp typ=T_TEMPS;
int t;
};


struct quaternion {
MsgTyp typ=T_QUAT;
real Q[4];
};

struct rotation {
MsgTyp typ=T_ROT;
real R[3];
};

struct gps{
MsgTyp typ=T_GPS;
real GPST [3];
};

struct position{
MsgTyp typ=T_POS;
real P[3];

};

struct message {
MsgTyp typ;
real  M[4];

struct quaternion quat;
struct rotation rot;
struct position pos;
struct ping pin;
struct temps tem;
struct gps gpss;
};

/*
struct temps t1;

t1.... = 0;

UART1.enable();

UART1.write( &t1, sizeof( t1 ));

UART1.read( void *x, size_t len )


size_t UART::write( const void *x, size_t len )
*/



#endif /* STRUCTURE_H_INCLUDED */

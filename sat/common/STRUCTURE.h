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

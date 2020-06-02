/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:19:24+02:00
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
 
 
#include "system/qb50.h"
#include "Control_attitude.h"
#include "ADCSCLASS.h"

#include <safe_stm32f4xx.h>

using namespace qb50;


adcs adcs1;
	message msg;
	signed int T;
	int p;
	unsigned int clk_0;
	signed int pos[3];





void controle_attitude(int period)
{


do{
adcs1.setADCSOn();                                                                                                                                      //1.	Mise de l�ADCS sous tension.
adcs1.InitADCS();

clk_0 =ticks();
do{

adcs1.writeADCSMessage(adcs1.M_PING);
msg=adcs1.readADCSmsg();                                                                                                                         //2.	V�rification que l�ADCS est fonctionnelle.
if (msg.typ== T_PING)
adcs1.PING=msg.pin;

if (adcs1.PING.p==1) break;

}


while((ticks()-clk_0)<=1000);


if (adcs1.PING.p==0)
		break;


T=adcs1.ReadExtMemorieint(&p);                                                                                         //3.	Envoyer le temps
adcs1.M_TEM.tem.t=T;
adcs1.writeADCSMessage(adcs1.M_TEM);


pos[0]=adcs1.ReadExtMemorieint(&p);                                                                                 //Envoyer les param�tres GPS.
pos[1]=adcs1.ReadExtMemorieint(&p);
pos[2]=adcs1.ReadExtMemorieint(&p);
for (int i=0;i<=2;i++) {adcs1.M_GPS.gpss.GPST[i]=pos[i];}
adcs1.writeADCSMessage(adcs1.M_GPS);





adcs1.writeADCSMessage(adcs1.M_control_attitude);	                               	                               //4.	activation du mode d�tumbling.

do{
clk_0=ticks();
adcs1.writeADCSMessage(adcs1.M_MES);                                                                         //5.	activation du mode de mesure.

mesure() ;                                                                                                                                  //6.	reception des parametres d'attitude.
delay(period-ticks()+clk_0);



} while (1);

	adcs1.writeADCSMessage(adcs1.M_STOP);	                //7.	Arr�t de tout calcul dans l�ADCS.


	adcs1.setADCSOff();				//8.	d�sactivation de l�ADCS.

}while(1);

}


void mesure ( )
{

 adcs1.writeADCSMessage(adcs1.M_QUAT);	                                                                   //6.	reception des parametres d'attitude.
msg=adcs1.readADCSmsg();
if(msg.typ==T_QUAT)
adcs1.QUAT=msg.quat;
//fair un save

adcs1.writeADCSMessage(adcs1.M_ROT);
msg=adcs1.readADCSmsg();
if(msg.typ==T_ROT)
adcs1.ROT=msg.rot;
//fair un save

adcs1.writeADCSMessage(adcs1.M_POS);
msg=adcs1.readADCSmsg();
if(msg.typ==T_POS)
adcs1.POS=msg.pos;
//fair un save
};


/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:19:22+02:00
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
 
#ifndef ADCSCLASS_H_INCLUDED
#define ADCSCLASS_H_INCLUDED



#include "STRUCTURE.h"

class adcs
{

public:
int p=0;
void setADCSOn();
void InitADCS();
message readADCSmsg();
void writeADCSMessage(message a);
void WriteExtMemorie(signed int A,signed int *B);
int ReadExtMemorieint(signed int *D);
void setADCSOff();
adcs();

quaternion QUAT;
rotation ROT;
ping PING;
position POS;
message M_control_attitude;
message M_PING;
message M_GPS;
message M_POS;
message M_TEM;
message M_MES;
message M_QUAT;
message M_ROT;
message M_STOP;



};
#endif /* ADCSCLASS_H_INCLUDED */


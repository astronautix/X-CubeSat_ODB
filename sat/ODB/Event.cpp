/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:21:37+02:00
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
 
 
#include "Event.h"

using namespace qb50;


const char *Event::events[] = {
   "GENERIC",
   "AD_SUCCESS", "AD_FAILURE", "VBAT_HIGH",  "VBAT_LOW",   "PA_HIGH",
   "PA_LOW",     "FORM",       "WOD_EMPTY",  "(unknown)",  "(unknown)",
   "CMD_1",      "CMD_2",      "CMD_3",      "CMD_4",      "CMD_5",
   "CMD_6",      "CMD_7",      "CMD_8",      "CMD_9",      "CMD_10",
   "CMD_11",     "CMD_12",     "CMD_13",     "CMD_14",     "CMD_15"
};


//  - - - - - - - - -  //
//  S T R U C T O R S  //
//  - - - - - - - - -  //

Event::Event( event_t type, Form *form )
   : type( type ), form( form )
{ ; }


Event::~Event()
{
   if(( type == FORM ) && ( form != 0 ))
      delete form;
}


/*EoF*/

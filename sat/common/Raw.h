/**
 * @Author: Olivier Piras
 * @Date:   2020-06-02T16:07:30+02:00
 * @Email:  qb50@oprs.eu
 * @Project: X-CubeSat
 * @Last modified by:   Olivier Piras
 * @Last modified time: 2020-06-02T16:19:39+02:00
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
 
#ifndef _QB50_COMMUN_RAW_H
#define _QB50_COMMUN_RAW_H

namespace qb50{

    struct mag_field_raw{
        double B_x_raw;
        double B_y_raw;
        double B_z_raw;

        double Temperature;
    };

    struct sun_sensor_raw{

        //Values from the nine sun-sensors
        double sun_raw[9];
    };

    struct gyr_raw{

        double gyr_x;
        double gyr_y;
        double gyr_z;
    };

    typedef struct {
        struct mag_field_raw MAGRAW;
        struct sun_sensor_raw SUNRAW;
        struct gyr_raw GYRRAW;
    } Sat_raw;

    extern qb50::Sat_raw Current_raw;
}

#endif /*_QB50_COMMUN_RAW_H*/

/*EoF*/

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

#ifndef _QB50_COMMUN_STATE_H
#define _QB50_COMMUN_STATE_H

#include "Message.h"

namespace qb50 {

    struct mag_field{
        double B_x;
        double B_y;
        double B_z;
    };


    struct sun_vector{
        double s_x;
        double s_y;
        double s_z;
    };

    struct position{
        double pos_x;
        double pos_y;
        double pos_z;
    };

    struct pwm_values{
        int x_dir;
        int y_dir;
        int z_dir;

        uint32_t duty_cycle_x;
        uint32_t duty_cycle_y;
        uint32_t duty_cycle_z;
    };

    struct quaternion{
        double Q[7];
    };

    struct test_adc{
        SensorSample<uint16_t> adc_temp[5];
    };

    struct moment_mag{
        double mu_x;
        double mu_y;
        double mu_z;
    };

    struct heure{
        uint16_t hours;
        uint16_t minutes;
        uint16_t seconds;

        uint16_t day;
        uint16_t month;
        uint16_t year;
    };

    typedef struct {
        struct mag_field MAGFIE;
        struct sun_vector SUNVEC;
        struct position POS;
        struct pwm_values PWM;
        struct quaternion QUAT;
        struct test_adc TADC;
        struct moment_mag MOMAG;
        enum   CmdID LCR;
    } Sat_state;

    extern qb50::Sat_state Current_state;
}


#endif /*_QB50_COMMUN_STATE_H*/

/*EoF*/

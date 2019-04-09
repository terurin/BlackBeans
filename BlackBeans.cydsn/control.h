#if 0
#pragma once
#ifndef __CONTROL_HEADER_GURAD__
#define __CONTROL_HEADER_GUARD__

#include <stddef.h>
#include <stdbool.h>
#include "dsptype.h"


//タイヤの制御を行う

q1516_t wheel_pulse_per_m(q1516_t radius_m,     //タイヤの直径[m]
                          q1516_t gear,         //ギア比[タイヤ/内部ギア]
                          int pulse_per_rocate  //内部ギア1周あたりのパルス数
);

typedef void (*driver_callback_t)(q1516_t);
typedef int32_t (*encoder_callback_t)();
/*
struct wheel {
    pid_controllor_t* controllor;
    driver_callback_t driver;    // non-NULL
    encoder_callback_t encoder;  // non-NULL
    q1516_t pulse_per_m;         // non-NULL
};

typedef struct wheel wheel_t;
//所有権はwheel_controllorに移動する
void wheel_init(wheel_t*,
                driver_callback_t driver,
                encoder_callback_t encoder,
                pid_controllor_t* controllor,
                q1516_t pulse_per_m);
q1516_t wheel_move_m(wheel_t*, q1516_t target_m);

//モーターの配置の記述用


*/
struct omniwheel {
    size_t size;
    wheel_t* wheels;
    wheel_layout_t* layouts;
};
typedef struct omniwheel omniwheel_t;

void omniwheel_init(omniwheel_t* omniwheel, wheel_t* wheels, wheel_layout_t* layouts, size_t n);
void omniwheel_move_m(omniwheel_t*, q1516_t vx, q1516_t vy, q1516_t rps);

#endif
#endif

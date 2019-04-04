#pragma once
#ifndef __CONTROL_HEADER_GURAD__
#define __CONTROL_HEADER_GUARD__

#include <stddef.h>
#include <stdbool.h>
#include "dsptype.h"

struct pid_controllor {
    uint32_t interval_us;
    q1516_t p, i, d;
    q1516_t result, last_per_us, sum;
};
typedef struct pid_controllor pid_controllor_t;

void pid_controllor_init(pid_controllor_t*, uint32_t interval_us, q1516_t p, q1516_t i, q1516_t d);

q1516_t pid_controllor_control(pid_controllor_t*, q1516_t error);
bool pid_controllor_overflow(const pid_controllor_t*, q1516_t limit);

//タイヤの制御を行う

q1516_t wheel_pulse_per_m(q1516_t radius_m,     //タイヤの直径[m]
                          q1516_t gear,         //ギア比[タイヤ/内部ギア]
                          int pulse_per_rocate  //内部ギア1周あたりのパルス数
);

typedef void (*driver_callback_t)(q1516_t);
typedef int32_t (*encoder_callback_t)();

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
struct wheel_layout {
    q1516_t radius_m,sin,cos;//ベクトル分解用
   
};
typedef struct wheel_layout wheel_layout_t;

void wheel_layout_init(wheel_layout_t*, float radius_m, float angle_rad);

struct omniwheel {
    size_t size;
    wheel_t* wheels;
    wheel_layout_t* layouts;
};
typedef struct omniwheel omniwheel_t;

void omniwheel_init(omniwheel_t* omniwheel, wheel_t* wheels, wheel_layout_t* layouts, size_t n);
void omniwheel_move_m(omniwheel_t*, q1516_t vx, q1516_t vy, q1516_t rps);

#endif

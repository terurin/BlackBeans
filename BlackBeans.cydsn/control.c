#include "control.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "clip.h"
#include "dspmath.h"
void pid_controllor_init(pid_controllor_t* pid,
                         q1516_t interval_us,
                         q1516_t p,
                         q1516_t i,
                         q1516_t d) {
    assert(pid);
    pid->interval_us = interval_us;
    pid->p = p;
    pid->i = i;
    pid->d = d;
    pid->last_per_us = 0;
    pid->sum = 0;
}

q1516_t pid_controllor_control(pid_controllor_t* pid, q1516_t error) {
    assert(pid);
    // preload
    const uint32_t interval_us = pid->interval_us;
    const q1516_t error_per_us = error / interval_us;
    const q1516_t sum = pid->sum + error;
    const q1516_t diff_per_us = (q4716_t)(pid->last_per_us - error_per_us) << 16 / pid->interval_us;
    // compute
    q3132_t result = pid->result;
    result += (q3132_t)pid->p * error_per_us;
    result += (q3132_t)pid->i * sum;
    result += (q3132_t)pid->d * diff_per_us;
    // update
    pid->sum = sum;
    pid->last_per_us = error_per_us;
    return pid->result = result >> 16;
}

bool pid_controllor_overflow(const pid_controllor_t* pid, q1516_t limit) {
    assert(pid);
    return abs32(pid->sum) > limit;
}

q1516_t wheel_pulse_per_m(q1516_t radius_m, q1516_t gear, int pulse) {
    const q1516_t pulse_per_rocate = pulse * gear;  //車輪一周あたりのパルス数
    const q1516_t pulse_per_rad =
        ((q4716_t)pulse_per_rocate << 16) / q1516_2pi;  // radあたりのパルス数
    const q1516_t pulse_per_m = pulse_per_rad * radius_m;
    return pulse_per_m;
}

void wheel_init(wheel_t* wheel,
                driver_callback_t driver,
                encoder_callback_t encoder,
                pid_controllor_t* controllor,
                q1516_t pulse_per_m) {
    assert(wheel);
    assert(driver);
    assert(encoder);
    assert(controllor);
    assert(pulse_per_m);

    wheel->driver = driver;
    wheel->encoder = encoder;
    wheel->controllor = controllor;
    wheel->pulse_per_m = pulse_per_m;
}

q1516_t wheel_move_m(wheel_t* wheel, q1516_t target_m) {
    assert(wheel);
    // error算出
    const int32_t feedback_pulse = wheel->encoder();
    const q1516_t feedback_m = ((q4716_t)feedback_pulse << 16) / wheel->pulse_per_m;
    const int error_m = target_m - feedback_m;
    //制御
    const q1516_t output = pid_controllor_control(wheel->controllor, error_m);
    const q1516_t output_cliped = (q1516_t)clip16(output);
    wheel->driver(output_cliped);
    //動作結果を算出する
    return feedback_m;
}

void wheel_layout_init(wheel_layout_t* wheel, float radius_m, float angle_rad) {
    assert(wheel);
    assert(radius_m > 0);
    wheel->radius_m = Q1516_CAST(radius_m);
    wheel->sin = Q1516_CAST(sinf(angle_rad));
    wheel->cos = Q1516_CAST(sinf(angle_rad));
}

void omniwheel_init(omniwheel_t* omniwheel, wheel_t* wheels, wheel_layout_t* layouts, size_t n) {
    assert(omniwheel);
    assert(wheels);
    assert(layouts);
    omniwheel->wheels = wheels;
    omniwheel->layouts = layouts;
}

void omniwheel_move(omniwheel_t* omni, q1516_t vx, q1516_t vy, q1516_t rps) {
    assert(omni);
    for (size_t i = 0; i < omni->size; i++) {
        wheel_layout_t* layout = &omni->layouts[i];
        q3132_t sum_m = 0;
        sum_m += (q3132_t)layout->sin * vy;
        sum_m += (q3132_t)layout->cos * vx;
        sum_m += (q3132_t)layout->radius_m * rps;
        wheel_move_m(&omni->wheels[i], clip32(sum_m>>16));
    }
}

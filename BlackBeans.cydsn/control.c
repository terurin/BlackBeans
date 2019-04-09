#if 0
#include "control.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "clip.h"
#include "dspmath.h"
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
q1516_t wheel_pulse_per_m(q1516_t radius_m, q1516_t gear, int pulse) {
    const q1516_t pulse_per_rocate = pulse * gear;  //車輪一周あたりのパルス数
    const q1516_t pulse_per_rad =
        ((q4716_t)pulse_per_rocate << 16) / q1516_2pi;  // radあたりのパルス数
    const q1516_t pulse_per_m = pulse_per_rad * radius_m;
    return pulse_per_m;
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

void omniwheel_init(omniwheel_t* omniwheel, wheel_t* wheels, wheel_layout_t* layouts, size_t n) {
    assert(omniwheel);
    assert(wheels);
    assert(layouts);
    omniwheel->wheels = wheels;
    omniwheel->layouts = layouts;
    omniwheel->size=n;
}

void omniwheel_move_m(omniwheel_t* omni, q1516_t vx, q1516_t vy, q1516_t rps) {
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

#endif
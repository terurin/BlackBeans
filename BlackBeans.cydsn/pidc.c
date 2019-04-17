#include "pidc.h"
#include <dspmath.h>
#include <assert.h>
void pid_controllor_init(pid_controllor_t*pid ,float p, float i, float d){
    assert(pid);
    pid->p = p*(1<<16);
    pid->i = i*(1<<16);
    pid->d = d*(1<<16);
    pid->last = 0;
    pid->sum = 0;
}

q1516_t pid_controllor_control(pid_controllor_t* pid, q1516_t error) {
    assert(pid);
    // preload
    const q1516_t sum = pid->sum + error;
    const q1516_t diff = (q4716_t)(pid->last - error) << 16;
    // compute
    q3132_t result = pid->result;
    result += (q3132_t)pid->p * error;
    result += (q3132_t)pid->i * sum;
    result += (q3132_t)pid->d * diff;
    // update
    pid->sum = sum;
    pid->last = error;
    return pid->result = result >> 16;
}

bool pid_controllor_overflow(const pid_controllor_t* pid, q1516_t limit) {
    assert(pid);
    return abs32(pid->sum) > limit;
}
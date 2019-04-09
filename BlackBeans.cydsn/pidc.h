#pragma once
#ifndef __TASKS_PID_CONTROLLOR_HEADER_GUARD__
#define __TASKS_PID_CONTROLLOR_HEADER_GUARD__
#include <stdint.h>
#include <stdbool.h>
#include <dsptype.h>
    
struct pid_controllor {
    q1516_t p, i, d;
    q1516_t result, last, sum;
};
typedef struct pid_controllor pid_controllor_t;

void pid_controllor_init(pid_controllor_t*,float p, float i, float d);

q1516_t pid_controllor_control(pid_controllor_t*, q1516_t error);
bool pid_controllor_overflow(const pid_controllor_t*, q1516_t limit);

    
#endif
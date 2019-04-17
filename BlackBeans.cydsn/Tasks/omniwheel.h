#pragma once
#ifndef __TASKS_OMNIWHEEL_HEADER_GUARD__
#define __TASKS_OMNIWHEEL_HEADER_GUARD__
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <dsptype.h>

struct wheel_layout {
    float distance_m,angle_rad;
};
typedef struct wheel_layout wheel_layout_t;


struct omniwheel{
    uint32_t interval_us;
    size_t wheel_size;
    void** wheel_objects;
    void (*wheel_target_mps)(void*,q1516_t);
    q1516_t (*wheel_feedback_mps)(void*);
    q1516_t** divide;//3x[size]
    q1516_t*** marges;//[size-2] 3x3
};
typedef struct omniwheel omniwheel_t;
    
void omniwheel_init(omniwheel_t*,uint32_t inverval_us,wheel_layout_t*,size_t);
void omniwheel_assign(omniwheel_t*,
    void (*wheel_target_mps)(void*,q1516_t),
    q1516_t (*wheel_feedback_mps)(void*),
    void** wheel_objects,size_t size);
void omniwheel_lanch(omniwheel_t*,const char*name,size_t stack,size_t priority);
void omniwheel_process(omniwheel_t*);
#endif 
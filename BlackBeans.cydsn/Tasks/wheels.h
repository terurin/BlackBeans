#pragma once
#ifndef __WHEELS_HEADER_GUARD__
#define __WHEELS_HEADER_GUARD__
    
#include <stdint.h>
#include <stdbool.h>

#include <dsptype.h>
    
struct task_wheels{
    //setup on initializer 
    uint32_t interval_us;
    int32_t(*encoder)(void*);//[rad]
    void *encoder_object;
    void (*motor)(void*,int);
    void *motor_object;
    
};
typedef struct task_wheels task_wheels_t;

void task_wheels_init(task_wheels_t*,
    uint32_t interval_us,
    q1516_t (*encoder)(void*),void* encoder_object,
    q1516_t (*motor)(void*,int),void* motor_object);
void task_wheels_lanch(task_wheels_t* task,const char* name);
void task_wheels_process(void* task);
    
#endif
    
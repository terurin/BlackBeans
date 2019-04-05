#pragma once
#ifndef __WHEELS_HEADER_GUARD__
#define __WHEELS_HEADER_GUARD__
    
#include <stdint.h>
#include <stdbool.h>

#include <dsptype.h>
    
struct task_wheels{
    //setup on initializer 
    uint32_t interval_us;
    
    
};
typedef struct task_wheels task_wheels_t;

void task_wheels_init(task_wheels_t*,
    uint32_t interval_us);




void task_wheels_lanch(task_wheels_t* task,const char* name);
void task_wheels_process(void* task);
    
#endif
    
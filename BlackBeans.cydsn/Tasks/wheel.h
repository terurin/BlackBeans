#pragma once
#ifndef __WHEEL_HEADER_GUARD__
#define __WHEEL_HEADER_GUARD__
    
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <dsptype.h>
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/semphr.h>
#include "pidc.h"
    
struct wheel{
    //周辺機器(必須)
    int32_t (*encoder_pulse)(void *object);
    void* encoder_object;
    void (*pwm)(void* object,q31_t);
    void* pwm_object;
    //周辺機器(オプション)
    q1516_t (*current_a)(void *object);
    void *current_object;    
    q1516_t (*voltage_v)(void *object);
    void *voltage_object;
    //制御パラメータ
    //mps ... meter per second
    q1516_t (*controllor)(void *object,q1516_t error);
    void* controllor_object;
    uint32_t interval_us;
    q1516_t m_per_pulse;
    xSemaphoreHandle mutex;
    q1516_t tg_mps;
    q1516_t fb_mps;
};
typedef struct wheel wheel_t;
//initalize functions 
float wheel_m_per_pulse(float radius_m,float gear,int pulse_per_rocate);
void task_wheel_init(wheel_t*,uint32_t interval_us,
    float m_per_pulse);
//void task_wheel_assign_current_a(wheel_t* p,q1516_t (*current_a)(void*),void *object);
void task_wheel_assign_voltage_v(wheel_t* p,q1516_t (*voltage_v)(void*),void *object);
void task_wheel_assign_encoder(wheel_t* p,int32_t (*encoder)(void*),void *object);
void task_wheel_assign_pwm(wheel_t* p,void (*pwm)(void*,q31_t),void *object);
void task_wheel_assign_controllor(wheel_t *p,q1516_t(*controllor)(void*,q1516_t),void* object);

void task_wheel_lanch(wheel_t*,const char* name,size_t stack,size_t priority);
void task_wheel_process(wheel_t*);
void task_wheel_target(wheel_t*, q1516_t mps);
q1516_t task_wheel_feedback(wheel_t*);

#endif
    
#include "wheel.h"
#include <assert.h>
#include <task.h>
#include <Driver/timer.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <clip.h>
#define forever() for(;;)
#include <string.h>
//略語　
//fb ... feedback
//tg ... target
//fw ... forward
//semphr 279 tick
//no

float wheel_m_per_pulse(float radius_m,float gear,int pulse_per_rocate){
    const static float m_pi=3.14159265358979323846;
    return ((2.0f*m_pi*gear*radius_m)/pulse_per_rocate)*(1<<16);
}

void task_wheel_init(wheel_t* wheel,uint32_t interval_us,float m_per_pulse){   
    assert(wheel);
    memset(wheel,0,sizeof(wheel_t));
    wheel->interval_us=interval_us; 
    wheel->m_per_pulse=m_per_pulse*(1<<16);
    wheel->mutex=xSemaphoreCreateMutex();
}

/*void task_wheel_assign_current_a(wheel_t* wheel,q1516_t (*current_a)(void*),void *object){
    assert(wheel);
    wheel->current_a=current_a;
    wheel->current_object=object;
}*/

void task_wheel_assign_voltage_v(wheel_t* wheel,q1516_t (*voltage_v)(void*),void *object){
    assert(wheel);
    wheel->voltage_v=voltage_v;
    wheel->voltage_object=object;
}

void task_wheel_assign_encoder(wheel_t* wheel,int32_t (*encoder)(void*),void *object){
    assert(wheel);
    wheel->encoder_pulse=encoder;
    wheel->encoder_object=object;
}

void task_wheel_assign_pwm(wheel_t* wheel,void (*wheelwm)(void*,q31_t),void *object){
    assert(wheel);
    wheel->pwm=wheelwm;
    wheel->pwm_object=object;
}
 
void task_wheel_assign_controllor(wheel_t *wheel,q1516_t(*controllor)(void*,q1516_t),void* object){
    assert(wheel);
    wheel->controllor=controllor;
    wheel->controllor_object=object;
}


void task_wheel_lanch(wheel_t* wheel,const char* name,size_t stack,size_t priority){
    typedef void(*task)(void*);
    assert(wheel);
    xTaskCreate((task)task_wheel_process,name,stack,wheel,priority,NULL);
}
    
void task_wheel_process(wheel_t* wheel){
    assert(wheel);
    const portTickType interval_tick=wheel->interval_us/portTICK_PERIOD_US;
    portTickType last_tick=xTaskGetTickCount();
    tick_t last;// volatile uint32_t log;
    forever(){
        last=profiling_timer_now();
        //update
        q1516_t fb_mps=0;
        if (wheel->encoder_pulse){
            const q1516_t fb_m=wheel->encoder_pulse(wheel->encoder_object)*wheel->m_per_pulse;
            fb_mps=((q4716_t)fb_m*1000000)/wheel->interval_us; 
        }
        const int32_t fb_v=wheel->voltage_v?wheel->voltage_v(wheel->voltage_object):0;
        //const int32_t fb_a=wheel->current_a?wheel->current_a(wheel->current_object):0;
        xSemaphoreTake(wheel->mutex,portMAX_DELAY);
        const q1516_t tg_mps=wheel->tg_mps;
        wheel->fb_mps=fb_mps;
        xSemaphoreGive(wheel->mutex);
        //compute
        const q1516_t fw_v = wheel->controllor?wheel->controllor(wheel->controllor_object,tg_mps-fb_mps):0; 
        const q31_t fw_duty =(fb_v>0)?clip32(((q1548_t)fw_v<<48)/fb_v):0;
        //output 
        if (wheel->pwm)wheel->pwm(wheel->pwm_object,fw_duty);
        //log=profiling_timer_duration(&last);
        vTaskDelayUntil(&last_tick,interval_tick);       
    }
}
void task_wheel_target(wheel_t* wheel, q1516_t mps){
    assert(wheel);
    xSemaphoreTake(wheel->mutex,portMAX_DELAY);
    wheel->tg_mps=mps;
    xSemaphoreGive(wheel->mutex);
}
q1516_t task_wheel_feedback(wheel_t* wheel){
    assert(wheel);
    xSemaphoreTake(wheel->mutex,portMAX_DELAY);
    const q1516_t fb_mps=wheel->fb_mps;
    xSemaphoreGive(wheel->mutex);
    return fb_mps;
}
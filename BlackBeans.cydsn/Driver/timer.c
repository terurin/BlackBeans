#include "timer.h"
#include <project.h>
#include <FreeRTOS.h>
#include <assert.h>

static uint64_t logtimer_tick_ms;
static void logtimer_interrupt(){    
    logtimer_tick_ms++;
}

void logtimer_init(){
    logtimer_tick_ms=0;
    LogInterrupt_StartEx(logtimer_interrupt);       
}

void logtimer_clear(){
    //クリティカルなのでロックする
    LogInterrupt_Disable();    
    logtimer_tick_ms=0; 
    LogInterrupt_Enable();
}

uint64_t logtimer_now_ms(){
    //クリティカルなのでロックする
    LogInterrupt_Disable();
    const uint64_t now_ms = logtimer_tick_ms; 
    LogInterrupt_Enable();
    return now_ms;
}

void profiling_timer_init(){
    ProfilingTimer_Start();
}

tick_t profiling_timer_now(){
    return ProfilingTimer_ReadCounter();
}

tick_t profiling_timer_duration(tick_t* last){
    assert(last);
    tick_t now  = profiling_timer_now();
    //down-counter
    tick_t diff;
    if (*last>=now){
        diff = *last-now;
    }else{
        const tick_t period= ProfilingTimer_ReadPeriod();
        diff = period-(now-*last);
    }
    *last=now;
    return diff;
}

uint32_t profiling_timer_duration_us(tick_t* last){
    static const tick_t tick_per_us=24;
    const tick_t tick= profiling_timer_duration(last);
    return tick/tick_per_us;
}
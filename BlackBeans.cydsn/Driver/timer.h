#pragma once
#ifndef __DRIVER_TIMER_HEADER_GUARD__
#define __DRIVER_TIMER_HEADER_GUARD__
    
#include <stdint.h>
// ms単位で計測できる疑似タイマー
void logtimer_init();   
uint64_t logtimer_now_ms();
void logtimer_clear();
static inline uint64_t logtimer_now_s(){
    return logtimer_now_s()/1000;
}

typedef uint32_t tick_t;
void profiling_timer_init();
tick_t profiling_timer_now();
tick_t profiling_timer_duration(tick_t* last);
uint32_t profiling_timer_duration_us(tick_t* last);

#endif
    
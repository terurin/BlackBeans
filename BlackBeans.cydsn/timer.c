#include "timer.h"
#include <project.h>
#include <stdint.h>
#define FREQUENCY (26000000)
//const static unsigned int timer_frequency= 26000000;//25bit程度なので入る

void timer_wait_us(unsigned int us){
    const static unsigned tick_per_us = FREQUENCY/1000000; 
    const unsigned int tick = us*tick_per_us;
    unsigned int value;
    
    ProfileTimer_Stop();
    ProfileTimer_WriteCounter(tick);
    ProfileTimer_WritePeriod(tick);
    ProfileTimer_Enable();
    do{
        value =ProfileTimer_ReadCounter();
    }while(value!=tick);
}




void timer_wait_ms(unsigned int ms){
    const static unsigned tick_per_ms = FREQUENCY/1000; 
    const unsigned int tick = ms*tick_per_ms;
    unsigned int value;
    
    ProfileTimer_Stop();
    ProfileTimer_WriteCounter(tick);
    ProfileTimer_WritePeriod(tick);
    ProfileTimer_Enable();
    do{
        value =ProfileTimer_ReadCounter();
    }while(value!=tick);
}

void timer_wait_s(unsigned int s){
    const static unsigned tick_per_s = FREQUENCY; 
    const unsigned int tick = s*tick_per_s;
    unsigned int value;
    
    ProfileTimer_Stop();
    ProfileTimer_WriteCounter(tick);
    ProfileTimer_WritePeriod(tick);
    ProfileTimer_Enable();
    do{
        value =ProfileTimer_ReadCounter();
    }while(value!=tick);
}
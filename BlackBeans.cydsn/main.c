/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

#include <stdbool.h>
#include <stdlib.h>
#include "dspmath.h"
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "Tasks/wheel.h"
#include "Driver/timer.h"
//#include "Driver/adc.h"
#include "rtos.h"

static void hardward_init();

int main(void){
    
    rtos_init();
    hardward_init();
    CyGlobalIntEnable; 
    
    rtos_lanch();
}

static void hardward_init(){
    UART_Start();   
    logtimer_init();
    profiling_timer_init();
}
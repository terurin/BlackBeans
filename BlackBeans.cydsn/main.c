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
#include "FreeRTOS.h"
#include "task.h"
#include "Tasks/wheel.h"
#include "Driver/timer.h"
#include "rtos.h"
#include "mattrix.h"
#include "Driver/file_uart.h"
#include <string.h>
static void test();
static void hardward_init();


int main(void){
    
    rtos_init();
    hardward_init();
    test();
    //while(1);
    rtos_lanch();
}

static void hardward_init(){
    CyGlobalIntEnable;     
    logtimer_init();
    profiling_timer_init();
    //file_uart_init(&uart,32);
}

void test(){
    //file_puts(&uart,"hello world\r");
    //file_flush(&uart);
    
    //mattrix_float_dump(result,80,inv,3,3);
    //UART_PutString(result);
    
}

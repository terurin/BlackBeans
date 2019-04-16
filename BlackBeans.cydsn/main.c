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
#include "mattrix.h"
static void test();
static void hardward_init();
int main(void){
    
    rtos_init();
    hardward_init();
    test();
    rtos_lanch();
}

static void hardward_init(){
    UART_Start();   
    logtimer_init();
    profiling_timer_init();
    CyGlobalIntEnable; 
}

void test(){
    char result[256];
    float **mat=mattrix_float_new(3,3);
    float **inv =mattrix_float_new(3,3);
    mattrix_float_fill(mat,0,3,3);
    mat[0][0]=1;
    mat[1][2]=1;
    mat[2][1]=1;
    mattrix_float_dump(result,256,mat,3,3);
    UART_PutString(result);
    mattrix_float_inverse(inv,mat,3);
    
    //mattrix_float_dump(result,80,inv,3,3);
    //UART_PutString(result);
    
}

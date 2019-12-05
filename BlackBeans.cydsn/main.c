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
#include "dsptype.h"
#include "bldc.h"
#include <string.h>
#include "shell.h"
#include "omni.h"
#include "timer.h"
static void test();
static void init();


int main(void){
    volatile int dummy=0;
    init();
    while(1){
        shell_process();
        //dummy++;
    }
}

static void init(){
    CyGlobalIntEnable;     
    bldc_init();
    shell_init();
}
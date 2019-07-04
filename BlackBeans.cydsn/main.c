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
static void test();
static void init();


int main(void){
    init();
    while(1){
        shell_process();
    }
}

static void init(){
    CyGlobalIntEnable;     
    bldc_init();
    shell_init();
    
}
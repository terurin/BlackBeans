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
static void test();
static void hardward_init();


int main(void){
    hardward_init();
    test();
    while(1);
}

static void hardward_init(){
    CyGlobalIntEnable;     
    bldc_init();
}

void test(){
    bldc_write_raw(0,2000);
    bldc_write_raw(1,2000);
    bldc_write_raw(2,2000);
}

#include "motor.h"
#include "bldc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "text.h"
#define M_PI 3.141592
#include <math.h>
#define BUFFER_SIZE (256)

static q1516_t table_x[3];
static q1516_t table_y[3];

void motor_init(){

    const static float angle[3]={0.0f,M_PI*2.0f/3.0f,-M_PI*2.0f/3.0f};
    float t;
    unsigned int i;
    for (i=0;i<sizeof(angle)/sizeof(angle[0]);i++){
        table_x[i]=cosf(angle[i])*(1<<16);
        table_y[i]=sinf(angle[i])*(1<<16);
    }
    
}




void motor_fraction(int argc,char** argv){
    int i;
    for (i=1;i<argc;i++){
        bldc_control(i,NULL,NULL);
        bldc_write_raw(i-1,atoi(argv[i]));
    }
    for (;i<4;i++){
        bldc_control(i,NULL,NULL);
        bldc_write_raw(i-1,0);
    }
}

void motor_q15(int argc,char** argv){
    int i;
    for (i=1;i<argc;i++){
        bldc_control(i,NULL,NULL);
        bldc_write(i-1,atoi(argv[i]));
    }
    for (;i<bldc_count+1;i++){
        bldc_control(i,NULL,NULL);
        bldc_write(i-1,0);
    }
}

void motor_encoder(int argc,char** argv){
    (void)argc;
    (void)argv;
    char line[256];
    snprintf(line,sizeof(line),"%d %d %d\n",
        bldc_read(0),bldc_read(1),bldc_read(2));
    
    shell_puts(line);
}

void motor_status(int argc,char** argv){
    (void)argc;
    (void)argv; 
    int i;
    char buffer[BUFFER_SIZE]="";
    char tmp[BUFFER_SIZE];
    for (i=0;i<bldc_count-1;i++){
        sprintf(tmp,"%02x ",bldc_status(i)&0x7);
        strcat(buffer,tmp);
    }
    sprintf(tmp,"%02x\n",bldc_status(i)&0x7);
    strcat(buffer,tmp);
    shell_puts(buffer);
}

#define M_PI (3.14f)
void motor_rocate(int argc,char** argv){
    
    const q1516_t x = argc>1?atoff(argv[1])*(1<<16):0;
    const q1516_t y = argc>2?atoff(argv[2])*(1<<16):0;
    const q1516_t theta = argc>3?atoff(argv[3])*(1<<16):0;
    
    int i;
    q3132_t t;
    for (i=0;i<bldc_count;i++){
        t=(q3132_t)table_x[i]*x+(q3132_t)table_y[i]*y;
        bldc_write(i,t>>16);
    }
}


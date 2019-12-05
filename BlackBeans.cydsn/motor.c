#include "motor.h"
#include "bldc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "text.h"
#include<math.h>
#define BUFFER_SIZE (256)

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
    char tmp[8];
    for (i=0;i<bldc_count-1;i++){
        strcat(buffer,text_bits(tmp,bldc_status(i),7));
        strcat(buffer," ");
    }
    strcat(buffer,text_bits(tmp,bldc_status(bldc_count-1),6));
    strcat(buffer,"\n");
    shell_puts(buffer);
}

#define M_PI (3.14f)
void motor_rocate(int argc,char** argv){

    const static float table_x[]={sinf(0.0),sinf(M_PI*2.0f/3.0f),sinf(M_PI*4.0f/3.0f)};
    const static float table_y[]={cosf(0.0),cosf(M_PI*2.0f/3.0f),cosf(M_PI*4.0f/3.0f)};
    
    const float x = argc>1?atoff(argv[1]):0;
    const float y = argc>1?atoff(argv[2]):0;
    const float theta = argc>1?atoff(argv[3]):0;
    
    int i;
    q3132_t t;
    for (i=0;i<bldc_count;i++){
        t=table_x[i]*x+table_y[i]*y;
        bldc_write(i,t*100);
    }
}


#include "motor.h"
#include "bldc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "text.h"

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

void motor_rocate(int argc,char** argv){
    //TODO
}


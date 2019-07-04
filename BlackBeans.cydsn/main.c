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
static void process();
static void parse (char*);

int main(void){
    hardward_init();
    while(1){
        process();
    }
}

static void hardward_init(){
    CyGlobalIntEnable;     
    bldc_init();
    bldc_write_raw(1,0);
    bldc_write_raw(2,0);
    bldc_write_raw(3,0);
    UART_Start();
}

void test(){
    bldc_write_raw(1,2000);
    bldc_write_raw(2,2000);
    bldc_write_raw(3,2000);
}

void process(){
    static const char newline='\n';
    static char buffer[256];
    static int index=0;
    while (UART_GetRxBufferSize()>0){
        const char c=UART_GetChar();
        if (c!=newline&&c!='\0'){
            buffer[index++]=c;
            if (index<256)continue;
            index=0;
            UART_PutString("Error:overflow\n");
        }else{
            buffer[index]='\0';
            index=0;
            parse(buffer);
        }   
    }
}

static void motor_fraction(int argc,char** argv);

void parse(char* str){
    static const char split[]=" ";
    char *argv[16],*word,*pos;
    
    argv[0]=word=strtok_r(str,split,&pos);
    size_t argc=0;
    for (argc=1;argc<16;argc++){
        word=strtok_r(NULL,split,&pos);
        if (!word)break;
        argv[argc]=word;
    }
    
    const char*const name=argv[0];
    if (!strcmp(name,"mf")){
        motor_fraction(argc,argv);
    }else{
        UART_PutString("Error:");
        UART_PutString(name);
        UART_PutChar('\n');
    }
}

static void motor_fraction(int argc,char** argv){
    int i;
    for (i=1;i<argc;i++){
        bldc_write_raw(i,atoi(argv[i]));
    }
    for (;i<4;i++){
        bldc_write_raw(i,0);
    }
    
    
    
}
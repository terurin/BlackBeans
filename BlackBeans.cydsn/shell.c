#include "shell.h"
#include "project.h"
#include "bldc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char newline='\n';
#define BUFFER_SIZE (256)
void (*shell_puts_func)(const char*);

void shell_init(){
    UART_Start();
    USBUART_Start(0, USBUART_3V_OPERATION);
}

void shell_puts(const char* str){
    if (shell_puts_func){
        shell_puts_func(str);
    }
}

static void parse(char*);
static inline void uart_process();
static inline void usb_process();
void shell_process(){
    //出力切り替え(uart)
    shell_puts_func=UART_PutString;
    uart_process();
    //出力切り替え(usb)
    shell_puts_func=USBUART_PutString;
    usb_process();
}

void uart_process(){
    static char rbuffer[BUFFER_SIZE];
    static int rindex=0;
    
    while (UART_GetRxBufferSize()>0){
        const char c=UART_GetChar();
        if (c!=newline&&c!='\0'){
            rbuffer[rindex++]=c;
            if (rindex<BUFFER_SIZE)continue;
            rindex=0;
            shell_puts("Error:overflow\n");
        }else{
            rbuffer[rindex]='\0';
            rindex=0;
            parse(rbuffer);
        }   
    }
}

void usb_process(){
    static char rbuffer[BUFFER_SIZE];
    static int rindex=0;  
    static int state=0;
    
    switch(state){
    case 0:   
        if (!USBUART_bGetConfiguration()){
            return;
        }
        USBUART_CDC_Init();
        state=1;
    case 1:
        if (!USBUART_CDCIsReady()){
            return;
        }
        
        while (USBUART_GetCount()>0){
            const char c = USBUART_GetChar();
            if (c!=newline&&c!='\0'){
                rbuffer[rindex++]=c;
                if (rindex<BUFFER_SIZE)continue;
                rindex=0;
                shell_puts("Error:overflow\n");
            }else{
                rbuffer[rindex]='\0';
                rindex=0;
                parse(rbuffer);
            }    
        }
        break;
    }
}

static void motor_fraction(int argc,char** argv);
static void motor_q15(int argc,char** argv);
static void motor_encoder(int argc,char** argv);

void parse(char* str){
    static const char split[]=" ";
    char *argv[16],*word,*pos;
    char buffer[256];
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
    }else if (!strcmp(name,"mq")){
        motor_q15(argc,argv);
    }else if (!strcmp(name,"me")){
        motor_encoder(argc,argv);
    }else{
        sniprintf(buffer,sizeof(buffer),"Error:%s\n",name);
        shell_puts(buffer);
    }
}

void motor_fraction(int argc,char** argv){
    int i;
    for (i=1;i<argc;i++){
        bldc_write_raw(i-1,atoi(argv[i]));
    }
    for (;i<4;i++){
        bldc_write_raw(i-1,0);
    }
}

void motor_q15(int argc,char** argv){
    int i;
    for (i=1;i<argc;i++){
        bldc_write(i-1,atoi(argv[i]));
    }
    for (;i<4;i++){
        bldc_write(i-1,0);
    }
}

void motor_encoder(int argc,char** argv){
    char line[256];
    snprintf(line,sizeof(line),"%d %d %d\n",
        bldc_read(0),bldc_read(1),bldc_read(2));
    
    shell_puts(line);
}

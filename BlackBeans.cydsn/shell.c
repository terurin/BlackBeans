#include "shell.h"
#include "project.h"
#include "bldc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "motor.h"
static const char newline='\n';
#define BUFFER_SIZE (256)
void (*shell_puts_func)(const char*);

void shell_init(){
    UART_Start();
    //USBUART_Start(0, USBUART_3V_OPERATION);
}

void shell_puts(const char* str){
    if (shell_puts_func){
        shell_puts_func(str);
    }
}

static inline void uart_process();
static inline void usb_process();
void shell_process(){
    //出力切り替え(uart)
    shell_puts_func=UART_PutString;
    uart_process();
    //出力切り替え(usb)
    //shell_puts_func=USBUART_PutString;
    //usb_process();
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
            shell_system(rbuffer);
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
                shell_system(rbuffer);
            }    
        }
        break;
    }
}

static void shell_error(int,char**);//default error
static void shell_echo(int,char**);

void shell_system(char* str){
    static const char split[]=" ";
    char *argv[16],*word,*pos;
    argv[0]=word=strtok_r(str,split,&pos);
    size_t argc=0;
    for (argc=1;argc<16;argc++){
        word=strtok_r(NULL,split,&pos);
        if (!word)break;
        argv[argc]=word;
    }
    
    shell_run(argc,argv);
}

void shell_run(int argc,char **argv){
    
    const static struct{
        const char* name;
        void (*func)(int,char**);
    }items[]={
        {"mq",motor_q15},
        {"mf",motor_fraction},
        {"me",motor_encoder},
        {"ms",motor_status},
        {"echo",shell_echo}
    };
    
    unsigned int i;
    const char *name=argv[0];
    
    if (!argc)return;
    for (i=0;i<(sizeof(items)/sizeof(items[0]));i++){
        if (!strcmp(name,items[i].name)){
            items[i].func(argc,argv);
            return;
        }
    }
    shell_error(1,argv);
}


void shell_error(int argc,char** argv){
    (void)argc;
    char buffer[BUFFER_SIZE]="";
    strcat(buffer,"Erorr:");
    strcat(buffer,argv[0]);
    strcat(buffer,"\n");
    shell_puts(buffer);
}

void shell_echo(int argc,char** argv){
    int i;
    char buffer[BUFFER_SIZE]="";
    for (i=1;i<argc-1;i++){
        strcat(buffer,argv[i]);
        strcat(buffer," ");
    }
    strcat(buffer,argv[argc-1]);
    strcat(buffer,"\n");
    shell_puts(buffer);
}
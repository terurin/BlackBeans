#include "shell.h"
#include "project.h"
#include "bldc.h"
#include <stdlib.h>
static const char newline='\n';
#define BUFFER_SIZE (256)

void shell_init(){
    UART_Start();
}

static void parse(char*);
static inline void uart_process();
static inline void usb_process();
void shell_process(){
    uart_process();
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
            UART_PutString("Error:overflow\n");
        }else{
            rbuffer[rindex]='\0';
            rindex=0;
            parse(rbuffer);
        }   
    }
}

static void motor_fraction(int argc,char** argv);
static void motor_q15(int argc,char** argv);
static void motor_encoder(int argc,char** argv);

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
    }else if (!strcmp(name,"mq")){
        motor_q15(argc,argv);
    }else if (!strcmp(name,"me")){
        motor_encoder(argc,argv);
    }else{
        UART_PutString("Error:");
        UART_PutString(name);
        UART_PutChar('\n');
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
    char buf[32];
    for (int i=0;i<3;i++){
        UART_PutString(itoa(bldc_read(i),buf,10));
        UART_PutChar(' ');
    }
    UART_PutChar(newline);
}

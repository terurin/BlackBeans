#include "shell.h"
#include "project.h"
#include "bldc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "motor.h"
static const char newline='\n';
#define BUFFER_SIZE (256)
#define ARGUMENT_SIZE (16)
void (*shell_puts_func)(const char*);

static inline void uart_process();
static inline void usb_process();

struct shell_item{
    const char* name;//コマンド名
    void (*func)(int,char**);//関数
    const char* usage;//使い方
};
typedef struct shell_item shell_item_t;
const static shell_item_t items[]={
    {"mq",motor_q15,"motor q15-format"},
    {"mf",motor_fraction,"motor fraction by 8000"},
    {"me",motor_encoder,"encoder"},
    {"ms",motor_status,"motor status"},
    {"echo",shell_echo,"echo echo"},
    {"mr",motor_rocate,"motor rocate}"},
    {"help",shell_help,"system help"},
    {"ls",shell_list,"list command"},
    {"!",shell_last,"run last command"},
    {"repeat",shell_repeat,"repeat"},
};
//過去一回分の記録
static char history_buffer[BUFFER_SIZE];
static char history_argc=0;
static char *history_argv[ARGUMENT_SIZE];

void shell_init(){
    UART_Start();
    //USBUART_Start(0, USBUART_3V_OPERATION);
    shell_puts_func=UART_PutString;
    motor_init();
}

void shell_puts(const char* str){
    if (shell_puts_func){
        shell_puts_func(str);
    }
}

void shell_process(){
    //出力切り替え(uart)
    uart_process();
    //出力切り替え(usb)
    //usb_process();
}

void uart_process(){
    static char rbuffer[BUFFER_SIZE];
    static int rindex=0;
    shell_puts_func=UART_PutString;
    while (UART_GetRxBufferSize()>0){
        const char c=UART_GetChar();
        if (c!=newline&&c!='\0'){
            rbuffer[rindex++]=c;
            if (rindex<BUFFER_SIZE)continue;
            rindex=0;
            shell_puts("Error:Overflow\n");
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
    shell_puts_func=USBUART_PutString;
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
    unsigned int i;
    const char *name=argv[0];
    
    if (!argc)return;
    for (i=0;i<(sizeof(items)/sizeof(items[0]));i++){
        if (!strcmp(name,items[i].name)){
            items[i].func(argc,argv);
            //historyに記録する
            if (strcmp(name,"!")){
                char* it=history_buffer;
                int j;
                history_argc=argc;
                for (j=0;j<argc;j++){
                    history_argv[j]=strcpy(it,argv[j]);
                    it+=strlen(argv[j])+1;
                }
            }
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

void shell_help(int argc,char** argv){
    (void)argc,(void)argv;
    unsigned int index;
    const shell_item_t* object;
    char buffer[BUFFER_SIZE]="";
    for (index=0;index<sizeof(items)/sizeof(items[0]);index++){
        object=&items[index];
        strncat(buffer,object->name,BUFFER_SIZE);
        strncat(buffer,"\t",BUFFER_SIZE);
        strncat(buffer,object->usage,BUFFER_SIZE);
        strncat(buffer,"\n",BUFFER_SIZE);
    }
    shell_puts(buffer);
}

void shell_list(int argc,char** argv){
    (void)argc,(void)argv;
    unsigned int index;
    shell_item_t* object;
    char buffer[BUFFER_SIZE]="";
    
    strncat(buffer,items[0].name,BUFFER_SIZE);
    for (index=1;index<sizeof(items)/sizeof(items[0]);index++){
        strncat(buffer,",",BUFFER_SIZE);
        strncat(buffer,items[index].name,BUFFER_SIZE);
    }
    strncat(buffer,"\n",BUFFER_SIZE);
    shell_puts(buffer);
}

void shell_last(int argc,char** argv){
    (void)argc,(void)argv;
    shell_run(history_argc,history_argv);
}

void shell_repeat(int argc,char **argv){
    do{
        shell_run(argc-1,argv+1);
    }while(UART_GetRxBufferSize()==0);
}
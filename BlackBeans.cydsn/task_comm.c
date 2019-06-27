#include "task_comm.h"
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <project.h>
#include <shell.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <USBUART_1.h>
//あとでけす
#include <pwm.h>
#include <rst.h>
#include <hall.h>

const static char newline='\n';
#define LINE_SIZE 32
void check_port(shell_puts_t writer,int argc,const char **argv);

void task_comm_init(){
   

}

void check_port(shell_puts_t writer,int argc,const char **argv){
    int pwm=0,rst=0;
    char buffer[64];
    buffer[0]=0;
    if (argc<2){
        strcat(buffer,"port pwm rst\n");
    }else{
        pwm=atoi(argv[1]);
        rst=atoi(argv[2]);
    }
//    pwm_Write(pwm);
    //rst_Write(rst);
    
    strcat(buffer,"hall:");
    //int hall=hall_Read();
    /*for (int i=2;i>=0;i--){
        strcat(buffer,hall&(1<<i)?"1":"0");
    }*/
    strcat(buffer,"\n");
    writer(buffer);
}

static void task_comm_process(void*);

void task_comm_lanch(size_t stack,size_t priority){
    xTaskCreate(task_usb_process,"usb",stack,NULL,priority+1,NULL);
    xTaskCreate(task_uart_process,"uart",stack,NULL,priority,NULL);
}

static shell_t shell_init_comm(){
    shell_t shell;
    shell_init(&shell,32);
    shell_join_basic(&shell);
    shell_join_simple(&shell,"port",check_port);
    return shell;
}


void task_uart_process(void* object){
    (void)object;
    //周期呼び出し
    const static portTickType interval_tick=10/portTICK_PERIOD_US;
    portTickType last_tick=xTaskGetTickCount();
    //バッファ
    static char line[LINE_SIZE+1];
    size_t index=0;
    shell_t shell = shell_init_comm();
    UART_Start();
    for (;;){
        //uart
        while (UART_GetRxBufferSize()>0){
            const char c = UART_ReadRxData();
            if (c!=newline){
                line[index++]=c;
                if (index==LINE_SIZE){
                    index=0;
                    UART_PutString("Error:Overflow\n");
                }
                continue;
            }
            
            //process
            line[index]=0;
            shell_parse(&shell,UART_PutString,line);
            index=0;

        }
        vTaskDelayUntil(&last_tick,interval_tick);   
    }
}

//安全なusb putsを提供する
static void usb_puts(const char* str){
    while(USBUART_1_DataIsReady());
    USBUART_1_PutString(str);
}

void task_usb_process(void* object){
    (void)object;
    //周期呼び出し
    const static portTickType interval_tick=10/portTICK_PERIOD_US;
    portTickType last_tick=xTaskGetTickCount();
    //バッファ
    static char line[LINE_SIZE];
    size_t index=0;
    shell_t shell = shell_init_comm();
    //usb 初期化
    USBUART_1_Start(0,USBUART_1_3V_OPERATION);
    while(!USBUART_1_bGetConfiguration()){
        vTaskDelayUntil(&last_tick,interval_tick);
    }
    USBUART_1_CDC_Init();//usb(cdc)通信擁立!!    
    for (;;){
        //uart
        while (USBUART_1_CDCIsReady()&&USBUART_1_GetCount()>0){
            const char c = USBUART_1_GetChar();
            //add charactor into buffer
            if (c!=newline){
                line[index++]=c;
                if (index==LINE_SIZE){
                    index=0;
                    usb_puts("Error:Overflow\n");
                }
                continue;
            }

            line[index]=0;
            shell_parse(&shell,usb_puts,line);
            index=0;  
        }
        vTaskDelayUntil(&last_tick,interval_tick);
    }
}
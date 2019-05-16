#include "task_uart.h"
#include <FreeRTOS.h>
#include <task.h>
#include <project.h>
#include <shell.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
const static char newline='\n';

void task_uart_init(){
    UART_Start();    
}




static void task_uart_process(void*);
void task_uart_lanch(const char*name, size_t stack,size_t priority){
    xTaskCreate(task_uart_process,name,stack,NULL,priority,NULL);
}

static char* test(char* buffer,size_t buffer_size,const char** argv,int argc){
    (void)buffer;
    (void)buffer_size;
    (void)argv;
    (void)argc;
    return "test";
}

static void task_uart_process(void* object){
    (void)object;
    //周期呼び出し
    const static portTickType interval_tick=10/portTICK_PERIOD_US;
    portTickType last_tick=xTaskGetTickCount();
    //行読み込み
    const static size_t line_size=32;
    const static size_t buffer_size=128;
    char*const line =(char*) pvPortMalloc(line_size);
    char*const buffer=(char*)pvPortMalloc(buffer_size);
    //シェルの構築
    shell_t shell;
    shell_init(&shell,32);
    shell_join(&shell,"test",test);
    shell_dump(&shell,UART_PutString);
    
    int index=0;
    for(;;){
        while (UART_ReadRxStatus()&UART_RX_STS_FIFO_NOTEMPTY){
            const char c = UART_ReadRxData();
            if (c==newline){
                line[index]=0;
                const char*const response=shell_parse(&shell,buffer,buffer_size,line);
                UART_PutString(response);
                //UART_PutString(line);
                UART_PutChar(newline);
                index=0;
            }else{
                line[index++]=c;
                if (index==line_size){
                    index=0;
                    UART_ClearRxBuffer();
                    UART_PutString("Error:Overflow\n");
                    
                }
            }
        }
        vTaskDelayUntil(&last_tick,interval_tick);   
    }
    
    
    
}

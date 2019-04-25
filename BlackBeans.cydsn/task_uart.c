#include "task_uart.h"
#include <FreeRTOS.h>
#include <task.h>
#include <project.h>

const static char newline='\r';

void task_uart_init(){
    UART_Start();    
}
static void task_uart_process(void*);
void task_uart_lanch(const char*name, size_t stack,size_t priority){
    xTaskCreate(task_uart_process,name,stack,NULL,priority,NULL);
}
static void task_uart_process(void* object){
    (void)object;
    //周期呼び出し
    const static portTickType interval_tick=10/portTICK_PERIOD_US;
    portTickType last_tick=xTaskGetTickCount();
    //行読み込み
    const static size_t line_size=32;
    char*const line =(char*) pvPortMalloc(line_size);
    int index=0;
    for(;;){
        while (UART_GetRxBufferSize()>0){
            const char c = UART_GetByte();
            if (c=='\r'){
                line[index]=0;
                UART_PutString(line);
                index++;
            }else{
                line[index++]=c;
                if (index==line_size){
                    index=0;
                    UART_PutString("Error:Overflow\r");
                }
            }
        }
        vTaskDelayUntil(&last_tick,interval_tick);   
    }
    
    
    
}

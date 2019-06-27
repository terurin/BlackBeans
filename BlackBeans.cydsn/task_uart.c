#include "task_uart.h"
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <project.h>
#include <shell.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//Shell
const static char newline='\n';
static shell_t* shell;
//Buffer
#define RX_BUFFER_SIZE (32)
static char* rx_buffer;
//Task
const static portTickType interval_tick=10/portTICK_PERIOD_US;
portTickType last_tick;

void task_uart_lanch(size_t stack,size_t priority,shell_t* _shell){
    shell=_shell;
    UART_Start();
    xTaskCreate(task_uart_process,"uart",stack,NULL,priority,NULL);
}

void task_uart_process(void* object){
    (void)object;
    //周期呼び出し
    last_tick=xTaskGetTickCount();
    //バッファ
    size_t index=0;
    for (;;){
        //uart
        while (UART_GetRxBufferSize()>0){
            const char c = UART_ReadRxData();
            if (c!=newline){
                rx_buffer[index++]=c;
                if (index==RX_BUFFER_SIZE){
                    index=0;
                    UART_PutString("Error:Overflow\n");
                }
                continue;
            }
            
            //process
            rx_buffer[index]=0;
            shell_parse(shell,UART_PutString,rx_buffer);
            index=0;

        }
        vTaskDelayUntil(&last_tick,interval_tick);   
    }
}
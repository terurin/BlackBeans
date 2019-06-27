#include "task_usb.h"
//FreeRTOS
#include <FreeRTOS.h>
#include <task.h>
//Driver
#include <USBUART_cdc.h>
//Custom
#include <shell.h>
//Shell
const static char newline='\n';
static shell_t *shell;
//Buffer
#define TX_BUFFER_SIZE (128)
#define RX_BUFFER_SIZE (32)
static char* tx_buffer;
static size_t tx_index;
static char* rx_buffer;
//Task
const static portTickType interval_tick=10/portTICK_PERIOD_US;
portTickType last_tick;

void task_usb_launch(size_t stack,size_t priority,shell_t* _shell){
    shell=_shell;
    //bufferのメモリ確保
    tx_buffer =(char*)pvPortMalloc(sizeof(char)*TX_BUFFER_SIZE);
    tx_buffer[0]=0;
    tx_index=0;
    rx_buffer =(char*)pvPortMalloc(sizeof(char)*RX_BUFFER_SIZE);
    //タスク立ち上げ
    xTaskCreate(task_usb_process,"usb",stack,NULL,priority,NULL);
}

static void flush(){
    tx_buffer[tx_index]=0;
    while(USBUART_DataIsReady()){
        vTaskDelayUntil(&last_tick,interval_tick);
    }
    USBUART_PutString(tx_buffer);
    tx_index=0;
}

static void write(const char* str){
    for (;*str;str++){
        tx_buffer[tx_index++]=*str;
        if(tx_index<TX_BUFFER_SIZE-1)continue;
        flush();
    }
}

void task_usb_process(void* object){
    (void)object;
    //周期呼び出し
    
    last_tick=xTaskGetTickCount();
    //バッファ
    size_t index=0;
    //usb 初期化
    USBUART_Start(0,USBUART_3V_OPERATION);
    while(!USBUART_bGetConfiguration()){
        vTaskDelayUntil(&last_tick,interval_tick);
    }
    USBUART_CDC_Init();  
    for (;;){
        //uart
        while (USBUART_CDCIsReady()&&USBUART_GetCount()>0){
            const char c = USBUART_GetChar();
            //add charactor into buffer
            if (c!=newline){
                rx_buffer[index++]=c;
                if (index==RX_BUFFER_SIZE){
                    index=0;
                    write("Error:Overflow\n");
                }
                continue;
            }

            rx_buffer[index]=0;
            shell_parse(shell,write,rx_buffer);
            index=0;  
        }
        flush();
        vTaskDelayUntil(&last_tick,interval_tick);
    }
}
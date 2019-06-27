#include "rtos.h"
#include <project.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Tasks/wheel.h>
#include <Tasks/monitor.h>
#include "task_comm.h"
void rtos_init( void ){
/* Port layer functions that need to be copied into the vector table. */
extern void xPortPendSVHandler( void );
extern void xPortSysTickHandler( void );
extern void vPortSVCHandler( void );
extern cyisraddress CyRamVectors[];

	/* Install the OS Interrupt Handlers. */
	CyRamVectors[ 11 ] = ( cyisraddress ) vPortSVCHandler;
	CyRamVectors[ 14 ] = ( cyisraddress ) xPortPendSVHandler;
	CyRamVectors[ 15 ] = ( cyisraddress ) xPortSysTickHandler;
}
//heap
//uint8_t ucHeap[ configTOTAL_HEAP_SIZE ] __attribute__ ( (section( ".my_heap" ) ) );
//task
static wheel_t wheel;

void rtos_lanch(){
    //task_wheel_init(&wheel,100,1e-3);
    //task_wheel_lanch(&wheel,"wheel",64,4);
        
    //task_monitor_init();
    //task_monitor_lanch("monitor",64,4);
    //task_comm_init();
    //task_comm_lanch(64,1);
    vTaskStartScheduler(); 
}
/*
void vApplicationIdleHook(void ){
    static char line[32]={0};
    static size_t index=0;
    UART_Start();
    while (UART_GetRxBufferSize()>0){
        const char c=UART_GetChar();
        if (c=='\r'){
            line[index]='\0';
            UART_PutString(line);
            index=0;
        }else if (index<sizeof(line)/sizeof(line[0])-1){
            line[index++]=c;
        }else {
            UART_PutString("overflow\r");
            index=0;
        }
           
    }
}*/

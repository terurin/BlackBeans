#include "rtos.h"
#include <project.h>
#include <FreeRTOS.h>
#include <task.h>
#include "task_usb.h"
#include "task_uart.h"
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
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ] __attribute__ ( (section( ".my_heap" ) ) );




void rtos_lanch(){
    //shell
    shell_t shell_usb,shell_uart;
    shell_init(&shell_usb,32);
    shell_join_basic(&shell_usb);
    shell_init(&shell_uart,32);
    shell_join_basic(&shell_uart);
    //tasks
    task_usb_launch(64,1,&shell_usb);
    task_uart_lanch(64,1,&shell_uart);
    vTaskStartScheduler(); 
}

#include "rtos.h"
#include <project.h>
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/task.h>
#include <Tasks/wheel.h>
#include <Tasks/monitor.h>
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
//task
static wheel_t wheel;

void rtos_lanch(){
    task_wheel_init(&wheel,100,1e-3);
    task_wheel_lanch(&wheel,"wheel",64,4);
        
    task_monitor_init();
    task_monitor_lanch("monitor",64,4);
   
    vTaskStartScheduler(); 
}
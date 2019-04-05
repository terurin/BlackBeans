#include "rtos.h"
#include <project.h>
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/task.h>
#include <Tasks/wheels.h>
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

void rtos_lanch(){
    task_wheels_t wheels;
    task_wheels_init(&wheels,100);
    task_wheels_lanch(&wheels,"wheel");
    
    
    vTaskStartScheduler(); 
}
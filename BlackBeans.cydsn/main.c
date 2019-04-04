/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dspmath.h"
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
static void mainTask( void *pvParameters );
void prvHardwareSetup( void )
{
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

int main(void){
        
    prvHardwareSetup();
    
    UART_Start();
    CyGlobalIntEnable; /* Enable global interrupts. */
    xTaskCreate(mainTask,"main",100,NULL,1,NULL);
    vTaskStartScheduler();    

}

static void mainTask( void *pvParameters ){
  const TickType_t xTicksToDelay = 1000 / portTICK_PERIOD_MS;

  while(1) {
      vTaskDelay( xTicksToDelay );
  }
}
/* [] END OF FILE */

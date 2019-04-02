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

int main(void){
    UART_Start();
    CyGlobalIntEnable; /* Enable global interrupts. */

    int n=fact(16);
    char buffer[16];
    UART_PutString(itoa(n,buffer,10));
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    while(true){
        while(UART_GetRxBufferSize()==0);
        UART_PutChar(UART_GetChar());
        UART_PutString(itoa(n,buffer,10));
        UART_PutCRLF(1);
    }
}

/* [] END OF FILE */

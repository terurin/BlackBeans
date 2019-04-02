/*******************************************************************************
* File Name: MotorEncoderC_INT.c
* Version 3.0
*
* Description:
*  This file contains the Interrupt Service Routine (ISR) for the Quadrature
*  Decoder component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "MotorEncoderC.h"
#include "MotorEncoderC_PVT.h"
#include "cyapicallbacks.h"

volatile int32 MotorEncoderC_count32SoftPart = 0;


/*******************************************************************************
* FUNCTION NAME: void MotorEncoderC_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for Quadrature Decoder Component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  MotorEncoderC_count32SoftPart - modified to update hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*  MotorEncoderC_swStatus - modified with the updated values of STATUS
*  register.
*
*******************************************************************************/
CY_ISR( MotorEncoderC_ISR )
{
   uint8 MotorEncoderC_swStatus;

   MotorEncoderC_swStatus = MotorEncoderC_STATUS_REG;

    #ifdef MotorEncoderC_ISR_ENTRY_CALLBACK
        MotorEncoderC_ISR_EntryCallback();
    #endif /* MotorEncoderC_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START MotorEncoderC_ISR_START` */

    /* `#END` */

    if (0u != (MotorEncoderC_swStatus & MotorEncoderC_COUNTER_OVERFLOW))
    {
        MotorEncoderC_count32SoftPart += (int32) MotorEncoderC_COUNTER_MAX_VALUE;
    }
    else if (0u != (MotorEncoderC_swStatus & MotorEncoderC_COUNTER_UNDERFLOW))
    {
        MotorEncoderC_count32SoftPart -= (int32) MotorEncoderC_COUNTER_INIT_VALUE;
    }
    else
    {
        /* Nothing to do here */
    }
    
    if (0u != (MotorEncoderC_swStatus & MotorEncoderC_COUNTER_RESET))
    {
        MotorEncoderC_count32SoftPart = 0;
    }
    
    /* User code required at end of ISR */
    /* `#START MotorEncoderC_ISR_END` */

    /* `#END` */

    #ifdef MotorEncoderC_ISR_EXIT_CALLBACK
        MotorEncoderC_ISR_ExitCallback();
    #endif /* MotorEncoderC_ISR_EXIT_CALLBACK */
}


/* [] END OF FILE */

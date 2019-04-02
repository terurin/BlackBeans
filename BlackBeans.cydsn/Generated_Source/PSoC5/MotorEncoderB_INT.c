/*******************************************************************************
* File Name: MotorEncoderB_INT.c
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

#include "MotorEncoderB.h"
#include "MotorEncoderB_PVT.h"
#include "cyapicallbacks.h"

volatile int32 MotorEncoderB_count32SoftPart = 0;


/*******************************************************************************
* FUNCTION NAME: void MotorEncoderB_ISR
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
*  MotorEncoderB_count32SoftPart - modified to update hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*  MotorEncoderB_swStatus - modified with the updated values of STATUS
*  register.
*
*******************************************************************************/
CY_ISR( MotorEncoderB_ISR )
{
   uint8 MotorEncoderB_swStatus;

   MotorEncoderB_swStatus = MotorEncoderB_STATUS_REG;

    #ifdef MotorEncoderB_ISR_ENTRY_CALLBACK
        MotorEncoderB_ISR_EntryCallback();
    #endif /* MotorEncoderB_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START MotorEncoderB_ISR_START` */

    /* `#END` */

    if (0u != (MotorEncoderB_swStatus & MotorEncoderB_COUNTER_OVERFLOW))
    {
        MotorEncoderB_count32SoftPart += (int32) MotorEncoderB_COUNTER_MAX_VALUE;
    }
    else if (0u != (MotorEncoderB_swStatus & MotorEncoderB_COUNTER_UNDERFLOW))
    {
        MotorEncoderB_count32SoftPart -= (int32) MotorEncoderB_COUNTER_INIT_VALUE;
    }
    else
    {
        /* Nothing to do here */
    }
    
    if (0u != (MotorEncoderB_swStatus & MotorEncoderB_COUNTER_RESET))
    {
        MotorEncoderB_count32SoftPart = 0;
    }
    
    /* User code required at end of ISR */
    /* `#START MotorEncoderB_ISR_END` */

    /* `#END` */

    #ifdef MotorEncoderB_ISR_EXIT_CALLBACK
        MotorEncoderB_ISR_ExitCallback();
    #endif /* MotorEncoderB_ISR_EXIT_CALLBACK */
}


/* [] END OF FILE */

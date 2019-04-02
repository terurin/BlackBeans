/*******************************************************************************
* File Name: MotorEncoderA_INT.c
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

#include "MotorEncoderA.h"
#include "MotorEncoderA_PVT.h"
#include "cyapicallbacks.h"

volatile int32 MotorEncoderA_count32SoftPart = 0;


/*******************************************************************************
* FUNCTION NAME: void MotorEncoderA_ISR
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
*  MotorEncoderA_count32SoftPart - modified to update hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*  MotorEncoderA_swStatus - modified with the updated values of STATUS
*  register.
*
*******************************************************************************/
CY_ISR( MotorEncoderA_ISR )
{
   uint8 MotorEncoderA_swStatus;

   MotorEncoderA_swStatus = MotorEncoderA_STATUS_REG;

    #ifdef MotorEncoderA_ISR_ENTRY_CALLBACK
        MotorEncoderA_ISR_EntryCallback();
    #endif /* MotorEncoderA_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START MotorEncoderA_ISR_START` */

    /* `#END` */

    if (0u != (MotorEncoderA_swStatus & MotorEncoderA_COUNTER_OVERFLOW))
    {
        MotorEncoderA_count32SoftPart += (int32) MotorEncoderA_COUNTER_MAX_VALUE;
    }
    else if (0u != (MotorEncoderA_swStatus & MotorEncoderA_COUNTER_UNDERFLOW))
    {
        MotorEncoderA_count32SoftPart -= (int32) MotorEncoderA_COUNTER_INIT_VALUE;
    }
    else
    {
        /* Nothing to do here */
    }
    
    if (0u != (MotorEncoderA_swStatus & MotorEncoderA_COUNTER_RESET))
    {
        MotorEncoderA_count32SoftPart = 0;
    }
    
    /* User code required at end of ISR */
    /* `#START MotorEncoderA_ISR_END` */

    /* `#END` */

    #ifdef MotorEncoderA_ISR_EXIT_CALLBACK
        MotorEncoderA_ISR_ExitCallback();
    #endif /* MotorEncoderA_ISR_EXIT_CALLBACK */
}


/* [] END OF FILE */

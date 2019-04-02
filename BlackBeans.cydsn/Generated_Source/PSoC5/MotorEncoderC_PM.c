/*******************************************************************************
* File Name: MotorEncoderC_PM.c
* Version 3.0
*
* Description:
*  This file contains the setup, control and status commands to support 
*  component operations in low power mode.  
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

static MotorEncoderC_BACKUP_STRUCT MotorEncoderC_backup = {0u};


/*******************************************************************************
* Function Name: MotorEncoderC_SaveConfig
********************************************************************************
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void MotorEncoderC_SaveConfig(void) 
{
    #if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT)
        MotorEncoderC_Cnt8_SaveConfig();
    #else 
        /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_16_BIT) || 
         * (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT)
         */
        MotorEncoderC_Cnt16_SaveConfig();
    #endif /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: MotorEncoderC_RestoreConfig
********************************************************************************
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void MotorEncoderC_RestoreConfig(void) 
{
    #if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT)
        MotorEncoderC_Cnt8_RestoreConfig();
    #else 
        /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_16_BIT) || 
         * (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT) 
         */
        MotorEncoderC_Cnt16_RestoreConfig();
    #endif /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: MotorEncoderC_Sleep
********************************************************************************
* 
* Summary:
*  Prepare Quadrature Decoder Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  MotorEncoderC_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MotorEncoderC_Sleep(void) 
{
    if (0u != (MotorEncoderC_SR_AUX_CONTROL & MotorEncoderC_INTERRUPTS_ENABLE))
    {
        MotorEncoderC_backup.enableState = 1u;
    }
    else /* The Quadrature Decoder Component is disabled */
    {
        MotorEncoderC_backup.enableState = 0u;
    }

    MotorEncoderC_Stop();
    MotorEncoderC_SaveConfig();
}


/*******************************************************************************
* Function Name: MotorEncoderC_Wakeup
********************************************************************************
*
* Summary:
*  Prepare Quadrature Decoder Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  MotorEncoderC_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void MotorEncoderC_Wakeup(void) 
{
    MotorEncoderC_RestoreConfig();

    if (MotorEncoderC_backup.enableState != 0u)
    {
        #if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT)
            MotorEncoderC_Cnt8_Enable();
        #else 
            /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_16_BIT) || 
            *  (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT) 
            */
            MotorEncoderC_Cnt16_Enable();
        #endif /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT) */

        /* Enable component's operation */
        MotorEncoderC_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */


/*******************************************************************************
* File Name: MotorEncoderB_PM.c
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

#include "MotorEncoderB.h"

static MotorEncoderB_BACKUP_STRUCT MotorEncoderB_backup = {0u};


/*******************************************************************************
* Function Name: MotorEncoderB_SaveConfig
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
void MotorEncoderB_SaveConfig(void) 
{
    #if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT)
        MotorEncoderB_Cnt8_SaveConfig();
    #else 
        /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_16_BIT) || 
         * (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT)
         */
        MotorEncoderB_Cnt16_SaveConfig();
    #endif /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: MotorEncoderB_RestoreConfig
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
void MotorEncoderB_RestoreConfig(void) 
{
    #if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT)
        MotorEncoderB_Cnt8_RestoreConfig();
    #else 
        /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_16_BIT) || 
         * (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT) 
         */
        MotorEncoderB_Cnt16_RestoreConfig();
    #endif /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: MotorEncoderB_Sleep
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
*  MotorEncoderB_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MotorEncoderB_Sleep(void) 
{
    if (0u != (MotorEncoderB_SR_AUX_CONTROL & MotorEncoderB_INTERRUPTS_ENABLE))
    {
        MotorEncoderB_backup.enableState = 1u;
    }
    else /* The Quadrature Decoder Component is disabled */
    {
        MotorEncoderB_backup.enableState = 0u;
    }

    MotorEncoderB_Stop();
    MotorEncoderB_SaveConfig();
}


/*******************************************************************************
* Function Name: MotorEncoderB_Wakeup
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
*  MotorEncoderB_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void MotorEncoderB_Wakeup(void) 
{
    MotorEncoderB_RestoreConfig();

    if (MotorEncoderB_backup.enableState != 0u)
    {
        #if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT)
            MotorEncoderB_Cnt8_Enable();
        #else 
            /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_16_BIT) || 
            *  (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT) 
            */
            MotorEncoderB_Cnt16_Enable();
        #endif /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT) */

        /* Enable component's operation */
        MotorEncoderB_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */


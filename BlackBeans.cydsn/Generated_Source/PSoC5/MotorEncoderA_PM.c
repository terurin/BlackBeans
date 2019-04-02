/*******************************************************************************
* File Name: MotorEncoderA_PM.c
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

#include "MotorEncoderA.h"

static MotorEncoderA_BACKUP_STRUCT MotorEncoderA_backup = {0u};


/*******************************************************************************
* Function Name: MotorEncoderA_SaveConfig
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
void MotorEncoderA_SaveConfig(void) 
{
    #if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT)
        MotorEncoderA_Cnt8_SaveConfig();
    #else 
        /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_16_BIT) || 
         * (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT)
         */
        MotorEncoderA_Cnt16_SaveConfig();
    #endif /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: MotorEncoderA_RestoreConfig
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
void MotorEncoderA_RestoreConfig(void) 
{
    #if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT)
        MotorEncoderA_Cnt8_RestoreConfig();
    #else 
        /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_16_BIT) || 
         * (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT) 
         */
        MotorEncoderA_Cnt16_RestoreConfig();
    #endif /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: MotorEncoderA_Sleep
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
*  MotorEncoderA_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MotorEncoderA_Sleep(void) 
{
    if (0u != (MotorEncoderA_SR_AUX_CONTROL & MotorEncoderA_INTERRUPTS_ENABLE))
    {
        MotorEncoderA_backup.enableState = 1u;
    }
    else /* The Quadrature Decoder Component is disabled */
    {
        MotorEncoderA_backup.enableState = 0u;
    }

    MotorEncoderA_Stop();
    MotorEncoderA_SaveConfig();
}


/*******************************************************************************
* Function Name: MotorEncoderA_Wakeup
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
*  MotorEncoderA_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void MotorEncoderA_Wakeup(void) 
{
    MotorEncoderA_RestoreConfig();

    if (MotorEncoderA_backup.enableState != 0u)
    {
        #if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT)
            MotorEncoderA_Cnt8_Enable();
        #else 
            /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_16_BIT) || 
            *  (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT) 
            */
            MotorEncoderA_Cnt16_Enable();
        #endif /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT) */

        /* Enable component's operation */
        MotorEncoderA_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */


/*******************************************************************************
* File Name: BLDCMotor_1_QuadEncoder_PM.c
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

#include "BLDCMotor_1_QuadEncoder.h"

static BLDCMotor_1_QuadEncoder_BACKUP_STRUCT BLDCMotor_1_QuadEncoder_backup = {0u};


/*******************************************************************************
* Function Name: BLDCMotor_1_QuadEncoder_SaveConfig
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
void BLDCMotor_1_QuadEncoder_SaveConfig(void) 
{
    #if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT)
        BLDCMotor_1_QuadEncoder_Cnt8_SaveConfig();
    #else 
        /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_16_BIT) || 
         * (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT)
         */
        BLDCMotor_1_QuadEncoder_Cnt16_SaveConfig();
    #endif /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: BLDCMotor_1_QuadEncoder_RestoreConfig
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
void BLDCMotor_1_QuadEncoder_RestoreConfig(void) 
{
    #if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT)
        BLDCMotor_1_QuadEncoder_Cnt8_RestoreConfig();
    #else 
        /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_16_BIT) || 
         * (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT) 
         */
        BLDCMotor_1_QuadEncoder_Cnt16_RestoreConfig();
    #endif /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: BLDCMotor_1_QuadEncoder_Sleep
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
*  BLDCMotor_1_QuadEncoder_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void BLDCMotor_1_QuadEncoder_Sleep(void) 
{
    if (0u != (BLDCMotor_1_QuadEncoder_SR_AUX_CONTROL & BLDCMotor_1_QuadEncoder_INTERRUPTS_ENABLE))
    {
        BLDCMotor_1_QuadEncoder_backup.enableState = 1u;
    }
    else /* The Quadrature Decoder Component is disabled */
    {
        BLDCMotor_1_QuadEncoder_backup.enableState = 0u;
    }

    BLDCMotor_1_QuadEncoder_Stop();
    BLDCMotor_1_QuadEncoder_SaveConfig();
}


/*******************************************************************************
* Function Name: BLDCMotor_1_QuadEncoder_Wakeup
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
*  BLDCMotor_1_QuadEncoder_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void BLDCMotor_1_QuadEncoder_Wakeup(void) 
{
    BLDCMotor_1_QuadEncoder_RestoreConfig();

    if (BLDCMotor_1_QuadEncoder_backup.enableState != 0u)
    {
        #if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT)
            BLDCMotor_1_QuadEncoder_Cnt8_Enable();
        #else 
            /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_16_BIT) || 
            *  (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT) 
            */
            BLDCMotor_1_QuadEncoder_Cnt16_Enable();
        #endif /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT) */

        /* Enable component's operation */
        BLDCMotor_1_QuadEncoder_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */


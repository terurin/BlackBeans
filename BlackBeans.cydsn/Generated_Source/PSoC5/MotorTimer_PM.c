/*******************************************************************************
* File Name: MotorTimer_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "MotorTimer.h"

static MotorTimer_backupStruct MotorTimer_backup;


/*******************************************************************************
* Function Name: MotorTimer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  MotorTimer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void MotorTimer_SaveConfig(void) 
{
    #if (!MotorTimer_UsingFixedFunction)
        MotorTimer_backup.TimerUdb = MotorTimer_ReadCounter();
        MotorTimer_backup.InterruptMaskValue = MotorTimer_STATUS_MASK;
        #if (MotorTimer_UsingHWCaptureCounter)
            MotorTimer_backup.TimerCaptureCounter = MotorTimer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!MotorTimer_UDB_CONTROL_REG_REMOVED)
            MotorTimer_backup.TimerControlRegister = MotorTimer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: MotorTimer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  MotorTimer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void MotorTimer_RestoreConfig(void) 
{   
    #if (!MotorTimer_UsingFixedFunction)

        MotorTimer_WriteCounter(MotorTimer_backup.TimerUdb);
        MotorTimer_STATUS_MASK =MotorTimer_backup.InterruptMaskValue;
        #if (MotorTimer_UsingHWCaptureCounter)
            MotorTimer_SetCaptureCount(MotorTimer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!MotorTimer_UDB_CONTROL_REG_REMOVED)
            MotorTimer_WriteControlRegister(MotorTimer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: MotorTimer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  MotorTimer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void MotorTimer_Sleep(void) 
{
    #if(!MotorTimer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(MotorTimer_CTRL_ENABLE == (MotorTimer_CONTROL & MotorTimer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            MotorTimer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            MotorTimer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    MotorTimer_Stop();
    MotorTimer_SaveConfig();
}


/*******************************************************************************
* Function Name: MotorTimer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  MotorTimer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void MotorTimer_Wakeup(void) 
{
    MotorTimer_RestoreConfig();
    #if(!MotorTimer_UDB_CONTROL_REG_REMOVED)
        if(MotorTimer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                MotorTimer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */

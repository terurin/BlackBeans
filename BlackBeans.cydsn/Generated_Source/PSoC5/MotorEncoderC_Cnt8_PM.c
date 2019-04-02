/*******************************************************************************
* File Name: MotorEncoderC_Cnt8_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "MotorEncoderC_Cnt8.h"

static MotorEncoderC_Cnt8_backupStruct MotorEncoderC_Cnt8_backup;


/*******************************************************************************
* Function Name: MotorEncoderC_Cnt8_SaveConfig
********************************************************************************
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
*  MotorEncoderC_Cnt8_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void MotorEncoderC_Cnt8_SaveConfig(void) 
{
    #if (!MotorEncoderC_Cnt8_UsingFixedFunction)

        MotorEncoderC_Cnt8_backup.CounterUdb = MotorEncoderC_Cnt8_ReadCounter();

        #if(!MotorEncoderC_Cnt8_ControlRegRemoved)
            MotorEncoderC_Cnt8_backup.CounterControlRegister = MotorEncoderC_Cnt8_ReadControlRegister();
        #endif /* (!MotorEncoderC_Cnt8_ControlRegRemoved) */

    #endif /* (!MotorEncoderC_Cnt8_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderC_Cnt8_RestoreConfig
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
*  MotorEncoderC_Cnt8_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void MotorEncoderC_Cnt8_RestoreConfig(void) 
{      
    #if (!MotorEncoderC_Cnt8_UsingFixedFunction)

       MotorEncoderC_Cnt8_WriteCounter(MotorEncoderC_Cnt8_backup.CounterUdb);

        #if(!MotorEncoderC_Cnt8_ControlRegRemoved)
            MotorEncoderC_Cnt8_WriteControlRegister(MotorEncoderC_Cnt8_backup.CounterControlRegister);
        #endif /* (!MotorEncoderC_Cnt8_ControlRegRemoved) */

    #endif /* (!MotorEncoderC_Cnt8_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderC_Cnt8_Sleep
********************************************************************************
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
*  MotorEncoderC_Cnt8_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void MotorEncoderC_Cnt8_Sleep(void) 
{
    #if(!MotorEncoderC_Cnt8_ControlRegRemoved)
        /* Save Counter's enable state */
        if(MotorEncoderC_Cnt8_CTRL_ENABLE == (MotorEncoderC_Cnt8_CONTROL & MotorEncoderC_Cnt8_CTRL_ENABLE))
        {
            /* Counter is enabled */
            MotorEncoderC_Cnt8_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            MotorEncoderC_Cnt8_backup.CounterEnableState = 0u;
        }
    #else
        MotorEncoderC_Cnt8_backup.CounterEnableState = 1u;
        if(MotorEncoderC_Cnt8_backup.CounterEnableState != 0u)
        {
            MotorEncoderC_Cnt8_backup.CounterEnableState = 0u;
        }
    #endif /* (!MotorEncoderC_Cnt8_ControlRegRemoved) */
    
    MotorEncoderC_Cnt8_Stop();
    MotorEncoderC_Cnt8_SaveConfig();
}


/*******************************************************************************
* Function Name: MotorEncoderC_Cnt8_Wakeup
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
*  MotorEncoderC_Cnt8_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void MotorEncoderC_Cnt8_Wakeup(void) 
{
    MotorEncoderC_Cnt8_RestoreConfig();
    #if(!MotorEncoderC_Cnt8_ControlRegRemoved)
        if(MotorEncoderC_Cnt8_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            MotorEncoderC_Cnt8_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!MotorEncoderC_Cnt8_ControlRegRemoved) */
    
}


/* [] END OF FILE */

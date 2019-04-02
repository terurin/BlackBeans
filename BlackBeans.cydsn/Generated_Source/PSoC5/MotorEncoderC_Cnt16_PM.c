/*******************************************************************************
* File Name: MotorEncoderC_Cnt16_PM.c  
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

#include "MotorEncoderC_Cnt16.h"

static MotorEncoderC_Cnt16_backupStruct MotorEncoderC_Cnt16_backup;


/*******************************************************************************
* Function Name: MotorEncoderC_Cnt16_SaveConfig
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
*  MotorEncoderC_Cnt16_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void MotorEncoderC_Cnt16_SaveConfig(void) 
{
    #if (!MotorEncoderC_Cnt16_UsingFixedFunction)

        MotorEncoderC_Cnt16_backup.CounterUdb = MotorEncoderC_Cnt16_ReadCounter();

        #if(!MotorEncoderC_Cnt16_ControlRegRemoved)
            MotorEncoderC_Cnt16_backup.CounterControlRegister = MotorEncoderC_Cnt16_ReadControlRegister();
        #endif /* (!MotorEncoderC_Cnt16_ControlRegRemoved) */

    #endif /* (!MotorEncoderC_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderC_Cnt16_RestoreConfig
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
*  MotorEncoderC_Cnt16_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void MotorEncoderC_Cnt16_RestoreConfig(void) 
{      
    #if (!MotorEncoderC_Cnt16_UsingFixedFunction)

       MotorEncoderC_Cnt16_WriteCounter(MotorEncoderC_Cnt16_backup.CounterUdb);

        #if(!MotorEncoderC_Cnt16_ControlRegRemoved)
            MotorEncoderC_Cnt16_WriteControlRegister(MotorEncoderC_Cnt16_backup.CounterControlRegister);
        #endif /* (!MotorEncoderC_Cnt16_ControlRegRemoved) */

    #endif /* (!MotorEncoderC_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderC_Cnt16_Sleep
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
*  MotorEncoderC_Cnt16_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void MotorEncoderC_Cnt16_Sleep(void) 
{
    #if(!MotorEncoderC_Cnt16_ControlRegRemoved)
        /* Save Counter's enable state */
        if(MotorEncoderC_Cnt16_CTRL_ENABLE == (MotorEncoderC_Cnt16_CONTROL & MotorEncoderC_Cnt16_CTRL_ENABLE))
        {
            /* Counter is enabled */
            MotorEncoderC_Cnt16_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            MotorEncoderC_Cnt16_backup.CounterEnableState = 0u;
        }
    #else
        MotorEncoderC_Cnt16_backup.CounterEnableState = 1u;
        if(MotorEncoderC_Cnt16_backup.CounterEnableState != 0u)
        {
            MotorEncoderC_Cnt16_backup.CounterEnableState = 0u;
        }
    #endif /* (!MotorEncoderC_Cnt16_ControlRegRemoved) */
    
    MotorEncoderC_Cnt16_Stop();
    MotorEncoderC_Cnt16_SaveConfig();
}


/*******************************************************************************
* Function Name: MotorEncoderC_Cnt16_Wakeup
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
*  MotorEncoderC_Cnt16_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void MotorEncoderC_Cnt16_Wakeup(void) 
{
    MotorEncoderC_Cnt16_RestoreConfig();
    #if(!MotorEncoderC_Cnt16_ControlRegRemoved)
        if(MotorEncoderC_Cnt16_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            MotorEncoderC_Cnt16_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!MotorEncoderC_Cnt16_ControlRegRemoved) */
    
}


/* [] END OF FILE */

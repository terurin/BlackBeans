/*******************************************************************************
* File Name: MotorEncoderB_Cnt16_PM.c  
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

#include "MotorEncoderB_Cnt16.h"

static MotorEncoderB_Cnt16_backupStruct MotorEncoderB_Cnt16_backup;


/*******************************************************************************
* Function Name: MotorEncoderB_Cnt16_SaveConfig
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
*  MotorEncoderB_Cnt16_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void MotorEncoderB_Cnt16_SaveConfig(void) 
{
    #if (!MotorEncoderB_Cnt16_UsingFixedFunction)

        MotorEncoderB_Cnt16_backup.CounterUdb = MotorEncoderB_Cnt16_ReadCounter();

        #if(!MotorEncoderB_Cnt16_ControlRegRemoved)
            MotorEncoderB_Cnt16_backup.CounterControlRegister = MotorEncoderB_Cnt16_ReadControlRegister();
        #endif /* (!MotorEncoderB_Cnt16_ControlRegRemoved) */

    #endif /* (!MotorEncoderB_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderB_Cnt16_RestoreConfig
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
*  MotorEncoderB_Cnt16_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void MotorEncoderB_Cnt16_RestoreConfig(void) 
{      
    #if (!MotorEncoderB_Cnt16_UsingFixedFunction)

       MotorEncoderB_Cnt16_WriteCounter(MotorEncoderB_Cnt16_backup.CounterUdb);

        #if(!MotorEncoderB_Cnt16_ControlRegRemoved)
            MotorEncoderB_Cnt16_WriteControlRegister(MotorEncoderB_Cnt16_backup.CounterControlRegister);
        #endif /* (!MotorEncoderB_Cnt16_ControlRegRemoved) */

    #endif /* (!MotorEncoderB_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderB_Cnt16_Sleep
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
*  MotorEncoderB_Cnt16_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void MotorEncoderB_Cnt16_Sleep(void) 
{
    #if(!MotorEncoderB_Cnt16_ControlRegRemoved)
        /* Save Counter's enable state */
        if(MotorEncoderB_Cnt16_CTRL_ENABLE == (MotorEncoderB_Cnt16_CONTROL & MotorEncoderB_Cnt16_CTRL_ENABLE))
        {
            /* Counter is enabled */
            MotorEncoderB_Cnt16_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            MotorEncoderB_Cnt16_backup.CounterEnableState = 0u;
        }
    #else
        MotorEncoderB_Cnt16_backup.CounterEnableState = 1u;
        if(MotorEncoderB_Cnt16_backup.CounterEnableState != 0u)
        {
            MotorEncoderB_Cnt16_backup.CounterEnableState = 0u;
        }
    #endif /* (!MotorEncoderB_Cnt16_ControlRegRemoved) */
    
    MotorEncoderB_Cnt16_Stop();
    MotorEncoderB_Cnt16_SaveConfig();
}


/*******************************************************************************
* Function Name: MotorEncoderB_Cnt16_Wakeup
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
*  MotorEncoderB_Cnt16_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void MotorEncoderB_Cnt16_Wakeup(void) 
{
    MotorEncoderB_Cnt16_RestoreConfig();
    #if(!MotorEncoderB_Cnt16_ControlRegRemoved)
        if(MotorEncoderB_Cnt16_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            MotorEncoderB_Cnt16_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!MotorEncoderB_Cnt16_ControlRegRemoved) */
    
}


/* [] END OF FILE */

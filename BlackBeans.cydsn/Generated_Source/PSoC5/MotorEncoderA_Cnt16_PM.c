/*******************************************************************************
* File Name: MotorEncoderA_Cnt16_PM.c  
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

#include "MotorEncoderA_Cnt16.h"

static MotorEncoderA_Cnt16_backupStruct MotorEncoderA_Cnt16_backup;


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt16_SaveConfig
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
*  MotorEncoderA_Cnt16_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void MotorEncoderA_Cnt16_SaveConfig(void) 
{
    #if (!MotorEncoderA_Cnt16_UsingFixedFunction)

        MotorEncoderA_Cnt16_backup.CounterUdb = MotorEncoderA_Cnt16_ReadCounter();

        #if(!MotorEncoderA_Cnt16_ControlRegRemoved)
            MotorEncoderA_Cnt16_backup.CounterControlRegister = MotorEncoderA_Cnt16_ReadControlRegister();
        #endif /* (!MotorEncoderA_Cnt16_ControlRegRemoved) */

    #endif /* (!MotorEncoderA_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt16_RestoreConfig
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
*  MotorEncoderA_Cnt16_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void MotorEncoderA_Cnt16_RestoreConfig(void) 
{      
    #if (!MotorEncoderA_Cnt16_UsingFixedFunction)

       MotorEncoderA_Cnt16_WriteCounter(MotorEncoderA_Cnt16_backup.CounterUdb);

        #if(!MotorEncoderA_Cnt16_ControlRegRemoved)
            MotorEncoderA_Cnt16_WriteControlRegister(MotorEncoderA_Cnt16_backup.CounterControlRegister);
        #endif /* (!MotorEncoderA_Cnt16_ControlRegRemoved) */

    #endif /* (!MotorEncoderA_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt16_Sleep
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
*  MotorEncoderA_Cnt16_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void MotorEncoderA_Cnt16_Sleep(void) 
{
    #if(!MotorEncoderA_Cnt16_ControlRegRemoved)
        /* Save Counter's enable state */
        if(MotorEncoderA_Cnt16_CTRL_ENABLE == (MotorEncoderA_Cnt16_CONTROL & MotorEncoderA_Cnt16_CTRL_ENABLE))
        {
            /* Counter is enabled */
            MotorEncoderA_Cnt16_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            MotorEncoderA_Cnt16_backup.CounterEnableState = 0u;
        }
    #else
        MotorEncoderA_Cnt16_backup.CounterEnableState = 1u;
        if(MotorEncoderA_Cnt16_backup.CounterEnableState != 0u)
        {
            MotorEncoderA_Cnt16_backup.CounterEnableState = 0u;
        }
    #endif /* (!MotorEncoderA_Cnt16_ControlRegRemoved) */
    
    MotorEncoderA_Cnt16_Stop();
    MotorEncoderA_Cnt16_SaveConfig();
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt16_Wakeup
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
*  MotorEncoderA_Cnt16_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void MotorEncoderA_Cnt16_Wakeup(void) 
{
    MotorEncoderA_Cnt16_RestoreConfig();
    #if(!MotorEncoderA_Cnt16_ControlRegRemoved)
        if(MotorEncoderA_Cnt16_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            MotorEncoderA_Cnt16_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!MotorEncoderA_Cnt16_ControlRegRemoved) */
    
}


/* [] END OF FILE */

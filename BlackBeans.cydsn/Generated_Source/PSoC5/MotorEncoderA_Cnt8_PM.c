/*******************************************************************************
* File Name: MotorEncoderA_Cnt8_PM.c  
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

#include "MotorEncoderA_Cnt8.h"

static MotorEncoderA_Cnt8_backupStruct MotorEncoderA_Cnt8_backup;


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_SaveConfig
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
*  MotorEncoderA_Cnt8_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void MotorEncoderA_Cnt8_SaveConfig(void) 
{
    #if (!MotorEncoderA_Cnt8_UsingFixedFunction)

        MotorEncoderA_Cnt8_backup.CounterUdb = MotorEncoderA_Cnt8_ReadCounter();

        #if(!MotorEncoderA_Cnt8_ControlRegRemoved)
            MotorEncoderA_Cnt8_backup.CounterControlRegister = MotorEncoderA_Cnt8_ReadControlRegister();
        #endif /* (!MotorEncoderA_Cnt8_ControlRegRemoved) */

    #endif /* (!MotorEncoderA_Cnt8_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_RestoreConfig
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
*  MotorEncoderA_Cnt8_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void MotorEncoderA_Cnt8_RestoreConfig(void) 
{      
    #if (!MotorEncoderA_Cnt8_UsingFixedFunction)

       MotorEncoderA_Cnt8_WriteCounter(MotorEncoderA_Cnt8_backup.CounterUdb);

        #if(!MotorEncoderA_Cnt8_ControlRegRemoved)
            MotorEncoderA_Cnt8_WriteControlRegister(MotorEncoderA_Cnt8_backup.CounterControlRegister);
        #endif /* (!MotorEncoderA_Cnt8_ControlRegRemoved) */

    #endif /* (!MotorEncoderA_Cnt8_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_Sleep
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
*  MotorEncoderA_Cnt8_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void MotorEncoderA_Cnt8_Sleep(void) 
{
    #if(!MotorEncoderA_Cnt8_ControlRegRemoved)
        /* Save Counter's enable state */
        if(MotorEncoderA_Cnt8_CTRL_ENABLE == (MotorEncoderA_Cnt8_CONTROL & MotorEncoderA_Cnt8_CTRL_ENABLE))
        {
            /* Counter is enabled */
            MotorEncoderA_Cnt8_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            MotorEncoderA_Cnt8_backup.CounterEnableState = 0u;
        }
    #else
        MotorEncoderA_Cnt8_backup.CounterEnableState = 1u;
        if(MotorEncoderA_Cnt8_backup.CounterEnableState != 0u)
        {
            MotorEncoderA_Cnt8_backup.CounterEnableState = 0u;
        }
    #endif /* (!MotorEncoderA_Cnt8_ControlRegRemoved) */
    
    MotorEncoderA_Cnt8_Stop();
    MotorEncoderA_Cnt8_SaveConfig();
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_Wakeup
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
*  MotorEncoderA_Cnt8_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void MotorEncoderA_Cnt8_Wakeup(void) 
{
    MotorEncoderA_Cnt8_RestoreConfig();
    #if(!MotorEncoderA_Cnt8_ControlRegRemoved)
        if(MotorEncoderA_Cnt8_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            MotorEncoderA_Cnt8_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!MotorEncoderA_Cnt8_ControlRegRemoved) */
    
}


/* [] END OF FILE */

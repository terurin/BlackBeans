/*******************************************************************************
* File Name: BLDCMotor_1_QEIEncoder_PM.c  
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

#include "BLDCMotor_1_QEIEncoder.h"

static BLDCMotor_1_QEIEncoder_backupStruct BLDCMotor_1_QEIEncoder_backup;


/*******************************************************************************
* Function Name: BLDCMotor_1_QEIEncoder_SaveConfig
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
*  BLDCMotor_1_QEIEncoder_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void BLDCMotor_1_QEIEncoder_SaveConfig(void) 
{
    #if (!BLDCMotor_1_QEIEncoder_UsingFixedFunction)

        BLDCMotor_1_QEIEncoder_backup.CounterUdb = BLDCMotor_1_QEIEncoder_ReadCounter();

        #if(!BLDCMotor_1_QEIEncoder_ControlRegRemoved)
            BLDCMotor_1_QEIEncoder_backup.CounterControlRegister = BLDCMotor_1_QEIEncoder_ReadControlRegister();
        #endif /* (!BLDCMotor_1_QEIEncoder_ControlRegRemoved) */

    #endif /* (!BLDCMotor_1_QEIEncoder_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCMotor_1_QEIEncoder_RestoreConfig
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
*  BLDCMotor_1_QEIEncoder_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void BLDCMotor_1_QEIEncoder_RestoreConfig(void) 
{      
    #if (!BLDCMotor_1_QEIEncoder_UsingFixedFunction)

       BLDCMotor_1_QEIEncoder_WriteCounter(BLDCMotor_1_QEIEncoder_backup.CounterUdb);

        #if(!BLDCMotor_1_QEIEncoder_ControlRegRemoved)
            BLDCMotor_1_QEIEncoder_WriteControlRegister(BLDCMotor_1_QEIEncoder_backup.CounterControlRegister);
        #endif /* (!BLDCMotor_1_QEIEncoder_ControlRegRemoved) */

    #endif /* (!BLDCMotor_1_QEIEncoder_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCMotor_1_QEIEncoder_Sleep
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
*  BLDCMotor_1_QEIEncoder_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void BLDCMotor_1_QEIEncoder_Sleep(void) 
{
    #if(!BLDCMotor_1_QEIEncoder_ControlRegRemoved)
        /* Save Counter's enable state */
        if(BLDCMotor_1_QEIEncoder_CTRL_ENABLE == (BLDCMotor_1_QEIEncoder_CONTROL & BLDCMotor_1_QEIEncoder_CTRL_ENABLE))
        {
            /* Counter is enabled */
            BLDCMotor_1_QEIEncoder_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            BLDCMotor_1_QEIEncoder_backup.CounterEnableState = 0u;
        }
    #else
        BLDCMotor_1_QEIEncoder_backup.CounterEnableState = 1u;
        if(BLDCMotor_1_QEIEncoder_backup.CounterEnableState != 0u)
        {
            BLDCMotor_1_QEIEncoder_backup.CounterEnableState = 0u;
        }
    #endif /* (!BLDCMotor_1_QEIEncoder_ControlRegRemoved) */
    
    BLDCMotor_1_QEIEncoder_Stop();
    BLDCMotor_1_QEIEncoder_SaveConfig();
}


/*******************************************************************************
* Function Name: BLDCMotor_1_QEIEncoder_Wakeup
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
*  BLDCMotor_1_QEIEncoder_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void BLDCMotor_1_QEIEncoder_Wakeup(void) 
{
    BLDCMotor_1_QEIEncoder_RestoreConfig();
    #if(!BLDCMotor_1_QEIEncoder_ControlRegRemoved)
        if(BLDCMotor_1_QEIEncoder_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            BLDCMotor_1_QEIEncoder_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!BLDCMotor_1_QEIEncoder_ControlRegRemoved) */
    
}


/* [] END OF FILE */

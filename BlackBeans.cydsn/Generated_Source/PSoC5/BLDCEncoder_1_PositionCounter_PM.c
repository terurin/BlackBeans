/*******************************************************************************
* File Name: BLDCEncoder_1_PositionCounter_PM.c  
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

#include "BLDCEncoder_1_PositionCounter.h"

static BLDCEncoder_1_PositionCounter_backupStruct BLDCEncoder_1_PositionCounter_backup;


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_SaveConfig
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
*  BLDCEncoder_1_PositionCounter_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void BLDCEncoder_1_PositionCounter_SaveConfig(void) 
{
    #if (!BLDCEncoder_1_PositionCounter_UsingFixedFunction)

        BLDCEncoder_1_PositionCounter_backup.CounterUdb = BLDCEncoder_1_PositionCounter_ReadCounter();

        #if(!BLDCEncoder_1_PositionCounter_ControlRegRemoved)
            BLDCEncoder_1_PositionCounter_backup.CounterControlRegister = BLDCEncoder_1_PositionCounter_ReadControlRegister();
        #endif /* (!BLDCEncoder_1_PositionCounter_ControlRegRemoved) */

    #endif /* (!BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_RestoreConfig
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
*  BLDCEncoder_1_PositionCounter_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void BLDCEncoder_1_PositionCounter_RestoreConfig(void) 
{      
    #if (!BLDCEncoder_1_PositionCounter_UsingFixedFunction)

       BLDCEncoder_1_PositionCounter_WriteCounter(BLDCEncoder_1_PositionCounter_backup.CounterUdb);

        #if(!BLDCEncoder_1_PositionCounter_ControlRegRemoved)
            BLDCEncoder_1_PositionCounter_WriteControlRegister(BLDCEncoder_1_PositionCounter_backup.CounterControlRegister);
        #endif /* (!BLDCEncoder_1_PositionCounter_ControlRegRemoved) */

    #endif /* (!BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_Sleep
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
*  BLDCEncoder_1_PositionCounter_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void BLDCEncoder_1_PositionCounter_Sleep(void) 
{
    #if(!BLDCEncoder_1_PositionCounter_ControlRegRemoved)
        /* Save Counter's enable state */
        if(BLDCEncoder_1_PositionCounter_CTRL_ENABLE == (BLDCEncoder_1_PositionCounter_CONTROL & BLDCEncoder_1_PositionCounter_CTRL_ENABLE))
        {
            /* Counter is enabled */
            BLDCEncoder_1_PositionCounter_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            BLDCEncoder_1_PositionCounter_backup.CounterEnableState = 0u;
        }
    #else
        BLDCEncoder_1_PositionCounter_backup.CounterEnableState = 1u;
        if(BLDCEncoder_1_PositionCounter_backup.CounterEnableState != 0u)
        {
            BLDCEncoder_1_PositionCounter_backup.CounterEnableState = 0u;
        }
    #endif /* (!BLDCEncoder_1_PositionCounter_ControlRegRemoved) */
    
    BLDCEncoder_1_PositionCounter_Stop();
    BLDCEncoder_1_PositionCounter_SaveConfig();
}


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_Wakeup
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
*  BLDCEncoder_1_PositionCounter_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void BLDCEncoder_1_PositionCounter_Wakeup(void) 
{
    BLDCEncoder_1_PositionCounter_RestoreConfig();
    #if(!BLDCEncoder_1_PositionCounter_ControlRegRemoved)
        if(BLDCEncoder_1_PositionCounter_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            BLDCEncoder_1_PositionCounter_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!BLDCEncoder_1_PositionCounter_ControlRegRemoved) */
    
}


/* [] END OF FILE */

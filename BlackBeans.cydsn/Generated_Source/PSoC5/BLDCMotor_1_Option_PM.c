/*******************************************************************************
* File Name: BLDCMotor_1_Option_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "BLDCMotor_1_Option.h"

/* Check for removal by optimization */
#if !defined(BLDCMotor_1_Option_Sync_ctrl_reg__REMOVED)

static BLDCMotor_1_Option_BACKUP_STRUCT  BLDCMotor_1_Option_backup = {0u};

    
/*******************************************************************************
* Function Name: BLDCMotor_1_Option_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void BLDCMotor_1_Option_SaveConfig(void) 
{
    BLDCMotor_1_Option_backup.controlState = BLDCMotor_1_Option_Control;
}


/*******************************************************************************
* Function Name: BLDCMotor_1_Option_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void BLDCMotor_1_Option_RestoreConfig(void) 
{
     BLDCMotor_1_Option_Control = BLDCMotor_1_Option_backup.controlState;
}


/*******************************************************************************
* Function Name: BLDCMotor_1_Option_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void BLDCMotor_1_Option_Sleep(void) 
{
    BLDCMotor_1_Option_SaveConfig();
}


/*******************************************************************************
* Function Name: BLDCMotor_1_Option_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void BLDCMotor_1_Option_Wakeup(void)  
{
    BLDCMotor_1_Option_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */

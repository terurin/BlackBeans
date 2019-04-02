/*******************************************************************************
* File Name: BLDCMotor_1_PWM_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "BLDCMotor_1_PWM.h"

static BLDCMotor_1_PWM_backupStruct BLDCMotor_1_PWM_backup;


/*******************************************************************************
* Function Name: BLDCMotor_1_PWM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  BLDCMotor_1_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void BLDCMotor_1_PWM_SaveConfig(void) 
{

    #if(!BLDCMotor_1_PWM_UsingFixedFunction)
        #if(!BLDCMotor_1_PWM_PWMModeIsCenterAligned)
            BLDCMotor_1_PWM_backup.PWMPeriod = BLDCMotor_1_PWM_ReadPeriod();
        #endif /* (!BLDCMotor_1_PWM_PWMModeIsCenterAligned) */
        BLDCMotor_1_PWM_backup.PWMUdb = BLDCMotor_1_PWM_ReadCounter();
        #if (BLDCMotor_1_PWM_UseStatus)
            BLDCMotor_1_PWM_backup.InterruptMaskValue = BLDCMotor_1_PWM_STATUS_MASK;
        #endif /* (BLDCMotor_1_PWM_UseStatus) */

        #if(BLDCMotor_1_PWM_DeadBandMode == BLDCMotor_1_PWM__B_PWM__DBM_256_CLOCKS || \
            BLDCMotor_1_PWM_DeadBandMode == BLDCMotor_1_PWM__B_PWM__DBM_2_4_CLOCKS)
            BLDCMotor_1_PWM_backup.PWMdeadBandValue = BLDCMotor_1_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(BLDCMotor_1_PWM_KillModeMinTime)
             BLDCMotor_1_PWM_backup.PWMKillCounterPeriod = BLDCMotor_1_PWM_ReadKillTime();
        #endif /* (BLDCMotor_1_PWM_KillModeMinTime) */

        #if(BLDCMotor_1_PWM_UseControl)
            BLDCMotor_1_PWM_backup.PWMControlRegister = BLDCMotor_1_PWM_ReadControlRegister();
        #endif /* (BLDCMotor_1_PWM_UseControl) */
    #endif  /* (!BLDCMotor_1_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCMotor_1_PWM_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  BLDCMotor_1_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void BLDCMotor_1_PWM_RestoreConfig(void) 
{
        #if(!BLDCMotor_1_PWM_UsingFixedFunction)
            #if(!BLDCMotor_1_PWM_PWMModeIsCenterAligned)
                BLDCMotor_1_PWM_WritePeriod(BLDCMotor_1_PWM_backup.PWMPeriod);
            #endif /* (!BLDCMotor_1_PWM_PWMModeIsCenterAligned) */

            BLDCMotor_1_PWM_WriteCounter(BLDCMotor_1_PWM_backup.PWMUdb);

            #if (BLDCMotor_1_PWM_UseStatus)
                BLDCMotor_1_PWM_STATUS_MASK = BLDCMotor_1_PWM_backup.InterruptMaskValue;
            #endif /* (BLDCMotor_1_PWM_UseStatus) */

            #if(BLDCMotor_1_PWM_DeadBandMode == BLDCMotor_1_PWM__B_PWM__DBM_256_CLOCKS || \
                BLDCMotor_1_PWM_DeadBandMode == BLDCMotor_1_PWM__B_PWM__DBM_2_4_CLOCKS)
                BLDCMotor_1_PWM_WriteDeadTime(BLDCMotor_1_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(BLDCMotor_1_PWM_KillModeMinTime)
                BLDCMotor_1_PWM_WriteKillTime(BLDCMotor_1_PWM_backup.PWMKillCounterPeriod);
            #endif /* (BLDCMotor_1_PWM_KillModeMinTime) */

            #if(BLDCMotor_1_PWM_UseControl)
                BLDCMotor_1_PWM_WriteControlRegister(BLDCMotor_1_PWM_backup.PWMControlRegister);
            #endif /* (BLDCMotor_1_PWM_UseControl) */
        #endif  /* (!BLDCMotor_1_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: BLDCMotor_1_PWM_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  BLDCMotor_1_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void BLDCMotor_1_PWM_Sleep(void) 
{
    #if(BLDCMotor_1_PWM_UseControl)
        if(BLDCMotor_1_PWM_CTRL_ENABLE == (BLDCMotor_1_PWM_CONTROL & BLDCMotor_1_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            BLDCMotor_1_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            BLDCMotor_1_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (BLDCMotor_1_PWM_UseControl) */

    /* Stop component */
    BLDCMotor_1_PWM_Stop();

    /* Save registers configuration */
    BLDCMotor_1_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: BLDCMotor_1_PWM_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  BLDCMotor_1_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void BLDCMotor_1_PWM_Wakeup(void) 
{
     /* Restore registers values */
    BLDCMotor_1_PWM_RestoreConfig();

    if(BLDCMotor_1_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        BLDCMotor_1_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */

/*******************************************************************************
* File Name: BLDCDriver_1_PulseGenerator_PM.c
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

#include "BLDCDriver_1_PulseGenerator.h"

static BLDCDriver_1_PulseGenerator_backupStruct BLDCDriver_1_PulseGenerator_backup;


/*******************************************************************************
* Function Name: BLDCDriver_1_PulseGenerator_SaveConfig
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
*  BLDCDriver_1_PulseGenerator_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void BLDCDriver_1_PulseGenerator_SaveConfig(void) 
{

    #if(!BLDCDriver_1_PulseGenerator_UsingFixedFunction)
        #if(!BLDCDriver_1_PulseGenerator_PWMModeIsCenterAligned)
            BLDCDriver_1_PulseGenerator_backup.PWMPeriod = BLDCDriver_1_PulseGenerator_ReadPeriod();
        #endif /* (!BLDCDriver_1_PulseGenerator_PWMModeIsCenterAligned) */
        BLDCDriver_1_PulseGenerator_backup.PWMUdb = BLDCDriver_1_PulseGenerator_ReadCounter();
        #if (BLDCDriver_1_PulseGenerator_UseStatus)
            BLDCDriver_1_PulseGenerator_backup.InterruptMaskValue = BLDCDriver_1_PulseGenerator_STATUS_MASK;
        #endif /* (BLDCDriver_1_PulseGenerator_UseStatus) */

        #if(BLDCDriver_1_PulseGenerator_DeadBandMode == BLDCDriver_1_PulseGenerator__B_PWM__DBM_256_CLOCKS || \
            BLDCDriver_1_PulseGenerator_DeadBandMode == BLDCDriver_1_PulseGenerator__B_PWM__DBM_2_4_CLOCKS)
            BLDCDriver_1_PulseGenerator_backup.PWMdeadBandValue = BLDCDriver_1_PulseGenerator_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(BLDCDriver_1_PulseGenerator_KillModeMinTime)
             BLDCDriver_1_PulseGenerator_backup.PWMKillCounterPeriod = BLDCDriver_1_PulseGenerator_ReadKillTime();
        #endif /* (BLDCDriver_1_PulseGenerator_KillModeMinTime) */

        #if(BLDCDriver_1_PulseGenerator_UseControl)
            BLDCDriver_1_PulseGenerator_backup.PWMControlRegister = BLDCDriver_1_PulseGenerator_ReadControlRegister();
        #endif /* (BLDCDriver_1_PulseGenerator_UseControl) */
    #endif  /* (!BLDCDriver_1_PulseGenerator_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCDriver_1_PulseGenerator_RestoreConfig
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
*  BLDCDriver_1_PulseGenerator_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void BLDCDriver_1_PulseGenerator_RestoreConfig(void) 
{
        #if(!BLDCDriver_1_PulseGenerator_UsingFixedFunction)
            #if(!BLDCDriver_1_PulseGenerator_PWMModeIsCenterAligned)
                BLDCDriver_1_PulseGenerator_WritePeriod(BLDCDriver_1_PulseGenerator_backup.PWMPeriod);
            #endif /* (!BLDCDriver_1_PulseGenerator_PWMModeIsCenterAligned) */

            BLDCDriver_1_PulseGenerator_WriteCounter(BLDCDriver_1_PulseGenerator_backup.PWMUdb);

            #if (BLDCDriver_1_PulseGenerator_UseStatus)
                BLDCDriver_1_PulseGenerator_STATUS_MASK = BLDCDriver_1_PulseGenerator_backup.InterruptMaskValue;
            #endif /* (BLDCDriver_1_PulseGenerator_UseStatus) */

            #if(BLDCDriver_1_PulseGenerator_DeadBandMode == BLDCDriver_1_PulseGenerator__B_PWM__DBM_256_CLOCKS || \
                BLDCDriver_1_PulseGenerator_DeadBandMode == BLDCDriver_1_PulseGenerator__B_PWM__DBM_2_4_CLOCKS)
                BLDCDriver_1_PulseGenerator_WriteDeadTime(BLDCDriver_1_PulseGenerator_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(BLDCDriver_1_PulseGenerator_KillModeMinTime)
                BLDCDriver_1_PulseGenerator_WriteKillTime(BLDCDriver_1_PulseGenerator_backup.PWMKillCounterPeriod);
            #endif /* (BLDCDriver_1_PulseGenerator_KillModeMinTime) */

            #if(BLDCDriver_1_PulseGenerator_UseControl)
                BLDCDriver_1_PulseGenerator_WriteControlRegister(BLDCDriver_1_PulseGenerator_backup.PWMControlRegister);
            #endif /* (BLDCDriver_1_PulseGenerator_UseControl) */
        #endif  /* (!BLDCDriver_1_PulseGenerator_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: BLDCDriver_1_PulseGenerator_Sleep
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
*  BLDCDriver_1_PulseGenerator_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void BLDCDriver_1_PulseGenerator_Sleep(void) 
{
    #if(BLDCDriver_1_PulseGenerator_UseControl)
        if(BLDCDriver_1_PulseGenerator_CTRL_ENABLE == (BLDCDriver_1_PulseGenerator_CONTROL & BLDCDriver_1_PulseGenerator_CTRL_ENABLE))
        {
            /*Component is enabled */
            BLDCDriver_1_PulseGenerator_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            BLDCDriver_1_PulseGenerator_backup.PWMEnableState = 0u;
        }
    #endif /* (BLDCDriver_1_PulseGenerator_UseControl) */

    /* Stop component */
    BLDCDriver_1_PulseGenerator_Stop();

    /* Save registers configuration */
    BLDCDriver_1_PulseGenerator_SaveConfig();
}


/*******************************************************************************
* Function Name: BLDCDriver_1_PulseGenerator_Wakeup
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
*  BLDCDriver_1_PulseGenerator_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void BLDCDriver_1_PulseGenerator_Wakeup(void) 
{
     /* Restore registers values */
    BLDCDriver_1_PulseGenerator_RestoreConfig();

    if(BLDCDriver_1_PulseGenerator_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        BLDCDriver_1_PulseGenerator_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */

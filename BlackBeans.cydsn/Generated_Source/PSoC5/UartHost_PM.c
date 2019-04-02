/*******************************************************************************
* File Name: UartHost_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "UartHost.h"


/***************************************
* Local data allocation
***************************************/

static UartHost_BACKUP_STRUCT  UartHost_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: UartHost_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the UartHost_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UartHost_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UartHost_SaveConfig(void)
{
    #if(UartHost_CONTROL_REG_REMOVED == 0u)
        UartHost_backup.cr = UartHost_CONTROL_REG;
    #endif /* End UartHost_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: UartHost_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the nonretention control register except FIFO.
*  Does not restore the FIFO which is a set of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UartHost_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling UartHost_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void UartHost_RestoreConfig(void)
{
    #if(UartHost_CONTROL_REG_REMOVED == 0u)
        UartHost_CONTROL_REG = UartHost_backup.cr;
    #endif /* End UartHost_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: UartHost_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The UartHost_Sleep() API saves the current component state. Then it
*  calls the UartHost_Stop() function and calls 
*  UartHost_SaveConfig() to save the hardware configuration.
*  Call the UartHost_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UartHost_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UartHost_Sleep(void)
{
    #if(UartHost_RX_ENABLED || UartHost_HD_ENABLED)
        if((UartHost_RXSTATUS_ACTL_REG  & UartHost_INT_ENABLE) != 0u)
        {
            UartHost_backup.enableState = 1u;
        }
        else
        {
            UartHost_backup.enableState = 0u;
        }
    #else
        if((UartHost_TXSTATUS_ACTL_REG  & UartHost_INT_ENABLE) !=0u)
        {
            UartHost_backup.enableState = 1u;
        }
        else
        {
            UartHost_backup.enableState = 0u;
        }
    #endif /* End UartHost_RX_ENABLED || UartHost_HD_ENABLED*/

    UartHost_Stop();
    UartHost_SaveConfig();
}


/*******************************************************************************
* Function Name: UartHost_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  UartHost_Sleep() was called. The UartHost_Wakeup() function
*  calls the UartHost_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  UartHost_Sleep() function was called, the UartHost_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UartHost_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UartHost_Wakeup(void)
{
    UartHost_RestoreConfig();
    #if( (UartHost_RX_ENABLED) || (UartHost_HD_ENABLED) )
        UartHost_ClearRxBuffer();
    #endif /* End (UartHost_RX_ENABLED) || (UartHost_HD_ENABLED) */
    #if(UartHost_TX_ENABLED || UartHost_HD_ENABLED)
        UartHost_ClearTxBuffer();
    #endif /* End UartHost_TX_ENABLED || UartHost_HD_ENABLED */

    if(UartHost_backup.enableState != 0u)
    {
        UartHost_Enable();
    }
}


/* [] END OF FILE */

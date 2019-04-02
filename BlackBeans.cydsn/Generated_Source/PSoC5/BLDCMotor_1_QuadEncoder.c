/*******************************************************************************
* File Name: BLDCMotor_1_QuadEncoder.c  
* Version 3.0
*
* Description:
*  This file provides the source code to the API for the Quadrature Decoder
*  component.
*
* Note:
*  None.
*   
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "BLDCMotor_1_QuadEncoder.h"

#if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT)
    #include "BLDCMotor_1_QuadEncoder_PVT.h"
#endif /* BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT */

uint8 BLDCMotor_1_QuadEncoder_initVar = 0u;


/*******************************************************************************
* Function Name: BLDCMotor_1_QuadEncoder_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default QuadDec configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void BLDCMotor_1_QuadEncoder_Init(void) 
{
    #if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT)
        /* Disable Interrupt. */
        CyIntDisable(BLDCMotor_1_QuadEncoder_ISR_NUMBER);
        /* Set the ISR to point to the BLDCMotor_1_QuadEncoder_isr Interrupt. */
        (void) CyIntSetVector(BLDCMotor_1_QuadEncoder_ISR_NUMBER, & BLDCMotor_1_QuadEncoder_ISR);
        /* Set the priority. */
        CyIntSetPriority(BLDCMotor_1_QuadEncoder_ISR_NUMBER, BLDCMotor_1_QuadEncoder_ISR_PRIORITY);
    #endif /* BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: BLDCMotor_1_QuadEncoder_Enable
********************************************************************************
*
* Summary:
*  This function enable interrupts from Component and also enable Component's
*  ISR in case of 32-bit counter.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void BLDCMotor_1_QuadEncoder_Enable(void) 
{
    uint8 enableInterrupts;

    BLDCMotor_1_QuadEncoder_SetInterruptMask(BLDCMotor_1_QuadEncoder_INIT_INT_MASK);

    /* Clear pending interrupts. */
    (void) BLDCMotor_1_QuadEncoder_GetEvents();
    
    enableInterrupts = CyEnterCriticalSection();

    /* Enable interrupts from Statusi register */
    BLDCMotor_1_QuadEncoder_SR_AUX_CONTROL |= BLDCMotor_1_QuadEncoder_INTERRUPTS_ENABLE;

    CyExitCriticalSection(enableInterrupts);        

    #if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT)
        /* Enable Component interrupts */
        CyIntEnable(BLDCMotor_1_QuadEncoder_ISR_NUMBER);
    #endif /* BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: BLDCMotor_1_QuadEncoder_Start
********************************************************************************
*
* Summary:
*  Initializes UDBs and other relevant hardware.
*  Resets counter, enables or disables all relevant interrupts.
*  Starts monitoring the inputs and counting.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  BLDCMotor_1_QuadEncoder_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void BLDCMotor_1_QuadEncoder_Start(void) 
{
    #if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT)
        BLDCMotor_1_QuadEncoder_Cnt8_Start();
        BLDCMotor_1_QuadEncoder_Cnt8_WriteCounter(BLDCMotor_1_QuadEncoder_COUNTER_INIT_VALUE);
    #else
        /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_16_BIT) || 
        *  (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT) 
        */
        BLDCMotor_1_QuadEncoder_Cnt16_Start();
        BLDCMotor_1_QuadEncoder_Cnt16_WriteCounter(BLDCMotor_1_QuadEncoder_COUNTER_INIT_VALUE);
    #endif /* BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT */
    
    #if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT)        
       BLDCMotor_1_QuadEncoder_count32SoftPart = 0;
    #endif /* BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT */

    if (BLDCMotor_1_QuadEncoder_initVar == 0u)
    {
        BLDCMotor_1_QuadEncoder_Init();
        BLDCMotor_1_QuadEncoder_initVar = 1u;
    }

    BLDCMotor_1_QuadEncoder_Enable();
}


/*******************************************************************************
* Function Name: BLDCMotor_1_QuadEncoder_Stop
********************************************************************************
*
* Summary:
*  Turns off UDBs and other relevant hardware.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void BLDCMotor_1_QuadEncoder_Stop(void) 
{
    uint8 enableInterrupts;

    #if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT)
        BLDCMotor_1_QuadEncoder_Cnt8_Stop();
    #else 
        /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_16_BIT) ||
        *  (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT)
        */
        BLDCMotor_1_QuadEncoder_Cnt16_Stop();    /* counter disable */
    #endif /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT) */
 
    enableInterrupts = CyEnterCriticalSection();

    /* Disable interrupts interrupts from Statusi register */
    BLDCMotor_1_QuadEncoder_SR_AUX_CONTROL &= (uint8) (~BLDCMotor_1_QuadEncoder_INTERRUPTS_ENABLE);

    CyExitCriticalSection(enableInterrupts);

    #if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT)
        CyIntDisable(BLDCMotor_1_QuadEncoder_ISR_NUMBER);    /* interrupt disable */
    #endif /* BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: BLDCMotor_1_QuadEncoder_GetCounter
********************************************************************************
*
* Summary:
*  Reports the current value of the counter.
*
* Parameters:
*  None.
*
* Return:
*  The counter value. Return type is signed and per the counter size setting.
*  A positive value indicates clockwise movement (B before A).
*
* Global variables:
*  BLDCMotor_1_QuadEncoder_count32SoftPart - used to get hi 16 bit for current value
*  of the 32-bit counter, when Counter size equal 32-bit.
*
*******************************************************************************/
int8 BLDCMotor_1_QuadEncoder_GetCounter(void) 
{
    int8 count;
    uint8 tmpCnt;

    #if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT)
        int16 hwCount;

        CyIntDisable(BLDCMotor_1_QuadEncoder_ISR_NUMBER);

        tmpCnt = BLDCMotor_1_QuadEncoder_Cnt16_ReadCounter();
        hwCount = (int16) ((int32) tmpCnt - (int32) BLDCMotor_1_QuadEncoder_COUNTER_INIT_VALUE);
        count = BLDCMotor_1_QuadEncoder_count32SoftPart + hwCount;

        CyIntEnable(BLDCMotor_1_QuadEncoder_ISR_NUMBER);
    #else 
        /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT) || 
        *  (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_16_BIT)
        */
        #if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT)
            tmpCnt = BLDCMotor_1_QuadEncoder_Cnt8_ReadCounter();
        #else /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_16_BIT) */
            tmpCnt = BLDCMotor_1_QuadEncoder_Cnt16_ReadCounter();
        #endif  /* BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT */

        count = (int8) ((int16) tmpCnt -
                (int16) BLDCMotor_1_QuadEncoder_COUNTER_INIT_VALUE);

    #endif /* BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT */ 

    return (count);
}


/*******************************************************************************
* Function Name: BLDCMotor_1_QuadEncoder_SetCounter
********************************************************************************
*
* Summary:
*  Sets the current value of the counter.
*
* Parameters:
*  value:  The new value. Parameter type is signed and per the counter size
*  setting.
*
* Return:
*  None.
*
* Global variables:
*  BLDCMotor_1_QuadEncoder_count32SoftPart - modified to set hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void BLDCMotor_1_QuadEncoder_SetCounter(int8 value) 
{
    #if ((BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT) || \
         (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_16_BIT))
        uint8 count;
        
        if (value >= 0)
        {
            count = (uint8) value + BLDCMotor_1_QuadEncoder_COUNTER_INIT_VALUE;
        }
        else
        {
            count = BLDCMotor_1_QuadEncoder_COUNTER_INIT_VALUE - (uint8)(-value);
        }
        #if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT)
            BLDCMotor_1_QuadEncoder_Cnt8_WriteCounter(count);
        #else /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_16_BIT) */
            BLDCMotor_1_QuadEncoder_Cnt16_WriteCounter(count);
        #endif  /* BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT */
    #else /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT) */
        CyIntDisable(BLDCMotor_1_QuadEncoder_ISR_NUMBER);

        BLDCMotor_1_QuadEncoder_Cnt16_WriteCounter(BLDCMotor_1_QuadEncoder_COUNTER_INIT_VALUE);
        BLDCMotor_1_QuadEncoder_count32SoftPart = value;

        CyIntEnable(BLDCMotor_1_QuadEncoder_ISR_NUMBER);
    #endif  /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT) ||
             * (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_16_BIT)
             */
}


/*******************************************************************************
* Function Name: BLDCMotor_1_QuadEncoder_GetEvents
********************************************************************************
* 
* Summary:
*   Reports the current status of events. This function clears the bits of the 
*   status register.
*
* Parameters:
*  None.
*
* Return:
*  The events, as bits in an unsigned 8-bit value:
*    Bit      Description
*     0        Counter overflow.
*     1        Counter underflow.
*     2        Counter reset due to index, if index input is used.
*     3        Invalid A, B inputs state transition.
*
*******************************************************************************/
uint8 BLDCMotor_1_QuadEncoder_GetEvents(void) 
{
    return (BLDCMotor_1_QuadEncoder_STATUS_REG & BLDCMotor_1_QuadEncoder_INIT_INT_MASK);
}


/*******************************************************************************
* Function Name: BLDCMotor_1_QuadEncoder_SetInterruptMask
********************************************************************************
*
* Summary:
*  Enables / disables interrupts due to the events.
*  For the 32-bit counter, the overflow, underflow and reset interrupts cannot
*  be disabled, these bits are ignored.
*
* Parameters:
*  mask: Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*
* Return:
*  None.
*
*******************************************************************************/
void BLDCMotor_1_QuadEncoder_SetInterruptMask(uint8 mask) 
{
    #if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT)
        /* Underflow, Overflow and Reset interrupts for 32-bit Counter are always enable */
        mask |= (BLDCMotor_1_QuadEncoder_COUNTER_OVERFLOW | BLDCMotor_1_QuadEncoder_COUNTER_UNDERFLOW |
                 BLDCMotor_1_QuadEncoder_COUNTER_RESET);
    #endif /* BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT */

    BLDCMotor_1_QuadEncoder_STATUS_MASK = mask;
}


/*******************************************************************************
* Function Name: BLDCMotor_1_QuadEncoder_GetInterruptMask
********************************************************************************
*
* Summary:
*  Reports the current interrupt mask settings.
*
* Parameters:
*  None.
*
* Return:
*  Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*  For the 32-bit counter, the overflow, underflow and reset enable bits are
*  always set.
*
*******************************************************************************/
uint8 BLDCMotor_1_QuadEncoder_GetInterruptMask(void) 
{
    return (BLDCMotor_1_QuadEncoder_STATUS_MASK & BLDCMotor_1_QuadEncoder_INIT_INT_MASK);
}


/* [] END OF FILE */

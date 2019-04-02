/*******************************************************************************
* File Name: MotorEncoderB.c  
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

#include "MotorEncoderB.h"

#if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT)
    #include "MotorEncoderB_PVT.h"
#endif /* MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT */

uint8 MotorEncoderB_initVar = 0u;


/*******************************************************************************
* Function Name: MotorEncoderB_Init
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
void MotorEncoderB_Init(void) 
{
    #if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT)
        /* Disable Interrupt. */
        CyIntDisable(MotorEncoderB_ISR_NUMBER);
        /* Set the ISR to point to the MotorEncoderB_isr Interrupt. */
        (void) CyIntSetVector(MotorEncoderB_ISR_NUMBER, & MotorEncoderB_ISR);
        /* Set the priority. */
        CyIntSetPriority(MotorEncoderB_ISR_NUMBER, MotorEncoderB_ISR_PRIORITY);
    #endif /* MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: MotorEncoderB_Enable
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
void MotorEncoderB_Enable(void) 
{
    uint8 enableInterrupts;

    MotorEncoderB_SetInterruptMask(MotorEncoderB_INIT_INT_MASK);

    /* Clear pending interrupts. */
    (void) MotorEncoderB_GetEvents();
    
    enableInterrupts = CyEnterCriticalSection();

    /* Enable interrupts from Statusi register */
    MotorEncoderB_SR_AUX_CONTROL |= MotorEncoderB_INTERRUPTS_ENABLE;

    CyExitCriticalSection(enableInterrupts);        

    #if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT)
        /* Enable Component interrupts */
        CyIntEnable(MotorEncoderB_ISR_NUMBER);
    #endif /* MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: MotorEncoderB_Start
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
*  MotorEncoderB_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MotorEncoderB_Start(void) 
{
    #if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT)
        MotorEncoderB_Cnt8_Start();
        MotorEncoderB_Cnt8_WriteCounter(MotorEncoderB_COUNTER_INIT_VALUE);
    #else
        /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_16_BIT) || 
        *  (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT) 
        */
        MotorEncoderB_Cnt16_Start();
        MotorEncoderB_Cnt16_WriteCounter(MotorEncoderB_COUNTER_INIT_VALUE);
    #endif /* MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT */
    
    #if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT)        
       MotorEncoderB_count32SoftPart = 0;
    #endif /* MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT */

    if (MotorEncoderB_initVar == 0u)
    {
        MotorEncoderB_Init();
        MotorEncoderB_initVar = 1u;
    }

    MotorEncoderB_Enable();
}


/*******************************************************************************
* Function Name: MotorEncoderB_Stop
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
void MotorEncoderB_Stop(void) 
{
    uint8 enableInterrupts;

    #if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT)
        MotorEncoderB_Cnt8_Stop();
    #else 
        /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_16_BIT) ||
        *  (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT)
        */
        MotorEncoderB_Cnt16_Stop();    /* counter disable */
    #endif /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT) */
 
    enableInterrupts = CyEnterCriticalSection();

    /* Disable interrupts interrupts from Statusi register */
    MotorEncoderB_SR_AUX_CONTROL &= (uint8) (~MotorEncoderB_INTERRUPTS_ENABLE);

    CyExitCriticalSection(enableInterrupts);

    #if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT)
        CyIntDisable(MotorEncoderB_ISR_NUMBER);    /* interrupt disable */
    #endif /* MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: MotorEncoderB_GetCounter
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
*  MotorEncoderB_count32SoftPart - used to get hi 16 bit for current value
*  of the 32-bit counter, when Counter size equal 32-bit.
*
*******************************************************************************/
int32 MotorEncoderB_GetCounter(void) 
{
    int32 count;
    uint16 tmpCnt;

    #if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT)
        int16 hwCount;

        CyIntDisable(MotorEncoderB_ISR_NUMBER);

        tmpCnt = MotorEncoderB_Cnt16_ReadCounter();
        hwCount = (int16) ((int32) tmpCnt - (int32) MotorEncoderB_COUNTER_INIT_VALUE);
        count = MotorEncoderB_count32SoftPart + hwCount;

        CyIntEnable(MotorEncoderB_ISR_NUMBER);
    #else 
        /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT) || 
        *  (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_16_BIT)
        */
        #if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT)
            tmpCnt = MotorEncoderB_Cnt8_ReadCounter();
        #else /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_16_BIT) */
            tmpCnt = MotorEncoderB_Cnt16_ReadCounter();
        #endif  /* MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT */

        count = (int32) ((int32) tmpCnt -
                (int32) MotorEncoderB_COUNTER_INIT_VALUE);

    #endif /* MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT */ 

    return (count);
}


/*******************************************************************************
* Function Name: MotorEncoderB_SetCounter
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
*  MotorEncoderB_count32SoftPart - modified to set hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MotorEncoderB_SetCounter(int32 value) 
{
    #if ((MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT) || \
         (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_16_BIT))
        uint16 count;
        
        if (value >= 0)
        {
            count = (uint16) value + MotorEncoderB_COUNTER_INIT_VALUE;
        }
        else
        {
            count = MotorEncoderB_COUNTER_INIT_VALUE - (uint16)(-value);
        }
        #if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT)
            MotorEncoderB_Cnt8_WriteCounter(count);
        #else /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_16_BIT) */
            MotorEncoderB_Cnt16_WriteCounter(count);
        #endif  /* MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT */
    #else /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT) */
        CyIntDisable(MotorEncoderB_ISR_NUMBER);

        MotorEncoderB_Cnt16_WriteCounter(MotorEncoderB_COUNTER_INIT_VALUE);
        MotorEncoderB_count32SoftPart = value;

        CyIntEnable(MotorEncoderB_ISR_NUMBER);
    #endif  /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT) ||
             * (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_16_BIT)
             */
}


/*******************************************************************************
* Function Name: MotorEncoderB_GetEvents
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
uint8 MotorEncoderB_GetEvents(void) 
{
    return (MotorEncoderB_STATUS_REG & MotorEncoderB_INIT_INT_MASK);
}


/*******************************************************************************
* Function Name: MotorEncoderB_SetInterruptMask
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
void MotorEncoderB_SetInterruptMask(uint8 mask) 
{
    #if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT)
        /* Underflow, Overflow and Reset interrupts for 32-bit Counter are always enable */
        mask |= (MotorEncoderB_COUNTER_OVERFLOW | MotorEncoderB_COUNTER_UNDERFLOW |
                 MotorEncoderB_COUNTER_RESET);
    #endif /* MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT */

    MotorEncoderB_STATUS_MASK = mask;
}


/*******************************************************************************
* Function Name: MotorEncoderB_GetInterruptMask
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
uint8 MotorEncoderB_GetInterruptMask(void) 
{
    return (MotorEncoderB_STATUS_MASK & MotorEncoderB_INIT_INT_MASK);
}


/* [] END OF FILE */

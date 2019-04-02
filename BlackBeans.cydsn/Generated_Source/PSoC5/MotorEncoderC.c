/*******************************************************************************
* File Name: MotorEncoderC.c  
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

#include "MotorEncoderC.h"

#if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT)
    #include "MotorEncoderC_PVT.h"
#endif /* MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT */

uint8 MotorEncoderC_initVar = 0u;


/*******************************************************************************
* Function Name: MotorEncoderC_Init
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
void MotorEncoderC_Init(void) 
{
    #if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT)
        /* Disable Interrupt. */
        CyIntDisable(MotorEncoderC_ISR_NUMBER);
        /* Set the ISR to point to the MotorEncoderC_isr Interrupt. */
        (void) CyIntSetVector(MotorEncoderC_ISR_NUMBER, & MotorEncoderC_ISR);
        /* Set the priority. */
        CyIntSetPriority(MotorEncoderC_ISR_NUMBER, MotorEncoderC_ISR_PRIORITY);
    #endif /* MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: MotorEncoderC_Enable
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
void MotorEncoderC_Enable(void) 
{
    uint8 enableInterrupts;

    MotorEncoderC_SetInterruptMask(MotorEncoderC_INIT_INT_MASK);

    /* Clear pending interrupts. */
    (void) MotorEncoderC_GetEvents();
    
    enableInterrupts = CyEnterCriticalSection();

    /* Enable interrupts from Statusi register */
    MotorEncoderC_SR_AUX_CONTROL |= MotorEncoderC_INTERRUPTS_ENABLE;

    CyExitCriticalSection(enableInterrupts);        

    #if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT)
        /* Enable Component interrupts */
        CyIntEnable(MotorEncoderC_ISR_NUMBER);
    #endif /* MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: MotorEncoderC_Start
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
*  MotorEncoderC_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MotorEncoderC_Start(void) 
{
    #if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT)
        MotorEncoderC_Cnt8_Start();
        MotorEncoderC_Cnt8_WriteCounter(MotorEncoderC_COUNTER_INIT_VALUE);
    #else
        /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_16_BIT) || 
        *  (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT) 
        */
        MotorEncoderC_Cnt16_Start();
        MotorEncoderC_Cnt16_WriteCounter(MotorEncoderC_COUNTER_INIT_VALUE);
    #endif /* MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT */
    
    #if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT)        
       MotorEncoderC_count32SoftPart = 0;
    #endif /* MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT */

    if (MotorEncoderC_initVar == 0u)
    {
        MotorEncoderC_Init();
        MotorEncoderC_initVar = 1u;
    }

    MotorEncoderC_Enable();
}


/*******************************************************************************
* Function Name: MotorEncoderC_Stop
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
void MotorEncoderC_Stop(void) 
{
    uint8 enableInterrupts;

    #if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT)
        MotorEncoderC_Cnt8_Stop();
    #else 
        /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_16_BIT) ||
        *  (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT)
        */
        MotorEncoderC_Cnt16_Stop();    /* counter disable */
    #endif /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT) */
 
    enableInterrupts = CyEnterCriticalSection();

    /* Disable interrupts interrupts from Statusi register */
    MotorEncoderC_SR_AUX_CONTROL &= (uint8) (~MotorEncoderC_INTERRUPTS_ENABLE);

    CyExitCriticalSection(enableInterrupts);

    #if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT)
        CyIntDisable(MotorEncoderC_ISR_NUMBER);    /* interrupt disable */
    #endif /* MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: MotorEncoderC_GetCounter
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
*  MotorEncoderC_count32SoftPart - used to get hi 16 bit for current value
*  of the 32-bit counter, when Counter size equal 32-bit.
*
*******************************************************************************/
int32 MotorEncoderC_GetCounter(void) 
{
    int32 count;
    uint16 tmpCnt;

    #if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT)
        int16 hwCount;

        CyIntDisable(MotorEncoderC_ISR_NUMBER);

        tmpCnt = MotorEncoderC_Cnt16_ReadCounter();
        hwCount = (int16) ((int32) tmpCnt - (int32) MotorEncoderC_COUNTER_INIT_VALUE);
        count = MotorEncoderC_count32SoftPart + hwCount;

        CyIntEnable(MotorEncoderC_ISR_NUMBER);
    #else 
        /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT) || 
        *  (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_16_BIT)
        */
        #if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT)
            tmpCnt = MotorEncoderC_Cnt8_ReadCounter();
        #else /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_16_BIT) */
            tmpCnt = MotorEncoderC_Cnt16_ReadCounter();
        #endif  /* MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT */

        count = (int32) ((int32) tmpCnt -
                (int32) MotorEncoderC_COUNTER_INIT_VALUE);

    #endif /* MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT */ 

    return (count);
}


/*******************************************************************************
* Function Name: MotorEncoderC_SetCounter
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
*  MotorEncoderC_count32SoftPart - modified to set hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MotorEncoderC_SetCounter(int32 value) 
{
    #if ((MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT) || \
         (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_16_BIT))
        uint16 count;
        
        if (value >= 0)
        {
            count = (uint16) value + MotorEncoderC_COUNTER_INIT_VALUE;
        }
        else
        {
            count = MotorEncoderC_COUNTER_INIT_VALUE - (uint16)(-value);
        }
        #if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT)
            MotorEncoderC_Cnt8_WriteCounter(count);
        #else /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_16_BIT) */
            MotorEncoderC_Cnt16_WriteCounter(count);
        #endif  /* MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT */
    #else /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT) */
        CyIntDisable(MotorEncoderC_ISR_NUMBER);

        MotorEncoderC_Cnt16_WriteCounter(MotorEncoderC_COUNTER_INIT_VALUE);
        MotorEncoderC_count32SoftPart = value;

        CyIntEnable(MotorEncoderC_ISR_NUMBER);
    #endif  /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT) ||
             * (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_16_BIT)
             */
}


/*******************************************************************************
* Function Name: MotorEncoderC_GetEvents
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
uint8 MotorEncoderC_GetEvents(void) 
{
    return (MotorEncoderC_STATUS_REG & MotorEncoderC_INIT_INT_MASK);
}


/*******************************************************************************
* Function Name: MotorEncoderC_SetInterruptMask
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
void MotorEncoderC_SetInterruptMask(uint8 mask) 
{
    #if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT)
        /* Underflow, Overflow and Reset interrupts for 32-bit Counter are always enable */
        mask |= (MotorEncoderC_COUNTER_OVERFLOW | MotorEncoderC_COUNTER_UNDERFLOW |
                 MotorEncoderC_COUNTER_RESET);
    #endif /* MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT */

    MotorEncoderC_STATUS_MASK = mask;
}


/*******************************************************************************
* Function Name: MotorEncoderC_GetInterruptMask
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
uint8 MotorEncoderC_GetInterruptMask(void) 
{
    return (MotorEncoderC_STATUS_MASK & MotorEncoderC_INIT_INT_MASK);
}


/* [] END OF FILE */

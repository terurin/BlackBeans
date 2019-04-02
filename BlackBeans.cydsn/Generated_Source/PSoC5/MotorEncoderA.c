/*******************************************************************************
* File Name: MotorEncoderA.c  
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

#include "MotorEncoderA.h"

#if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT)
    #include "MotorEncoderA_PVT.h"
#endif /* MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT */

uint8 MotorEncoderA_initVar = 0u;


/*******************************************************************************
* Function Name: MotorEncoderA_Init
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
void MotorEncoderA_Init(void) 
{
    #if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT)
        /* Disable Interrupt. */
        CyIntDisable(MotorEncoderA_ISR_NUMBER);
        /* Set the ISR to point to the MotorEncoderA_isr Interrupt. */
        (void) CyIntSetVector(MotorEncoderA_ISR_NUMBER, & MotorEncoderA_ISR);
        /* Set the priority. */
        CyIntSetPriority(MotorEncoderA_ISR_NUMBER, MotorEncoderA_ISR_PRIORITY);
    #endif /* MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: MotorEncoderA_Enable
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
void MotorEncoderA_Enable(void) 
{
    uint8 enableInterrupts;

    MotorEncoderA_SetInterruptMask(MotorEncoderA_INIT_INT_MASK);

    /* Clear pending interrupts. */
    (void) MotorEncoderA_GetEvents();
    
    enableInterrupts = CyEnterCriticalSection();

    /* Enable interrupts from Statusi register */
    MotorEncoderA_SR_AUX_CONTROL |= MotorEncoderA_INTERRUPTS_ENABLE;

    CyExitCriticalSection(enableInterrupts);        

    #if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT)
        /* Enable Component interrupts */
        CyIntEnable(MotorEncoderA_ISR_NUMBER);
    #endif /* MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: MotorEncoderA_Start
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
*  MotorEncoderA_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MotorEncoderA_Start(void) 
{
    #if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT)
        MotorEncoderA_Cnt8_Start();
        MotorEncoderA_Cnt8_WriteCounter(MotorEncoderA_COUNTER_INIT_VALUE);
    #else
        /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_16_BIT) || 
        *  (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT) 
        */
        MotorEncoderA_Cnt16_Start();
        MotorEncoderA_Cnt16_WriteCounter(MotorEncoderA_COUNTER_INIT_VALUE);
    #endif /* MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT */
    
    #if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT)        
       MotorEncoderA_count32SoftPart = 0;
    #endif /* MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT */

    if (MotorEncoderA_initVar == 0u)
    {
        MotorEncoderA_Init();
        MotorEncoderA_initVar = 1u;
    }

    MotorEncoderA_Enable();
}


/*******************************************************************************
* Function Name: MotorEncoderA_Stop
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
void MotorEncoderA_Stop(void) 
{
    uint8 enableInterrupts;

    #if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT)
        MotorEncoderA_Cnt8_Stop();
    #else 
        /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_16_BIT) ||
        *  (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT)
        */
        MotorEncoderA_Cnt16_Stop();    /* counter disable */
    #endif /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT) */
 
    enableInterrupts = CyEnterCriticalSection();

    /* Disable interrupts interrupts from Statusi register */
    MotorEncoderA_SR_AUX_CONTROL &= (uint8) (~MotorEncoderA_INTERRUPTS_ENABLE);

    CyExitCriticalSection(enableInterrupts);

    #if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT)
        CyIntDisable(MotorEncoderA_ISR_NUMBER);    /* interrupt disable */
    #endif /* MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: MotorEncoderA_GetCounter
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
*  MotorEncoderA_count32SoftPart - used to get hi 16 bit for current value
*  of the 32-bit counter, when Counter size equal 32-bit.
*
*******************************************************************************/
int32 MotorEncoderA_GetCounter(void) 
{
    int32 count;
    uint16 tmpCnt;

    #if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT)
        int16 hwCount;

        CyIntDisable(MotorEncoderA_ISR_NUMBER);

        tmpCnt = MotorEncoderA_Cnt16_ReadCounter();
        hwCount = (int16) ((int32) tmpCnt - (int32) MotorEncoderA_COUNTER_INIT_VALUE);
        count = MotorEncoderA_count32SoftPart + hwCount;

        CyIntEnable(MotorEncoderA_ISR_NUMBER);
    #else 
        /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT) || 
        *  (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_16_BIT)
        */
        #if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT)
            tmpCnt = MotorEncoderA_Cnt8_ReadCounter();
        #else /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_16_BIT) */
            tmpCnt = MotorEncoderA_Cnt16_ReadCounter();
        #endif  /* MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT */

        count = (int32) ((int32) tmpCnt -
                (int32) MotorEncoderA_COUNTER_INIT_VALUE);

    #endif /* MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT */ 

    return (count);
}


/*******************************************************************************
* Function Name: MotorEncoderA_SetCounter
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
*  MotorEncoderA_count32SoftPart - modified to set hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MotorEncoderA_SetCounter(int32 value) 
{
    #if ((MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT) || \
         (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_16_BIT))
        uint16 count;
        
        if (value >= 0)
        {
            count = (uint16) value + MotorEncoderA_COUNTER_INIT_VALUE;
        }
        else
        {
            count = MotorEncoderA_COUNTER_INIT_VALUE - (uint16)(-value);
        }
        #if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT)
            MotorEncoderA_Cnt8_WriteCounter(count);
        #else /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_16_BIT) */
            MotorEncoderA_Cnt16_WriteCounter(count);
        #endif  /* MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT */
    #else /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT) */
        CyIntDisable(MotorEncoderA_ISR_NUMBER);

        MotorEncoderA_Cnt16_WriteCounter(MotorEncoderA_COUNTER_INIT_VALUE);
        MotorEncoderA_count32SoftPart = value;

        CyIntEnable(MotorEncoderA_ISR_NUMBER);
    #endif  /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT) ||
             * (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_16_BIT)
             */
}


/*******************************************************************************
* Function Name: MotorEncoderA_GetEvents
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
uint8 MotorEncoderA_GetEvents(void) 
{
    return (MotorEncoderA_STATUS_REG & MotorEncoderA_INIT_INT_MASK);
}


/*******************************************************************************
* Function Name: MotorEncoderA_SetInterruptMask
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
void MotorEncoderA_SetInterruptMask(uint8 mask) 
{
    #if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT)
        /* Underflow, Overflow and Reset interrupts for 32-bit Counter are always enable */
        mask |= (MotorEncoderA_COUNTER_OVERFLOW | MotorEncoderA_COUNTER_UNDERFLOW |
                 MotorEncoderA_COUNTER_RESET);
    #endif /* MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT */

    MotorEncoderA_STATUS_MASK = mask;
}


/*******************************************************************************
* Function Name: MotorEncoderA_GetInterruptMask
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
uint8 MotorEncoderA_GetInterruptMask(void) 
{
    return (MotorEncoderA_STATUS_MASK & MotorEncoderA_INIT_INT_MASK);
}


/* [] END OF FILE */

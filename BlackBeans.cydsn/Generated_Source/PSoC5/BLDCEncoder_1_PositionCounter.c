/*******************************************************************************
* File Name: BLDCEncoder_1_PositionCounter.c  
* Version 3.0
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
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

uint8 BLDCEncoder_1_PositionCounter_initVar = 0u;


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void BLDCEncoder_1_PositionCounter_Init(void) 
{
        #if (!BLDCEncoder_1_PositionCounter_UsingFixedFunction && !BLDCEncoder_1_PositionCounter_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!BLDCEncoder_1_PositionCounter_UsingFixedFunction && !BLDCEncoder_1_PositionCounter_ControlRegRemoved) */
        
        #if(!BLDCEncoder_1_PositionCounter_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 BLDCEncoder_1_PositionCounter_interruptState;
        #endif /* (!BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
        
        #if (BLDCEncoder_1_PositionCounter_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            BLDCEncoder_1_PositionCounter_CONTROL &= BLDCEncoder_1_PositionCounter_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                BLDCEncoder_1_PositionCounter_CONTROL2 &= ((uint8)(~BLDCEncoder_1_PositionCounter_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                BLDCEncoder_1_PositionCounter_CONTROL3 &= ((uint8)(~BLDCEncoder_1_PositionCounter_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (BLDCEncoder_1_PositionCounter_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                BLDCEncoder_1_PositionCounter_CONTROL |= BLDCEncoder_1_PositionCounter_ONESHOT;
            #endif /* (BLDCEncoder_1_PositionCounter_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            BLDCEncoder_1_PositionCounter_CONTROL2 |= BLDCEncoder_1_PositionCounter_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            BLDCEncoder_1_PositionCounter_RT1 &= ((uint8)(~BLDCEncoder_1_PositionCounter_RT1_MASK));
            BLDCEncoder_1_PositionCounter_RT1 |= BLDCEncoder_1_PositionCounter_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            BLDCEncoder_1_PositionCounter_RT1 &= ((uint8)(~BLDCEncoder_1_PositionCounter_SYNCDSI_MASK));
            BLDCEncoder_1_PositionCounter_RT1 |= BLDCEncoder_1_PositionCounter_SYNCDSI_EN;

        #else
            #if(!BLDCEncoder_1_PositionCounter_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = BLDCEncoder_1_PositionCounter_CONTROL & ((uint8)(~BLDCEncoder_1_PositionCounter_CTRL_CMPMODE_MASK));
            BLDCEncoder_1_PositionCounter_CONTROL = ctrl | BLDCEncoder_1_PositionCounter_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = BLDCEncoder_1_PositionCounter_CONTROL & ((uint8)(~BLDCEncoder_1_PositionCounter_CTRL_CAPMODE_MASK));
            
            #if( 0 != BLDCEncoder_1_PositionCounter_CAPTURE_MODE_CONF)
                BLDCEncoder_1_PositionCounter_CONTROL = ctrl | BLDCEncoder_1_PositionCounter_DEFAULT_CAPTURE_MODE;
            #else
                BLDCEncoder_1_PositionCounter_CONTROL = ctrl;
            #endif /* 0 != BLDCEncoder_1_PositionCounter_CAPTURE_MODE */ 
            
            #endif /* (!BLDCEncoder_1_PositionCounter_ControlRegRemoved) */
        #endif /* (BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!BLDCEncoder_1_PositionCounter_UsingFixedFunction)
            BLDCEncoder_1_PositionCounter_ClearFIFO();
        #endif /* (!BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        BLDCEncoder_1_PositionCounter_WritePeriod(BLDCEncoder_1_PositionCounter_INIT_PERIOD_VALUE);
        #if (!(BLDCEncoder_1_PositionCounter_UsingFixedFunction && (CY_PSOC5A)))
            BLDCEncoder_1_PositionCounter_WriteCounter(BLDCEncoder_1_PositionCounter_INIT_COUNTER_VALUE);
        #endif /* (!(BLDCEncoder_1_PositionCounter_UsingFixedFunction && (CY_PSOC5A))) */
        BLDCEncoder_1_PositionCounter_SetInterruptMode(BLDCEncoder_1_PositionCounter_INIT_INTERRUPTS_MASK);
        
        #if (!BLDCEncoder_1_PositionCounter_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)BLDCEncoder_1_PositionCounter_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            BLDCEncoder_1_PositionCounter_WriteCompare(BLDCEncoder_1_PositionCounter_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            BLDCEncoder_1_PositionCounter_interruptState = CyEnterCriticalSection();
            
            BLDCEncoder_1_PositionCounter_STATUS_AUX_CTRL |= BLDCEncoder_1_PositionCounter_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(BLDCEncoder_1_PositionCounter_interruptState);
            
        #endif /* (!BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void BLDCEncoder_1_PositionCounter_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (BLDCEncoder_1_PositionCounter_UsingFixedFunction)
        BLDCEncoder_1_PositionCounter_GLOBAL_ENABLE |= BLDCEncoder_1_PositionCounter_BLOCK_EN_MASK;
        BLDCEncoder_1_PositionCounter_GLOBAL_STBY_ENABLE |= BLDCEncoder_1_PositionCounter_BLOCK_STBY_EN_MASK;
    #endif /* (BLDCEncoder_1_PositionCounter_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!BLDCEncoder_1_PositionCounter_ControlRegRemoved || BLDCEncoder_1_PositionCounter_UsingFixedFunction)
        BLDCEncoder_1_PositionCounter_CONTROL |= BLDCEncoder_1_PositionCounter_CTRL_ENABLE;                
    #endif /* (!BLDCEncoder_1_PositionCounter_ControlRegRemoved || BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  BLDCEncoder_1_PositionCounter_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void BLDCEncoder_1_PositionCounter_Start(void) 
{
    if(BLDCEncoder_1_PositionCounter_initVar == 0u)
    {
        BLDCEncoder_1_PositionCounter_Init();
        
        BLDCEncoder_1_PositionCounter_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    BLDCEncoder_1_PositionCounter_Enable();        
}


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void BLDCEncoder_1_PositionCounter_Stop(void) 
{
    /* Disable Counter */
    #if(!BLDCEncoder_1_PositionCounter_ControlRegRemoved || BLDCEncoder_1_PositionCounter_UsingFixedFunction)
        BLDCEncoder_1_PositionCounter_CONTROL &= ((uint8)(~BLDCEncoder_1_PositionCounter_CTRL_ENABLE));        
    #endif /* (!BLDCEncoder_1_PositionCounter_ControlRegRemoved || BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (BLDCEncoder_1_PositionCounter_UsingFixedFunction)
        BLDCEncoder_1_PositionCounter_GLOBAL_ENABLE &= ((uint8)(~BLDCEncoder_1_PositionCounter_BLOCK_EN_MASK));
        BLDCEncoder_1_PositionCounter_GLOBAL_STBY_ENABLE &= ((uint8)(~BLDCEncoder_1_PositionCounter_BLOCK_STBY_EN_MASK));
    #endif /* (BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void BLDCEncoder_1_PositionCounter_SetInterruptMode(uint8 interruptsMask) 
{
    BLDCEncoder_1_PositionCounter_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   BLDCEncoder_1_PositionCounter_ReadStatusRegister(void) 
{
    return BLDCEncoder_1_PositionCounter_STATUS;
}


#if(!BLDCEncoder_1_PositionCounter_ControlRegRemoved)
/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   BLDCEncoder_1_PositionCounter_ReadControlRegister(void) 
{
    return BLDCEncoder_1_PositionCounter_CONTROL;
}


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    BLDCEncoder_1_PositionCounter_WriteControlRegister(uint8 control) 
{
    BLDCEncoder_1_PositionCounter_CONTROL = control;
}

#endif  /* (!BLDCEncoder_1_PositionCounter_ControlRegRemoved) */


#if (!(BLDCEncoder_1_PositionCounter_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void BLDCEncoder_1_PositionCounter_WriteCounter(uint8 counter) \
                                   
{
    #if(BLDCEncoder_1_PositionCounter_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (BLDCEncoder_1_PositionCounter_GLOBAL_ENABLE & BLDCEncoder_1_PositionCounter_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        BLDCEncoder_1_PositionCounter_GLOBAL_ENABLE |= BLDCEncoder_1_PositionCounter_BLOCK_EN_MASK;
        CY_SET_REG16(BLDCEncoder_1_PositionCounter_COUNTER_LSB_PTR, (uint16)counter);
        BLDCEncoder_1_PositionCounter_GLOBAL_ENABLE &= ((uint8)(~BLDCEncoder_1_PositionCounter_BLOCK_EN_MASK));
    #else
        CY_SET_REG8(BLDCEncoder_1_PositionCounter_COUNTER_LSB_PTR, counter);
    #endif /* (BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
}
#endif /* (!(BLDCEncoder_1_PositionCounter_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) The present value of the counter.
*
*******************************************************************************/
uint8 BLDCEncoder_1_PositionCounter_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(BLDCEncoder_1_PositionCounter_UsingFixedFunction)
		(void)CY_GET_REG16(BLDCEncoder_1_PositionCounter_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(BLDCEncoder_1_PositionCounter_COUNTER_LSB_PTR_8BIT);
	#endif/* (BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(BLDCEncoder_1_PositionCounter_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB_PTR));
    #endif /* (BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) Present Capture value.
*
*******************************************************************************/
uint8 BLDCEncoder_1_PositionCounter_ReadCapture(void) 
{
    #if(BLDCEncoder_1_PositionCounter_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB_PTR));
    #endif /* (BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint8) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void BLDCEncoder_1_PositionCounter_WritePeriod(uint8 period) 
{
    #if(BLDCEncoder_1_PositionCounter_UsingFixedFunction)
        CY_SET_REG16(BLDCEncoder_1_PositionCounter_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG8(BLDCEncoder_1_PositionCounter_PERIOD_LSB_PTR, period);
    #endif /* (BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint8) Present period value.
*
*******************************************************************************/
uint8 BLDCEncoder_1_PositionCounter_ReadPeriod(void) 
{
    #if(BLDCEncoder_1_PositionCounter_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(BLDCEncoder_1_PositionCounter_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(BLDCEncoder_1_PositionCounter_PERIOD_LSB_PTR));
    #endif /* (BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
}


#if (!BLDCEncoder_1_PositionCounter_UsingFixedFunction)
/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void BLDCEncoder_1_PositionCounter_WriteCompare(uint8 compare) \
                                   
{
    #if(BLDCEncoder_1_PositionCounter_UsingFixedFunction)
        CY_SET_REG16(BLDCEncoder_1_PositionCounter_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG8(BLDCEncoder_1_PositionCounter_COMPARE_LSB_PTR, compare);
    #endif /* (BLDCEncoder_1_PositionCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint8) Present compare value.
*
*******************************************************************************/
uint8 BLDCEncoder_1_PositionCounter_ReadCompare(void) 
{
    return (CY_GET_REG8(BLDCEncoder_1_PositionCounter_COMPARE_LSB_PTR));
}


#if (BLDCEncoder_1_PositionCounter_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void BLDCEncoder_1_PositionCounter_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    BLDCEncoder_1_PositionCounter_CONTROL &= ((uint8)(~BLDCEncoder_1_PositionCounter_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    BLDCEncoder_1_PositionCounter_CONTROL |= compareMode;
}
#endif  /* (BLDCEncoder_1_PositionCounter_COMPARE_MODE_SOFTWARE) */


#if (BLDCEncoder_1_PositionCounter_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void BLDCEncoder_1_PositionCounter_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    BLDCEncoder_1_PositionCounter_CONTROL &= ((uint8)(~BLDCEncoder_1_PositionCounter_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    BLDCEncoder_1_PositionCounter_CONTROL |= ((uint8)((uint8)captureMode << BLDCEncoder_1_PositionCounter_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (BLDCEncoder_1_PositionCounter_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: BLDCEncoder_1_PositionCounter_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void BLDCEncoder_1_PositionCounter_ClearFIFO(void) 
{

    while(0u != (BLDCEncoder_1_PositionCounter_ReadStatusRegister() & BLDCEncoder_1_PositionCounter_STATUS_FIFONEMP))
    {
        (void)BLDCEncoder_1_PositionCounter_ReadCapture();
    }

}
#endif  /* (!BLDCEncoder_1_PositionCounter_UsingFixedFunction) */


/* [] END OF FILE */


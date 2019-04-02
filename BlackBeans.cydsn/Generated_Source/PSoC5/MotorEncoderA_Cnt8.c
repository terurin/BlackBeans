/*******************************************************************************
* File Name: MotorEncoderA_Cnt8.c  
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

#include "MotorEncoderA_Cnt8.h"

uint8 MotorEncoderA_Cnt8_initVar = 0u;


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_Init
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
void MotorEncoderA_Cnt8_Init(void) 
{
        #if (!MotorEncoderA_Cnt8_UsingFixedFunction && !MotorEncoderA_Cnt8_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!MotorEncoderA_Cnt8_UsingFixedFunction && !MotorEncoderA_Cnt8_ControlRegRemoved) */
        
        #if(!MotorEncoderA_Cnt8_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 MotorEncoderA_Cnt8_interruptState;
        #endif /* (!MotorEncoderA_Cnt8_UsingFixedFunction) */
        
        #if (MotorEncoderA_Cnt8_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            MotorEncoderA_Cnt8_CONTROL &= MotorEncoderA_Cnt8_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                MotorEncoderA_Cnt8_CONTROL2 &= ((uint8)(~MotorEncoderA_Cnt8_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                MotorEncoderA_Cnt8_CONTROL3 &= ((uint8)(~MotorEncoderA_Cnt8_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (MotorEncoderA_Cnt8_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                MotorEncoderA_Cnt8_CONTROL |= MotorEncoderA_Cnt8_ONESHOT;
            #endif /* (MotorEncoderA_Cnt8_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            MotorEncoderA_Cnt8_CONTROL2 |= MotorEncoderA_Cnt8_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            MotorEncoderA_Cnt8_RT1 &= ((uint8)(~MotorEncoderA_Cnt8_RT1_MASK));
            MotorEncoderA_Cnt8_RT1 |= MotorEncoderA_Cnt8_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            MotorEncoderA_Cnt8_RT1 &= ((uint8)(~MotorEncoderA_Cnt8_SYNCDSI_MASK));
            MotorEncoderA_Cnt8_RT1 |= MotorEncoderA_Cnt8_SYNCDSI_EN;

        #else
            #if(!MotorEncoderA_Cnt8_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = MotorEncoderA_Cnt8_CONTROL & ((uint8)(~MotorEncoderA_Cnt8_CTRL_CMPMODE_MASK));
            MotorEncoderA_Cnt8_CONTROL = ctrl | MotorEncoderA_Cnt8_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = MotorEncoderA_Cnt8_CONTROL & ((uint8)(~MotorEncoderA_Cnt8_CTRL_CAPMODE_MASK));
            
            #if( 0 != MotorEncoderA_Cnt8_CAPTURE_MODE_CONF)
                MotorEncoderA_Cnt8_CONTROL = ctrl | MotorEncoderA_Cnt8_DEFAULT_CAPTURE_MODE;
            #else
                MotorEncoderA_Cnt8_CONTROL = ctrl;
            #endif /* 0 != MotorEncoderA_Cnt8_CAPTURE_MODE */ 
            
            #endif /* (!MotorEncoderA_Cnt8_ControlRegRemoved) */
        #endif /* (MotorEncoderA_Cnt8_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!MotorEncoderA_Cnt8_UsingFixedFunction)
            MotorEncoderA_Cnt8_ClearFIFO();
        #endif /* (!MotorEncoderA_Cnt8_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        MotorEncoderA_Cnt8_WritePeriod(MotorEncoderA_Cnt8_INIT_PERIOD_VALUE);
        #if (!(MotorEncoderA_Cnt8_UsingFixedFunction && (CY_PSOC5A)))
            MotorEncoderA_Cnt8_WriteCounter(MotorEncoderA_Cnt8_INIT_COUNTER_VALUE);
        #endif /* (!(MotorEncoderA_Cnt8_UsingFixedFunction && (CY_PSOC5A))) */
        MotorEncoderA_Cnt8_SetInterruptMode(MotorEncoderA_Cnt8_INIT_INTERRUPTS_MASK);
        
        #if (!MotorEncoderA_Cnt8_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)MotorEncoderA_Cnt8_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            MotorEncoderA_Cnt8_WriteCompare(MotorEncoderA_Cnt8_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            MotorEncoderA_Cnt8_interruptState = CyEnterCriticalSection();
            
            MotorEncoderA_Cnt8_STATUS_AUX_CTRL |= MotorEncoderA_Cnt8_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(MotorEncoderA_Cnt8_interruptState);
            
        #endif /* (!MotorEncoderA_Cnt8_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_Enable
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
void MotorEncoderA_Cnt8_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (MotorEncoderA_Cnt8_UsingFixedFunction)
        MotorEncoderA_Cnt8_GLOBAL_ENABLE |= MotorEncoderA_Cnt8_BLOCK_EN_MASK;
        MotorEncoderA_Cnt8_GLOBAL_STBY_ENABLE |= MotorEncoderA_Cnt8_BLOCK_STBY_EN_MASK;
    #endif /* (MotorEncoderA_Cnt8_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!MotorEncoderA_Cnt8_ControlRegRemoved || MotorEncoderA_Cnt8_UsingFixedFunction)
        MotorEncoderA_Cnt8_CONTROL |= MotorEncoderA_Cnt8_CTRL_ENABLE;                
    #endif /* (!MotorEncoderA_Cnt8_ControlRegRemoved || MotorEncoderA_Cnt8_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_Start
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
*  MotorEncoderA_Cnt8_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void MotorEncoderA_Cnt8_Start(void) 
{
    if(MotorEncoderA_Cnt8_initVar == 0u)
    {
        MotorEncoderA_Cnt8_Init();
        
        MotorEncoderA_Cnt8_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    MotorEncoderA_Cnt8_Enable();        
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_Stop
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
void MotorEncoderA_Cnt8_Stop(void) 
{
    /* Disable Counter */
    #if(!MotorEncoderA_Cnt8_ControlRegRemoved || MotorEncoderA_Cnt8_UsingFixedFunction)
        MotorEncoderA_Cnt8_CONTROL &= ((uint8)(~MotorEncoderA_Cnt8_CTRL_ENABLE));        
    #endif /* (!MotorEncoderA_Cnt8_ControlRegRemoved || MotorEncoderA_Cnt8_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (MotorEncoderA_Cnt8_UsingFixedFunction)
        MotorEncoderA_Cnt8_GLOBAL_ENABLE &= ((uint8)(~MotorEncoderA_Cnt8_BLOCK_EN_MASK));
        MotorEncoderA_Cnt8_GLOBAL_STBY_ENABLE &= ((uint8)(~MotorEncoderA_Cnt8_BLOCK_STBY_EN_MASK));
    #endif /* (MotorEncoderA_Cnt8_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_SetInterruptMode
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
void MotorEncoderA_Cnt8_SetInterruptMode(uint8 interruptsMask) 
{
    MotorEncoderA_Cnt8_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_ReadStatusRegister
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
uint8   MotorEncoderA_Cnt8_ReadStatusRegister(void) 
{
    return MotorEncoderA_Cnt8_STATUS;
}


#if(!MotorEncoderA_Cnt8_ControlRegRemoved)
/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_ReadControlRegister
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
uint8   MotorEncoderA_Cnt8_ReadControlRegister(void) 
{
    return MotorEncoderA_Cnt8_CONTROL;
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_WriteControlRegister
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
void    MotorEncoderA_Cnt8_WriteControlRegister(uint8 control) 
{
    MotorEncoderA_Cnt8_CONTROL = control;
}

#endif  /* (!MotorEncoderA_Cnt8_ControlRegRemoved) */


#if (!(MotorEncoderA_Cnt8_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_WriteCounter
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
void MotorEncoderA_Cnt8_WriteCounter(uint8 counter) \
                                   
{
    #if(MotorEncoderA_Cnt8_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (MotorEncoderA_Cnt8_GLOBAL_ENABLE & MotorEncoderA_Cnt8_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        MotorEncoderA_Cnt8_GLOBAL_ENABLE |= MotorEncoderA_Cnt8_BLOCK_EN_MASK;
        CY_SET_REG16(MotorEncoderA_Cnt8_COUNTER_LSB_PTR, (uint16)counter);
        MotorEncoderA_Cnt8_GLOBAL_ENABLE &= ((uint8)(~MotorEncoderA_Cnt8_BLOCK_EN_MASK));
    #else
        CY_SET_REG8(MotorEncoderA_Cnt8_COUNTER_LSB_PTR, counter);
    #endif /* (MotorEncoderA_Cnt8_UsingFixedFunction) */
}
#endif /* (!(MotorEncoderA_Cnt8_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_ReadCounter
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
uint8 MotorEncoderA_Cnt8_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(MotorEncoderA_Cnt8_UsingFixedFunction)
		(void)CY_GET_REG16(MotorEncoderA_Cnt8_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(MotorEncoderA_Cnt8_COUNTER_LSB_PTR_8BIT);
	#endif/* (MotorEncoderA_Cnt8_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(MotorEncoderA_Cnt8_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(MotorEncoderA_Cnt8_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(MotorEncoderA_Cnt8_STATICCOUNT_LSB_PTR));
    #endif /* (MotorEncoderA_Cnt8_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_ReadCapture
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
uint8 MotorEncoderA_Cnt8_ReadCapture(void) 
{
    #if(MotorEncoderA_Cnt8_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(MotorEncoderA_Cnt8_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG8(MotorEncoderA_Cnt8_STATICCOUNT_LSB_PTR));
    #endif /* (MotorEncoderA_Cnt8_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_WritePeriod
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
void MotorEncoderA_Cnt8_WritePeriod(uint8 period) 
{
    #if(MotorEncoderA_Cnt8_UsingFixedFunction)
        CY_SET_REG16(MotorEncoderA_Cnt8_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG8(MotorEncoderA_Cnt8_PERIOD_LSB_PTR, period);
    #endif /* (MotorEncoderA_Cnt8_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_ReadPeriod
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
uint8 MotorEncoderA_Cnt8_ReadPeriod(void) 
{
    #if(MotorEncoderA_Cnt8_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(MotorEncoderA_Cnt8_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(MotorEncoderA_Cnt8_PERIOD_LSB_PTR));
    #endif /* (MotorEncoderA_Cnt8_UsingFixedFunction) */
}


#if (!MotorEncoderA_Cnt8_UsingFixedFunction)
/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_WriteCompare
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
void MotorEncoderA_Cnt8_WriteCompare(uint8 compare) \
                                   
{
    #if(MotorEncoderA_Cnt8_UsingFixedFunction)
        CY_SET_REG16(MotorEncoderA_Cnt8_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG8(MotorEncoderA_Cnt8_COMPARE_LSB_PTR, compare);
    #endif /* (MotorEncoderA_Cnt8_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_ReadCompare
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
uint8 MotorEncoderA_Cnt8_ReadCompare(void) 
{
    return (CY_GET_REG8(MotorEncoderA_Cnt8_COMPARE_LSB_PTR));
}


#if (MotorEncoderA_Cnt8_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_SetCompareMode
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
void MotorEncoderA_Cnt8_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    MotorEncoderA_Cnt8_CONTROL &= ((uint8)(~MotorEncoderA_Cnt8_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    MotorEncoderA_Cnt8_CONTROL |= compareMode;
}
#endif  /* (MotorEncoderA_Cnt8_COMPARE_MODE_SOFTWARE) */


#if (MotorEncoderA_Cnt8_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_SetCaptureMode
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
void MotorEncoderA_Cnt8_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    MotorEncoderA_Cnt8_CONTROL &= ((uint8)(~MotorEncoderA_Cnt8_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    MotorEncoderA_Cnt8_CONTROL |= ((uint8)((uint8)captureMode << MotorEncoderA_Cnt8_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (MotorEncoderA_Cnt8_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: MotorEncoderA_Cnt8_ClearFIFO
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
void MotorEncoderA_Cnt8_ClearFIFO(void) 
{

    while(0u != (MotorEncoderA_Cnt8_ReadStatusRegister() & MotorEncoderA_Cnt8_STATUS_FIFONEMP))
    {
        (void)MotorEncoderA_Cnt8_ReadCapture();
    }

}
#endif  /* (!MotorEncoderA_Cnt8_UsingFixedFunction) */


/* [] END OF FILE */


/*******************************************************************************
* File Name: BLDCDriver_1_PulseGenerator.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "BLDCDriver_1_PulseGenerator.h"

/* Error message for removed <resource> through optimization */
#ifdef BLDCDriver_1_PulseGenerator_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* BLDCDriver_1_PulseGenerator_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 BLDCDriver_1_PulseGenerator_initVar = 0u;


/*******************************************************************************
* Function Name: BLDCDriver_1_PulseGenerator_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  BLDCDriver_1_PulseGenerator_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void BLDCDriver_1_PulseGenerator_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(BLDCDriver_1_PulseGenerator_initVar == 0u)
    {
        BLDCDriver_1_PulseGenerator_Init();
        BLDCDriver_1_PulseGenerator_initVar = 1u;
    }
    BLDCDriver_1_PulseGenerator_Enable();

}


/*******************************************************************************
* Function Name: BLDCDriver_1_PulseGenerator_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  BLDCDriver_1_PulseGenerator_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void BLDCDriver_1_PulseGenerator_Init(void) 
{
    #if (BLDCDriver_1_PulseGenerator_UsingFixedFunction || BLDCDriver_1_PulseGenerator_UseControl)
        uint8 ctrl;
    #endif /* (BLDCDriver_1_PulseGenerator_UsingFixedFunction || BLDCDriver_1_PulseGenerator_UseControl) */

    #if(!BLDCDriver_1_PulseGenerator_UsingFixedFunction)
        #if(BLDCDriver_1_PulseGenerator_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 BLDCDriver_1_PulseGenerator_interruptState;
        #endif /* (BLDCDriver_1_PulseGenerator_UseStatus) */
    #endif /* (!BLDCDriver_1_PulseGenerator_UsingFixedFunction) */

    #if (BLDCDriver_1_PulseGenerator_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        BLDCDriver_1_PulseGenerator_CONTROL |= BLDCDriver_1_PulseGenerator_CFG0_MODE;
        #if (BLDCDriver_1_PulseGenerator_DeadBand2_4)
            BLDCDriver_1_PulseGenerator_CONTROL |= BLDCDriver_1_PulseGenerator_CFG0_DB;
        #endif /* (BLDCDriver_1_PulseGenerator_DeadBand2_4) */

        ctrl = BLDCDriver_1_PulseGenerator_CONTROL3 & ((uint8 )(~BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_MASK));
        BLDCDriver_1_PulseGenerator_CONTROL3 = ctrl | BLDCDriver_1_PulseGenerator_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        BLDCDriver_1_PulseGenerator_RT1 &= ((uint8)(~BLDCDriver_1_PulseGenerator_RT1_MASK));
        BLDCDriver_1_PulseGenerator_RT1 |= BLDCDriver_1_PulseGenerator_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        BLDCDriver_1_PulseGenerator_RT1 &= ((uint8)(~BLDCDriver_1_PulseGenerator_SYNCDSI_MASK));
        BLDCDriver_1_PulseGenerator_RT1 |= BLDCDriver_1_PulseGenerator_SYNCDSI_EN;

    #elif (BLDCDriver_1_PulseGenerator_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = BLDCDriver_1_PulseGenerator_CONTROL & ((uint8)(~BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_MASK));
        BLDCDriver_1_PulseGenerator_CONTROL = ctrl | BLDCDriver_1_PulseGenerator_DEFAULT_COMPARE2_MODE |
                                   BLDCDriver_1_PulseGenerator_DEFAULT_COMPARE1_MODE;
    #endif /* (BLDCDriver_1_PulseGenerator_UsingFixedFunction) */

    #if (!BLDCDriver_1_PulseGenerator_UsingFixedFunction)
        #if (BLDCDriver_1_PulseGenerator_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            BLDCDriver_1_PulseGenerator_AUX_CONTROLDP0 |= (BLDCDriver_1_PulseGenerator_AUX_CTRL_FIFO0_CLR);
        #else /* (BLDCDriver_1_PulseGenerator_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            BLDCDriver_1_PulseGenerator_AUX_CONTROLDP0 |= (BLDCDriver_1_PulseGenerator_AUX_CTRL_FIFO0_CLR);
            BLDCDriver_1_PulseGenerator_AUX_CONTROLDP1 |= (BLDCDriver_1_PulseGenerator_AUX_CTRL_FIFO0_CLR);
        #endif /* (BLDCDriver_1_PulseGenerator_Resolution == 8) */

        BLDCDriver_1_PulseGenerator_WriteCounter(BLDCDriver_1_PulseGenerator_INIT_PERIOD_VALUE);
    #endif /* (!BLDCDriver_1_PulseGenerator_UsingFixedFunction) */

    BLDCDriver_1_PulseGenerator_WritePeriod(BLDCDriver_1_PulseGenerator_INIT_PERIOD_VALUE);

        #if (BLDCDriver_1_PulseGenerator_UseOneCompareMode)
            BLDCDriver_1_PulseGenerator_WriteCompare(BLDCDriver_1_PulseGenerator_INIT_COMPARE_VALUE1);
        #else
            BLDCDriver_1_PulseGenerator_WriteCompare1(BLDCDriver_1_PulseGenerator_INIT_COMPARE_VALUE1);
            BLDCDriver_1_PulseGenerator_WriteCompare2(BLDCDriver_1_PulseGenerator_INIT_COMPARE_VALUE2);
        #endif /* (BLDCDriver_1_PulseGenerator_UseOneCompareMode) */

        #if (BLDCDriver_1_PulseGenerator_KillModeMinTime)
            BLDCDriver_1_PulseGenerator_WriteKillTime(BLDCDriver_1_PulseGenerator_MinimumKillTime);
        #endif /* (BLDCDriver_1_PulseGenerator_KillModeMinTime) */

        #if (BLDCDriver_1_PulseGenerator_DeadBandUsed)
            BLDCDriver_1_PulseGenerator_WriteDeadTime(BLDCDriver_1_PulseGenerator_INIT_DEAD_TIME);
        #endif /* (BLDCDriver_1_PulseGenerator_DeadBandUsed) */

    #if (BLDCDriver_1_PulseGenerator_UseStatus || BLDCDriver_1_PulseGenerator_UsingFixedFunction)
        BLDCDriver_1_PulseGenerator_SetInterruptMode(BLDCDriver_1_PulseGenerator_INIT_INTERRUPTS_MODE);
    #endif /* (BLDCDriver_1_PulseGenerator_UseStatus || BLDCDriver_1_PulseGenerator_UsingFixedFunction) */

    #if (BLDCDriver_1_PulseGenerator_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        BLDCDriver_1_PulseGenerator_GLOBAL_ENABLE |= BLDCDriver_1_PulseGenerator_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        BLDCDriver_1_PulseGenerator_CONTROL2 |= BLDCDriver_1_PulseGenerator_CTRL2_IRQ_SEL;
    #else
        #if(BLDCDriver_1_PulseGenerator_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            BLDCDriver_1_PulseGenerator_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            BLDCDriver_1_PulseGenerator_STATUS_AUX_CTRL |= BLDCDriver_1_PulseGenerator_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(BLDCDriver_1_PulseGenerator_interruptState);

            /* Clear the FIFO to enable the BLDCDriver_1_PulseGenerator_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            BLDCDriver_1_PulseGenerator_ClearFIFO();
        #endif /* (BLDCDriver_1_PulseGenerator_UseStatus) */
    #endif /* (BLDCDriver_1_PulseGenerator_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCDriver_1_PulseGenerator_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void BLDCDriver_1_PulseGenerator_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (BLDCDriver_1_PulseGenerator_UsingFixedFunction)
        BLDCDriver_1_PulseGenerator_GLOBAL_ENABLE |= BLDCDriver_1_PulseGenerator_BLOCK_EN_MASK;
        BLDCDriver_1_PulseGenerator_GLOBAL_STBY_ENABLE |= BLDCDriver_1_PulseGenerator_BLOCK_STBY_EN_MASK;
    #endif /* (BLDCDriver_1_PulseGenerator_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (BLDCDriver_1_PulseGenerator_UseControl || BLDCDriver_1_PulseGenerator_UsingFixedFunction)
        BLDCDriver_1_PulseGenerator_CONTROL |= BLDCDriver_1_PulseGenerator_CTRL_ENABLE;
    #endif /* (BLDCDriver_1_PulseGenerator_UseControl || BLDCDriver_1_PulseGenerator_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: BLDCDriver_1_PulseGenerator_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void BLDCDriver_1_PulseGenerator_Stop(void) 
{
    #if (BLDCDriver_1_PulseGenerator_UseControl || BLDCDriver_1_PulseGenerator_UsingFixedFunction)
        BLDCDriver_1_PulseGenerator_CONTROL &= ((uint8)(~BLDCDriver_1_PulseGenerator_CTRL_ENABLE));
    #endif /* (BLDCDriver_1_PulseGenerator_UseControl || BLDCDriver_1_PulseGenerator_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (BLDCDriver_1_PulseGenerator_UsingFixedFunction)
        BLDCDriver_1_PulseGenerator_GLOBAL_ENABLE &= ((uint8)(~BLDCDriver_1_PulseGenerator_BLOCK_EN_MASK));
        BLDCDriver_1_PulseGenerator_GLOBAL_STBY_ENABLE &= ((uint8)(~BLDCDriver_1_PulseGenerator_BLOCK_STBY_EN_MASK));
    #endif /* (BLDCDriver_1_PulseGenerator_UsingFixedFunction) */
}

#if (BLDCDriver_1_PulseGenerator_UseOneCompareMode)
    #if (BLDCDriver_1_PulseGenerator_CompareMode1SW)


        /*******************************************************************************
        * Function Name: BLDCDriver_1_PulseGenerator_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void BLDCDriver_1_PulseGenerator_SetCompareMode(uint8 comparemode) 
        {
            #if(BLDCDriver_1_PulseGenerator_UsingFixedFunction)

                #if(0 != BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_SHIFT) */

                BLDCDriver_1_PulseGenerator_CONTROL3 &= ((uint8)(~BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                BLDCDriver_1_PulseGenerator_CONTROL3 |= comparemodemasked;

            #elif (BLDCDriver_1_PulseGenerator_UseControl)

                #if(0 != BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_SHIFT)) &
                                                BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_MASK;
                #endif /* (0 != BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_SHIFT) */

                #if(0 != BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_SHIFT)) &
                                               BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_MASK;
                #endif /* (0 != BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                BLDCDriver_1_PulseGenerator_CONTROL &= ((uint8)(~(BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_MASK |
                                            BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_MASK)));
                BLDCDriver_1_PulseGenerator_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (BLDCDriver_1_PulseGenerator_UsingFixedFunction) */
        }
    #endif /* BLDCDriver_1_PulseGenerator_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (BLDCDriver_1_PulseGenerator_CompareMode1SW)


        /*******************************************************************************
        * Function Name: BLDCDriver_1_PulseGenerator_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void BLDCDriver_1_PulseGenerator_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_SHIFT)) &
                                           BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_MASK;
            #endif /* (0 != BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_SHIFT) */

            #if (BLDCDriver_1_PulseGenerator_UseControl)
                BLDCDriver_1_PulseGenerator_CONTROL &= ((uint8)(~BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                BLDCDriver_1_PulseGenerator_CONTROL |= comparemodemasked;
            #endif /* (BLDCDriver_1_PulseGenerator_UseControl) */
        }
    #endif /* BLDCDriver_1_PulseGenerator_CompareMode1SW */

#if (BLDCDriver_1_PulseGenerator_CompareMode2SW)


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void BLDCDriver_1_PulseGenerator_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_SHIFT)) &
                                                 BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_MASK;
        #endif /* (0 != BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_SHIFT) */

        #if (BLDCDriver_1_PulseGenerator_UseControl)
            BLDCDriver_1_PulseGenerator_CONTROL &= ((uint8)(~BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            BLDCDriver_1_PulseGenerator_CONTROL |= comparemodemasked;
        #endif /* (BLDCDriver_1_PulseGenerator_UseControl) */
    }
    #endif /*BLDCDriver_1_PulseGenerator_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!BLDCDriver_1_PulseGenerator_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void BLDCDriver_1_PulseGenerator_WriteCounter(uint16 counter) \
                                       
    {
        CY_SET_REG16(BLDCDriver_1_PulseGenerator_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint16 BLDCDriver_1_PulseGenerator_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(BLDCDriver_1_PulseGenerator_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG16(BLDCDriver_1_PulseGenerator_CAPTURE_LSB_PTR));
    }

    #if (BLDCDriver_1_PulseGenerator_UseStatus)


        /*******************************************************************************
        * Function Name: BLDCDriver_1_PulseGenerator_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void BLDCDriver_1_PulseGenerator_ClearFIFO(void) 
        {
            while(0u != (BLDCDriver_1_PulseGenerator_ReadStatusRegister() & BLDCDriver_1_PulseGenerator_STATUS_FIFONEMPTY))
            {
                (void)BLDCDriver_1_PulseGenerator_ReadCapture();
            }
        }

    #endif /* BLDCDriver_1_PulseGenerator_UseStatus */

#endif /* !BLDCDriver_1_PulseGenerator_UsingFixedFunction */


/*******************************************************************************
* Function Name: BLDCDriver_1_PulseGenerator_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void BLDCDriver_1_PulseGenerator_WritePeriod(uint16 period) 
{
    #if(BLDCDriver_1_PulseGenerator_UsingFixedFunction)
        CY_SET_REG16(BLDCDriver_1_PulseGenerator_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG16(BLDCDriver_1_PulseGenerator_PERIOD_LSB_PTR, period);
    #endif /* (BLDCDriver_1_PulseGenerator_UsingFixedFunction) */
}

#if (BLDCDriver_1_PulseGenerator_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void BLDCDriver_1_PulseGenerator_WriteCompare(uint16 compare) \
                                       
    {
        #if(BLDCDriver_1_PulseGenerator_UsingFixedFunction)
            CY_SET_REG16(BLDCDriver_1_PulseGenerator_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(BLDCDriver_1_PulseGenerator_COMPARE1_LSB_PTR, compare);
        #endif /* (BLDCDriver_1_PulseGenerator_UsingFixedFunction) */

        #if (BLDCDriver_1_PulseGenerator_PWMMode == BLDCDriver_1_PulseGenerator__B_PWM__DITHER)
            #if(BLDCDriver_1_PulseGenerator_UsingFixedFunction)
                CY_SET_REG16(BLDCDriver_1_PulseGenerator_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG16(BLDCDriver_1_PulseGenerator_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (BLDCDriver_1_PulseGenerator_UsingFixedFunction) */
        #endif /* (BLDCDriver_1_PulseGenerator_PWMMode == BLDCDriver_1_PulseGenerator__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void BLDCDriver_1_PulseGenerator_WriteCompare1(uint16 compare) \
                                        
    {
        #if(BLDCDriver_1_PulseGenerator_UsingFixedFunction)
            CY_SET_REG16(BLDCDriver_1_PulseGenerator_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG16(BLDCDriver_1_PulseGenerator_COMPARE1_LSB_PTR, compare);
        #endif /* (BLDCDriver_1_PulseGenerator_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void BLDCDriver_1_PulseGenerator_WriteCompare2(uint16 compare) \
                                        
    {
        #if(BLDCDriver_1_PulseGenerator_UsingFixedFunction)
            CY_SET_REG16(BLDCDriver_1_PulseGenerator_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG16(BLDCDriver_1_PulseGenerator_COMPARE2_LSB_PTR, compare);
        #endif /* (BLDCDriver_1_PulseGenerator_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (BLDCDriver_1_PulseGenerator_DeadBandUsed)


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void BLDCDriver_1_PulseGenerator_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!BLDCDriver_1_PulseGenerator_DeadBand2_4)
            CY_SET_REG8(BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            BLDCDriver_1_PulseGenerator_DEADBAND_COUNT &= ((uint8)(~BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_SHIFT)
                BLDCDriver_1_PulseGenerator_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_SHIFT)) &
                                                    BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_MASK;
            #else
                BLDCDriver_1_PulseGenerator_DEADBAND_COUNT |= deadtime & BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_MASK;
            #endif /* (BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_SHIFT) */

        #endif /* (!BLDCDriver_1_PulseGenerator_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 BLDCDriver_1_PulseGenerator_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!BLDCDriver_1_PulseGenerator_DeadBand2_4)
            return (CY_GET_REG8(BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(BLDCDriver_1_PulseGenerator_DEADBAND_COUNT & BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_MASK)) >>
                                                                           BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_SHIFT));
            #else
                return (BLDCDriver_1_PulseGenerator_DEADBAND_COUNT & BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_MASK);
            #endif /* (BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_SHIFT) */
        #endif /* (!BLDCDriver_1_PulseGenerator_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (BLDCDriver_1_PulseGenerator_UseStatus || BLDCDriver_1_PulseGenerator_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void BLDCDriver_1_PulseGenerator_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(BLDCDriver_1_PulseGenerator_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 BLDCDriver_1_PulseGenerator_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(BLDCDriver_1_PulseGenerator_STATUS_PTR));
    }

#endif /* (BLDCDriver_1_PulseGenerator_UseStatus || BLDCDriver_1_PulseGenerator_UsingFixedFunction) */


#if (BLDCDriver_1_PulseGenerator_UseControl)


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 BLDCDriver_1_PulseGenerator_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(BLDCDriver_1_PulseGenerator_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void BLDCDriver_1_PulseGenerator_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(BLDCDriver_1_PulseGenerator_CONTROL_PTR, control);
    }

#endif /* (BLDCDriver_1_PulseGenerator_UseControl) */


#if (!BLDCDriver_1_PulseGenerator_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint16 BLDCDriver_1_PulseGenerator_ReadCapture(void) 
    {
        return (CY_GET_REG16(BLDCDriver_1_PulseGenerator_CAPTURE_LSB_PTR));
    }

#endif /* (!BLDCDriver_1_PulseGenerator_UsingFixedFunction) */


#if (BLDCDriver_1_PulseGenerator_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint16 BLDCDriver_1_PulseGenerator_ReadCompare(void) 
    {
        #if(BLDCDriver_1_PulseGenerator_UsingFixedFunction)
            return ((uint16)CY_GET_REG16(BLDCDriver_1_PulseGenerator_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG16(BLDCDriver_1_PulseGenerator_COMPARE1_LSB_PTR));
        #endif /* (BLDCDriver_1_PulseGenerator_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 BLDCDriver_1_PulseGenerator_ReadCompare1(void) 
    {
        return (CY_GET_REG16(BLDCDriver_1_PulseGenerator_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint16 BLDCDriver_1_PulseGenerator_ReadCompare2(void) 
    {
        return (CY_GET_REG16(BLDCDriver_1_PulseGenerator_COMPARE2_LSB_PTR));
    }

#endif /* (BLDCDriver_1_PulseGenerator_UseOneCompareMode) */


/*******************************************************************************
* Function Name: BLDCDriver_1_PulseGenerator_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint16 BLDCDriver_1_PulseGenerator_ReadPeriod(void) 
{
    #if(BLDCDriver_1_PulseGenerator_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(BLDCDriver_1_PulseGenerator_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(BLDCDriver_1_PulseGenerator_PERIOD_LSB_PTR));
    #endif /* (BLDCDriver_1_PulseGenerator_UsingFixedFunction) */
}

#if ( BLDCDriver_1_PulseGenerator_KillModeMinTime)


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void BLDCDriver_1_PulseGenerator_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(BLDCDriver_1_PulseGenerator_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: BLDCDriver_1_PulseGenerator_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 BLDCDriver_1_PulseGenerator_ReadKillTime(void) 
    {
        return (CY_GET_REG8(BLDCDriver_1_PulseGenerator_KILLMODEMINTIME_PTR));
    }

#endif /* ( BLDCDriver_1_PulseGenerator_KillModeMinTime) */

/* [] END OF FILE */

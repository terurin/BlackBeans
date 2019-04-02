/*******************************************************************************
* File Name: UartHost.c
* Version 2.50
*
* Description:
*  This file provides all API functionality of the UART component
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
#if (UartHost_INTERNAL_CLOCK_USED)
    #include "UartHost_IntClock.h"
#endif /* End UartHost_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 UartHost_initVar = 0u;

#if (UartHost_TX_INTERRUPT_ENABLED && UartHost_TX_ENABLED)
    volatile uint8 UartHost_txBuffer[UartHost_TX_BUFFER_SIZE];
    volatile uint8 UartHost_txBufferRead = 0u;
    uint8 UartHost_txBufferWrite = 0u;
#endif /* (UartHost_TX_INTERRUPT_ENABLED && UartHost_TX_ENABLED) */

#if (UartHost_RX_INTERRUPT_ENABLED && (UartHost_RX_ENABLED || UartHost_HD_ENABLED))
    uint8 UartHost_errorStatus = 0u;
    volatile uint8 UartHost_rxBuffer[UartHost_RX_BUFFER_SIZE];
    volatile uint8 UartHost_rxBufferRead  = 0u;
    volatile uint8 UartHost_rxBufferWrite = 0u;
    volatile uint8 UartHost_rxBufferLoopDetect = 0u;
    volatile uint8 UartHost_rxBufferOverflow   = 0u;
    #if (UartHost_RXHW_ADDRESS_ENABLED)
        volatile uint8 UartHost_rxAddressMode = UartHost_RX_ADDRESS_MODE;
        volatile uint8 UartHost_rxAddressDetected = 0u;
    #endif /* (UartHost_RXHW_ADDRESS_ENABLED) */
#endif /* (UartHost_RX_INTERRUPT_ENABLED && (UartHost_RX_ENABLED || UartHost_HD_ENABLED)) */


/*******************************************************************************
* Function Name: UartHost_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  UartHost_Start() sets the initVar variable, calls the
*  UartHost_Init() function, and then calls the
*  UartHost_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The UartHost_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time UartHost_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the UartHost_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UartHost_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(UartHost_initVar == 0u)
    {
        UartHost_Init();
        UartHost_initVar = 1u;
    }

    UartHost_Enable();
}


/*******************************************************************************
* Function Name: UartHost_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call UartHost_Init() because
*  the UartHost_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void UartHost_Init(void) 
{
    #if(UartHost_RX_ENABLED || UartHost_HD_ENABLED)

        #if (UartHost_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(UartHost_RX_VECT_NUM, &UartHost_RXISR);
            CyIntSetPriority(UartHost_RX_VECT_NUM, UartHost_RX_PRIOR_NUM);
            UartHost_errorStatus = 0u;
        #endif /* (UartHost_RX_INTERRUPT_ENABLED) */

        #if (UartHost_RXHW_ADDRESS_ENABLED)
            UartHost_SetRxAddressMode(UartHost_RX_ADDRESS_MODE);
            UartHost_SetRxAddress1(UartHost_RX_HW_ADDRESS1);
            UartHost_SetRxAddress2(UartHost_RX_HW_ADDRESS2);
        #endif /* End UartHost_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        UartHost_RXBITCTR_PERIOD_REG = UartHost_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        UartHost_RXSTATUS_MASK_REG  = UartHost_INIT_RX_INTERRUPTS_MASK;
    #endif /* End UartHost_RX_ENABLED || UartHost_HD_ENABLED*/

    #if(UartHost_TX_ENABLED)
        #if (UartHost_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(UartHost_TX_VECT_NUM, &UartHost_TXISR);
            CyIntSetPriority(UartHost_TX_VECT_NUM, UartHost_TX_PRIOR_NUM);
        #endif /* (UartHost_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (UartHost_TXCLKGEN_DP)
            UartHost_TXBITCLKGEN_CTR_REG = UartHost_BIT_CENTER;
            UartHost_TXBITCLKTX_COMPLETE_REG = ((UartHost_NUMBER_OF_DATA_BITS +
                        UartHost_NUMBER_OF_START_BIT) * UartHost_OVER_SAMPLE_COUNT) - 1u;
        #else
            UartHost_TXBITCTR_PERIOD_REG = ((UartHost_NUMBER_OF_DATA_BITS +
                        UartHost_NUMBER_OF_START_BIT) * UartHost_OVER_SAMPLE_8) - 1u;
        #endif /* End UartHost_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (UartHost_TX_INTERRUPT_ENABLED)
            UartHost_TXSTATUS_MASK_REG = UartHost_TX_STS_FIFO_EMPTY;
        #else
            UartHost_TXSTATUS_MASK_REG = UartHost_INIT_TX_INTERRUPTS_MASK;
        #endif /*End UartHost_TX_INTERRUPT_ENABLED*/

    #endif /* End UartHost_TX_ENABLED */

    #if(UartHost_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        UartHost_WriteControlRegister( \
            (UartHost_ReadControlRegister() & (uint8)~UartHost_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(UartHost_PARITY_TYPE << UartHost_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End UartHost_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: UartHost_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call UartHost_Enable() because the UartHost_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UartHost_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void UartHost_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (UartHost_RX_ENABLED || UartHost_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        UartHost_RXBITCTR_CONTROL_REG |= UartHost_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        UartHost_RXSTATUS_ACTL_REG  |= UartHost_INT_ENABLE;

        #if (UartHost_RX_INTERRUPT_ENABLED)
            UartHost_EnableRxInt();

            #if (UartHost_RXHW_ADDRESS_ENABLED)
                UartHost_rxAddressDetected = 0u;
            #endif /* (UartHost_RXHW_ADDRESS_ENABLED) */
        #endif /* (UartHost_RX_INTERRUPT_ENABLED) */
    #endif /* (UartHost_RX_ENABLED || UartHost_HD_ENABLED) */

    #if(UartHost_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!UartHost_TXCLKGEN_DP)
            UartHost_TXBITCTR_CONTROL_REG |= UartHost_CNTR_ENABLE;
        #endif /* End UartHost_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        UartHost_TXSTATUS_ACTL_REG |= UartHost_INT_ENABLE;
        #if (UartHost_TX_INTERRUPT_ENABLED)
            UartHost_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            UartHost_EnableTxInt();
        #endif /* (UartHost_TX_INTERRUPT_ENABLED) */
     #endif /* (UartHost_TX_INTERRUPT_ENABLED) */

    #if (UartHost_INTERNAL_CLOCK_USED)
        UartHost_IntClock_Start();  /* Enable the clock */
    #endif /* (UartHost_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: UartHost_Stop
********************************************************************************
*
* Summary:
*  Disables the UART operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void UartHost_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (UartHost_RX_ENABLED || UartHost_HD_ENABLED)
        UartHost_RXBITCTR_CONTROL_REG &= (uint8) ~UartHost_CNTR_ENABLE;
    #endif /* (UartHost_RX_ENABLED || UartHost_HD_ENABLED) */

    #if (UartHost_TX_ENABLED)
        #if(!UartHost_TXCLKGEN_DP)
            UartHost_TXBITCTR_CONTROL_REG &= (uint8) ~UartHost_CNTR_ENABLE;
        #endif /* (!UartHost_TXCLKGEN_DP) */
    #endif /* (UartHost_TX_ENABLED) */

    #if (UartHost_INTERNAL_CLOCK_USED)
        UartHost_IntClock_Stop();   /* Disable the clock */
    #endif /* (UartHost_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (UartHost_RX_ENABLED || UartHost_HD_ENABLED)
        UartHost_RXSTATUS_ACTL_REG  &= (uint8) ~UartHost_INT_ENABLE;

        #if (UartHost_RX_INTERRUPT_ENABLED)
            UartHost_DisableRxInt();
        #endif /* (UartHost_RX_INTERRUPT_ENABLED) */
    #endif /* (UartHost_RX_ENABLED || UartHost_HD_ENABLED) */

    #if (UartHost_TX_ENABLED)
        UartHost_TXSTATUS_ACTL_REG &= (uint8) ~UartHost_INT_ENABLE;

        #if (UartHost_TX_INTERRUPT_ENABLED)
            UartHost_DisableTxInt();
        #endif /* (UartHost_TX_INTERRUPT_ENABLED) */
    #endif /* (UartHost_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: UartHost_ReadControlRegister
********************************************************************************
*
* Summary:
*  Returns the current value of the control register.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the control register.
*
*******************************************************************************/
uint8 UartHost_ReadControlRegister(void) 
{
    #if (UartHost_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(UartHost_CONTROL_REG);
    #endif /* (UartHost_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: UartHost_WriteControlRegister
********************************************************************************
*
* Summary:
*  Writes an 8-bit value into the control register
*
* Parameters:
*  control:  control register value
*
* Return:
*  None.
*
*******************************************************************************/
void  UartHost_WriteControlRegister(uint8 control) 
{
    #if (UartHost_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       UartHost_CONTROL_REG = control;
    #endif /* (UartHost_CONTROL_REG_REMOVED) */
}


#if(UartHost_RX_ENABLED || UartHost_HD_ENABLED)
    /*******************************************************************************
    * Function Name: UartHost_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      UartHost_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      UartHost_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      UartHost_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      UartHost_RX_STS_BREAK            Interrupt on break.
    *      UartHost_RX_STS_OVERRUN          Interrupt on overrun error.
    *      UartHost_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      UartHost_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void UartHost_SetRxInterruptMode(uint8 intSrc) 
    {
        UartHost_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: UartHost_ReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Returns the next byte of received data. This function returns data without
    *  checking the status. You must check the status separately.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Received data from RX register
    *
    * Global Variables:
    *  UartHost_rxBuffer - RAM buffer pointer for save received data.
    *  UartHost_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  UartHost_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  UartHost_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 UartHost_ReadRxData(void) 
    {
        uint8 rxData;

    #if (UartHost_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        UartHost_DisableRxInt();

        locRxBufferRead  = UartHost_rxBufferRead;
        locRxBufferWrite = UartHost_rxBufferWrite;

        if( (UartHost_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = UartHost_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= UartHost_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            UartHost_rxBufferRead = locRxBufferRead;

            if(UartHost_rxBufferLoopDetect != 0u)
            {
                UartHost_rxBufferLoopDetect = 0u;
                #if ((UartHost_RX_INTERRUPT_ENABLED) && (UartHost_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( UartHost_HD_ENABLED )
                        if((UartHost_CONTROL_REG & UartHost_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            UartHost_RXSTATUS_MASK_REG  |= UartHost_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        UartHost_RXSTATUS_MASK_REG  |= UartHost_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end UartHost_HD_ENABLED */
                #endif /* ((UartHost_RX_INTERRUPT_ENABLED) && (UartHost_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = UartHost_RXDATA_REG;
        }

        UartHost_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = UartHost_RXDATA_REG;

    #endif /* (UartHost_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: UartHost_ReadRxStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the receiver status register and the software
    *  buffer overflow status.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Current state of the status register.
    *
    * Side Effect:
    *  All status register bits are clear-on-read except
    *  UartHost_RX_STS_FIFO_NOTEMPTY.
    *  UartHost_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  UartHost_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   UartHost_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   UartHost_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 UartHost_ReadRxStatus(void) 
    {
        uint8 status;

        status = UartHost_RXSTATUS_REG & UartHost_RX_HW_MASK;

    #if (UartHost_RX_INTERRUPT_ENABLED)
        if(UartHost_rxBufferOverflow != 0u)
        {
            status |= UartHost_RX_STS_SOFT_BUFF_OVER;
            UartHost_rxBufferOverflow = 0u;
        }
    #endif /* (UartHost_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: UartHost_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. UartHost_GetChar() is
    *  designed for ASCII characters and returns a uint8 where 1 to 255 are values
    *  for valid characters and 0 indicates an error occurred or no data is present.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Character read from UART RX buffer. ASCII characters from 1 to 255 are valid.
    *  A returned zero signifies an error condition or no data available.
    *
    * Global Variables:
    *  UartHost_rxBuffer - RAM buffer pointer for save received data.
    *  UartHost_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  UartHost_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  UartHost_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 UartHost_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (UartHost_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        UartHost_DisableRxInt();

        locRxBufferRead  = UartHost_rxBufferRead;
        locRxBufferWrite = UartHost_rxBufferWrite;

        if( (UartHost_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = UartHost_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= UartHost_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            UartHost_rxBufferRead = locRxBufferRead;

            if(UartHost_rxBufferLoopDetect != 0u)
            {
                UartHost_rxBufferLoopDetect = 0u;
                #if( (UartHost_RX_INTERRUPT_ENABLED) && (UartHost_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( UartHost_HD_ENABLED )
                        if((UartHost_CONTROL_REG & UartHost_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            UartHost_RXSTATUS_MASK_REG |= UartHost_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        UartHost_RXSTATUS_MASK_REG |= UartHost_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end UartHost_HD_ENABLED */
                #endif /* UartHost_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = UartHost_RXSTATUS_REG;
            if((rxStatus & UartHost_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = UartHost_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (UartHost_RX_STS_BREAK | UartHost_RX_STS_PAR_ERROR |
                                UartHost_RX_STS_STOP_ERROR | UartHost_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        UartHost_EnableRxInt();

    #else

        rxStatus =UartHost_RXSTATUS_REG;
        if((rxStatus & UartHost_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = UartHost_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (UartHost_RX_STS_BREAK | UartHost_RX_STS_PAR_ERROR |
                            UartHost_RX_STS_STOP_ERROR | UartHost_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (UartHost_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: UartHost_GetByte
    ********************************************************************************
    *
    * Summary:
    *  Reads UART RX buffer immediately, returns received character and error
    *  condition.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  MSB contains status and LSB contains UART RX data. If the MSB is nonzero,
    *  an error has occurred.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 UartHost_GetByte(void) 
    {
        
    #if (UartHost_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        UartHost_DisableRxInt();
        locErrorStatus = (uint16)UartHost_errorStatus;
        UartHost_errorStatus = 0u;
        UartHost_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | UartHost_ReadRxData() );
    #else
        return ( ((uint16)UartHost_ReadRxStatus() << 8u) | UartHost_ReadRxData() );
    #endif /* UartHost_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: UartHost_GetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received bytes available in the RX buffer.
    *  * RX software buffer is disabled (RX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty RX FIFO or 1 for not empty RX FIFO.
    *  * RX software buffer is enabled: returns the number of bytes available in 
    *    the RX software buffer. Bytes available in the RX FIFO do not take to 
    *    account.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  uint8: Number of bytes in the RX buffer. 
    *    Return value type depends on RX Buffer Size parameter.
    *
    * Global Variables:
    *  UartHost_rxBufferWrite - used to calculate left bytes.
    *  UartHost_rxBufferRead - used to calculate left bytes.
    *  UartHost_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 UartHost_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (UartHost_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        UartHost_DisableRxInt();

        if(UartHost_rxBufferRead == UartHost_rxBufferWrite)
        {
            if(UartHost_rxBufferLoopDetect != 0u)
            {
                size = UartHost_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(UartHost_rxBufferRead < UartHost_rxBufferWrite)
        {
            size = (UartHost_rxBufferWrite - UartHost_rxBufferRead);
        }
        else
        {
            size = (UartHost_RX_BUFFER_SIZE - UartHost_rxBufferRead) + UartHost_rxBufferWrite;
        }

        UartHost_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((UartHost_RXSTATUS_REG & UartHost_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (UartHost_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: UartHost_ClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the receiver memory buffer and hardware RX FIFO of all received data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UartHost_rxBufferWrite - cleared to zero.
    *  UartHost_rxBufferRead - cleared to zero.
    *  UartHost_rxBufferLoopDetect - cleared to zero.
    *  UartHost_rxBufferOverflow - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may
    *  have remained in the RAM.
    *
    * Side Effects:
    *  Any received data not read from the RAM or FIFO buffer will be lost.
    *
    *******************************************************************************/
    void UartHost_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        UartHost_RXDATA_AUX_CTL_REG |= (uint8)  UartHost_RX_FIFO_CLR;
        UartHost_RXDATA_AUX_CTL_REG &= (uint8) ~UartHost_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (UartHost_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        UartHost_DisableRxInt();

        UartHost_rxBufferRead = 0u;
        UartHost_rxBufferWrite = 0u;
        UartHost_rxBufferLoopDetect = 0u;
        UartHost_rxBufferOverflow = 0u;

        UartHost_EnableRxInt();

    #endif /* (UartHost_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: UartHost_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  UartHost__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  UartHost__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  UartHost__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  UartHost__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  UartHost__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UartHost_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  UartHost_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void UartHost_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(UartHost_RXHW_ADDRESS_ENABLED)
            #if(UartHost_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* UartHost_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = UartHost_CONTROL_REG & (uint8)~UartHost_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << UartHost_CTRL_RXADDR_MODE0_SHIFT);
                UartHost_CONTROL_REG = tmpCtrl;

                #if(UartHost_RX_INTERRUPT_ENABLED && \
                   (UartHost_RXBUFFERSIZE > UartHost_FIFO_LENGTH) )
                    UartHost_rxAddressMode = addressMode;
                    UartHost_rxAddressDetected = 0u;
                #endif /* End UartHost_RXBUFFERSIZE > UartHost_FIFO_LENGTH*/
            #endif /* End UartHost_CONTROL_REG_REMOVED */
        #else /* UartHost_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End UartHost_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: UartHost_SetRxAddress1
    ********************************************************************************
    *
    * Summary:
    *  Sets the first of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #1 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void UartHost_SetRxAddress1(uint8 address) 
    {
        UartHost_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: UartHost_SetRxAddress2
    ********************************************************************************
    *
    * Summary:
    *  Sets the second of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #2 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void UartHost_SetRxAddress2(uint8 address) 
    {
        UartHost_RXADDRESS2_REG = address;
    }

#endif  /* UartHost_RX_ENABLED || UartHost_HD_ENABLED*/


#if( (UartHost_TX_ENABLED) || (UartHost_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: UartHost_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   UartHost_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   UartHost_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   UartHost_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   UartHost_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void UartHost_SetTxInterruptMode(uint8 intSrc) 
    {
        UartHost_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: UartHost_WriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a byte of data into the transmit buffer to be sent when the bus is
    *  available without checking the TX status register. You must check status
    *  separately.
    *
    * Parameters:
    *  txDataByte: data byte
    *
    * Return:
    * None.
    *
    * Global Variables:
    *  UartHost_txBuffer - RAM buffer pointer for save data for transmission
    *  UartHost_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  UartHost_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  UartHost_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void UartHost_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(UartHost_initVar != 0u)
        {
        #if (UartHost_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            UartHost_DisableTxInt();

            if( (UartHost_txBufferRead == UartHost_txBufferWrite) &&
                ((UartHost_TXSTATUS_REG & UartHost_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                UartHost_TXDATA_REG = txDataByte;
            }
            else
            {
                if(UartHost_txBufferWrite >= UartHost_TX_BUFFER_SIZE)
                {
                    UartHost_txBufferWrite = 0u;
                }

                UartHost_txBuffer[UartHost_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                UartHost_txBufferWrite++;
            }

            UartHost_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            UartHost_TXDATA_REG = txDataByte;

        #endif /*(UartHost_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: UartHost_ReadTxStatus
    ********************************************************************************
    *
    * Summary:
    *  Reads the status register for the TX portion of the UART.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Contents of the status register
    *
    * Theory:
    *  This function reads the TX status register, which is cleared on read.
    *  It is up to the user to handle all bits in this return value accordingly,
    *  even if the bit was not enabled as an interrupt source the event happened
    *  and must be handled accordingly.
    *
    *******************************************************************************/
    uint8 UartHost_ReadTxStatus(void) 
    {
        return(UartHost_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: UartHost_PutChar
    ********************************************************************************
    *
    * Summary:
    *  Puts a byte of data into the transmit buffer to be sent when the bus is
    *  available. This is a blocking API that waits until the TX buffer has room to
    *  hold the data.
    *
    * Parameters:
    *  txDataByte: Byte containing the data to transmit
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UartHost_txBuffer - RAM buffer pointer for save data for transmission
    *  UartHost_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  UartHost_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  UartHost_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void UartHost_PutChar(uint8 txDataByte) 
    {
    #if (UartHost_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((UartHost_TX_BUFFER_SIZE > UartHost_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            UartHost_DisableTxInt();
        #endif /* (UartHost_TX_BUFFER_SIZE > UartHost_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = UartHost_txBufferWrite;
            locTxBufferRead  = UartHost_txBufferRead;

        #if ((UartHost_TX_BUFFER_SIZE > UartHost_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            UartHost_EnableTxInt();
        #endif /* (UartHost_TX_BUFFER_SIZE > UartHost_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(UartHost_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((UartHost_TXSTATUS_REG & UartHost_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            UartHost_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= UartHost_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            UartHost_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((UartHost_TX_BUFFER_SIZE > UartHost_MAX_BYTE_VALUE) && (CY_PSOC3))
            UartHost_DisableTxInt();
        #endif /* (UartHost_TX_BUFFER_SIZE > UartHost_MAX_BYTE_VALUE) && (CY_PSOC3) */

            UartHost_txBufferWrite = locTxBufferWrite;

        #if ((UartHost_TX_BUFFER_SIZE > UartHost_MAX_BYTE_VALUE) && (CY_PSOC3))
            UartHost_EnableTxInt();
        #endif /* (UartHost_TX_BUFFER_SIZE > UartHost_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (UartHost_TXSTATUS_REG & UartHost_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                UartHost_SetPendingTxInt();
            }
        }

    #else

        while((UartHost_TXSTATUS_REG & UartHost_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        UartHost_TXDATA_REG = txDataByte;

    #endif /* UartHost_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: UartHost_PutString
    ********************************************************************************
    *
    * Summary:
    *  Sends a NULL terminated string to the TX buffer for transmission.
    *
    * Parameters:
    *  string[]: Pointer to the null terminated string array residing in RAM or ROM
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UartHost_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void UartHost_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(UartHost_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                UartHost_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: UartHost_PutArray
    ********************************************************************************
    *
    * Summary:
    *  Places N bytes of data from a memory array into the TX buffer for
    *  transmission.
    *
    * Parameters:
    *  string[]: Address of the memory array residing in RAM or ROM.
    *  byteCount: Number of bytes to be transmitted. The type depends on TX Buffer
    *             Size parameter.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UartHost_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void UartHost_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(UartHost_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                UartHost_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: UartHost_PutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Writes a byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) to the transmit buffer.
    *
    * Parameters:
    *  txDataByte: Data byte to transmit before the carriage return and line feed.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UartHost_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void UartHost_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(UartHost_initVar != 0u)
        {
            UartHost_PutChar(txDataByte);
            UartHost_PutChar(0x0Du);
            UartHost_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: UartHost_GetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of bytes in the TX buffer which are waiting to be 
    *  transmitted.
    *  * TX software buffer is disabled (TX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty TX FIFO, 1 for not full TX FIFO or 4 for full TX FIFO.
    *  * TX software buffer is enabled: returns the number of bytes in the TX 
    *    software buffer which are waiting to be transmitted. Bytes available in the
    *    TX FIFO do not count.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Number of bytes used in the TX buffer. Return value type depends on the TX 
    *  Buffer Size parameter.
    *
    * Global Variables:
    *  UartHost_txBufferWrite - used to calculate left space.
    *  UartHost_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 UartHost_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (UartHost_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        UartHost_DisableTxInt();

        if(UartHost_txBufferRead == UartHost_txBufferWrite)
        {
            size = 0u;
        }
        else if(UartHost_txBufferRead < UartHost_txBufferWrite)
        {
            size = (UartHost_txBufferWrite - UartHost_txBufferRead);
        }
        else
        {
            size = (UartHost_TX_BUFFER_SIZE - UartHost_txBufferRead) +
                    UartHost_txBufferWrite;
        }

        UartHost_EnableTxInt();

    #else

        size = UartHost_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & UartHost_TX_STS_FIFO_FULL) != 0u)
        {
            size = UartHost_FIFO_LENGTH;
        }
        else if((size & UartHost_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (UartHost_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: UartHost_ClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears all data from the TX buffer and hardware TX FIFO.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UartHost_txBufferWrite - cleared to zero.
    *  UartHost_txBufferRead - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may have
    *  remained in the RAM.
    *
    * Side Effects:
    *  Data waiting in the transmit buffer is not sent; a byte that is currently
    *  transmitting finishes transmitting.
    *
    *******************************************************************************/
    void UartHost_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        UartHost_TXDATA_AUX_CTL_REG |= (uint8)  UartHost_TX_FIFO_CLR;
        UartHost_TXDATA_AUX_CTL_REG &= (uint8) ~UartHost_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (UartHost_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        UartHost_DisableTxInt();

        UartHost_txBufferRead = 0u;
        UartHost_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        UartHost_EnableTxInt();

    #endif /* (UartHost_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: UartHost_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   UartHost_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   UartHost_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   UartHost_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   UartHost_SEND_WAIT_REINIT - Performs both options: 
    *      UartHost_SEND_BREAK and UartHost_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UartHost_initVar - checked to identify that the component has been
    *     initialized.
    *  txPeriod - static variable, used for keeping TX period configuration.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  SendBreak function initializes registers to send 13-bit break signal. It is
    *  important to return the registers configuration to normal for continue 8-bit
    *  operation.
    *  There are 3 variants for this API usage:
    *  1) SendBreak(3) - function will send the Break signal and take care on the
    *     configuration returning. Function will block CPU until transmission
    *     complete.
    *  2) User may want to use blocking time if UART configured to the low speed
    *     operation
    *     Example for this case:
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     SendBreak(1);     - complete Break operation
    *  3) Same to 2) but user may want to initialize and use the interrupt to
    *     complete break operation.
    *     Example for this case:
    *     Initialize TX interrupt with "TX - On TX Complete" parameter
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     When interrupt appear with UartHost_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The UartHost_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void UartHost_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(UartHost_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(UartHost_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == UartHost_SEND_BREAK) ||
                (retMode == UartHost_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                UartHost_WriteControlRegister(UartHost_ReadControlRegister() |
                                                      UartHost_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                UartHost_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = UartHost_TXSTATUS_REG;
                }
                while((tmpStat & UartHost_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == UartHost_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == UartHost_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = UartHost_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & UartHost_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == UartHost_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == UartHost_REINIT) ||
                (retMode == UartHost_SEND_WAIT_REINIT) )
            {
                UartHost_WriteControlRegister(UartHost_ReadControlRegister() &
                                              (uint8)~UartHost_CTRL_HD_SEND_BREAK);
            }

        #else /* UartHost_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == UartHost_SEND_BREAK) ||
                (retMode == UartHost_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (UartHost_PARITY_TYPE != UartHost__B_UART__NONE_REVB) || \
                                    (UartHost_PARITY_TYPE_SW != 0u) )
                    UartHost_WriteControlRegister(UartHost_ReadControlRegister() |
                                                          UartHost_CTRL_HD_SEND_BREAK);
                #endif /* End UartHost_PARITY_TYPE != UartHost__B_UART__NONE_REVB  */

                #if(UartHost_TXCLKGEN_DP)
                    txPeriod = UartHost_TXBITCLKTX_COMPLETE_REG;
                    UartHost_TXBITCLKTX_COMPLETE_REG = UartHost_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = UartHost_TXBITCTR_PERIOD_REG;
                    UartHost_TXBITCTR_PERIOD_REG = UartHost_TXBITCTR_BREAKBITS8X;
                #endif /* End UartHost_TXCLKGEN_DP */

                /* Send zeros */
                UartHost_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = UartHost_TXSTATUS_REG;
                }
                while((tmpStat & UartHost_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == UartHost_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == UartHost_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = UartHost_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & UartHost_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == UartHost_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == UartHost_REINIT) ||
                (retMode == UartHost_SEND_WAIT_REINIT) )
            {

            #if(UartHost_TXCLKGEN_DP)
                UartHost_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                UartHost_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End UartHost_TXCLKGEN_DP */

            #if( (UartHost_PARITY_TYPE != UartHost__B_UART__NONE_REVB) || \
                 (UartHost_PARITY_TYPE_SW != 0u) )
                UartHost_WriteControlRegister(UartHost_ReadControlRegister() &
                                                      (uint8) ~UartHost_CTRL_HD_SEND_BREAK);
            #endif /* End UartHost_PARITY_TYPE != NONE */
            }
        #endif    /* End UartHost_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: UartHost_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       UartHost_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       UartHost_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears UartHost_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void UartHost_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( UartHost_CONTROL_REG_REMOVED == 0u )
            UartHost_WriteControlRegister(UartHost_ReadControlRegister() |
                                                  UartHost_CTRL_MARK);
        #endif /* End UartHost_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( UartHost_CONTROL_REG_REMOVED == 0u )
            UartHost_WriteControlRegister(UartHost_ReadControlRegister() &
                                                  (uint8) ~UartHost_CTRL_MARK);
        #endif /* End UartHost_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* EndUartHost_TX_ENABLED */

#if(UartHost_HD_ENABLED)


    /*******************************************************************************
    * Function Name: UartHost_LoadRxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the receiver configuration in half duplex mode. After calling this
    *  function, the UART is ready to receive data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the transmitter
    *  configuration.
    *
    *******************************************************************************/
    void UartHost_LoadRxConfig(void) 
    {
        UartHost_WriteControlRegister(UartHost_ReadControlRegister() &
                                                (uint8)~UartHost_CTRL_HD_SEND);
        UartHost_RXBITCTR_PERIOD_REG = UartHost_HD_RXBITCTR_INIT;

    #if (UartHost_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        UartHost_SetRxInterruptMode(UartHost_INIT_RX_INTERRUPTS_MASK);
    #endif /* (UartHost_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: UartHost_LoadTxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the transmitter configuration in half duplex mode. After calling this
    *  function, the UART is ready to transmit data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the receiver configuration.
    *
    *******************************************************************************/
    void UartHost_LoadTxConfig(void) 
    {
    #if (UartHost_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        UartHost_SetRxInterruptMode(0u);
    #endif /* (UartHost_RX_INTERRUPT_ENABLED) */

        UartHost_WriteControlRegister(UartHost_ReadControlRegister() | UartHost_CTRL_HD_SEND);
        UartHost_RXBITCTR_PERIOD_REG = UartHost_HD_TXBITCTR_INIT;
    }

#endif  /* UartHost_HD_ENABLED */


/* [] END OF FILE */

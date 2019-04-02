/*******************************************************************************
* File Name: UartHostINT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "UartHost.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (UartHost_RX_INTERRUPT_ENABLED && (UartHost_RX_ENABLED || UartHost_HD_ENABLED))
    /*******************************************************************************
    * Function Name: UartHost_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UartHost_rxBuffer - RAM buffer pointer for save received data.
    *  UartHost_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  UartHost_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  UartHost_rxBufferOverflow - software overflow flag. Set to one
    *     when UartHost_rxBufferWrite index overtakes
    *     UartHost_rxBufferRead index.
    *  UartHost_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when UartHost_rxBufferWrite is equal to
    *    UartHost_rxBufferRead
    *  UartHost_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  UartHost_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(UartHost_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef UartHost_RXISR_ENTRY_CALLBACK
        UartHost_RXISR_EntryCallback();
    #endif /* UartHost_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START UartHost_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = UartHost_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in UartHost_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (UartHost_RX_STS_BREAK | 
                            UartHost_RX_STS_PAR_ERROR |
                            UartHost_RX_STS_STOP_ERROR | 
                            UartHost_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                UartHost_errorStatus |= readStatus & ( UartHost_RX_STS_BREAK | 
                                                            UartHost_RX_STS_PAR_ERROR | 
                                                            UartHost_RX_STS_STOP_ERROR | 
                                                            UartHost_RX_STS_OVERRUN);
                /* `#START UartHost_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef UartHost_RXISR_ERROR_CALLBACK
                UartHost_RXISR_ERROR_Callback();
            #endif /* UartHost_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & UartHost_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = UartHost_RXDATA_REG;
            #if (UartHost_RXHW_ADDRESS_ENABLED)
                if(UartHost_rxAddressMode == (uint8)UartHost__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & UartHost_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & UartHost_RX_STS_ADDR_MATCH) != 0u)
                        {
                            UartHost_rxAddressDetected = 1u;
                        }
                        else
                        {
                            UartHost_rxAddressDetected = 0u;
                        }
                    }
                    if(UartHost_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        UartHost_rxBuffer[UartHost_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    UartHost_rxBuffer[UartHost_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                UartHost_rxBuffer[UartHost_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (UartHost_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(UartHost_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        UartHost_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    UartHost_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(UartHost_rxBufferWrite >= UartHost_RX_BUFFER_SIZE)
                    {
                        UartHost_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(UartHost_rxBufferWrite == UartHost_rxBufferRead)
                    {
                        UartHost_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (UartHost_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            UartHost_RXSTATUS_MASK_REG  &= (uint8)~UartHost_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(UartHost_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (UartHost_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & UartHost_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START UartHost_RXISR_END` */

        /* `#END` */

    #ifdef UartHost_RXISR_EXIT_CALLBACK
        UartHost_RXISR_ExitCallback();
    #endif /* UartHost_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (UartHost_RX_INTERRUPT_ENABLED && (UartHost_RX_ENABLED || UartHost_HD_ENABLED)) */


#if (UartHost_TX_INTERRUPT_ENABLED && UartHost_TX_ENABLED)
    /*******************************************************************************
    * Function Name: UartHost_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UartHost_txBuffer - RAM buffer pointer for transmit data from.
    *  UartHost_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  UartHost_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(UartHost_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef UartHost_TXISR_ENTRY_CALLBACK
        UartHost_TXISR_EntryCallback();
    #endif /* UartHost_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START UartHost_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((UartHost_txBufferRead != UartHost_txBufferWrite) &&
             ((UartHost_TXSTATUS_REG & UartHost_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(UartHost_txBufferRead >= UartHost_TX_BUFFER_SIZE)
            {
                UartHost_txBufferRead = 0u;
            }

            UartHost_TXDATA_REG = UartHost_txBuffer[UartHost_txBufferRead];

            /* Set next pointer */
            UartHost_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START UartHost_TXISR_END` */

        /* `#END` */

    #ifdef UartHost_TXISR_EXIT_CALLBACK
        UartHost_TXISR_ExitCallback();
    #endif /* UartHost_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (UartHost_TX_INTERRUPT_ENABLED && UartHost_TX_ENABLED) */


/* [] END OF FILE */

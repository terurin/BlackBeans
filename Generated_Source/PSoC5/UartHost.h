/*******************************************************************************
* File Name: UartHost.h
* Version 2.50
*
* Description:
*  Contains the function prototypes and constants available to the UART
*  user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_UART_UartHost_H)
#define CY_UART_UartHost_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define UartHost_RX_ENABLED                     (1u)
#define UartHost_TX_ENABLED                     (1u)
#define UartHost_HD_ENABLED                     (0u)
#define UartHost_RX_INTERRUPT_ENABLED           (1u)
#define UartHost_TX_INTERRUPT_ENABLED           (1u)
#define UartHost_INTERNAL_CLOCK_USED            (1u)
#define UartHost_RXHW_ADDRESS_ENABLED           (0u)
#define UartHost_OVER_SAMPLE_COUNT              (8u)
#define UartHost_PARITY_TYPE                    (0u)
#define UartHost_PARITY_TYPE_SW                 (0u)
#define UartHost_BREAK_DETECT                   (0u)
#define UartHost_BREAK_BITS_TX                  (11u)
#define UartHost_BREAK_BITS_RX                  (11u)
#define UartHost_TXCLKGEN_DP                    (1u)
#define UartHost_USE23POLLING                   (1u)
#define UartHost_FLOW_CONTROL                   (0u)
#define UartHost_CLK_FREQ                       (0u)
#define UartHost_TX_BUFFER_SIZE                 (32u)
#define UartHost_RX_BUFFER_SIZE                 (32u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(UartHost_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define UartHost_CONTROL_REG_REMOVED            (0u)
#else
    #define UartHost_CONTROL_REG_REMOVED            (1u)
#endif /* End UartHost_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct UartHost_backupStruct_
{
    uint8 enableState;

    #if(UartHost_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End UartHost_CONTROL_REG_REMOVED */

} UartHost_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void UartHost_Start(void) ;
void UartHost_Stop(void) ;
uint8 UartHost_ReadControlRegister(void) ;
void UartHost_WriteControlRegister(uint8 control) ;

void UartHost_Init(void) ;
void UartHost_Enable(void) ;
void UartHost_SaveConfig(void) ;
void UartHost_RestoreConfig(void) ;
void UartHost_Sleep(void) ;
void UartHost_Wakeup(void) ;

/* Only if RX is enabled */
#if( (UartHost_RX_ENABLED) || (UartHost_HD_ENABLED) )

    #if (UartHost_RX_INTERRUPT_ENABLED)
        #define UartHost_EnableRxInt()  CyIntEnable (UartHost_RX_VECT_NUM)
        #define UartHost_DisableRxInt() CyIntDisable(UartHost_RX_VECT_NUM)
        CY_ISR_PROTO(UartHost_RXISR);
    #endif /* UartHost_RX_INTERRUPT_ENABLED */

    void UartHost_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void UartHost_SetRxAddress1(uint8 address) ;
    void UartHost_SetRxAddress2(uint8 address) ;

    void  UartHost_SetRxInterruptMode(uint8 intSrc) ;
    uint8 UartHost_ReadRxData(void) ;
    uint8 UartHost_ReadRxStatus(void) ;
    uint8 UartHost_GetChar(void) ;
    uint16 UartHost_GetByte(void) ;
    uint8 UartHost_GetRxBufferSize(void)
                                                            ;
    void UartHost_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define UartHost_GetRxInterruptSource   UartHost_ReadRxStatus

#endif /* End (UartHost_RX_ENABLED) || (UartHost_HD_ENABLED) */

/* Only if TX is enabled */
#if(UartHost_TX_ENABLED || UartHost_HD_ENABLED)

    #if(UartHost_TX_INTERRUPT_ENABLED)
        #define UartHost_EnableTxInt()  CyIntEnable (UartHost_TX_VECT_NUM)
        #define UartHost_DisableTxInt() CyIntDisable(UartHost_TX_VECT_NUM)
        #define UartHost_SetPendingTxInt() CyIntSetPending(UartHost_TX_VECT_NUM)
        #define UartHost_ClearPendingTxInt() CyIntClearPending(UartHost_TX_VECT_NUM)
        CY_ISR_PROTO(UartHost_TXISR);
    #endif /* UartHost_TX_INTERRUPT_ENABLED */

    void UartHost_SetTxInterruptMode(uint8 intSrc) ;
    void UartHost_WriteTxData(uint8 txDataByte) ;
    uint8 UartHost_ReadTxStatus(void) ;
    void UartHost_PutChar(uint8 txDataByte) ;
    void UartHost_PutString(const char8 string[]) ;
    void UartHost_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void UartHost_PutCRLF(uint8 txDataByte) ;
    void UartHost_ClearTxBuffer(void) ;
    void UartHost_SetTxAddressMode(uint8 addressMode) ;
    void UartHost_SendBreak(uint8 retMode) ;
    uint8 UartHost_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define UartHost_PutStringConst         UartHost_PutString
    #define UartHost_PutArrayConst          UartHost_PutArray
    #define UartHost_GetTxInterruptSource   UartHost_ReadTxStatus

#endif /* End UartHost_TX_ENABLED || UartHost_HD_ENABLED */

#if(UartHost_HD_ENABLED)
    void UartHost_LoadRxConfig(void) ;
    void UartHost_LoadTxConfig(void) ;
#endif /* End UartHost_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_UartHost) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    UartHost_CyBtldrCommStart(void) CYSMALL ;
    void    UartHost_CyBtldrCommStop(void) CYSMALL ;
    void    UartHost_CyBtldrCommReset(void) CYSMALL ;
    cystatus UartHost_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus UartHost_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_UartHost)
        #define CyBtldrCommStart    UartHost_CyBtldrCommStart
        #define CyBtldrCommStop     UartHost_CyBtldrCommStop
        #define CyBtldrCommReset    UartHost_CyBtldrCommReset
        #define CyBtldrCommWrite    UartHost_CyBtldrCommWrite
        #define CyBtldrCommRead     UartHost_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_UartHost) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define UartHost_BYTE2BYTE_TIME_OUT (25u)
    #define UartHost_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define UartHost_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define UartHost_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define UartHost_SET_SPACE      (0x00u)
#define UartHost_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (UartHost_TX_ENABLED) || (UartHost_HD_ENABLED) )
    #if(UartHost_TX_INTERRUPT_ENABLED)
        #define UartHost_TX_VECT_NUM            (uint8)UartHost_TXInternalInterrupt__INTC_NUMBER
        #define UartHost_TX_PRIOR_NUM           (uint8)UartHost_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* UartHost_TX_INTERRUPT_ENABLED */

    #define UartHost_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define UartHost_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define UartHost_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(UartHost_TX_ENABLED)
        #define UartHost_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (UartHost_HD_ENABLED) */
        #define UartHost_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (UartHost_TX_ENABLED) */

    #define UartHost_TX_STS_COMPLETE            (uint8)(0x01u << UartHost_TX_STS_COMPLETE_SHIFT)
    #define UartHost_TX_STS_FIFO_EMPTY          (uint8)(0x01u << UartHost_TX_STS_FIFO_EMPTY_SHIFT)
    #define UartHost_TX_STS_FIFO_FULL           (uint8)(0x01u << UartHost_TX_STS_FIFO_FULL_SHIFT)
    #define UartHost_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << UartHost_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (UartHost_TX_ENABLED) || (UartHost_HD_ENABLED)*/

#if( (UartHost_RX_ENABLED) || (UartHost_HD_ENABLED) )
    #if(UartHost_RX_INTERRUPT_ENABLED)
        #define UartHost_RX_VECT_NUM            (uint8)UartHost_RXInternalInterrupt__INTC_NUMBER
        #define UartHost_RX_PRIOR_NUM           (uint8)UartHost_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* UartHost_RX_INTERRUPT_ENABLED */
    #define UartHost_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define UartHost_RX_STS_BREAK_SHIFT             (0x01u)
    #define UartHost_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define UartHost_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define UartHost_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define UartHost_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define UartHost_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define UartHost_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define UartHost_RX_STS_MRKSPC           (uint8)(0x01u << UartHost_RX_STS_MRKSPC_SHIFT)
    #define UartHost_RX_STS_BREAK            (uint8)(0x01u << UartHost_RX_STS_BREAK_SHIFT)
    #define UartHost_RX_STS_PAR_ERROR        (uint8)(0x01u << UartHost_RX_STS_PAR_ERROR_SHIFT)
    #define UartHost_RX_STS_STOP_ERROR       (uint8)(0x01u << UartHost_RX_STS_STOP_ERROR_SHIFT)
    #define UartHost_RX_STS_OVERRUN          (uint8)(0x01u << UartHost_RX_STS_OVERRUN_SHIFT)
    #define UartHost_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << UartHost_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define UartHost_RX_STS_ADDR_MATCH       (uint8)(0x01u << UartHost_RX_STS_ADDR_MATCH_SHIFT)
    #define UartHost_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << UartHost_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define UartHost_RX_HW_MASK                     (0x7Fu)
#endif /* End (UartHost_RX_ENABLED) || (UartHost_HD_ENABLED) */

/* Control Register definitions */
#define UartHost_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define UartHost_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define UartHost_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define UartHost_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define UartHost_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define UartHost_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define UartHost_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define UartHost_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define UartHost_CTRL_HD_SEND               (uint8)(0x01u << UartHost_CTRL_HD_SEND_SHIFT)
#define UartHost_CTRL_HD_SEND_BREAK         (uint8)(0x01u << UartHost_CTRL_HD_SEND_BREAK_SHIFT)
#define UartHost_CTRL_MARK                  (uint8)(0x01u << UartHost_CTRL_MARK_SHIFT)
#define UartHost_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << UartHost_CTRL_PARITY_TYPE0_SHIFT)
#define UartHost_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << UartHost_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define UartHost_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define UartHost_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define UartHost_SEND_BREAK                         (0x00u)
#define UartHost_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define UartHost_REINIT                             (0x02u)
#define UartHost_SEND_WAIT_REINIT                   (0x03u)

#define UartHost_OVER_SAMPLE_8                      (8u)
#define UartHost_OVER_SAMPLE_16                     (16u)

#define UartHost_BIT_CENTER                         (UartHost_OVER_SAMPLE_COUNT - 2u)

#define UartHost_FIFO_LENGTH                        (4u)
#define UartHost_NUMBER_OF_START_BIT                (1u)
#define UartHost_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define UartHost_TXBITCTR_BREAKBITS8X   ((UartHost_BREAK_BITS_TX * UartHost_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define UartHost_TXBITCTR_BREAKBITS ((UartHost_BREAK_BITS_TX * UartHost_OVER_SAMPLE_COUNT) - 1u)

#define UartHost_HALF_BIT_COUNT   \
                            (((UartHost_OVER_SAMPLE_COUNT / 2u) + (UartHost_USE23POLLING * 1u)) - 2u)
#if (UartHost_OVER_SAMPLE_COUNT == UartHost_OVER_SAMPLE_8)
    #define UartHost_HD_TXBITCTR_INIT   (((UartHost_BREAK_BITS_TX + \
                            UartHost_NUMBER_OF_START_BIT) * UartHost_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define UartHost_RXBITCTR_INIT  ((((UartHost_BREAK_BITS_RX + UartHost_NUMBER_OF_START_BIT) \
                            * UartHost_OVER_SAMPLE_COUNT) + UartHost_HALF_BIT_COUNT) - 1u)

#else /* UartHost_OVER_SAMPLE_COUNT == UartHost_OVER_SAMPLE_16 */
    #define UartHost_HD_TXBITCTR_INIT   ((8u * UartHost_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define UartHost_RXBITCTR_INIT      (((7u * UartHost_OVER_SAMPLE_COUNT) - 1u) + \
                                                      UartHost_HALF_BIT_COUNT)
#endif /* End UartHost_OVER_SAMPLE_COUNT */

#define UartHost_HD_RXBITCTR_INIT                   UartHost_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 UartHost_initVar;
#if (UartHost_TX_INTERRUPT_ENABLED && UartHost_TX_ENABLED)
    extern volatile uint8 UartHost_txBuffer[UartHost_TX_BUFFER_SIZE];
    extern volatile uint8 UartHost_txBufferRead;
    extern uint8 UartHost_txBufferWrite;
#endif /* (UartHost_TX_INTERRUPT_ENABLED && UartHost_TX_ENABLED) */
#if (UartHost_RX_INTERRUPT_ENABLED && (UartHost_RX_ENABLED || UartHost_HD_ENABLED))
    extern uint8 UartHost_errorStatus;
    extern volatile uint8 UartHost_rxBuffer[UartHost_RX_BUFFER_SIZE];
    extern volatile uint8 UartHost_rxBufferRead;
    extern volatile uint8 UartHost_rxBufferWrite;
    extern volatile uint8 UartHost_rxBufferLoopDetect;
    extern volatile uint8 UartHost_rxBufferOverflow;
    #if (UartHost_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 UartHost_rxAddressMode;
        extern volatile uint8 UartHost_rxAddressDetected;
    #endif /* (UartHost_RXHW_ADDRESS_ENABLED) */
#endif /* (UartHost_RX_INTERRUPT_ENABLED && (UartHost_RX_ENABLED || UartHost_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define UartHost__B_UART__AM_SW_BYTE_BYTE 1
#define UartHost__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define UartHost__B_UART__AM_HW_BYTE_BY_BYTE 3
#define UartHost__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define UartHost__B_UART__AM_NONE 0

#define UartHost__B_UART__NONE_REVB 0
#define UartHost__B_UART__EVEN_REVB 1
#define UartHost__B_UART__ODD_REVB 2
#define UartHost__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define UartHost_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define UartHost_NUMBER_OF_STOP_BITS    (1u)

#if (UartHost_RXHW_ADDRESS_ENABLED)
    #define UartHost_RX_ADDRESS_MODE    (0u)
    #define UartHost_RX_HW_ADDRESS1     (0u)
    #define UartHost_RX_HW_ADDRESS2     (0u)
#endif /* (UartHost_RXHW_ADDRESS_ENABLED) */

#define UartHost_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << UartHost_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << UartHost_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << UartHost_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << UartHost_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << UartHost_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << UartHost_RX_STS_BREAK_SHIFT) \
                                        | (0 << UartHost_RX_STS_OVERRUN_SHIFT))

#define UartHost_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << UartHost_TX_STS_COMPLETE_SHIFT) \
                                        | (1 << UartHost_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << UartHost_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << UartHost_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef UartHost_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define UartHost_CONTROL_REG \
                            (* (reg8 *) UartHost_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define UartHost_CONTROL_PTR \
                            (  (reg8 *) UartHost_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End UartHost_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(UartHost_TX_ENABLED)
    #define UartHost_TXDATA_REG          (* (reg8 *) UartHost_BUART_sTX_TxShifter_u0__F0_REG)
    #define UartHost_TXDATA_PTR          (  (reg8 *) UartHost_BUART_sTX_TxShifter_u0__F0_REG)
    #define UartHost_TXDATA_AUX_CTL_REG  (* (reg8 *) UartHost_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define UartHost_TXDATA_AUX_CTL_PTR  (  (reg8 *) UartHost_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define UartHost_TXSTATUS_REG        (* (reg8 *) UartHost_BUART_sTX_TxSts__STATUS_REG)
    #define UartHost_TXSTATUS_PTR        (  (reg8 *) UartHost_BUART_sTX_TxSts__STATUS_REG)
    #define UartHost_TXSTATUS_MASK_REG   (* (reg8 *) UartHost_BUART_sTX_TxSts__MASK_REG)
    #define UartHost_TXSTATUS_MASK_PTR   (  (reg8 *) UartHost_BUART_sTX_TxSts__MASK_REG)
    #define UartHost_TXSTATUS_ACTL_REG   (* (reg8 *) UartHost_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define UartHost_TXSTATUS_ACTL_PTR   (  (reg8 *) UartHost_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(UartHost_TXCLKGEN_DP)
        #define UartHost_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) UartHost_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define UartHost_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) UartHost_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define UartHost_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) UartHost_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define UartHost_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) UartHost_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define UartHost_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) UartHost_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define UartHost_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) UartHost_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define UartHost_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) UartHost_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define UartHost_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) UartHost_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define UartHost_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) UartHost_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define UartHost_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) UartHost_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* UartHost_TXCLKGEN_DP */

#endif /* End UartHost_TX_ENABLED */

#if(UartHost_HD_ENABLED)

    #define UartHost_TXDATA_REG             (* (reg8 *) UartHost_BUART_sRX_RxShifter_u0__F1_REG )
    #define UartHost_TXDATA_PTR             (  (reg8 *) UartHost_BUART_sRX_RxShifter_u0__F1_REG )
    #define UartHost_TXDATA_AUX_CTL_REG     (* (reg8 *) UartHost_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define UartHost_TXDATA_AUX_CTL_PTR     (  (reg8 *) UartHost_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define UartHost_TXSTATUS_REG           (* (reg8 *) UartHost_BUART_sRX_RxSts__STATUS_REG )
    #define UartHost_TXSTATUS_PTR           (  (reg8 *) UartHost_BUART_sRX_RxSts__STATUS_REG )
    #define UartHost_TXSTATUS_MASK_REG      (* (reg8 *) UartHost_BUART_sRX_RxSts__MASK_REG )
    #define UartHost_TXSTATUS_MASK_PTR      (  (reg8 *) UartHost_BUART_sRX_RxSts__MASK_REG )
    #define UartHost_TXSTATUS_ACTL_REG      (* (reg8 *) UartHost_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define UartHost_TXSTATUS_ACTL_PTR      (  (reg8 *) UartHost_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End UartHost_HD_ENABLED */

#if( (UartHost_RX_ENABLED) || (UartHost_HD_ENABLED) )
    #define UartHost_RXDATA_REG             (* (reg8 *) UartHost_BUART_sRX_RxShifter_u0__F0_REG )
    #define UartHost_RXDATA_PTR             (  (reg8 *) UartHost_BUART_sRX_RxShifter_u0__F0_REG )
    #define UartHost_RXADDRESS1_REG         (* (reg8 *) UartHost_BUART_sRX_RxShifter_u0__D0_REG )
    #define UartHost_RXADDRESS1_PTR         (  (reg8 *) UartHost_BUART_sRX_RxShifter_u0__D0_REG )
    #define UartHost_RXADDRESS2_REG         (* (reg8 *) UartHost_BUART_sRX_RxShifter_u0__D1_REG )
    #define UartHost_RXADDRESS2_PTR         (  (reg8 *) UartHost_BUART_sRX_RxShifter_u0__D1_REG )
    #define UartHost_RXDATA_AUX_CTL_REG     (* (reg8 *) UartHost_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define UartHost_RXBITCTR_PERIOD_REG    (* (reg8 *) UartHost_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define UartHost_RXBITCTR_PERIOD_PTR    (  (reg8 *) UartHost_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define UartHost_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) UartHost_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define UartHost_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) UartHost_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define UartHost_RXBITCTR_COUNTER_REG   (* (reg8 *) UartHost_BUART_sRX_RxBitCounter__COUNT_REG )
    #define UartHost_RXBITCTR_COUNTER_PTR   (  (reg8 *) UartHost_BUART_sRX_RxBitCounter__COUNT_REG )

    #define UartHost_RXSTATUS_REG           (* (reg8 *) UartHost_BUART_sRX_RxSts__STATUS_REG )
    #define UartHost_RXSTATUS_PTR           (  (reg8 *) UartHost_BUART_sRX_RxSts__STATUS_REG )
    #define UartHost_RXSTATUS_MASK_REG      (* (reg8 *) UartHost_BUART_sRX_RxSts__MASK_REG )
    #define UartHost_RXSTATUS_MASK_PTR      (  (reg8 *) UartHost_BUART_sRX_RxSts__MASK_REG )
    #define UartHost_RXSTATUS_ACTL_REG      (* (reg8 *) UartHost_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define UartHost_RXSTATUS_ACTL_PTR      (  (reg8 *) UartHost_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (UartHost_RX_ENABLED) || (UartHost_HD_ENABLED) */

#if(UartHost_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define UartHost_INTCLOCK_CLKEN_REG     (* (reg8 *) UartHost_IntClock__PM_ACT_CFG)
    #define UartHost_INTCLOCK_CLKEN_PTR     (  (reg8 *) UartHost_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define UartHost_INTCLOCK_CLKEN_MASK    UartHost_IntClock__PM_ACT_MSK
#endif /* End UartHost_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(UartHost_TX_ENABLED)
    #define UartHost_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End UartHost_TX_ENABLED */

#if(UartHost_HD_ENABLED)
    #define UartHost_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End UartHost_HD_ENABLED */

#if( (UartHost_RX_ENABLED) || (UartHost_HD_ENABLED) )
    #define UartHost_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (UartHost_RX_ENABLED) || (UartHost_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define UartHost_WAIT_1_MS      UartHost_BL_CHK_DELAY_MS   

#define UartHost_TXBUFFERSIZE   UartHost_TX_BUFFER_SIZE
#define UartHost_RXBUFFERSIZE   UartHost_RX_BUFFER_SIZE

#if (UartHost_RXHW_ADDRESS_ENABLED)
    #define UartHost_RXADDRESSMODE  UartHost_RX_ADDRESS_MODE
    #define UartHost_RXHWADDRESS1   UartHost_RX_HW_ADDRESS1
    #define UartHost_RXHWADDRESS2   UartHost_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define UartHost_RXAddressMode  UartHost_RXADDRESSMODE
#endif /* (UartHost_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define UartHost_initvar                    UartHost_initVar

#define UartHost_RX_Enabled                 UartHost_RX_ENABLED
#define UartHost_TX_Enabled                 UartHost_TX_ENABLED
#define UartHost_HD_Enabled                 UartHost_HD_ENABLED
#define UartHost_RX_IntInterruptEnabled     UartHost_RX_INTERRUPT_ENABLED
#define UartHost_TX_IntInterruptEnabled     UartHost_TX_INTERRUPT_ENABLED
#define UartHost_InternalClockUsed          UartHost_INTERNAL_CLOCK_USED
#define UartHost_RXHW_Address_Enabled       UartHost_RXHW_ADDRESS_ENABLED
#define UartHost_OverSampleCount            UartHost_OVER_SAMPLE_COUNT
#define UartHost_ParityType                 UartHost_PARITY_TYPE

#if( UartHost_TX_ENABLED && (UartHost_TXBUFFERSIZE > UartHost_FIFO_LENGTH))
    #define UartHost_TXBUFFER               UartHost_txBuffer
    #define UartHost_TXBUFFERREAD           UartHost_txBufferRead
    #define UartHost_TXBUFFERWRITE          UartHost_txBufferWrite
#endif /* End UartHost_TX_ENABLED */
#if( ( UartHost_RX_ENABLED || UartHost_HD_ENABLED ) && \
     (UartHost_RXBUFFERSIZE > UartHost_FIFO_LENGTH) )
    #define UartHost_RXBUFFER               UartHost_rxBuffer
    #define UartHost_RXBUFFERREAD           UartHost_rxBufferRead
    #define UartHost_RXBUFFERWRITE          UartHost_rxBufferWrite
    #define UartHost_RXBUFFERLOOPDETECT     UartHost_rxBufferLoopDetect
    #define UartHost_RXBUFFER_OVERFLOW      UartHost_rxBufferOverflow
#endif /* End UartHost_RX_ENABLED */

#ifdef UartHost_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define UartHost_CONTROL                UartHost_CONTROL_REG
#endif /* End UartHost_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(UartHost_TX_ENABLED)
    #define UartHost_TXDATA                 UartHost_TXDATA_REG
    #define UartHost_TXSTATUS               UartHost_TXSTATUS_REG
    #define UartHost_TXSTATUS_MASK          UartHost_TXSTATUS_MASK_REG
    #define UartHost_TXSTATUS_ACTL          UartHost_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(UartHost_TXCLKGEN_DP)
        #define UartHost_TXBITCLKGEN_CTR        UartHost_TXBITCLKGEN_CTR_REG
        #define UartHost_TXBITCLKTX_COMPLETE    UartHost_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define UartHost_TXBITCTR_PERIOD        UartHost_TXBITCTR_PERIOD_REG
        #define UartHost_TXBITCTR_CONTROL       UartHost_TXBITCTR_CONTROL_REG
        #define UartHost_TXBITCTR_COUNTER       UartHost_TXBITCTR_COUNTER_REG
    #endif /* UartHost_TXCLKGEN_DP */
#endif /* End UartHost_TX_ENABLED */

#if(UartHost_HD_ENABLED)
    #define UartHost_TXDATA                 UartHost_TXDATA_REG
    #define UartHost_TXSTATUS               UartHost_TXSTATUS_REG
    #define UartHost_TXSTATUS_MASK          UartHost_TXSTATUS_MASK_REG
    #define UartHost_TXSTATUS_ACTL          UartHost_TXSTATUS_ACTL_REG
#endif /* End UartHost_HD_ENABLED */

#if( (UartHost_RX_ENABLED) || (UartHost_HD_ENABLED) )
    #define UartHost_RXDATA                 UartHost_RXDATA_REG
    #define UartHost_RXADDRESS1             UartHost_RXADDRESS1_REG
    #define UartHost_RXADDRESS2             UartHost_RXADDRESS2_REG
    #define UartHost_RXBITCTR_PERIOD        UartHost_RXBITCTR_PERIOD_REG
    #define UartHost_RXBITCTR_CONTROL       UartHost_RXBITCTR_CONTROL_REG
    #define UartHost_RXBITCTR_COUNTER       UartHost_RXBITCTR_COUNTER_REG
    #define UartHost_RXSTATUS               UartHost_RXSTATUS_REG
    #define UartHost_RXSTATUS_MASK          UartHost_RXSTATUS_MASK_REG
    #define UartHost_RXSTATUS_ACTL          UartHost_RXSTATUS_ACTL_REG
#endif /* End  (UartHost_RX_ENABLED) || (UartHost_HD_ENABLED) */

#if(UartHost_INTERNAL_CLOCK_USED)
    #define UartHost_INTCLOCK_CLKEN         UartHost_INTCLOCK_CLKEN_REG
#endif /* End UartHost_INTERNAL_CLOCK_USED */

#define UartHost_WAIT_FOR_COMLETE_REINIT    UartHost_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_UartHost_H */


/* [] END OF FILE */

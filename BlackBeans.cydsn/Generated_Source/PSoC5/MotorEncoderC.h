/*******************************************************************************
* File Name: MotorEncoderC.h  
* Version 3.0
*
* Description:
*  This file provides constants and parameter values for the Quadrature
*  Decoder component.
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

#if !defined(CY_QUADRATURE_DECODER_MotorEncoderC_H)
#define CY_QUADRATURE_DECODER_MotorEncoderC_H

#include "cyfitter.h"
#include "CyLib.h"
#include "cytypes.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component QuadDec_v3_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#define MotorEncoderC_COUNTER_SIZE               (32u)
#define MotorEncoderC_COUNTER_SIZE_8_BIT         (8u)
#define MotorEncoderC_COUNTER_SIZE_16_BIT        (16u)
#define MotorEncoderC_COUNTER_SIZE_32_BIT        (32u)

#if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT)
    #include "MotorEncoderC_Cnt8.h"
#else 
    /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_16_BIT) || 
    *  (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT) 
    */
    #include "MotorEncoderC_Cnt16.h"
#endif /* MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT */

extern uint8 MotorEncoderC_initVar;


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define MotorEncoderC_COUNTER_RESOLUTION         (4u)


/***************************************
*       Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
} MotorEncoderC_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  MotorEncoderC_Init(void) ;
void  MotorEncoderC_Start(void) ;
void  MotorEncoderC_Stop(void) ;
void  MotorEncoderC_Enable(void) ;
uint8 MotorEncoderC_GetEvents(void) ;
void  MotorEncoderC_SetInterruptMask(uint8 mask) ;
uint8 MotorEncoderC_GetInterruptMask(void) ;
int32 MotorEncoderC_GetCounter(void) ;
void  MotorEncoderC_SetCounter(int32 value)
;
void  MotorEncoderC_Sleep(void) ;
void  MotorEncoderC_Wakeup(void) ;
void  MotorEncoderC_SaveConfig(void) ;
void  MotorEncoderC_RestoreConfig(void) ;

#if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT)
    CY_ISR_PROTO(MotorEncoderC_ISR);
#endif /* MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT */


/***************************************
*           API Constants
***************************************/

#if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT)
    #define MotorEncoderC_ISR_NUMBER             ((uint8) MotorEncoderC_isr__INTC_NUMBER)
    #define MotorEncoderC_ISR_PRIORITY           ((uint8) MotorEncoderC_isr__INTC_PRIOR_NUM)
#endif /* MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT */


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define MotorEncoderC_GLITCH_FILTERING           (0u)
#define MotorEncoderC_INDEX_INPUT                (0u)


/***************************************
*    Initial Parameter Constants
***************************************/

#if (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT)
    #define MotorEncoderC_COUNTER_INIT_VALUE    (0x80u)
#else 
    /* (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_16_BIT) ||
    *  (MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_32_BIT)
    */
    #define MotorEncoderC_COUNTER_INIT_VALUE    (0x8000u)
    #define MotorEncoderC_COUNTER_MAX_VALUE     (0x7FFFu)
#endif /* MotorEncoderC_COUNTER_SIZE == MotorEncoderC_COUNTER_SIZE_8_BIT */


/***************************************
*             Registers
***************************************/

#define MotorEncoderC_STATUS_REG                 (* (reg8 *) MotorEncoderC_bQuadDec_Stsreg__STATUS_REG)
#define MotorEncoderC_STATUS_PTR                 (  (reg8 *) MotorEncoderC_bQuadDec_Stsreg__STATUS_REG)
#define MotorEncoderC_STATUS_MASK                (* (reg8 *) MotorEncoderC_bQuadDec_Stsreg__MASK_REG)
#define MotorEncoderC_STATUS_MASK_PTR            (  (reg8 *) MotorEncoderC_bQuadDec_Stsreg__MASK_REG)
#define MotorEncoderC_SR_AUX_CONTROL             (* (reg8 *) MotorEncoderC_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)
#define MotorEncoderC_SR_AUX_CONTROL_PTR         (  (reg8 *) MotorEncoderC_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)


/***************************************
*        Register Constants
***************************************/

#define MotorEncoderC_COUNTER_OVERFLOW_SHIFT     (0x00u)
#define MotorEncoderC_COUNTER_UNDERFLOW_SHIFT    (0x01u)
#define MotorEncoderC_COUNTER_RESET_SHIFT        (0x02u)
#define MotorEncoderC_INVALID_IN_SHIFT           (0x03u)
#define MotorEncoderC_COUNTER_OVERFLOW           ((uint8) (0x01u << MotorEncoderC_COUNTER_OVERFLOW_SHIFT))
#define MotorEncoderC_COUNTER_UNDERFLOW          ((uint8) (0x01u << MotorEncoderC_COUNTER_UNDERFLOW_SHIFT))
#define MotorEncoderC_COUNTER_RESET              ((uint8) (0x01u << MotorEncoderC_COUNTER_RESET_SHIFT))
#define MotorEncoderC_INVALID_IN                 ((uint8) (0x01u << MotorEncoderC_INVALID_IN_SHIFT))

#define MotorEncoderC_INTERRUPTS_ENABLE_SHIFT    (0x04u)
#define MotorEncoderC_INTERRUPTS_ENABLE          ((uint8)(0x01u << MotorEncoderC_INTERRUPTS_ENABLE_SHIFT))
#define MotorEncoderC_INIT_INT_MASK              (0x0Fu)


/******************************************************************************************
* Following code are OBSOLETE and must not be used starting from Quadrature Decoder 2.20
******************************************************************************************/
#define MotorEncoderC_DISABLE                    (0x00u)


#endif /* CY_QUADRATURE_DECODER_MotorEncoderC_H */


/* [] END OF FILE */

/*******************************************************************************
* File Name: MotorEncoderA.h  
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

#if !defined(CY_QUADRATURE_DECODER_MotorEncoderA_H)
#define CY_QUADRATURE_DECODER_MotorEncoderA_H

#include "cyfitter.h"
#include "CyLib.h"
#include "cytypes.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component QuadDec_v3_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#define MotorEncoderA_COUNTER_SIZE               (32u)
#define MotorEncoderA_COUNTER_SIZE_8_BIT         (8u)
#define MotorEncoderA_COUNTER_SIZE_16_BIT        (16u)
#define MotorEncoderA_COUNTER_SIZE_32_BIT        (32u)

#if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT)
    #include "MotorEncoderA_Cnt8.h"
#else 
    /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_16_BIT) || 
    *  (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT) 
    */
    #include "MotorEncoderA_Cnt16.h"
#endif /* MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT */

extern uint8 MotorEncoderA_initVar;


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define MotorEncoderA_COUNTER_RESOLUTION         (4u)


/***************************************
*       Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
} MotorEncoderA_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  MotorEncoderA_Init(void) ;
void  MotorEncoderA_Start(void) ;
void  MotorEncoderA_Stop(void) ;
void  MotorEncoderA_Enable(void) ;
uint8 MotorEncoderA_GetEvents(void) ;
void  MotorEncoderA_SetInterruptMask(uint8 mask) ;
uint8 MotorEncoderA_GetInterruptMask(void) ;
int32 MotorEncoderA_GetCounter(void) ;
void  MotorEncoderA_SetCounter(int32 value)
;
void  MotorEncoderA_Sleep(void) ;
void  MotorEncoderA_Wakeup(void) ;
void  MotorEncoderA_SaveConfig(void) ;
void  MotorEncoderA_RestoreConfig(void) ;

#if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT)
    CY_ISR_PROTO(MotorEncoderA_ISR);
#endif /* MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT */


/***************************************
*           API Constants
***************************************/

#if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT)
    #define MotorEncoderA_ISR_NUMBER             ((uint8) MotorEncoderA_isr__INTC_NUMBER)
    #define MotorEncoderA_ISR_PRIORITY           ((uint8) MotorEncoderA_isr__INTC_PRIOR_NUM)
#endif /* MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT */


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define MotorEncoderA_GLITCH_FILTERING           (0u)
#define MotorEncoderA_INDEX_INPUT                (0u)


/***************************************
*    Initial Parameter Constants
***************************************/

#if (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT)
    #define MotorEncoderA_COUNTER_INIT_VALUE    (0x80u)
#else 
    /* (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_16_BIT) ||
    *  (MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_32_BIT)
    */
    #define MotorEncoderA_COUNTER_INIT_VALUE    (0x8000u)
    #define MotorEncoderA_COUNTER_MAX_VALUE     (0x7FFFu)
#endif /* MotorEncoderA_COUNTER_SIZE == MotorEncoderA_COUNTER_SIZE_8_BIT */


/***************************************
*             Registers
***************************************/

#define MotorEncoderA_STATUS_REG                 (* (reg8 *) MotorEncoderA_bQuadDec_Stsreg__STATUS_REG)
#define MotorEncoderA_STATUS_PTR                 (  (reg8 *) MotorEncoderA_bQuadDec_Stsreg__STATUS_REG)
#define MotorEncoderA_STATUS_MASK                (* (reg8 *) MotorEncoderA_bQuadDec_Stsreg__MASK_REG)
#define MotorEncoderA_STATUS_MASK_PTR            (  (reg8 *) MotorEncoderA_bQuadDec_Stsreg__MASK_REG)
#define MotorEncoderA_SR_AUX_CONTROL             (* (reg8 *) MotorEncoderA_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)
#define MotorEncoderA_SR_AUX_CONTROL_PTR         (  (reg8 *) MotorEncoderA_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)


/***************************************
*        Register Constants
***************************************/

#define MotorEncoderA_COUNTER_OVERFLOW_SHIFT     (0x00u)
#define MotorEncoderA_COUNTER_UNDERFLOW_SHIFT    (0x01u)
#define MotorEncoderA_COUNTER_RESET_SHIFT        (0x02u)
#define MotorEncoderA_INVALID_IN_SHIFT           (0x03u)
#define MotorEncoderA_COUNTER_OVERFLOW           ((uint8) (0x01u << MotorEncoderA_COUNTER_OVERFLOW_SHIFT))
#define MotorEncoderA_COUNTER_UNDERFLOW          ((uint8) (0x01u << MotorEncoderA_COUNTER_UNDERFLOW_SHIFT))
#define MotorEncoderA_COUNTER_RESET              ((uint8) (0x01u << MotorEncoderA_COUNTER_RESET_SHIFT))
#define MotorEncoderA_INVALID_IN                 ((uint8) (0x01u << MotorEncoderA_INVALID_IN_SHIFT))

#define MotorEncoderA_INTERRUPTS_ENABLE_SHIFT    (0x04u)
#define MotorEncoderA_INTERRUPTS_ENABLE          ((uint8)(0x01u << MotorEncoderA_INTERRUPTS_ENABLE_SHIFT))
#define MotorEncoderA_INIT_INT_MASK              (0x0Fu)


/******************************************************************************************
* Following code are OBSOLETE and must not be used starting from Quadrature Decoder 2.20
******************************************************************************************/
#define MotorEncoderA_DISABLE                    (0x00u)


#endif /* CY_QUADRATURE_DECODER_MotorEncoderA_H */


/* [] END OF FILE */

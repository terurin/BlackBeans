/*******************************************************************************
* File Name: MotorEncoderB.h  
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

#if !defined(CY_QUADRATURE_DECODER_MotorEncoderB_H)
#define CY_QUADRATURE_DECODER_MotorEncoderB_H

#include "cyfitter.h"
#include "CyLib.h"
#include "cytypes.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component QuadDec_v3_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#define MotorEncoderB_COUNTER_SIZE               (32u)
#define MotorEncoderB_COUNTER_SIZE_8_BIT         (8u)
#define MotorEncoderB_COUNTER_SIZE_16_BIT        (16u)
#define MotorEncoderB_COUNTER_SIZE_32_BIT        (32u)

#if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT)
    #include "MotorEncoderB_Cnt8.h"
#else 
    /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_16_BIT) || 
    *  (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT) 
    */
    #include "MotorEncoderB_Cnt16.h"
#endif /* MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT */

extern uint8 MotorEncoderB_initVar;


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define MotorEncoderB_COUNTER_RESOLUTION         (4u)


/***************************************
*       Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
} MotorEncoderB_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  MotorEncoderB_Init(void) ;
void  MotorEncoderB_Start(void) ;
void  MotorEncoderB_Stop(void) ;
void  MotorEncoderB_Enable(void) ;
uint8 MotorEncoderB_GetEvents(void) ;
void  MotorEncoderB_SetInterruptMask(uint8 mask) ;
uint8 MotorEncoderB_GetInterruptMask(void) ;
int32 MotorEncoderB_GetCounter(void) ;
void  MotorEncoderB_SetCounter(int32 value)
;
void  MotorEncoderB_Sleep(void) ;
void  MotorEncoderB_Wakeup(void) ;
void  MotorEncoderB_SaveConfig(void) ;
void  MotorEncoderB_RestoreConfig(void) ;

#if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT)
    CY_ISR_PROTO(MotorEncoderB_ISR);
#endif /* MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT */


/***************************************
*           API Constants
***************************************/

#if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT)
    #define MotorEncoderB_ISR_NUMBER             ((uint8) MotorEncoderB_isr__INTC_NUMBER)
    #define MotorEncoderB_ISR_PRIORITY           ((uint8) MotorEncoderB_isr__INTC_PRIOR_NUM)
#endif /* MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT */


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define MotorEncoderB_GLITCH_FILTERING           (0u)
#define MotorEncoderB_INDEX_INPUT                (0u)


/***************************************
*    Initial Parameter Constants
***************************************/

#if (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT)
    #define MotorEncoderB_COUNTER_INIT_VALUE    (0x80u)
#else 
    /* (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_16_BIT) ||
    *  (MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_32_BIT)
    */
    #define MotorEncoderB_COUNTER_INIT_VALUE    (0x8000u)
    #define MotorEncoderB_COUNTER_MAX_VALUE     (0x7FFFu)
#endif /* MotorEncoderB_COUNTER_SIZE == MotorEncoderB_COUNTER_SIZE_8_BIT */


/***************************************
*             Registers
***************************************/

#define MotorEncoderB_STATUS_REG                 (* (reg8 *) MotorEncoderB_bQuadDec_Stsreg__STATUS_REG)
#define MotorEncoderB_STATUS_PTR                 (  (reg8 *) MotorEncoderB_bQuadDec_Stsreg__STATUS_REG)
#define MotorEncoderB_STATUS_MASK                (* (reg8 *) MotorEncoderB_bQuadDec_Stsreg__MASK_REG)
#define MotorEncoderB_STATUS_MASK_PTR            (  (reg8 *) MotorEncoderB_bQuadDec_Stsreg__MASK_REG)
#define MotorEncoderB_SR_AUX_CONTROL             (* (reg8 *) MotorEncoderB_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)
#define MotorEncoderB_SR_AUX_CONTROL_PTR         (  (reg8 *) MotorEncoderB_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)


/***************************************
*        Register Constants
***************************************/

#define MotorEncoderB_COUNTER_OVERFLOW_SHIFT     (0x00u)
#define MotorEncoderB_COUNTER_UNDERFLOW_SHIFT    (0x01u)
#define MotorEncoderB_COUNTER_RESET_SHIFT        (0x02u)
#define MotorEncoderB_INVALID_IN_SHIFT           (0x03u)
#define MotorEncoderB_COUNTER_OVERFLOW           ((uint8) (0x01u << MotorEncoderB_COUNTER_OVERFLOW_SHIFT))
#define MotorEncoderB_COUNTER_UNDERFLOW          ((uint8) (0x01u << MotorEncoderB_COUNTER_UNDERFLOW_SHIFT))
#define MotorEncoderB_COUNTER_RESET              ((uint8) (0x01u << MotorEncoderB_COUNTER_RESET_SHIFT))
#define MotorEncoderB_INVALID_IN                 ((uint8) (0x01u << MotorEncoderB_INVALID_IN_SHIFT))

#define MotorEncoderB_INTERRUPTS_ENABLE_SHIFT    (0x04u)
#define MotorEncoderB_INTERRUPTS_ENABLE          ((uint8)(0x01u << MotorEncoderB_INTERRUPTS_ENABLE_SHIFT))
#define MotorEncoderB_INIT_INT_MASK              (0x0Fu)


/******************************************************************************************
* Following code are OBSOLETE and must not be used starting from Quadrature Decoder 2.20
******************************************************************************************/
#define MotorEncoderB_DISABLE                    (0x00u)


#endif /* CY_QUADRATURE_DECODER_MotorEncoderB_H */


/* [] END OF FILE */

/*******************************************************************************
* File Name: BLDCMotor_1_QuadEncoder.h  
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

#if !defined(CY_QUADRATURE_DECODER_BLDCMotor_1_QuadEncoder_H)
#define CY_QUADRATURE_DECODER_BLDCMotor_1_QuadEncoder_H

#include "cyfitter.h"
#include "CyLib.h"
#include "cytypes.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component QuadDec_v3_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#define BLDCMotor_1_QuadEncoder_COUNTER_SIZE               (8u)
#define BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT         (8u)
#define BLDCMotor_1_QuadEncoder_COUNTER_SIZE_16_BIT        (16u)
#define BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT        (32u)

#if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT)
    #include "BLDCMotor_1_QuadEncoder_Cnt8.h"
#else 
    /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_16_BIT) || 
    *  (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT) 
    */
    #include "BLDCMotor_1_QuadEncoder_Cnt16.h"
#endif /* BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT */

extern uint8 BLDCMotor_1_QuadEncoder_initVar;


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define BLDCMotor_1_QuadEncoder_COUNTER_RESOLUTION         (4u)


/***************************************
*       Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
} BLDCMotor_1_QuadEncoder_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  BLDCMotor_1_QuadEncoder_Init(void) ;
void  BLDCMotor_1_QuadEncoder_Start(void) ;
void  BLDCMotor_1_QuadEncoder_Stop(void) ;
void  BLDCMotor_1_QuadEncoder_Enable(void) ;
uint8 BLDCMotor_1_QuadEncoder_GetEvents(void) ;
void  BLDCMotor_1_QuadEncoder_SetInterruptMask(uint8 mask) ;
uint8 BLDCMotor_1_QuadEncoder_GetInterruptMask(void) ;
int8 BLDCMotor_1_QuadEncoder_GetCounter(void) ;
void  BLDCMotor_1_QuadEncoder_SetCounter(int8 value)
;
void  BLDCMotor_1_QuadEncoder_Sleep(void) ;
void  BLDCMotor_1_QuadEncoder_Wakeup(void) ;
void  BLDCMotor_1_QuadEncoder_SaveConfig(void) ;
void  BLDCMotor_1_QuadEncoder_RestoreConfig(void) ;

#if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT)
    CY_ISR_PROTO(BLDCMotor_1_QuadEncoder_ISR);
#endif /* BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT */


/***************************************
*           API Constants
***************************************/

#if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT)
    #define BLDCMotor_1_QuadEncoder_ISR_NUMBER             ((uint8) BLDCMotor_1_QuadEncoder_isr__INTC_NUMBER)
    #define BLDCMotor_1_QuadEncoder_ISR_PRIORITY           ((uint8) BLDCMotor_1_QuadEncoder_isr__INTC_PRIOR_NUM)
#endif /* BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT */


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define BLDCMotor_1_QuadEncoder_GLITCH_FILTERING           (0u)
#define BLDCMotor_1_QuadEncoder_INDEX_INPUT                (0u)


/***************************************
*    Initial Parameter Constants
***************************************/

#if (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT)
    #define BLDCMotor_1_QuadEncoder_COUNTER_INIT_VALUE    (0x80u)
#else 
    /* (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_16_BIT) ||
    *  (BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_32_BIT)
    */
    #define BLDCMotor_1_QuadEncoder_COUNTER_INIT_VALUE    (0x8000u)
    #define BLDCMotor_1_QuadEncoder_COUNTER_MAX_VALUE     (0x7FFFu)
#endif /* BLDCMotor_1_QuadEncoder_COUNTER_SIZE == BLDCMotor_1_QuadEncoder_COUNTER_SIZE_8_BIT */


/***************************************
*             Registers
***************************************/

#define BLDCMotor_1_QuadEncoder_STATUS_REG                 (* (reg8 *) BLDCMotor_1_QuadEncoder_bQuadDec_Stsreg__STATUS_REG)
#define BLDCMotor_1_QuadEncoder_STATUS_PTR                 (  (reg8 *) BLDCMotor_1_QuadEncoder_bQuadDec_Stsreg__STATUS_REG)
#define BLDCMotor_1_QuadEncoder_STATUS_MASK                (* (reg8 *) BLDCMotor_1_QuadEncoder_bQuadDec_Stsreg__MASK_REG)
#define BLDCMotor_1_QuadEncoder_STATUS_MASK_PTR            (  (reg8 *) BLDCMotor_1_QuadEncoder_bQuadDec_Stsreg__MASK_REG)
#define BLDCMotor_1_QuadEncoder_SR_AUX_CONTROL             (* (reg8 *) BLDCMotor_1_QuadEncoder_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)
#define BLDCMotor_1_QuadEncoder_SR_AUX_CONTROL_PTR         (  (reg8 *) BLDCMotor_1_QuadEncoder_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)


/***************************************
*        Register Constants
***************************************/

#define BLDCMotor_1_QuadEncoder_COUNTER_OVERFLOW_SHIFT     (0x00u)
#define BLDCMotor_1_QuadEncoder_COUNTER_UNDERFLOW_SHIFT    (0x01u)
#define BLDCMotor_1_QuadEncoder_COUNTER_RESET_SHIFT        (0x02u)
#define BLDCMotor_1_QuadEncoder_INVALID_IN_SHIFT           (0x03u)
#define BLDCMotor_1_QuadEncoder_COUNTER_OVERFLOW           ((uint8) (0x01u << BLDCMotor_1_QuadEncoder_COUNTER_OVERFLOW_SHIFT))
#define BLDCMotor_1_QuadEncoder_COUNTER_UNDERFLOW          ((uint8) (0x01u << BLDCMotor_1_QuadEncoder_COUNTER_UNDERFLOW_SHIFT))
#define BLDCMotor_1_QuadEncoder_COUNTER_RESET              ((uint8) (0x01u << BLDCMotor_1_QuadEncoder_COUNTER_RESET_SHIFT))
#define BLDCMotor_1_QuadEncoder_INVALID_IN                 ((uint8) (0x01u << BLDCMotor_1_QuadEncoder_INVALID_IN_SHIFT))

#define BLDCMotor_1_QuadEncoder_INTERRUPTS_ENABLE_SHIFT    (0x04u)
#define BLDCMotor_1_QuadEncoder_INTERRUPTS_ENABLE          ((uint8)(0x01u << BLDCMotor_1_QuadEncoder_INTERRUPTS_ENABLE_SHIFT))
#define BLDCMotor_1_QuadEncoder_INIT_INT_MASK              (0x0Fu)


/******************************************************************************************
* Following code are OBSOLETE and must not be used starting from Quadrature Decoder 2.20
******************************************************************************************/
#define BLDCMotor_1_QuadEncoder_DISABLE                    (0x00u)


#endif /* CY_QUADRATURE_DECODER_BLDCMotor_1_QuadEncoder_H */


/* [] END OF FILE */

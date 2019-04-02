/*******************************************************************************
* File Name: EncoderClock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_EncoderClock_H)
#define CY_CLOCK_EncoderClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void EncoderClock_Start(void) ;
void EncoderClock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void EncoderClock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void EncoderClock_StandbyPower(uint8 state) ;
void EncoderClock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 EncoderClock_GetDividerRegister(void) ;
void EncoderClock_SetModeRegister(uint8 modeBitMask) ;
void EncoderClock_ClearModeRegister(uint8 modeBitMask) ;
uint8 EncoderClock_GetModeRegister(void) ;
void EncoderClock_SetSourceRegister(uint8 clkSource) ;
uint8 EncoderClock_GetSourceRegister(void) ;
#if defined(EncoderClock__CFG3)
void EncoderClock_SetPhaseRegister(uint8 clkPhase) ;
uint8 EncoderClock_GetPhaseRegister(void) ;
#endif /* defined(EncoderClock__CFG3) */

#define EncoderClock_Enable()                       EncoderClock_Start()
#define EncoderClock_Disable()                      EncoderClock_Stop()
#define EncoderClock_SetDivider(clkDivider)         EncoderClock_SetDividerRegister(clkDivider, 1u)
#define EncoderClock_SetDividerValue(clkDivider)    EncoderClock_SetDividerRegister((clkDivider) - 1u, 1u)
#define EncoderClock_SetMode(clkMode)               EncoderClock_SetModeRegister(clkMode)
#define EncoderClock_SetSource(clkSource)           EncoderClock_SetSourceRegister(clkSource)
#if defined(EncoderClock__CFG3)
#define EncoderClock_SetPhase(clkPhase)             EncoderClock_SetPhaseRegister(clkPhase)
#define EncoderClock_SetPhaseValue(clkPhase)        EncoderClock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(EncoderClock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define EncoderClock_CLKEN              (* (reg8 *) EncoderClock__PM_ACT_CFG)
#define EncoderClock_CLKEN_PTR          ((reg8 *) EncoderClock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define EncoderClock_CLKSTBY            (* (reg8 *) EncoderClock__PM_STBY_CFG)
#define EncoderClock_CLKSTBY_PTR        ((reg8 *) EncoderClock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define EncoderClock_DIV_LSB            (* (reg8 *) EncoderClock__CFG0)
#define EncoderClock_DIV_LSB_PTR        ((reg8 *) EncoderClock__CFG0)
#define EncoderClock_DIV_PTR            ((reg16 *) EncoderClock__CFG0)

/* Clock MSB divider configuration register. */
#define EncoderClock_DIV_MSB            (* (reg8 *) EncoderClock__CFG1)
#define EncoderClock_DIV_MSB_PTR        ((reg8 *) EncoderClock__CFG1)

/* Mode and source configuration register */
#define EncoderClock_MOD_SRC            (* (reg8 *) EncoderClock__CFG2)
#define EncoderClock_MOD_SRC_PTR        ((reg8 *) EncoderClock__CFG2)

#if defined(EncoderClock__CFG3)
/* Analog clock phase configuration register */
#define EncoderClock_PHASE              (* (reg8 *) EncoderClock__CFG3)
#define EncoderClock_PHASE_PTR          ((reg8 *) EncoderClock__CFG3)
#endif /* defined(EncoderClock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define EncoderClock_CLKEN_MASK         EncoderClock__PM_ACT_MSK
#define EncoderClock_CLKSTBY_MASK       EncoderClock__PM_STBY_MSK

/* CFG2 field masks */
#define EncoderClock_SRC_SEL_MSK        EncoderClock__CFG2_SRC_SEL_MASK
#define EncoderClock_MODE_MASK          (~(EncoderClock_SRC_SEL_MSK))

#if defined(EncoderClock__CFG3)
/* CFG3 phase mask */
#define EncoderClock_PHASE_MASK         EncoderClock__CFG3_PHASE_DLY_MASK
#endif /* defined(EncoderClock__CFG3) */

#endif /* CY_CLOCK_EncoderClock_H */


/* [] END OF FILE */

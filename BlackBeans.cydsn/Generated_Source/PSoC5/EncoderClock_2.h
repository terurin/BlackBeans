/*******************************************************************************
* File Name: EncoderClock_2.h
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

#if !defined(CY_CLOCK_EncoderClock_2_H)
#define CY_CLOCK_EncoderClock_2_H

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

void EncoderClock_2_Start(void) ;
void EncoderClock_2_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void EncoderClock_2_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void EncoderClock_2_StandbyPower(uint8 state) ;
void EncoderClock_2_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 EncoderClock_2_GetDividerRegister(void) ;
void EncoderClock_2_SetModeRegister(uint8 modeBitMask) ;
void EncoderClock_2_ClearModeRegister(uint8 modeBitMask) ;
uint8 EncoderClock_2_GetModeRegister(void) ;
void EncoderClock_2_SetSourceRegister(uint8 clkSource) ;
uint8 EncoderClock_2_GetSourceRegister(void) ;
#if defined(EncoderClock_2__CFG3)
void EncoderClock_2_SetPhaseRegister(uint8 clkPhase) ;
uint8 EncoderClock_2_GetPhaseRegister(void) ;
#endif /* defined(EncoderClock_2__CFG3) */

#define EncoderClock_2_Enable()                       EncoderClock_2_Start()
#define EncoderClock_2_Disable()                      EncoderClock_2_Stop()
#define EncoderClock_2_SetDivider(clkDivider)         EncoderClock_2_SetDividerRegister(clkDivider, 1u)
#define EncoderClock_2_SetDividerValue(clkDivider)    EncoderClock_2_SetDividerRegister((clkDivider) - 1u, 1u)
#define EncoderClock_2_SetMode(clkMode)               EncoderClock_2_SetModeRegister(clkMode)
#define EncoderClock_2_SetSource(clkSource)           EncoderClock_2_SetSourceRegister(clkSource)
#if defined(EncoderClock_2__CFG3)
#define EncoderClock_2_SetPhase(clkPhase)             EncoderClock_2_SetPhaseRegister(clkPhase)
#define EncoderClock_2_SetPhaseValue(clkPhase)        EncoderClock_2_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(EncoderClock_2__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define EncoderClock_2_CLKEN              (* (reg8 *) EncoderClock_2__PM_ACT_CFG)
#define EncoderClock_2_CLKEN_PTR          ((reg8 *) EncoderClock_2__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define EncoderClock_2_CLKSTBY            (* (reg8 *) EncoderClock_2__PM_STBY_CFG)
#define EncoderClock_2_CLKSTBY_PTR        ((reg8 *) EncoderClock_2__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define EncoderClock_2_DIV_LSB            (* (reg8 *) EncoderClock_2__CFG0)
#define EncoderClock_2_DIV_LSB_PTR        ((reg8 *) EncoderClock_2__CFG0)
#define EncoderClock_2_DIV_PTR            ((reg16 *) EncoderClock_2__CFG0)

/* Clock MSB divider configuration register. */
#define EncoderClock_2_DIV_MSB            (* (reg8 *) EncoderClock_2__CFG1)
#define EncoderClock_2_DIV_MSB_PTR        ((reg8 *) EncoderClock_2__CFG1)

/* Mode and source configuration register */
#define EncoderClock_2_MOD_SRC            (* (reg8 *) EncoderClock_2__CFG2)
#define EncoderClock_2_MOD_SRC_PTR        ((reg8 *) EncoderClock_2__CFG2)

#if defined(EncoderClock_2__CFG3)
/* Analog clock phase configuration register */
#define EncoderClock_2_PHASE              (* (reg8 *) EncoderClock_2__CFG3)
#define EncoderClock_2_PHASE_PTR          ((reg8 *) EncoderClock_2__CFG3)
#endif /* defined(EncoderClock_2__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define EncoderClock_2_CLKEN_MASK         EncoderClock_2__PM_ACT_MSK
#define EncoderClock_2_CLKSTBY_MASK       EncoderClock_2__PM_STBY_MSK

/* CFG2 field masks */
#define EncoderClock_2_SRC_SEL_MSK        EncoderClock_2__CFG2_SRC_SEL_MASK
#define EncoderClock_2_MODE_MASK          (~(EncoderClock_2_SRC_SEL_MSK))

#if defined(EncoderClock_2__CFG3)
/* CFG3 phase mask */
#define EncoderClock_2_PHASE_MASK         EncoderClock_2__CFG3_PHASE_DLY_MASK
#endif /* defined(EncoderClock_2__CFG3) */

#endif /* CY_CLOCK_EncoderClock_2_H */


/* [] END OF FILE */

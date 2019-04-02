/*******************************************************************************
* File Name: LowClock.h
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

#if !defined(CY_CLOCK_LowClock_H)
#define CY_CLOCK_LowClock_H

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

void LowClock_Start(void) ;
void LowClock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void LowClock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void LowClock_StandbyPower(uint8 state) ;
void LowClock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 LowClock_GetDividerRegister(void) ;
void LowClock_SetModeRegister(uint8 modeBitMask) ;
void LowClock_ClearModeRegister(uint8 modeBitMask) ;
uint8 LowClock_GetModeRegister(void) ;
void LowClock_SetSourceRegister(uint8 clkSource) ;
uint8 LowClock_GetSourceRegister(void) ;
#if defined(LowClock__CFG3)
void LowClock_SetPhaseRegister(uint8 clkPhase) ;
uint8 LowClock_GetPhaseRegister(void) ;
#endif /* defined(LowClock__CFG3) */

#define LowClock_Enable()                       LowClock_Start()
#define LowClock_Disable()                      LowClock_Stop()
#define LowClock_SetDivider(clkDivider)         LowClock_SetDividerRegister(clkDivider, 1u)
#define LowClock_SetDividerValue(clkDivider)    LowClock_SetDividerRegister((clkDivider) - 1u, 1u)
#define LowClock_SetMode(clkMode)               LowClock_SetModeRegister(clkMode)
#define LowClock_SetSource(clkSource)           LowClock_SetSourceRegister(clkSource)
#if defined(LowClock__CFG3)
#define LowClock_SetPhase(clkPhase)             LowClock_SetPhaseRegister(clkPhase)
#define LowClock_SetPhaseValue(clkPhase)        LowClock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(LowClock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define LowClock_CLKEN              (* (reg8 *) LowClock__PM_ACT_CFG)
#define LowClock_CLKEN_PTR          ((reg8 *) LowClock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define LowClock_CLKSTBY            (* (reg8 *) LowClock__PM_STBY_CFG)
#define LowClock_CLKSTBY_PTR        ((reg8 *) LowClock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define LowClock_DIV_LSB            (* (reg8 *) LowClock__CFG0)
#define LowClock_DIV_LSB_PTR        ((reg8 *) LowClock__CFG0)
#define LowClock_DIV_PTR            ((reg16 *) LowClock__CFG0)

/* Clock MSB divider configuration register. */
#define LowClock_DIV_MSB            (* (reg8 *) LowClock__CFG1)
#define LowClock_DIV_MSB_PTR        ((reg8 *) LowClock__CFG1)

/* Mode and source configuration register */
#define LowClock_MOD_SRC            (* (reg8 *) LowClock__CFG2)
#define LowClock_MOD_SRC_PTR        ((reg8 *) LowClock__CFG2)

#if defined(LowClock__CFG3)
/* Analog clock phase configuration register */
#define LowClock_PHASE              (* (reg8 *) LowClock__CFG3)
#define LowClock_PHASE_PTR          ((reg8 *) LowClock__CFG3)
#endif /* defined(LowClock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define LowClock_CLKEN_MASK         LowClock__PM_ACT_MSK
#define LowClock_CLKSTBY_MASK       LowClock__PM_STBY_MSK

/* CFG2 field masks */
#define LowClock_SRC_SEL_MSK        LowClock__CFG2_SRC_SEL_MASK
#define LowClock_MODE_MASK          (~(LowClock_SRC_SEL_MSK))

#if defined(LowClock__CFG3)
/* CFG3 phase mask */
#define LowClock_PHASE_MASK         LowClock__CFG3_PHASE_DLY_MASK
#endif /* defined(LowClock__CFG3) */

#endif /* CY_CLOCK_LowClock_H */


/* [] END OF FILE */

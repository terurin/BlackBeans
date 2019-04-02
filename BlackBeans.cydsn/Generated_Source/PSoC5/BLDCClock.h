/*******************************************************************************
* File Name: BLDCClock.h
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

#if !defined(CY_CLOCK_BLDCClock_H)
#define CY_CLOCK_BLDCClock_H

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

void BLDCClock_Start(void) ;
void BLDCClock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void BLDCClock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void BLDCClock_StandbyPower(uint8 state) ;
void BLDCClock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 BLDCClock_GetDividerRegister(void) ;
void BLDCClock_SetModeRegister(uint8 modeBitMask) ;
void BLDCClock_ClearModeRegister(uint8 modeBitMask) ;
uint8 BLDCClock_GetModeRegister(void) ;
void BLDCClock_SetSourceRegister(uint8 clkSource) ;
uint8 BLDCClock_GetSourceRegister(void) ;
#if defined(BLDCClock__CFG3)
void BLDCClock_SetPhaseRegister(uint8 clkPhase) ;
uint8 BLDCClock_GetPhaseRegister(void) ;
#endif /* defined(BLDCClock__CFG3) */

#define BLDCClock_Enable()                       BLDCClock_Start()
#define BLDCClock_Disable()                      BLDCClock_Stop()
#define BLDCClock_SetDivider(clkDivider)         BLDCClock_SetDividerRegister(clkDivider, 1u)
#define BLDCClock_SetDividerValue(clkDivider)    BLDCClock_SetDividerRegister((clkDivider) - 1u, 1u)
#define BLDCClock_SetMode(clkMode)               BLDCClock_SetModeRegister(clkMode)
#define BLDCClock_SetSource(clkSource)           BLDCClock_SetSourceRegister(clkSource)
#if defined(BLDCClock__CFG3)
#define BLDCClock_SetPhase(clkPhase)             BLDCClock_SetPhaseRegister(clkPhase)
#define BLDCClock_SetPhaseValue(clkPhase)        BLDCClock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(BLDCClock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define BLDCClock_CLKEN              (* (reg8 *) BLDCClock__PM_ACT_CFG)
#define BLDCClock_CLKEN_PTR          ((reg8 *) BLDCClock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define BLDCClock_CLKSTBY            (* (reg8 *) BLDCClock__PM_STBY_CFG)
#define BLDCClock_CLKSTBY_PTR        ((reg8 *) BLDCClock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define BLDCClock_DIV_LSB            (* (reg8 *) BLDCClock__CFG0)
#define BLDCClock_DIV_LSB_PTR        ((reg8 *) BLDCClock__CFG0)
#define BLDCClock_DIV_PTR            ((reg16 *) BLDCClock__CFG0)

/* Clock MSB divider configuration register. */
#define BLDCClock_DIV_MSB            (* (reg8 *) BLDCClock__CFG1)
#define BLDCClock_DIV_MSB_PTR        ((reg8 *) BLDCClock__CFG1)

/* Mode and source configuration register */
#define BLDCClock_MOD_SRC            (* (reg8 *) BLDCClock__CFG2)
#define BLDCClock_MOD_SRC_PTR        ((reg8 *) BLDCClock__CFG2)

#if defined(BLDCClock__CFG3)
/* Analog clock phase configuration register */
#define BLDCClock_PHASE              (* (reg8 *) BLDCClock__CFG3)
#define BLDCClock_PHASE_PTR          ((reg8 *) BLDCClock__CFG3)
#endif /* defined(BLDCClock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define BLDCClock_CLKEN_MASK         BLDCClock__PM_ACT_MSK
#define BLDCClock_CLKSTBY_MASK       BLDCClock__PM_STBY_MSK

/* CFG2 field masks */
#define BLDCClock_SRC_SEL_MSK        BLDCClock__CFG2_SRC_SEL_MASK
#define BLDCClock_MODE_MASK          (~(BLDCClock_SRC_SEL_MSK))

#if defined(BLDCClock__CFG3)
/* CFG3 phase mask */
#define BLDCClock_PHASE_MASK         BLDCClock__CFG3_PHASE_DLY_MASK
#endif /* defined(BLDCClock__CFG3) */

#endif /* CY_CLOCK_BLDCClock_H */


/* [] END OF FILE */

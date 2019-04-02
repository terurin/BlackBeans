/*******************************************************************************
* File Name: BLDCMotor_1_FastClock.h
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

#if !defined(CY_CLOCK_BLDCMotor_1_FastClock_H)
#define CY_CLOCK_BLDCMotor_1_FastClock_H

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

void BLDCMotor_1_FastClock_Start(void) ;
void BLDCMotor_1_FastClock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void BLDCMotor_1_FastClock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void BLDCMotor_1_FastClock_StandbyPower(uint8 state) ;
void BLDCMotor_1_FastClock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 BLDCMotor_1_FastClock_GetDividerRegister(void) ;
void BLDCMotor_1_FastClock_SetModeRegister(uint8 modeBitMask) ;
void BLDCMotor_1_FastClock_ClearModeRegister(uint8 modeBitMask) ;
uint8 BLDCMotor_1_FastClock_GetModeRegister(void) ;
void BLDCMotor_1_FastClock_SetSourceRegister(uint8 clkSource) ;
uint8 BLDCMotor_1_FastClock_GetSourceRegister(void) ;
#if defined(BLDCMotor_1_FastClock__CFG3)
void BLDCMotor_1_FastClock_SetPhaseRegister(uint8 clkPhase) ;
uint8 BLDCMotor_1_FastClock_GetPhaseRegister(void) ;
#endif /* defined(BLDCMotor_1_FastClock__CFG3) */

#define BLDCMotor_1_FastClock_Enable()                       BLDCMotor_1_FastClock_Start()
#define BLDCMotor_1_FastClock_Disable()                      BLDCMotor_1_FastClock_Stop()
#define BLDCMotor_1_FastClock_SetDivider(clkDivider)         BLDCMotor_1_FastClock_SetDividerRegister(clkDivider, 1u)
#define BLDCMotor_1_FastClock_SetDividerValue(clkDivider)    BLDCMotor_1_FastClock_SetDividerRegister((clkDivider) - 1u, 1u)
#define BLDCMotor_1_FastClock_SetMode(clkMode)               BLDCMotor_1_FastClock_SetModeRegister(clkMode)
#define BLDCMotor_1_FastClock_SetSource(clkSource)           BLDCMotor_1_FastClock_SetSourceRegister(clkSource)
#if defined(BLDCMotor_1_FastClock__CFG3)
#define BLDCMotor_1_FastClock_SetPhase(clkPhase)             BLDCMotor_1_FastClock_SetPhaseRegister(clkPhase)
#define BLDCMotor_1_FastClock_SetPhaseValue(clkPhase)        BLDCMotor_1_FastClock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(BLDCMotor_1_FastClock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define BLDCMotor_1_FastClock_CLKEN              (* (reg8 *) BLDCMotor_1_FastClock__PM_ACT_CFG)
#define BLDCMotor_1_FastClock_CLKEN_PTR          ((reg8 *) BLDCMotor_1_FastClock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define BLDCMotor_1_FastClock_CLKSTBY            (* (reg8 *) BLDCMotor_1_FastClock__PM_STBY_CFG)
#define BLDCMotor_1_FastClock_CLKSTBY_PTR        ((reg8 *) BLDCMotor_1_FastClock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define BLDCMotor_1_FastClock_DIV_LSB            (* (reg8 *) BLDCMotor_1_FastClock__CFG0)
#define BLDCMotor_1_FastClock_DIV_LSB_PTR        ((reg8 *) BLDCMotor_1_FastClock__CFG0)
#define BLDCMotor_1_FastClock_DIV_PTR            ((reg16 *) BLDCMotor_1_FastClock__CFG0)

/* Clock MSB divider configuration register. */
#define BLDCMotor_1_FastClock_DIV_MSB            (* (reg8 *) BLDCMotor_1_FastClock__CFG1)
#define BLDCMotor_1_FastClock_DIV_MSB_PTR        ((reg8 *) BLDCMotor_1_FastClock__CFG1)

/* Mode and source configuration register */
#define BLDCMotor_1_FastClock_MOD_SRC            (* (reg8 *) BLDCMotor_1_FastClock__CFG2)
#define BLDCMotor_1_FastClock_MOD_SRC_PTR        ((reg8 *) BLDCMotor_1_FastClock__CFG2)

#if defined(BLDCMotor_1_FastClock__CFG3)
/* Analog clock phase configuration register */
#define BLDCMotor_1_FastClock_PHASE              (* (reg8 *) BLDCMotor_1_FastClock__CFG3)
#define BLDCMotor_1_FastClock_PHASE_PTR          ((reg8 *) BLDCMotor_1_FastClock__CFG3)
#endif /* defined(BLDCMotor_1_FastClock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define BLDCMotor_1_FastClock_CLKEN_MASK         BLDCMotor_1_FastClock__PM_ACT_MSK
#define BLDCMotor_1_FastClock_CLKSTBY_MASK       BLDCMotor_1_FastClock__PM_STBY_MSK

/* CFG2 field masks */
#define BLDCMotor_1_FastClock_SRC_SEL_MSK        BLDCMotor_1_FastClock__CFG2_SRC_SEL_MASK
#define BLDCMotor_1_FastClock_MODE_MASK          (~(BLDCMotor_1_FastClock_SRC_SEL_MSK))

#if defined(BLDCMotor_1_FastClock__CFG3)
/* CFG3 phase mask */
#define BLDCMotor_1_FastClock_PHASE_MASK         BLDCMotor_1_FastClock__CFG3_PHASE_DLY_MASK
#endif /* defined(BLDCMotor_1_FastClock__CFG3) */

#endif /* CY_CLOCK_BLDCMotor_1_FastClock_H */


/* [] END OF FILE */

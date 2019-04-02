/*******************************************************************************
* File Name: UartHost_IntClock.h
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

#if !defined(CY_CLOCK_UartHost_IntClock_H)
#define CY_CLOCK_UartHost_IntClock_H

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

void UartHost_IntClock_Start(void) ;
void UartHost_IntClock_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void UartHost_IntClock_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void UartHost_IntClock_StandbyPower(uint8 state) ;
void UartHost_IntClock_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 UartHost_IntClock_GetDividerRegister(void) ;
void UartHost_IntClock_SetModeRegister(uint8 modeBitMask) ;
void UartHost_IntClock_ClearModeRegister(uint8 modeBitMask) ;
uint8 UartHost_IntClock_GetModeRegister(void) ;
void UartHost_IntClock_SetSourceRegister(uint8 clkSource) ;
uint8 UartHost_IntClock_GetSourceRegister(void) ;
#if defined(UartHost_IntClock__CFG3)
void UartHost_IntClock_SetPhaseRegister(uint8 clkPhase) ;
uint8 UartHost_IntClock_GetPhaseRegister(void) ;
#endif /* defined(UartHost_IntClock__CFG3) */

#define UartHost_IntClock_Enable()                       UartHost_IntClock_Start()
#define UartHost_IntClock_Disable()                      UartHost_IntClock_Stop()
#define UartHost_IntClock_SetDivider(clkDivider)         UartHost_IntClock_SetDividerRegister(clkDivider, 1u)
#define UartHost_IntClock_SetDividerValue(clkDivider)    UartHost_IntClock_SetDividerRegister((clkDivider) - 1u, 1u)
#define UartHost_IntClock_SetMode(clkMode)               UartHost_IntClock_SetModeRegister(clkMode)
#define UartHost_IntClock_SetSource(clkSource)           UartHost_IntClock_SetSourceRegister(clkSource)
#if defined(UartHost_IntClock__CFG3)
#define UartHost_IntClock_SetPhase(clkPhase)             UartHost_IntClock_SetPhaseRegister(clkPhase)
#define UartHost_IntClock_SetPhaseValue(clkPhase)        UartHost_IntClock_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(UartHost_IntClock__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define UartHost_IntClock_CLKEN              (* (reg8 *) UartHost_IntClock__PM_ACT_CFG)
#define UartHost_IntClock_CLKEN_PTR          ((reg8 *) UartHost_IntClock__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define UartHost_IntClock_CLKSTBY            (* (reg8 *) UartHost_IntClock__PM_STBY_CFG)
#define UartHost_IntClock_CLKSTBY_PTR        ((reg8 *) UartHost_IntClock__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define UartHost_IntClock_DIV_LSB            (* (reg8 *) UartHost_IntClock__CFG0)
#define UartHost_IntClock_DIV_LSB_PTR        ((reg8 *) UartHost_IntClock__CFG0)
#define UartHost_IntClock_DIV_PTR            ((reg16 *) UartHost_IntClock__CFG0)

/* Clock MSB divider configuration register. */
#define UartHost_IntClock_DIV_MSB            (* (reg8 *) UartHost_IntClock__CFG1)
#define UartHost_IntClock_DIV_MSB_PTR        ((reg8 *) UartHost_IntClock__CFG1)

/* Mode and source configuration register */
#define UartHost_IntClock_MOD_SRC            (* (reg8 *) UartHost_IntClock__CFG2)
#define UartHost_IntClock_MOD_SRC_PTR        ((reg8 *) UartHost_IntClock__CFG2)

#if defined(UartHost_IntClock__CFG3)
/* Analog clock phase configuration register */
#define UartHost_IntClock_PHASE              (* (reg8 *) UartHost_IntClock__CFG3)
#define UartHost_IntClock_PHASE_PTR          ((reg8 *) UartHost_IntClock__CFG3)
#endif /* defined(UartHost_IntClock__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define UartHost_IntClock_CLKEN_MASK         UartHost_IntClock__PM_ACT_MSK
#define UartHost_IntClock_CLKSTBY_MASK       UartHost_IntClock__PM_STBY_MSK

/* CFG2 field masks */
#define UartHost_IntClock_SRC_SEL_MSK        UartHost_IntClock__CFG2_SRC_SEL_MASK
#define UartHost_IntClock_MODE_MASK          (~(UartHost_IntClock_SRC_SEL_MSK))

#if defined(UartHost_IntClock__CFG3)
/* CFG3 phase mask */
#define UartHost_IntClock_PHASE_MASK         UartHost_IntClock__CFG3_PHASE_DLY_MASK
#endif /* defined(UartHost_IntClock__CFG3) */

#endif /* CY_CLOCK_UartHost_IntClock_H */


/* [] END OF FILE */

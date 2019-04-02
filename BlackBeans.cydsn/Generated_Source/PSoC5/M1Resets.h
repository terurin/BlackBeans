/*******************************************************************************
* File Name: M1Resets.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_M1Resets_H) /* Pins M1Resets_H */
#define CY_PINS_M1Resets_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "M1Resets_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 M1Resets__PORT == 15 && ((M1Resets__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    M1Resets_Write(uint8 value);
void    M1Resets_SetDriveMode(uint8 mode);
uint8   M1Resets_ReadDataReg(void);
uint8   M1Resets_Read(void);
void    M1Resets_SetInterruptMode(uint16 position, uint16 mode);
uint8   M1Resets_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the M1Resets_SetDriveMode() function.
     *  @{
     */
        #define M1Resets_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define M1Resets_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define M1Resets_DM_RES_UP          PIN_DM_RES_UP
        #define M1Resets_DM_RES_DWN         PIN_DM_RES_DWN
        #define M1Resets_DM_OD_LO           PIN_DM_OD_LO
        #define M1Resets_DM_OD_HI           PIN_DM_OD_HI
        #define M1Resets_DM_STRONG          PIN_DM_STRONG
        #define M1Resets_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define M1Resets_MASK               M1Resets__MASK
#define M1Resets_SHIFT              M1Resets__SHIFT
#define M1Resets_WIDTH              3u

/* Interrupt constants */
#if defined(M1Resets__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in M1Resets_SetInterruptMode() function.
     *  @{
     */
        #define M1Resets_INTR_NONE      (uint16)(0x0000u)
        #define M1Resets_INTR_RISING    (uint16)(0x0001u)
        #define M1Resets_INTR_FALLING   (uint16)(0x0002u)
        #define M1Resets_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define M1Resets_INTR_MASK      (0x01u) 
#endif /* (M1Resets__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define M1Resets_PS                     (* (reg8 *) M1Resets__PS)
/* Data Register */
#define M1Resets_DR                     (* (reg8 *) M1Resets__DR)
/* Port Number */
#define M1Resets_PRT_NUM                (* (reg8 *) M1Resets__PRT) 
/* Connect to Analog Globals */                                                  
#define M1Resets_AG                     (* (reg8 *) M1Resets__AG)                       
/* Analog MUX bux enable */
#define M1Resets_AMUX                   (* (reg8 *) M1Resets__AMUX) 
/* Bidirectional Enable */                                                        
#define M1Resets_BIE                    (* (reg8 *) M1Resets__BIE)
/* Bit-mask for Aliased Register Access */
#define M1Resets_BIT_MASK               (* (reg8 *) M1Resets__BIT_MASK)
/* Bypass Enable */
#define M1Resets_BYP                    (* (reg8 *) M1Resets__BYP)
/* Port wide control signals */                                                   
#define M1Resets_CTL                    (* (reg8 *) M1Resets__CTL)
/* Drive Modes */
#define M1Resets_DM0                    (* (reg8 *) M1Resets__DM0) 
#define M1Resets_DM1                    (* (reg8 *) M1Resets__DM1)
#define M1Resets_DM2                    (* (reg8 *) M1Resets__DM2) 
/* Input Buffer Disable Override */
#define M1Resets_INP_DIS                (* (reg8 *) M1Resets__INP_DIS)
/* LCD Common or Segment Drive */
#define M1Resets_LCD_COM_SEG            (* (reg8 *) M1Resets__LCD_COM_SEG)
/* Enable Segment LCD */
#define M1Resets_LCD_EN                 (* (reg8 *) M1Resets__LCD_EN)
/* Slew Rate Control */
#define M1Resets_SLW                    (* (reg8 *) M1Resets__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define M1Resets_PRTDSI__CAPS_SEL       (* (reg8 *) M1Resets__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define M1Resets_PRTDSI__DBL_SYNC_IN    (* (reg8 *) M1Resets__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define M1Resets_PRTDSI__OE_SEL0        (* (reg8 *) M1Resets__PRTDSI__OE_SEL0) 
#define M1Resets_PRTDSI__OE_SEL1        (* (reg8 *) M1Resets__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define M1Resets_PRTDSI__OUT_SEL0       (* (reg8 *) M1Resets__PRTDSI__OUT_SEL0) 
#define M1Resets_PRTDSI__OUT_SEL1       (* (reg8 *) M1Resets__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define M1Resets_PRTDSI__SYNC_OUT       (* (reg8 *) M1Resets__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(M1Resets__SIO_CFG)
    #define M1Resets_SIO_HYST_EN        (* (reg8 *) M1Resets__SIO_HYST_EN)
    #define M1Resets_SIO_REG_HIFREQ     (* (reg8 *) M1Resets__SIO_REG_HIFREQ)
    #define M1Resets_SIO_CFG            (* (reg8 *) M1Resets__SIO_CFG)
    #define M1Resets_SIO_DIFF           (* (reg8 *) M1Resets__SIO_DIFF)
#endif /* (M1Resets__SIO_CFG) */

/* Interrupt Registers */
#if defined(M1Resets__INTSTAT)
    #define M1Resets_INTSTAT            (* (reg8 *) M1Resets__INTSTAT)
    #define M1Resets_SNAP               (* (reg8 *) M1Resets__SNAP)
    
	#define M1Resets_0_INTTYPE_REG 		(* (reg8 *) M1Resets__0__INTTYPE)
	#define M1Resets_1_INTTYPE_REG 		(* (reg8 *) M1Resets__1__INTTYPE)
	#define M1Resets_2_INTTYPE_REG 		(* (reg8 *) M1Resets__2__INTTYPE)
#endif /* (M1Resets__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_M1Resets_H */


/* [] END OF FILE */

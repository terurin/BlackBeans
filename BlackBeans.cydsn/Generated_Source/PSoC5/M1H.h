/*******************************************************************************
* File Name: M1H.h  
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

#if !defined(CY_PINS_M1H_H) /* Pins M1H_H */
#define CY_PINS_M1H_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "M1H_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 M1H__PORT == 15 && ((M1H__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    M1H_Write(uint8 value);
void    M1H_SetDriveMode(uint8 mode);
uint8   M1H_ReadDataReg(void);
uint8   M1H_Read(void);
void    M1H_SetInterruptMode(uint16 position, uint16 mode);
uint8   M1H_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the M1H_SetDriveMode() function.
     *  @{
     */
        #define M1H_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define M1H_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define M1H_DM_RES_UP          PIN_DM_RES_UP
        #define M1H_DM_RES_DWN         PIN_DM_RES_DWN
        #define M1H_DM_OD_LO           PIN_DM_OD_LO
        #define M1H_DM_OD_HI           PIN_DM_OD_HI
        #define M1H_DM_STRONG          PIN_DM_STRONG
        #define M1H_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define M1H_MASK               M1H__MASK
#define M1H_SHIFT              M1H__SHIFT
#define M1H_WIDTH              3u

/* Interrupt constants */
#if defined(M1H__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in M1H_SetInterruptMode() function.
     *  @{
     */
        #define M1H_INTR_NONE      (uint16)(0x0000u)
        #define M1H_INTR_RISING    (uint16)(0x0001u)
        #define M1H_INTR_FALLING   (uint16)(0x0002u)
        #define M1H_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define M1H_INTR_MASK      (0x01u) 
#endif /* (M1H__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define M1H_PS                     (* (reg8 *) M1H__PS)
/* Data Register */
#define M1H_DR                     (* (reg8 *) M1H__DR)
/* Port Number */
#define M1H_PRT_NUM                (* (reg8 *) M1H__PRT) 
/* Connect to Analog Globals */                                                  
#define M1H_AG                     (* (reg8 *) M1H__AG)                       
/* Analog MUX bux enable */
#define M1H_AMUX                   (* (reg8 *) M1H__AMUX) 
/* Bidirectional Enable */                                                        
#define M1H_BIE                    (* (reg8 *) M1H__BIE)
/* Bit-mask for Aliased Register Access */
#define M1H_BIT_MASK               (* (reg8 *) M1H__BIT_MASK)
/* Bypass Enable */
#define M1H_BYP                    (* (reg8 *) M1H__BYP)
/* Port wide control signals */                                                   
#define M1H_CTL                    (* (reg8 *) M1H__CTL)
/* Drive Modes */
#define M1H_DM0                    (* (reg8 *) M1H__DM0) 
#define M1H_DM1                    (* (reg8 *) M1H__DM1)
#define M1H_DM2                    (* (reg8 *) M1H__DM2) 
/* Input Buffer Disable Override */
#define M1H_INP_DIS                (* (reg8 *) M1H__INP_DIS)
/* LCD Common or Segment Drive */
#define M1H_LCD_COM_SEG            (* (reg8 *) M1H__LCD_COM_SEG)
/* Enable Segment LCD */
#define M1H_LCD_EN                 (* (reg8 *) M1H__LCD_EN)
/* Slew Rate Control */
#define M1H_SLW                    (* (reg8 *) M1H__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define M1H_PRTDSI__CAPS_SEL       (* (reg8 *) M1H__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define M1H_PRTDSI__DBL_SYNC_IN    (* (reg8 *) M1H__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define M1H_PRTDSI__OE_SEL0        (* (reg8 *) M1H__PRTDSI__OE_SEL0) 
#define M1H_PRTDSI__OE_SEL1        (* (reg8 *) M1H__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define M1H_PRTDSI__OUT_SEL0       (* (reg8 *) M1H__PRTDSI__OUT_SEL0) 
#define M1H_PRTDSI__OUT_SEL1       (* (reg8 *) M1H__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define M1H_PRTDSI__SYNC_OUT       (* (reg8 *) M1H__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(M1H__SIO_CFG)
    #define M1H_SIO_HYST_EN        (* (reg8 *) M1H__SIO_HYST_EN)
    #define M1H_SIO_REG_HIFREQ     (* (reg8 *) M1H__SIO_REG_HIFREQ)
    #define M1H_SIO_CFG            (* (reg8 *) M1H__SIO_CFG)
    #define M1H_SIO_DIFF           (* (reg8 *) M1H__SIO_DIFF)
#endif /* (M1H__SIO_CFG) */

/* Interrupt Registers */
#if defined(M1H__INTSTAT)
    #define M1H_INTSTAT            (* (reg8 *) M1H__INTSTAT)
    #define M1H_SNAP               (* (reg8 *) M1H__SNAP)
    
	#define M1H_0_INTTYPE_REG 		(* (reg8 *) M1H__0__INTTYPE)
	#define M1H_1_INTTYPE_REG 		(* (reg8 *) M1H__1__INTTYPE)
	#define M1H_2_INTTYPE_REG 		(* (reg8 *) M1H__2__INTTYPE)
#endif /* (M1H__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_M1H_H */


/* [] END OF FILE */

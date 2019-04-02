/*******************************************************************************
* File Name: M1Y.h  
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

#if !defined(CY_PINS_M1Y_H) /* Pins M1Y_H */
#define CY_PINS_M1Y_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "M1Y_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 M1Y__PORT == 15 && ((M1Y__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    M1Y_Write(uint8 value);
void    M1Y_SetDriveMode(uint8 mode);
uint8   M1Y_ReadDataReg(void);
uint8   M1Y_Read(void);
void    M1Y_SetInterruptMode(uint16 position, uint16 mode);
uint8   M1Y_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the M1Y_SetDriveMode() function.
     *  @{
     */
        #define M1Y_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define M1Y_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define M1Y_DM_RES_UP          PIN_DM_RES_UP
        #define M1Y_DM_RES_DWN         PIN_DM_RES_DWN
        #define M1Y_DM_OD_LO           PIN_DM_OD_LO
        #define M1Y_DM_OD_HI           PIN_DM_OD_HI
        #define M1Y_DM_STRONG          PIN_DM_STRONG
        #define M1Y_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define M1Y_MASK               M1Y__MASK
#define M1Y_SHIFT              M1Y__SHIFT
#define M1Y_WIDTH              1u

/* Interrupt constants */
#if defined(M1Y__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in M1Y_SetInterruptMode() function.
     *  @{
     */
        #define M1Y_INTR_NONE      (uint16)(0x0000u)
        #define M1Y_INTR_RISING    (uint16)(0x0001u)
        #define M1Y_INTR_FALLING   (uint16)(0x0002u)
        #define M1Y_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define M1Y_INTR_MASK      (0x01u) 
#endif /* (M1Y__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define M1Y_PS                     (* (reg8 *) M1Y__PS)
/* Data Register */
#define M1Y_DR                     (* (reg8 *) M1Y__DR)
/* Port Number */
#define M1Y_PRT_NUM                (* (reg8 *) M1Y__PRT) 
/* Connect to Analog Globals */                                                  
#define M1Y_AG                     (* (reg8 *) M1Y__AG)                       
/* Analog MUX bux enable */
#define M1Y_AMUX                   (* (reg8 *) M1Y__AMUX) 
/* Bidirectional Enable */                                                        
#define M1Y_BIE                    (* (reg8 *) M1Y__BIE)
/* Bit-mask for Aliased Register Access */
#define M1Y_BIT_MASK               (* (reg8 *) M1Y__BIT_MASK)
/* Bypass Enable */
#define M1Y_BYP                    (* (reg8 *) M1Y__BYP)
/* Port wide control signals */                                                   
#define M1Y_CTL                    (* (reg8 *) M1Y__CTL)
/* Drive Modes */
#define M1Y_DM0                    (* (reg8 *) M1Y__DM0) 
#define M1Y_DM1                    (* (reg8 *) M1Y__DM1)
#define M1Y_DM2                    (* (reg8 *) M1Y__DM2) 
/* Input Buffer Disable Override */
#define M1Y_INP_DIS                (* (reg8 *) M1Y__INP_DIS)
/* LCD Common or Segment Drive */
#define M1Y_LCD_COM_SEG            (* (reg8 *) M1Y__LCD_COM_SEG)
/* Enable Segment LCD */
#define M1Y_LCD_EN                 (* (reg8 *) M1Y__LCD_EN)
/* Slew Rate Control */
#define M1Y_SLW                    (* (reg8 *) M1Y__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define M1Y_PRTDSI__CAPS_SEL       (* (reg8 *) M1Y__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define M1Y_PRTDSI__DBL_SYNC_IN    (* (reg8 *) M1Y__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define M1Y_PRTDSI__OE_SEL0        (* (reg8 *) M1Y__PRTDSI__OE_SEL0) 
#define M1Y_PRTDSI__OE_SEL1        (* (reg8 *) M1Y__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define M1Y_PRTDSI__OUT_SEL0       (* (reg8 *) M1Y__PRTDSI__OUT_SEL0) 
#define M1Y_PRTDSI__OUT_SEL1       (* (reg8 *) M1Y__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define M1Y_PRTDSI__SYNC_OUT       (* (reg8 *) M1Y__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(M1Y__SIO_CFG)
    #define M1Y_SIO_HYST_EN        (* (reg8 *) M1Y__SIO_HYST_EN)
    #define M1Y_SIO_REG_HIFREQ     (* (reg8 *) M1Y__SIO_REG_HIFREQ)
    #define M1Y_SIO_CFG            (* (reg8 *) M1Y__SIO_CFG)
    #define M1Y_SIO_DIFF           (* (reg8 *) M1Y__SIO_DIFF)
#endif /* (M1Y__SIO_CFG) */

/* Interrupt Registers */
#if defined(M1Y__INTSTAT)
    #define M1Y_INTSTAT            (* (reg8 *) M1Y__INTSTAT)
    #define M1Y_SNAP               (* (reg8 *) M1Y__SNAP)
    
	#define M1Y_0_INTTYPE_REG 		(* (reg8 *) M1Y__0__INTTYPE)
#endif /* (M1Y__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_M1Y_H */


/* [] END OF FILE */
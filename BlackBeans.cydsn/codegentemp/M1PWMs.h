/*******************************************************************************
* File Name: M1PWMs.h  
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

#if !defined(CY_PINS_M1PWMs_H) /* Pins M1PWMs_H */
#define CY_PINS_M1PWMs_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "M1PWMs_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 M1PWMs__PORT == 15 && ((M1PWMs__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    M1PWMs_Write(uint8 value);
void    M1PWMs_SetDriveMode(uint8 mode);
uint8   M1PWMs_ReadDataReg(void);
uint8   M1PWMs_Read(void);
void    M1PWMs_SetInterruptMode(uint16 position, uint16 mode);
uint8   M1PWMs_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the M1PWMs_SetDriveMode() function.
     *  @{
     */
        #define M1PWMs_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define M1PWMs_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define M1PWMs_DM_RES_UP          PIN_DM_RES_UP
        #define M1PWMs_DM_RES_DWN         PIN_DM_RES_DWN
        #define M1PWMs_DM_OD_LO           PIN_DM_OD_LO
        #define M1PWMs_DM_OD_HI           PIN_DM_OD_HI
        #define M1PWMs_DM_STRONG          PIN_DM_STRONG
        #define M1PWMs_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define M1PWMs_MASK               M1PWMs__MASK
#define M1PWMs_SHIFT              M1PWMs__SHIFT
#define M1PWMs_WIDTH              3u

/* Interrupt constants */
#if defined(M1PWMs__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in M1PWMs_SetInterruptMode() function.
     *  @{
     */
        #define M1PWMs_INTR_NONE      (uint16)(0x0000u)
        #define M1PWMs_INTR_RISING    (uint16)(0x0001u)
        #define M1PWMs_INTR_FALLING   (uint16)(0x0002u)
        #define M1PWMs_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define M1PWMs_INTR_MASK      (0x01u) 
#endif /* (M1PWMs__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define M1PWMs_PS                     (* (reg8 *) M1PWMs__PS)
/* Data Register */
#define M1PWMs_DR                     (* (reg8 *) M1PWMs__DR)
/* Port Number */
#define M1PWMs_PRT_NUM                (* (reg8 *) M1PWMs__PRT) 
/* Connect to Analog Globals */                                                  
#define M1PWMs_AG                     (* (reg8 *) M1PWMs__AG)                       
/* Analog MUX bux enable */
#define M1PWMs_AMUX                   (* (reg8 *) M1PWMs__AMUX) 
/* Bidirectional Enable */                                                        
#define M1PWMs_BIE                    (* (reg8 *) M1PWMs__BIE)
/* Bit-mask for Aliased Register Access */
#define M1PWMs_BIT_MASK               (* (reg8 *) M1PWMs__BIT_MASK)
/* Bypass Enable */
#define M1PWMs_BYP                    (* (reg8 *) M1PWMs__BYP)
/* Port wide control signals */                                                   
#define M1PWMs_CTL                    (* (reg8 *) M1PWMs__CTL)
/* Drive Modes */
#define M1PWMs_DM0                    (* (reg8 *) M1PWMs__DM0) 
#define M1PWMs_DM1                    (* (reg8 *) M1PWMs__DM1)
#define M1PWMs_DM2                    (* (reg8 *) M1PWMs__DM2) 
/* Input Buffer Disable Override */
#define M1PWMs_INP_DIS                (* (reg8 *) M1PWMs__INP_DIS)
/* LCD Common or Segment Drive */
#define M1PWMs_LCD_COM_SEG            (* (reg8 *) M1PWMs__LCD_COM_SEG)
/* Enable Segment LCD */
#define M1PWMs_LCD_EN                 (* (reg8 *) M1PWMs__LCD_EN)
/* Slew Rate Control */
#define M1PWMs_SLW                    (* (reg8 *) M1PWMs__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define M1PWMs_PRTDSI__CAPS_SEL       (* (reg8 *) M1PWMs__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define M1PWMs_PRTDSI__DBL_SYNC_IN    (* (reg8 *) M1PWMs__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define M1PWMs_PRTDSI__OE_SEL0        (* (reg8 *) M1PWMs__PRTDSI__OE_SEL0) 
#define M1PWMs_PRTDSI__OE_SEL1        (* (reg8 *) M1PWMs__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define M1PWMs_PRTDSI__OUT_SEL0       (* (reg8 *) M1PWMs__PRTDSI__OUT_SEL0) 
#define M1PWMs_PRTDSI__OUT_SEL1       (* (reg8 *) M1PWMs__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define M1PWMs_PRTDSI__SYNC_OUT       (* (reg8 *) M1PWMs__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(M1PWMs__SIO_CFG)
    #define M1PWMs_SIO_HYST_EN        (* (reg8 *) M1PWMs__SIO_HYST_EN)
    #define M1PWMs_SIO_REG_HIFREQ     (* (reg8 *) M1PWMs__SIO_REG_HIFREQ)
    #define M1PWMs_SIO_CFG            (* (reg8 *) M1PWMs__SIO_CFG)
    #define M1PWMs_SIO_DIFF           (* (reg8 *) M1PWMs__SIO_DIFF)
#endif /* (M1PWMs__SIO_CFG) */

/* Interrupt Registers */
#if defined(M1PWMs__INTSTAT)
    #define M1PWMs_INTSTAT            (* (reg8 *) M1PWMs__INTSTAT)
    #define M1PWMs_SNAP               (* (reg8 *) M1PWMs__SNAP)
    
	#define M1PWMs_0_INTTYPE_REG 		(* (reg8 *) M1PWMs__0__INTTYPE)
	#define M1PWMs_1_INTTYPE_REG 		(* (reg8 *) M1PWMs__1__INTTYPE)
	#define M1PWMs_2_INTTYPE_REG 		(* (reg8 *) M1PWMs__2__INTTYPE)
#endif /* (M1PWMs__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_M1PWMs_H */


/* [] END OF FILE */

/*******************************************************************************
* File Name: HallsA.h  
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

#if !defined(CY_PINS_HallsA_H) /* Pins HallsA_H */
#define CY_PINS_HallsA_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "HallsA_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 HallsA__PORT == 15 && ((HallsA__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    HallsA_Write(uint8 value);
void    HallsA_SetDriveMode(uint8 mode);
uint8   HallsA_ReadDataReg(void);
uint8   HallsA_Read(void);
void    HallsA_SetInterruptMode(uint16 position, uint16 mode);
uint8   HallsA_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the HallsA_SetDriveMode() function.
     *  @{
     */
        #define HallsA_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define HallsA_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define HallsA_DM_RES_UP          PIN_DM_RES_UP
        #define HallsA_DM_RES_DWN         PIN_DM_RES_DWN
        #define HallsA_DM_OD_LO           PIN_DM_OD_LO
        #define HallsA_DM_OD_HI           PIN_DM_OD_HI
        #define HallsA_DM_STRONG          PIN_DM_STRONG
        #define HallsA_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define HallsA_MASK               HallsA__MASK
#define HallsA_SHIFT              HallsA__SHIFT
#define HallsA_WIDTH              3u

/* Interrupt constants */
#if defined(HallsA__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in HallsA_SetInterruptMode() function.
     *  @{
     */
        #define HallsA_INTR_NONE      (uint16)(0x0000u)
        #define HallsA_INTR_RISING    (uint16)(0x0001u)
        #define HallsA_INTR_FALLING   (uint16)(0x0002u)
        #define HallsA_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define HallsA_INTR_MASK      (0x01u) 
#endif /* (HallsA__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define HallsA_PS                     (* (reg8 *) HallsA__PS)
/* Data Register */
#define HallsA_DR                     (* (reg8 *) HallsA__DR)
/* Port Number */
#define HallsA_PRT_NUM                (* (reg8 *) HallsA__PRT) 
/* Connect to Analog Globals */                                                  
#define HallsA_AG                     (* (reg8 *) HallsA__AG)                       
/* Analog MUX bux enable */
#define HallsA_AMUX                   (* (reg8 *) HallsA__AMUX) 
/* Bidirectional Enable */                                                        
#define HallsA_BIE                    (* (reg8 *) HallsA__BIE)
/* Bit-mask for Aliased Register Access */
#define HallsA_BIT_MASK               (* (reg8 *) HallsA__BIT_MASK)
/* Bypass Enable */
#define HallsA_BYP                    (* (reg8 *) HallsA__BYP)
/* Port wide control signals */                                                   
#define HallsA_CTL                    (* (reg8 *) HallsA__CTL)
/* Drive Modes */
#define HallsA_DM0                    (* (reg8 *) HallsA__DM0) 
#define HallsA_DM1                    (* (reg8 *) HallsA__DM1)
#define HallsA_DM2                    (* (reg8 *) HallsA__DM2) 
/* Input Buffer Disable Override */
#define HallsA_INP_DIS                (* (reg8 *) HallsA__INP_DIS)
/* LCD Common or Segment Drive */
#define HallsA_LCD_COM_SEG            (* (reg8 *) HallsA__LCD_COM_SEG)
/* Enable Segment LCD */
#define HallsA_LCD_EN                 (* (reg8 *) HallsA__LCD_EN)
/* Slew Rate Control */
#define HallsA_SLW                    (* (reg8 *) HallsA__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define HallsA_PRTDSI__CAPS_SEL       (* (reg8 *) HallsA__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define HallsA_PRTDSI__DBL_SYNC_IN    (* (reg8 *) HallsA__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define HallsA_PRTDSI__OE_SEL0        (* (reg8 *) HallsA__PRTDSI__OE_SEL0) 
#define HallsA_PRTDSI__OE_SEL1        (* (reg8 *) HallsA__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define HallsA_PRTDSI__OUT_SEL0       (* (reg8 *) HallsA__PRTDSI__OUT_SEL0) 
#define HallsA_PRTDSI__OUT_SEL1       (* (reg8 *) HallsA__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define HallsA_PRTDSI__SYNC_OUT       (* (reg8 *) HallsA__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(HallsA__SIO_CFG)
    #define HallsA_SIO_HYST_EN        (* (reg8 *) HallsA__SIO_HYST_EN)
    #define HallsA_SIO_REG_HIFREQ     (* (reg8 *) HallsA__SIO_REG_HIFREQ)
    #define HallsA_SIO_CFG            (* (reg8 *) HallsA__SIO_CFG)
    #define HallsA_SIO_DIFF           (* (reg8 *) HallsA__SIO_DIFF)
#endif /* (HallsA__SIO_CFG) */

/* Interrupt Registers */
#if defined(HallsA__INTSTAT)
    #define HallsA_INTSTAT            (* (reg8 *) HallsA__INTSTAT)
    #define HallsA_SNAP               (* (reg8 *) HallsA__SNAP)
    
	#define HallsA_0_INTTYPE_REG 		(* (reg8 *) HallsA__0__INTTYPE)
	#define HallsA_1_INTTYPE_REG 		(* (reg8 *) HallsA__1__INTTYPE)
	#define HallsA_2_INTTYPE_REG 		(* (reg8 *) HallsA__2__INTTYPE)
#endif /* (HallsA__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_HallsA_H */


/* [] END OF FILE */

/*******************************************************************************
* File Name: M1H.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_M1H_ALIASES_H) /* Pins M1H_ALIASES_H */
#define CY_PINS_M1H_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define M1H_0			(M1H__0__PC)
#define M1H_0_INTR	((uint16)((uint16)0x0001u << M1H__0__SHIFT))

#define M1H_1			(M1H__1__PC)
#define M1H_1_INTR	((uint16)((uint16)0x0001u << M1H__1__SHIFT))

#define M1H_2			(M1H__2__PC)
#define M1H_2_INTR	((uint16)((uint16)0x0001u << M1H__2__SHIFT))

#define M1H_INTR_ALL	 ((uint16)(M1H_0_INTR| M1H_1_INTR| M1H_2_INTR))

#endif /* End Pins M1H_ALIASES_H */


/* [] END OF FILE */

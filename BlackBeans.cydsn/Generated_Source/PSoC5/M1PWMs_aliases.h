/*******************************************************************************
* File Name: M1PWMs.h  
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

#if !defined(CY_PINS_M1PWMs_ALIASES_H) /* Pins M1PWMs_ALIASES_H */
#define CY_PINS_M1PWMs_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define M1PWMs_0			(M1PWMs__0__PC)
#define M1PWMs_0_INTR	((uint16)((uint16)0x0001u << M1PWMs__0__SHIFT))

#define M1PWMs_1			(M1PWMs__1__PC)
#define M1PWMs_1_INTR	((uint16)((uint16)0x0001u << M1PWMs__1__SHIFT))

#define M1PWMs_2			(M1PWMs__2__PC)
#define M1PWMs_2_INTR	((uint16)((uint16)0x0001u << M1PWMs__2__SHIFT))

#define M1PWMs_INTR_ALL	 ((uint16)(M1PWMs_0_INTR| M1PWMs_1_INTR| M1PWMs_2_INTR))

#endif /* End Pins M1PWMs_ALIASES_H */


/* [] END OF FILE */

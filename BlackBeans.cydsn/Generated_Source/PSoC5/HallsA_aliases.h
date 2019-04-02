/*******************************************************************************
* File Name: HallsA.h  
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

#if !defined(CY_PINS_HallsA_ALIASES_H) /* Pins HallsA_ALIASES_H */
#define CY_PINS_HallsA_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define HallsA_0			(HallsA__0__PC)
#define HallsA_0_INTR	((uint16)((uint16)0x0001u << HallsA__0__SHIFT))

#define HallsA_1			(HallsA__1__PC)
#define HallsA_1_INTR	((uint16)((uint16)0x0001u << HallsA__1__SHIFT))

#define HallsA_2			(HallsA__2__PC)
#define HallsA_2_INTR	((uint16)((uint16)0x0001u << HallsA__2__SHIFT))

#define HallsA_INTR_ALL	 ((uint16)(HallsA_0_INTR| HallsA_1_INTR| HallsA_2_INTR))

#endif /* End Pins HallsA_ALIASES_H */


/* [] END OF FILE */

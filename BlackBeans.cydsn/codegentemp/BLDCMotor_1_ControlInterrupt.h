/*******************************************************************************
* File Name: BLDCMotor_1_ControlInterrupt.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_BLDCMotor_1_ControlInterrupt_H)
#define CY_ISR_BLDCMotor_1_ControlInterrupt_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void BLDCMotor_1_ControlInterrupt_Start(void);
void BLDCMotor_1_ControlInterrupt_StartEx(cyisraddress address);
void BLDCMotor_1_ControlInterrupt_Stop(void);

CY_ISR_PROTO(BLDCMotor_1_ControlInterrupt_Interrupt);

void BLDCMotor_1_ControlInterrupt_SetVector(cyisraddress address);
cyisraddress BLDCMotor_1_ControlInterrupt_GetVector(void);

void BLDCMotor_1_ControlInterrupt_SetPriority(uint8 priority);
uint8 BLDCMotor_1_ControlInterrupt_GetPriority(void);

void BLDCMotor_1_ControlInterrupt_Enable(void);
uint8 BLDCMotor_1_ControlInterrupt_GetState(void);
void BLDCMotor_1_ControlInterrupt_Disable(void);

void BLDCMotor_1_ControlInterrupt_SetPending(void);
void BLDCMotor_1_ControlInterrupt_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the BLDCMotor_1_ControlInterrupt ISR. */
#define BLDCMotor_1_ControlInterrupt_INTC_VECTOR            ((reg32 *) BLDCMotor_1_ControlInterrupt__INTC_VECT)

/* Address of the BLDCMotor_1_ControlInterrupt ISR priority. */
#define BLDCMotor_1_ControlInterrupt_INTC_PRIOR             ((reg8 *) BLDCMotor_1_ControlInterrupt__INTC_PRIOR_REG)

/* Priority of the BLDCMotor_1_ControlInterrupt interrupt. */
#define BLDCMotor_1_ControlInterrupt_INTC_PRIOR_NUMBER      BLDCMotor_1_ControlInterrupt__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable BLDCMotor_1_ControlInterrupt interrupt. */
#define BLDCMotor_1_ControlInterrupt_INTC_SET_EN            ((reg32 *) BLDCMotor_1_ControlInterrupt__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the BLDCMotor_1_ControlInterrupt interrupt. */
#define BLDCMotor_1_ControlInterrupt_INTC_CLR_EN            ((reg32 *) BLDCMotor_1_ControlInterrupt__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the BLDCMotor_1_ControlInterrupt interrupt state to pending. */
#define BLDCMotor_1_ControlInterrupt_INTC_SET_PD            ((reg32 *) BLDCMotor_1_ControlInterrupt__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the BLDCMotor_1_ControlInterrupt interrupt. */
#define BLDCMotor_1_ControlInterrupt_INTC_CLR_PD            ((reg32 *) BLDCMotor_1_ControlInterrupt__INTC_CLR_PD_REG)


#endif /* CY_ISR_BLDCMotor_1_ControlInterrupt_H */


/* [] END OF FILE */

/*******************************************************************************
* File Name: MotorEncoderA_isr.h
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
#if !defined(CY_ISR_MotorEncoderA_isr_H)
#define CY_ISR_MotorEncoderA_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void MotorEncoderA_isr_Start(void);
void MotorEncoderA_isr_StartEx(cyisraddress address);
void MotorEncoderA_isr_Stop(void);

CY_ISR_PROTO(MotorEncoderA_isr_Interrupt);

void MotorEncoderA_isr_SetVector(cyisraddress address);
cyisraddress MotorEncoderA_isr_GetVector(void);

void MotorEncoderA_isr_SetPriority(uint8 priority);
uint8 MotorEncoderA_isr_GetPriority(void);

void MotorEncoderA_isr_Enable(void);
uint8 MotorEncoderA_isr_GetState(void);
void MotorEncoderA_isr_Disable(void);

void MotorEncoderA_isr_SetPending(void);
void MotorEncoderA_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the MotorEncoderA_isr ISR. */
#define MotorEncoderA_isr_INTC_VECTOR            ((reg32 *) MotorEncoderA_isr__INTC_VECT)

/* Address of the MotorEncoderA_isr ISR priority. */
#define MotorEncoderA_isr_INTC_PRIOR             ((reg8 *) MotorEncoderA_isr__INTC_PRIOR_REG)

/* Priority of the MotorEncoderA_isr interrupt. */
#define MotorEncoderA_isr_INTC_PRIOR_NUMBER      MotorEncoderA_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable MotorEncoderA_isr interrupt. */
#define MotorEncoderA_isr_INTC_SET_EN            ((reg32 *) MotorEncoderA_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the MotorEncoderA_isr interrupt. */
#define MotorEncoderA_isr_INTC_CLR_EN            ((reg32 *) MotorEncoderA_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the MotorEncoderA_isr interrupt state to pending. */
#define MotorEncoderA_isr_INTC_SET_PD            ((reg32 *) MotorEncoderA_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the MotorEncoderA_isr interrupt. */
#define MotorEncoderA_isr_INTC_CLR_PD            ((reg32 *) MotorEncoderA_isr__INTC_CLR_PD_REG)


#endif /* CY_ISR_MotorEncoderA_isr_H */


/* [] END OF FILE */

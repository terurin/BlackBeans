/*******************************************************************************
* File Name: MotorInterval.h
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
#if !defined(CY_ISR_MotorInterval_H)
#define CY_ISR_MotorInterval_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void MotorInterval_Start(void);
void MotorInterval_StartEx(cyisraddress address);
void MotorInterval_Stop(void);

CY_ISR_PROTO(MotorInterval_Interrupt);

void MotorInterval_SetVector(cyisraddress address);
cyisraddress MotorInterval_GetVector(void);

void MotorInterval_SetPriority(uint8 priority);
uint8 MotorInterval_GetPriority(void);

void MotorInterval_Enable(void);
uint8 MotorInterval_GetState(void);
void MotorInterval_Disable(void);

void MotorInterval_SetPending(void);
void MotorInterval_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the MotorInterval ISR. */
#define MotorInterval_INTC_VECTOR            ((reg32 *) MotorInterval__INTC_VECT)

/* Address of the MotorInterval ISR priority. */
#define MotorInterval_INTC_PRIOR             ((reg8 *) MotorInterval__INTC_PRIOR_REG)

/* Priority of the MotorInterval interrupt. */
#define MotorInterval_INTC_PRIOR_NUMBER      MotorInterval__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable MotorInterval interrupt. */
#define MotorInterval_INTC_SET_EN            ((reg32 *) MotorInterval__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the MotorInterval interrupt. */
#define MotorInterval_INTC_CLR_EN            ((reg32 *) MotorInterval__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the MotorInterval interrupt state to pending. */
#define MotorInterval_INTC_SET_PD            ((reg32 *) MotorInterval__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the MotorInterval interrupt. */
#define MotorInterval_INTC_CLR_PD            ((reg32 *) MotorInterval__INTC_CLR_PD_REG)


#endif /* CY_ISR_MotorInterval_H */


/* [] END OF FILE */

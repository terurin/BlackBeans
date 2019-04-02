/*******************************************************************************
* File Name: BLDCDriver_1_PulseGenerator.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_BLDCDriver_1_PulseGenerator_H)
#define CY_PWM_BLDCDriver_1_PulseGenerator_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 BLDCDriver_1_PulseGenerator_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define BLDCDriver_1_PulseGenerator_Resolution                     (16u)
#define BLDCDriver_1_PulseGenerator_UsingFixedFunction             (1u)
#define BLDCDriver_1_PulseGenerator_DeadBandMode                   (0u)
#define BLDCDriver_1_PulseGenerator_KillModeMinTime                (0u)
#define BLDCDriver_1_PulseGenerator_KillMode                       (1u)
#define BLDCDriver_1_PulseGenerator_PWMMode                        (0u)
#define BLDCDriver_1_PulseGenerator_PWMModeIsCenterAligned         (0u)
#define BLDCDriver_1_PulseGenerator_DeadBandUsed                   (0u)
#define BLDCDriver_1_PulseGenerator_DeadBand2_4                    (0u)

#if !defined(BLDCDriver_1_PulseGenerator_PWMUDB_genblk8_stsreg__REMOVED)
    #define BLDCDriver_1_PulseGenerator_UseStatus                  (1u)
#else
    #define BLDCDriver_1_PulseGenerator_UseStatus                  (0u)
#endif /* !defined(BLDCDriver_1_PulseGenerator_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(BLDCDriver_1_PulseGenerator_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define BLDCDriver_1_PulseGenerator_UseControl                 (1u)
#else
    #define BLDCDriver_1_PulseGenerator_UseControl                 (0u)
#endif /* !defined(BLDCDriver_1_PulseGenerator_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define BLDCDriver_1_PulseGenerator_UseOneCompareMode              (1u)
#define BLDCDriver_1_PulseGenerator_MinimumKillTime                (1u)
#define BLDCDriver_1_PulseGenerator_EnableMode                     (0u)

#define BLDCDriver_1_PulseGenerator_CompareMode1SW                 (0u)
#define BLDCDriver_1_PulseGenerator_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define BLDCDriver_1_PulseGenerator__B_PWM__DISABLED 0
#define BLDCDriver_1_PulseGenerator__B_PWM__ASYNCHRONOUS 1
#define BLDCDriver_1_PulseGenerator__B_PWM__SINGLECYCLE 2
#define BLDCDriver_1_PulseGenerator__B_PWM__LATCHED 3
#define BLDCDriver_1_PulseGenerator__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define BLDCDriver_1_PulseGenerator__B_PWM__DBMDISABLED 0
#define BLDCDriver_1_PulseGenerator__B_PWM__DBM_2_4_CLOCKS 1
#define BLDCDriver_1_PulseGenerator__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define BLDCDriver_1_PulseGenerator__B_PWM__ONE_OUTPUT 0
#define BLDCDriver_1_PulseGenerator__B_PWM__TWO_OUTPUTS 1
#define BLDCDriver_1_PulseGenerator__B_PWM__DUAL_EDGE 2
#define BLDCDriver_1_PulseGenerator__B_PWM__CENTER_ALIGN 3
#define BLDCDriver_1_PulseGenerator__B_PWM__DITHER 5
#define BLDCDriver_1_PulseGenerator__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define BLDCDriver_1_PulseGenerator__B_PWM__LESS_THAN 1
#define BLDCDriver_1_PulseGenerator__B_PWM__LESS_THAN_OR_EQUAL 2
#define BLDCDriver_1_PulseGenerator__B_PWM__GREATER_THAN 3
#define BLDCDriver_1_PulseGenerator__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define BLDCDriver_1_PulseGenerator__B_PWM__EQUAL 0
#define BLDCDriver_1_PulseGenerator__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!BLDCDriver_1_PulseGenerator_UsingFixedFunction)
        uint16 PWMUdb;               /* PWM Current Counter value  */
        #if(!BLDCDriver_1_PulseGenerator_PWMModeIsCenterAligned)
            uint16 PWMPeriod;
        #endif /* (!BLDCDriver_1_PulseGenerator_PWMModeIsCenterAligned) */
        #if (BLDCDriver_1_PulseGenerator_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (BLDCDriver_1_PulseGenerator_UseStatus) */

        /* Backup for Deadband parameters */
        #if(BLDCDriver_1_PulseGenerator_DeadBandMode == BLDCDriver_1_PulseGenerator__B_PWM__DBM_256_CLOCKS || \
            BLDCDriver_1_PulseGenerator_DeadBandMode == BLDCDriver_1_PulseGenerator__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(BLDCDriver_1_PulseGenerator_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (BLDCDriver_1_PulseGenerator_KillModeMinTime) */

        /* Backup control register */
        #if(BLDCDriver_1_PulseGenerator_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (BLDCDriver_1_PulseGenerator_UseControl) */

    #endif /* (!BLDCDriver_1_PulseGenerator_UsingFixedFunction) */

}BLDCDriver_1_PulseGenerator_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    BLDCDriver_1_PulseGenerator_Start(void) ;
void    BLDCDriver_1_PulseGenerator_Stop(void) ;

#if (BLDCDriver_1_PulseGenerator_UseStatus || BLDCDriver_1_PulseGenerator_UsingFixedFunction)
    void  BLDCDriver_1_PulseGenerator_SetInterruptMode(uint8 interruptMode) ;
    uint8 BLDCDriver_1_PulseGenerator_ReadStatusRegister(void) ;
#endif /* (BLDCDriver_1_PulseGenerator_UseStatus || BLDCDriver_1_PulseGenerator_UsingFixedFunction) */

#define BLDCDriver_1_PulseGenerator_GetInterruptSource() BLDCDriver_1_PulseGenerator_ReadStatusRegister()

#if (BLDCDriver_1_PulseGenerator_UseControl)
    uint8 BLDCDriver_1_PulseGenerator_ReadControlRegister(void) ;
    void  BLDCDriver_1_PulseGenerator_WriteControlRegister(uint8 control)
          ;
#endif /* (BLDCDriver_1_PulseGenerator_UseControl) */

#if (BLDCDriver_1_PulseGenerator_UseOneCompareMode)
   #if (BLDCDriver_1_PulseGenerator_CompareMode1SW)
       void    BLDCDriver_1_PulseGenerator_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (BLDCDriver_1_PulseGenerator_CompareMode1SW) */
#else
    #if (BLDCDriver_1_PulseGenerator_CompareMode1SW)
        void    BLDCDriver_1_PulseGenerator_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (BLDCDriver_1_PulseGenerator_CompareMode1SW) */
    #if (BLDCDriver_1_PulseGenerator_CompareMode2SW)
        void    BLDCDriver_1_PulseGenerator_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (BLDCDriver_1_PulseGenerator_CompareMode2SW) */
#endif /* (BLDCDriver_1_PulseGenerator_UseOneCompareMode) */

#if (!BLDCDriver_1_PulseGenerator_UsingFixedFunction)
    uint16   BLDCDriver_1_PulseGenerator_ReadCounter(void) ;
    uint16 BLDCDriver_1_PulseGenerator_ReadCapture(void) ;

    #if (BLDCDriver_1_PulseGenerator_UseStatus)
            void BLDCDriver_1_PulseGenerator_ClearFIFO(void) ;
    #endif /* (BLDCDriver_1_PulseGenerator_UseStatus) */

    void    BLDCDriver_1_PulseGenerator_WriteCounter(uint16 counter)
            ;
#endif /* (!BLDCDriver_1_PulseGenerator_UsingFixedFunction) */

void    BLDCDriver_1_PulseGenerator_WritePeriod(uint16 period)
        ;
uint16 BLDCDriver_1_PulseGenerator_ReadPeriod(void) ;

#if (BLDCDriver_1_PulseGenerator_UseOneCompareMode)
    void    BLDCDriver_1_PulseGenerator_WriteCompare(uint16 compare)
            ;
    uint16 BLDCDriver_1_PulseGenerator_ReadCompare(void) ;
#else
    void    BLDCDriver_1_PulseGenerator_WriteCompare1(uint16 compare)
            ;
    uint16 BLDCDriver_1_PulseGenerator_ReadCompare1(void) ;
    void    BLDCDriver_1_PulseGenerator_WriteCompare2(uint16 compare)
            ;
    uint16 BLDCDriver_1_PulseGenerator_ReadCompare2(void) ;
#endif /* (BLDCDriver_1_PulseGenerator_UseOneCompareMode) */


#if (BLDCDriver_1_PulseGenerator_DeadBandUsed)
    void    BLDCDriver_1_PulseGenerator_WriteDeadTime(uint8 deadtime) ;
    uint8   BLDCDriver_1_PulseGenerator_ReadDeadTime(void) ;
#endif /* (BLDCDriver_1_PulseGenerator_DeadBandUsed) */

#if ( BLDCDriver_1_PulseGenerator_KillModeMinTime)
    void BLDCDriver_1_PulseGenerator_WriteKillTime(uint8 killtime) ;
    uint8 BLDCDriver_1_PulseGenerator_ReadKillTime(void) ;
#endif /* ( BLDCDriver_1_PulseGenerator_KillModeMinTime) */

void BLDCDriver_1_PulseGenerator_Init(void) ;
void BLDCDriver_1_PulseGenerator_Enable(void) ;
void BLDCDriver_1_PulseGenerator_Sleep(void) ;
void BLDCDriver_1_PulseGenerator_Wakeup(void) ;
void BLDCDriver_1_PulseGenerator_SaveConfig(void) ;
void BLDCDriver_1_PulseGenerator_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define BLDCDriver_1_PulseGenerator_INIT_PERIOD_VALUE          (7199u)
#define BLDCDriver_1_PulseGenerator_INIT_COMPARE_VALUE1        (127u)
#define BLDCDriver_1_PulseGenerator_INIT_COMPARE_VALUE2        (63u)
#define BLDCDriver_1_PulseGenerator_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(1u <<   \
                                                    BLDCDriver_1_PulseGenerator_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    BLDCDriver_1_PulseGenerator_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    BLDCDriver_1_PulseGenerator_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    BLDCDriver_1_PulseGenerator_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define BLDCDriver_1_PulseGenerator_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_SHIFT)
#define BLDCDriver_1_PulseGenerator_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_SHIFT)
#define BLDCDriver_1_PulseGenerator_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (BLDCDriver_1_PulseGenerator_UsingFixedFunction)
   #define BLDCDriver_1_PulseGenerator_PERIOD_LSB              (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMHW__PER0)
   #define BLDCDriver_1_PulseGenerator_PERIOD_LSB_PTR          ( (reg16 *) BLDCDriver_1_PulseGenerator_PWMHW__PER0)
   #define BLDCDriver_1_PulseGenerator_COMPARE1_LSB            (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMHW__CNT_CMP0)
   #define BLDCDriver_1_PulseGenerator_COMPARE1_LSB_PTR        ( (reg16 *) BLDCDriver_1_PulseGenerator_PWMHW__CNT_CMP0)
   #define BLDCDriver_1_PulseGenerator_COMPARE2_LSB            (0x00u)
   #define BLDCDriver_1_PulseGenerator_COMPARE2_LSB_PTR        (0x00u)
   #define BLDCDriver_1_PulseGenerator_COUNTER_LSB             (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMHW__CNT_CMP0)
   #define BLDCDriver_1_PulseGenerator_COUNTER_LSB_PTR         ( (reg16 *) BLDCDriver_1_PulseGenerator_PWMHW__CNT_CMP0)
   #define BLDCDriver_1_PulseGenerator_CAPTURE_LSB             (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMHW__CAP0)
   #define BLDCDriver_1_PulseGenerator_CAPTURE_LSB_PTR         ( (reg16 *) BLDCDriver_1_PulseGenerator_PWMHW__CAP0)
   #define BLDCDriver_1_PulseGenerator_RT1                     (*(reg8 *)  BLDCDriver_1_PulseGenerator_PWMHW__RT1)
   #define BLDCDriver_1_PulseGenerator_RT1_PTR                 ( (reg8 *)  BLDCDriver_1_PulseGenerator_PWMHW__RT1)

#else
   #if (BLDCDriver_1_PulseGenerator_Resolution == 8u) /* 8bit - PWM */

       #if(BLDCDriver_1_PulseGenerator_PWMModeIsCenterAligned)
           #define BLDCDriver_1_PulseGenerator_PERIOD_LSB      (*(reg8 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__D1_REG)
           #define BLDCDriver_1_PulseGenerator_PERIOD_LSB_PTR  ((reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #else
           #define BLDCDriver_1_PulseGenerator_PERIOD_LSB      (*(reg8 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__F0_REG)
           #define BLDCDriver_1_PulseGenerator_PERIOD_LSB_PTR  ((reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #endif /* (BLDCDriver_1_PulseGenerator_PWMModeIsCenterAligned) */

       #define BLDCDriver_1_PulseGenerator_COMPARE1_LSB        (*(reg8 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define BLDCDriver_1_PulseGenerator_COMPARE1_LSB_PTR    ((reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define BLDCDriver_1_PulseGenerator_COMPARE2_LSB        (*(reg8 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define BLDCDriver_1_PulseGenerator_COMPARE2_LSB_PTR    ((reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define BLDCDriver_1_PulseGenerator_COUNTERCAP_LSB      (*(reg8 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define BLDCDriver_1_PulseGenerator_COUNTERCAP_LSB_PTR  ((reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define BLDCDriver_1_PulseGenerator_COUNTER_LSB         (*(reg8 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define BLDCDriver_1_PulseGenerator_COUNTER_LSB_PTR     ((reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define BLDCDriver_1_PulseGenerator_CAPTURE_LSB         (*(reg8 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__F1_REG)
       #define BLDCDriver_1_PulseGenerator_CAPTURE_LSB_PTR     ((reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(BLDCDriver_1_PulseGenerator_PWMModeIsCenterAligned)
               #define BLDCDriver_1_PulseGenerator_PERIOD_LSB      (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__D1_REG)
               #define BLDCDriver_1_PulseGenerator_PERIOD_LSB_PTR  ((reg16 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #else
               #define BLDCDriver_1_PulseGenerator_PERIOD_LSB      (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__F0_REG)
               #define BLDCDriver_1_PulseGenerator_PERIOD_LSB_PTR  ((reg16 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__F0_REG)
            #endif /* (BLDCDriver_1_PulseGenerator_PWMModeIsCenterAligned) */

            #define BLDCDriver_1_PulseGenerator_COMPARE1_LSB       (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define BLDCDriver_1_PulseGenerator_COMPARE1_LSB_PTR   ((reg16 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define BLDCDriver_1_PulseGenerator_COMPARE2_LSB       (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define BLDCDriver_1_PulseGenerator_COMPARE2_LSB_PTR   ((reg16 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define BLDCDriver_1_PulseGenerator_COUNTERCAP_LSB     (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define BLDCDriver_1_PulseGenerator_COUNTERCAP_LSB_PTR ((reg16 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define BLDCDriver_1_PulseGenerator_COUNTER_LSB        (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define BLDCDriver_1_PulseGenerator_COUNTER_LSB_PTR    ((reg16 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define BLDCDriver_1_PulseGenerator_CAPTURE_LSB        (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__F1_REG)
            #define BLDCDriver_1_PulseGenerator_CAPTURE_LSB_PTR    ((reg16 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__F1_REG)
        #else
            #if(BLDCDriver_1_PulseGenerator_PWMModeIsCenterAligned)
               #define BLDCDriver_1_PulseGenerator_PERIOD_LSB      (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
               #define BLDCDriver_1_PulseGenerator_PERIOD_LSB_PTR  ((reg16 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #else
               #define BLDCDriver_1_PulseGenerator_PERIOD_LSB      (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
               #define BLDCDriver_1_PulseGenerator_PERIOD_LSB_PTR  ((reg16 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
            #endif /* (BLDCDriver_1_PulseGenerator_PWMModeIsCenterAligned) */

            #define BLDCDriver_1_PulseGenerator_COMPARE1_LSB       (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define BLDCDriver_1_PulseGenerator_COMPARE1_LSB_PTR   ((reg16 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define BLDCDriver_1_PulseGenerator_COMPARE2_LSB       (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define BLDCDriver_1_PulseGenerator_COMPARE2_LSB_PTR   ((reg16 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define BLDCDriver_1_PulseGenerator_COUNTERCAP_LSB     (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define BLDCDriver_1_PulseGenerator_COUNTERCAP_LSB_PTR ((reg16 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define BLDCDriver_1_PulseGenerator_COUNTER_LSB        (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define BLDCDriver_1_PulseGenerator_COUNTER_LSB_PTR    ((reg16 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define BLDCDriver_1_PulseGenerator_CAPTURE_LSB        (*(reg16 *) BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
            #define BLDCDriver_1_PulseGenerator_CAPTURE_LSB_PTR    ((reg16 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define BLDCDriver_1_PulseGenerator_AUX_CONTROLDP1          (*(reg8 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define BLDCDriver_1_PulseGenerator_AUX_CONTROLDP1_PTR      ((reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (BLDCDriver_1_PulseGenerator_Resolution == 8) */

   #define BLDCDriver_1_PulseGenerator_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define BLDCDriver_1_PulseGenerator_AUX_CONTROLDP0          (*(reg8 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define BLDCDriver_1_PulseGenerator_AUX_CONTROLDP0_PTR      ((reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (BLDCDriver_1_PulseGenerator_UsingFixedFunction) */

#if(BLDCDriver_1_PulseGenerator_KillModeMinTime )
    #define BLDCDriver_1_PulseGenerator_KILLMODEMINTIME        (*(reg8 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define BLDCDriver_1_PulseGenerator_KILLMODEMINTIME_PTR    ((reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (BLDCDriver_1_PulseGenerator_KillModeMinTime ) */

#if(BLDCDriver_1_PulseGenerator_DeadBandMode == BLDCDriver_1_PulseGenerator__B_PWM__DBM_256_CLOCKS)
    #define BLDCDriver_1_PulseGenerator_DEADBAND_COUNT         (*(reg8 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_PTR     ((reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define BLDCDriver_1_PulseGenerator_DEADBAND_LSB_PTR       ((reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define BLDCDriver_1_PulseGenerator_DEADBAND_LSB           (*(reg8 *)  BLDCDriver_1_PulseGenerator_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(BLDCDriver_1_PulseGenerator_DeadBandMode == BLDCDriver_1_PulseGenerator__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (BLDCDriver_1_PulseGenerator_UsingFixedFunction)
        #define BLDCDriver_1_PulseGenerator_DEADBAND_COUNT         (*(reg8 *)  BLDCDriver_1_PulseGenerator_PWMHW__CFG0)
        #define BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_PTR     ((reg8 *)   BLDCDriver_1_PulseGenerator_PWMHW__CFG0)
        #define BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define BLDCDriver_1_PulseGenerator_DEADBAND_COUNT         (*(reg8 *)  BLDCDriver_1_PulseGenerator_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_PTR     ((reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define BLDCDriver_1_PulseGenerator_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (BLDCDriver_1_PulseGenerator_UsingFixedFunction) */
#endif /* (BLDCDriver_1_PulseGenerator_DeadBandMode == BLDCDriver_1_PulseGenerator__B_PWM__DBM_256_CLOCKS) */



#if (BLDCDriver_1_PulseGenerator_UsingFixedFunction)
    #define BLDCDriver_1_PulseGenerator_STATUS                 (*(reg8 *) BLDCDriver_1_PulseGenerator_PWMHW__SR0)
    #define BLDCDriver_1_PulseGenerator_STATUS_PTR             ((reg8 *) BLDCDriver_1_PulseGenerator_PWMHW__SR0)
    #define BLDCDriver_1_PulseGenerator_STATUS_MASK            (*(reg8 *) BLDCDriver_1_PulseGenerator_PWMHW__SR0)
    #define BLDCDriver_1_PulseGenerator_STATUS_MASK_PTR        ((reg8 *) BLDCDriver_1_PulseGenerator_PWMHW__SR0)
    #define BLDCDriver_1_PulseGenerator_CONTROL                (*(reg8 *) BLDCDriver_1_PulseGenerator_PWMHW__CFG0)
    #define BLDCDriver_1_PulseGenerator_CONTROL_PTR            ((reg8 *) BLDCDriver_1_PulseGenerator_PWMHW__CFG0)
    #define BLDCDriver_1_PulseGenerator_CONTROL2               (*(reg8 *) BLDCDriver_1_PulseGenerator_PWMHW__CFG1)
    #define BLDCDriver_1_PulseGenerator_CONTROL3               (*(reg8 *) BLDCDriver_1_PulseGenerator_PWMHW__CFG2)
    #define BLDCDriver_1_PulseGenerator_GLOBAL_ENABLE          (*(reg8 *) BLDCDriver_1_PulseGenerator_PWMHW__PM_ACT_CFG)
    #define BLDCDriver_1_PulseGenerator_GLOBAL_ENABLE_PTR      ( (reg8 *) BLDCDriver_1_PulseGenerator_PWMHW__PM_ACT_CFG)
    #define BLDCDriver_1_PulseGenerator_GLOBAL_STBY_ENABLE     (*(reg8 *) BLDCDriver_1_PulseGenerator_PWMHW__PM_STBY_CFG)
    #define BLDCDriver_1_PulseGenerator_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) BLDCDriver_1_PulseGenerator_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define BLDCDriver_1_PulseGenerator_BLOCK_EN_MASK          (BLDCDriver_1_PulseGenerator_PWMHW__PM_ACT_MSK)
    #define BLDCDriver_1_PulseGenerator_BLOCK_STBY_EN_MASK     (BLDCDriver_1_PulseGenerator_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define BLDCDriver_1_PulseGenerator_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define BLDCDriver_1_PulseGenerator_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define BLDCDriver_1_PulseGenerator_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define BLDCDriver_1_PulseGenerator_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define BLDCDriver_1_PulseGenerator_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define BLDCDriver_1_PulseGenerator_CTRL_ENABLE            (uint8)((uint8)0x01u << BLDCDriver_1_PulseGenerator_CTRL_ENABLE_SHIFT)
    #define BLDCDriver_1_PulseGenerator_CTRL_RESET             (uint8)((uint8)0x01u << BLDCDriver_1_PulseGenerator_CTRL_RESET_SHIFT)
    #define BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_SHIFT)
    #define BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define BLDCDriver_1_PulseGenerator_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define BLDCDriver_1_PulseGenerator_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << BLDCDriver_1_PulseGenerator_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define BLDCDriver_1_PulseGenerator_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define BLDCDriver_1_PulseGenerator_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define BLDCDriver_1_PulseGenerator_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define BLDCDriver_1_PulseGenerator_STATUS_TC_INT_EN_MASK_SHIFT            (BLDCDriver_1_PulseGenerator_STATUS_TC_SHIFT - 4u)
    #define BLDCDriver_1_PulseGenerator_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define BLDCDriver_1_PulseGenerator_STATUS_CMP1_INT_EN_MASK_SHIFT          (BLDCDriver_1_PulseGenerator_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define BLDCDriver_1_PulseGenerator_STATUS_TC              (uint8)((uint8)0x01u << BLDCDriver_1_PulseGenerator_STATUS_TC_SHIFT)
    #define BLDCDriver_1_PulseGenerator_STATUS_CMP1            (uint8)((uint8)0x01u << BLDCDriver_1_PulseGenerator_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define BLDCDriver_1_PulseGenerator_STATUS_TC_INT_EN_MASK              (uint8)((uint8)BLDCDriver_1_PulseGenerator_STATUS_TC >> 4u)
    #define BLDCDriver_1_PulseGenerator_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)BLDCDriver_1_PulseGenerator_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define BLDCDriver_1_PulseGenerator_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define BLDCDriver_1_PulseGenerator_RT1_MASK              (uint8)((uint8)0x03u << BLDCDriver_1_PulseGenerator_RT1_SHIFT)
    #define BLDCDriver_1_PulseGenerator_SYNC                  (uint8)((uint8)0x03u << BLDCDriver_1_PulseGenerator_RT1_SHIFT)
    #define BLDCDriver_1_PulseGenerator_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define BLDCDriver_1_PulseGenerator_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << BLDCDriver_1_PulseGenerator_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define BLDCDriver_1_PulseGenerator_SYNCDSI_EN            (uint8)((uint8)0x0Fu << BLDCDriver_1_PulseGenerator_SYNCDSI_SHIFT)


#else
    #define BLDCDriver_1_PulseGenerator_STATUS                (*(reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define BLDCDriver_1_PulseGenerator_STATUS_PTR            ((reg8 *)    BLDCDriver_1_PulseGenerator_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define BLDCDriver_1_PulseGenerator_STATUS_MASK           (*(reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_genblk8_stsreg__MASK_REG)
    #define BLDCDriver_1_PulseGenerator_STATUS_MASK_PTR       ((reg8 *)    BLDCDriver_1_PulseGenerator_PWMUDB_genblk8_stsreg__MASK_REG)
    #define BLDCDriver_1_PulseGenerator_STATUS_AUX_CTRL       (*(reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define BLDCDriver_1_PulseGenerator_CONTROL               (*(reg8 *)   BLDCDriver_1_PulseGenerator_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define BLDCDriver_1_PulseGenerator_CONTROL_PTR           ((reg8 *)    BLDCDriver_1_PulseGenerator_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define BLDCDriver_1_PulseGenerator_CTRL_ENABLE_SHIFT      (0x07u)
    #define BLDCDriver_1_PulseGenerator_CTRL_RESET_SHIFT       (0x06u)
    #define BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define BLDCDriver_1_PulseGenerator_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define BLDCDriver_1_PulseGenerator_CTRL_ENABLE            (uint8)((uint8)0x01u << BLDCDriver_1_PulseGenerator_CTRL_ENABLE_SHIFT)
    #define BLDCDriver_1_PulseGenerator_CTRL_RESET             (uint8)((uint8)0x01u << BLDCDriver_1_PulseGenerator_CTRL_RESET_SHIFT)
    #define BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << BLDCDriver_1_PulseGenerator_CTRL_CMPMODE2_SHIFT)
    #define BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << BLDCDriver_1_PulseGenerator_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define BLDCDriver_1_PulseGenerator_STATUS_KILL_SHIFT          (0x05u)
    #define BLDCDriver_1_PulseGenerator_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define BLDCDriver_1_PulseGenerator_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define BLDCDriver_1_PulseGenerator_STATUS_TC_SHIFT            (0x02u)
    #define BLDCDriver_1_PulseGenerator_STATUS_CMP2_SHIFT          (0x01u)
    #define BLDCDriver_1_PulseGenerator_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define BLDCDriver_1_PulseGenerator_STATUS_KILL_INT_EN_MASK_SHIFT          (BLDCDriver_1_PulseGenerator_STATUS_KILL_SHIFT)
    #define BLDCDriver_1_PulseGenerator_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (BLDCDriver_1_PulseGenerator_STATUS_FIFONEMPTY_SHIFT)
    #define BLDCDriver_1_PulseGenerator_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (BLDCDriver_1_PulseGenerator_STATUS_FIFOFULL_SHIFT)
    #define BLDCDriver_1_PulseGenerator_STATUS_TC_INT_EN_MASK_SHIFT            (BLDCDriver_1_PulseGenerator_STATUS_TC_SHIFT)
    #define BLDCDriver_1_PulseGenerator_STATUS_CMP2_INT_EN_MASK_SHIFT          (BLDCDriver_1_PulseGenerator_STATUS_CMP2_SHIFT)
    #define BLDCDriver_1_PulseGenerator_STATUS_CMP1_INT_EN_MASK_SHIFT          (BLDCDriver_1_PulseGenerator_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define BLDCDriver_1_PulseGenerator_STATUS_KILL            (uint8)((uint8)0x00u << BLDCDriver_1_PulseGenerator_STATUS_KILL_SHIFT )
    #define BLDCDriver_1_PulseGenerator_STATUS_FIFOFULL        (uint8)((uint8)0x01u << BLDCDriver_1_PulseGenerator_STATUS_FIFOFULL_SHIFT)
    #define BLDCDriver_1_PulseGenerator_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << BLDCDriver_1_PulseGenerator_STATUS_FIFONEMPTY_SHIFT)
    #define BLDCDriver_1_PulseGenerator_STATUS_TC              (uint8)((uint8)0x01u << BLDCDriver_1_PulseGenerator_STATUS_TC_SHIFT)
    #define BLDCDriver_1_PulseGenerator_STATUS_CMP2            (uint8)((uint8)0x01u << BLDCDriver_1_PulseGenerator_STATUS_CMP2_SHIFT)
    #define BLDCDriver_1_PulseGenerator_STATUS_CMP1            (uint8)((uint8)0x01u << BLDCDriver_1_PulseGenerator_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define BLDCDriver_1_PulseGenerator_STATUS_KILL_INT_EN_MASK            (BLDCDriver_1_PulseGenerator_STATUS_KILL)
    #define BLDCDriver_1_PulseGenerator_STATUS_FIFOFULL_INT_EN_MASK        (BLDCDriver_1_PulseGenerator_STATUS_FIFOFULL)
    #define BLDCDriver_1_PulseGenerator_STATUS_FIFONEMPTY_INT_EN_MASK      (BLDCDriver_1_PulseGenerator_STATUS_FIFONEMPTY)
    #define BLDCDriver_1_PulseGenerator_STATUS_TC_INT_EN_MASK              (BLDCDriver_1_PulseGenerator_STATUS_TC)
    #define BLDCDriver_1_PulseGenerator_STATUS_CMP2_INT_EN_MASK            (BLDCDriver_1_PulseGenerator_STATUS_CMP2)
    #define BLDCDriver_1_PulseGenerator_STATUS_CMP1_INT_EN_MASK            (BLDCDriver_1_PulseGenerator_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define BLDCDriver_1_PulseGenerator_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define BLDCDriver_1_PulseGenerator_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define BLDCDriver_1_PulseGenerator_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define BLDCDriver_1_PulseGenerator_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define BLDCDriver_1_PulseGenerator_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* BLDCDriver_1_PulseGenerator_UsingFixedFunction */

#endif  /* CY_PWM_BLDCDriver_1_PulseGenerator_H */


/* [] END OF FILE */

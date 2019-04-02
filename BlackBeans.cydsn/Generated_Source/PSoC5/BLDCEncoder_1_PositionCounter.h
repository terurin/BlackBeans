/*******************************************************************************
* File Name: BLDCEncoder_1_PositionCounter.h  
* Version 3.0
*
*  Description:
*   Contains the function prototypes and constants available to the counter
*   user module.
*
*   Note:
*    None
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
    
#if !defined(CY_COUNTER_BLDCEncoder_1_PositionCounter_H)
#define CY_COUNTER_BLDCEncoder_1_PositionCounter_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 BLDCEncoder_1_PositionCounter_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Counter_v3_0 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Error message for removed BLDCEncoder_1_PositionCounter_CounterUDB_sCTRLReg_ctrlreg through optimization */
#ifdef BLDCEncoder_1_PositionCounter_CounterUDB_sCTRLReg_ctrlreg__REMOVED
    #error Counter_v3_0 detected with a constant 0 for the enable, a \
                                constant 0 for the count or constant 1 for \
                                the reset. This will prevent the component from\
                                operating.
#endif /* BLDCEncoder_1_PositionCounter_CounterUDB_sCTRLReg_ctrlreg__REMOVED */


/**************************************
*           Parameter Defaults        
**************************************/

#define BLDCEncoder_1_PositionCounter_Resolution            8u
#define BLDCEncoder_1_PositionCounter_UsingFixedFunction    0u
#define BLDCEncoder_1_PositionCounter_ControlRegRemoved     0u
#define BLDCEncoder_1_PositionCounter_COMPARE_MODE_SOFTWARE 0u
#define BLDCEncoder_1_PositionCounter_CAPTURE_MODE_SOFTWARE 0u
#define BLDCEncoder_1_PositionCounter_RunModeUsed           0u


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Mode API Support
 * Backup structure for Sleep Wake up operations
 *************************************************************************/
typedef struct
{
    uint8 CounterEnableState; 
    uint8 CounterUdb;         /* Current Counter Value */

    #if (!BLDCEncoder_1_PositionCounter_ControlRegRemoved)
        uint8 CounterControlRegister;               /* Counter Control Register */
    #endif /* (!BLDCEncoder_1_PositionCounter_ControlRegRemoved) */

}BLDCEncoder_1_PositionCounter_backupStruct;


/**************************************
 *  Function Prototypes
 *************************************/
void    BLDCEncoder_1_PositionCounter_Start(void) ;
void    BLDCEncoder_1_PositionCounter_Stop(void) ;
void    BLDCEncoder_1_PositionCounter_SetInterruptMode(uint8 interruptsMask) ;
uint8   BLDCEncoder_1_PositionCounter_ReadStatusRegister(void) ;
#define BLDCEncoder_1_PositionCounter_GetInterruptSource() BLDCEncoder_1_PositionCounter_ReadStatusRegister()
#if(!BLDCEncoder_1_PositionCounter_ControlRegRemoved)
    uint8   BLDCEncoder_1_PositionCounter_ReadControlRegister(void) ;
    void    BLDCEncoder_1_PositionCounter_WriteControlRegister(uint8 control) \
        ;
#endif /* (!BLDCEncoder_1_PositionCounter_ControlRegRemoved) */
#if (!(BLDCEncoder_1_PositionCounter_UsingFixedFunction && (CY_PSOC5A)))
    void    BLDCEncoder_1_PositionCounter_WriteCounter(uint8 counter) \
            ; 
#endif /* (!(BLDCEncoder_1_PositionCounter_UsingFixedFunction && (CY_PSOC5A))) */
uint8  BLDCEncoder_1_PositionCounter_ReadCounter(void) ;
uint8  BLDCEncoder_1_PositionCounter_ReadCapture(void) ;
void    BLDCEncoder_1_PositionCounter_WritePeriod(uint8 period) \
    ;
uint8  BLDCEncoder_1_PositionCounter_ReadPeriod( void ) ;
#if (!BLDCEncoder_1_PositionCounter_UsingFixedFunction)
    void    BLDCEncoder_1_PositionCounter_WriteCompare(uint8 compare) \
        ;
    uint8  BLDCEncoder_1_PositionCounter_ReadCompare( void ) \
        ;
#endif /* (!BLDCEncoder_1_PositionCounter_UsingFixedFunction) */

#if (BLDCEncoder_1_PositionCounter_COMPARE_MODE_SOFTWARE)
    void    BLDCEncoder_1_PositionCounter_SetCompareMode(uint8 compareMode) ;
#endif /* (BLDCEncoder_1_PositionCounter_COMPARE_MODE_SOFTWARE) */
#if (BLDCEncoder_1_PositionCounter_CAPTURE_MODE_SOFTWARE)
    void    BLDCEncoder_1_PositionCounter_SetCaptureMode(uint8 captureMode) ;
#endif /* (BLDCEncoder_1_PositionCounter_CAPTURE_MODE_SOFTWARE) */
void BLDCEncoder_1_PositionCounter_ClearFIFO(void)     ;
void BLDCEncoder_1_PositionCounter_Init(void)          ;
void BLDCEncoder_1_PositionCounter_Enable(void)        ;
void BLDCEncoder_1_PositionCounter_SaveConfig(void)    ;
void BLDCEncoder_1_PositionCounter_RestoreConfig(void) ;
void BLDCEncoder_1_PositionCounter_Sleep(void)         ;
void BLDCEncoder_1_PositionCounter_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Counter__CompareModes, Used in Compare Mode retained for backward compatibility of tests*/
#define BLDCEncoder_1_PositionCounter__B_COUNTER__LESS_THAN 1
#define BLDCEncoder_1_PositionCounter__B_COUNTER__LESS_THAN_OR_EQUAL 2
#define BLDCEncoder_1_PositionCounter__B_COUNTER__EQUAL 0
#define BLDCEncoder_1_PositionCounter__B_COUNTER__GREATER_THAN 3
#define BLDCEncoder_1_PositionCounter__B_COUNTER__GREATER_THAN_OR_EQUAL 4
#define BLDCEncoder_1_PositionCounter__B_COUNTER__SOFTWARE 5

/* Enumerated Type Counter_CompareModes */
#define BLDCEncoder_1_PositionCounter_CMP_MODE_LT 1u
#define BLDCEncoder_1_PositionCounter_CMP_MODE_LTE 2u
#define BLDCEncoder_1_PositionCounter_CMP_MODE_EQ 0u
#define BLDCEncoder_1_PositionCounter_CMP_MODE_GT 3u
#define BLDCEncoder_1_PositionCounter_CMP_MODE_GTE 4u
#define BLDCEncoder_1_PositionCounter_CMP_MODE_SOFTWARE_CONTROLLED 5u

/* Enumerated Type B_Counter__CaptureModes, Used in Capture Mode retained for backward compatibility of tests*/
#define BLDCEncoder_1_PositionCounter__B_COUNTER__NONE 0
#define BLDCEncoder_1_PositionCounter__B_COUNTER__RISING_EDGE 1
#define BLDCEncoder_1_PositionCounter__B_COUNTER__FALLING_EDGE 2
#define BLDCEncoder_1_PositionCounter__B_COUNTER__EITHER_EDGE 3
#define BLDCEncoder_1_PositionCounter__B_COUNTER__SOFTWARE_CONTROL 4

/* Enumerated Type Counter_CompareModes */
#define BLDCEncoder_1_PositionCounter_CAP_MODE_NONE 0u
#define BLDCEncoder_1_PositionCounter_CAP_MODE_RISE 1u
#define BLDCEncoder_1_PositionCounter_CAP_MODE_FALL 2u
#define BLDCEncoder_1_PositionCounter_CAP_MODE_BOTH 3u
#define BLDCEncoder_1_PositionCounter_CAP_MODE_SOFTWARE_CONTROLLED 4u


/***************************************
 *  Initialization Values
 **************************************/
#define BLDCEncoder_1_PositionCounter_CAPTURE_MODE_CONF       0u
#define BLDCEncoder_1_PositionCounter_INIT_PERIOD_VALUE       255u
#define BLDCEncoder_1_PositionCounter_INIT_COUNTER_VALUE      255u
#if (BLDCEncoder_1_PositionCounter_UsingFixedFunction)
#define BLDCEncoder_1_PositionCounter_INIT_INTERRUPTS_MASK    ((uint8)((uint8)0u << BLDCEncoder_1_PositionCounter_STATUS_ZERO_INT_EN_MASK_SHIFT))
#else
#define BLDCEncoder_1_PositionCounter_INIT_COMPARE_VALUE      255u
#define BLDCEncoder_1_PositionCounter_INIT_INTERRUPTS_MASK ((uint8)((uint8)0u << BLDCEncoder_1_PositionCounter_STATUS_ZERO_INT_EN_MASK_SHIFT) | \
        ((uint8)((uint8)0u << BLDCEncoder_1_PositionCounter_STATUS_CAPTURE_INT_EN_MASK_SHIFT)) | \
        ((uint8)((uint8)0u << BLDCEncoder_1_PositionCounter_STATUS_CMP_INT_EN_MASK_SHIFT)) | \
        ((uint8)((uint8)0u << BLDCEncoder_1_PositionCounter_STATUS_OVERFLOW_INT_EN_MASK_SHIFT)) | \
        ((uint8)((uint8)0u << BLDCEncoder_1_PositionCounter_STATUS_UNDERFLOW_INT_EN_MASK_SHIFT)))
#define BLDCEncoder_1_PositionCounter_DEFAULT_COMPARE_MODE    1u

#if( 0 != BLDCEncoder_1_PositionCounter_CAPTURE_MODE_CONF)
    #define BLDCEncoder_1_PositionCounter_DEFAULT_CAPTURE_MODE    ((uint8)((uint8)0u << BLDCEncoder_1_PositionCounter_CTRL_CAPMODE0_SHIFT))
#else    
    #define BLDCEncoder_1_PositionCounter_DEFAULT_CAPTURE_MODE    (0u )
#endif /* ( 0 != BLDCEncoder_1_PositionCounter_CAPTURE_MODE_CONF) */

#endif /* (BLDCEncoder_1_PositionCounter_UsingFixedFunction) */


/**************************************
 *  Registers
 *************************************/
#if (BLDCEncoder_1_PositionCounter_UsingFixedFunction)
    #define BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB     (*(reg16 *) BLDCEncoder_1_PositionCounter_CounterHW__CAP0 )
    #define BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB_PTR ( (reg16 *) BLDCEncoder_1_PositionCounter_CounterHW__CAP0 )
    #define BLDCEncoder_1_PositionCounter_PERIOD_LSB          (*(reg16 *) BLDCEncoder_1_PositionCounter_CounterHW__PER0 )
    #define BLDCEncoder_1_PositionCounter_PERIOD_LSB_PTR      ( (reg16 *) BLDCEncoder_1_PositionCounter_CounterHW__PER0 )
    /* MODE must be set to 1 to set the compare value */
    #define BLDCEncoder_1_PositionCounter_COMPARE_LSB         (*(reg16 *) BLDCEncoder_1_PositionCounter_CounterHW__CNT_CMP0 )
    #define BLDCEncoder_1_PositionCounter_COMPARE_LSB_PTR     ( (reg16 *) BLDCEncoder_1_PositionCounter_CounterHW__CNT_CMP0 )
    /* MODE must be set to 0 to get the count */
    #define BLDCEncoder_1_PositionCounter_COUNTER_LSB         (*(reg16 *) BLDCEncoder_1_PositionCounter_CounterHW__CNT_CMP0 )
    #define BLDCEncoder_1_PositionCounter_COUNTER_LSB_PTR     ( (reg16 *) BLDCEncoder_1_PositionCounter_CounterHW__CNT_CMP0 )
    #define BLDCEncoder_1_PositionCounter_RT1                 (*(reg8 *) BLDCEncoder_1_PositionCounter_CounterHW__RT1)
    #define BLDCEncoder_1_PositionCounter_RT1_PTR             ( (reg8 *) BLDCEncoder_1_PositionCounter_CounterHW__RT1)
#else
    
    #if (BLDCEncoder_1_PositionCounter_Resolution <= 8u) /* 8-bit Counter */
    
        #define BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB     (*(reg8 *) \
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__F0_REG )
        #define BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB_PTR ( (reg8 *) \
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__F0_REG )
        #define BLDCEncoder_1_PositionCounter_PERIOD_LSB          (*(reg8 *) \
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D0_REG )
        #define BLDCEncoder_1_PositionCounter_PERIOD_LSB_PTR      ( (reg8 *) \
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D0_REG )
        #define BLDCEncoder_1_PositionCounter_COMPARE_LSB         (*(reg8 *) \
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D1_REG )
        #define BLDCEncoder_1_PositionCounter_COMPARE_LSB_PTR     ( (reg8 *) \
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D1_REG )
        #define BLDCEncoder_1_PositionCounter_COUNTER_LSB         (*(reg8 *) \
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__A0_REG )  
        #define BLDCEncoder_1_PositionCounter_COUNTER_LSB_PTR     ( (reg8 *)\
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__A0_REG )
    
    #elif(BLDCEncoder_1_PositionCounter_Resolution <= 16u) /* 16-bit Counter */
        #if(CY_PSOC3) /* 8-bit address space */ 
            #define BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB     (*(reg16 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__F0_REG )
            #define BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB_PTR ( (reg16 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__F0_REG )
            #define BLDCEncoder_1_PositionCounter_PERIOD_LSB          (*(reg16 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D0_REG )
            #define BLDCEncoder_1_PositionCounter_PERIOD_LSB_PTR      ( (reg16 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D0_REG )
            #define BLDCEncoder_1_PositionCounter_COMPARE_LSB         (*(reg16 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D1_REG )
            #define BLDCEncoder_1_PositionCounter_COMPARE_LSB_PTR     ( (reg16 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D1_REG )
            #define BLDCEncoder_1_PositionCounter_COUNTER_LSB         (*(reg16 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__A0_REG )  
            #define BLDCEncoder_1_PositionCounter_COUNTER_LSB_PTR     ( (reg16 *)\
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB     (*(reg16 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__16BIT_F0_REG )
            #define BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB_PTR ( (reg16 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__16BIT_F0_REG )
            #define BLDCEncoder_1_PositionCounter_PERIOD_LSB          (*(reg16 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__16BIT_D0_REG )
            #define BLDCEncoder_1_PositionCounter_PERIOD_LSB_PTR      ( (reg16 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__16BIT_D0_REG )
            #define BLDCEncoder_1_PositionCounter_COMPARE_LSB         (*(reg16 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__16BIT_D1_REG )
            #define BLDCEncoder_1_PositionCounter_COMPARE_LSB_PTR     ( (reg16 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__16BIT_D1_REG )
            #define BLDCEncoder_1_PositionCounter_COUNTER_LSB         (*(reg16 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__16BIT_A0_REG )  
            #define BLDCEncoder_1_PositionCounter_COUNTER_LSB_PTR     ( (reg16 *)\
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */   
    #elif(BLDCEncoder_1_PositionCounter_Resolution <= 24u) /* 24-bit Counter */
        
        #define BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB     (*(reg32 *) \
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__F0_REG )
        #define BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB_PTR ( (reg32 *) \
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__F0_REG )
        #define BLDCEncoder_1_PositionCounter_PERIOD_LSB          (*(reg32 *) \
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D0_REG )
        #define BLDCEncoder_1_PositionCounter_PERIOD_LSB_PTR      ( (reg32 *) \
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D0_REG )
        #define BLDCEncoder_1_PositionCounter_COMPARE_LSB         (*(reg32 *) \
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D1_REG )
        #define BLDCEncoder_1_PositionCounter_COMPARE_LSB_PTR     ( (reg32 *) \
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D1_REG )
        #define BLDCEncoder_1_PositionCounter_COUNTER_LSB         (*(reg32 *) \
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__A0_REG )  
        #define BLDCEncoder_1_PositionCounter_COUNTER_LSB_PTR     ( (reg32 *)\
            BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__A0_REG )
    
    #else /* 32-bit Counter */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB     (*(reg32 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__F0_REG )
            #define BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB_PTR ( (reg32 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__F0_REG )
            #define BLDCEncoder_1_PositionCounter_PERIOD_LSB          (*(reg32 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D0_REG )
            #define BLDCEncoder_1_PositionCounter_PERIOD_LSB_PTR      ( (reg32 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D0_REG )
            #define BLDCEncoder_1_PositionCounter_COMPARE_LSB         (*(reg32 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D1_REG )
            #define BLDCEncoder_1_PositionCounter_COMPARE_LSB_PTR     ( (reg32 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__D1_REG )
            #define BLDCEncoder_1_PositionCounter_COUNTER_LSB         (*(reg32 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__A0_REG )  
            #define BLDCEncoder_1_PositionCounter_COUNTER_LSB_PTR     ( (reg32 *)\
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB     (*(reg32 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__32BIT_F0_REG )
            #define BLDCEncoder_1_PositionCounter_STATICCOUNT_LSB_PTR ( (reg32 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__32BIT_F0_REG )
            #define BLDCEncoder_1_PositionCounter_PERIOD_LSB          (*(reg32 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__32BIT_D0_REG )
            #define BLDCEncoder_1_PositionCounter_PERIOD_LSB_PTR      ( (reg32 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__32BIT_D0_REG )
            #define BLDCEncoder_1_PositionCounter_COMPARE_LSB         (*(reg32 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__32BIT_D1_REG )
            #define BLDCEncoder_1_PositionCounter_COMPARE_LSB_PTR     ( (reg32 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__32BIT_D1_REG )
            #define BLDCEncoder_1_PositionCounter_COUNTER_LSB         (*(reg32 *) \
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__32BIT_A0_REG )  
            #define BLDCEncoder_1_PositionCounter_COUNTER_LSB_PTR     ( (reg32 *)\
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */   
    #endif

	#define BLDCEncoder_1_PositionCounter_COUNTER_LSB_PTR_8BIT     ( (reg8 *)\
                BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__A0_REG )
				
    #define BLDCEncoder_1_PositionCounter_AUX_CONTROLDP0 \
        (*(reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__DP_AUX_CTL_REG)
    
    #define BLDCEncoder_1_PositionCounter_AUX_CONTROLDP0_PTR \
        ( (reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u0__DP_AUX_CTL_REG)
    
    #if (BLDCEncoder_1_PositionCounter_Resolution == 16 || BLDCEncoder_1_PositionCounter_Resolution == 24 || BLDCEncoder_1_PositionCounter_Resolution == 32)
       #define BLDCEncoder_1_PositionCounter_AUX_CONTROLDP1 \
           (*(reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u1__DP_AUX_CTL_REG)
       #define BLDCEncoder_1_PositionCounter_AUX_CONTROLDP1_PTR \
           ( (reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u1__DP_AUX_CTL_REG)
    #endif /* (BLDCEncoder_1_PositionCounter_Resolution == 16 || BLDCEncoder_1_PositionCounter_Resolution == 24 || BLDCEncoder_1_PositionCounter_Resolution == 32) */
    
    #if (BLDCEncoder_1_PositionCounter_Resolution == 24 || BLDCEncoder_1_PositionCounter_Resolution == 32)
       #define BLDCEncoder_1_PositionCounter_AUX_CONTROLDP2 \
           (*(reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u2__DP_AUX_CTL_REG)
       #define BLDCEncoder_1_PositionCounter_AUX_CONTROLDP2_PTR \
           ( (reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u2__DP_AUX_CTL_REG)
    #endif /* (BLDCEncoder_1_PositionCounter_Resolution == 24 || BLDCEncoder_1_PositionCounter_Resolution == 32) */
    
    #if (BLDCEncoder_1_PositionCounter_Resolution == 32)
       #define BLDCEncoder_1_PositionCounter_AUX_CONTROLDP3 \
           (*(reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u3__DP_AUX_CTL_REG)
       #define BLDCEncoder_1_PositionCounter_AUX_CONTROLDP3_PTR \
           ( (reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sC8_counterdp_u3__DP_AUX_CTL_REG)
    #endif /* (BLDCEncoder_1_PositionCounter_Resolution == 32) */

#endif  /* (BLDCEncoder_1_PositionCounter_UsingFixedFunction) */

#if (BLDCEncoder_1_PositionCounter_UsingFixedFunction)
    #define BLDCEncoder_1_PositionCounter_STATUS         (*(reg8 *) BLDCEncoder_1_PositionCounter_CounterHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define BLDCEncoder_1_PositionCounter_STATUS_MASK             (*(reg8 *) BLDCEncoder_1_PositionCounter_CounterHW__SR0 )
    #define BLDCEncoder_1_PositionCounter_STATUS_MASK_PTR         ( (reg8 *) BLDCEncoder_1_PositionCounter_CounterHW__SR0 )
    #define BLDCEncoder_1_PositionCounter_CONTROL                 (*(reg8 *) BLDCEncoder_1_PositionCounter_CounterHW__CFG0)
    #define BLDCEncoder_1_PositionCounter_CONTROL_PTR             ( (reg8 *) BLDCEncoder_1_PositionCounter_CounterHW__CFG0)
    #define BLDCEncoder_1_PositionCounter_CONTROL2                (*(reg8 *) BLDCEncoder_1_PositionCounter_CounterHW__CFG1)
    #define BLDCEncoder_1_PositionCounter_CONTROL2_PTR            ( (reg8 *) BLDCEncoder_1_PositionCounter_CounterHW__CFG1)
    #if (CY_PSOC3 || CY_PSOC5LP)
        #define BLDCEncoder_1_PositionCounter_CONTROL3       (*(reg8 *) BLDCEncoder_1_PositionCounter_CounterHW__CFG2)
        #define BLDCEncoder_1_PositionCounter_CONTROL3_PTR   ( (reg8 *) BLDCEncoder_1_PositionCounter_CounterHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define BLDCEncoder_1_PositionCounter_GLOBAL_ENABLE           (*(reg8 *) BLDCEncoder_1_PositionCounter_CounterHW__PM_ACT_CFG)
    #define BLDCEncoder_1_PositionCounter_GLOBAL_ENABLE_PTR       ( (reg8 *) BLDCEncoder_1_PositionCounter_CounterHW__PM_ACT_CFG)
    #define BLDCEncoder_1_PositionCounter_GLOBAL_STBY_ENABLE      (*(reg8 *) BLDCEncoder_1_PositionCounter_CounterHW__PM_STBY_CFG)
    #define BLDCEncoder_1_PositionCounter_GLOBAL_STBY_ENABLE_PTR  ( (reg8 *) BLDCEncoder_1_PositionCounter_CounterHW__PM_STBY_CFG)
    

    /********************************
    *    Constants
    ********************************/

    /* Fixed Function Block Chosen */
    #define BLDCEncoder_1_PositionCounter_BLOCK_EN_MASK          BLDCEncoder_1_PositionCounter_CounterHW__PM_ACT_MSK
    #define BLDCEncoder_1_PositionCounter_BLOCK_STBY_EN_MASK     BLDCEncoder_1_PositionCounter_CounterHW__PM_STBY_MSK 
    
    /* Control Register Bit Locations */    
    /* As defined in Register Map, part of TMRX_CFG0 register */
    #define BLDCEncoder_1_PositionCounter_CTRL_ENABLE_SHIFT      0x00u
    #define BLDCEncoder_1_PositionCounter_ONESHOT_SHIFT          0x02u
    /* Control Register Bit Masks */
    #define BLDCEncoder_1_PositionCounter_CTRL_ENABLE            ((uint8)((uint8)0x01u << BLDCEncoder_1_PositionCounter_CTRL_ENABLE_SHIFT))         
    #define BLDCEncoder_1_PositionCounter_ONESHOT                ((uint8)((uint8)0x01u << BLDCEncoder_1_PositionCounter_ONESHOT_SHIFT))

    /* Control2 Register Bit Masks */
    /* Set the mask for run mode */
    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        #define BLDCEncoder_1_PositionCounter_CTRL_MODE_SHIFT        0x01u    
        #define BLDCEncoder_1_PositionCounter_CTRL_MODE_MASK         ((uint8)((uint8)0x07u << BLDCEncoder_1_PositionCounter_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Use CFG2 Mode bits to set run mode */
        #define BLDCEncoder_1_PositionCounter_CTRL_MODE_SHIFT        0x00u    
        #define BLDCEncoder_1_PositionCounter_CTRL_MODE_MASK         ((uint8)((uint8)0x03u << BLDCEncoder_1_PositionCounter_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    /* Set the mask for interrupt (raw/status register) */
    #define BLDCEncoder_1_PositionCounter_CTRL2_IRQ_SEL_SHIFT     0x00u
    #define BLDCEncoder_1_PositionCounter_CTRL2_IRQ_SEL          ((uint8)((uint8)0x01u << BLDCEncoder_1_PositionCounter_CTRL2_IRQ_SEL_SHIFT))     
    
    /* Status Register Bit Locations */
    #define BLDCEncoder_1_PositionCounter_STATUS_ZERO_SHIFT      0x07u  /* As defined in Register Map, part of TMRX_SR0 register */ 

    /* Status Register Interrupt Enable Bit Locations */
    #define BLDCEncoder_1_PositionCounter_STATUS_ZERO_INT_EN_MASK_SHIFT      (BLDCEncoder_1_PositionCounter_STATUS_ZERO_SHIFT - 0x04u)

    /* Status Register Bit Masks */                           
    #define BLDCEncoder_1_PositionCounter_STATUS_ZERO            ((uint8)((uint8)0x01u << BLDCEncoder_1_PositionCounter_STATUS_ZERO_SHIFT))

    /* Status Register Interrupt Bit Masks*/
    #define BLDCEncoder_1_PositionCounter_STATUS_ZERO_INT_EN_MASK       ((uint8)((uint8)BLDCEncoder_1_PositionCounter_STATUS_ZERO >> 0x04u))
    
    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define BLDCEncoder_1_PositionCounter_RT1_SHIFT            0x04u
    #define BLDCEncoder_1_PositionCounter_RT1_MASK             ((uint8)((uint8)0x03u << BLDCEncoder_1_PositionCounter_RT1_SHIFT))  /* Sync TC and CMP bit masks */
    #define BLDCEncoder_1_PositionCounter_SYNC                 ((uint8)((uint8)0x03u << BLDCEncoder_1_PositionCounter_RT1_SHIFT))
    #define BLDCEncoder_1_PositionCounter_SYNCDSI_SHIFT        0x00u
    #define BLDCEncoder_1_PositionCounter_SYNCDSI_MASK         ((uint8)((uint8)0x0Fu << BLDCEncoder_1_PositionCounter_SYNCDSI_SHIFT)) /* Sync all DSI inputs */
    #define BLDCEncoder_1_PositionCounter_SYNCDSI_EN           ((uint8)((uint8)0x0Fu << BLDCEncoder_1_PositionCounter_SYNCDSI_SHIFT)) /* Sync all DSI inputs */
    
#else /* !BLDCEncoder_1_PositionCounter_UsingFixedFunction */
    #define BLDCEncoder_1_PositionCounter_STATUS               (* (reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sSTSReg_stsreg__STATUS_REG )
    #define BLDCEncoder_1_PositionCounter_STATUS_PTR           (  (reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sSTSReg_stsreg__STATUS_REG )
    #define BLDCEncoder_1_PositionCounter_STATUS_MASK          (* (reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sSTSReg_stsreg__MASK_REG )
    #define BLDCEncoder_1_PositionCounter_STATUS_MASK_PTR      (  (reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sSTSReg_stsreg__MASK_REG )
    #define BLDCEncoder_1_PositionCounter_STATUS_AUX_CTRL      (*(reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sSTSReg_stsreg__STATUS_AUX_CTL_REG)
    #define BLDCEncoder_1_PositionCounter_STATUS_AUX_CTRL_PTR  ( (reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sSTSReg_stsreg__STATUS_AUX_CTL_REG)
    #define BLDCEncoder_1_PositionCounter_CONTROL              (* (reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sCTRLReg_ctrlreg__CONTROL_REG )
    #define BLDCEncoder_1_PositionCounter_CONTROL_PTR          (  (reg8 *) BLDCEncoder_1_PositionCounter_CounterUDB_sCTRLReg_ctrlreg__CONTROL_REG )


    /********************************
    *    Constants
    ********************************/
    /* Control Register Bit Locations */
    #define BLDCEncoder_1_PositionCounter_CTRL_CAPMODE0_SHIFT    0x03u       /* As defined by Verilog Implementation */
    #define BLDCEncoder_1_PositionCounter_CTRL_RESET_SHIFT       0x06u       /* As defined by Verilog Implementation */
    #define BLDCEncoder_1_PositionCounter_CTRL_ENABLE_SHIFT      0x07u       /* As defined by Verilog Implementation */
    /* Control Register Bit Masks */
    #define BLDCEncoder_1_PositionCounter_CTRL_CMPMODE_MASK      0x07u 
    #define BLDCEncoder_1_PositionCounter_CTRL_CAPMODE_MASK      0x03u  
    #define BLDCEncoder_1_PositionCounter_CTRL_RESET             ((uint8)((uint8)0x01u << BLDCEncoder_1_PositionCounter_CTRL_RESET_SHIFT))  
    #define BLDCEncoder_1_PositionCounter_CTRL_ENABLE            ((uint8)((uint8)0x01u << BLDCEncoder_1_PositionCounter_CTRL_ENABLE_SHIFT)) 

    /* Status Register Bit Locations */
    #define BLDCEncoder_1_PositionCounter_STATUS_CMP_SHIFT       0x00u       /* As defined by Verilog Implementation */
    #define BLDCEncoder_1_PositionCounter_STATUS_ZERO_SHIFT      0x01u       /* As defined by Verilog Implementation */
    #define BLDCEncoder_1_PositionCounter_STATUS_OVERFLOW_SHIFT  0x02u       /* As defined by Verilog Implementation */
    #define BLDCEncoder_1_PositionCounter_STATUS_UNDERFLOW_SHIFT 0x03u       /* As defined by Verilog Implementation */
    #define BLDCEncoder_1_PositionCounter_STATUS_CAPTURE_SHIFT   0x04u       /* As defined by Verilog Implementation */
    #define BLDCEncoder_1_PositionCounter_STATUS_FIFOFULL_SHIFT  0x05u       /* As defined by Verilog Implementation */
    #define BLDCEncoder_1_PositionCounter_STATUS_FIFONEMP_SHIFT  0x06u       /* As defined by Verilog Implementation */
    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define BLDCEncoder_1_PositionCounter_STATUS_CMP_INT_EN_MASK_SHIFT       BLDCEncoder_1_PositionCounter_STATUS_CMP_SHIFT       
    #define BLDCEncoder_1_PositionCounter_STATUS_ZERO_INT_EN_MASK_SHIFT      BLDCEncoder_1_PositionCounter_STATUS_ZERO_SHIFT      
    #define BLDCEncoder_1_PositionCounter_STATUS_OVERFLOW_INT_EN_MASK_SHIFT  BLDCEncoder_1_PositionCounter_STATUS_OVERFLOW_SHIFT  
    #define BLDCEncoder_1_PositionCounter_STATUS_UNDERFLOW_INT_EN_MASK_SHIFT BLDCEncoder_1_PositionCounter_STATUS_UNDERFLOW_SHIFT 
    #define BLDCEncoder_1_PositionCounter_STATUS_CAPTURE_INT_EN_MASK_SHIFT   BLDCEncoder_1_PositionCounter_STATUS_CAPTURE_SHIFT   
    #define BLDCEncoder_1_PositionCounter_STATUS_FIFOFULL_INT_EN_MASK_SHIFT  BLDCEncoder_1_PositionCounter_STATUS_FIFOFULL_SHIFT  
    #define BLDCEncoder_1_PositionCounter_STATUS_FIFONEMP_INT_EN_MASK_SHIFT  BLDCEncoder_1_PositionCounter_STATUS_FIFONEMP_SHIFT  
    /* Status Register Bit Masks */                
    #define BLDCEncoder_1_PositionCounter_STATUS_CMP             ((uint8)((uint8)0x01u << BLDCEncoder_1_PositionCounter_STATUS_CMP_SHIFT))  
    #define BLDCEncoder_1_PositionCounter_STATUS_ZERO            ((uint8)((uint8)0x01u << BLDCEncoder_1_PositionCounter_STATUS_ZERO_SHIFT)) 
    #define BLDCEncoder_1_PositionCounter_STATUS_OVERFLOW        ((uint8)((uint8)0x01u << BLDCEncoder_1_PositionCounter_STATUS_OVERFLOW_SHIFT)) 
    #define BLDCEncoder_1_PositionCounter_STATUS_UNDERFLOW       ((uint8)((uint8)0x01u << BLDCEncoder_1_PositionCounter_STATUS_UNDERFLOW_SHIFT)) 
    #define BLDCEncoder_1_PositionCounter_STATUS_CAPTURE         ((uint8)((uint8)0x01u << BLDCEncoder_1_PositionCounter_STATUS_CAPTURE_SHIFT)) 
    #define BLDCEncoder_1_PositionCounter_STATUS_FIFOFULL        ((uint8)((uint8)0x01u << BLDCEncoder_1_PositionCounter_STATUS_FIFOFULL_SHIFT))
    #define BLDCEncoder_1_PositionCounter_STATUS_FIFONEMP        ((uint8)((uint8)0x01u << BLDCEncoder_1_PositionCounter_STATUS_FIFONEMP_SHIFT))
    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define BLDCEncoder_1_PositionCounter_STATUS_CMP_INT_EN_MASK            BLDCEncoder_1_PositionCounter_STATUS_CMP                    
    #define BLDCEncoder_1_PositionCounter_STATUS_ZERO_INT_EN_MASK           BLDCEncoder_1_PositionCounter_STATUS_ZERO            
    #define BLDCEncoder_1_PositionCounter_STATUS_OVERFLOW_INT_EN_MASK       BLDCEncoder_1_PositionCounter_STATUS_OVERFLOW        
    #define BLDCEncoder_1_PositionCounter_STATUS_UNDERFLOW_INT_EN_MASK      BLDCEncoder_1_PositionCounter_STATUS_UNDERFLOW       
    #define BLDCEncoder_1_PositionCounter_STATUS_CAPTURE_INT_EN_MASK        BLDCEncoder_1_PositionCounter_STATUS_CAPTURE         
    #define BLDCEncoder_1_PositionCounter_STATUS_FIFOFULL_INT_EN_MASK       BLDCEncoder_1_PositionCounter_STATUS_FIFOFULL        
    #define BLDCEncoder_1_PositionCounter_STATUS_FIFONEMP_INT_EN_MASK       BLDCEncoder_1_PositionCounter_STATUS_FIFONEMP         
    

    /* StatusI Interrupt Enable bit Location in the Auxilliary Control Register */
    #define BLDCEncoder_1_PositionCounter_STATUS_ACTL_INT_EN     0x10u /* As defined for the ACTL Register */
    
    /* Datapath Auxillary Control Register definitions */
    #define BLDCEncoder_1_PositionCounter_AUX_CTRL_FIFO0_CLR         0x01u   /* As defined by Register map */
    #define BLDCEncoder_1_PositionCounter_AUX_CTRL_FIFO1_CLR         0x02u   /* As defined by Register map */
    #define BLDCEncoder_1_PositionCounter_AUX_CTRL_FIFO0_LVL         0x04u   /* As defined by Register map */
    #define BLDCEncoder_1_PositionCounter_AUX_CTRL_FIFO1_LVL         0x08u   /* As defined by Register map */
    #define BLDCEncoder_1_PositionCounter_STATUS_ACTL_INT_EN_MASK    0x10u   /* As defined for the ACTL Register */
    
#endif /* BLDCEncoder_1_PositionCounter_UsingFixedFunction */

#endif  /* CY_COUNTER_BLDCEncoder_1_PositionCounter_H */


/* [] END OF FILE */


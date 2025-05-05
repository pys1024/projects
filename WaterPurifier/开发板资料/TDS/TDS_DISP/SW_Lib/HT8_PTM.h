/**
  ******************************************************************************
  * @file HT8_PTM.h
  * @brief The header file of the PTM library.
  * @author Holtek Semiconductor inc.
  * @version V1.0.0
  * @date 2020-06-15
  ******************************************************************************
  * @attention
  *
  * Firmware Disclaimer Information
  *
  * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
  *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
  *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
  *    other intellectual property laws.
  *
  * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
  *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
  *    other than HOLTEK and the customer.
  *
  * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
  *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
  *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
  *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
  *
  * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
  ************************************************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef _PTM_H_
#define _PTM_H_

#include "HT8_MCU_IP_SEL.h"

#define	PTM_ENABLE()			(_pton = 1)
#define	PTM_DISABLE()			(_pton = 0)
#define	PTM_CCRA_ISR_ENABLE()	(_ptmae = 1)
#define	PTM_CCRA_ISR_DISABLE()	(_ptmae = 0)
#define PTM_SET_FLAG_A()		(_ptmaf = 1)
#define PTM_CLEAR_FLAG_A()		(_ptmaf = 0)
#define	PTM_CCRP_ISR_ENABLE()	(_ptmpe = 1)
#define	PTM_CCRP_ISR_DISABLE()	(_ptmpe = 0)
#define PTM_SET_FLAG_P()		(_ptmpf = 1)
#define PTM_CLEAR_FLAG_P()		(_ptmpf = 0)
#define	PTM_PAUSE()				(_ptpau = 1)
#define	PTM_RUN()				(_ptpau = 0)
#define	PTM_GET_CCRA_FLAG()		_ptmaf
#define	PTM_GET_CCRP_FLAG()		_ptmpf


/******** PTM work mode select ********/

//	#define	PTM_TIMER_COUNTER_MODE			1
//	#define	PTM_PWM_OUTPUT_MODE				1	
//	#define	PTM_COMPARE_MATCH_MODE			1
//	#define	PTM_SINGLE_PULSE_OUTPUT_MODE	1

/**** end of PTM work mode select *****/


/************* clock select **************/

//	#define		PTM_FSYS_DIV4			1
//	#define		PTM_FSYS				1
//	#define		PTM_FH_DIV16			1
//	#define		PTM_FH_DIV64			1
//	#define		PTM_FSUB				1
//	#define		PTM_TCK_RISING_EDGE	1
//	#define		PTM_TCK_FALLING_EDGE	1

/********* end of clock select *********/
	
/** TMn Counter Clear condition selection **/
	
//	#define		PTM_CCRA_MATCH			1
//	#define		PTM_CCRP_MATCH			1	

/* end of TMn Counter Clear condition selection */


	
/****** PWM OUTPUT MODE setting ******/
	
#ifdef	PTM_PWM_OUTPUT_MODE		
/* PTP Output control */
//	#define		PTM_ACTIVE_LOW	 			1
//	#define		PTM_ACTIVE_HIGH				1
/* end of PTP Output control */
	
/***** PTP Output polarity control *****/
//	#define		PTM_NON_INVERTED			1
//	#define		PTM_INVERTED				1
/* end of PTP Output polarity control */

#endif

/* end of PWM OUTPUT MODE setting */
///////////////////////////////////////////////////////


/****** SINGLE PULSE OUTPUT MODE setting ******/
		
#ifdef	PTM_SINGLE_PULSE_OUTPUT_MODE		
/* PTP Output control */
//	#define		PTM_ACTIVE_LOW				1
//	#define		PTM_ACTIVE_HIGH				1
/* end of PTP Output control */
	
/***** PTP Output polarity control *****/
//	#define		PTM_NON_INVERTED			1
//	#define		PTM_INVERTED				1
/* end of PTP Output polarity control */			
#endif

/* end of SINGLE PULSE OUTPUT MODE setting */
//////////////////////////////////////////////////////


/* TIMER COUNTER MODE setting */

#ifdef	PTM_TIMER_COUNTER_MODE

	#define	PTM_GET_COUNTER_VALUE()	(_ptmdh<<8 | _ptmdl)

#endif
/* end of TIMER COUNTER MODE setting */

///////////////////////////////////////////////////////	


/* COMPARE MATCH MODE setting */

#ifdef	PTM_COMPARE_MATCH_MODE	
	/* Select PTP pin output function */
//	#define		PTM_NO_CHANGE				1	/* No change */
//	#define		PTM_OUTPUT_LOW				1	/* Output low */
//	#define		PTM_OUTPUT_HIGH				1	/* Output high */
//	#define		PTM_OUTPUT_TOGGLE			1	/* Toggle output */
/* end of Select PTP pin output function */
	
/* PTP Output initialization status control */
//	#define		PTM_INITIAL_LOW				1	/* Initial low */
//	#define		PTM_INITIAL_HIGH			1	/* Initial high */
/* end of PTP Output control */
	
/***** PTP Output polarity control *****/
//	#define		PTM_NON_INVERTED			1
//	#define		PTM_INVERTED				1
/* end of PTP Output polarity control */	
#endif

/* end of COMPARE MATCH MODE setting */
//////////////////////////////////////////////////////


void PTM_Init(void);
void PTM_PwmOutputConfig(void);
void PTM_PwmUpdate(u16 TempCCRA,u16 TempCCRP);
void PTM_SinglePulseOutputConfig(void);
void PTM_SinglePulseUpdate(u16 TempCCRA);
void PTM_CounterModeConfig(u16 TempPeriod);
void PTM_CompareMatchOutputConfig();
void PTM_CompareMatchOutputUpdate(u16 TempMatchTime);

#endif
/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/
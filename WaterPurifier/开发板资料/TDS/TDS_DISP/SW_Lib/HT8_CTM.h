/**
  ******************************************************************************
  * @file HT8_CTM.h
  * @brief The header file of the CTM library.
  * @author Holtek Semiconductor Inc.
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

#ifndef _CTM_H_
#define _CTM_H_

#include "HT8_MCU_IP_SEL.h"

#define	CTM_ENABLE()			(_cton = 1)
#define	CTM_DISABLE()			(_cton = 0)

#define	CTM_CCRA_ISR_ENABLE()	(_ctmae = 1)
#define	CTM_CCRA_ISR_DISABLE()	(_ctmae = 0)
#define CTM_SET_FLAG_A()		(_ctmaf = 1)
#define CTM_CLEAR_FLAG_A()		(_ctmaf = 0)

#define	CTM_CCRP_ISR_ENABLE()	(_ctmpe = 1)
#define	CTM_CCRP_ISR_DISABLE()	(_ctmpe = 0)
#define CTM_SET_FLAG_P()		(_ctmpf = 1)
#define CTM_CLEAR_FLAG_P()		(_ctmpf = 0)

#define	CTM_PAUSE()				(_ctpau = 1)
#define	CTM_RUN()				(_ctpau = 0)

#define	CTM_GET_CCRA_FLAG()		_ctmaf
#define	CTM_GET_CCRP_FLAG()		_ctmpf


/******** CTM work mode select ********/
	#define	CTM_TIMER_COUNTER_MODE		1
//	#define	CTM_PWM_OUTPUT_MODE			1
//	#define	CTM_COMPARE_MATCH_MODE		1
/**** end of CTM work mode select *****/


/************* clock select **************/
	#define		CTM_FSYS_DIV4			1
//	#define		CTM_FSYS				1
//	#define		CTM_FH_DIV16			1
//	#define		CTM_FH_DIV64			1
//	#define		CTM_FSUB				1
//	#define		CTM_TCK_RISING_EDGE		1
//	#define		CTM_TCK_FALLING_EDGE	1
/********* end of clock select *********/
	
/** TMn Counter Clear condition selection **/
	#define		CTM_CCRA_MATCH			1
//	#define		CTM_CCRP_MATCH			1
/* end of TMn Counter Clear condition selection */

	
/****** PWM OUTPUT MODE setting ******/
#ifdef	CTM_PWM_OUTPUT_MODE			
/**** Select CTP pin output function ****/
//	#define		CTM_OUTPUT_INACTIVE_STATE	1
//	#define		CTM_OUTPUT_ACTIVE_STATE		1
//	#define		CTM_PWM_OUTPUT				1
//	#define		CTM_UNDEFINE				1
/* end of Select CTP pin output function */
	
/* CTP Output control */
//	#define		CTM_ACTIVE_LOW				1
//	#define		CTM_ACTIVE_HIGH				1
/* end of CTP Output control */
	
/***** CTP Output polarity control *****/
//	#define		CTM_NON_INVERTED			1
//	#define		CTM_INVERTED				1
/* end of CTP Output polarity control */
	
/* CTP PWM duty/period control */
//	#define		CTM_CCRP_P_CCRA_D			1	/* CCRP ¡V period; CCRA ¡V duty */
//	#define		CTM_CCRP_D_CCRA_P			1	/* CCRP ¡V duty; CCRA ¡V period */
/* end of CTP PWM duty/period control */

#endif/* end of PWM OUTPUT MODE setting */


/* TIMER COUNTER MODE setting */
#ifdef	CTM_TIMER_COUNTER_MODE

	#define	CTM_GET_COUNTER_VALUE()		(_ctmdh<<8 | _ctmdl)

#endif/* end of TIMER COUNTER MODE setting */	


/* COMPARE MATCH MODE setting */
#ifdef	CTM_COMPARE_MATCH_MODE
/* Select CTP pin output function */
//	#define		CTM_NO_CHANGE				1	/* No change */
//	#define		CTM_OUTPUT_LOW				1	/* Output low */
//	#define		CTM_OUTPUT_HIGH				1	/* Output high */
//	#define		CTM_OUTPUT_TOGGLE			1	/* Toggle output */
/* end of Select CTP pin output function */
	
/* CTP Output initialization status control */
//	#define		CTM_INITIAL_LOW				1	/* Initial low */
//	#define		CTM_INITIAL_HIGH			1	/* Initial high */
/* end of CTP Output control */
	
/***** CTP Output polarity control *****/
//	#define		CTM_NON_INVERTED			1
//	#define		CTM_INVERTED				1
/* end of CTP Output polarity control */
#endif/* end of COMPARE MATCH MODE setting */


void CTM_Init(void);
void CTM_PwmOutputConfig(void);
void CTM_PwmUpdate(u16 TempCCRA,u8 TempCCRP);
void CTM_CounterModeConfig(u16 TempPeriod);
void CTM_CompareMatchOutputConfig();
void CTM_CompareMatchOutputUpdate(u16 TempMatchTime);


#endif

/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/

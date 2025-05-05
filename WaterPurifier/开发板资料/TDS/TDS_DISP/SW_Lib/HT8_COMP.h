/**
  ******************************************************************************
  * @file HT8_COMP.h
  * @brief The header file of the comparator library.
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

#ifndef _HT8_COMP_H_
#define _HT8_COMP_H_

#include "HT8_MCU_IP_SEL.h"


#define	COMP_ENABLE()				_cmpen = 1
#define	COMP_DISABLE()				_cmpen = 0;

#define	COMP_ISR_ENABLE()			_cpe = 1
#define	COMP_ISR_DISABLE()			_cpe = 0

#define	COMP_SET_FLAG()				_cpf = 1
#define	COMP_CLEAR_FLAG()			_cpf = 0

#define	COMP_GET_FLAG()				_cpf
	
/********** comparator output polarity select **********/	
//	#define	COMP_OUTPUT_NOT_INVERTED	(1)		//default
//	#define	COMP_OUTPUT_INVERTED		(1)
/****** end of comparator output polarity select *******/	
	
/******** Comparator Hysteresis function control *******/		
//	#define	COMP_HYSTERESIS_ENABLE		(1)		//default
//	#define	COMP_HYSTERESIS_DISABLE		(1)
/**** end of Comparator Hysteresis function control ****/
	

void COMP_Init();

#endif

/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/
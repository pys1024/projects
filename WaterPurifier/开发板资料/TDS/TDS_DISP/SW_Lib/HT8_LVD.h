/**
  ******************************************************************************
  * @file HT8_LVR.h
  * @brief The header file of the LVD library.
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

#ifndef _LVD_H_
#define _LVD_H_

#include "HT8_MCU_IP_SEL.h"

#define	LVD_ENABLE()			_lvden = 1
#define	LVD_DISABLE()			_lvden = 0
#define	LVD_ISR_ENABLE()		_lve = 1
#define	LVD_ISR_DISABLE()		_lve = 0
#define	LVD_SET_FLAG()			_lvf = 1
#define	LVD_CLEAR_FLAG()		_lvf = 0
#define	LVD_GET_FLAG()			_lvf

/************* LVD voltage select *************/
//	#define	LVD_VOLTAGE_1V8			1	//LVD voltage 1.8V
//	#define	LVD_VOLTAGE_2V0			1	//LVD voltage 2.0V
//	#define	LVD_VOLTAGE_2V4			1	//LVD voltage 2.4V
//	#define	LVD_VOLTAGE_2V7			1	//LVD voltage 2.7V
//	#define	LVD_VOLTAGE_3V0			1	//LVD voltage 3.0V
//	#define	LVD_VOLTAGE_3V3			1	//LVD voltage 3.3V
//	#define	LVD_VOLTAGE_3V6			1	//LVD voltage 3.6V
//	#define	LVD_VOLTAGE_4V0			1	//LVD voltage 4.0V
/******** end of LVD voltage select *********/

void LVD_Init();

#endif

/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/
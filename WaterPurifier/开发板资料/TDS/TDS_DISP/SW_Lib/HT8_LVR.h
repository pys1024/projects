/**
  ******************************************************************************
  * @file HT8_LVR.h
  * @brief The header file of the LVR library.
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

#ifndef _LVR_H_
#define _LVR_H_

#include "HT8_MCU_IP_SEL.h"

#define	LVR_CLEAR_FLAG()	_lvrf = 0

/************* LVR voltage select *************/
//	#define	LVR_VOLTAGE_1V7			1	//LVR voltage 1.7V
//	#define	LVR_VOLTAGE_1V9			1	//LVR voltage 1.9V
//	#define	LVR_VOLTAGE_2V55		1	//LVR voltage 2.55V
//	#define	LVR_VOLTAGE_3V15		1	//LVR voltage 3.15V
//	#define	LVR_VOLTAGE_3V8			1	//LVR voltage 3.8V
//	#define	LVR_DISABLE				1	//LVR disable
/******** end of LVR voltage select *********/

void LVR_Init();

#endif

/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/
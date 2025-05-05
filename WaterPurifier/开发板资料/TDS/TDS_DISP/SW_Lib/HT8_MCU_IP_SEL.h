/**
  ******************************************************************************
  * @file HT8_MCU_IP_SEL.h
  * @brief This file is used to selece MCU and IP function.
  * @author Holtek Semiconductor Inc.
  * @version V1.0.0
  * @date 2019-07-23
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

#ifndef _HT8_MCU_IP_SEL_H_
#define	_HT8_MCU_IP_SEL_H_

#include "HT8_Type.h"

#include "HT66F3185.h"

/************** IP select *************/
	#define	_SYS_Clock		(1)   	
	#define	_GPIO			(1)   
//	#define	_EXTI			(1)   
//	#define	_ADC			(1)   
	#define	_CTM			(1)
//	#define	_STM			(1)
//	#define	_PTM			(1)
//	#define	_SIM			(1)
	#define	_UART			(1)
//	#define	_TIMEBASE		(1)
//	#define	_WDT			(1)
//	#define	_COMP			(1)
//	#define	_LVD			(1)
//	#define	_LVR			(1)
//	#define	_EEPROM			(1)
/******** end of IP select ************/

/************** IP define *************/

/******** end of IP define ************/

#ifdef	_SYS_Clock
	#include "HT8_SYS_Clock.h"
#endif

#ifdef	_GPIO
	#include "HT8_GPIO.h"
#endif

#ifdef	_EXTI
	#include "HT8_EXTI.h"
#endif

#ifdef	_ADC
	#include "HT8_ADC.h"
#endif

#ifdef	_CTM
	#include "HT8_CTM.h"
#endif

#ifdef	_STM
	#include "HT8_STM.h"
#endif

#ifdef	_PTM
	#include "HT8_PTM.h"
#endif

#ifdef	_SIM
	#include "HT8_SIM.h"
#endif

#ifdef	_UART
	#include "HT8_UART.h"
#endif

#ifdef	_TIMEBASE
	#include "HT8_TIMEBASE.h"
#endif

#ifdef	_WDT
	#include "HT8_WDT.h"
#endif

#ifdef	_COMP
	#include "HT8_COMP.h"
#endif

#ifdef	_LVD
	#include "HT8_LVD.h"
#endif

#ifdef	_LVR
	#include "HT8_LVR.h"
#endif

#ifdef	_EEPROM
	#include "HT8_EEPROM.h"
#endif

#include "HT8_it.h"

#endif

/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/

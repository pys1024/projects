/**
  ******************************************************************************
  * @file HT8_EXTI.h
  * @brief The header file of the external interrupt library.
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

#ifndef _HT8_EXTI_H_
#define _HT8_EXTI_H_

#include "HT8_MCU_IP_SEL.h"

#define	INT0_ISR_ENABLE()			_int0e = 1
#define	INT0_ISR_DISABLE()			_int0e = 0
#define	INT1_ISR_ENABLE()			_int1e = 1
#define	INT1_ISR_DISABLE()			_int1e = 0

#define	INT0_SET_FLAG()				_int0f = 1
#define	INT0_CLEAR_FLAG()			_int0f = 0
#define	INT1_SET_FLAG()				_int1f = 1
#define	INT1_CLEAR_FLAG()			_int1f = 0


/******** INT0 interrupr edge control ********/
//	#define	INT0_DISABLE				(1)
//	#define	INT0_RISING_EDGE			(1)
//	#define	INT0_FALLING_EDGE			(1)
//	#define	INT0_BOTH_EDGE				(1)
/**** end of INT0 interrupr edge control *****/


/******** INT1 interrupr edge control ********/
//	#define	INT1_DISABLE				(1)
//	#define	INT1_RISING_EDGE			(1)
//	#define	INT1_FALLING_EDGE			(1)
//	#define	INT1_BOTH_EDGE				(1)
/**** end of INT1 interrupr edge control *****/

void EXIT_Init();

#endif

/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/
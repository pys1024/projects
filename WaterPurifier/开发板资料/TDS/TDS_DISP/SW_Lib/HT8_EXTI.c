/**
  ******************************************************************************
  * @file HT8_EXIT.c
  * @brief This file provides all the external interrupt initialization firmware functions.
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

/* Includes ------------------------------------------------------------------*/
#include "HT8_EXTI.h"



/**
  * @brief external interrupt 0/1 initialization function.
  * @par Parameters:
  * None
  * @retval
  * None
  */
void EXIT_Init()
{
	#ifdef	INT0_DISABLE
		_int0s1 = 0; _int0s0 = 0;	//disable external interrupt 0
	
	#elif	INT0_RISING_EDGE
		_int0s1 = 0; _int0s0 = 1;	//rising edge trigger external interrupt 0				
		
	#elif	INT0_FALLING_EDGE
		_int0s1 = 1; _int0s0 = 0;	//falling edge trigger external interrupt 0
		
	#elif	INT0_BOTH_EDGE
		_int0s1 = 1; _int0s0 = 1;	//both edge trigger external interrupt 0

	#endif	
	
	
	#ifdef	INT1_DISABLE
		_int1s1 = 0; _int1s0 = 0;	//disable external interrupt 1
	
	#elif	INT1_RISING_EDGE
		_int1s1 = 0; _int1s0 = 1;	//rising edge trigger external interrupt 1
		
	#elif	INT1_FALLING_EDGE
		_int1s1 = 1; _int1s0 = 0;	//falling edge trigger external interrupt 1
		
	#elif	INT1_BOTH_EDGE
		_int1s1 = 1; _int1s0 = 1;	//both edge trigger external interrupt 1

	#endif		
}

/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/
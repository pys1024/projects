/**
  ******************************************************************************
  * @file HT8_LVD.c
  * @brief This file provides all the LVD firmware functions.
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


#include "HT8_LVD.h"



/**
  * @brief LVD initialization function.
  * @param[in] Non.
  * @retval Non.
  */
void LVD_Init()
{
#ifdef	LVD_VOLTAGE_1V8
	_vlvd2 = 0; _vlvd1 = 0; _vlvd0 = 0;
	
#elif	LVD_VOLTAGE_2V0
	_vlvd2 = 0; _vlvd1 = 0; _vlvd0 = 1;
		
#elif	LVD_VOLTAGE_2V4
	_vlvd2 = 0; _vlvd1 = 1; _vlvd0 = 0;
	
#elif	LVD_VOLTAGE_2V7
	_vlvd2 = 0; _vlvd1 = 1; _vlvd0 = 1;
	
#elif	LVD_VOLTAGE_3V0
	_vlvd2 = 1; _vlvd1 = 0; _vlvd0 = 0;
	
#elif	LVD_VOLTAGE_3V3
	_vlvd2 = 1; _vlvd1 = 0; _vlvd0 = 1;
	
#elif	LVD_VOLTAGE_3V6
	_vlvd2 = 1; _vlvd1 = 1; _vlvd0 =0;
	
#elif	LVD_VOLTAGE_4V0
	_vlvd2 = 1; _vlvd1 = 1; _vlvd0 = 1;
#endif
}

/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/
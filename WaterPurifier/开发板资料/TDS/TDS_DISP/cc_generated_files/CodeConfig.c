/**
  ******************************************************************************
  * @file CodeConfig.c
  * @ brief CodeConfig setting file.
  * @author Holtek Semiconductor Inc.
  * @version V1.0.0
  * @date 2021-07-26
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

#include "CodeConfig.h"

void System_Init()
{	
	SysClock_Init();
	GPIO_Init();
	CTM_Init();
	Communicate_Init();
//end System Init
}

void System_Setting()
{
//Begin System Setting
	MF1F_CLEAR_FLAG();
	MF1E_ENABLE();
	EMI_ENABLE();

	CTM_CounterModeConfig(1000);
	CTM_CLEAR_FLAG_A();
	CTM_CCRA_ISR_ENABLE();
	CTM_ENABLE();

//end System Setting
}

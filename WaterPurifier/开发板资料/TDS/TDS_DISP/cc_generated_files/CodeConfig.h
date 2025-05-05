/**
  ******************************************************************************
  * @file CodeConfig.h
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

#ifndef __CodeConfig_H__
#define __CodeConfig_H__ 1

#include "../SW_Lib/HT8_MCU_IP_SEL.h"

#define seg1 _pc0
#define seg2 _pc1
#define seg3 _pc2
#define seg4 _pc4
#define seg5 _pc5
#define seg6 _pa1
#define seg7 _pa3
#define seg8 _pb6

#define com1 _pb5
#define com2 _pb4
#define com3 _pb3
#define com4 _pa7
#define com5 _pa6
#define com6 _pa5
#define com7 _pa4
#define com8 _pb2


#define key1 _pb0
#define key2 _pb1





void System_Init();
void System_Setting();

#define	DELAY_CNT	4000
#endif

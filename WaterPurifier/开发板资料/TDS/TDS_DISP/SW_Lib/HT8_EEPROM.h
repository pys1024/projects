/**
  ******************************************************************************
  * @file HT8_EEPROM.h
  * @brief This file  contains the handlers.
  * @author Holtek Semiconductor Inc.
  * @version V1.0.0
  * @date 2020-01-21
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

#ifndef _HT8_EEPROM_H_
#define	_HT8_EEPROM_H_


#include "HT8_MCU_IP_SEL.h"

/********** EEPROM Erase and Write time select **********/
//	#define		Erase_3200us_Write_2200us		(1)
//	#define		Erase_3700us_Write_3000us		(1)
/******* end of EEPROM Erase and Write time select ******/	

void EEPROM_Write_Byte(u8 Data,u8 adr);			//EEPROM Write byte mode,No interrupt
void EEPROM_Write_page(u8 *Data,u8 page,u8 len);			//EEPROM Write page mode,No interrupt
u8 EEPROM_Read_Byte(u8 adr);						//EEPROM Read byte mode,No interrupt
void EEPROM_Read_page(u8 *Data,u8 page,u8 len);			//EEPROM Read page mode,No interrupt
void EEPROM_Erase_page(u8 page,u8 len);					//EEPROM Erase page

#endif

/******************* (C) COPYRIGHT 2020 Holtek Semiconductor Inc *****END OF FILE****/
/**
  ******************************************************************************
  * @file HT8_EEPROM.c
  * @brief This file provides all the EEPROM firmware functions.
  * @author Holtek Semiconductor Inc.
  * @version V1.0.0
  * @date 2020-06-30
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

#include "HT8_EEPROM.h"


/**
  * @brief EEPROM write byte function.
  * @param[in] The data you want to write to EEPROM.
  * It can be 0x00~0xff.
  * @param[in] Specifies EEPROM address.
  * It can be 0x00~0x7f.
  * @retval None
  */
void EEPROM_Write_Byte(u8 Data,u8 adr)
{
	u8 TempEMI,TempEEC;
	
	TempEMI = _emi;	
	
	/*config EEPROM init*/
	_mp2h = 1;			

	_mp2l = 0x40;
	
	TempEEC = _iar2;
	
	_iar2 &= 0x6f;
	/*config EEPROM address*/	
	_eea = adr;	
	
	/*Write data*/
	_eed = Data;
	
/********** EEPROM Erase and Write time select **********/
	#ifdef	Erase_3200us_Write_2200us	
		_iar2 &= 0x7f;
		
	#else
		_iar2 |= 0x80;
	
	#endif	
/******* end of EEPROM Erase and Write time select ******/			
	_emi = 0;		//disable global interrupt		
	/*config EEPROM Write mode*/
	_iar2 |= 0x08;
	_iar2 |= 0x04;
	
	_emi = TempEMI;		//enable global interrupt		
	
	/* waitting for EEPROM write finishing */
	while(_iar2 & 0x04);
	_iar2 = TempEEC;
	_mp2h = 0;
		
	return;
}


/**
  * @brief EEPROM write page function.
  * @param[in] The array pointer to be written to EEPROM.
  * @param[in] Specifies EEPROM page.
  * It can be 0~7,page 0 is mapping to address 0x00~0x0f,
  * page 1 is mapping to address 0x10~0x1f,page n is mapping to address (n-1)*16~(n-1)*16+15.
  * @param[in] The Byte number, it's length can up to 16.
  * @retval None
  */
void EEPROM_Write_page(u8 *Data,u8 page,u8 len)
{
	u8 i;
	u8 TempEMI,TempEEC;
	if (len > 16)	len = 16;

	EEPROM_Erase_page(page,len);
		
	TempEMI = _emi;	
	
	/*config EEPROM init*/
	_mp2h = 1;			
	
	_mp2l = 0x40;
	
	TempEEC = _iar2;
	
	_iar2 &= 0x6f;
	/*config EEPROM page*/

	_eea = page<<4;

/********** EEPROM Erase and Write time select **********/
	#ifdef	Erase_3200us_Write_2200us	
		_iar2 &= 0x7f;
		
	#else
		_iar2 |= 0x80;
	
	#endif	
/******* end of EEPROM Erase and Write time select ******/		

	_iar2 |= 0x10;
	/*Write data*/
	for(i = 0; i < len; i++)
		_eed = Data[i];	
	
	_mp2h = 1;	
	_mp2l = 0x40;	
	
	_emi = 0;		//disable global interrupt
	/*config EEPROM Write mode*/
	_iar2 |= 0x08;
	_iar2 |= 0x04;

	_emi = TempEMI;		//enable global interrupt			
		
	/* waitting for EEPROM write finishing */
	while(_iar2 & 0x04);
	_iar2 = TempEEC;
	_mp2h = 0;

	return;
}
	

/**
  * @brief EEPROM read byte function.
  * @param[in] Specifies EEPROM address that you want to read.
  * It can be 0x00~0x7f.
  * @retval EEPROM data.
  */
u8 EEPROM_Read_Byte(u8 adr)
{
	u8 TempEEC;
	u8 EP_Read_Data;
	
	/* config EEPROM init */
	_mp2h = 1;
	_mp2l = 0x40;
	
	TempEEC = _iar2;
	
	_iar2 &= 0x6f;
	/*config EEPROM address*/
	
	_eea = adr;	
	
	/*config EEPROM Read mode*/
	_iar2 |= 0x02;
	_iar2 |= 0x01;
	
	/* waitting for EEPROM Read finishing */
	while(_iar2 & 0x01);
	_iar2 = TempEEC;
	_mp2h = 0;
		
	EP_Read_Data = _eed;		//Read data
	return EP_Read_Data;			
}

/**
  * @brief EEPROM read page function.
  * @param[in] The array pointer to be reading to EEPROM.
  * @param[in] Specifies EEPROM page.
  * It can be 0~7,page 0 is mapping to address 0x00~0x0f,
  * page 1 is mapping to address 0x10~0x1f,page n is mapping to address (n-1)*16~(n-1)*16+15.  
  * @param[in] The Byte number, it's length can up to 16.
  * @retval None
  */
void EEPROM_Read_page(u8 *Data,u8 page,u8 len)
{
	u8 i;
	u8 TempEEC;
	
	if (len > 16)	len = 16;
	
	/*config EEPROM init*/
	_mp2h = 1;			

	_mp2l = 0x40;
	
	TempEEC = _iar2;
	
	_iar2 &= 0x6f;
	/*config EEPROM page*/
	
	_eea = page<<4;
	
	/*config EEPROM Read mode*/	
	_iar2 |= 0x10;
	_iar2 |= 0x02;
			
	/*Read data*/
	for(i = 0; i < len; i++)
	{
		_mp2h = 1;
		_mp2l = 0x40;
		_iar2 |= 0x01;
		while(_iar2 & 0x01);
		Data[i] = _eed;
	}	
	_iar2 = TempEEC;
	_mp2h = 0;
		
	return;
}



/**
  * @brief EEPROM erase page function.
  * @param[in] Specifies EEPROM page, It can be 0x00,0x10,0x20~0x70.
  * It can be 0~7,page 0 is mapping to address 0x00~0x0f,
  * page 1 is mapping to address 0x10~0x1f,page n is mapping to address (n-1)*16~(n-1)*16+15.
  * @param[in] The Byte number, it's length can up to 16.  
  * @retval None
  */
void EEPROM_Erase_page(u8 page,u8 len)
{
	u8 i;
	u8 TempEMI,TempEEC;
	
	if (len > 16)	len = 16;
	
	TempEMI = _emi;
	
	/*config EEPROM init*/
	_mp2h = 1;			

	_mp2l = 0x40;
	TempEEC = _iar2;
	
	_iar2 &= 0x6f;
	/*config EEPROM page*/
	
	_eea = page<<4;

/********** EEPROM Erase and Write time select **********/
	#ifdef	Erase_3200us_Write_2200us	
		_iar2 &= 0x7f;
		
	#else
		_iar2 |= 0x80;
	
	#endif	
/******* end of EEPROM Erase and Write time select ******/		

	_iar2 |= 0x10;
	/*Write data*/
	for(i = 0; i < 16; i++)
		_eed = 0x55;	
	
	_mp2h = 1;	
	_mp2l = 0x40;
	
	_emi = 0;		//disable global interrupt	
	
	/*config EEPROM Write mode*/
	_iar2 |= 0x40;
	_iar2 |= 0x20;
	
	_emi = TempEMI;		//enable global interrupt			
	/* waitting for EEPROM write finishing */
	while(_iar2 & 0x20);
	_iar2 = TempEEC;
	_mp2h = 0;
		
	return;
}

/******************* (C) COPYRIGHT 2020 Holtek Semiconductor Inc *****END OF FILE****/
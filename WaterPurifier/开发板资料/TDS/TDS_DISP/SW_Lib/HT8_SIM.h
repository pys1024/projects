/**
  ******************************************************************************
  * @file HT8_SIM.h
  * @brief The header file of the SIM library.
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

#ifndef _HT8_SIM_h_
#define	_HT8_SIM_h_

#include "HT8_MCU_IP_SEL.h"

#define	SIM_ENABLE()			_simen = 1	//enable SIM IP
#define	SIM_DISABLE()			_simen = 0	//disable SIM IP
#define	SIM_SPI_CS_ENABLE()		_csen  = 1	//SIM SPI SCS pin enable
#define	SIM_SPI_CS_DISABLE()	_csen  = 0	//SIM SPI SCS pin disable
#define	SIM_ISR_ENABLE()		_sime  = 1	//enable SIM interrupt
#define	SIM_ISR_DISABLE()		_sime  = 0	//disable SIM interrupt
#define	SIM_SET_FLAG()			_simf  = 1	//set SIM interrupt flag
#define	SIM_CLEAR_FLAG()		_simf  = 0	//clear SIM interrupt flag
#define	SIM_GET_FLAG()			_simf  

/**************** SIM mode select ****************/
//	#define SIM_I2C_Slave			1	//set SIM as I2C slave mode
//	#define SIM_SPI_Master			1	//set SIM as SPI master mode
//	#define SIM_SPI_Slave			1	//set SIM as SPI slave mode
/************ end of SIM mode select *************/



/*********************************SIM I2C funtion configuration************************************/
//#define	SIM_I2C_DEVICEADDR		0X72			//I2C Slave address

#define	SIM_I2CTO_ENABLE()		_simtoen = 1	//enable I2C Time-out control
#define	SIM_I2CTO_DISABLE()		_simtoen = 0	//disable I2C Time-out control

/*********** SIM I2C debounce time select ***********/
//	#define	SIM_I2C_DEBOUNCE_DISABLE	1	//I2C no debounce
//	#define	SIM_I2C_DEBOUNCE_2_FSYS		1	//I2C debounce 2 Fsys,default
//	#define	SIM_I2C_DEBOUNCE_4_FSYS		1	//I2C debounce 4 Fsys
/******* end of SIM I2C debounce time select ********/



/*********************************SPI funtion configuration************************************/

/*SIM SPI write collision flag*/
#define	SIM_SPI_WRITE_COLLISION_FLAG	 	_wcol	

/*SIM SPI Transmit/Receive complete flag*/
#define	SIM_SPI_TRX_FLAG					_trf

/*********************** SIM SPI master clock select **********************/
//	#define	SIM_SPI_Master_FSYS_DIV4	1 	//SIM SPI master clock is fSYS /4 		
//	#define SIM_SPI_Master_FSYS_DIV16	1 	//SIM SPI master clock is fSYS /16
//	#define	SIM_SPI_Master_FSYS_DIV64	1 	//SIM SPI master clock is fSYS /64
//	#define SIM_SPI_Master_FSUB			1 	//SIM SPI master clock is fSUB
//	#define SIM_SPI_Master_FPTM			1 	//SIM SPI master clock is PTM CCRP match frequency/2
/* end of SIM SPI master clock select */

/*********************** SIM SPI SCS pin control select **********************/
//	#define	SIM_SPI_CS_ON			1 	//enable SCS pin control
//	#define SIM_SPI_CS_OFF			1 	//disable SCS pin control
/* end of SIM SPI SCS pin control select */

/********* SIM SPI SCK clock active edge type selection ********/
//	#define SIM_SPI_SCK_HIGH_RISING_EDGE		1	 //SCK is high base level and data capture at SCK rising edge
//	#define SIM_SPI_SCK_HIGH_FALLING_EDGE		1	 //SCK is high base level and data capture at SCK falling edge
//	#define SIM_SPI_SCK_LOW_FALLING_EDGE		1	 //SCK is low base level and data capture at SCK falling edge
//	#define SIM_SPI_SCK_LOW_RISING_EDGE			1	 //SCK is low base level and data capture at SCK rising edge
/* end of SIM SPI SCK clock active edge type selection */

/*************** SIM SPI data shift order ****************/
//	#define SIM_SPI_LSB_FIRST			1
//	#define SIM_SPI_MSB_FIRST			1
/* end of SIM SPI data shift order */



void SIM_Init();

void SIM_I2C_Timeout_Update(u8 timeout);

u8 SIM_SPI_MasterSendData(u8 TX_data);

#endif

/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/
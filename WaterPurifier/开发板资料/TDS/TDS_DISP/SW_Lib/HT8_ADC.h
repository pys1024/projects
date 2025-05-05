/**
  ******************************************************************************
  * @file HT8_ADC.h
  * @brief The header file of the ADC library.
  * @author Holtek Semiconductor Inc.
  * @version V1.0.0
  * @date 2020-07-08
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

#ifndef _ADC_H_
#define _ADC_H_

#include "HT8_MCU_IP_SEL.h"

#define		ADC_ENABLE()			_adcen = 1
#define		ADC_DISABLE()			_adcen = 0

#define		ADC_ISR_ENABLE()		_ade = 1
#define		ADC_ISR_DISABLE()		_ade = 0
#define		ADC_CLEAR_ISR_FLAG()	_adf = 0
#define		ADC_SET_ISR_FLAG()		_adf = 1

#define		ADC_PGA_FROM_VREFI()	_pgais = 0
#define		ADC_PGA_FROM_VBGREF()	_pgais = 1

#define		ADC_PGA_ENABLE()		_adpgaen = 1
#define		ADC_PGA_DISABLE()		_adpgaen = 0

#define		ADC_VBG_ENABLE()		_vbgren = 1
#define		ADC_VBG_DISABLE()		_vbgren = 0

#define		ADC_START()				{_start = 0; _start = 1; _start = 0;}


/* attention¡GADC clock is 0.1 to 2MHz,such as FSYS=8MHz¡A
 * ADC clock from FSYS/64 ~ FSYS/8 is OK.
 */
/******************* select ADC clock **********************/
//	#define		ADC_CLOCK_FSYS				(1)
//	#define		ADC_CLOCK_FSYS_DIV2			(1)
//	#define		ADC_CLOCK_FSYS_DIV4			(1)
//	#define		ADC_CLOCK_FSYS_DIV8			(1)
//	#define		ADC_CLOCK_FSYS_DIV16		(1)
//	#define		ADC_CLOCK_FSYS_DIV32		(1)
//	#define		ADC_CLOCK_FSYS_DIV64		(1)
//	#define		ADC_CLOCK_FSYS_DIV128		(1)
/* end of select ADC clock */


/*************** select ADC reference voltage **************/ 
//	#define		ADC_REF_VOLTAGE_VDD			(1)
//	#define		ADC_REF_VOLTAGE_VREF		(1) 
//	#define		ADC_REF_VOLTAGE_VR			(1)
/* end of select ADC reference voltage */

/*************** select ADC PGA input signal **************/ 
//	#define		ADC_PGA_REFI				(1)
//	#define		ADC_PGA_VBG					(1)
/* end of select ADC PGA input signal */

/*************** select ADC PGA Gain **************/ 
//	#define		ADC_PGA_OFF				(1)
//	#define		ADC_PGA_1_0					(1)
//	#define		ADC_PGA_1_667				(1)
//	#define		ADC_PGA_2_5					(1)
//	#define		ADC_PGA_3_333				(1)
/* end of select ADC PGA Gain */


/***************** select ADC data format ******************/
//	#define		ADC_VALUE_ALIGN_LEFT		(1)
//	#define		ADC_VALUE_ALIGN_RIGHT		(1)
//end of select ADC data format


/* READ ADC VALUE function */

#ifdef	ADC_VALUE_ALIGN_LEFT
	
	#define	ADC_READ_VALUE()	(_sadoh << 4) | (_sadol >> 4)
	
#else 

	#define	ADC_READ_VALUE()	(_sadoh << 8) | _sadol

#endif



/**
  * @brief ADC analog channel selection list
  */
typedef enum 
{
	ADC_CH0  = (u8)0x00, /**< Analog channel 0 */
	ADC_CH1  = (u8)0x01, /**< Analog channel 1 */
	ADC_CH2  = (u8)0x02, /**< Analog channel 2 */
	ADC_CH3  = (u8)0x03, /**< Analog channel 3 */
	ADC_CH4  = (u8)0x04, /**< Analog channel 4 */
	ADC_CH5  = (u8)0x05, /**< Analog channel 5 */
	ADC_CH6  = (u8)0x06, /**< Analog channel 6 */
	ADC_CH7  = (u8)0x07, /**< Analog channel 7 */
	ADC_CH8  = (u8)0x08, /**< Analog channel 8 */
	ADC_CH9  = (u8)0x09, /**< Analog channel 9 */
	ADC_CH10 = (u8)0x0a, /**< Analog channel 10 */
	ADC_CH11 = (u8)0x0b, /**< Analog channel 11 */
	ADC_CH_INTERNAL_VDD		 = (u8)0x10,
	ADC_CH_INTERNAL_VDD_DIV2 = (u8)0x20,
	ADC_CH_INTERNAL_VDD_DIV4 = (u8)0x30,
	ADC_CH_INTERNAL_VR		 = (u8)0x50,
	ADC_CH_INTERNAL_VR_DIV2	 = (u8)0x60,
	ADC_CH_INTERNAL_VR_DIV4	 = (u8)0x70,   
} ADC_Channel_TypeDef;



/** ADC initialization function¡Ayou can select 
  * ADC clock,ADC reference voltage and AD converter
  * data format in ADC.h **/
void ADC_Init(void);

/** ADC channel select function¡Ayou can select 
  * the channel that you want to use **/
void ADC_SelectChannel(u8 ADC_Channel);

/** ADC converter function¡A when you have already
  * select one channel¡Ayou can get the AD converter
  * value by this function **/
u16 ADC_GetValue(void);

/** ADC channel select and get AD converter data
  * function **/
u16 ADC_GetChannelValue(u8 ADC_Channel);


#endif//end of _HT8_ADC_H_

/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/

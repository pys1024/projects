/**
  ******************************************************************************
  * @file HT8_GPIO.h
  * @brief The header file of the GPIO library.
  * @author Holtek Semiconductor Inc.
  * @version V1.0.0
  * @date 2019-10-04
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

#ifndef _GPIO_H_
#define _GPIO_H_

#include "HT8_MCU_IP_SEL.h"


/******************* select PA0 function **********************/
//	#define		PA0_Outout_H		(1)		//Output mode,output high
	#define		PA0_Outout_L		(1)		//Output mode,output low
//	#define		PA0_Input			(1)		//Input mode,folting
//	#define		PA0_Input_PU		(1)  	//Input mode,pull high
//	#define		PA0_STP				(1)		//STM output function
//	#define		PA0_SCOM0			(1)		//LCD function SCOM0
//	#define		PA0_SSEG0			(1)		//LCD function SSEG0
//	#define		PA0_PU				(1)		//Pull high
//	#define		PA0_WU				(1)		//Wake up enable
/* end of select PA0 function */

/******************* select PA1 function **********************/
//	#define		PA1_Outout_H		(1)		//Output mode,output high
	#define		PA1_Outout_L		(1)		//Output mode,output low
//	#define		PA1_Input			(1)		//Input mode,folting
//	#define		PA1_Input_PU		(1)  	//Input mode,pull high
//	#define		PA1_INT0			(1)		//External interrupt 0 function
//	#define		PA1_SDO				(1)		//SIM SDO function
//	#define		PA1_SCOM1			(1)		//LCD function SCOM1
//	#define		PA1_SSEG1			(1)		//LCD function SSEG1
//	#define		PA1_PU				(1)		//Pull high
//	#define		PA1_WU				(1)		//Wake up enable
/* end of select PA1 function */

/******************* select PA2 function **********************/
//	#define		PA2_Outout_H		(1)		//Output mode,output high
	#define		PA2_Outout_L		(1)		//Output mode,output low
//	#define		PA2_Input			(1)		//Input mode,folting
//	#define		PA2_Input_PU		(1)  	//Input mode,pull high
//	#define		PA2_INT1			(1)		//External interrupt 1 function
//	#define		PA2_SCOM2			(1)		//LCD function SCOM2
//	#define		PA2_SSEG2			(1)		//LCD function SSEG2
//	#define		PA2_PU				(1)		//Pull high
//	#define		PA2_WU				(1)		//Wake up enable
/* end of select PA2 function */

/******************* select PA3 function **********************/
//	#define		PA3_Outout_H		(1)		//Output mode,output high
	#define		PA3_Outout_L		(1)		//Output mode,output low
//	#define		PA3_Input			(1)		//Input mode,folting
//	#define		PA3_Input_PU		(1)  	//Input mode,pull high
//	#define		PA3_SDI_SDA			(1)		//SIM SDI/SDA function
//	#define		PA3_CX				(1)		//Comparator output function
//	#define		PA3_SCOM3			(1)		//LCD function SCOM3
//	#define		PA3_SSEG3			(1)		//LCD function SSEG3
//	#define		PA3_PU				(1)		//Pull high
//	#define		PA3_WU				(1)		//Wake up enable
/* end of select PA3 function */

/******************* select PA4 function **********************/
//	#define		PA4_Outout_H		(1)		//Output mode,output high
	#define		PA4_Outout_L		(1)		//Output mode,output low
//	#define		PA4_Input			(1)		//Input mode,folting
//	#define		PA4_Input_PU		(1)  	//Input mode,pull high
//	#define		PA4_PTCK			(1)		//PTM TCK input function
//	#define		PA4_SCOM4			(1)		//LCD function SCOM4
//	#define		PA4_SSEG4			(1)		//LCD function SSEG4
//	#define		PA4_AN3				(1)		//A/D channel 3 function
//	#define		PA4_PU				(1)		//Pull high
//	#define		PA4_WU				(1)		//Wake up enable
/* end of select PA4 function */

/******************* select PA5 function **********************/
//	#define		PA5_Outout_H		(1)		//Output mode,output high
	#define		PA5_Outout_L		(1)		//Output mode,output low
//	#define		PA5_Input			(1)		//Input mode,folting
//	#define		PA5_Input_PU		(1)  	//Input mode,pull high
//	#define		PA5_SCOM5			(1)		//LCD function SCOM5
//	#define		PA5_SSEG5			(1)		//LCD function SSEG5
//	#define		PA5_AN4				(1)		//A/D channel 4 function
//	#define		PA5_VREFI			(1)		//ADC reference voltage input function
//	#define		PA5_PU				(1)		//Pull high
//	#define		PA5_WU				(1)		//Wake up enable
/* end of select PA5 function */

/******************* select PA6 function **********************/
//	#define		PA6_Outout_H		(1)		//Output mode,output high
	#define		PA6_Outout_L		(1)		//Output mode,output low
//	#define		PA6_Input			(1)		//Input mode,folting
//	#define		PA6_Input_PU		(1)  	//Input mode,pull high
//	#define		PA6_CTCK			(1)		//CTM TCK input function
//	#define		PA6_SCOM6			(1)		//LCD function SCOM6
//	#define		PA6_SSEG6			(1)		//LCD function SSEG6
//	#define		PA6_AN5				(1)		//A/D channel 5 function
//	#define		PA6_VREF			(1)		//ADC reference voltage input function
//	#define		PA6_PU				(1)		//Pull high
//	#define		PA6_WU				(1)		//Wake up enable
/* end of select PA6 function */

/******************* select PA7 function **********************/
//	#define		PA7_Outout_H		(1)		//Output mode,output high
	#define		PA7_Outout_L		(1)		//Output mode,output low
//	#define		PA7_Input			(1)		//Input mode,folting
//	#define		PA7_Input_PU		(1)  	//Input mode,pull high
//	#define		PA7_PTP				(1)		//PTM output function
//	#define		PA7_SCOM7			(1)		//LCD function SCOM7
//	#define		PA7_SSEG7			(1)		//LCD function SSEG7
//	#define		PA7_AN6				(1)		//A/D channel 6 function
//	#define		PA7_PU				(1)		//Pull high	
//	#define		PA7_WU				(1)		//Wake up enable
/* end of select PA7 function */


/******************* select PB0 function **********************/
//	#define		PB0_Outout_H		(1)		//Output mode,output high
//	#define		PB0_Outout_L		(1)		//Output mode,output low
	#define		PB0_Input			(1)		//Input mode,folting
//	#define		PB0_Input_PU		(1)  	//Input mode,pull high
//	#define		PB0_INT0			(1)		//External interrupt 0 function
//	#define		PB0_SCOM8			(1)		//LCD function SCOM8
//	#define		PB0_SSEG8			(1)		//LCD function SSEG8
//	#define		PB0_AN0				(1)		//A/D channel 0 function
	#define		PB0_PU				(1)		//Pull high	
/* end of select PB0 function */

/******************* select PB1 function **********************/
//	#define		PB1_Outout_H		(1)		//Output mode,output high
//	#define		PB1_Outout_L		(1)		//Output mode,output low
	#define		PB1_Input			(1)		//Input mode,folting
//	#define		PB1_Input_PU		(1)  	//Input mode,pull high
//	#define		PB1_INT1			(1)		//External interrupt 1 function
//	#define		PB1_SCOM9			(1)		//LCD function SCOM9
//	#define		PB1_SSEG9			(1)		//LCD function SSEG9
//	#define		PB1_AN1				(1)		//A/D channel 1 function
	#define		PB1_PU				(1)		//Pull high	
/* end of select PB1 function */

/******************* select PB2 function **********************/
//	#define		PB2_Outout_H		(1)		//Output mode,output high
	#define		PB2_Outout_L		(1)		//Output mode,output low
//	#define		PB2_Input			(1)		//Input mode,folting
//	#define		PB2_Input_PU		(1)  	//Input mode,pull high
//	#define		PB2_STCK			(1)		//STM TCK input function
//	#define		PB2_STP				(1)		//STM output function
//	#define		PB2_SCOM10			(1)		//LCD function SCOM10
//	#define		PB2_SSEG10			(1)		//LCD function SSEG10
//	#define		PB2_AN2				(1)		//A/D channel 2 function
//	#define		PB2_PU				(1)		//Pull high	
/* end of select PB2 function */

/******************* select PB3 function **********************/
//	#define		PB3_Outout_H		(1)		//Output mode,output high
	#define		PB3_Outout_L		(1)		//Output mode,output low
//	#define		PB3_Input			(1)		//Input mode,folting
//	#define		PB3_Input_PU		(1)  	//Input mode,pull high
//	#define		PB3_CTP				(1)		//CTM output function
//	#define		PB3_SCOM11			(1)		//LCD function SCOM11
//	#define		PB3_SSEG11			(1)		//LCD function SSEG11
//	#define		PB3_AN7				(1)		//A/D channel 7 function
//	#define		PB3_PU				(1)		//Pull high	
/* end of select PB3 function */

/******************* select PB4 function **********************/
//	#define		PB4_Outout_H		(1)		//Output mode,output high
	#define		PB4_Outout_L		(1)		//Output mode,output low
//	#define		PB4_Input			(1)		//Input mode,folting
//	#define		PB4_Input_PU		(1)  	//Input mode,pull high
//	#define		PB4_CLO				(1)		//System Clock Output function
//	#define		PB4_SCOM12			(1)		//LCD function SCOM12
//	#define		PB4_SSEG12			(1)		//LCD function SSEG12
//	#define		PB4_AN8				(1)		//A/D channel 8 function
//	#define		PB4_PU				(1)		//Pull high	
/* end of select PB4 function */

/******************* select PB5 function **********************/
//	#define		PB5_Outout_H		(1)		//Output mode,output high
	#define		PB5_Outout_L		(1)		//Output mode,output low
//	#define		PB5_Input			(1)		//Input mode,folting
//	#define		PB5_Input_PU		(1)  	//Input mode,pull high
//	#define		PB5_SCSB			(1)		//SPI slave select function
//	#define		PB5_CN				(1)		//Comparator input function
//	#define		PB5_SCOM13			(1)		//LCD function SCOM13
//	#define		PB5_SSEG13			(1)		//LCD function SSEG13
//	#define		PB5_PU				(1)		//Pull high	
/* end of select PB5 function */

/******************* select PB6 function **********************/
//	#define		PB6_Outout_H		(1)		//Output mode,output high
	#define		PB6_Outout_L		(1)		//Output mode,output low
//	#define		PB6_Input			(1)		//Input mode,folting
//	#define		PB6_Input_PU		(1)  	//Input mode,pull high
//	#define		PB6_SCK_SCL			(1)		//SIM SCK/SCL function
//	#define		PB6_CP				(1)		//Comparator input function
//	#define		PB6_SCOM14			(1)		//LCD function SCOM14
//	#define		PB6_SSEG14			(1)		//LCD function SSEG14
//	#define		PB6_PU				(1)		//Pull high	
/* end of select PB6 function */


/******************* select PC0 function **********************/
//	#define		PC0_Outout_H		(1)		//Output mode,output high
	#define		PC0_Outout_L		(1)		//Output mode,output low
//	#define		PC0_Input			(1)		//Input mode,folting
//	#define		PC0_Input_PU		(1)  	//Input mode,pull high
//	#define		PC0_TX				(1)		//UART TX function
//	#define		PC0_SCOM15			(1)		//LCD function SCOM15
//	#define		PC0_SSEG15			(1)		//LCD function SSEG15
//	#define		PC0_PU				(1)		//Pull high	
/* end of select PC0 function */

/******************* select PC1 function **********************/
//	#define		PC1_Outout_H		(1)		//Output mode,output high
	#define		PC1_Outout_L		(1)		//Output mode,output low
//	#define		PC1_Input			(1)		//Input mode,folting
//	#define		PC1_Input_PU		(1)  	//Input mode,pull high
//	#define		PC1_RX_TX			(1)		//UART mode: RX function; 1-wire mode: RX and TX function
//	#define		PC1_SCOM16			(1)		//LCD function SCOM16
//	#define		PC1_SSEG16			(1)		//LCD function SSEG16
//	#define		PC1_PU				(1)		//Pull high	
/* end of select PC1 function */

/******************* select PC2 function **********************/
//	#define		PC2_Outout_H		(1)		//Output mode,output high
	#define		PC2_Outout_L		(1)		//Output mode,output low
//	#define		PC2_Input			(1)		//Input mode,folting
//	#define		PC2_Input_PU		(1)  	//Input mode,pull high
//	#define		PC2_SDO				(1)		//SIM SDO function
//	#define		PC2_RESB				(1)		//external reset function 
//	#define		PC2_SCOM17			(1)		//LCD function SCOM17
//	#define		PC2_SSEG17			(1)		//LCD function SSEG17
//	#define		PC2_PU				(1)		//Pull high	
/* end of select PC2 function */

/******************* select PC3 function **********************/
//	#define		PC3_Outout_H		(1)		//Output mode,output high
	#define		PC3_Outout_L		(1)		//Output mode,output low
//	#define		PC3_Input			(1)		//Input mode,folting
//	#define		PC3_Input_PU		(1)  	//Input mode,pull high
//	#define		PC3_SDO				(1)		//SIM SDO function 
//	#define		PC3_SCOM18			(1)		//LCD function SCOM18
//	#define		PC3_SSEG18			(1)		//LCD function SSEG18
//	#define		PC3_PU				(1)		//Pull high	
/* end of select PC3 function */

/******************* select PC4 function **********************/
//	#define		PC4_Outout_H		(1)		//Output mode,output high
	#define		PC4_Outout_L		(1)		//Output mode,output low
//	#define		PC4_Input			(1)		//Input mode,folting
//	#define		PC4_Input_PU		(1)  	//Input mode,pull high
//	#define		PC4_SDI_SDA			(1)		//SIM SDI/SDA function 
//	#define		PC4_SCOM19			(1)		//LCD function SCOM19
//	#define		PC4_SSEG19			(1)		//LCD function SSEG19
//	#define		PC4_PU				(1)		//Pull high	
/* end of select PC4 function */

/******************* select PC5 function **********************/
//	#define		PC5_Outout_H		(1)		//Output mode,output high
	#define		PC5_Outout_L		(1)		//Output mode,output low
//	#define		PC5_Input			(1)		//Input mode,folting
//	#define		PC5_Input_PU		(1)  	//Input mode,pull high
//	#define		PC5_SCK_SCL			(1)		//SIM SCK/SCL function
//	#define		PC5_SCOM20			(1)		//LCD function SCOM20
//	#define		PC5_SSEG20			(1)		//LCD function SSEG20
//	#define		PC5_PU				(1)		//Pull high	
/* end of select PC5 function */

/******************* select PC6 function **********************/
//	#define		PC6_Outout_H		(1)		//Output mode,output high
	#define		PC6_Outout_L		(1)		//Output mode,output low
//	#define		PC6_Input			(1)		//Input mode,folting
//	#define		PC6_Input_PU		(1)  	//Input mode,pull high
//	#define		PC6_SCSB			(1)		//SPI slave select function
//	#define		PC6_SCOM21			(1)		//LCD function SCOM21
//	#define		PC6_SSEG21			(1)		//LCD function SSEG21
//	#define		PC6_PU				(1)		//Pull high	
/* end of select PC6 function */


/******************* select PD0 function **********************/
//	#define		PD0_Outout_H		(1)		//Output mode,output high
	#define		PD0_Outout_L		(1)		//Output mode,output low
//	#define		PD0_Input			(1)		//Input mode,folting
//	#define		PD0_Input_PU		(1)  	//Input mode,pull high
//	#define		PD0_PTP				(1)		//PTM output function
//	#define		PD0_SSEG22			(1)		//LCD function
//	#define		PD0_PU				(1)		//Pull high	
/* end of select PD0 function */

/******************* select PD1 function **********************/
//	#define		PD1_Outout_H		(1)		//Output mode,output high
//	#define		PD1_Outout_L		(1)		//Output mode,output low
//	#define		PD1_Input			(1)		//Input mode,folting
//	#define		PD1_Input_PU		(1)  	//Input mode,pull high
	#define		PD1_RX_TX			(1)		//UART mode: RX function; 1-wire mode: RX and TX function
//	#define		PD1_SSEG23			(1)		//LCD function
//	#define		PD1_AN11			(1)		//A/D channel 11 function
//	#define		PD1_PU				(1)		//Pull high	
/* end of select PD1 function */

/******************* select PD2 function **********************/
//	#define		PD2_Outout_H		(1)		//Output mode,output high
//	#define		PD2_Outout_L		(1)		//Output mode,output low
//	#define		PD2_Input			(1)		//Input mode,folting
//	#define		PD2_Input_PU		(1)  	//Input mode,pull high
	#define		PD2_TX				(1)		//UART TX function
//	#define		PD2_SSEG24			(1)		//LCD function
//	#define		PD2_AN10			(1)		//A/D channel 10 function
//	#define		PD2_PU				(1)		//Pull high	
/* end of select PD2 function */

/******************* select PD3 function **********************/
//	#define		PD3_Outout_H		(1)		//Output mode,output high
	#define		PD3_Outout_L		(1)		//Output mode,output low
//	#define		PD3_Input			(1)		//Input mode,folting
//	#define		PD3_Input_PU		(1)  	//Input mode,pull high
//	#define		PD3_CTP				(1)		//CTM output function
//	#define		PD3_SSEG25			(1)		//LCD function
//	#define		PD3_AN9				(1)		//A/D channel 9 function
//	#define		PD3_PU				(1)		//Pull high	
/* end of select PD3 function */



/******************* select PA0~PA3 source current  **********************/
	#define		PA0_PA3_Level_0		(1)		//PA0~PA3 source current Level 0
//	#define		PA0_PA3_Level_1		(1)		//PA0~PA3 source current Level 1
//	#define		PA0_PA3_Level_2		(1)		//PA0~PA3 source current Level 2
//	#define		PA0_PA3_Level_3		(1)		//PA0~PA3 source current Level 3
/* end of select PA0~PA3 source current */

/******************* select PA4~PA7 source current  **********************/
	#define		PA4_PA7_Level_0		(1)		//PA4~PA7 source current Level 0
//	#define		PA4_PA7_Level_1		(1)		//PA4~PA7 source current Level 1
//	#define		PA4_PA7_Level_2		(1)		//PA4~PA7 source current Level 2
//	#define		PA4_PA7_Level_3		(1)		//PA4~PA7 source current Level 3
/* end of select PA4~PA7 source current */

/******************* select PB0~PB3 source current  **********************/
	#define		PB0_PB3_Level_0		(1)		//PB0~PB3 source current Level 0
//	#define		PB0_PB3_Level_1		(1)		//PB0~PB3 source current Level 1
//	#define		PB0_PB3_Level_2		(1)		//PB0~PB3 source current Level 2
//	#define		PB0_PB3_Level_3		(1)		//PB0~PB3 source current Level 3
/* end of select PB0~PB3 source current */

/******************* select PB4~PB6 source current  **********************/
	#define		PB4_PB6_Level_0		(1)		//PB4~PB6 source current Level 0
//	#define		PB4_PB6_Level_1		(1)		//PB4~PB6 source current Level 1
//	#define		PB4_PB6_Level_2		(1)		//PB4~PB6 source current Level 2
//	#define		PB4_PB6_Level_3		(1)		//PB4~PB6 source current Level 3
/* end of select PB4~PB6 source current */

/******************* select PC0~PC3 source current  **********************/
	#define		PC0_PC3_Level_0		(1)		//PC0~PC3 source current Level 0
//	#define		PC0_PC3_Level_1		(1)		//PC0~PC3 source current Level 1
//	#define		PC0_PC3_Level_2		(1)		//PC0~PC3 source current Level 2
//	#define		PC0_PC3_Level_3		(1)		//PC0~PC3 source current Level 3
/* end of select PC0~PC3 source current */

/******************* select PC4~PC6 source current  **********************/
	#define		PC4_PC6_Level_0		(1)		//PC4~PC6 source current Level 0
//	#define		PC4_PC6_Level_1		(1)		//PC4~PC6 source current Level 1
//	#define		PC4_PC6_Level_2		(1)		//PC4~PC6 source current Level 2
//	#define		PC4_PC6_Level_3		(1)		//PC4~PC6 source current Level 3
/* end of select PC4~PC6 source current */

/******************* select PD0~PD3 source current  **********************/
	#define		PD0_PD3_Level_0		(1)		//PD0~PD3 source current Level 0
//	#define		PD0_PD3_Level_1		(1)		//PD0~PD3 source current Level 1
//	#define		PD0_PD3_Level_2		(1)		//PD0~PD3 source current Level 2
//	#define		PD0_PD3_Level_3		(1)		//PD0~PD3 source current Level 3
/* end of select PD0~PD3 source current */



/** GPIO initialization function，you can select **/
void GPIO_Init(void);


#endif//end of _HT8_GPIO_H_

/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/

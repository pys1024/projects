/**
  ******************************************************************************
  * @file HT8_GPIO.c
  * @brief This file provides all the GPIO firmware functions.
  * @author Holtek Semiconductor Inc
  * @version V1.0.0
  * @date 2019-07-23
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
#include "HT8_GPIO.h"

/** GPIO initialization function¡Ayou can select **/
void GPIO_Init(void)
{
/******************* select PA0 function **********************/
	#ifdef	PA0_Outout_H	
		_pas01 = 0; _pas00 = 0; _pac0 = 0; _pa0 = 1;			
			
	#elif	PA0_Outout_L	
		_pas01 = 0; _pas00 = 0; _pac0 = 0; _pa0 = 0;
			
	#elif	PA0_Input	
		_pas01 = 0; _pas00 = 0; _pac0 = 1; 
			
	#elif	PA0_Input_PU	
		_pas01 = 0; _pas00 = 0; _pac0 = 1; _papu0 = 1;
		
	#elif	PA0_STP		
		_pas01 = 1; _pas00 = 0;	_pac0 = 0; _pa0 = 0;
			
	#elif	PA0_SCOM0	
		_pas01 = 1; _pas00 = 1;	_pac0 = 0; _comsegs0 = 0;
		
	#elif	PA0_SSEG0
		_pas01 = 1; _pas00 = 1;	_pac0 = 0; _comsegs0 = 1;	
	#endif		

	#ifdef	PA0_PU	
		_papu0 = 1;	
	#endif
	
	#ifdef	PA0_WU
		_pawu0 = 1;
	#endif
/***************** end of select PA0 function *****************/	

/******************* select PA1 function **********************/
	#ifdef	PA1_Outout_H	
		_pas03 = 0; _pas02 = 0; _pac1 = 0; _pa1 = 1;			
			
	#elif	PA1_Outout_L	
		_pas03 = 0; _pas02 = 0; _pac1 = 0; _pa1 = 0;
			
	#elif	PA1_Input	
		_pas03 = 0; _pas02 = 0; _pac1 = 1; 
			
	#elif	PA1_Input_PU	
		_pas03 = 0; _pas02 = 0; _pac1 = 1; _papu1 = 1;
		
	#elif	PA1_INT0
		_int0ps = 1; _pas03 = 0; _pas02 = 0; _pac1 = 1;
				
	#elif	PA1_SDO	
		_pas03 = 1; _pas02 = 0;
			
	#elif	PA1_SCOM1	
		_pas03 = 1; _pas02 = 1; _pac1 = 0; _comsegs1 = 0;
		
	#elif	PA1_SSEG1	
		_pas03 = 1; _pas02 = 1; _pac1 = 0; _comsegs1 = 1;			
	#endif		

	#ifdef	PA1_PU	
		_papu1 = 1;
	#endif
	
	#ifdef	PA1_WU
		_pawu1 = 1;
	#endif
/***************** end of select PA1 function *****************/	

/******************* select PA2 function **********************/
	#ifdef	PA2_Outout_H	
		_pas05 = 0; _pas04 = 0; _pac2 = 0; _pa2 = 1;			
			
	#elif	PA2_Outout_L	
		_pas05 = 0; _pas04 = 0; _pac2 = 0; _pa2 = 0;
			
	#elif	PA2_Input	
		_pas05 = 0; _pas04 = 0; _pac2 = 1; 
			
	#elif	PA2_Input_PU	
		_pas05 = 0; _pas04 = 0; _pac2 = 1; _papu2 = 1;
		
	#elif	PA2_INT1
		_int1ps = 1; _pas05 = 0; _pas04 = 0; _pac2 = 1;
			
	#elif	PA2_SCOM2	
		_pas05 = 1; _pas04 = 1; _pac2 = 0; _comsegs2 = 0;

	#elif	PA2_SSEG2	
		_pas05 = 1; _pas04 = 1; _pac2 = 0; _comsegs2 = 1;
	#endif		

	#ifdef	PA2_PU	
		_papu2 = 1;
	#endif
	
	#ifdef	PA2_WU
		_pawu2 = 1;
	#endif
/***************** end of select PA2 function *****************/

/******************* select PA3 function **********************/
	#ifdef	PA3_Outout_H	
		_pas07 = 0; _pas06 = 0; _pac3 = 0; _pa3 = 1;			
			
	#elif	PA3_Outout_L	
		_pas07 = 0; _pas06 = 0; _pac3 = 0; _pa3 = 0;
			
	#elif	PA3_Input	
		_pas07 = 0; _pas06 = 0; _pac3 = 1; 
			
	#elif	PA3_Input_PU	
		_pas07 = 0; _pas06 = 0; _pac3 = 1; _papu3 = 1;
		
	#elif	PA3_SDI_SDA
		_sdi_sdaps = 1; _pas07 = 0; _pas06 = 1; 
			
	#elif 	PA3_CX	
		_pas07 = 1; _pas06 = 0; 
			
	#elif	PA3_SCOM3	
		_pas07 = 1; _pas06 = 1; _pac3 = 0; _comsegs3 = 0;

	#elif	PA3_SSEG3	
		_pas07 = 1; _pas06 = 1; _pac3 = 0; _comsegs3 = 1;
	#endif		

	#ifdef	PA3_PU	
		_papu3 = 1;
	#endif
	
	#ifdef	PA3_WU
		_pawu3 = 1;
	#endif
/***************** end of select PA3 function *****************/	

/******************* select PA4 function **********************/
	#ifdef	PA4_Outout_H	
		_pas11 = 0; _pas10 = 0; _pac4 = 0; _pa4 = 1;			
			
	#elif	PA4_Outout_L	
		_pas11 = 0; _pas10 = 0; _pac4 = 0; _pa4 = 0;
			
	#elif	PA4_Input	
		_pas11 = 0; _pas10 = 0; _pac4 = 1; 
			
	#elif	PA4_Input_PU	
		_pas11 = 0; _pas10 = 0; _pac4 = 1; _papu4 = 1;
		
	#elif	PA4_PTCK
		_pas11 = 0; _pas10 = 0; _pac4 = 1;
			
	#elif	PA4_SCOM4	
		_pas11 = 1; _pas10 = 0; _pac4 = 0; _comsegs4 = 0;

	#elif	PA4_SSEG4	
		_pas11 = 1; _pas10 = 0; _pac4 = 0; _comsegs4 = 1;
	
	#elif	PA4_AN3	
		_pas11 = 1; _pas10 = 1; 
		
	#endif		

	#ifdef	PA4_PU	
		_papu4 = 1;
	#endif
	
	#ifdef	PA4_WU
		_pawu4 = 1;
	#endif
/***************** end of select PA4 function *****************/

/******************* select PA5 function **********************/
	#ifdef	PA5_Outout_H	
		_pas13 = 0; _pas12 = 0; _pac5 = 0; _pa5 = 1;			
			
	#elif	PA5_Outout_L	
		_pas13 = 0; _pas12 = 0; _pac5 = 0; _pa5 = 0;
			
	#elif	PA5_Input	
		_pas13 = 0; _pas12 = 0; _pac5 = 1; 
			
	#elif	PA5_Input_PU	
		_pas13 = 0; _pas12 = 0; _pac5 = 1; _papu5 = 1;
		
	#elif	PA5_SCOM5	
		_pas13 = 0; _pas12 = 1; _pac5 = 0; _comsegs5 = 0;

	#elif	PA5_SSEG5	
		_pas13 = 0; _pas12 = 1; _pac5 = 0; _comsegs5 = 1;
	
	#elif	PA5_AN4		
		_pas13 = 1; _pas12 = 0;
		
	#elif 	PA5_VREFI	
		_pas13 = 1; _pas12 = 1;
		
	#endif		

	#ifdef	PA5_PU	
		_papu5 = 1;
	#endif
	
	#ifdef	PA5_WU
		_pawu5 = 1;
	#endif
/***************** end of select PA5 function *****************/

/******************* select PA6 function **********************/
	#ifdef	PA6_Outout_H	
		_pas15 = 0; _pas14 = 0; _pac6 = 0; _pa6 = 1;			
			
	#elif	PA6_Outout_L	
		_pas15 = 0; _pas14 = 0; _pac6 = 0; _pa6 = 0;
			
	#elif	PA6_Input	
		_pas15 = 0; _pas14 = 0; _pac6 = 1; 
			
	#elif	PA6_Input_PU	
		_pas15 = 0; _pas14 = 0; _pac6 = 1; _papu6 = 1;
		
	#elif	PA6_CTCK
		_pas15 = 0; _pas14 = 0; _pac6 = 1;
				
	#elif	PA6_SCOM6
		_pas15 = 0; _pas14 = 1; _pac6 = 0; _comsegs6 = 0;

	#elif	PA6_SSEG6	
		_pas15 = 0; _pas14 = 1; _pac6 = 0; _comsegs6 = 1;
		
	#elif	PA6_AN5	
		_pas15 = 1; _pas14 = 0; 
				
	#elif	PA6_VREF	
		_pas15 = 1; _pas14 = 1; 	
		
	#endif		

	#ifdef	PA6_PU	
		_papu6 = 1;
	#endif
	
	#ifdef	PA6_WU
		_pawu6 = 1;
	#endif
/***************** end of select PA6 function *****************/

/******************* select PA7 function **********************/
	#ifdef	PA7_Outout_H	
		_pas17 = 0; _pas16 = 0; _pac7 = 0; _pa7 = 1;			
			
	#elif	PA7_Outout_L	
		_pas17 = 0; _pas16 = 0; _pac7 = 0; _pa7 = 0;
			
	#elif	PA7_Input	
		_pas17 = 0; _pas16 = 0; _pac7 = 1; 
			
	#elif	PA7_Input_PU	
		_pas17 = 0; _pas16 = 0; _pac7 = 1; _papu7 = 1;
		
	#elif 	PA7_PTP	
		_pas17 = 0; _pas16 = 1; _pac7 = 0; _pa7 = 0;
				
	#elif	PA7_SCOM7
		_pas17 = 1; _pas16 = 0; _pac7 = 0; _comsegs7 = 0;

	#elif	PA7_SSEG7	
		_pas17 = 1; _pas16 = 0; _pac7 = 0; _comsegs7 = 1;
		
	#elif	PA7_AN6	
		_pas17 = 1; _pas16 = 1;		
		
	#endif		

	#ifdef	PA7_PU	
		_papu7 = 1;
	#endif
	
	#ifdef	PA7_WU
		_pawu7 = 1;
	#endif
/***************** end of select PA7 function *****************/


/******************* select PB0 function **********************/
	#ifndef FSUB_LXT 
	
		#ifdef	PB0_Outout_H	
			_pbs01 = 0; _pbs00 = 0; _pbc0 = 0; _pb0 = 1;			
				
		#elif	PB0_Outout_L	
			_pbs01 = 0; _pbs00 = 0; _pbc0 = 0; _pb0 = 0;
				
		#elif	PB0_Input	
			_pbs01 = 0; _pbs00 = 0; _pbc0 = 1; 
				
		#elif	PB0_Input_PU	
			_pbs01 = 0; _pbs00 = 0; _pbc0 = 1; _pbpu0 = 1;
			
		#elif	PB0_INT0		
			_int0ps = 0; _pbs01 = 0; _pbs00 = 0; _pbc0 = 1;
				
		#elif	PB0_SCOM8
			_pbs01 = 0; _pbs00 = 1; _pbc0 = 1; _comsegs8 = 0;

		#elif	PB0_SSEG8
			_pbs01 = 0; _pbs00 = 1; _pbc0 = 1; _comsegs8 = 1;
				
		#elif	PB0_AN0	
			_pbs01 = 1; _pbs00 = 0;	
		
		#endif		
	
		#ifdef	PB0_PU	
			_pbpu0 = 1;
		#endif
		
	#endif
/***************** end of select PB0 function *****************/	

/******************* select PB1 function **********************/
	#ifndef FSUB_LXT 

		#ifdef	PB1_Outout_H	
			_pbs03 = 0; _pbs02 = 0; _pbc1 = 0; _pb1 = 1;			
				
		#elif	PB1_Outout_L	
			_pbs03 = 0; _pbs02 = 0; _pbc1 = 0; _pb1 = 0;
				
		#elif	PB1_Input	
			_pbs03 = 0; _pbs02 = 0; _pbc1 = 1; 
				
		#elif	PB1_Input_PU	
			_pbs03 = 0; _pbs02 = 0; _pbc1 = 1; _pbpu1 = 1;
			
		#elif	PB1_INT1
			_int1ps = 0; _pbs03 = 0; _pbs02 = 0; _pbc1 = 1;
					
		#elif	PB1_SCOM9
			_pbs03 = 0; _pbs02 = 1; _pbc1 = 0; _comsegs9 = 0;

		#elif	PB1_SSEG9	
			_pbs03 = 0; _pbs02 = 1; _pbc1 = 0; _comsegs9 = 1;
				
		#elif	PB1_AN1	
			_pbs03 = 1; _pbs02 = 0;
	
		#endif		
	
		#ifdef	PB1_PU	
			_pbpu1 = 1;
		#endif
		
	#endif
/***************** end of select PB1 function *****************/	

/******************* select PB2 function **********************/
	#ifdef	PB2_Outout_H	
		_pbs05 = 0; _pbs04 = 0; _pbc2 = 0; _pb2 = 1;			
			
	#elif	PB2_Outout_L	
		_pbs05 = 0; _pbs04 = 0; _pbc2 = 0; _pb2 = 0;
			
	#elif	PB2_Input	
		_pbs05 = 0; _pbs04 = 0; _pbc2 = 1; 
			
	#elif	PB2_Input_PU	
		_pbs05 = 0; _pbs04 = 0; _pbc2 = 1; _pbpu2 = 1;
		
	#elif	PB2_STCK
		_pbs05 = 0; _pbs04 = 0; _pbc2 = 1;
		
	#elif	PB2_STP
		_pbs05 = 0; _pbs04 = 1; _pbc2 = 0; _pb2 = 0;
			
	#elif	PB2_SCOM10
		_pbs05 = 1; _pbs04 = 0; _pbc2 = 0; _comsegs10 = 0;

	#elif	PB2_SSEG10	
		_pbs05 = 1; _pbs04 = 0; _pbc2 = 0; _comsegs10 = 1;
		
	#elif	PB2_AN2	
		_pbs05 = 1; _pbs04 = 1;
		
	#endif		

	#ifdef	PB2_PU	
		_pbpu2 = 1;
	#endif
/***************** end of select PB2 function *****************/

/******************* select PB3 function **********************/
	#ifdef	PB3_Outout_H	
		_pbs07 = 0; _pbs06 = 0; _pbc3 = 0; _pb3 = 1;			
			
	#elif	PB3_Outout_L	
		_pbs07 = 0; _pbs06 = 0; _pbc3 = 0; _pb3 = 0;
			
	#elif	PB3_Input	
		_pbs07 = 0; _pbs06 = 0; _pbc3 = 1; 
			
	#elif	PB3_Input_PU	
		_pbs07 = 0; _pbs06 = 0; _pbc3 = 1; _pbpu3 = 1;
		
	#elif	PB3_CTP
		_pbs07 = 0; _pbs06 = 1;  _pbc3 = 0; _pb3 = 0;
			
	#elif 	PB3_SCOM11
		_pbs07 = 1; _pbs06 = 0; _pbc3 = 0; _comsegs11 = 0;

	#elif 	PB3_SSEG11	
		_pbs07 = 1; _pbs06 = 0; _pbc3 = 0; _comsegs11 = 1;
			
	#elif	PB3_AN7	
		_pbs07 = 1; _pbs06 = 1; 
		
	#endif		

	#ifdef	PB3_PU	
		_pbpu3 = 1;
	#endif
/***************** end of select PB3 function *****************/	

/******************* select PB4 function **********************/
	#ifdef	PB4_Outout_H	
		_pbs11 = 0; _pbs10 = 0; _pbc4 = 0; _pb4 = 1;			
			
	#elif	PB4_Outout_L	
		_pbs11 = 0; _pbs10 = 0; _pbc4 = 0; _pb4 = 0;
			
	#elif	PB4_Input	
		_pbs11 = 0; _pbs10 = 0; _pbc4 = 1; 
			
	#elif	PB4_Input_PU	
		_pbs11 = 0; _pbs10 = 0; _pbc4 = 1; _pbpu4 = 1;
		
	#elif	PB4_CLO
		_pbs11 = 0; _pbs10 = 1; 
			
	#elif	PB4_SCOM12
		_pbs11 = 1; _pbs10 = 0; _pbc4 = 0; _comsegs12 = 0;

	#elif	PB4_SSEG12	
		_pbs11 = 1; _pbs10 = 0; _pbc4 = 0; _comsegs12 = 1;
	
	#elif	PB4_AN8	
		_pbs11 = 1; _pbs10 = 1; 
		
	#endif		

	#ifdef	PB4_PU	
		_pbpu4 = 1;
	#endif
/***************** end of select PB4 function *****************/

/******************* select PB5 function **********************/
	#ifdef	PB5_Outout_H	
		_pbs13 = 0; _pbs12 = 0; _pbc5 = 0; _pb5 = 1;			
			
	#elif	PB5_Outout_L	
		_pbs13 = 0; _pbs12 = 0; _pbc5 = 0; _pb5 = 0;
			
	#elif	PB5_Input	
		_pbs13 = 0; _pbs12 = 0; _pbc5 = 1; 
			
	#elif	PB5_Input_PU	
		_pbs13 = 0; _pbs12 = 0; _pbc5 = 1; _pbpu5 = 1;
		
	#elif	PB5_SCSB	
		_scsbps = 1; _pbs13 = 0; _pbs12 = 1;
		
	#elif 	PB5_CN	
		_pbs13 = 1; _pbs12 = 0;
		
	#elif	PB5_SCOM13
		_pbs13 = 1; _pbs12 = 1; _pbc5 = 0; _comsegs13 = 0;

	#elif	PB5_SSEG13	
		_pbs13 = 1; _pbs12 = 1; _pbc5 = 0; _comsegs13 = 1;		
	#endif		

	#ifdef	PB5_PU	
		_pbpu5 = 1;
	#endif
/***************** end of select PB5 function *****************/

/******************* select PB6 function **********************/
	#ifdef	PB6_Outout_H	
		_pbs15 = 0; _pbs14 = 0; _pbc6 = 0; _pb6 = 1;			
			
	#elif	PB6_Outout_L	
		_pbs15 = 0; _pbs14 = 0; _pbc6 = 0; _pb6 = 0;
			
	#elif	PB6_Input	
		_pbs15 = 0; _pbs14 = 0; _pbc6 = 1; 
			
	#elif	PB6_Input_PU	
		_pbs15 = 0; _pbs14 = 0; _pbc6 = 1; _pbpu6 = 1;
		
	#elif	PB6_SCK_SCL
		_sck_sclps = 1; _pbs15 = 0; _pbs14 = 1; 
		
	#elif	PB6_CP	
		_pbs15 = 1; _pbs14 = 0; 
				
	#elif	PB6_SCOM14
		_pbs15 = 1; _pbs14 = 1; _pbc6 = 0; _comsegs14 = 0;

	#elif	PB6_SSEG14	
		_pbs15 = 1; _pbs14 = 1; _pbc6 = 0; _comsegs14 = 1;
	#endif		

	#ifdef	PB6_PU	
		_pbpu6 = 1;
	#endif
/***************** end of select PB6 function *****************/	


/******************* select PC0 function **********************/
	#ifndef FH_HXT_SE10M 
	
	   #ifndef FH_HXT_L10M 
	
			#ifdef	PC0_Outout_H	
				_pcs01 = 0; _pcs00 = 0; _pcc0 = 0; _pc0 = 1;			
					
			#elif	PC0_Outout_L	
				_pcs01 = 0; _pcs00 = 0; _pcc0 = 0; _pc0 = 0;
					
			#elif	PC0_Input	
				_pcs01 = 0; _pcs00 = 0; _pcc0 = 1; 
					
			#elif	PC0_Input_PU	
				_pcs01 = 0; _pcs00 = 0; _pcc0 = 1; _pcpu0 = 1;
				
			#elif	PC0_TX	
				_pcs01 = 0; _pcs00 = 1;	
					
			#elif	PC0_SCOM15
				_pcs01 = 1; _pcs00 = 0;	_pcc0 = 0; _comsegs15 = 0;

			#elif	PC0_SSEG15	
				_pcs01 = 1; _pcs00 = 0;	_pcc0 = 0; _comsegs15 = 1;
			#endif		
		
			#ifdef	PC0_PU	
				_pcpu0 = 1;
			#endif
			
		#endif
		
	#endif
/***************** end of select PC0 function *****************/	

/******************* select PC1 function **********************/
	#ifndef FH_HXT_SE10M 
	
	   #ifndef FH_HXT_L10M 
	
			#ifdef	PC1_Outout_H	
				_pcs03 = 0; _pcs02 = 0; _pcc1 = 0; _pc1 = 1;			
					
			#elif	PC1_Outout_L	
				_pcs03 = 0; _pcs02 = 0; _pcc1 = 0; _pc1 = 0;
					
			#elif	PC1_Input	
				_pcs03 = 0; _pcs02 = 0; _pcc1 = 1; 
					
			#elif	PC1_Input_PU	
				_pcs03 = 0; _pcs02 = 0; _pcc1 = 1; _pcpu1 = 1;
				
			#elif	PC1_RX_TX
				_rxps = 1; _pcs03 = 0; _pcs02 = 1;
									
			#elif	PC1_SCOM16
				_pcs03 = 1; _pcs02 = 0; _pcc1 = 0; _comsegs16 = 0;

			#elif	PC1_SSEG16	
				_pcs03 = 1; _pcs02 = 0; _pcc1 = 0; _comsegs16 = 1;
			#endif		
		
			#ifdef	PC1_PU	
				_pcpu1 = 1;
			#endif
			
		#endif
		
	#endif
/***************** end of select PC1 function *****************/	

/******************* select PC2 function **********************/
	#ifdef	PC2_Outout_H	
		_rstc = 0b01010101; _pcs05 = 0; _pcs04 = 0; _pcc2 = 0; _pc2 = 1;					
			
	#elif	PC2_Outout_L	
		_rstc = 0b01010101; _pcs05 = 0; _pcs04 = 0; _pcc2 = 0; _pc2 = 0;		
			
	#elif	PC2_Input	
		_rstc = 0b01010101;
		_pcs05 = 0; _pcs04 = 0; _pcc2 = 1; 
			
	#elif	PC2_Input_PU	
		_rstc = 0b01010101; _pcs05 = 0; _pcs04 = 0; _pcc2 = 1; _pcpu2 = 1;	
		
	#elif	PC2_SDO
		_rstc = 0b01010101;	_pcs05 = 1; _pcs04 = 0;
		
	#elif	PC2_RESB
		_rstc = 0b10101010;	_pcs05 = 0; _pcs04 = 0;
			
	#elif	PC2_SCOM17
		_pcs05 = 1; _pcs04 = 1; _pcc2 = 0; _comsegs17 = 0;

	#elif	PC2_SSEG17	
		_pcs05 = 1; _pcs04 = 1; _pcc2 = 0; _comsegs17 = 1;
	#endif		

	#ifdef	PC2_PU	
		_pcpu2 = 1;
	#endif
/***************** end of select PC2 function *****************/

/******************* select PC3 function **********************/
	#ifdef	PC3_Outout_H	
		_pcs07 = 0; _pcs06 = 0; _pcc3 = 0; _pc3 = 1;			
			
	#elif	PC3_Outout_L	
		_pcs07 = 0; _pcs06 = 0; _pcc3 = 0; _pc3 = 0;
			
	#elif	PC3_Input	
		_pcs07 = 0; _pcs06 = 0; _pcc3 = 1; 
			
	#elif	PC3_Input_PU	
		_pcs07 = 0; _pcs06 = 0; _pcc3 = 1; _pcpu3 = 1;
		
	#elif	PC3_SDO
		_pcs07 = 1; _pcs06 = 0; 
			
	#elif	PC3_SCOM18
		_pcs07 = 1; _pcs06 = 1; _pcc3 = 0; _comsegs18 = 0;

	#elif	PC3_SSEG18	
		_pcs07 = 1; _pcs06 = 1; _pcc3 = 0; _comsegs18 = 1;
	#endif		

	#ifdef	PC3_PU	
		_pcpu3 = 1;
	#endif
/***************** end of select PC3 function *****************/	

/******************* select PC4 function **********************/
	#ifdef	PC4_Outout_H	
		_pcs11 = 0; _pcs10 = 0; _pcc4 = 0; _pc4 = 1;			
			
	#elif	PC4_Outout_L	
		_pcs11 = 0; _pcs10 = 0; _pcc4 = 0; _pc4 = 0;
			
	#elif	PC4_Input	
		_pcs11 = 0; _pcs10 = 0; _pcc4 = 1; 
			
	#elif	PC4_Input_PU	
		_pcs11 = 0; _pcs10 = 0; _pcc4 = 1; _pcpu4 = 1;
		
	#elif	PC4_SDI_SDA
		_sdi_sdaps = 0; _pcs11 = 1; _pcs10 = 0;
			
	#elif	PC4_SCOM19
		_pcs11 = 1; _pcs10 = 1; _pcc4 = 0; _comsegs19 = 0;

	#elif	PC4_SSEG19	
		_pcs11 = 1; _pcs10 = 1; _pcc4 = 0; _comsegs19 = 1;
	#endif		

	#ifdef	PC4_PU	
		_pcpu4 = 1;
	#endif
/***************** end of select PC4 function *****************/

/******************* select PC5 function **********************/
	#ifdef	PC5_Outout_H	
		_pcs13 = 0; _pcs12 = 0; _pcc5 = 0; _pc5 = 1;			
			
	#elif	PC5_Outout_L	
		_pcs13 = 0; _pcs12 = 0; _pcc5 = 0; _pc5 = 0;
			
	#elif	PC5_Input	
		_pcs13 = 0; _pcs12 = 0; _pcc5 = 1; 
			
	#elif	PC5_Input_PU	
		_pcs13 = 0; _pcs12 = 0; _pcc5 = 1; _pcpu5 = 1;
		
	#elif	PC5_SCK_SCL	
		_sck_sclps = 0; _pcs13 = 1; _pcs12 = 0;
		
	#elif	PC5_SCOM20
		_pcs13 = 1; _pcs12 = 1; _pcc5 = 0; _comsegs20 = 0;

	#elif	PC5_SSEG20	
		_pcs13 = 1; _pcs12 = 1; _pcc5 = 0; _comsegs20 = 1;	
	#endif		

	#ifdef	PC5_PU	
		_pcpu5 = 1;
	#endif
/***************** end of select PC5 function *****************/

/******************* select PC6 function **********************/
	#ifdef	PC6_Outout_H	
		_pcs15 = 0; _pcs14 = 0; _pcc6 = 0; _pc6 = 1;			
			
	#elif	PC6_Outout_L	
		_pcs15 = 0; _pcs14 = 0; _pcc6 = 0; _pc6 = 0;
			
	#elif	PC6_Input	
		_pcs15 = 0; _pcs14 = 0; _pcc6 = 1; 
			
	#elif	PC6_Input_PU	
		_pcs15 = 0; _pcs14 = 0; _pcc6 = 1; _pcpu6 = 1;
		
	#elif	PC6_SCSB
		_scsbps = 0; _pcs15 = 1; _pcs14 = 0;
				
	#elif	PC6_SCOM21
		_pcs15 = 1; _pcs14 = 1; _pcc6 = 0; _comsegs21 = 0;

	#elif	PC6_SSEG21	
		_pcs15 = 1; _pcs14 = 1; _pcc6 = 0; _comsegs21 = 1;
	#endif		

	#ifdef	PC6_PU	
		_pcpu6 = 1;
	#endif
/***************** end of select PC6 function *****************/


/******************* select PD0 function **********************/
	#ifdef	PD0_Outout_H	
		_pds01 = 0; _pds00 = 0; _pdc0 = 0; _pd0 = 1;			
			
	#elif	PD0_Outout_L	
		_pds01 = 0; _pds00 = 0; _pdc0 = 0; _pd0 = 0;
			
	#elif	PD0_Input	
		_pds01 = 0; _pds00 = 0; _pdc0 = 1; 
			
	#elif	PD0_Input_PU	
		_pds01 = 0; _pds00 = 0; _pdc0 = 1; _pdpu0 = 1;
		
	#elif	PD0_PTP	
		_pds01 = 1; _pds00 = 0;	_pdc0 = 0; _pd0 = 0;
			
	#elif	PD0_SSEG22	
		_pds01 = 1; _pds00 = 1;	_pdc0 = 0;
	
	#endif		

	#ifdef	PD0_PU	
		_pdpu0 = 1;
	#endif
/***************** end of select PD0 function *****************/	

/******************* select PD1 function **********************/
	#ifdef	PD1_Outout_H	
		_pds03 = 0; _pds02 = 0; _pdc1 = 0; _pd1 = 1;			
			
	#elif	PD1_Outout_L	
		_pds03 = 0; _pds02 = 0; _pdc1 = 0; _pd1 = 0;
			
	#elif	PD1_Input	
		_pds03 = 0; _pds02 = 0; _pdc1 = 1; 
			
	#elif	PD1_Input_PU	
		_pds03 = 0; _pds02 = 0; _pdc1 = 1; _pdpu1 = 1;
		
	#elif	PD1_RX_TX
		_rxps = 0; _pds03 = 0; _pds02 = 1; 
			
	#elif	PD1_SSEG23	
		_pds03 = 1; _pds02 = 0; _pdc1 = 0; 
		
	#elif	PD1_AN11	
		_pds03 = 1; _pds02 = 1; 
		
	#endif		

	#ifdef	PD1_PU	
		_pdpu1 = 1;
	#endif
/***************** end of select PD1 function *****************/	

/******************* select PD2 function **********************/
	#ifdef	PD2_Outout_H	
		_pds05 = 0; _pds04 = 0; _pdc2 = 0; _pd2 = 1;			
			
	#elif	PD2_Outout_L	
		_pds05 = 0; _pds04 = 0; _pdc2 = 0; _pd2 = 0;
			
	#elif	PD2_Input	
		_pds05 = 0; _pds04 = 0; _pdc2 = 1; 
			
	#elif	PD2_Input_PU	
		_pds05 = 0; _pds04 = 0; _pdc2 = 1; _pdpu2 = 1;
		
	#elif	PD2_TX
		_pds05 = 0; _pds04 = 1; 
			
	#elif	PD2_SSEG24	
		_pds05 = 1; _pds04 = 0; _pdc2 = 0;
		
	#elif	PD2_AN10
		_pds05 = 1; _pds04 = 1; 
	
	#endif		

	#ifdef	PD2_PU	
		_pdpu2 = 1;
	#endif
/***************** end of select PD2 function *****************/

/******************* select PD3 function **********************/
	#ifdef	PD3_Outout_H	
		_pds07 = 0; _pds06 = 0; _pdc3 = 0; _pd3 = 1;			
			
	#elif	PD3_Outout_L	
		_pds07 = 0; _pds06 = 0; _pdc3 = 0; _pd3 = 0;
			
	#elif	PD3_Input	
		_pds07 = 0; _pds06 = 0; _pdc3 = 1; 
			
	#elif	PD3_Input_PU	
		_pds07 = 0; _pds06 = 0; _pdc3 = 1; _pdpu3 = 1;
		
	#elif	PD3_CTP
		_pds07 = 0; _pds06 = 1; _pdc3 = 0; _pd3 = 0;

	#elif	PD3_SSEG25
		_pds07 = 1; _pds06 = 0; _pdc3 = 0;
		
	#elif	PD3_AN9
		_pds07 = 1; _pds06 = 1; 
		
	#endif		

	#ifdef	PD3_PU	
		_pdpu3 = 1;
	#endif
/***************** end of select PD3 function *****************/



/******************* select PA0~PA3 source current  **********************/
	#ifdef	PA0_PA3_Level_0	
		_sledc01 = 0; _sledc00 = 0;
	
	#elif	PA0_PA3_Level_1	
		_sledc01 = 0; _sledc00 = 1;
	
	#elif	PA0_PA3_Level_2	
		_sledc01 = 1; _sledc00 = 0;
	
	#elif	PA0_PA3_Level_3	
		_sledc01 = 1; _sledc00 = 1;
	
	#endif
/* end of select PA0~PA3 source current */

/******************* select PA4~PA7 source current  **********************/
	#ifdef	PA4_PA7_Level_0	
		_sledc03 = 0; _sledc02 = 0;
	
	#elif	PA4_PA7_Level_1	
		_sledc03 = 0; _sledc02 = 1;
	
	#elif	PA4_PA7_Level_2	
		_sledc03 = 1; _sledc02 = 0;
		
	#elif	PA4_PA7_Level_3	
		_sledc03 = 1; _sledc02 = 1;
	
	#endif
/* end of select PA4~PA7 source current */

/******************* select PB0~PB3 source current  **********************/
	#ifdef	PB0_PB3_Level_0	
		_sledc05 = 0; _sledc04 = 0;
	
	#elif	PB0_PB3_Level_1	
		_sledc05 = 0; _sledc04 = 1;
	
	#elif	PB0_PB3_Level_2	
		_sledc05 = 1; _sledc04 = 0;
	
	#elif	PB0_PB3_Level_3	
		_sledc05 = 1; _sledc04 = 1;
	
	#endif
/* end of select PB0~PB3 source current */

/******************* select PB4~PB6 source current  **********************/
	#ifdef	PB4_PB6_Level_0	
		_sledc07 = 0; _sledc06 = 0;
	
	#elif	PB4_PB6_Level_1	
		_sledc07 = 0; _sledc06 = 1;
	
	#elif	PB4_PB6_Level_2	
		_sledc07 = 1; _sledc06 = 0;
		
	#elif	PB4_PB6_Level_3	
		_sledc07 = 1; _sledc06 = 1;
	
	#endif
/* end of select PB4~PB6 source current */

/******************* select PC0~PC3 source current  **********************/
	#ifdef	PC0_PC3_Level_0	
		_sledc11 = 0; _sledc10 = 0;
	
	#elif	PC0_PC3_Level_1	
		_sledc11 = 0; _sledc10 = 1;
	
	#elif	PC0_PC3_Level_2	
		_sledc11 = 1; _sledc10 = 0;
	
	#elif	PC0_PC3_Level_3	
		_sledc11 = 1; _sledc10 = 1;
	
	#endif
/* end of select PC0~PC3 source current */

/******************* select PC4~PC6 source current  **********************/
	#ifdef	PC4_PC6_Level_0	
		_sledc13 = 0; _sledc12 = 0;
	
	#elif	PC4_PC6_Level_1	
		_sledc13 = 0; _sledc12 = 1;
	
	#elif	PC4_PC6_Level_2	
		_sledc13 = 1; _sledc12 = 0;
		
	#elif	PC4_PC6_Level_3	
		_sledc13 = 1; _sledc12 = 1;
	
	#endif
/* end of select PC4~PC6 source current */

/******************* select PD0~PD3 source current  **********************/
	#ifdef	PD0_PD3_Level_0	
		_sledc15 = 0; _sledc14 = 0;
	
	#elif	PD0_PD3_Level_1	
		_sledc15 = 0; _sledc14 = 1;
	
	#elif	PD0_PD3_Level_2	
		_sledc15 = 1; _sledc14 = 0;
	
	#elif	PD0_PD3_Level_3	
		_sledc15 = 1; _sledc14 = 1;
	
	#endif
/* end of select PD0~PD3 source current */
}
 
/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/
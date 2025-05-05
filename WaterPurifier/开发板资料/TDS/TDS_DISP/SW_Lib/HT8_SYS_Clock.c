/**
  ******************************************************************************
  * @file HT8_SYS_Clock.c
  * @brief This file provides all the system clock prescaler firmware functions.
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
#include "HT8_SYS_Clock.h"


/**
  * @brief Initializes the system clock according to the specified parameters.
  * @retval 
  * None
  */
void SysClock_Init()
{
/***************** High Frequency clock selection ******************/
#ifdef	FH_HIRC_8M	
	_hircen = 1; _hirc1 = 0;  _hirc0 = 0; while(!_hircf); _fhs = 0;		
		
#elif   FH_HIRC_12M	
	_hircen = 1; _hirc1 = 0;  _hirc0 = 1; while(!_hircf); _fhs = 0;			
			
#elif	FH_HIRC_16M	
	_hircen = 1; _hirc1 = 1;  _hirc0 = 0; while(!_hircf); _fhs = 0;		
		
#elif	FH_HXT_SE10M	//HXT frequency is smaller or equal than 10MHz
	_pcs01 = 1; _pcs00 = 1; _pcs03 = 1; _pcs02 = 1;	
	_hxtm = 0; _hxten = 1; while(!_hxtf); _fhs = 1;	
		
#elif	FH_HXT_L10M		//HXT frequency is larger than 10MHz
	_pcs01 = 1; _pcs00 = 1; _pcs03 = 1; _pcs02 = 1;	
	_hxtm = 1; _hxten = 1; while(!_hxtf); _fhs = 1;		
#endif			
/************** end of High Frequency clock selection **************/

/***************** Low Frequency clock selection ******************/
#ifdef	FSUB_LIRC
	_fss = 0;
			
#elif 	FSUB_LXT
	_pbs01 = 1; _pbs00 = 1;	_pbs03 = 1; _pbs02 = 1; 	
	_lxten = 1; while(!_lxtf); _fss = 1;
#endif			
/************** end of Low Frequency clock selection **************/	
	
/********************** system clock prescaler select **********************/
#ifdef	SYSCLOCK_FH
	_cks2 = 0; _cks1 = 0; _cks0 = 0;	//set Fsys as FH
	
#elif	SYSCLOCK_FH_DIV2
	_cks2 = 0; _cks1 = 0; _cks0 = 1;	//set Fsys as FH/2
	
#elif	SYSCLOCK_FH_DIV4
	_cks2 = 0; _cks1 = 1; _cks0 = 0;	//set Fsys as FH/4
	
#elif	SYSCLOCK_FH_DIV8
	_cks2 = 0; _cks1 = 1; _cks0 = 1;	//set Fsys as FH/8
	
#elif	SYSCLOCK_FH_DIV16
	_cks2 = 1; _cks1 = 0; _cks0 = 0;	//set Fsys as FH/16
	
#elif	SYSCLOCK_FH_DIV32
	_cks2 = 1; _cks1 = 0; _cks0 = 1;	//set Fsys as FH/32
	
#elif	SYSCLOCK_FH_DIV64
	_cks2 = 1; _cks1 = 1; _cks0 = 0;	//set Fsys as FH/64
	
#elif	SYSCLOCK_FSUB
	_cks2 = 1; _cks1 = 1; _cks0 = 1;	//set Fsys as FSUB(32.768K or 32K)
#endif	
/****************** end of system clock prescaler select *******************/
}

/**
  * @brief select enter halt mode.
  * @param[in] HALT_Mode: specifies the halt mode.
  * can have one of the values of @ref HALT_Mode_TypeDef.
  * @retval  none.
  */
void EnterHaltMode(u8 Halt_Mode)
{
	_scc &= 0xfc;
	_scc |= Halt_Mode;
	_halt();		
}
/* end of EnterHaltMode(u8 Halt_Mode) */

/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/
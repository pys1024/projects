/**
  ******************************************************************************
  * @file HT8_PTM.c
  * @brief This file provides all the PTM firmware functions.
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


#include "HT8_PTM.h"


/**
  * @brief PTM initialization function.
  * @param[in] Non.
  * @retval Non.
  */
void PTM_Init(void)
{
/******************** work mode select ********************/
	#ifdef	PTM_TIMER_COUNTER_MODE
		_ptm1 = 1; _ptm0 = 1;	//Select PTM timer/counter Mode
		
	#elif	PTM_PWM_OUTPUT_MODE
		_ptm1 = 1; _ptm0 = 0;	
		_ptio1 = 1; _ptio0 = 0; //Select PTM PWM Output Mode
		
	#elif	PTM_SINGLE_PULSE_OUTPUT_MODE
		_ptm1 = 1; _ptm0 = 0;	
		_ptio1 = 1; _ptio0 = 1;	//Select PTM Single Pulse Output Mode
	
	#elif	PTM_COMPARE_MATCH_MODE	//Select PTM Compare Match Output Mode
		_ptm1 = 0; _ptm0 = 0;
					
	#endif
/**************** end of work mode select ****************/		

/********************* clock select **********************/
	#ifdef	PTM_FSYS_DIV4
		_ptck2 = 0; _ptck1 = 0; _ptck0 = 0;	//Select PTM Counter clock Fsys/4
	
	#elif	PTM_FSYS
		_ptck2 = 0; _ptck1 = 0; _ptck0 = 1;	//Select PTM Counter clock Fsys
	
	#elif	PTM_FH_DIV16
		_ptck2 = 0; _ptck1 = 1; _ptck0 = 0; //Select PTM Counter clock FH/16
		
	#elif	PTM_FH_DIV64
		_ptck2 = 0; _ptck1 = 1; _ptck0 = 1; //Select PTM Counter clock FH/64
		
	#elif	PTM_FSUB
		_ptck2 = 1; _ptck1 = 0; _ptck0 = 0;	//Select PTM Counter clock Fsub
		
	#elif	PTM_TCK_RISING_EDGE
		_ptck2 = 1; _ptck1 = 1; _ptck0 = 0; //Select PTM Counter clock TCKn rising edge clock
		
	#elif	PTM_TCK_FALLING_EDGE
		_ptck2 = 1; _ptck1 = 1; _ptck0 = 1; //Select PTM Counter clock TCKn falling edge clock
		
	#endif	
/********************* end of clock select **********************/
		
/************ TMn Counter Clear condition selection *************/	
	#ifdef	PTM_CCRA_MATCH
		_ptcclr = 1;	//PTM Counter Clear condition selection PTM Comparator A match		
	#else
		_ptcclr = 0;	//PTM Counter Clear condition selection PTM Comparator P match	
	#endif
/********* end of TMn Counter Clear condition selection *********/	
}


/**
  * @brief PTM PWM configure function.
  * Specify the following parameters in HT8_PTM.h, 
  * 1.PTP pin Output control
  * 2.PTP Output polarity control
  * @param[in] Non.
  * @retval Non.
  */
void PTM_PwmOutputConfig(void)
{
/************* PTP Output control **************/		
	#ifdef	PTM_ACTIVE_LOW
		_ptoc = 0;					//active low
	#else
		_ptoc = 1;					//active high
	#endif
/********* end of PTP Output control **********/	
		
/******** PTP Output polarity control ********/
	#ifdef	PTM_NON_INVERTED
		_ptpol = 0;					//no inverted
	#else
		_ptpol = 1;					//inverted
	#endif
/******** PTP Output polarity control ********/	
}


/**
  * @brief PTM PWM update function.
  * @param[in] CCRA value.
  * the TempCCRA value is 1~1023;
  * @param[in] CCRP value.
  * the TempCCRP value is 0~1023,
  * if TempCCRP>0, PWM Duty=TempCCRA/TempCCRP,
  * if TempCCRP is 0, PWM Duty=TempCCRA/1024,
  * @retval Non.
  */
void PTM_PwmUpdate(u16 TempCCRA,u16 TempCCRP)
{
	_ptmal = TempCCRA & 0x00ff;	//
	_ptmah = TempCCRA >> 8;		//
	_ptmrpl = TempCCRP & 0x00ff;	//
	_ptmrph = TempCCRP >> 8;
}


/**
  * @brief PTM SinglePulse configure function.
  * Specify the following parameters in HT8_PTM.h, 
  * 1.PTP pin Output control
  * 2.PTP Output polarity control
  * @param[in] Non.
  * @retval Non.
  */
void PTM_SinglePulseOutputConfig(void)
{
/************* PTP Output control **************/		
	#ifdef	PTM_ACTIVE_LOW
		_ptoc = 0;					//active low
	#else
		_ptoc = 1;					//active high
	#endif
/********* end of PTP Output control **********/	
		
/******** PTP Output polarity control ********/
	#ifdef	PTM_NON_INVERTED
		_ptpol = 0;					//no inverted
	#else
		_ptpol = 1;					//inverted
	#endif
/******** PTP Output polarity control ********/	
}


/**
  * @brief PTM SinglePulse update function.
  * @param[in] CCRA value.
  * the value is 1~1023,pulse width is TempCCRA*Tclock.
  * @retval Non.
  */
void PTM_SinglePulseUpdate(u16 TempCCRA)
{
	_ptmal = TempCCRA & 0x00ff;	//
	_ptmah = TempCCRA >> 8;		//
}


/**
  * @brief PTM timer/counter mode period config function.
  * @param[in] period value,
  * select CCRA_MATCH,the value is 1~1023;
  * overflow time=TempPeriod * Tclock,
  * select CCRP_MATCH,the value is 0~1023.
  * if TempPeriod>0, overflow time=TempPeriod * Tclock, 
  * if TempPeriod is 0,overflow time=1024 * Tclock ;
  * @retval None.
  */
void PTM_CounterModeConfig(u16 TempPeriod)
{
#ifdef	PTM_CCRA_MATCH
	_ptmal = TempPeriod & 0x00ff;	//
	_ptmah = TempPeriod >> 8;		//
#else	
	_ptmrpl = TempPeriod & 0x00ff;	//
	_ptmrph = TempPeriod >> 8;		//
#endif		
}


/**
  * @brief PTM compare match output config function,
  * Specify the following parameters in HT8_PTM.h, 
  * 1.PTP pin output function select
  * 2.PTP pin Output initial status control
  * 3.PTP Output polarity control
  * @param[in] None.
  * @retval None.
  */
void PTM_CompareMatchOutputConfig()
{
/********** Select PTP pin output function **********/	
	#ifdef	PTM_NO_CHANGE
		_ptio1 = 0; _ptio0 = 0;	//
	
	#elif	PTM_OUTPUT_LOW
		_ptio1 = 0; _ptio0 = 1;	//
	
	#elif	PTM_OUTPUT_HIGH
		_ptio1 = 1; _ptio0 = 0;	//
	
	#elif	PTM_OUTPUT_TOGGLE
		_ptio1 = 1; _ptio0 = 1;	//
	#endif
/********** Select PTP pin output function **********/	

/***** PTP Output initialization status control *****/			
	#ifdef	PTM_INITIAL_LOW
		_ptoc = 0;				//
	#else
		_ptoc = 1;	
	#endif			
/** end of PTP Output initialization status control */		

/*********** PTP Output polarity control ************/	
	#ifdef	PTM_NON_INVERTED
		_ptpol = 0;				//
	#else
		_ptpol = 1;				//
	#endif
/******* end of PTP Output polarity control ********/
}


/**
  * @brief PTM compare match output update function,
  * @param[in] MatchTime value,
  * select CCRA_MATCH,the value is 1~1023;
  * match time=TempMatchTime * Tclock,
  * select CCRP_MATCH,the value is 0~1023.
  * if TempMatchTime>0, match time=TempMatchTime * Tclock, 
  * if TempMatchTime is 0,match time=1024 * Tclock ;
  * @retval None.
  */
void PTM_CompareMatchOutputUpdate(u16 TempMatchTime)
{
#ifdef	PTM_CCRA_MATCH
	_ptmal = TempMatchTime & 0x00ff;	//
	_ptmah = TempMatchTime >> 8;		//
#else	
	_ptmal = 1;	//
	_ptmah = 0;		//
	_ptmrpl = TempMatchTime & 0x00ff;	//
	_ptmrph = TempMatchTime >> 8;
#endif	
}


/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/
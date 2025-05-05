/**
  ******************************************************************************
  * @file HT8_STM.c
  * @brief This file provides all the STM firmware functions.
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


#include "HT8_STM.h"

/**
  * @brief STM initialization function.
  * @param[in] Non.
  * @retval Non.
  */
void STM_Init(void)
{
/******************** work mode select ********************/
	#ifdef	STM_TIMER_COUNTER_MODE
		_stm1 = 1; _stm0 = 1;	//Select STM timer/counter Mode		
		
	#elif	STM_PWM_OUTPUT_MODE
		_stm1 = 1; _stm0 = 0;	
		_stio1 = 1; _stio0 = 0;	 //Select STM PWM Output Mode	
	
	#elif	STM_SINGLE_PULSE_OUTPUT_MODE
		_stm1 = 1; _stm0 = 0;	
		_stio1 = 1; _stio0 = 1;	//Select STM Single Pulse Output Mode
		
	#elif	STM_COMPARE_MATCH_MODE	//Select STM Compare Match Output Mode
		_stm1 = 0; _stm0 = 0;	
	
	#endif
/** end of work mode select **/		


/********************* clock select **********************/
	#ifdef	STM_FSYS_DIV4
		_stck2 = 0; _stck1 = 0; _stck0 = 0;	//Select STM Counter clock Fsys/4
	
	#elif	STM_FSYS
		_stck2 = 0; _stck1 = 0; _stck0 = 1;	//Select STM Counter clock Fsys
	
	#elif	STM_FH_DIV16
		_stck2 = 0; _stck1 = 1; _stck0 = 0; //Select STM Counter clock FH/16
		
	#elif	STM_FH_DIV64
		_stck2 = 0; _stck1 = 1; _stck0 = 1; //Select STM Counter clock FH/64
		
	#elif	STM_FSUB
		_stck2 = 1; _stck1 = 0; _stck0 = 0;	//Select STM Counter clock Fsub

	#elif	STM_TCK_RISING_EDGE
		_stck2 = 1; _stck1 = 1; _stck0 = 0; //Select STM Counter clock TCKn rising edge clock
		
	#elif	STM_TCK_FALLING_EDGE
		_stck2 = 1; _stck1 = 1; _stck0 = 1; //Select STM Counter clock TCKn falling edge clock

	#endif	
/** end of clock select **/
		
/************ STM Counter Clear condition selection *************/	
	#ifdef	STM_CCRA_MATCH
		_stcclr = 1;	//STM Counter Clear condition selection STM Comparator A match		
	#else
		_stcclr = 0;	//STM Counter Clear condition selection STM Comparator P match	
	#endif
/********* end of STM Counter Clear condition selection *********/	
}


/**
  * @brief STM PWM configure function.
  * Specify the following parameters in HT8_STM.h,\n
  * 1.STP Output control\n
  * 2.STP Output polarity control\n
  * @param[in] Non.
  * @retval Non.
  */
void STM_PwmOutputConfig(void)
{
/************* STP Output control **************/		
	#ifdef	STM_ACTIVE_LOW
		_stoc = 0;					//active low
	#else
		_stoc = 1;					//active high
	#endif
/********* end of STP Output control ***********/	
		
/******** STP Output polarity control **********/
	#ifdef	STM_NON_INVERTED
		_stpol = 0;					//no inverted
	#else
		_stpol = 1;					//inverted
	#endif
/******** STP Output polarity control **********/	

/*********** PWM duty and period control *******/
	#ifdef	STM_CCRP_P_CCRA_D
		_stdpx = 0;
	#else
		_stdpx = 1;
	#endif
/* end of Output polarity control */
}


/**
  * @brief STM PWM update function.
  * @param[in] CCRA value.
  * the TempCCRA value is 1~65535;
  * @param[in] CCRP value.
  * the TempCCRP value is 0~255,
  * if TempCCRP>0, PWM Duty=TempCCRA/(256*TempCCRP),frequency=1/(256*TempCCRP*Tclock);
  * if TempCCRP is 0, PWM Duty=TempCCRA/65536,frequency=1/(65536*Tclock).
  * @retval Non.
  */
void STM_PwmUpdate(u16 TempCCRA,u8 TempCCRP)
{
	_stmal = TempCCRA & 0x00ff;	//
	_stmah = TempCCRA >> 8;		//
	
	_stmrp = TempCCRP;			
}


/**
  * @brief STM SinglePulse configure function.
  * Specify the following parameters in HT8_STM.h\n
  * 1.STP Output control\n
  * 2.STP Output polarity control\n
  * @param[in] Non.
  * @retval Non.
  */
void STM_SinglePulseOutputConfig(void)
{
/************* STP Output control **************/		
	#ifdef	STM_ACTIVE_LOW
		_stoc = 0;					//active low
	#else
		_stoc = 1;					//active high
	#endif
/********* end of STP Output control **********/	
	
/******** STP Output polarity control ********/
	#ifdef	STM_NON_INVERTED
		_stpol = 0;					//no inverted
	#else
		_stpol = 1;					//inverted
	#endif
/******** STP Output polarity control ********/	
}


/**
  * @brief STM SinglePulse update function.
  * @param[in] pulse width value.
  * the value is 1~65535,pulse width is TempCCRA*Tclock.
  * @retval Non.
  */
void STM_SinglePulseUpdate(u16 TempCCRA)
{
	_stmal = TempCCRA & 0x00ff;	//
	_stmah = TempCCRA >> 8;		//
}


  
/**
  * @brief STM timer/counter mode period config function.
  * @param[in] period value,
  * select CCRA_MATCH,the value is 1~65535;
  * overflow time=TempPeriod * Tclock,
  * select CCRP_MATCH,the value is 0~255.
  * if TempPeriod>0, overflow time=256 * TempPeriod * Tclock, 
  * if TempPeriod is 0,overflow time=65536 * Tclock ;
  * @retval None.
  */
void STM_CounterModeConfig(u16 TempPeriod)
{
#ifdef	STM_CCRA_MATCH
	_stmal = TempPeriod & 0x00ff;	//
	_stmah = TempPeriod >> 8;		//
#else	
	_stmrp = TempPeriod & 0x00ff;			
#endif		
}


/**
  * @brief STM compare match output config function,
  * Specify the following parameters in HT8_STM.h\n 
  * 1.STP Output function select;
  * 2.STP Output initial status control;
  * 3.STP Output polarity control;
  * @param[in] None.
  * @retval None.
  */
void STM_CompareMatchOutputConfig()
{
/********** Select STP pin output function **********/	
	#ifdef	STM_NO_CHANGE
		_stio1 = 0; _stio0 = 0;	//
	
	#elif	STM_OUTPUT_LOW
		_stio1 = 0; _stio0 = 1;	//
	
	#elif	STM_OUTPUT_HIGH
		_stio1 = 1; _stio0 = 0;	//
	
	#elif	STM_OUTPUT_TOGGLE
		_stio1 = 1; _stio0 = 1;	//
	#endif
/********** Select STP pin output function **********/	

/***** STP Output initialization status control *****/			
	#ifdef	STM_INITIAL_LOW
		_stoc = 0;				//
	#else
		_stoc = 1;	
	#endif			
/** end of STP Output initialization status control */		

/*********** STP Output polarity control ************/	
	#ifdef	STM_NON_INVERTED
		_stpol = 0;				//
	#else
		_stpol = 1;				//
	#endif
/******* end of STP Output polarity control ********/
}



/**
  * @brief STM compare match output update function,
  * @param[in] MatchTime value,
  * select CCRA_MATCH,the value is 1~65535;
  * match time=TempMatchTime * Tclock,
  * select CCRP_MATCH,the value is 0~255.
  * if TempMatchTime>0, match time=256 * TempMatchTime * Tclock, 
  * if TempMatchTime is 0,match time=65536 * Tclock ;
  * @retval None.
  */  
void STM_CompareMatchOutputUpdate(u16 TempMatchTime)
{
#ifdef	STM_CCRA_MATCH
	_stmal = TempMatchTime & 0x00ff;	//
	_stmah = TempMatchTime >> 8;		//
#else	
	_stmal = 1;							//
	_stmah = 0;							//	
	_stmrp = TempMatchTime & 0x00ff;			
#endif	
}

/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/
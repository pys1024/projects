/**
  ******************************************************************************
  * @file HT8_CTM.c
  * @brief This file provides all the CTM firmware functions.
  * @author Holtek Semiconductor Inc.
  * @version V1.0.0
  * @date 2019-08-02
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


#include "HT8_CTM.h"



/**
  * @brief CTM initialization function.
  * @param[in] Non.
  * @retval Non.
  */
void CTM_Init(void)
{
/******************** work mode select ********************/
	#ifdef	CTM_TIMER_COUNTER_MODE
		_ctm1 = 1; _ctm0 = 1;	//Select CTM timer/counter Mode
		
	#elif	CTM_PWM_OUTPUT_MODE
		_ctm1 = 1; _ctm0 = 0;	//Select CTM PWM Output Mode
		_ctio1 = 1; _ctio0 = 0;		//
	
	#elif	CTM_COMPARE_MATCH_MODE	//Select CTM Compare Match Output Mode
		_ctm1 = 0; _ctm0 = 0;
		
	#endif
/**************** end of work mode select ****************/		

/********************* clock select **********************/
	#ifdef	CTM_FSYS_DIV4
		_ctck2 = 0; _ctck1 = 0; _ctck0 = 0;	//Select CTM Counter clock Fsys/4
	
	#elif	CTM_FSYS
		_ctck2 = 0; _ctck1 = 0; _ctck0 = 1;	//Select CTM Counter clock Fsys
	
	#elif	CTM_FH_DIV16
		_ctck2 = 0; _ctck1 = 1; _ctck0 = 0; //Select CTM Counter clock FH/16
		
	#elif	CTM_FH_DIV64
		_ctck2 = 0; _ctck1 = 1; _ctck0 = 1; //Select CTM Counter clock FH/64
		
	#elif	CTM_FSUB
		_ctck2 = 1; _ctck1 = 0; _ctck0 = 0;	//Select CTM Counter clock Fsub
		
	#elif	CTM_TCK_RISING_EDGE
		_ctck2 = 1; _ctck1 = 1; _ctck0 = 0; //Select CTM Counter clock TCKn rising edge clock
		
	#elif	CTM_TCK_FALLING_EDGE
		_ctck2 = 1; _ctck1 = 1; _ctck0 = 1;//Select CTM Counter clock TCKn falling edge clock
		
	#endif	
/********************* end of clock select **********************/
		
/************ TMn Counter Clear condition selection *************/	
	#ifdef	CTM_CCRA_MATCH
		_ctcclr = 1;	//CTM Counter Clear condition selection CTM Comparator A match		
	#else
		_ctcclr = 0;	//CTM Counter Clear condition selection CTM Comparator P match	
	#endif
/********* end of TMn Counter Clear condition selection *********/	
}


/**
  * @brief CTM PWM configure function.
  * Specify the following parameters in HT8_CTM.h, 
  * 1¡BCTP pin output function select
  * 2¡BCTP pin Output control
  * 3¡BCTP Output polarity control
  * 4¡BCTM PWM duty/period control
  * @param[in] Non.
  * @retval Non.
  */
void CTM_PwmOutputConfig(void)
{
/************* CTP Output control **************/		
	#ifdef	CTM_ACTIVE_LOW
		_ctoc = 0;					//active low
	#else
		_ctoc = 1;					//active high
	#endif
/********* end of CTP Output control **********/	
		
/******** CTP Output polarity control ********/
	#ifdef	CTM_NON_INVERTED
		_ctpol = 0;					//no inverted
	#else
		_ctpol = 1;					//inverted
	#endif
/******** CTP Output polarity control ********/

/******** CTM PWM duty/period control ********/		
	#ifdef	CTM_CCRP_P_CCRA_D
		_ctdpx = 0;					//CCRP_P_CCRA_D
	#else
		_ctdpx = 1;					//CCRP_D_CCRA_P
	#endif
/**** end of CTM PWM duty/period control ****/		
}


/**
  * @brief CTM PWM update function.
  * @param[in] CCRA value.
  * the TempCCRA value is 1~65535;
  * @param[in] CCRP value.
  * the TempCCRP value is 0~255,
  * if TempCCRP>0, PWM Duty=TempCCRA/(256*TempCCRP),frequency=1/(256*TempCCRP*Tclock);
  * if TempCCRP is 0, PWM Duty=TempCCRA/65536,frequency=1/(65536*Tclock).
  * @retval Non.
  */
void CTM_PwmUpdate(u16 TempCCRA,u8 TempCCRP)
{
	_ctmal = TempCCRA & 0x00ff;	//
	_ctmah = TempCCRA>>8;		//
	_ctmrp = TempCCRP;			//
}


/**
  * @brief CTM timer/counter mode period config function.
  * @param[in] period value,
  * select CCRA_MATCH,the value is 1~65535;
  * overflow time=TempPeriod * Tclock,
  * select CCRP_MATCH,the value is 0~255.
  * if TempPeriod>0, overflow time=256 * TempPeriod * Tclock, 
  * if TempPeriod is 0,overflow time=65536 * Tclock ;
  * @retval None.
  */
void CTM_CounterModeConfig(u16 TempPeriod)
{
#ifdef	CTM_CCRA_MATCH
	_ctmal = TempPeriod & 0x00ff;	//
	_ctmah = TempPeriod >> 8;		//
#else	
	_ctmrp = TempPeriod & 0x00ff;	//
#endif		
}


/**
  * @brief CTM compare match output config function,
  * Specify the following parameters in HT8_CTM.h, 
  * 1¡BCTP pin output function select
  * 2¡BCTP pin Output initial status control
  * 3¡BCTP Output polarity control
  * @param[in] None.
  * @retval None.
  */
void CTM_CompareMatchOutputConfig()
{
/********** Select CTP pin output function **********/	
	#ifdef	CTM_NO_CHANGE
		_ctio1 = 0; _ctio0 = 0;	//
	
	#elif	CTM_OUTPUT_LOW
		_ctio1 = 0; _ctio0 = 1;	//
	
	#elif	CTM_OUTPUT_HIGH
		_ctio1 = 1; _ctio0 = 0;	//
	
	#elif	CTM_OUTPUT_TOGGLE
		_ctio1 = 1; _ctio0 = 1;	//
	#endif
/********** Select CTP pin output function **********/	

/***** CTP Output initialization status control *****/			
	#ifdef	CTM_INITIAL_LOW
		_ctoc = 0;				//
	#else
		_ctoc = 1;	
	#endif			
/** end of CTP Output initialization status control */		

/*********** CTP Output polarity control ************/	
	#ifdef	CTM_NON_INVERTED
		_ctpol = 0;				//
	#else
		_ctpol = 1;				//
	#endif
/******* end of CTP Output polarity control ********/
}


/**
  * @brief CTM compare match output update function,
  * @param[in] MatchTime value,
  * select CCRA_MATCH,the value is 1~65535;
  * match time=TempMatchTime * Tclock,
  * select CCRP_MATCH,the value is 0~255.
  * if TempMatchTime>0, match time=256 * TempMatchTime * Tclock, 
  * if TempMatchTime is 0,match time=65536 * Tclock ;
  * @retval None.
  */
void CTM_CompareMatchOutputUpdate(u16 TempMatchTime)
{
#ifdef	CTM_CCRA_MATCH
	_ctmal = TempMatchTime & 0x00ff;	//
	_ctmah = TempMatchTime >> 8;		//
#else	
	_ctmal = 1;
	_ctmah = 0;
	_ctmrp = TempMatchTime & 0x00ff;	//
#endif	
}

/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/
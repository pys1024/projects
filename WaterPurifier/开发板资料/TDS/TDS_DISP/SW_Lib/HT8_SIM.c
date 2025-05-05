/**
  ******************************************************************************
  * @file HT8_SIM.c
  * @brief This file provides all the SIM firmware functions.
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


#include "HT8_SIM.h"

void SIM_Init()
{
#ifdef	SIM_I2C_Slave
	
	_sim2 =1; _sim1 = 1; _sim0 = 0;	//select I2C slave mode
	_sima = SIM_I2C_DEVICEADDR;   		//I2C slave address 
	#ifdef	SIM_I2C_DEBOUNCE_DISABLE
		_simdeb1 = 0; _simdeb0 = 0;		//no debounce time
	#elif	SIM_I2C_DEBOUNCE_2_FSYS
		_simdeb1 = 0; _simdeb0 = 1;		//debounce time 2 Fsys
	#else
		_simdeb1 = 1; _simdeb0 = 0;		//debounce time 4 Fsys
	#endif		
	
#elif	SIM_SPI_Master
	
	/*SIM SPI SCS pin control selectr*/
	#ifdef	SIM_SPI_CS_ON
			_csen=1;	//SCS pin control enable
	#elif	SIM_SPI_CS_OFF
			_csen=0;	//SCS pin control disable
	#endif	
	
	/*SIM SPI SCK clock active edge type selection*/
	#ifdef SIM_SPI_SCK_HIGH_RISING_EDGE
			_ckpolb=0; _ckeg=0;	 //SCK is high base level and data capture at SCK rising edge	
	#elif  SIM_SPI_SCK_HIGH_FALLING_EDGE 
			_ckpolb=0; _ckeg=1; //SCK is high base level and data capture at SCK falling edge	
	#elif  SIM_SPI_SCK_LOW_FALLING_EDGE	
			_ckpolb=1; _ckeg=0; //SCK is low base level and data capture at SCK falling edge		
	#elif  SIM_SPI_SCK_LOW_RISING_EDGE 
			_ckpolb=1; _ckeg=1;	//SCK is low base level and data capture at SCK rising edge
	#endif
	
	/*SIM SPI data shift order*/
	#ifdef	SIM_SPI_LSB_FIRST
			_mls=0;	//LSB first
	#elif	SIM_SPI_MSB_FIRST
			_mls=1;	//MSB first    
	#endif

	/*SIM SPI master clock select*/
	#ifdef	SIM_SPI_Master_FSYS_DIV4
			_sim2=0; _sim1=0; _sim0=0;	//SPI master clock is fSYS /4		
	#elif	SIM_SPI_Master_FSYS_DIV16
			_sim2=0; _sim1=0; _sim0=1;	//SPI master clock is fSYS /16	
	#elif	SIM_SPI_Master_FSYS_DIV64
			_sim2=0; _sim1=1; _sim0=0;	//SPI master clock is fSYS /64	
	#elif	SIM_SPI_Master_FSUB
			_sim2=0; _sim1=1; _sim0=1;	//SPI master clock is fSUB	
	#elif	SIM_SPI_Master_FPTM
			_sim2=1; _sim1=0; _sim0=0;	//SPI master clock is PTM CCRP match frequency/2
	#endif		


#elif	SIM_SPI_Slave
	
	/*SIM SPI SCK clock active edge type selection*/
	#ifdef SIM_SPI_SCK_HIGH_RISING_EDGE
			_ckpolb=0; _ckeg=0;	 //SCK is high base level and data capture at SCK rising edge	
	#elif  SIM_SPI_SCK_HIGH_FALLING_EDGE 
			_ckpolb=0; _ckeg=1; //SCK is high base level and data capture at SCK falling edge	
	#elif  SIM_SPI_SCK_LOW_FALLING_EDGE	
			_ckpolb=1; _ckeg=0; //SCK is low base level and data capture at SCK falling edge		
	#elif  SIM_SPI_SCK_LOW_RISING_EDGE 
			_ckpolb=1; _ckeg=1;	//SCK is low base level and data capture at SCK rising edge
	#endif
	
	/*SIM SPI data shift order*/
	#ifdef	SIM_SPI_LSB_FIRST
			_mls=0;	//LSB first
	#elif	SIM_SPI_MSB_FIRST
			_mls=1;	//MSB first    
	#endif	

	/*SIM SPI SCS pin control selectr*/
	#ifdef	SIM_SPI_CS_ON
			_csen=1;	//SCS pin control enable
	#elif	SIM_SPI_CS_OFF
			_csen=0;	//SCS pin control disable 
	#endif	
	_sim2=1; _sim1=0; _sim0=1; //Select SPI slave mode	
	
#endif
}


/**
  * @brief I2C Time-out update function,
  * @param[in] Time-out value,
  * the value is 0~63.
  * time= (timeout+1) * Tsub * 32, 
  * @retval None.
  */
void SIM_I2C_Timeout_Update(u8 timeout)
{
	_simtoc &= 0xc0;
	timeout &= 0x3f;
	_simtoc |= timeout;
}


/**
  * @brief SIM SPI peripheral Transmits a Data and read data from simd.
  * @par Parameters:
  * @TX_data : Data Byte to be transmitted.
  * @retval: RX_data,read data from simd
  */
u8 SIM_SPI_MasterSendData(u8 TX_data)
{
	u8 RX_data=0;
	do
	{
		SIM_SPI_WRITE_COLLISION_FLAG = 0;
		
		_simd = TX_data;			//writer data to simd
		GCC_CLRWDT();	
		GCC_DELAY(20);
		
	}while(SIM_SPI_WRITE_COLLISION_FLAG == 1);	 //check Write Collisio
	
	while(SIM_SPI_TRX_FLAG == 0)	//Is spi data transfer completed?
	{
		GCC_CLRWDT();
	}
	SIM_SPI_TRX_FLAG = 0;			//clear trf
	RX_data = _simd;			//read data from simd	
	return(RX_data);
}


void __attribute((interrupt(0x28))) SIM_ISR(void)  //I2C interrupt service routine
{
#ifdef	SIM_I2C_Slave

	if (!_simtof)  			//not I2C timeout
	{
		if (1 == _haas)   	//_haas=1,address match trig interrupt	
		{
			if (1 == _srw)  //srw=1:slave in transfer mode
			{
					_htx = 1;					
					_simd = _acc; 		//write data to _simd to release SCL line
			}
			else  //srw=0:slave in receive mode
			{
				_htx  = 0;
				_txak = 0;
				_acc  = _simd;		//dummy read from _simd to release SCL line
			}
		}
		else	//_haas=0,data trig interrupt
		{
			if (1 == _htx)  		//htx=1:slave in write state;
			{
				if (1 == _rxak)  	//rxak=1:master stop receiving next byte,master releases scl bus
				{
					_htx  = 0; 
					_txak = 0;
					_acc  = _simd;	//dummy read from _simd to release SCL line
				}	
				else 				//rxak=0:master wants to receive next byte;
				{
					_simd = _acc;	//send data, User can program here 
					
				}
			}
			else  //htx=0:slave in read state
			{
				_acc  = _simd;		// read data, User can program here 
			}
		}	
	}	
	else	//I2C communication timeout
	{
		_simtof  = 0;
		_simtoen = 1;
		/* user define,such as set a I2C timeout flag */
	}
	
#elif	SIM_SPI_Slave

	while(SIM_SPI_TRX_FLAG == 0)	//Is spi data transfer completed?
	{
		GCC_CLRWDT();
	}
	_acc = _simd;	//receive data, User can program here 		
	SIM_SPI_TRX_FLAG = 0;			//clear trf

	do
	{
		SIM_SPI_WRITE_COLLISION_FLAG = 0;
		_simd = _acc;					//write data to simd,the data will be transmit next interrupt, User can program here
		GCC_CLRWDT();	
	}while(SIM_SPI_WRITE_COLLISION_FLAG==1); 	//ceck Write Collision
	
#endif

	SIM_CLEAR_FLAG();  //clear SIM interrupt flag
}


/******************* (C) COPYRIGHT 2019 Holtek Semiconductor Inc *****END OF FILE****/
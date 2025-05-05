#include "HT8_UART.h"




unsigned char Rx_Count;				// A variable for UART & IIC recive data
unsigned char Tx_Count;				// A variable for UART & IIC send data


volatile unsigned char Uart_4msCnt;	// A variable for Timing UART


volatile unsigned char Rx_databuf[11];	//An array to store datas the UART or IIC received
volatile unsigned char Tx_databuf[8];	//An array to store datas the UART or IIC needs to send



void Clear_Rx_databuf()
{
	unsigned char i;
	for(i = 0;i < 11;i++)
	{
		Rx_databuf[i] = 0;
	}
}


void Clear_Tx_databuf()
{
	unsigned char i;
	for(i = 0;i < 8;i++)
	{
		Tx_databuf[i] = 0;
	}
}


volatile bit F_UART;		// UART flag used to send data

void Communicate_Init()
{

	_pds05 = 0;				//TX pin is PD2
	_pds04 = 1;	

	_pds03 = 0;				//RX pin is PD1
	_pds02 = 1;	
	_ifs &= 0xfe;

	
	_ucr1 = 0b10000000;		// Enable UART, 8bit data, none parity check, 1 stop bit
	_ucr2 = 0b00000000;
	_wake = 1;				// Enable UART wakeup function 
	_brgh = 1;				// Baud rate:9600
	_brg = 51;	
	_txen = 1;				// Enable UART send data function
	_rxen = 1;				// Enable UART receive data function
	_rie = 1;				// Enable UART receive interrupt
	_urf = 0;
	_ure = 1;				// Enable UART interrupt
	_emi = 1;				// Enable global interrupt	
}

/**
  * @brief UART Interruption routine,receive data.
  * @par Parameters:
  * None
  * @retval
  * None
  */
void __attribute((interrupt(0x2c))) UART_ISR(void)
{
	if(_rxif == 1)
	{	
		Uart_4msCnt = 0;
		if(Rx_Count<11)
		{
			Rx_databuf[Rx_Count] = _txr_rxr;
			Rx_Count ++;

			if(_rxif == 1)
			{
				Uart_4msCnt = 0;
				if(Rx_Count<11)
				{	
					Rx_databuf[Rx_Count] = _txr_rxr;
					Rx_Count ++;
				}
				else _acc= _txr_rxr;
			}
		}
		else _acc= _txr_rxr;
	}		
}

/**
  * @brief UART send data.
  * @par Parameters:
  * None
  * @retval
  * None
  */


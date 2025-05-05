
#include "cc_generated_files\CodeConfig.h"

#define u8 unsigned char
#define u16 unsigned int

#define WaitStatus     0     //按键过程的状态
#define PressStatus    1
#define ReleaseStatus  2
#define IDEStatus      3
#define KeyTime       80  //长按超时时间 

u8 KeyStatus=0;
u8 KeyVal=0;
u16 KeyCnt=0;
u8 KeyPress=0;//1按下，2长按

extern unsigned char Rx_Count;				// A variable for UART & IIC recive data
extern unsigned char Tx_Count;				// A variable for UART & IIC send data
extern volatile bit F_UART;

extern volatile unsigned char Uart_4msCnt;	// A variable for Timing UART


extern volatile unsigned char Rx_databuf[11];	//An array to store datas the UART or IIC received
extern volatile unsigned char Tx_databuf[8];	//An array to store datas the UART or IIC needs to send

volatile unsigned char dispbuff[8];

const u8 dispcode[10]={0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F};

u16 tds1=0;
u16 tds2=0;
u16 temp1=0;
u16 temp2=0;

volatile bit f10ms=0;
bit flash,loop;
u8 cnt500ms=0;
bit f500ms=0;
u8 cnt5s=0;
u8 cnt1s=0;
u8 workmode=0;
u8 calstep=0;
u8 dispmode=0;

void KeyScan();


void delay_ms(u16 ms)
{
	while(ms--)
	{
		GCC_DELAY(2000);
		GCC_CLRWDT();
	}	
	
}


void setbuff(u8 data)
{
	u8 i;
	for(i=0;i<8;i++)dispbuff[i]=data;
}


void disptds_l(u16 data)
{
	if(data==0xffff)
	{
		dispbuff[0]=0;
		dispbuff[1]=0;
		dispbuff[2]=0;
		dispbuff[3]=0;	
	}
	else if(data==0xfffe)
	{
		dispbuff[0]=0x40;
		dispbuff[1]=0x40;
		dispbuff[2]=0x40;
		dispbuff[3]=0x40;	
	}
	else
	{
		dispbuff[0]=dispcode[data/1000%10];
		dispbuff[1]=dispcode[data/100%10];
		dispbuff[2]=dispcode[data/10%10];
		dispbuff[3]=dispcode[data%10];	
	}
}

void disptds_r(u16 data)
{
	if(data==0xffff)
	{
		dispbuff[4]=0;
		dispbuff[5]=0;
		dispbuff[6]=0;
		dispbuff[7]=0;	
	}
	else if(data==0xfffe)
	{
		dispbuff[4]=0x40;
		dispbuff[5]=0x40;
		dispbuff[6]=0x40;
		dispbuff[7]=0x40;	
	}
	else
	{
		dispbuff[4]=dispcode[data/1000%10];
		dispbuff[5]=dispcode[data/100%10];
		dispbuff[6]=dispcode[data/10%10];
		dispbuff[7]=dispcode[data%10];	
	}
}


void disptemp_l(u16 data)
{
	dispbuff[0]=0;
	dispbuff[1]=dispcode[data/100%10];
	dispbuff[2]=(dispcode[data/10%10]|0x80);
	dispbuff[3]=dispcode[data%10];	
}

void disptemp_r(u16 data)
{
	dispbuff[4]=0;
	dispbuff[5]=dispcode[data/100%10];
	dispbuff[6]=(dispcode[data/10%10]|0x80);
	dispbuff[7]=dispcode[data%10];	
}

void SendData()
{
	if(F_UART == 1)
	{
		F_UART = 0;	
		unsigned char i = 0;
		
		Tx_databuf[0]=0x55;
		Tx_databuf[1]=0x07;
		Tx_databuf[7]=Tx_databuf[0]+Tx_databuf[1]+Tx_databuf[2]+Tx_databuf[3]+Tx_databuf[4]+Tx_databuf[5]+Tx_databuf[6];
		while(i < 8)
		{
			if(_tidle == 1)
			{
				GCC_NOP();
				GCC_DELAY(100);
				if(_txif == 1)
				{
					GCC_DELAY(100);
					_txr_rxr = Tx_databuf[i];		
					i++;
				}
			}
		}
		Clear_Tx_databuf();
	}
}


void Rx_Data_Handle()
{
	unsigned char checksum;
	u16 val=0;
	if(Rx_databuf[0] == 0x55)
	{
		checksum = Rx_databuf[0]+Rx_databuf[1]+Rx_databuf[2]+Rx_databuf[3]+Rx_databuf[4]+Rx_databuf[5]+Rx_databuf[6]+Rx_databuf[7]+Rx_databuf[8]+Rx_databuf[9];
		if(Rx_databuf[10] == checksum)
		{
			switch(Rx_databuf[2])
			{
				case 0x83:						//TDS校准应答信号
						if(calstep==0)
						calstep=1;
						else if(calstep==1)
						calstep=2;
						
				break;

				case 0x85:						//电导率和温度
					
					if(Rx_databuf[3]==0x01)
					{
						val=Rx_databuf[4];
						val<<=8;
						val+=Rx_databuf[5];
						tds1=val/20;
						
						val=Rx_databuf[6];
						val<<=8;
						val+=Rx_databuf[7];
						temp1=val;
					}
					else if(Rx_databuf[3]==0x02)
					{
						val=Rx_databuf[4];
						val<<=8;
						val+=Rx_databuf[5];
						tds2=val/20;
						
						val=Rx_databuf[6];
						val<<=8;
						val+=Rx_databuf[7];
						temp2=val;
					}
					
					
					
					
					
				break;
				default: break;
			}
			Clear_Rx_databuf();
		}
	}
}



void main() 
{
	
	
    System_Init();
    System_Setting();
    GCC_DELAY(DELAY_CNT); 
	F_UART=0;
	
	
	_sledc0=0;
	_sledc1=0;
	
	
	
    while (1)
    {
        GCC_CLRWDT();
        
		if(Uart_4msCnt>100)
		{
			Uart_4msCnt = 0;
			Rx_Count = 0;
			Rx_Data_Handle();				// Prosessing communication data
		}
		SendData();
		
		if(f10ms)
		{
			f10ms=0;
			
			if(++cnt500ms>=50)
			{
				cnt500ms=0;
				if(++cnt5s>=10)
				{
					cnt5s=0;
					loop=~loop;
				}
				
				f500ms=1;
				flash=~flash;	
			}
			
				
			KeyScan();
			
			switch(workmode)
			{
				case 0://正常显示模式
					if(f500ms)
					{
						f500ms=0;
						if(flash)
						{
							Tx_databuf[2]=0x05;
							Tx_databuf[3]=0x01;//ch
							Tx_databuf[4]=0x00;
							Tx_databuf[5]=0x00;
							Tx_databuf[6]=0x00;
						}
						else
						{
							Tx_databuf[2]=0x05;
							Tx_databuf[3]=0x02;//ch
							Tx_databuf[4]=0x00;
							Tx_databuf[5]=0x00;
							Tx_databuf[6]=0x00;
						}
						F_UART=1;	
					}
			
					switch(dispmode)
					{
						case 0://tds
							disptds_l(tds1);
							disptds_r(tds2);
						break;	
						case 1://temp
							disptemp_l(temp1);
							disptemp_r(temp2);
						break;
						case 2://tds && temp 5s

							if(loop)	
							{
								disptds_l(tds1);
								disptds_r(tds2);
							}
							else
							{
								disptemp_l(temp1);
								disptemp_r(temp2);	
							}
						
						break;
					}
				
				
				
				
				break;
				case 1://校准模式
					switch(calstep)
					{
						case 0://闪烁显示tds1的500校准点
							if(flash)
							{
								disptds_l(0xffff);	
							}
							else
							{
								disptds_l(500);	
							}
							
							disptds_r(0xfffe);
						break;	
						case 1://闪烁显示tds1的500校准点
							if(flash)
							{
								disptds_r(0xffff);	
							}
							else
							{
								disptds_r(500);	
							}
							
							disptds_l(0xfffe);
						break;
						case 2:
						
							disptds_l(0xfffe);
							disptds_r(0xfffe);
							cnt5s=0;
							cnt1s=0;
						break;
					}
				break;	
			}
			
			
			if(KeyPress==2)
			{
				KeyPress=0;
				
				if(KeyVal==1)
				{
					if(workmode==0)
					{
						workmode=1;
						calstep=0;
					}
					else
					{
						workmode=0;
						calstep=0;
					}	
					
				}
				
			}
			else if(KeyPress==3)
			{
				u8 i;
				KeyPress=0;
				if(KeyVal==1)
				{
					if(workmode==1)
					{
						if(calstep==0)
						{
							//发送校准指令	
							Tx_databuf[2]=0x03;
							Tx_databuf[3]=0x01;//ch
							Tx_databuf[4]=0x01;
							Tx_databuf[5]=0x13;//hex(5000)=0x1388 500us/cm
							Tx_databuf[6]=0x88;
							F_UART=1;
						}
						else if(calstep==1)
						{
							//发送校准指令	
							Tx_databuf[2]=0x03;
							Tx_databuf[3]=0x02;//ch
							Tx_databuf[4]=0x01;
							Tx_databuf[5]=0x13;//hex(5000)=0x1388 500us/cm
							Tx_databuf[6]=0x88;
							F_UART=1;
						}
						else
						{
							//exit cal
							workmode=0;
							calstep=0;	
						}
					}	
				}
				else if(KeyVal==2)
				{
					if(workmode==0)
					{
						if(dispmode==0)
						{
							dispmode=1;
						}	
						else if(dispmode==1)
						{
							dispmode=2;
						}
						else if(dispmode==2)
						{
							dispmode=0;
						}
						
						for(i=0;i<3;i++)
						{
							disptds_l(dispmode+1);
							delay_ms(200);	
							disptds_l(0xffff);
							delay_ms(200);
						}
					}
				}
				
			}
			
		}
		
		
    }
}

void __attribute((interrupt(0x10))) PTM_CTM_ISR(void)
{
	
	static u8 cnt=0,cnt10ms=0;
	static u8 gap=0;
	u8 temp=0;
	
	if (_ctmaf == 1)//2ms
	{
		_ctmaf = 0;
		
		if(++cnt10ms>=20)
		{
			cnt10ms=0;
			f10ms=1;	
		}
		
		Uart_4msCnt++;
		if(Uart_4msCnt>200)
		{
			Uart_4msCnt = 200;
		}	
		seg1=0;
		seg2=0;
		seg3=0;
		seg4=0;
		seg5=0;
		seg6=0;
		seg7=0;
		seg8=0;
		com1=1;
		com2=1;
		com3=1;
		com4=1;
		com5=1;
		com6=1;
		com7=1;
		com8=1;
		
		
		if(++gap>=4)
		{
			gap=0;
			temp=dispbuff[cnt];
			
			if(temp&0x01)seg1=1;else seg1=0;
			if(temp&0x02)seg2=1;else seg2=0;
			if(temp&0x04)seg3=1;else seg3=0;
			if(temp&0x08)seg4=1;else seg4=0;
			if(temp&0x10)seg5=1;else seg5=0;
			if(temp&0x20)seg6=1;else seg6=0;
			if(temp&0x40)seg7=1;else seg7=0;
			if(temp&0x80)seg8=1;else seg8=0;
			
			switch(cnt)
			{
				case 0:com1=0;cnt++;break;
				case 1:com2=0;cnt++;break;
				case 2:com3=0;cnt++;break;
				case 3:com4=0;cnt++;break;
				case 4:com5=0;cnt++;break;
				case 5:com6=0;cnt++;break;
				case 6:com7=0;cnt++;break;
				case 7:com8=0;cnt=0;break;	
			}
		}
	}
}

u8 getkey()
{
	if(key1==0)return 1;
	else if(key2==0)return 2;
	
	return 0;
}

void KeyScan()
{
	static u8 temp;
	
	temp=getkey();
	switch(KeyStatus)
	{
		case WaitStatus:       //等待状态
				
			  if(temp!=0x00)
			  {
                                  
				KeyStatus=PressStatus;
			  }
			  break;
	   case PressStatus:     //按下状态
			if(temp!=0x00)
			{
				KeyVal=temp;
				KeyPress=1;
				KeyStatus=ReleaseStatus;
			}
			else
			{
				KeyStatus=WaitStatus;
			}
			break;

		case ReleaseStatus:   //等待释放状态
			if(temp!=0x00)
			{
				KeyVal=temp;
				if(++KeyCnt==KeyTime)  //长按检测
				{
					KeyPress=2;
					KeyCnt=0;
					KeyStatus=IDEStatus;
				}
			}
			else
			{
				KeyPress=3;
				KeyCnt=0;
				KeyStatus=WaitStatus;
			}
             
			break;
		case IDEStatus:
            
               
			if(temp==0x00)
			{
				KeyStatus=WaitStatus;
			}
		break;
        default:KeyStatus=WaitStatus;break;
	}
}


/**
  ******************************************************************************
  * @file    EST Brick of Dr.luck/input.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#include "input.h"
#include "ads7957.h"
#include "stm324xg_eval.h"
#include "disassembly.h"
#include "excutefunction.h"
#include "outputworkmode.h"
#include "usart_sensor.h"
#include "iic_sensor.h"
#include "C_Protocol.h"

void SetByte(uint32_t rambase, uint8_t par);
void SetFloat(uint32_t rambase, float par);
uint8_t GetByte(uint32_t rambase);
float Byte4ToFloat(uint8_t byte[], uint8_t len);

void Input_init(void)
{
//F6     F12   F15   E15   F8       F14      E7       E10
//I_ONA I_ONB I_ONC I_OND  LEGDETA  LEGDETB  LEGDETC  LEGDETD
	
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);			//使能GPIOE时钟
  
	//I_ONA I_ONB I_ONC I_OND
	//初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;							//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;							//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);										//初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6  | GPIO_Pin_12  | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;							//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;							//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);										//初始化
	
	GPIO_ResetBits(GPIOE,GPIO_Pin_15);  //拉低保护
	GPIO_ResetBits(GPIOF,GPIO_Pin_6 | GPIO_Pin_12 | GPIO_Pin_15);  //拉低保护
	
//LEGDETA		LEGDETB		LEGDETC		LEGDETD

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;							//
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;							//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);										//初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;							//
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;							//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);										//初始化

}

void Color(uint8_t port,uint8_t mode, uint32_t bufferrambase)//接受3个Byte数据
{
	{
	if( 1 <= port && port <= 4 )
		switch(mode)
		{
			case 1://Color  1Byte
					ChangeMode(port,2);
					switch(Channel_State[port-1][3])
					{
						case 1:SetByte(bufferrambase, 0x02);
							break;
						case 2:SetByte(bufferrambase, 0x04);
							break;
						case 3:SetByte(bufferrambase, 0x08);
							break;
						case 4:SetByte(bufferrambase, 0x10);
							break;
						case 5:SetByte(bufferrambase, 0x20);
							break;
						case 6:SetByte(bufferrambase, 0x40);
							break;
						case 7:SetByte(bufferrambase, 0x80);
							break;
						default:SetByte(bufferrambase, 0x01);
						break;
					}
					SetByte(bufferrambase+1, 0x00);
			
				break;
			
			case 2://Reflected Light Intensity  4Byte
					ChangeMode(port,0);
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 3://Ambient Light Intensity  4Byte
					ChangeMode(port,1);
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 4://reset
				ChangeMode(port,0x11);
//				UartPortSend(port,0x44);
//				UartPortSend(port,0x11);
//				UartPortSend(port,0xFF ^ 44 ^ 11);
				break;
				default:break;
		}
	}
}

void Gyro(uint8_t port,uint8_t mode, uint32_t bufferrambase)//接受4个Byte数据，中间两个左低右高
{
	if( 1 <= port && port <= 4 )
	{
		switch(mode)
		{
			case 1://Angle  4Byte
					ChangeMode(port,0);
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 2://Rate  4Byte
					ChangeMode(port,1);
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 3://Angle and Rate  8Byte
					ChangeMode(port,3);
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
					SetFloat(bufferrambase+4, Byte4ToFloat(Channel_State[port-1],4));
				break;
					
			case 4://重置
				ChangeMode(port,0x11);
//				UartPortSend(port,0x44);
//				UartPortSend(port,0x11);
//				UartPortSend(port,0xFF ^ 44 ^ 11);
				break;	
				default:break;
		}
	}
}

void Infrared(uint8_t port,uint8_t mode, uint32_t bufferrambase)
{
	float temp;
	if( 1 <= port && port <= 4 )
	{
		switch(mode)
		{
			case 1://Proximity  4Byte
					ChangeMode(port,0);
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 2://Beacon  10Byte
					UartPort[port-1].channel = GetByte(bufferrambase);
					ChangeMode(port,1);
			
					temp = Byte4ToFloat(Channel_State[port-1],1);
					if(temp > 180)
					{
						temp = -(255-temp);
					}
					
					SetFloat(bufferrambase+1, temp);
					
					SetFloat(bufferrambase+5, Byte4ToFloat(Channel_State[port-1],5));
//					if(Byte4ToFloat(Channel_State[port-1],4) != 0)
//					{
//						SetByte(bufferrambase+9, 0x01);
//					}
				break;
			
			case 3://Remote  2Byte
					UartPort[port-1].channel = GetByte(bufferrambase);
					ChangeMode(port,2);
					
					switch(Channel_State[port-1][4])
					{
						case 1:SetByte(bufferrambase+1, 0x02);SetByte(bufferrambase+2, 0x00);
							break;
						case 2:SetByte(bufferrambase+1, 0x04);SetByte(bufferrambase+2, 0x00);
							break;
						case 3:SetByte(bufferrambase+1, 0x08);SetByte(bufferrambase+2, 0x00);
							break;
						case 4:SetByte(bufferrambase+1, 0x10);SetByte(bufferrambase+2, 0x00);
							break;
						case 5:SetByte(bufferrambase+1, 0x20);SetByte(bufferrambase+2, 0x00);
							break;
						case 6:SetByte(bufferrambase+1, 0x40);SetByte(bufferrambase+2, 0x00);
							break;
						case 7:SetByte(bufferrambase+1, 0x80);SetByte(bufferrambase+2, 0x00);
							break;
						case 8:SetByte(bufferrambase+1, 0x00);SetByte(bufferrambase+2, 0x01);
							break;
						case 9:SetByte(bufferrambase+1, 0x00);SetByte(bufferrambase+2, 0x02);
							break;
						case 10:SetByte(bufferrambase+1, 0x00);SetByte(bufferrambase+2, 0x04);
							break;
						case 11:SetByte(bufferrambase+1, 0x00);SetByte(bufferrambase+2, 0x08);
							break;
						default:SetByte(bufferrambase+1, 0x01);SetByte(bufferrambase+2, 0x00);
							break;
					}
				break;
			
				default:break;
		}
	}
}

void Temperature(uint8_t port,uint8_t mode, uint32_t bufferrambase)
	
{
	if( 1 <= port && port<= 4 )
	{
		switch(mode)
		{
			case 1://Celsius  4Byte
					IicPort[port-1].Mode = 0;
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 2://Fahrenheit  4Byte
					IicPort[port-1].Mode = 1;
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0)*9/5+32);
				break;
			
				default:break;
		}
	}
}

void Touch(uint8_t port,uint8_t mode, uint32_t bufferrambase)
{
	if( 1 <= port && port<= 4 && mode == 1)
	{
		//state 1Byte
		SetByte(bufferrambase, Channel_State[port-1][3]);
	}
}

void Ultrasonic(uint8_t port,uint8_t mode, uint32_t bufferrambase)
{
	if( 1 <= port && port <= 4)
	{
		switch(mode)
		{
			case 1://ADV Distance Centimeter 5Byte  			  
					if(GetByte(bufferrambase) == 0)
					{
						ChangeMode(port,0);
						ChangeMode(port,3);
					}else if(GetByte(bufferrambase) == 1)
					{
						ChangeMode(port,0);
					}
					SetFloat(bufferrambase+1, Byte4ToFloat(Channel_State[port-1],1));
				break;
			
			case 2://ADV Distance Inches 5Byte 
					if(GetByte(bufferrambase) == 0)
					{
						ChangeMode(port,1);
						ChangeMode(port,4);
					}else if(GetByte(bufferrambase) == 1)
					{
						ChangeMode(port,1);
					}
					SetFloat(bufferrambase+1, Byte4ToFloat(Channel_State[port-1],1));
				break;
			
			case 3://Distance Centimeter 4Byte
					ChangeMode(port,0);
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 4://Distance Inches 4Byte
					ChangeMode(port,1);
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 5://Presence  1Byte
					ChangeMode(port,2);
					SetByte(bufferrambase, Channel_State[port-1][3]);
				break;
			
				default:break;
		}
	}
}

void Recharge(uint8_t port,uint8_t mode, uint32_t bufferrambase)
{
	if( 1 <= port && port <= 4 )
	{
		switch(mode)
		{
			case 1://In Voltage 4Byte
					IicPort[port].Mode =0;
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 2://In Current 4Byte
					IicPort[port].Mode =1;
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 3://In Wattage 4Byte
					IicPort[port].Mode =5;
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 4://Out Voltage 4Byte
					IicPort[port].Mode =2;
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 5://Out Current 4Byte
					IicPort[port].Mode =3;
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 6://Out Wattage 4Byte
					IicPort[port].Mode =6;
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 7://Joule 4Byte
					IicPort[port].Mode =4;
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
				default:break;
		}
	}
}

void Sound(uint8_t port,uint8_t mode, uint32_t bufferrambase)
{
	if( 1 <= port && port<= 4 )
	{
		switch(mode)
		{
			case 1://Measure-dB 4Byte
					Channel_State[port-1][2]=0x01;
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 2://Measure-dBa 4Byte
					Channel_State[port-1][2]=0x02;
					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 3://calibrate 5Byte
//					SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			default:break;
		}
	}
}

void Motor(uint8_t port,uint8_t mode, uint32_t bufferrambase)
{
	if( 5 <= port && port <= 8 )
	{
		switch(mode)
		{
			case 1://degrees
			Channel_State[port-1][2] = 0x01;
			SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 2://rotation
			Channel_State[port-1][2] = 0x02;
			SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 3://power
			Channel_State[port-1][2] = 0x03;
			SetFloat(bufferrambase, Byte4ToFloat(Channel_State[port-1],0));
				break;
			
			case 4:
				Motor_Reset(port);
				break;
			
				default:break;
		}
	}
}

uint8_t LegoIdtoEstId(uint8_t ID)
{
	uint8_t est_id = 0x00;
	
	switch(ID)
	{
		case 0x1D:est_id = 0x01;//颜色
			break;
		case 0x20:est_id = 0x02;//陀螺仪
			break;
		case 0x21:est_id = 0x03;//红外
			break;
		case 0x07:est_id = 0x04;//大型电机
			break;
		case 0x08:est_id = 0x05;//中型电机
			break;
		case 0x06:est_id = 0x06;//温度
			break;
		case 0x10:est_id = 0x07;//触碰
			break;
		case 0x1E:est_id = 0x08;//超声
			break;
		case 0x63:est_id = 0x09;//能量表
			break;
		case 0x03:est_id = 0x0A;//NXT声音
			break;
		default:break;
	}
	return est_id;
}

uint8_t LegoModetoEstMode(uint8_t ID, uint8_t Mode)
{
	uint8_t est_mode = 0x00;
	
	switch(ID)
	{
		case 0x1D://颜色
			if(Mode == 0)
			{
				est_mode = 2;
			}else if(Mode == 1)
			{
				est_mode = 3;
			}else if(Mode == 2)
			{
				est_mode = 1;
			}
//			else if(Mode == 0x11)
//			{
//				est_mode = 2;
//			}
			break;
			
		case 0x20://陀螺仪
			if(Mode == 0)
			{
				est_mode = 1;
			}else if(Mode == 1)
			{
				est_mode = 2;
			}else if(Mode == 3)
			{
				est_mode = 3;
			}
//			else if(Mode == 0x11)
//			{
//				est_mode = 1;
//			}
			break;
		
		case 0x1E://超声
			if(Mode == 0)
			{
				est_mode = 3;
			}else if(Mode == 1)
			{
				est_mode = 4;
			}else if(Mode == 2)
			{
				est_mode = 5;
			}else if(Mode == 3)
			{
				est_mode = 1;
			}else if(Mode == 4)
			{
				est_mode = 2;
			}
			break;
			
		case 0x63://能量表
			if(Mode == 0)
			{
				est_mode = 1;
			}else if(Mode == 1)
			{
				est_mode = 2;
			}else if(Mode == 2)
			{
				est_mode = 4;
			}else if(Mode == 3)
			{
				est_mode = 5;
			}else if(Mode == 4)
			{
				est_mode = 7;
			}else if(Mode == 5)
			{
				est_mode = 3;
			}else if(Mode == 6)
			{
				est_mode = 6;
			}
			break;
				
			//红外、电机、温度、触碰、声音 模式不变
			
		case 0x21://红外
			if(Mode == 0)
			{
				est_mode = 1;
			}else if(Mode == 1)
			{
				est_mode = 2;
			}else if(Mode == 2)
			{
				est_mode = 3;
			}
			break;
			
		case 0x08://大型电机
			if(Mode == 0)
			{
				est_mode = 1;
			}else if(Mode == 1)
			{
				est_mode = 2;
			}else if(Mode == 2)
			{
				est_mode = 3;
			}
			break;
		case 0x07://中型电机
			if(Mode == 0)
			{
				est_mode = 1;
			}else if(Mode == 1)
			{
				est_mode = 2;
			}else if(Mode == 2)
			{
				est_mode = 3;
			}
			break;
		case 0x06://温度
			if(Mode == 0)
			{
				est_mode = 1;
			}else if(Mode == 1)
			{
				est_mode = 2;
			}
			break;
		case 0x10://触碰
			if(Mode == 0)
			{
				est_mode = 1;
			}
			break;
		case 0x03://NXT声音
			if(Mode == 0)
			{
				est_mode = 1;
			}else if(Mode == 1)
			{
				est_mode = 2;
			}
			break;
		default:break;
	}
	return est_mode;
}


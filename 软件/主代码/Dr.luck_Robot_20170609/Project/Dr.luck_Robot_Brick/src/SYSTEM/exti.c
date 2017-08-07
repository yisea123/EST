/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/exti.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief  
  ******************************************************************************
**/
#include "exti.h"

int Pluse[4]={0};

//外部中断5服务程序
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
	{
//		if(TACHOA0==1)	  
//		{				 
			if(TACHOA1 == TACHOA0) --Pluse[0];
			else ++Pluse[0];
//		}
		
		EXTI_ClearITPendingBit(EXTI_Line5);//清除LINE5上的中断标志位 
	}
	if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
	{
//		if(TACHOC0==1)	  
//		{				 
			if(TACHOC1 == TACHOC0) --Pluse[2];
			else ++Pluse[2];
//		}
		
		EXTI_ClearITPendingBit(EXTI_Line7);//清除LINE5上的中断标志位 
	}
	if(EXTI_GetITStatus(EXTI_Line9)!=RESET)
	{
//		if(TACHOD0==1)	  
//		{				 
			if(TACHOD1 == TACHOD0) --Pluse[3];
			else ++Pluse[3];
//		}
		
		EXTI_ClearITPendingBit(EXTI_Line9);//清除LINE5上的中断标志位 
	} 	
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
	{
//		if(TACHOB0 == 1)	  
//		{				 
			if(TACHOB1 == TACHOB0) --Pluse[1];
			else ++Pluse[1];
//		}
		
		EXTI_ClearITPendingBit(EXTI_Line13);//清除LINE5上的中断标志位 
	} 
}

void TACHO_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOC,GPIOE时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_13|GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}
	   
//外部中断初始化程序
//初始化PE2~4,PA0为中断输入.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	TACHO_Init(); //按键对应的IO口初始化
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource5);//PE5 连接到中断线5
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource13);//PE13 连接到中断线13
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource7);//PC7 连接到中断线7
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource9);//PC9 连接到中断线9
	
	/* 配置EXTI_Line5,13,7,9 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line5 | EXTI_Line13 | EXTI_Line7 | EXTI_Line9;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
	
 	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	   
}













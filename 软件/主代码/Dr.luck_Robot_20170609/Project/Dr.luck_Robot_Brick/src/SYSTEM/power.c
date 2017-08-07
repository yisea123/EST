/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/power.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    06-Nov-2016
  * @brief   
  ******************************************************************************
**/
#include "power.h"

void POWER_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOC,GPIOE时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //K对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
	
//	GPIO_SetBits(GPIOE, GPIO_Pin_2);
} 


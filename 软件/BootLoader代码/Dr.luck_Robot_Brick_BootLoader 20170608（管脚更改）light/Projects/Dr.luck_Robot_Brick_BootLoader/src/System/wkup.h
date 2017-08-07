#ifndef __WKUP_H
#define __WKUP_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//待机唤醒 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
					    
//引脚定义
/*******************************************************/
#define WKUP_INT_GPIO_PORT                GPIOE
#define WKUP_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOE
#define WKUP_INT_GPIO_PIN                 GPIO_Pin_4
#define WKUP_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOE
#define WKUP_INT_EXTI_PINSOURCE           EXTI_PinSource4
#define WKUP_INT_EXTI_LINE                EXTI_Line4
#define WKUP_INT_EXTI_IRQ                 EXTI4_IRQn

#define WKUP_IRQHandler                   EXTI4_IRQHandler

void WKUP_Init(void); 			//PE4 WKUP唤醒初始化
void Sys_Enter_Stopby(void);	//系统进入待机模式
#endif



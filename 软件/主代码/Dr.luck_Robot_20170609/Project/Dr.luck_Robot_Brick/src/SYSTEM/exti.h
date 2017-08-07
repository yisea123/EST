/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/exti.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief  
  ******************************************************************************
**/
#ifndef __EXTI_H
#define __EXIT_H	 

#include "sys.h"  	

#define TACHOA0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5) 
#define TACHOB0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)
#define TACHOC0 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)
#define TACHOD0 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)

#define TACHOA1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) 
#define TACHOB1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)
#define TACHOC1 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)
#define TACHOD1 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)

void EXTIX_Init(void);	//外部中断初始化		 			

extern int Pluse[];

#endif




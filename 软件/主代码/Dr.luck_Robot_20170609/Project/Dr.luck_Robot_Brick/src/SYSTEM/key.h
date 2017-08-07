/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/key,h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __KEY_H
#define __KEY_H	 

#include "sys.h" 

/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)	//PC14
#define KEY1 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)	//PC15
#define KEY2 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0) 	//PF0
#define KEY3 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) 	//PE3
#define KEY4 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_1)		//PF1
#define KEY5 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) 	//PE4

void KEY_Init(void);	//IO初始化
u8 KEY_Scan(u8);  		//按键扫描函数	

#endif

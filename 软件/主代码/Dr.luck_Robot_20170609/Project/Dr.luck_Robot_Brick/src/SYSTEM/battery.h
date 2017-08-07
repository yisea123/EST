/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/battery.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __BATTERY_H
#define __BATTERY_H	

#include "sys.h" 

#define	BATTERY()			(GPIOG->IDR&GPIO_Pin_2)

void Battery_Init(void);//≥ı ºªØ		 

#endif


















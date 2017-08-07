/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/delay.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __DELAY_H
#define __DELAY_H 			   

#include <sys.h>	  
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif






























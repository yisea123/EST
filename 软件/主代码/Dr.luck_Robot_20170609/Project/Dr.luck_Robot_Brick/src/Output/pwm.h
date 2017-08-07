/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/pwm.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   Main program body according to ST source
  ******************************************************************************
**/

#ifndef _PWM_H
#define _PWM_H

#include "sys.h"
#include "OptimizePid.h"

#define PORT_A0    PGout(10) 
#define PORT_A1    PGout(11) 

#define PORT_B0    PDout(4) 
#define PORT_B1    PDout(5)

#define PORT_C0    PGout(7) 
#define PORT_C1    PGout(8) 

#define PORT_D0    PGout(6) 
#define PORT_D1    PGout(5) 


/*
	使用方法：
	初始化：
			TIM3_PWM_Init(100-1,84-1);	//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.  
	修改占空比：
			TIM_SetCompare1(TIM3,100);	//修改比较值，修改占空比,0-100,
	设定TIM3的通道1，对应管脚为PA6
*/

void PWM_Init(void);
void TIM4_PWM_Init(u32 arr,u32 psc);
void PWM_IO_Init(void);

//功能函数
void Motor_DirectionSet(uint8_t OChannel, int MotorDirection);
void Motor_PowerSet(uint8_t OChannel, int MotorSpeed);

#endif

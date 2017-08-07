#ifndef __MAIN_H
#define __MAIN_H

/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/test.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   Header for test.c module
  ******************************************************************************
  */
#include "sys.h"

extern u8 motor1_on;
extern u8 motor2_on;
extern u8 motor3_on;
extern u8 motor4_on;
extern u8 motor1_ok;
extern u8 motor2_ok;
extern u8 motor3_ok;
extern u8 motor4_ok;
extern u8 motor_state;
extern u8 count;

extern void UI_Test(u8 button);//100ms÷¥––
extern void Motor_Movement();

#endif 


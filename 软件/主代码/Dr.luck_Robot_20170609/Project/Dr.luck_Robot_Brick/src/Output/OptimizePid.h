/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/OptimizePid.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   Main program body according to ST source
  ******************************************************************************
**/
#ifndef __OPTIMIZEPID_H
#define __OPTIMIZEPID_H

#include "sys.h"

float PID_IncrementMode(float ValueWant,float ValueNow);
float PID_AbsoluteMode(u8 Ochannel,float ValueWant,float ValueNow);
float PID_AbsoluteMode2(u8 DeviceID, u8 Ochannel, float ValueWant,float ValueNow);

extern float in_kp;
extern float in_ki;
extern float in_kd;

extern float ab_kp;
extern float ab_ki;
extern float ab_kd;

#endif

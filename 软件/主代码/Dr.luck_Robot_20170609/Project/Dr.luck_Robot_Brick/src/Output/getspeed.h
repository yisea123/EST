/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/getspeed.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   Main program body according to ST source
  ******************************************************************************
**/

#ifndef _GETSPEED_H_
#define _GETSPEED_H_

#include "sys.h"

/*
    光电编码器测速、方向
		功能：测速、测转向
		输出信号接口：PA0，PA1
*/

//初始化函数
void ENC_Init(void);		//光电编码初始化

//功能函数
void ENC_Reset(uint8_t Ochannel);
extern int ENC_GetCurrentPluse(uint8_t Ochannel);

//角度检测进程
//1ms一次
void ENC_SumAngleProcess(int SumPluse[4], int SumPluseForDisplay[4]);
//测速进程
//每100ms执行一次
void ENC_CurrentPowerProcess(int CurrentPower[4]);

//全局变量
extern uint8_t PowerProcessKey[4];
extern int CurrentPower[4];
extern int SumPluse[4];
extern int SumPluseForDisplay[4];

extern unsigned int Runtime_PID[4];

#endif

/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/outputworkmode.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   Main program body according to ST source
  ******************************************************************************
**/

#ifndef _OUTPUTWORKMODE_H
#define _OUTPUTWORKMODE_H

#include "sys.h"
#include "pwm.h"
#include "getspeed.h"
#include "OptimizePid.h"

extern uint8_t Motor_Invert[4];
extern uint8_t motor_state[4];
extern uint8_t motor_on_state[4];

//初始化函数
void Output_Init(void);

//计时函数
//每10ms执行一次（必须）
void MotorRunTime_Check(void);

//端口电机检测函数，每1s执行一次
void ChannelMotorCheck(u8 Channel_State[32][13]);

//模式函数
uint8_t Meduim_Motor_Off(uint8_t Ochannel, uint8_t BreakAtEnd);
uint8_t Meduim_Motor_On(uint8_t Ochannel, int Power, uint8_t Invert);
uint8_t Meduim_Motor_OnForSecond(uint8_t Ochannel, int Power, float Seconds, uint8_t BreakAtEnd, uint8_t Invert);
uint8_t Meduim_Motor_OnForDegrees(uint8_t Ochannel, int Power, float Degrees, uint8_t BreakAtEnd, uint8_t Invert);
uint8_t Meduim_Motor_OnForRotations(uint8_t Ochannel, int Power, float Rotations, uint8_t BreakAtEnd, uint8_t Invert);

uint8_t Large_Motor_Off(uint8_t Ochannel, uint8_t BreakAtEnd);
uint8_t Large_Motor_On(uint8_t Ochannel, int Power, uint8_t Invert);
uint8_t Large_Motor_OnForSecond(uint8_t Ochannel, int Power, float Seconds, uint8_t BreakAtEnd, uint8_t Invert);
uint8_t Large_Motor_OnForDegrees(uint8_t Ochannel, int Power, float Degrees, uint8_t BreakAtEnd, uint8_t Invert);
uint8_t Large_Motor_OnForRotations(uint8_t Ochannel, int Power, int Rotations, uint8_t BreakAtEnd, uint8_t Invert);

uint8_t Move_Steering_Off(uint8_t Ochannel1, uint8_t Ochannel2, uint8_t BreakAtEnd);
uint8_t Move_Steering_On(uint8_t Ochannel1, uint8_t Ochannel2,float Steering, float Power, uint8_t Invert);
uint8_t Move_Steering_OnForSecond(uint8_t Ochannel1, uint8_t Ochannel2, float Steering, float Power, float Seconds, uint8_t BreakAtEnd, uint8_t Invert);
uint8_t Move_Steering_OnForDegrees(uint8_t Ochannel1, uint8_t Ochannel2, float Steering, float Power, float Degrees, uint8_t BreakAtEnd, uint8_t Invert);
uint8_t Move_Steering_OnForRotations(uint8_t Ochannel1, uint8_t Ochannel2, float Steering, float Power, float Rotations, uint8_t BreakAtEnd, uint8_t Invert);

uint8_t Move_Tank_Off(uint8_t Ochannel1, uint8_t Ochannel2, uint8_t BreakAtEnd);
uint8_t Move_Tank_On(uint8_t Ochannel1, uint8_t Ochannel2, float Power1, float Power2, uint8_t Invert);
uint8_t Move_Tank_OnForSecond(uint8_t Ochannel1, uint8_t Ochannel2, float Power1, float Power2, float Seconds, uint8_t BreakAtEnd, uint8_t Invert);
uint8_t Move_Tank_OnForDegrees(uint8_t Ochannel1, uint8_t Ochannel2, float Power1, float Power2, float Degrees, uint8_t BreakAtEnd, uint8_t Invert);
uint8_t Move_Tank_OnForRotations(uint8_t Ochannel1, uint8_t Ochannel2, float Power1, float Power2, float Rotations, uint8_t BreakAtEnd, uint8_t Invert);

uint8_t Unregulated_Motor_On(uint8_t Ochannel, int Power, uint8_t Invert);

extern float Motor_Degrees(uint8_t Ochannel);
float Motor_Rotations(uint8_t Ochannel);
float Motor_CurrentPower(uint8_t Ochannel);
void Motor_Reset(uint8_t Ochannel);

#endif

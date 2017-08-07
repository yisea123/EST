/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/input.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __INPUT_H
#define __INPUT_H 	

#include "sys.h" 

void Input_init(void);
	
extern void Color(uint8_t port,uint8_t mode, uint32_t bufferrambase);							//颜色
extern void Gyro(uint8_t port,uint8_t mode, uint32_t bufferrambase);							//陀螺仪
extern void Infrared(uint8_t port,uint8_t mode, uint32_t bufferrambase);					//红外传感
extern void Temperature(uint8_t port,uint8_t mode, uint32_t bufferrambase);				//温度
extern void Touch(uint8_t port,uint8_t mode, uint32_t bufferrambase);							//触碰
extern void Ultrasonic(uint8_t port,uint8_t mode, uint32_t bufferrambase);				//超声波
extern void Recharge(uint8_t port,uint8_t mode, uint32_t bufferrambase);					//能量
extern void Sound(uint8_t port,uint8_t mode, uint32_t bufferrambase);							//NXT声音
extern void Motor(uint8_t port,uint8_t mode, uint32_t bufferrambase);                    //电机
extern uint8_t LegoIdtoEstId(uint8_t ID);
extern uint8_t LegoModetoEstMode(uint8_t ID, uint8_t Mode);

#endif

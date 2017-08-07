/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/brickperipheral.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __BRICKPERIPHERAL_H
#define __BRICKPERIPHERAL_H 	

#include "sys.h" 

extern void BriDisplay(uint8_t mode,uint8_t BufferSize, uint32_t bufferrambase);
extern u8 BriSound(uint8_t mode, uint32_t bufferrambase);
extern void BriButton(uint8_t mode, uint32_t bufferrambase);
extern void BriLight(uint8_t mode, uint32_t bufferrambase);
extern void BriBt(uint8_t mode, uint32_t bufferrambase);
extern void BriMessage(uint8_t mode, uint32_t bufferrambase);
extern void BriFile(uint8_t mode, uint32_t bufferrambase);
extern void BriWaveform(uint8_t mode, uint32_t bufferrambase);
extern void BriTimerSensor(uint8_t mode, uint32_t bufferrambase);				//计时器
extern void BriRawSensor(uint8_t mode, uint32_t bufferrambase);					//原始值
extern u8 BriLight_flag;
extern u8 Bributton_state;
extern u8 Bributton_value;
extern u8 Bributton_time;

#endif


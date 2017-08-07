/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/adc.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __ADC_H
#define __ADC_H	

#include "sys.h" 

extern u16 ADC1_Channel_Vol[10][10];//ADCB0、ADCB1、ADCC0、ADCC1、ADCD0、ADCD1、ADCMA、ADCMB、ADCMC、ADCMD、ADCA0、ADCA1、PB-VOL
												//C1     C0     A5     C3     A7     A6    C4     C5     B0     B1     F4     F5     F3
												
extern u16 ADC3_Channel_Vol[10][3];//ADCA0、ADCA1、PB-VOL
												//F4     F5     F3
												
void Adc1_Init(void); 				//ADC1通道初始化
void Adc3_Init(void); 				//ADC3通道初始化

u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  
u16 ReadADC1AverageValue(u16 Channel);
u16 ReadADC3AverageValue(u16 Channel);

#endif 

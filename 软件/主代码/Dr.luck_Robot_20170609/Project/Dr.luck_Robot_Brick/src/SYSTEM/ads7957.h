#ifndef __ADS7957_H
#define __ADS7957_H

/*
	使用方法：
		初始化：SPI_Init1();
		调用:		100ms执行一次 ADCChannel_To_Vol(ADC_Channel_Vol,Simulate_SPI_SendByte(0x2840));
						ADC_Channel_Vol[]数组中的元素即是对应通道的当前电压值[0,5.0]
*/

#include "stm32f4xx.h"

#define CS_H GPIO_SetBits(GPIOC, GPIO_Pin_1)
#define CS_L GPIO_ResetBits(GPIOC, GPIO_Pin_1) 
#define SCLK_H GPIO_SetBits(GPIOC, GPIO_Pin_2)  
#define SCLK_L GPIO_ResetBits(GPIOC, GPIO_Pin_2) 
#define MOSI_H GPIO_SetBits(GPIOC, GPIO_Pin_3)  
#define MOSI_L GPIO_ResetBits(GPIOC, GPIO_Pin_3)  
#define MISO GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) 

void SPI_Init1(void);
unsigned int Simulate_SPI_SendByte(unsigned int dt) ;
void ADCChannel_To_Vol(float ADC_Channel_Vol[],unsigned int SPIReturnData);

extern float ADC_Channel_Vol[16];

#endif

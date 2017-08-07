#ifndef _74HC165__H_
#define _74HC165__H_

#include "stdio.h"
#include "stm32fxxx_it.h" 

#define HC165_CK_1 GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define HC165_CK_0 GPIO_ResetBits(GPIOB, GPIO_Pin_9)

#define HC165_nPL_1 GPIO_SetBits(GPIOE, GPIO_Pin_10)
#define HC165_nPL_0 GPIO_ResetBits(GPIOE, GPIO_Pin_10)

#define HC165_OUT GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)

void Touch_74HC165_Init(void);

extern unsigned char readByteHC165(void);

#endif 


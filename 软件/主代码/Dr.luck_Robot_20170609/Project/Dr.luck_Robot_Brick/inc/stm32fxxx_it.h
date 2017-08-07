/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/stm32fxxx_it.H
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32Fxxx_IT_H
#define __STM32Fxxx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "disassembly.h"
	 
/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"
#include "lego_include.h"

/* Exported types ------------------------------------------------------------*/
	 
/*****系统时间*****/
extern uint32_t SystemTime,ProgramTime,SystemCountTime,SoundTime;
extern uint32_t GetSystemTimeMs(void);
extern uint32_t GetProgramTimeMs(void);

/*****系统滴答时钟中断*****/
extern UBYTE SysTick_1ms;		//时间片时基1ms
extern unsigned int SysTick_50us_my;
	 
///*****系统时间*****/
//extern uint32_t SystemTime,ProgramTime;
//extern uint32_t GetSystemTimeMs(void);
//extern uint32_t GetProgramTimeMs(void);

///*****系统滴答时钟中断*****/
//extern UBYTE SysTick_1ms;		//时间片时基1ms
//extern UBYTE SysTick_2ms;		//时间片时基2ms
//extern UBYTE SysTick_10ms;	//时间片时基10ms
//extern UBYTE SysTick_50ms;	//时间片时基50ms
//extern UBYTE SysTick_100ms;	//时间片时基100ms
//extern UBYTE SysTick_1s;	//时间片时基1s

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32Fxxx_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

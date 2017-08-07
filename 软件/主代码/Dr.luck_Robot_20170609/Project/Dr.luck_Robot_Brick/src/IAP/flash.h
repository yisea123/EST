/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/flash.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief  
  ******************************************************************************
**/
#ifndef __FLASH_H__
#define __FLASH_H__
#include "stm32f4xx.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 2048 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1                      //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////


//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解锁键值
//#define RDP_Key                  ((uint16_t)0x00A5)
//#define FLASH_KEY1               ((uint32_t)0x45670123)
//#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)
 
 
#define APP_CONFIG_ADDR 		0X08008010	//配置地址
#define APP_CONFIG_SET_VALUE	0X5555	//设置值
#define APP_CONFIG_CLEAR_VALUE	0x0000	//清零值

#define Update_Data_Length_L16 		0x08008000	//配置地址
#define Update_Data_Length_H16 		0x08008008	//配置地址

#define UPDATE_ADDRESS        (uint32_t)0x08080000 

u16 STMFLASH_ReadHalfWord(u32 faddr);		  //读出半字  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);				//指定地址开始读取指定长度数据
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);
//测试写入
void Test_Write(u32 WriteAddr,u16 WriteData);								   
#endif


















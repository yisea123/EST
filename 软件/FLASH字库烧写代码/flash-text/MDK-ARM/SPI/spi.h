/**
  ******************************************************************************
  * @file    EST Brick of Dr.luck/spi.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

//#define SPI_CLK 			PBout(3)
//#define SPI_MISO 			PBout(5)

//#define SPI_CLK 			PBout(3)
//#define SPI_MISO 			PBout(5)

//#define SPI_MOSI_READ PBin(4)

void SPI1_Init(void);			 					//初始化SPI1口
void SPI1_SetSpeed(u8 SpeedSet); 		//设置SPI1速度   
u8 SPI1_ReadWriteByte(u8 TxData);		//SPI1总线读写一个字节

#endif


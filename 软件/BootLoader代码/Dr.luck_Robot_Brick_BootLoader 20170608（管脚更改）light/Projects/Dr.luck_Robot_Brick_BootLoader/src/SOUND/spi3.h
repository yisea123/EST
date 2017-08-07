/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/spi3.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief  
  ******************************************************************************
**/
#ifndef __SPI3_H
#define __SPI3_H

#ifdef  SPI_GLOBALS
#define SPI_EXT 
#else   
#define SPI_EXT    extern
#endif
 
#include "sys.h"

SPI_EXT void SPI3_Init(void);			 //初始化SPI口
SPI_EXT void SPI3_SetSpeed(u8 SpeedSet); //设置SPI速度   
SPI_EXT u8 SPI3_ReadWriteByte(u8 TxData);//SPI总线读写一个字节

//SPI_EXT void SPI1_Init(void);			 //初始化SPI1口
//SPI_EXT void SPI1_SetSpeed(u8 SpeedSet); //设置SPI1速度  		 
//SPI_EXT u8 SPI1_ReadWriteByte(u8 TxData);//SPI1总线读写一个字节

#define	SPI_MP3_BUFNUM		32
SPI_EXT u8 SPI_Mp3_TxBuf[SPI_MP3_BUFNUM];

void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr);//配置DMAx_CHx
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);	//使能一次DMA传输		   

void MYDMA_Disable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);
	
#endif

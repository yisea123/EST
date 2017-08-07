/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/uart5.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __USART6_H
#define __USART6_H

#include "sys.h" 

/* debug define --------------------------------------------------------------*/
#define  SYSTEM_DEBUG			1
//#define  INPUT_DEBUG			1
//#define  INSTRUCT_DEBUG		1
#define  RAM_DEBUG				1
/* ---------------------------------------------------------------------------*/

//#define USART6_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART6_RX 			1		//使能（1）/禁止（0）串口1接收
#define Usart6_DMA_Len 4096   //定义DMA缓冲区长度/
#define BAUD_RATE    115200
#define WATE_TIME    Usart6_DMA_Len*20000/BAUD_RATE    //串口发送Usart6_DMA_Len长度字节所需要的时间，单位：ms
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);
//如果想串口中断接收，请不要注释以下宏定义
void usart6_init(u32 bound);
void USART6_Send_Byte(u16 Data);
	
#endif



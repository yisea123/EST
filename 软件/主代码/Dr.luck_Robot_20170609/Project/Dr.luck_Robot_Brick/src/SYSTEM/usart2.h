/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/usart2.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __USART2_H
#define __USART2_H

#include "sys.h"
#include "lego_include.h"

#define EN_USART2_RX 					1						//使能（1）/禁止（0）串口1接收

extern UBYTE Uart2RecBuf[];     //接收缓冲,最大USART_REC_LEN个字节.
extern u16 Uart2RecBufIn;

//extern UBYTE USART2_TX_BUF[];     //发送缓冲,最大USART2_SEN_LEN个字节
//extern u16 USART2_TX_POINTER;
//extern u16 USART2_TX_LEN;

extern u16 USART2_RX_STA;         				//接收状态标记	

//如果想串口中断接收，请不要注释以下宏定义
void usart2_init(u32 bound);
//UBYTE Uart2Write(u16 Data);

UBYTE USART2_Send_Byte(UBYTE Data);
void usart2_change(u32 bound);
	
#endif



/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/usart3.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __USART3_H
#define __USART3_H

#include "sys.h"
#include "lego_include.h"

#define EN_USART3_RX 					1						//使能（1）/禁止（0）串口1接收

extern UBYTE Uart3RecBuf[];     //接收缓冲,最大USART_REC_LEN个字节.
extern u16 Uart3RecBufIn;
extern UBYTE Usart3BreakTimer;

//extern UBYTE USART3_TX_BUF[];     //发送缓冲,最大USART3_SEN_LEN个字节
//extern u16 USART3_TX_POINTER;
//extern u16 USART3_TX_LEN;

extern u16 USART3_RX_STA;         				//接收状态标记	

//如果想串口中断接收，请不要注释以下宏定义
void usart3_init(u32 bound);

//UBYTE Uart3Write(u16 Data);
UBYTE USART3_Send_Byte(UBYTE Data);
void usart3_change(u32 bound);
	
#endif



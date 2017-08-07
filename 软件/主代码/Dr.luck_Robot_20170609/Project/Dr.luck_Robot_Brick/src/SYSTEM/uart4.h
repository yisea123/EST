/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/uart4.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __UART4_H
#define __UART4_H

#include "sys.h"
#include "lego_include.h"

#define EN_UART4_RX 					1						//使能（1）/禁止（0）串口1接收

extern UBYTE Uart1RecBuf[];     //接收缓冲,最大USART_REC_LEN个字节.
extern u16 Uart1RecBufIn;

//extern UBYTE UART4_TX_BUF[];     //发送缓冲,最大UART4_SEN_LEN个字节
//extern u16 UART4_TX_POINTER;
//extern u16 UART4_TX_LEN;

extern u16 UART4_RX_STA;         				//接收状态标记	

//如果想串口中断接收，请不要注释以下宏定义
void uart4_init(u32 bound);

//UBYTE Uart4Write(u16 Data);
UBYTE USART4_Send_Byte(UBYTE Data);
void usart4_change(u32 bound);
	
#endif



/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/uart5.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __UART5_H
#define __UART5_H
 
#include "sys.h" 

/* debug define --------------------------------------------------------------*/
#define  SYSTEM_DEBUG			1
//#define  INPUT_DEBUG			1
//#define  INSTRUCT_DEBUG		1
#define  RAM_DEBUG				1
/* ---------------------------------------------------------------------------*/

//#define UART5_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_UART5_RX 			1		//使能（1）/禁止（0）串口1接收
//	  	
//extern u8  UART5_RX_BUF[UART5_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
//extern u16 UART5_RX_STA;         		//接收状态标记	

//如果想串口中断接收，请不要注释以下宏定义
void uart5_init(u32 bound);
void UART5_Send_Byte(u16 Data);

extern u8 UART5_flag;
	
#endif


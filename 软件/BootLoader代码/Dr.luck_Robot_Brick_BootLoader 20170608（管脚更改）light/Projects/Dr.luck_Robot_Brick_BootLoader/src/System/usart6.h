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
#define  INPUT_DEBUG			1
#define  INSTRUCT_DEBUG		1
//#define  RAM_DEBUG				1
//#define  CASCADE_DEBUG				1
/* ---------------------------------------------------------------------------*/

//#define USART6_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART6_RX 			1		//使能（1）/禁止（0）串口1接收
//	  	
//extern u8  USART6_RX_BUF[USART6_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
//extern u16 USART6_RX_STA;         		//接收状态标记	

//如果想串口中断接收，请不要注释以下宏定义
extern void usart6_init(u32 bound);
void USART6_Send_Byte(u8 Data);


extern int usart6_flag;
	
#endif


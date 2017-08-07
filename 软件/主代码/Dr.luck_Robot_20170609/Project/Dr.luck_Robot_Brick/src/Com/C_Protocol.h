/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/C_Protocol.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
/*
	使用方法：
	1.在usbd_hid_core.c中的USBD_HID_DataOut内添加
		//-----------------------HID数据帧解析，每个包限定必须64个byte--------------------------------------
	if(!Receive_start&&!USB_Rx_Length)
	{
		for(i=0; i<USB_Rx_Cnt; i++)							//寻找开始符,USB_Rx_Cnt为64
		{
			if(*(USB_HID_Rx_Buffer + i) == 0x68) 	//接收到开始符
			{
				Receive_start = 1;
				USB_TimePic = 3000; 								//接收时间计时
				break;															//跳转后i不会自加1
			}
		}
	}
	if(Receive_start)
	{
		if(!USB_Rx_Length)			//第一个数据包
			for(; i<USB_Rx_Cnt; i++) 
				USB_Rx_Buff[USB_Rx_Length++] = *(USB_HID_Rx_Buffer + i); 
		else 										//紧接下来的数据包
			for(i=0; i<USB_Rx_Cnt; i++) 
				USB_Rx_Buff[USB_Rx_Length++] = *(USB_HID_Rx_Buffer + i); 
		
		if(USB_Rx_Length-1>4)		//进入数据域
		{
			if((USB_Rx_Length>5+USB_Rx_Buff[3]+USB_Rx_Buff[4]*256+1))  				 //避开数据域数据等于结束符问题
			{
				for(i=6+USB_Rx_Buff[3]+USB_Rx_Buff[4]*256; i<USB_Rx_Length; i++) //寻找结束符
				{
					if(*(USB_Rx_Buff + i) == 0x16) 		//接收到结束符
					{
						Receive_start = 0; 					 		//本帧数据接收结束，但无权接收下一帧数据
						USB_Rx_Length = i+1; 				 		//除去结束符后无关的数据
						break;
					}
				}		
			}		
		}
	}
		
	2.在SysTick_Handler(void)中添加
		//1ms执行一次
		FrameOrFlashBuffLoseCheck();
		
	3.在主函数中调用CheckOrder();

*/

#ifndef __C_PROTOCOL_H
#define __C_PROTOCOL_H

#include "sys.h"
#include "usart6.h"
#include "usbd_cdc_vcp.h"
#include "disassembly.h"
#include "usbd_hid_core.h"
#include "outputworkmode.h"
#include "flash.h"
#include "iap.h"

extern uint8_t USB_Rx_Buff[1024];
extern uint8_t Receive_start;
extern uint8_t Receive_end;

extern unsigned int USB_Rx_Length;
extern unsigned int USB_TimePic;
extern uint8_t errorflag;
extern uint8_t Channel_State[32][13];
extern uint8_t UserWareData[];	
extern u8 usbmasterflag;
extern int usbmastertimems;

extern uint8_t updateflash_start;				//包接收标志位 

extern void EnterProgram(u8 myfilename[]);
extern void EndProgram(void);

extern void CheckOrder(void);
extern void FrameOrFlashBuffLoseCheck(void);
extern void UpdateState(u8 Channel_State[32][13], u8 port, u8 id, u8 mode, float SensorData);

#endif

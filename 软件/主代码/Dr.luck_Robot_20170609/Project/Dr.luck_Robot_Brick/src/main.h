/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/main.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   Header for main.c module
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

//--------------usb include---------------------
#include <string.h>
#include <stdio.h>

//host需引用的头文件
#include "usb_hcd_int.h"
#include "usb_bsp.h"
#include "usbh_hid_core.h"
#include "usbh_core.h"
#include "usbh_usr.h"
//虚拟串口需引用的头文件
#include "usb_conf.h"
#include "usbd_desc.h"   //！！！与hid共用
#include "usbd_cdc_core.h"
#include "usbd_usr.h"		 //！！！与hid共用
#include "usbd_cdc_vcp.h"
#include "usbd_core.h"
//hid需引用
#include "usbd_hid_core.h"
#include "usb_dcd_int.h"
#include "usb_core.h"
#include "usb_conf.h"
#include "usbd_req.h"

#include "lego_include.h"
#include "battery.h"
#include "iic.h"
#include "key.h"
#include "sys.h"
#include "delay.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "uart4.h"
#include "usart6.h"
#include "rng.h"
#include "adc.h"
#include "exti.h"

#include "input.h"
#include "adc_sensor.h"
#include "usart_sensor.h"
#include "iic_sensor.h"

#include "pwm.h"
#include "outputworkmode.h"
#include "getspeed.h"

#include "disassembly.h"
#include "C_Protocol.h"
#include "excutefunction.h"
#include "brickperipheral.h"
#include "Bluetooth.h"

#include "ui_control.h"

#include "cascade.h"
#include "power.h"

#include "spi.h"
#include "w25qxx.h"
#include "malloc.h"
#include "myfatfs.h"
#include "ff.h"
#include "text.h"
#include "test.h"
#include "iwdg.h"

#include "vs10xx.h"
#include "spi3.h"
#include "music.h"
#include "piano.h"

#include "timer3.h"

#include "stdio.h"
#include "stm32fxxx_it.h" 
#include "stm324xg_eval.h"
#include "stm324xg_eval_ioe.h"

void HID_SendBuff(u8 *buff,u16 len);
void HID_SendBuffToHost(u8 *buff,u8 len);

extern void TimingDelay(__IO uint32_t nTime);

/* Private define ------------------------------------------------------------*/

extern __IO uint32_t Timingdelay;
extern u8 factorytest;
extern u8 sound_volume;
extern u8 language;
extern u8 sleep_time;
extern u8 light_volume;

extern u8 format_flag;

extern const u8 version[];
extern const u8 System_ID[];
extern const u8 System_Name[];

extern u8 lcd_value;
extern u8 light_volume;

/*****系统滴答时钟中断*****/
extern UBYTE SysTick_2ms;		//时间片时基2ms
extern UBYTE SysTick_10ms;	//时间片时基10ms
extern UBYTE SysTick_50ms;	//时间片时基50ms
extern UBYTE SysTick_100ms;	//时间片时基100ms
extern UBYTE SysTick_300ms;	//时间片时基100ms
extern UBYTE SysTick_1s;	//时间片时基1s

#endif /* __MAIN_H */

 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define blue_break PDout(11)// PD11
#define blue_break2 PDout(14)// PD14
//void bluetooth_Init(u8 at);
void blue_break_Init();
void bluetooth_respond(u8 *ButtonAction);
void bluetooch_waiting();
void bluetooth_msg();
void bluetooth_init();
extern u8 control;
extern u8 bluetooth_j;
extern u8 bluetooth_start;		 				    
extern u8 bluetooth_visual;	
extern u8 interface_state;
extern u8 bluetooth_locate;
extern u8 time_close;         
extern u8 time_open;         
extern u8 time_break;         
extern u8 time_dis;             
extern u8 time_bondlink;        
extern u8 time_link;
extern u8 time_clear;
extern u8 device_sum;        //绑定设备设备数量
extern u8 dis_sum;               //搜索设备数量
extern u8 device_bond[9][15];  //绑定设备
extern u8 device[9][15];       //搜索设备
extern u8 bluetooth_state;      //蓝牙当前状态   0：断开  1：连接     2
extern u8 judge_dis;
extern u8 bluetooth_judge;
extern u8 memory_Tab_Level;
extern u8 bluetooth_wait;
extern u8 bluetooth_window;
extern u8 position_judge;
extern u8 device_name[15];
extern u8 blue_1_state;
extern u8 blue_2_state;
extern u8 blue_3_state;
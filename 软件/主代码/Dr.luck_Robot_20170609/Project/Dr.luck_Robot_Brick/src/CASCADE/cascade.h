/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/cascade.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief  
  ******************************************************************************
**/
#ifndef __CASCADE_H
#define __CASCADE_H 	

//#include "sys.h"
//#include "usbh_hid_core.h"
#include "main.h"

#define laySize  	4
#define portSize  8

u8 HostSend_SortUpdate_Packet(u8 Current_Level);
void SlaveReply_SortUpdate_Packet(u8 Current_Level);

u8 HostSend_DControl_Packet(u8 Target_Level, u8 Port, u8 DeviceId, u8 mode , u8 Buff[], u8 BuffSize);
void SlaveReply_DControl_Packet(u8 Current_Level, u8 Port, u8 DeviceId, u8 mode);

u8 HostSend_DDataGet_Packet(u8 Target_Level, u8 Port, u8 DeviceId, u8 mode);
void SlaveReply_DDataGet_Packet(u8 Current_Level,u8 Channel_State[8][13]);
void SlaveReply_ConDevice_Packet(u8 Current_Level,u8 Channel_State[8][13]);
u8 HostSend_USBReset_Packet(void);
	

extern void SlaveCom_DataDecode(u8 RecBuff[], int RecBuffSize);
extern void HostCom_DataDecode(u8 RecBuff[], int RecBuffSize);

void UpdateCascadeNum_Process(void);
void SensorDataUpload_Process(void);

extern u8 SlaveComConnected;
extern u8 SlaveComConnected_old;
extern u8 CascadeLevel; 	//本机的级联位置
extern u8 CascadeNum; 		//级联数量
extern u8 usbrestart;

//对级联设备的操作命令缓存
typedef struct Cascade_Order
{
	u8 flag;
	u8 id;
	u8 mode;
	u8 bufflength;
	u8 buff[13];
	
}Cascade_Order_TypeDef;

////级联控制标志位
//typedef struct Cascade_flag
//{
//	u8 id;
//	u8 mode;
//	u8 flag;
//	
//}Cascade_flag_TypeDef; 

//extern Cascade_flag_TypeDef  Cascade_flag[4][8];

//级联设备信息与数据缓存
typedef struct Cascade_Device
{
	u8 id;
	u8 mode;
	u8 bufflength;
	u8 buff[9];
	
} Cascade_Device_TypeDef;

//级联功能信息
typedef struct Cascade_Function
{
	u8 allflag;
	Cascade_Order_TypeDef	Cascade_Order[portSize];  //从机级联指令缓存
	Cascade_Order_TypeDef Cascade_flag[laySize][portSize];  //主机级联指令缓存
	
//	Cascade_Device_TypeDef Cascade_Device[4][8];	//4台brick的8个外设
	
} Cascade_Function_TypeDef;

extern Cascade_Function_TypeDef	Cascade_Function;


#endif


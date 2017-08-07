/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/cascade.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief  
  ******************************************************************************
**/
#include "cascade.h"

/*
各功能的指令组说明

1.编号更新功能：
host发送：0x01+0x10(主到从)+TargetLevel
slave发送：0x01+0x11(从到主)+CurrentLevel

2.从机设备控制：
host发送：0x02+0x10(主到从)+TargetLevel+Port+DeviceID+mode+BuffSize+Buff
slave发送：0x02+0x11(从到主)+CurrentLevel+Port+DeviceID+mode

3.从机传感器设备模式设置：
host发送：0x03+0x10(主到从)+TargetLevel+Port+DeviceID+mode
slave发送：0x03+0x11(从到主)+CurrentLevel+Port+DeviceID+mode+BuffSize+DataBuff

4.从机所连设备上报master
slave发送：0x04+0x11(从到主)+CurrentLevel+DeviceMessage[8](依次8个端口的设备id)

5.向master发送请求编号更新功能：
slave发送：0x05+0x11(从到主)+CurrentLevel

6.向下级slave发送重启usb命令：
slave发送：0x06+0x10(主到从)+CurrentLevel
*/

#define USB_ADCCH	6			//usb使用ADC通道6来测量电压

//表示从机端口连接状态
//0,没有连接;
//1,已经连接;
u8 SlaveComConnected = 0;
u8 SlaveComConnected_old = 0;

u8 CascadeLevel = 0; 	//本机的级联位置
u8 CascadeNum = 0; 		//级联数量
u8 CascadeCheck = 0; 	//级联通道检测
u8 CascadeMaster = 0;	//级联主机标志，0代表没有级联、1代表级联主机、2代表级联非主机
u8 usbrestart = 0;		//usb重启标志位

Cascade_Function_TypeDef	Cascade_Function;

//Cascade_flag_TypeDef  Cascade_flag[4][8];


//----------------------级联下的主机及从机数据包发送函数----------------------------

//排序编号更新包
u8 HostSend_SortUpdate_Packet(u8 Current_Level)
{
	int _cnt=0;
	u8 data_to_send[3]={0};
	
	data_to_send[_cnt++]=0x01;					//功能码
	data_to_send[_cnt++]=0x10;					//host到slave
	data_to_send[_cnt++]=Current_Level;	//当前级联级别 0-3
	
	return USBH_SendData(data_to_send, _cnt);
}
//排序编号反馈包
void SlaveReply_SortUpdate_Packet(u8 Current_Level)
{
	int _cnt=0;
	static u8 data_to_send[3]={0};
	
	data_to_send[_cnt++]=0x01;					//功能码
	data_to_send[_cnt++]=0x11;					//host到slave
	data_to_send[_cnt++]=Current_Level;	//当前级联级别 0-3
	
	HID_SendBuffToHost(data_to_send, _cnt);
}


//从机设备控制数据包
//u8 Buff[]为控制所需的数据包
u8 HostSend_DControl_Packet(u8 Target_Level, u8 Port, u8 DeviceId, u8 mode , u8 Buff[], u8 BuffSize)
{
	int _cnt=0,i;
	u8 data_to_send[21]={0};
	
	data_to_send[_cnt++]=0x02;					//功能码
	data_to_send[_cnt++]=0x10;					//host到slave
	data_to_send[_cnt++]=Target_Level;	//目标级联级别 0-3
	data_to_send[_cnt++]=Port;					//控制端口
	data_to_send[_cnt++]=DeviceId;			//控制端口的设备
	data_to_send[_cnt++]=mode;					//控制模式
	data_to_send[_cnt++]=BuffSize;
	for(i=0;i<BuffSize;i++)
		data_to_send[_cnt++]=Buff[i];
	
	return USBH_SendData(data_to_send, _cnt);
//	printf("ENTER\n");
}
//从机设备控制任务完成反馈包
void SlaveReply_DControl_Packet(u8 Current_Level, u8 Port, u8 DeviceId, u8 mode)
{
	int _cnt=0;
	static u8 data_to_send[5]={0};
	
	data_to_send[_cnt++]=0x02;					//功能码
	data_to_send[_cnt++]=0x11;					//host到slave
	data_to_send[_cnt++]=Current_Level;	//当前级联级别 0-3
	data_to_send[_cnt++]=Port;					//控制端口
	data_to_send[_cnt++]=DeviceId;			//控制端口的设备
	data_to_send[_cnt++]=mode;					//控制模式
	
	HID_SendBuffToHost(data_to_send, _cnt);
}


//从机传感器工作模式设置包
//Port范围：1-8
u8 HostSend_DDataGet_Packet(u8 Target_Level, u8 Port, u8 DeviceId, u8 mode)
{
	int _cnt=0;
	u8 data_to_send[6]={0};
	
	data_to_send[_cnt++]=0x03;					//功能码
	data_to_send[_cnt++]=0x10;					//host到slave
	data_to_send[_cnt++]=Target_Level;	//目标级联级别 0-3
	data_to_send[_cnt++]=Port;					//控制端口
	data_to_send[_cnt++]=DeviceId;			//控制端口的设备
	data_to_send[_cnt++]=mode;					//控制模式
	
//	Channel_State[Target_Level*8+Port-1][0] = Port;
//	Channel_State[Target_Level*8+Port-1][1] = DeviceId;	//id不能由它改变
//	Channel_State[Target_Level*8+Port-1][2] = mode;
	
	return USBH_SendData(data_to_send, _cnt);
}
//从机设备控制任务完成反馈包
//u8 DataBuff[] 从机传感器某项模式下的输出数据
void SlaveReply_DDataGet_Packet(u8 Current_Level,u8 Channel_State[32][13])
{
	int _cnt=0,i,j;
	static u8 data_to_send[63]={0};
	
	data_to_send[_cnt++]=0x03;					//功能码
	data_to_send[_cnt++]=0x11;					//host到slave
	data_to_send[_cnt++]=Current_Level;	//当前级联级别 0-3
	
	for(i=0;i<4;i++)
	{
		for(j=0;j<10;j++)
			data_to_send[_cnt++]=Channel_State[i][3+j];
	}
	for(i=4;i<8;i++)
	{
		for(j=0;j<4;j++)
			data_to_send[_cnt++]=Channel_State[i][3+j];
	}
	
	HID_SendBuffToHost(data_to_send, _cnt);
}


//从机所连设备上报包
//u8 DeviceMessage[16]:从机当前8个端口信息，分别为Port1DeviceID + .... + Port8DeviceID
void SlaveReply_ConDevice_Packet(u8 Current_Level, u8 Channel_State[32][13])
{
	int _cnt=0,i;
	static u8 data_to_send[27]={0};
	
	data_to_send[_cnt++]=0x04;					//功能码
	data_to_send[_cnt++]=0x11;					//host到slave
	data_to_send[_cnt++]=Current_Level;	//当前级联级别 0-3
	for(i=0;i<8;i++)
	{
		data_to_send[_cnt++]=Channel_State[i][0];	//端口信息
		data_to_send[_cnt++]=Channel_State[i][1];	//端口信息
		data_to_send[_cnt++]=Channel_State[i][2];	//端口信息
	}
	
	HID_SendBuffToHost(data_to_send, _cnt);
}

//向master主机发送请求编号更新包
//u8 DeviceMessage[16]:从机当前8个端口信息，分别为Port1DeviceID + .... + Port8DeviceID
void SlaveReply_RequestUpdate_Packet()
{
	int _cnt=0;
	static u8 data_to_send[2]={0};
	
	data_to_send[_cnt++]=0x05;					//功能码
	data_to_send[_cnt++]=0x11;					//host到slave

	HID_SendBuffToHost(data_to_send, _cnt);
}


//u8 Buff[]为控制所需的数据包
u8 HostSend_USBReset_Packet(void)
{
	int _cnt=0;
	u8 data_to_send[2]={0};
	
	data_to_send[_cnt++]=0x06;					//功能码
	data_to_send[_cnt++]=0x10;					//host到slave

	return USBH_SendData(data_to_send, _cnt);
}


//----------------------usb端口数据接收处理----------------------------

//usb device端口接收数据的解码
//放在usb device接收主机数据位置执行
void SlaveCom_DataDecode(u8 RecBuff[], int RecBuffSize)
{
	int i,count;
	
	switch(RecBuff[0])	//功能码
	{
		case 0x01:				//编码更新
			CascadeLevel = RecBuff[2]+1;	//RecBuff[2]代表了上级编号
			if(HostComConnected==0 && SlaveComConnected==1)		//如果为 最后一级主控器
			{
				SlaveReply_SortUpdate_Packet(CascadeLevel);
			}
			else		//否则 继续给下级发送编号更新包
			{
				HostSend_SortUpdate_Packet(CascadeLevel);
			}
			break;
		case 0x02:				//设备控制
			if(CascadeLevel == RecBuff[2])	//如果 本机是目标收件方
			{
				//将数据存入响应位置并置标志位
				count = 4;
	
				Cascade_Function.Cascade_Order[RecBuff[3]-1].id 				= RecBuff[count++];
				Cascade_Function.Cascade_Order[RecBuff[3]-1].mode 			= RecBuff[count++];
				Cascade_Function.Cascade_Order[RecBuff[3]-1].bufflength = RecBuff[count++];
				for(i=0;i<Cascade_Function.Cascade_Order[RecBuff[3]-1].bufflength;i++)
				{
					Cascade_Function.Cascade_Order[RecBuff[3]-1].buff[i] 	= RecBuff[count++];
				}
				Cascade_Function.Cascade_Order[RecBuff[3]-1].flag = 1;

				Cascade_Function.allflag 												= 1;
				
#ifdef	CASCADE_DEBUG
					printf("		存入控制指令\n");
				
					printf("i %x  \r\n",i);
					printf("ID %x  \r\n",Cascade_Function.Cascade_Order[RecBuff[3]-1].id);
					printf("MODE %x  \r\n",Cascade_Function.Cascade_Order[RecBuff[3]-1].mode);
					printf("FLAG %x  \r\n",Cascade_Function.Cascade_Order[RecBuff[3]-1].flag);
					printf("bufflength %x  \r\n",Cascade_Function.Cascade_Order[RecBuff[3]-1].bufflength);
#endif
				
			}
			else		//否则 原封不动继续给下级发送控制包
			{
				USBH_SendData(RecBuff, RecBuffSize);
			}
			break;
		case 0x03:				//传感器模式设置
			if(CascadeLevel == RecBuff[2])	//如果 本机是目标收件方
			{
				//将数据存入响应位置并置标志位
				count = 4;
				Cascade_Function.Cascade_Order[RecBuff[3]-1].id 	= RecBuff[count++];
				Cascade_Function.Cascade_Order[RecBuff[3]-1].mode = RecBuff[count++];
				Cascade_Function.Cascade_Order[RecBuff[3]-1].flag = 1;
				Cascade_Function.allflag 													= 1;
				
				
#ifdef	CASCADE_DEBUG
				printf("		存入传感器模式设置指令\n");
#endif
				
			}
			else		//否则 原封不动继续给下级发送控制包
			{
				USBH_SendData(RecBuff, RecBuffSize);
			}
			break;
		case 0x06:				//usb restart设置
			
			usbrestart = 1;
			USBH_SendData(RecBuff, RecBuffSize);		//继续给下级发送控制包

			break;
	}
}

//usb host端口接收数据的解码
//放在usb host接收从机数据位置执行
void HostCom_DataDecode(u8 RecBuff[], int RecBuffSize)
{
	int i,j;
	
	switch(RecBuff[0])	//功能码
	{
		case 0x01:				//级联数量反馈
			if(CascadeMaster==1)		//如果为 Maaster主控器
			{
				CascadeNum = RecBuff[2];	//RecBuff[2]代表了上级编号
				CascadeCheck = CascadeNum;
			}
			else		//否则 原封不动继续给上级发送级联数量反馈包
			{
				HID_SendBuffToHost(RecBuff, RecBuffSize);
			}
			break;
		case 0x02:				//设备控制
			if(CascadeMaster==1)		//如果为 Maaster主控器
			{
				//控制完成
#ifdef	CASCADE_DEBUG
				printf("		Level:%d,Port:%d,ID:%d,Mode:%d  控制完成\n",RecBuff[2],RecBuff[3],RecBuff[4],RecBuff[5]);
#endif
				

				Cascade_Function.Cascade_flag[RecBuff[2]][RecBuff[3]].id		= RecBuff[4];
				Cascade_Function.Cascade_flag[RecBuff[2]][RecBuff[3]].mode	= RecBuff[5];
				Cascade_Function.Cascade_flag[RecBuff[2]][RecBuff[3]].flag = 1;
			}
			else		//否则 原封不动继续给上级发送任务完成包
			{
				HID_SendBuffToHost(RecBuff, RecBuffSize);
			}
			break;
		case 0x03:				//传感器模式设置
			if(CascadeMaster==1)		//如果为 Master主控器
			{
				//获取传感器数据
				
//#ifdef	CASCADE_DEBUG
				printf("		收到Level:%d  上传的数据：\n",RecBuff[2]);
//#endif
				
				
				for(i=0;i<4;i++)		//1-4号端口
				{
					for(j=0;j<10;j++)
					{
						Channel_State[RecBuff[2]*8+i][3+j] = RecBuff[3+i*10+j];
					}
				}
				for(i=0;i<4;i++)		//5-8号端口
				{
					for(j=0;j<4;j++)
					{
						Channel_State[RecBuff[2]*8+4+i][3+j] = RecBuff[43+i*4+j];
					}
				}
				
			}
			else		//否则 原封不动继续给上级
			{
				HID_SendBuffToHost(RecBuff, RecBuffSize);
			}
			break;
		case 0x04:				//从机所连设备信息
			if(CascadeMaster==1)		//如果为 Maaster主控器
			{
				//获取从机所连设备信息
				
#ifdef	CASCADE_DEBUG
				printf("		收到从机Level:%d上传的设备信息：\n",RecBuff[2]);
#endif

				for(i=0;i<8;i++)
				{
					for(j=0;j<3;j++)
					{
						Channel_State[RecBuff[2]*8+i][j] = RecBuff[3+i*3+j];
					}
				}
			}
			else		//否则 原封不动继续给上级
			{
				HID_SendBuffToHost(RecBuff, RecBuffSize);
			}
			break;
		case 0x05:				//从机请求编码更新
			if(CascadeMaster==1)		//如果为 Maaster主控器
			{
				//发送编号包
				HostSend_SortUpdate_Packet(0x00);
			}
			else		//否则 原封不动继续给上级
			{
				HID_SendBuffToHost(RecBuff, RecBuffSize);
			}
			break;
	}
	
}


//----------------------级联工作进程----------------------------

//级联编号更新进程
//1s执行一次
void UpdateCascadeNum_Process(void)
{
	static u8 HostComConnectedOld = 0;
	static u8 ComIDOld[9] = {0};
	static u8 SendCount = 0;
//	static u8 time100ms = 0;
	int i;
	
	if(usbmasterflag)	//说明是master，但不一定更电脑联通或有级联
	{
		if(HostComConnected==1)		//说明处于级联状态
		{
			CascadeLevel = 0;
			CascadeMaster = 1;
			if(!CascadeNum)
			{
				SendCount++;
				if(SendCount>=10)
				{
					HostSend_USBReset_Packet();
//					usbrestart = 1;
					SendCount = 0;

#ifdef	CASCADE_DEBUG
				printf("重启usb\n");
#endif
				
				}
				else{
					//发送编号包
					HostSend_SortUpdate_Packet(0x00);
				}
			}
			else{
				SendCount = 0;
			}
		}
		else{		// 没有级联
			CascadeLevel = 0; 	//清空本机的级联位置
			CascadeNum = 0; 		//级联数量
			CascadeMaster = 0;
		}
	}
	else	//也可能是master或者从机
	{
		if(HostComConnected==1&&SlaveComConnected==0)				//如果为 级联下的Maaster主控器
		{
			CascadeLevel = 0;
			CascadeMaster = 1;
			if(!CascadeNum || !CascadeCheck)
			{
				//发送编号包
				HostSend_SortUpdate_Packet(0x00);
				
				SendCount++;
				if(SendCount>=10)
				{
//					HostSend_USBReset_Packet();
//					usbrestart = 1;
					SendCount = 0;
					
#ifdef	CASCADE_DEBUG
				printf("重启usb\n");
#endif
					
				}
			}
			else{
				SendCount = 0;
			}
		}
		else if(HostComConnected==0 && SlaveComConnected==0)		//如果 没有级联
		{
			CascadeLevel = 0; 	//清空本机的级联位置
			CascadeNum = 0; 		//级联数量
			CascadeMaster = 0;
		}
		else	//如果 级联但不是master
		{
			CascadeNum = 0; 		//级联数量
			CascadeMaster = 2;
			if(!CascadeLevel)
			{
				//向上级发送编码请求
				SlaveReply_RequestUpdate_Packet();
			}
		}
	}
	
	
	if(CascadeMaster==2)		//级联非主机
	{
		//---slave设备的插入与拔出---
		if(!HostComConnectedOld && HostComConnected)	//如果 插入了slave设备
		{
			//向上级发送编码请求
			SlaveReply_RequestUpdate_Packet();
		}
		else if(HostComConnectedOld && !HostComConnected)	//如果	拔出了slave设备
		{
			//向上级发送编码请求
			SlaveReply_RequestUpdate_Packet();
		}
		HostComConnectedOld = HostComConnected;
		
		
		//---从机外设插入与拔出---
		for(i=0;i<8;i++)
		{
			if(!ComIDOld[i] && Channel_State[i][1])			//如果 识别到外设
			{
				//上传外设信息
				SlaveReply_ConDevice_Packet(CascadeLevel,Channel_State);
			}
			else if(ComIDOld[i] && !Channel_State[i][1])		//如果 外设拔出
			{
				//上传外设信息
				SlaveReply_ConDevice_Packet(CascadeLevel,Channel_State);
			}
			ComIDOld[i] = Channel_State[i][1];
		}
		
	}
	
//	//---3s发送一次usb通道检测信息---
//	if(CascadeMaster==1)		//级联主机
//	{
//		time100ms++;
//		if(time100ms>=30)
//		{
//			CascadeCheck = 0;
//			time100ms = 0;
//		}
//	}
	
	
}

//从机传感器数据上传进程
//100ms执行一次
void SensorDataUpload_Process(void)
{
	if(CascadeMaster==2)		//级联非主机
	{
		if(Channel_State[0][1]||Channel_State[1][1]||Channel_State[2][1]||Channel_State[3][1]||		//如果 从机端口有设备存在
		Channel_State[4][1]||Channel_State[5][1]||Channel_State[6][1]||Channel_State[7][1])
		{
			//从机上传数据
			SlaveReply_DDataGet_Packet(CascadeLevel,Channel_State);
		}
	}
}


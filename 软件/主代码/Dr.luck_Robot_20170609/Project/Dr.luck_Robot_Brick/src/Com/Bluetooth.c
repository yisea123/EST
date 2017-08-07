#include "Bluetooth.h"
#include "ui_pic.h"
#include "uc1638c.h"
#include "main.h"
#include "usart6.h"
u8 bluetooth_locate=3;
u8 bluetooth_j=3;  //标识更新辅助变量
u8 control = 0;  //界面转换
u8 interface_state = 0;    //状态切换   1代表正在蓝牙主界面，需加标识带  2代表返回蓝牙主界面，执行返回主界面后立刻打开蓝牙主界面
u8 bluetooth_start = 1;    //蓝牙开启
u8 bluetooth_visual = 0;  //蓝牙可视化
u8 device[9][15];           //搜索设备1
u8 dis_sum = 0;               //搜索设备数量
u8 device_sum = 0;          //绑定设备数量
u8 device_bond[9][15];      //绑定设备
u8 bluetooth_state = 0;     //蓝牙当前状态   0：断开  1：断开等待关闭    2：设备1连接    3：设备2连接    4：设备3连接
u8 time_open = 0;               //蓝牙转为主机后进行搜索缓冲时间
u8 time_break = 0;              //断开蓝牙后转为从机缓冲时间
u8 time_dis = 0;                //蓝牙搜索等待时间
u8 time_link = 0;                //连接搜索设备等待的过渡界面 
u8 time_close = 0;               //蓝牙转为从机后关闭蓝牙缓冲时间
u8 time_bondlink = 0;           //蓝牙绑定设备连接等待时间
u8 time_clear = 0;             //主机连接响应后蓝牙处理绑定信息缓冲时间
u8 bluetooth_judge = 0;     //判断当前操作哪个设备
u16 len = 0;              //usart6得到的数据长度
u8 t = 0;
u8 judge_dis = 0;   //判断是在设备界面进入操作界面还是搜索界面进入操作界面   0：设备与搜索界面   1：搜索结果界面连接蓝牙   2：搜索结果界面移除蓝牙
u8 bluetooth_wait = 0;             //缓冲界面辅助变量
u8 memory_Tab_Level = 0;      //主机连接响应后回到响应前的界面的记忆变量
u8 bluetooth_window = 0;        //滚动窗口
u8 position_judge = 0;       //界面标识带辅助变量
u8 at_id=0;
u8 bluetooth_role = 0;
u8 device_name[15];
u8 blue_1_state=0;
u8 blue_2_state=0;
u8 blue_3_state=0;
void bluetooth_judge_key();   //确定键判断
int diffrence_judge(int num);
void bluetooth_msg()       //数据判断
{
	u8 i;
  u8 temp = 0;
//	if(USART6_RX_STA&0x8000)
//	{	
//		len = USART6_RX_STA;  //得到此次接收到的数据长度						
		USART_ClearFlag(USART6,USART_IT_TC);

		if(USART6_Receive("et:0"))      
		{
			if(at_id==1)      //开启蓝牙
			{
				bluetooth_start = 1; 
//			W25QXX_Write((u8*)&bluetooth_start,16*1024*1024+300,sizeof(bluetooth_start));	
				interface_state = 1;
				status=0;
				printf("AT+RESET");
			}
			else if(at_id==2)    //蓝牙切换成从机
			{
				if(bluetooth_state == 1)
				{
					bluetooth_state = 0;
					time_close = 1;
				}		
			}
		}
		if(USART6_Receive("et:1"))  
		{
			if(at_id==2)       //蓝牙切换成主机
			{
				if(time_bondlink == 0)
					time_open = 1;
			}
			else if(at_id==3)
			{
				status=0;
				printf("AT+SHOW1");
				at_id=4;
			}
			else if(at_id==4)
			{
				status=0;
				printf("AT+NOTI1");
				at_id=5;
			}
			else if(at_id==5)
			{
				status=0;
				strcpy(device_name,"EST(");
				printf("AT+NAMEEST");
				at_id=6;
				
			}
		}
		if(USART6_Receive("et:E"))  
		{
			if(at_id==6)
			{
				status=0;
				USART6_RX_STA = 0;
				printf("AT+ADDR?");
				at_id=0;
			}
		}
		if(USART6_Receive("et:3"))  
		{
			if(at_id==1)    //关闭蓝牙
			{
				bluetooth_start = 0;
	//		W25QXX_Write((u8*)&bluetooth_start,16*1024*1024+300,sizeof(bluetooth_start));	
				interface_state = 1;
				Tab[tab_flag.MainTab_choice].Location = 2;
				UI_Update(tab_flag.Tab_Level,tab_flag.MainTab_choice,Tab);
				NotificationBar();                    //通知栏
				status=0;
				printf("AT+RESET");
			}
		}
		if(USART6_Receive("DDR:"))  
		{
				len=USART6_RX_STA;//得到此次接收到的数据长度	
				for(t = 12;t<len;t++)
				{
					device_name[t-8] = USART6_RX_BUF[t];							
				}
				device_name[t-8]=')';
				t++;
				device_name[t-8]='\0';
				W25QXX_Write( (u8*)&device_name,16*1024*1024+600,sizeof(device_name) );
		}
		if(USART6_Receive("ONN"))     //连接
		{
			time_link = 0;
			if(bluetooth_role==1)
			{
				printf("OK+NAM:%s\r\n",device_name);
			LCD_Display_pic(22, 57, 130, 38,bluetooth_connect);
			if(judge_dis == 1)  //从搜索结果界面连接蓝牙，更新绑定设备
			{
				if(diffrence_judge(device_sum))    ////判断是否已经绑定并获取当前连接设备号
				{
					strcpy(device_bond[0]," ");
					W25QXX_Write((u8*)&device_bond[0],16*1024*1024+500,sizeof(device_bond[0]));		
					strcpy((u8*)device_bond[0],(u8*)device[bluetooth_judge]);
					W25QXX_Write( (u8*)&device_bond[0],16*1024*1024+500,sizeof(device_bond[0]) );
					device_sum=1;
					W25QXX_Write( (u8*)&device_sum,16*1024*1024+400,sizeof(device_sum) );
				}
			}
			
				bluetooth_state = 2;
			NotificationBar();                    //通知栏
		}
		else
		{
				memory_Tab_Level = tab_flag.Tab_Level;
				tab_flag.Tab_Level = 7;
				bluetooth_locate=3;
				bluetooth_j=3;
		}
		}
		if(USART6_Receive("OST"))    //断开
		{
			at_id=2;
			bluetooth_role = 0;		
			if(bluetooth_state != 1)
			bluetooth_state = 0; 
		}
		if(USART6_Receive("AM:"))      //接收主机信息
		{
				len=USART6_RX_STA-1;//得到此次接收到的数据长度	
				for(t = 3;t<len;t++)
				{
						device[0][t-3] = USART6_RX_BUF[t];							
				}
				device[0][t-3]='\0';
				printf("%s",device[0]);
				if(diffrence_judge(device_sum))    ////判断是否已经绑定并获取当前连接设备号
				{
					strcpy(device_bond[0]," ");
					W25QXX_Write((u8*)&device_bond[0],16*1024*1024+500,sizeof(device_bond[0]));		
					strcpy((u8*)device_bond[0],(u8*)device[0]);
					W25QXX_Write((u8*)&device_bond[0],16*1024*1024+500,sizeof(device_bond[0]) );
					device_sum=1;
					W25QXX_Write((u8*)&device_sum,16*1024*1024+400,sizeof(device_sum) );
				}
		}
}
void bluetooth_respond(u8 *ButtonAction)       //蓝牙界面响应
{
//	tab_flag.update_flag = 1;				//不需要更新
//	tab_flag.sound_flag = 1;
	switch(control)//0:蓝牙主界面 1：设备与搜索界面 2：搜索结果界面 3：设备操作界面 4：提示界面
	{
		case 0:               //0:蓝牙主界面 
				switch(*ButtonAction)        //按键响应       bluetooth_locate；0连接  1显示  2蓝牙  3退出
				{
					case 0:
							tab_flag.update_flag = 1;
							tab_flag.sound_flag = 0;
							break;
					
					case 1:			//上
							if(bluetooth_locate > 0)
								bluetooth_locate--;	
							else
								tab_flag.sound_flag = 0;
							interface_state=3;
						break;
						
					case 2:			//下
							if(bluetooth_locate < 3)
								bluetooth_locate++;
							else
								tab_flag.sound_flag = 0;
							interface_state=3;
						break;
						
					case 3:			//左
//							tab_flag.update_flag = 0;				//不需要更新
							tab_flag.sound_flag = 0;
					interface_state=4;
						break;
						
					case 4:			//右
//							tab_flag.update_flag = 0;				//不需要更新
							tab_flag.sound_flag = 0;
							interface_state=4;
						break;
					
					case 5:			//确认
//							tab_flag.update_flag = 1;
//							tab_flag.sound_flag = 1;
							bluetooth_judge_key();
						break;
					
					case 6:			//返回
							bluetooth_locate = 3;
							bluetooth_j = 3;                //位置重置
							interface_state = 0;
							tab_flag.Tab_Level = 0;					//选择主界面
							Tab[tab_flag.MainTab_choice].SubLocation[Tab[tab_flag.MainTab_choice].Location] = 0;			//子函数坐标清零
							//主函数坐标不必清零
						break;	
					default:
							tab_flag.update_flag = 0;
							tab_flag.sound_flag = 0;	
							break;					
				}
			break;
		case 1:                //1：设备与搜索界面
				switch(*ButtonAction)         //按键响应    bluetooth_locate；  2搜索 3退出
				{
					case 0:
							tab_flag.update_flag = 1;
							tab_flag.sound_flag = 0;
							break;
					
					case 1:			//上
							if(bluetooth_locate > (9-device_sum))
								bluetooth_locate--;
							else
								tab_flag.sound_flag = 0;
							if(bluetooth_locate < 6-bluetooth_window)
							{
									bluetooth_window++;
									tab_flag.update_flag = 1;
									tab_flag.sound_flag = 1;
							}
							blue_1_state=1;
						break;	
							
					case 2:			//下
							if(bluetooth_locate < 10)
								bluetooth_locate++;
							else
								tab_flag.sound_flag = 0;
							if((bluetooth_locate > (8-bluetooth_window)) && (bluetooth_locate < 9))
							{
								bluetooth_window--;
								tab_flag.update_flag = 1;				
								tab_flag.sound_flag = 1;
							}
							blue_1_state=1;

						break;

					case 3:			//左
//							tab_flag.update_flag = 0;				//不需要更新
							tab_flag.sound_flag = 0;
						break;
						
					case 4:			//右
//							tab_flag.update_flag = 0;				//不需要更新
							tab_flag.sound_flag = 0;
						break;
					
					case 5:			//确认
						blue_1_state=0;
							tab_flag.update_flag = 1;				
							tab_flag.sound_flag = 1;
							bluetooth_judge_key();
						break;
					
					case 6:			//返回
								blue_1_state=0;
								tab_flag.update_flag = 1;				
								tab_flag.sound_flag = 1;
								bluetooth_locate = 3;				
								bluetooth_j = 3;
								control = 0;
								bluetooth_window = 0;
								interface_state = 2;
								tab_flag.Tab_Level = 0;					//选择主界面
								Tab[tab_flag.MainTab_choice].SubLocation[Tab[tab_flag.MainTab_choice].Location] = 0;			//子函数坐标清零						
						break;
					default:
							tab_flag.update_flag = 0;
							tab_flag.sound_flag = 0;	
							break;
						
				}

			break;
		case 2:              //2：搜索结果界面
				if(bluetooth_start == 1)       //蓝牙开启情况
				{
					switch(*ButtonAction)        //按键响应    bluetooth_locate；  0第一个设备  1第二个设备 。。。
					{
					case 0:
							tab_flag.update_flag = 1;
							tab_flag.sound_flag = 0;
							break;
						
						case 1:			//上
							if(bluetooth_locate > 0)
								bluetooth_locate--;
							else
								tab_flag.sound_flag = 0;
							if(bluetooth_locate < bluetooth_window)
							{
								position_judge = 1;
								bluetooth_window--;
								tab_flag.update_flag = 1;				
								tab_flag.sound_flag = 1;
							}
							blue_2_state=1;
							break;
							
						case 2:			//下
							if(bluetooth_locate < dis_sum-1)
								bluetooth_locate++;
							else
								tab_flag.sound_flag = 0;
							if(bluetooth_locate > (4+bluetooth_window))
							{
								position_judge = 1;
								bluetooth_window++;
								tab_flag.update_flag = 1;				
								tab_flag.sound_flag = 1;
							}
							blue_2_state=1;
							break;
							
						case 3:			//左
//								tab_flag.update_flag = 0;				//不需要更新
								tab_flag.sound_flag = 0;
							break;
							
						case 4:			//右
//								tab_flag.update_flag = 0;				//不需要更新
								tab_flag.sound_flag = 0;
							break;
						
						case 5:			//确认
							blue_2_state=0;
								if(dis_sum > 0)
									bluetooth_judge_key();
								tab_flag.update_flag = 1;				
								tab_flag.sound_flag = 1;
							break;
						
						case 6:			//返回
							blue_2_state=0;
								tab_flag.update_flag = 1;				
								tab_flag.sound_flag = 1;
								at_id=2;
								bluetooth_role = 0;						
								printf("AT+ROLE0");
								dis_sum = 0;
								bluetooth_locate = 9;
								bluetooth_j = 9;
								control = 1;
								bluetooth_window = 0;
								strcpy(device[0]," ");
							break;
						default:
							tab_flag.update_flag = 0;
							tab_flag.sound_flag = 0;	
							break;
							
					}
					if(((*ButtonAction == 1) || (*ButtonAction == 2)) && (dis_sum > 0) )//标识
					{
						if(bluetooth_j != bluetooth_locate)            //判断标识有无更新
						{
							LCD_Invert(29, 34+(bluetooth_j-bluetooth_window)*18, 113, 14);  //旧标识消失
							bluetooth_j = bluetooth_locate;
							LCD_Invert(29, 34+(bluetooth_locate-bluetooth_window)*18, 113, 14); //新标识显示
						}
					}
				}
				else           //蓝牙关闭情况
					switch(*ButtonAction)        //按键响应    bluetooth_locate；  0第一个设备  1第二个设备 。。。
					{
						case 0:
//								tab_flag.update_flag = 0;
//								tab_flag.sound_flag = 0;
							break;
						
						case 1:			//上
//								tab_flag.update_flag = 0;				//不需要更新
								tab_flag.sound_flag = 0;
							break;
							
						case 2:			//下
//								tab_flag.update_flag = 0;				//不需要更新
								tab_flag.sound_flag = 0;
							break;
							
						case 3:			//左
//								tab_flag.update_flag = 0;				//不需要更新
								tab_flag.sound_flag = 0;
							break;
							
						case 4:			//右
//								tab_flag.update_flag = 0;				//不需要更新
								tab_flag.sound_flag = 0;
							break;
						
						case 5:			//确认
								tab_flag.update_flag = 1;				
								tab_flag.sound_flag = 1;
								bluetooth_locate = 3;
								bluetooth_j = 3;                //位置重置
								interface_state = 0;
								control = 0;
								tab_flag.Tab_Level = 0;					//选择主界面
								Tab[tab_flag.MainTab_choice].SubLocation[Tab[tab_flag.MainTab_choice].Location] = 0;			//子函数坐标清零
								//主函数坐标不必清零
							break;
						
						case 6:			//返回
								tab_flag.update_flag = 1;				
								tab_flag.sound_flag = 1;
								bluetooth_locate = 3;
								bluetooth_j = 3;                //位置重置
								interface_state = 0;
								control = 0;
								tab_flag.Tab_Level = 0;					//选择主界面
								Tab[tab_flag.MainTab_choice].SubLocation[Tab[tab_flag.MainTab_choice].Location] = 0;			//子函数坐标清零
								//主函数坐标不必清零
							break;	
						default:
							tab_flag.update_flag = 0;
							tab_flag.sound_flag = 0;	
							break;						
					}
			break;
		case 3:               //3：设备操作界面
				switch(*ButtonAction)          //按键响应         bluetooth_locate； 2移除    3连接/断开
				{
					case 0:
							tab_flag.update_flag = 1;
							tab_flag.sound_flag = 0;
							break;
					
					case 1:			//上
							if(bluetooth_locate > 2)
								bluetooth_locate--;
							blue_3_state=1;
//							tab_flag.update_flag = 0;				//不需要更新
//							tab_flag.sound_flag = 0;
						break;
						
					case 2:			//下
							if(bluetooth_locate < 3)
								bluetooth_locate++;
							blue_3_state=1;
//							tab_flag.update_flag = 0;				//不需要更新
//							tab_flag.sound_flag = 0;
						break;
						
					case 3:			//左
//							tab_flag.update_flag = 0;				//不需要更新
							tab_flag.sound_flag = 0;
						break;
						
					case 4:			//右
//							tab_flag.update_flag = 0;				//不需要更新
							tab_flag.sound_flag = 0;
						break;
					
					case 5:			//确认
							blue_3_state=0;
							tab_flag.update_flag = 1;				
							tab_flag.sound_flag = 1;
							bluetooth_judge_key();
						break;
					
					case 6:			//返回
							blue_3_state=0;
							tab_flag.update_flag = 1;				
							tab_flag.sound_flag = 1;
							if(judge_dis == 1)        //返回到搜索结果界面
							{
								position_judge = 1;
								bluetooth_locate = bluetooth_judge;
								bluetooth_j = bluetooth_judge;
								control = 2;
							}
							else                    //返回到设备与搜索界面
							{
								blue_1_state==0;
								bluetooth_window = 0;
								bluetooth_locate = 9;
								bluetooth_j = 9;
								control = 1;
							}
						break;	
					default:
							tab_flag.update_flag = 0;
							tab_flag.sound_flag = 0;	
							break;							
				}
			break;
		case 4:                //4：提示界面
				switch(*ButtonAction)     //按键响应
				{
					case 0:
							tab_flag.update_flag = 1;
							tab_flag.sound_flag = 0;
							break;
					
					case 1:			//上
//							tab_flag.update_flag = 0;				//不需要更新
							tab_flag.sound_flag = 0;
						break;
						
					case 2:			//下
//							tab_flag.update_flag = 0;				//不需要更新
							tab_flag.sound_flag = 0;
						break;
						
					case 3:			//左
//							tab_flag.update_flag = 0;				//不需要更新
							tab_flag.sound_flag = 0;
						break;
						
					case 4:			//右
//							tab_flag.update_flag = 0;				//不需要更新
							tab_flag.sound_flag = 0;
						break;
					
					case 5:			//确认
							tab_flag.update_flag = 1;				
							tab_flag.sound_flag = 1;
							if((judge_dis == 1) && (time_link > 31) || (judge_dis == 2))  //从搜索结果界面连接蓝牙失败或移除搜索设备，返回搜索结果界面
							{
								position_judge = 1;
								time_link = 0;
								bluetooth_locate = bluetooth_j;
								control = 2;
								if(judge_dis == 2)                //移除设备后对最后一个设备进行判断
								{
									if(bluetooth_locate == dis_sum)
									{
										bluetooth_locate--;
										bluetooth_j--;
									}
								}								
							}
							else              //从搜索结果界面连接成功，返回设备与搜索界面，从设备与搜索界面连接，无论成功或失败都返回设备与搜索界面
							{
								dis_sum = 0;
								bluetooth_window = 0;
								bluetooth_locate = 9;
								bluetooth_j = 9;
								control = 1;
							}
						break;
					
					case 6:			//返回
							tab_flag.update_flag = 1;				
							tab_flag.sound_flag = 1;
							if(((judge_dis == 1) && (time_link > 31)) || (judge_dis == 2))  //从搜索结果界面连接蓝牙失败或移除搜索设备，返回搜索结果界面
							{
								position_judge = 1;
								time_link = 0;
								bluetooth_locate = bluetooth_j;
								control = 2;                  //2：搜索结果界面 
								if(judge_dis == 2)                         //移除设备后对最后一个设备进行判断
								{
									if(bluetooth_locate == dis_sum)
									{
										bluetooth_locate--;
										bluetooth_j--;
									}
								}								
							}
							else              //从搜索结果界面连接成功，返回设备与搜索界面，从设备与搜索界面连接，无论成功或失败都返回设备与搜索界面
							{
								dis_sum = 0;
								bluetooth_window = 0;
								bluetooth_locate = 9;
								bluetooth_j = 9;
								control = 1;                  //1：设备与搜索界面
							}
						break;
					default:
							tab_flag.update_flag = 0;
							tab_flag.sound_flag = 0;	
							break;							
				}
			break;
	}
}
void bluetooth_judge_key() //确定键判断
{
	u8 i = 0;
	switch(control)        //0:蓝牙主界面 1：设备与搜索界面 2：搜索结果界面 3：设备操作界面 4：提示界面
	{
		case 0:          //0:蓝牙主界面   
				switch(bluetooth_locate)
				{
					case 0:         //切换界面
							bluetooth_locate = 9;         
							bluetooth_j = 9;
							control = 1;     //1：设备与搜索界面     bluetooth_locate； 搜索
						break;
					case 1:          //可视化
							if(bluetooth_state == 0)
							{
									bluetooth_visual = 1-bluetooth_visual;
									W25QXX_Write((u8*)&bluetooth_visual,16*1024*1024+200,sizeof(bluetooth_visual));
									interface_state = 1;
									Tab[tab_flag.MainTab_choice].Location = 2;
									UI_Update(tab_flag.Tab_Level,tab_flag.MainTab_choice,Tab);
//									tab_flag.update_flag = 0;				//不需要更新
//									tab_flag.sound_flag = 0;
//									NotificationBar();
							}
//								printf("AT+VIEW\r\n");
							else
							{
								bluetooth_locate = 2;
								control = 4;
							}
						break;
					case 2:          //开启蓝牙
							if(bluetooth_state != 0)      //蓝牙处连接状态时关闭蓝牙
							{   
								time_break = 1;								
								bluetooth_state = 1;
//								blue_break = 0;
								printf("AT");			
//								bluetooth_locate = 2;
//								control = 4;								
							}
							else                        //蓝牙不是连接状态时开启和关闭蓝牙
							{
								if(bluetooth_start==0)
									printf("AT+ADTY0");
								else
									printf("AT+ADTY3");
								at_id=1;
//							tab_flag.update_flag = 0;				//不需要更新
//							tab_flag.sound_flag = 0;
							}
						break;
					case 3:	
							bluetooth_locate = 3;				
							bluetooth_j = 3;
							interface_state = 0;
							tab_flag.Tab_Level = 0;					//选择主界面
							Tab[tab_flag.MainTab_choice].SubLocation[Tab[tab_flag.MainTab_choice].Location] = 0;			//子函数坐标清零
							//主函数坐标不必清零						
						break;
				}
			break;
		case 1:          //1：设备与搜索界面 
				switch(bluetooth_locate)
				{
						case 0:
						case 1:
						case 2:
						case 3:
						case 4:
						case 5:
						case 6:
						case 7:
						case 8:
								bluetooth_judge = 8-bluetooth_locate;             // 0:绑定设备device_bond[0]   1:绑定设备device_bond[1]  .....8:绑定设备device_bond[8]
								if(bluetooth_start == 0) 
								{
									bluetooth_locate = 0;
									bluetooth_j = 0;
									control = 2;
								}
								else
								{
									judge_dis = 0;
									bluetooth_locate = 3;   
									bluetooth_j = 3;
									control = 3;
								}
							break;
					case 9:
							if(bluetooth_start == 1)   //蓝牙开启时
							{
								at_id=2;
								bluetooth_role=1;
								printf("AT+ROLE1");
								time_dis = 1;
							}
							bluetooth_locate = 0;
							bluetooth_j = 0;
							control = 2;          //2：搜索结果界面    bluetooth_locate； 第一个设备
						break;
					case 10:	   //返回上一界面
							blue_1_state=0;
							bluetooth_locate = 3;				
							bluetooth_j = 3;
							control = 0;
							interface_state = 2;
							tab_flag.Tab_Level = 0;					//选择主界面
							Tab[tab_flag.MainTab_choice].SubLocation[Tab[tab_flag.MainTab_choice].Location] = 0;			//子函数坐标清零						
						break;
				}			
			break;
		case 2:          //2：搜索结果界面
				bluetooth_judge = bluetooth_locate;    // 0:操作搜索设备1    1:操作搜索设备2    .....     8：操作搜索设备9
				judge_dis = 1;
				bluetooth_locate = 3;   
				control = 3;          //3：设备操作界面    bluetooth_locate； 连接/断开
			break;
		case 3:        //3：设备操作界面
				switch(bluetooth_locate)
				{
					case 2:                    //移除设备操作
							if((judge_dis == 0) && (bluetooth_state == 0))         //移除绑定设备
							{
								strcpy(device_bond[0]," ");
								W25QXX_Write((u8*)&device_bond[0],16*1024*1024+500,sizeof(device_bond[0]));					
//								printf("AT+CLEAR%d\r\n",bluetooth_judge);
//								if((device_sum>bluetooth_judge+1))
//								{
//									for(i=bluetooth_judge;i<device_sum-1;i++)
//										strcpy(device_bond[i],device_bond[i+1]);
//									strcpy(device_bond[device_sum-1]," ");
//								}
								device_sum--;
								W25QXX_Write((u8*)&device_sum,16*1024*1024+400,sizeof(device_sum));														
							}
							if(judge_dis == 1)          //移除搜索设备
							{
								strcpy(device[bluetooth_judge]," ");
								if((dis_sum>bluetooth_judge+1))
								{
									for(i = bluetooth_judge;i < dis_sum-1;i++)
										strcpy(device[i],device[i+1]);
									strcpy(device[dis_sum-1]," ");
								}
								dis_sum--;
								judge_dis = 2;
							}
							control = 4;          //4：提示界面
						break;
					case 3:                 //连接、断开设备操作
							if(bluetooth_state == 0)                //蓝牙断开状态
							{
								if(judge_dis == 0)         //连接绑定设备
								{
									at_id=2;
									bluetooth_role=1;
									time_link = 1;									
									printf("AT+ROLE1");
									time_bondlink = 1;
								}
								else                     //连接搜索设备
								{
									time_link = 1;
									printf("AT+CONN%d",bluetooth_judge);
								}
								control=4;       //4：提示界面
							}
							else                                 //蓝牙连接状态
							{
								if(bluetooth_state == bluetooth_judge+2)      //操作连接的蓝牙设备，当操作其它蓝牙设备时，不进行断开处理
								{
									time_break = 1;
									bluetooth_state = 0;    //转为断开状态
//									blue_break = 0;
									printf("AT");
									bluetooth_wait = 1;
								}
								control = 4;       //4：提示界面
							}
						break;
				}			
			break;
	}
}
void blue_break_Init()       //断开蓝牙IO口配置函数
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	 //使能PB端口时钟
		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //GPIOD11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOD.14
	GPIO_SetBits(GPIOD,GPIO_Pin_11);						 //PD.11 输出高

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //GPIOD14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOD.14
	GPIO_ResetBits(GPIOD,GPIO_Pin_14);						 //PD.14 输出低
}
int diffrence_judge(int num)          //判断绑定设备有无重复函数
{
  int i;
  for(i = 0;i < num;i++)
  {
    if(strcmp(device_bond[i],device[bluetooth_judge]) == 0)
		{
			bluetooth_state = i+2;
      break;
		}
  }
  if(i == num)
	{
		bluetooth_state = num+2;
    return 1;               //无重复
	}
  else
		return 0;            //绑定设备已存在
}
void bluetooch_waiting()
{
		if((time_close != 0) && (time_close != 10))     //蓝牙转为从机后关闭蓝牙缓冲时间
	{
		time_close++;
	}
	else if(time_close == 10)
	{
		time_close = 0;
		printf("AT+ADTY3");
	}
	if((time_open != 0) && (time_open != 10))     //蓝牙转为主机后进行搜索缓冲时间
		time_open++;
	else if(time_open == 10)
	{
		time_open = 0;
		printf("AT+DISC?");
	}
	if((time_bondlink != 0) && (time_bondlink != 10))     //蓝牙转为主机后进行连接绑定设备缓冲时间
		time_bondlink++;
	else if(time_bondlink == 10)
	{
		time_bondlink = 0;
		printf("AT+CONNL",bluetooth_judge);
	}
	if((time_break > 0) && (time_break < 20))   //断开蓝牙后转为从机缓冲时间
	{
		time_break++;
		if((bluetooth_wait == 1) && ((time_break == 1) || (time_break == 11)))
		{
			LCD_Display_pic(22, 50, 130, 52, bluetooth_wait2);
			LCD_Display_pic(22, 50, 130, 52, bluetooth_wait1);
		}
		else if((bluetooth_wait == 1)&&((time_break == 6)||(time_break == 16)))
		{
			LCD_Display_pic(22, 50, 130, 52, bluetooth_wait2);
			LCD_Display_pic(22, 50, 130, 52, bluetooth_wait3);
		}
		if(time_break == 10)
			blue_break = 1;
	}
	else if(time_break == 20)
	{
		time_break = 0;
		at_id=2;
		bluetooth_role=0;
		printf("AT+ROLE0");
		if(bluetooth_wait == 1)
		{
			bluetooth_wait = 0;
			LCD_Display_pic(22, 57, 130, 38,bluetooth_break);
		}
	}
	if(time_dis != 0)                         //蓝牙搜索时缓冲时间及过渡界面
	{
		time_dis++;
		if((time_dis == 1) || (time_dis == 11) || (time_dis == 21) || (time_dis == 31)|| (time_dis == 41)|| (time_dis == 51))     //过渡界面
		{
			LCD_Display_pic(22, 50, 130, 52, bluetooth_wait2);
			LCD_Display_pic(22, 50, 130, 52, bluetooth_wait1);
		}
		else if((time_dis == 6) || (time_dis == 16) || (time_dis == 26) || (time_dis == 36)|| (time_dis == 46)|| (time_dis == 56))
		{
			LCD_Display_pic(22, 50, 130, 52, bluetooth_wait2);
			LCD_Display_pic(22, 50, 130, 52, bluetooth_wait3);
		}
		else if(time_dis == 61)          //缓冲时间到，显示搜索结果
		{
			time_dis = 0;
			LCD_Display_pic(22, 12, 130, 111, bluetooth_ins);
			switch(dis_sum)                                     //搜索设备显示  
			{
				case 9:
				case 8:
				case 7:
				case 6:
				case 5:			
						LCD_Display_pic(35, 107, 13, 12,bluetooth_icon2);
						LCD_ShowString(58,107,device[4],12,0);	
				case 4:			
						LCD_Display_pic(35, 89, 13, 12,bluetooth_icon2);
						LCD_ShowString(58,89,device[3],12,0);				
				case 3:			
						LCD_Display_pic(35, 71, 13, 12,bluetooth_icon2);
						LCD_ShowString(58,71,device[2],12,0);
				case 2:			
						LCD_Display_pic(35, 53, 13, 12,bluetooth_icon2);
						LCD_ShowString(58,53,device[1],12,0);
				case 1:			
						LCD_Display_pic(35, 35, 13, 12,bluetooth_icon2);
						LCD_ShowString(58,35,device[0],12,0);
						LCD_Invert(29, 34, 113, 14);
					break;
				default:
					break;
			}
		}
	}
	if(time_link != 0)            //连接设备等待的过渡界面             
	{
		time_link++;
		if((time_link == 1) || (time_link == 11) || (time_link == 21))
		{
			LCD_Display_pic(22, 50, 130, 52, bluetooth_wait2);
			LCD_Display_pic(22, 50, 130, 52, bluetooth_wait1);
		}
		else if((time_link == 6) || (time_link == 16) || (time_link == 26))
		{
			LCD_Display_pic(22, 50, 130, 52, bluetooth_wait2);
			LCD_Display_pic(22, 50, 130, 52, bluetooth_wait3);
		}
		else if((time_link == 31))          //超时，连接失败
		{
			LCD_Display_pic(22, 50, 130, 52, bluetooth_linklose);
			if(judge_dis == 0)
			{
				at_id=2;
				bluetooth_role=0;
				printf("AT+ROLE0");
			}
		}
	}
	if((time_clear > 0) && (time_clear < 60))          //主机连接响应后蓝牙处理绑定信息缓冲时间
		time_clear++;
	else if(time_clear == 60)
	{
		time_clear = 0;
//		printf("AT+CLEAR%d\r\n",device_sum);
	}
}

void bluetooth_init()
{
	status=0;
	printf("AT+IMME1");
	at_id=3;
}


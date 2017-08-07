/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/outputworkmode.c
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   Main program body according to ST source
  ******************************************************************************
**/

#include "outputworkmode.h"
#include "excutefunction.h"
#include "usart6.h"
#include "C_Protocol.h"
#include "input.h"
#include "adc.h"
#include "ui_control.h"

uint8_t Motor_OnForSecond(u8 DeviceID, uint8_t Ochannel, int Power, float Seconds, uint8_t BreakAtEnd, uint8_t Invert);
uint8_t Motor_OnForDegrees(u8 DeviceID, uint8_t Ochannel, int Power, float Degrees, uint8_t BreakAtEnd, uint8_t Invert);

const int NoLimitedTime = -1;				//不限时标志

unsigned int RunTime_100ms[8] ={0};	//电机运动计时器,0--3为中型电机计时，4--7为大型电机计时

uint8_t Motor_Invert[4] = {0};           //电机反转标志
uint8_t motor_state[4] = {0};
uint8_t motor_on_state[4] = {0};

void Output_Init(void)
{
	u8 i,Tmp;
	PWM_Init();
	ENC_Init();
	
	//拔开时清零
	for(i=0;i<4;i++)
	{
		for (Tmp = 0;Tmp < 13;Tmp++)
		{
			Channel_State[i+4][Tmp]=0;
		}
		Channel_State[i+4][2]=1;//模式初始化为1
	}
}

//在主进程中每1ms执行一次
void MotorRunTime_Check(void)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		if(RunTime_100ms[i]>0) 
			RunTime_100ms[i]--;
	}
}

//-----------------------------工作模式---------------------------------

//BreakAtEnd:0 Break, 1 Coast;
//OChannel:	5--8
//Power:		-100--100
//Seconds:	精确到0.1s,当Seconds = NoLimitedTime 时为不限时间运行
//Degrees:	-n度--n度
//Rotations:-n圈--n圈
//return:		返回1代表工作模式完成


uint8_t Motor_OnForSecond(u8 DeviceID, uint8_t Ochannel, int Power, float Seconds, uint8_t BreakAtEnd, uint8_t Invert)
{
//	static uint8_t state[4] = {0};
	int SpeedPid_Power;
	
	if(Invert == 1)   Power = -Power;
	
	switch(DeviceID)	//设置参数
	{
		case 0x0a:      //大型电机
			if(Power<10 && Power>0) Power=10;
			if(Power<0 && Power>-10) Power=-10;
			break;
		case 0x04:			//中型电机
			if(Power<25 && Power>0) Power=25;
			if(Power<0 && Power>-25) Power=-25;
			break;
	}
	
	switch(motor_state[Ochannel-5])
	{
		case 0:			//第一次进入，开始计时
			//不限时
			if(Seconds == NoLimitedTime)
				Motor_PowerSet(Ochannel,Power);
			else if(Seconds >= 0)
			{
				RunTime_100ms[Ochannel-1] = Seconds*10;
				motor_state[Ochannel-5] = 1;
			}
			break;
		case 1:			//计时状态
			if(!RunTime_100ms[Ochannel-1])					//时间到
			{
				if(BreakAtEnd)
					Motor_DirectionSet(Ochannel,0);			//卡紧电机
				else
					Motor_PowerSet(Ochannel,0);					//自由停止
				
				motor_state[Ochannel-5] = 0;
				return 1;
			}
			
			//pid控速
			if(Power == 0)
			{
				Motor_DirectionSet(Ochannel,0);			//卡紧电机
			}
			else
			{
				SpeedPid_Power = PID_AbsoluteMode2(DeviceID,Ochannel,Power,CurrentPower[Ochannel-5]);
				Motor_PowerSet(Ochannel,SpeedPid_Power);
			}
			break;
	}
		
	return 0;
}



uint8_t Motor_OnForDegrees(u8 DeviceID, uint8_t Ochannel, int Power, float Degrees, uint8_t BreakAtEnd, uint8_t Invert)//for 中型电机
{
//	static uint8_t state[4] = {0};
	static int Power_Set[4];
	int SpeedPid_Power = 0;
	
	float angle_err = 0;
	static float angle_err_last[4] = {0};
	float angle_err_err = 0;
	static float angle_err_inte[4] = {0};
//	float angle_err_thre = 0;

	static float p=0;
	static float d=0;
	static float i=0;
	static float angle_err_inte_max = 0;
		
	static int stopping_count[4] = {0};
	
	switch(motor_state[Ochannel-5])
	{
		case 0:				//第一次进入，对编码器脉冲复位
			SumPluse[Ochannel-5] = 0;
			Runtime_PID[Ochannel-5] = 6;
		
			angle_err_last[Ochannel-5] = 0;
			angle_err_inte[Ochannel-5] = 0;							
			stopping_count[Ochannel-5] = 0;		
			

			motor_state[Ochannel-5] = 1;
			
		case 1:				//计角度状态，需要确保电机正转时角度是增加的
			if(Invert == 1)  Degrees = -Degrees;
		
			if(Power == 0)
			{
				Motor_DirectionSet(Ochannel,0);			//卡紧电机
			
				angle_err = Degrees-SumPluse[Ochannel-5];
				if( (angle_err<=0 && Degrees>=0)||(angle_err>=0 && Degrees<=0) )
				{
						motor_state[Ochannel-5] = 0;								//终止 复位
						if(BreakAtEnd)
						{
							Motor_DirectionSet(Ochannel,0);			//卡紧电机
						}
						else
						{
							Motor_PowerSet(Ochannel,0);					  //自由停止
						}
						return 1;
				}
			}
			else
			{
																																			//Power与Degree负负得正，决定方向
					if(Power<0 && Degrees>0) 																//转换后，Power始终为正，Degree决定方向
					{
						Degrees=-Degrees;
						Power=-Power;
					} 
					if(Power<0 && Degrees<0)
					{
						Degrees=-Degrees;
						Power=-Power;		
					}
					

					switch(DeviceID)	//设置参数
					{
						case 0x0a:      //大型电机
							if((Power>0) && (Power<10)) Power=10;
							break;
						case 0x04:			//中型电机
							if((Power>0) && (Power<25)) Power=25;
							break;
					}

					if(Runtime_PID[Ochannel-5] >= 6)
					{
							if(BreakAtEnd)
							{
									switch(DeviceID)	//设置参数
									{
										case 0x0a:      //大型电机
											p=1;//电机修改12.12
											d=20;//电机修改12.12
											i=10;//电机修改12.12								  // i*angle_err_inte_max >= Power_min
											angle_err_inte_max = 1;
											break;
										case 0x04:			//中型电机
											p=0.3;//电机修改12.12
											d=3.5;//32-35电机修改12.12
											i=25;//电机修改12.12
											angle_err_inte_max = 1;
											break;
									}

									angle_err = Degrees-SumPluse[Ochannel-5];//电机修改12.12
									
		//							angle_err_thre = 1.2f * angle_err;//电机修改12.12
		//							if(angle_err_thre<0) angle_err_thre=-angle_err_thre;
		//							if(angle_err_thre>90) angle_err_thre=90;
		//							
		//							if(angle_err<=angle_err_thre && angle_err>=-angle_err_thre && Power>40)
		//							{
		//								Power = 40;
		//							}
									
									if( (angle_err<=1 && Degrees>=0)||(angle_err>=-1 && Degrees<=0) )//&& CurrentPower[Ochannel-5]==0//电机修改12.12
									{
										motor_state[Ochannel-5] = 0;								//终止 复位
										Motor_DirectionSet(Ochannel,0);			//卡紧电机

										return 1;                             
									}						
								
									angle_err_err = angle_err - angle_err_last[Ochannel-5];
									angle_err_last[Ochannel-5] = angle_err;
								
									angle_err_inte[Ochannel-5] = angle_err_inte[Ochannel-5] + angle_err;
								
									if(angle_err_inte[Ochannel-5]>angle_err_inte_max)  angle_err_inte[Ochannel-5]=angle_err_inte_max;
									if(angle_err_inte[Ochannel-5]<-angle_err_inte_max)  angle_err_inte[Ochannel-5]=-angle_err_inte_max;
								
									Power_Set[Ochannel-5] = p*angle_err + d*angle_err_err + i*angle_err_inte[Ochannel-5];            //+-
						
									if(Power_Set[Ochannel-5]>0 && Power_Set[Ochannel-5]>Power)  Power_Set[Ochannel-5]=Power;
									if(Power_Set[Ochannel-5]<0 && Power_Set[Ochannel-5]<-Power)  Power_Set[Ochannel-5]=-Power;
									SpeedPid_Power = PID_AbsoluteMode2(DeviceID,Ochannel,Power_Set[Ochannel-5],CurrentPower[Ochannel-5]);//电机修改12.12
									Motor_PowerSet(Ochannel,SpeedPid_Power);
									
									if((SpeedPid_Power != 0) && (CurrentPower[Ochannel-5] == 0))
									{
											stopping_count[Ochannel-5]++;
											if(stopping_count[Ochannel-5] > 100)
											{
													motor_state[Ochannel-5] = 0;								//终止 复位
													Motor_DirectionSet(Ochannel,0);			//卡紧电机
													
													angle_err_last[Ochannel-5] = 0;
													angle_err_inte[Ochannel-5] = 0;
													
													stopping_count[Ochannel-5] = 0;
													
													return 1;  											
											}
									}
									else
									{
											stopping_count[Ochannel-5] = 0;
									}
							}
							else	
							{
									angle_err = Degrees-SumPluse[Ochannel-5];//电机修改12.12
								
									if((angle_err<=1 && Degrees>=0) || (Degrees<=0&& angle_err>=-1))
									{
										motor_state[Ochannel-5] = 0;								//终止 复位
										Motor_PowerSet(Ochannel,0);					  //自由停止
										
										return 1;                             
									}
									
									if(Degrees>0)  Power_Set[Ochannel-5]=Power;
									if(Degrees<0)	 Power_Set[Ochannel-5]=-Power;
									
									SpeedPid_Power = PID_AbsoluteMode2(DeviceID,Ochannel,Power_Set[Ochannel-5],CurrentPower[Ochannel-5]);//电机修改12.12
									Motor_PowerSet(Ochannel,SpeedPid_Power);	
							}
								
							Runtime_PID[Ochannel-5] = 0;
					}
			}
			break;
	}
		
	return 0;
}


//==============中型电机工作模式==============
uint8_t Meduim_Motor_Off(uint8_t Ochannel, uint8_t BreakAtEnd)
{
	if(BreakAtEnd)
		Motor_DirectionSet(Ochannel,0);			//卡紧电机
	else
		Motor_PowerSet(Ochannel,0);					//自由停止
	
	motor_on_state[Ochannel-5] = 0;
	return 1;
}

uint8_t Meduim_Motor_On(uint8_t Ochannel, int Power, uint8_t Invert)
{
	int SpeedPid_Power;

	if(Invert == 1)   Power = -Power;	
	
	if(Power<25 && Power>0) Power=25;
	if(Power<0 && Power>-25) Power=-25;
  
	
	if(Power == 0)
	{
		Motor_DirectionSet(Ochannel,0);			//卡紧电机
	}
	else
	{	
		SpeedPid_Power = PID_AbsoluteMode2(0x04,Ochannel,Power,CurrentPower[Ochannel-5]);
		Motor_PowerSet(Ochannel,SpeedPid_Power);
	}
	motor_on_state[Ochannel-5] = 1;
	return 1;
}

//使用过程需要一直调用
uint8_t Meduim_Motor_OnForSecond(uint8_t Ochannel, int Power, float Seconds, uint8_t BreakAtEnd, uint8_t Invert)
{
	return Motor_OnForSecond(0x04, Ochannel, Power, Seconds, BreakAtEnd, Invert);
}

//Degrees决定方向，Power不决定方向
uint8_t Meduim_Motor_OnForDegrees(uint8_t Ochannel, int Power, float Degrees, uint8_t BreakAtEnd, uint8_t Invert)
{
	return Motor_OnForDegrees(0x04, Ochannel, Power, Degrees, BreakAtEnd, Invert);
}

//Rotations：精确到0.1
uint8_t Meduim_Motor_OnForRotations(uint8_t Ochannel, int Power, float Rotations, uint8_t BreakAtEnd, uint8_t Invert)
{
	return Meduim_Motor_OnForDegrees(Ochannel,Power,Rotations*360.0f,BreakAtEnd, Invert);
}

//==============大型电机工作模式==============
uint8_t Large_Motor_Off(uint8_t Ochannel, uint8_t BreakAtEnd)
{
	if(BreakAtEnd)
		Motor_DirectionSet(Ochannel,0);			//卡紧电机
	else
		Motor_PowerSet(Ochannel,0);					//自由停止
	
	motor_on_state[Ochannel-5] = 0;
	return 1;
}

uint8_t Large_Motor_On(uint8_t Ochannel, int Power, uint8_t Invert)
{
	int SpeedPid_Power;

	if(Invert == 1)   Power = -Power;	
	
	if(Power<10 && Power>0) Power=10;
	if(Power<0 && Power>-10) Power=-10;
	
	if(Power == 0)
	{
		Motor_DirectionSet(Ochannel,0);			//卡紧电机
	}
	else
	{	
		SpeedPid_Power = PID_AbsoluteMode2(0x0a,Ochannel,Power,CurrentPower[Ochannel-5]);
		Motor_PowerSet(Ochannel,SpeedPid_Power);
	}
	motor_on_state[Ochannel-5] = 1;
	return 1;
}

uint8_t Large_Motor_OnForSecond(uint8_t Ochannel, int Power, float Seconds, uint8_t BreakAtEnd, uint8_t Invert)
{
	return Motor_OnForSecond(0x0a, Ochannel, Power, Seconds, BreakAtEnd, Invert);//电机修改12.12 电机 0x0a
}

uint8_t Large_Motor_OnForDegrees(uint8_t Ochannel, int Power, float Degrees, uint8_t BreakAtEnd, uint8_t Invert)
{
	return Motor_OnForDegrees(0x0a, Ochannel, Power, Degrees, BreakAtEnd, Invert);//电机修改12.12 电机 0x0a
}

uint8_t Large_Motor_OnForRotations(uint8_t Ochannel, int Power, int Rotations, uint8_t BreakAtEnd, uint8_t Invert)
{
	return Large_Motor_OnForDegrees(Ochannel,Power,Rotations*360.0f,BreakAtEnd, Invert);
}


//==============驾驶控制模式==============
uint8_t Move_Steering_Off(uint8_t Ochannel1, uint8_t Ochannel2, uint8_t BreakAtEnd)
{
	if(BreakAtEnd)
	{
		Motor_DirectionSet(Ochannel1,0);			//卡紧电机
		Motor_DirectionSet(Ochannel2,0);			//卡紧电机
	}
	else
	{
		Motor_PowerSet(Ochannel1,0);					//自由停止
		Motor_PowerSet(Ochannel2,0);					//自由停止
	}
	motor_on_state[Ochannel1-5] = 0;
	motor_on_state[Ochannel2-5] = 0;
	return 1;
}

uint8_t Move_Steering_On(uint8_t Ochannel1, uint8_t Ochannel2,float Steering, float Power, uint8_t Invert)
{
	int Power1,Power2;
	int SpeedPid_Power1,SpeedPid_Power2;
	
	if(Steering>=0)
	{
		Power1 = Power;
		Power2 = Power - Steering*Power/50;
	}
	else
	{
		Power1 = Power + Steering*Power/50;;
		Power2 = Power;
	}

	if(Invert == 1)
	{
		Power1 = -Power1;
		Power2 = -Power2;
	}
	
	if(Power1 == 0)
	{
		Motor_DirectionSet(Ochannel1,0);			//卡紧电机
	}
	else
	{	
		SpeedPid_Power1 = PID_AbsoluteMode2(0x0a,Ochannel1,Power1,CurrentPower[Ochannel1-5]);
		Motor_PowerSet(Ochannel1,SpeedPid_Power1);
	}
	
	if(Power2 == 0)
	{
		Motor_DirectionSet(Ochannel2,0);			//卡紧电机
	}
	else
	{	
		SpeedPid_Power2 = PID_AbsoluteMode2(0x0a,Ochannel2,Power2,CurrentPower[Ochannel2-5]);
		Motor_PowerSet(Ochannel2,SpeedPid_Power2);
	}

	motor_on_state[Ochannel1-5] = 1;
	motor_on_state[Ochannel2-5] = 1;
	return 1;
}

uint8_t Move_Steering_OnForSecond(uint8_t Ochannel1, uint8_t Ochannel2, float Steering, float Power, float Seconds, uint8_t BreakAtEnd, uint8_t Invert)
{
	static uint8_t flag1=0,flag2=0;
	int Power1,Power2;
	
	if(Steering>=0)
	{
		Power1 = Power;
		Power2 = Power - Steering*Power/50;
	}
	else
	{
		Power1 = Power + Steering*Power/50;;
		Power2 = Power;
	}
	
	if(Power1 != 0)
	{
		if(!flag1)
			flag1 = Large_Motor_OnForSecond( Ochannel1, Power1, Seconds, BreakAtEnd, Invert);
	}else
	{
		flag1 = 1;
		Motor_DirectionSet(Ochannel1,0);
	}
	if(Power2 != 0)
	{
		if(!flag2)
			flag2 = Large_Motor_OnForSecond( Ochannel2, Power2,Seconds, BreakAtEnd, Invert);
	}else
	{
		flag2 = 1;
		Motor_DirectionSet(Ochannel2,0);
	}
	
	if(flag1&flag2)
	{
		flag1 = 0;
		flag2 = 0;
		return 1;
	}else
	{return 0;}
	
}

uint8_t Move_Steering_OnForDegrees(uint8_t Ochannel1, uint8_t Ochannel2, float Steering, float Power, float Degrees, uint8_t BreakAtEnd, uint8_t Invert)
{
	static uint8_t flag1=0,flag2=0;
	int Power1,Power2;
	
	if(Steering>=0)
	{
		Power1 = Power;
		Power2 = Power - Steering*Power/50;
	}
	else
	{
		Power1 = Power + Steering*Power/50;;
		Power2 = Power;
	}
	
	if(Power1 != 0)
	{
		if(!flag1)
			flag1 = Large_Motor_OnForDegrees( Ochannel1, Power1, Degrees, BreakAtEnd, Invert);
	}else
	{
		flag1 = 1;
		Motor_DirectionSet(Ochannel1,0);
	}
	if(Power2 != 0)
	{
		if(!flag2)
			flag2 = Large_Motor_OnForDegrees( Ochannel2, Power2,Degrees, BreakAtEnd, Invert);
	}else
	{
		flag2 = 1;
		Motor_DirectionSet(Ochannel2,0);
	}
	
	if(flag1&flag2)
	{
		flag1 = 0;
		flag2 = 0;
		return 1;
	}else
	{
		return 0;
	}

}

uint8_t Move_Steering_OnForRotations(uint8_t Ochannel1, uint8_t Ochannel2, float Steering, float Power, float Rotations, uint8_t BreakAtEnd, uint8_t Invert)
{
	static uint8_t flag1=0,flag2=0;
	int Power1,Power2;
	
	if(Steering>=0)
	{
		Power1 = Power;
		Power2 = Power - Steering*Power/50;
	}
	else
	{
		Power1 = Power + Steering*Power/50;;
		Power2 = Power;
	}
	
	if(Power1 != 0)
	{
		if(!flag1)
			flag1 = Large_Motor_OnForDegrees( Ochannel1, Power1, Rotations*360.0f, BreakAtEnd, Invert);
	}else
	{
		flag1 = 1;
		Motor_DirectionSet(Ochannel1,0);
	}
	if(Power2 != 0)
	{
		if(!flag2)
			flag2 = Large_Motor_OnForDegrees( Ochannel2, Power2,Rotations*360.0f, BreakAtEnd, Invert);
	}else
	{
		flag2 = 1;
		Motor_DirectionSet(Ochannel2,0);
	}
	
	if(flag1&flag2)
	{
		flag1 = 0;
		flag2 = 0;
		return 1;
	}else
	{
		return 0;
	}
	
}

//==============坦克控制模式==============
uint8_t Move_Tank_Off(uint8_t Ochannel1, uint8_t Ochannel2, uint8_t BreakAtEnd)
{
	if(BreakAtEnd)
	{
		Motor_DirectionSet(Ochannel1,0);			//卡紧电机
		Motor_DirectionSet(Ochannel2,0);			//卡紧电机
	}
	else
	{
		Motor_PowerSet(Ochannel1,0);					//自由停止
		Motor_PowerSet(Ochannel2,0);					//自由停止
	}
	motor_on_state[Ochannel1-5] = 0;
	motor_on_state[Ochannel2-5] = 0;
	return 1;
}

uint8_t Move_Tank_On(uint8_t Ochannel1, uint8_t Ochannel2, float Power1, float Power2, uint8_t Invert)
{
	int SpeedPid_Power1,SpeedPid_Power2;

	if(Invert == 1)
	{
		Power1 = -Power1;
		Power2 = -Power2;
	}
		
	if(Power1 == 0)
	{
		Motor_DirectionSet(Ochannel1,0);			//卡紧电机
	}
	else
	{	
		SpeedPid_Power1 = PID_AbsoluteMode2(0x0a,Ochannel1,Power1,CurrentPower[Ochannel1-5]);
		Motor_PowerSet(Ochannel1,SpeedPid_Power1);
	}
	
	if(Power2 == 0)
	{
		Motor_DirectionSet(Ochannel2,0);			//卡紧电机
	}
	else
	{	
		SpeedPid_Power2 = PID_AbsoluteMode2(0x0a,Ochannel2,Power2,CurrentPower[Ochannel2-5]);
		Motor_PowerSet(Ochannel2,SpeedPid_Power2);
	}
	motor_on_state[Ochannel1-5] = 1;
	motor_on_state[Ochannel2-5] = 1;
	return 1;
}

uint8_t Move_Tank_OnForSecond(uint8_t Ochannel1, uint8_t Ochannel2, float Power1, float Power2, float Seconds, uint8_t BreakAtEnd, uint8_t Invert)
{
	static uint8_t flag1=0,flag2=0;
	
	if(Power1 != 0)
	{
		if(!flag1)
			flag1 = Large_Motor_OnForSecond( Ochannel1, Power1, Seconds, BreakAtEnd, Invert);
	}else
	{
		flag1 = 1;
		Motor_DirectionSet(Ochannel1,0);
	}
	if(Power2 != 0)
	{
		if(!flag2)
			flag2 = Large_Motor_OnForSecond( Ochannel2, Power2,Seconds, BreakAtEnd, Invert);
	}else
	{
		flag2 = 1;
		Motor_DirectionSet(Ochannel2,0);
	}
	
	if(flag1&flag2)
	{
		flag1 = 0;
		flag2 = 0;
		return 1;
	}else
	{return 0;}	
}

uint8_t Move_Tank_OnForDegrees(uint8_t Ochannel1, uint8_t Ochannel2, float Power1, float Power2, float Degrees, uint8_t BreakAtEnd, uint8_t Invert)
{
	static uint8_t flag1=0,flag2=0;
	
	if(Power1 != 0)
	{
		if(!flag1)
			flag1 = Large_Motor_OnForDegrees( Ochannel1, Power1, Degrees, BreakAtEnd, Invert);
	}else
	{
		flag1 = 1;
		Motor_DirectionSet(Ochannel1,0);
	}
	if(Power2 != 0)
	{
		if(!flag2)
			flag2 = Large_Motor_OnForDegrees( Ochannel2, Power2,Degrees, BreakAtEnd, Invert);
	}else
	{
		flag2 = 1;
		Motor_DirectionSet(Ochannel2,0);
	}
	
	if(flag1&flag2)
	{
		flag1 = 0;
		flag2 = 0;
		return 1;
	}else
	{
		return 0;
	}
}

uint8_t Move_Tank_OnForRotations(uint8_t Ochannel1, uint8_t Ochannel2, float Power1, float Power2, float Rotations, uint8_t BreakAtEnd, uint8_t Invert)
{
	static uint8_t flag1=0,flag2=0;
	
	if(Power1 != 0)
	{
		if(!flag1)
			flag1 = Large_Motor_OnForDegrees( Ochannel1, Power1, Rotations*360.0f, BreakAtEnd, Invert);
	}else
	{
		flag1 = 1;
		Motor_DirectionSet(Ochannel1,0);
	}
	
	if(Power2 != 0)
	{
		if(!flag2)
			flag2 = Large_Motor_OnForDegrees( Ochannel2, Power2, Rotations*360.0f, BreakAtEnd, Invert);
	}else
	{
		flag2 = 1;
		Motor_DirectionSet(Ochannel2,0);
	}

	if(flag1&flag2)
	{
		flag1 = 0;
		flag2 = 0;
		return 1;
	}else
	{
		return 0;
	}
}

uint8_t Unregulated_Motor_On(uint8_t Ochannel, int Power, uint8_t Invert)
{
	if(Invert == 1)   Power = -Power;	

	Motor_PowerSet(Ochannel,Power);
	
	return 1;
}

//==============电机参数返回模式===================

//输出电机旋转角度
//输出范围：(-n,n),精确到1度
//电机正转一定角度后，反转电机到0度后才会显示负度数
float Motor_Degrees(uint8_t Ochannel)
{
	return (float)SumPluseForDisplay[Ochannel-5];//电机修改12.12
}

float Motor_Rotations(uint8_t Ochannel)
{
	return (float)SumPluseForDisplay[Ochannel-5]/360;//电机修改12.12
}

//进行100等分，CurrentPower[Ochannel-5]
//范围[-100,100]
float Motor_CurrentPower(uint8_t Ochannel)
{
	return (float)CurrentPower[Ochannel-5];//电机修改12.12
}

//对一个端口的以上三种参数Degrees、Rotations、CurrentPower复位，不影响运行
void Motor_Reset(uint8_t Ochannel)
{
	SumPluse[Ochannel-5] = 0;
	CurrentPower[Ochannel-5] = 0;
	SumPluseForDisplay[Ochannel-5] = 0;
}


//-----------------功能函数----------------------------

//-----------------功能函数----------------------------

//电机端口检测
void ChannelMotorCheck(u8 Channel_State[32][13])
{
	u8 i,Tmp;
	u16 MotorValue;
	
	for(i=0;i<4;i++)
	{
		MotorValue = 2*ReadADC1AverageValue(6+i)*2500/4096;
		
		if(Motor_CurrentPower(i+5) == 0)
		{
			//端口判断
			if((200 < MotorValue && MotorValue < 380)||(2400 < MotorValue && MotorValue < 3000))		//大型电机
			{
				Channel_State[i+4][0] = i+5;
				Channel_State[i+4][1] = LegoIdtoEstId(0x07);//0x07;//
			}else if((380 < MotorValue && MotorValue < 540)||(1500 < MotorValue && MotorValue < 2400))	//中型电机
			{
				Channel_State[i+4][0] = i+5;
				Channel_State[i+4][1] = LegoIdtoEstId(0x08);//0x08;//
			}else	if((540 < MotorValue && MotorValue < 1000))																				//无电机
			{
				//拔开时清零
				for (Tmp = 0;Tmp < 13;Tmp++)
				{
					Channel_State[i+4][Tmp]=0;
				}
				Channel_State[i+4][2]=1;//模式初始化为1
				
	//			//电机端口脉冲复位
	//			for(j=0;j<4;j++)
	//				SumPluseForDisplay[j] = 0;
			}
		}
		
		//电机缓冲数据加载
		if(Channel_State[i+4][1] != 0 && Channel_State[i+4][2] == 1)
		{
			FloatToByte4(Motor_Degrees(i+5),Channel_State[i+4], 0);
		}else if(Channel_State[i+4][1] != 0 && Channel_State[i+4][2] == 2)
		{
			FloatToByte4(Motor_Rotations(i+5),Channel_State[i+4], 0);
		}else if(Channel_State[i+4][1] != 0 && Channel_State[i+4][2] == 3)
		{
			FloatToByte4(Motor_CurrentPower(i+5),Channel_State[i+4], 0);
		}
		
		if(Motor_CurrentPower(i+5) == 0 && programflag == 0 && motorcontrol_flag==0) //程序结束后的静止状态不锁电机
		{
			Motor_PowerSet(5+i,0);
		}
	}
}


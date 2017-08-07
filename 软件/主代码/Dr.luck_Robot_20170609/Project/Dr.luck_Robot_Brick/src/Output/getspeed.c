/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/getspeed.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   Main program body according to ST source
  ******************************************************************************
**/

#include "getspeed.h"
#include "exti.h"

const int LOGO_LargeMotorPForC = 360;				//乐高大型电机一圈脉冲数
const int LOGO_MiddleMotorPForC = 360;
const int Time_Period = 32400-1;						//脉冲计数范围为0---32400	
const int ENC_CNT_Reset = 0;								//TIM复位时的CNT值

int CurrentPower[4] = {0,0,0,0};						//四个端口当前速度值所在数值
int SumPluse[4] = {0,0,0,0};								//电机旋转总脉冲，正加负减
int SumPluseForDisplay[4] = {0,0,0,0};		//电机旋转总脉冲，正加负减，用于显示当前转角及圈数。

unsigned int Runtime_PID[4] = {0,0,0,0};

//--------------------------配置部分----------------------------------------
//void ENC1_Init(void)
//{
//  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//  TIM_ICInitTypeDef TIM_ICInitStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;
////  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

//  /* Configure PC.06,07 as encoder input */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_Init(GPIOE, &GPIO_InitStructure);

//  GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9);
//  GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_TIM9);
//  
//  /* Enable the TIM3 Update Interrupt */
////  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
////  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
////  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
////  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
////  NVIC_Init(&NVIC_InitStructure);
//  
//  /* Timer configuration in Encoder mode */ 
//  //TIM_DeInit(TIM3);
//  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//  
//  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // No prescaling 
//  TIM_TimeBaseStructure.TIM_Period = Time_Period; 
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
//  TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);
//  
//  TIM_EncoderInterfaceConfig(TIM9, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//  TIM_ICStructInit(&TIM_ICInitStructure);
//  TIM_ICInitStructure.TIM_ICFilter = 0;
//  TIM_ICInit(TIM9, &TIM_ICInitStructure);
//  
//  // Clear all pending interrupts
//  TIM_ClearFlag(TIM9, TIM_FLAG_Update);
//  TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);
//  
//  TIM_Cmd(TIM9, ENABLE); 
//}

//void ENC2_Init(void)
//{
//  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
//  TIM_ICInitTypeDef TIM_ICInitStructure;  
//  GPIO_InitTypeDef GPIO_InitStructure;
////  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
//  
//  /* Configure PC.06,07 as encoder input */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_Init(GPIOE, &GPIO_InitStructure);

//  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);
//  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);
//  
//  /* Enable the TIM3 Update Interrupt */
////  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
////  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
////  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
////  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
////  NVIC_Init(&NVIC_InitStructure);
//  
//  /* Timer configuration in Encoder mode */ 
//  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//  
//  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // No prescaling 
//  TIM_TimeBaseStructure.TIM_Period = Time_Period; 
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
//  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
//	  
//	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//  TIM_ICStructInit(&TIM_ICInitStructure);
//  TIM_ICInitStructure.TIM_ICFilter = 0;
//  TIM_ICInit(TIM1, &TIM_ICInitStructure);

//  // Clear all pending interrupts
//  TIM_ClearFlag(TIM1, TIM_FLAG_Update);
//  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
//  
//  TIM_Cmd(TIM1, ENABLE); 
//}

//void ENC3_Init(void)
//{
//  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
//  TIM_ICInitTypeDef TIM_ICInitStructure;  
//  GPIO_InitTypeDef GPIO_InitStructure;
////  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//  
//  /* Configure PC.06,07 as encoder input */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);


//  GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);
//  GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3);
//  
//  /* Enable the TIM3 Update Interrupt */
////  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
////  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
////  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
////  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
////  NVIC_Init(&NVIC_InitStructure);
//  
//  /* Timer configuration in Encoder mode */ 
//  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//  
//  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // No prescaling 
//  TIM_TimeBaseStructure.TIM_Period = Time_Period; 
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
//  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//  
//  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//  TIM_ICStructInit(&TIM_ICInitStructure);
//  TIM_ICInitStructure.TIM_ICFilter = 0;
//  TIM_ICInit(TIM3, &TIM_ICInitStructure);
//  
//  // Clear all pending interrupts
//  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
//  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
//  
//  TIM_Cmd(TIM3, ENABLE); 
//}

//void ENC4_Init(void)
//{
//  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
//  TIM_ICInitTypeDef TIM_ICInitStructure;  
//  GPIO_InitTypeDef GPIO_InitStructure;
////  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//  
//  /* Configure PC.06,07 as encoder input */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);

//  GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);
//  GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);
//  
//  /* Enable the TIM3 Update Interrupt */
////  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
////  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
////  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
////  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
////  NVIC_Init(&NVIC_InitStructure);
//  
//  /* Timer configuration in Encoder mode */ 
//  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//  
//  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // No prescaling 
//  TIM_TimeBaseStructure.TIM_Period = Time_Period; 
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
//  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
//  
//  TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//  TIM_ICStructInit(&TIM_ICInitStructure);
//  TIM_ICInitStructure.TIM_ICFilter = 0;
//  TIM_ICInit(TIM8, &TIM_ICInitStructure);
//  
//  // Clear all pending interrupts
//  TIM_ClearFlag(TIM8, TIM_FLAG_Update);
//  TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
//  
//  TIM_Cmd(TIM8, ENABLE); 
//}

void ENC_Init(void)
{
	EXTIX_Init();
//	ENC1_Init();
//	ENC2_Init();
//	ENC3_Init();
//	ENC4_Init();
}


//------------------------功能函数----------------------------------

//各计数器复位
void ENC_Reset(uint8_t Ochannel)
{
	switch(Ochannel)
	{
		case 5:Pluse[0]=0;break;
		case 6:Pluse[1]=0;break;
		case 7:Pluse[2]=0;break;
		case 8:Pluse[3]=0;break;
//		case 5:TIM9->CNT = ENC_CNT_Reset;break;
//		case 6:TIM1->CNT = ENC_CNT_Reset;break;
//		case 7:TIM3->CNT = ENC_CNT_Reset;break;
//		case 8:TIM8->CNT = ENC_CNT_Reset;break;
	}
}

//获取各通道电机当前脉冲值
int ENC_GetCurrentPluse(uint8_t Ochannel)
{
	int CurrentPluse = 0;
	switch(Ochannel)
	{
		case 5:CurrentPluse = (int) Pluse[0];break;
		case 6:CurrentPluse = (int) Pluse[1];break;
		case 7:CurrentPluse = (int) Pluse[2];break;
		case 8:CurrentPluse = (int) Pluse[3];break;
//		case 5:CurrentPluse = (int)(s16)TIM9->CNT;break;
//		case 6:CurrentPluse = (int)(s16)TIM1->CNT;break;
//		case 7:CurrentPluse = (int)(s16)TIM3->CNT;break;
//		case 8:CurrentPluse = (int)(s16)TIM8->CNT;break;
	}
	return CurrentPluse;
}

void Get_Runtime_PID()
{
	Runtime_PID[0] ++;
	Runtime_PID[1] ++;
	Runtime_PID[2] ++;
	Runtime_PID[3] ++;
}

//------------------------测速进程------------------------------

//检测电机走过角度进程
//每1ms执行一次该进程
void ENC_SumAngleProcess(int SumPluse[4], int SumPluseForDisplay[4])
{
	int CurrentPluse[4],CurrentPower[4];
	u8 i;
	static int OldPluse[4] = {0},
						 FirstIn[4] = {1,1,1,1};
						 
	for(i=0;i<4;i++)
	{
			CurrentPluse[i] = ENC_GetCurrentPluse(i+5);
			if(FirstIn[i])
			{
				OldPluse[i] = CurrentPluse[i];
				CurrentPower[i] = 0;
				FirstIn[i] = 0;
			}
			else{
				CurrentPower[i] = CurrentPluse[i] - OldPluse[i];			//这不是真正的CurrentPower
				if(CurrentPower[i]>20000)			//数值减少方向，计时器复位导致
				{
					CurrentPower[i] = -(Time_Period-CurrentPluse[i]+OldPluse[i]);
				}
				else if(CurrentPower[i]<-20000)			//数值增大方向，计时器复位导致
				{
					CurrentPower[i] = Time_Period-OldPluse[i]+CurrentPluse[i];
				}
				OldPluse[i] = CurrentPluse[i];
				SumPluse[i] +=CurrentPower[i];
				SumPluseForDisplay[i] += CurrentPower[i];
			}
	}
	
	Get_Runtime_PID();//得到PID的计算时间 1ms
}

//检测电机当前速度进程
//100ms执行一次
void ENC_CurrentPowerProcess(int CurrentPower[4])
{
	int CurrentPluse[4];
	u8 i;
	static int OldPluse[4] = {0},
						 FirstIn[4] = {1,1,1,1};
						 
	for(i=0;i<4;i++)
	{
		CurrentPluse[i] = ENC_GetCurrentPluse(i+5);
		if(FirstIn[i])
		{
			OldPluse[i] = CurrentPluse[i];
			CurrentPower[i] = 0;
			FirstIn[i] = 0;
		}
		else{
			CurrentPower[i] = CurrentPluse[i] - OldPluse[i];			//这不是真正的CurrentPower
			if(CurrentPower[i]>20000)			//数值减少方向，计时器复位导致
			{
				CurrentPower[i] = -(Time_Period-CurrentPluse[i]+OldPluse[i]);
			}
			else if(CurrentPower[i]<-20000)			//数值增大方向，计时器复位导致
			{
				CurrentPower[i] = Time_Period-OldPluse[i]+CurrentPluse[i];
			}
			OldPluse[i] = CurrentPluse[i];
		}
	}
}



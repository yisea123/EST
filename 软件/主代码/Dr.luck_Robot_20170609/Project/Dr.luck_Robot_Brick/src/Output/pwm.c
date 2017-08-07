/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/pwm.c
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   Main program body according to ST source
  ******************************************************************************
**/


#include "pwm.h"

//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM4_PWM_Init(u32 arr,u32 psc)
{
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4); //GPIOA6复用为定时器3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 |GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); 
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器14
	
	//初始化TIM3 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
	
	//初始化TIM13 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
	
	//初始化TIM13 Channel3 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
	
	//初始化TIM13 Channel4 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
	
  TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIM3

}

void PWM_IO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG, ENABLE);
	
//MAIN0   G10   MAIN1    G11
//MBIN0   D4    MBIN1    D5
//MCIN0   G7    MCIN1    G8
//MDIN0   G6    MDIN1    G5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_11 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
		
	GPIO_ResetBits(GPIOD,GPIO_Pin_4 | GPIO_Pin_5);
	GPIO_ResetBits(GPIOG,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_11);

//INTA0  E0    DIRA  G15
//INTB0  G13   DIRB  G12
//INTC0  D7    DIRC  D3
//INTD0  D2    DIRD  D1

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_7 );
	GPIO_ResetBits(GPIOE,GPIO_Pin_0);
	GPIO_ResetBits(GPIOG,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15 );
}

void PWM_Init(void)
{
	TIM4_PWM_Init(100-1,84-1);
	TIM_SetCompare1(TIM4,0);	//修改比较值，修改占空比,0-100
	TIM_SetCompare2(TIM4,0);
	TIM_SetCompare3(TIM4,0);
	TIM_SetCompare4(TIM4,0);
	PWM_IO_Init();
}


//------------------------功能函数-----------------------------------

//端口电机输出方向设置
//OChannel为5--8
//MotorDirection==1为正，0为卡紧，-1为反
void Motor_DirectionSet(uint8_t OChannel, int MotorDirection)
{
	switch(OChannel)
	{
		case 5:
			if(MotorDirection==1)
			{
//				GPIO_SetBits(GPIOA,GPIO_Pin_2);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_3);
				PORT_A0 = 0;
				PORT_A1 = 1;
			}
			else if(MotorDirection==-1){
//				GPIO_SetBits(GPIOA,GPIO_Pin_3);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_2);
				PORT_A0 = 1;
				PORT_A1 = 0;
			}
			else if(MotorDirection==0){
//				GPIO_SetBits(GPIOA,GPIO_Pin_2);
//				GPIO_SetBits(GPIOA,GPIO_Pin_3);
				PORT_A0 = 1;
				PORT_A1 = 1;
				TIM_SetCompare4(TIM4,100);
			}
			break;
		case 6:
			if(MotorDirection==1)
			{
//				GPIO_SetBits(GPIOA,GPIO_Pin_8);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_9);
				PORT_B0 = 0;
				PORT_B1 = 1;
			}
			else if(MotorDirection==-1){
//				GPIO_SetBits(GPIOA,GPIO_Pin_9);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_8);
				PORT_B0 = 1;
				PORT_B1 = 0;
			}
			else if(MotorDirection==0){
//				GPIO_SetBits(GPIOA,GPIO_Pin_8);
//				GPIO_SetBits(GPIOA,GPIO_Pin_9);
				PORT_B0 = 1;
				PORT_B1 = 1;
				TIM_SetCompare3(TIM4,100);
			}
			break;
		case 7:
			if(MotorDirection==1)
			{
//				GPIO_SetBits(GPIOE,GPIO_Pin_12);
//				GPIO_ResetBits(GPIOE,GPIO_Pin_13);
				PORT_C0 = 0;
				PORT_C1 = 1;
			}
			else if(MotorDirection==-1){
//				GPIO_SetBits(GPIOE,GPIO_Pin_13);
//				GPIO_ResetBits(GPIOE,GPIO_Pin_12);
				PORT_C0 = 1;
				PORT_C1 = 0;
			}
			else if(MotorDirection==0){
//				GPIO_SetBits(GPIOE,GPIO_Pin_12);
//				GPIO_SetBits(GPIOE,GPIO_Pin_13);
				PORT_C0 = 1;
				PORT_C1 = 1;
				TIM_SetCompare2(TIM4,100);
			}
			break;
		case 8:
			if(MotorDirection==1)
			{
//				GPIO_SetBits(GPIOE,GPIO_Pin_14);
//				GPIO_ResetBits(GPIOE,GPIO_Pin_15);
				PORT_D0 = 0;
				PORT_D1 = 1;
			}
			else if(MotorDirection==-1){
//				GPIO_SetBits(GPIOE,GPIO_Pin_15);
//				GPIO_ResetBits(GPIOE,GPIO_Pin_14);
				PORT_D0 = 1;
				PORT_D1 = 0;
			}
			else if(MotorDirection==0){
//				GPIO_SetBits(GPIOE,GPIO_Pin_14);
//				GPIO_SetBits(GPIOE,GPIO_Pin_15);
				PORT_D0 = 1;
				PORT_D1 = 1;
				TIM_SetCompare1(TIM4,100);
			}
			break;
	}
}

//端口电机输出速度设置,不考虑卡紧
// MotorSpeed 0-100
void Motor_PowerSet(uint8_t OChannel, int MotorSpeed)
{
	if(MotorSpeed>=0)
	{
		Motor_DirectionSet(OChannel,1);
	}else{
		Motor_DirectionSet(OChannel,-1);
		MotorSpeed = -MotorSpeed;
	}
	if(MotorSpeed >= 100)
	{
		MotorSpeed = 100;
	}
	switch(OChannel)
	{
		case 5:TIM_SetCompare4(TIM4,MotorSpeed);break;
		case 6:TIM_SetCompare3(TIM4,MotorSpeed);break;
		case 7:TIM_SetCompare2(TIM4,MotorSpeed);break;
		case 8:TIM_SetCompare1(TIM4,MotorSpeed);break;
	}
}



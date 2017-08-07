/**
  ******************************************************************************
  * @file    EST Brick of Dr.luck/iic.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#include "main.h"
#include "iic.h"
#include "delay.h"

//初始化IIC

//GPIO_TypeDef* GPIO_DIGI0_PORT[4] = {GPIOF,GPIOF,GPIOG,GPIOE};
//const uint16_t GPIO_DIGI0_PIN[4] = {GPIO_Pin_9,GPIO_Pin_13,GPIO_Pin_1,GPIO_Pin_11};
//GPIO_TypeDef* GPIO_DIGI1_PORT[4] = {GPIOF,GPIOF,GPIOG,GPIOE};
//const uint16_t GPIO_DIGI1_PIN[4] = {GPIO_Pin_7,GPIO_Pin_11,GPIO_Pin_0,GPIO_Pin_12};

//void IIC_init_port(u8 Port)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);			//使能GPIOE时钟
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_DIGI0_PIN[Port];
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;							//普通输出模式
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;						//推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
////	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;							//上拉
//	GPIO_Init(GPIO_DIGI0_PORT[Port], &GPIO_InitStructure);										//初始化
//		
//	GPIO_InitStructure.GPIO_Pin = GPIO_DIGI1_PIN[Port];
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;							//普通输出模式
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;						//推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
////	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;							//上拉
//	GPIO_Init(GPIO_DIGI1_PORT[Port], &GPIO_InitStructure);										//初始化
//}
	

void IIC_Init(void)
{	//DIGIA0		DIGIA1		DIGIB0		DIGIB1		DIGIC0		DIGIC1		DIGID0		DIGID1
	//F9		 		F7				F13				F11				G1				G0				E11				E12
	
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);			//使能GPIOE时钟

	//5脚配置
	
  //初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;							//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;						//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;							//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);										//初始化
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;							//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;						//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;							//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);										//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;							//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;						//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;							//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);										//初始化

	//6脚配置

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;							//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;						//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;							//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);										//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;							//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;						//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;							//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);										//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;							//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;						//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;							//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);										//初始化

//	IIC_SCL_1	=	1;	IIC_SDA_1	=	1;
//	IIC_SCL_2	=	1;	IIC_SDA_2	=	1;
//	IIC_SCL_3	=	1;	IIC_SDA_3	=	1;
//	IIC_SCL_4	=	1;	IIC_SDA_4	=	1;

}


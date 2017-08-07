/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/usart6.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#include "usart6.h"
#include "stdio.h"

//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
 
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x)
{
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART6->SR&0X40)==0);//循环发送,直到发送完毕   
	USART6->DR = (u8) ch;      
	return ch;
}
#endif

#if EN_USART6_RX   //如果使能了接收
////串口1中断服务程序
////注意,读取USARTx->SR能避免莫名其妙的错误   	
//u8 USART6_RX_BUF[USART6_REC_LEN];     //接收缓冲,最大UART_REC_LEN个字节.
////接收状态
////bit15，	接收完成标志
////bit14，	接收到0x0d
////bit13~0，	接收到的有效字节数目
//u16 USART6_RX_STA=0;       //接收状态标记	

//初始化IO 串口1 
//bound:波特率
void usart6_init(u32 bound){
	
//	USART_InitTypeDef USART_InitStructure;	
//	GPIO_InitTypeDef GPIO_InitStructure;    
//	
//    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOG  , ENABLE);    
//    //PG14->TX  PG9->Rx
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOG, &GPIO_InitStructure);
//    
//    GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6);
//    GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6);
//	//---------------------------------------------------------------
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);    
//    USART_InitStructure.USART_BaudRate = bound;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;
//    USART_InitStructure.USART_Parity = USART_Parity_No;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//    
//    USART_Init(USART6,&USART_InitStructure);   
//    USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
//    
//    USART_Cmd(USART6,ENABLE);
	
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOG,ENABLE); //使能GPIOG时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART6时钟

	//初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;							//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;							//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);										//初始化

	GPIO_ResetBits(GPIOD,GPIO_Pin_8);  //引脚拉低， 
	
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOG9复用为USART6
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOG14复用为USART6
	
	//USART6端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //GPIOC12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化GPIOC12
	//USART6端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //GPIOD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化GPIOD2

   //USART6 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART6, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART6, ENABLE);  //使能串口1
	
	//USART_ClearFlag(USART6, USART_FLAG_TC);
	
#if EN_USART6_RX
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart6 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif

}

void USART6_Send_Byte(u8 Data)
{
	while(!USART_GetFlagStatus(USART6,USART_FLAG_TXE));
	//USART_GetFlagStatus：得到发送状态位
	//USART_FLAG_TXE:发送寄存器为空 1：为空；0：忙状态
	USART_SendData(USART6,Data);
}

int usart6_flag = -1;

void USART6_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res2;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USART_ClearITPendingBit(USART6,USART_IT_RXNE);	//清楚中断标志位
		Res2=USART_ReceiveData(USART6);			//接收字符存入数组
		USART6_Send_Byte(Res2);
		
		usart6_flag = Res2-'0';
  }
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
}
#endif	
 

/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/usart3.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#include "usart3.h"
#include "usart_sensor.h"

////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

UBYTE Uart4RecBuf[UART_RECBUF_SIZE];     //接收缓冲
u16 Uart4RecBufIn = 0 ;

//UBYTE USART3_TX_BUF[UART_TXBUF_SIZE];     //发送缓冲
//u16 USART3_TX_POINTER = 0 ;
//u16 USART3_TX_LEN = 0 ;

#if EN_USART3_RX   
//如果使能了接收
//串口3中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误  

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA=0;       //接收状态标记	

//初始化IO 串口3 
//bound:波特率
void usart3_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE,ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
 
	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为USART3
	
	//USART3端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB ,&GPIO_InitStructure); //初始化PB10，PB11
	
	//RXIND_EN  E9
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;							//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;							//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);										//初始化

	GPIO_ResetBits(GPIOE,GPIO_Pin_9);  //拉低使能

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7  | GPIO_Pin_8| GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;							//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;							//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);										//初始化

   //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口3
						
  USART_Cmd(USART3, ENABLE);  //使能串口3 
	
	//USART_ClearFlag(USART3, USART_FLAG_TC);
	
#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
  GPIO_SetBits(GPIOB,GPIO_Pin_10);  //引脚GPIOA6拉低， 
}

UBYTE USART3_Send_Byte(UBYTE Data)
{
  UBYTE   Result = 1;
   	while(!USART_GetFlagStatus(USART3,USART_FLAG_TXE));	  //USART_GetFlagStatus：得到发送状态位
														  //USART_FLAG_TXE:发送寄存器为空 1：为空；0：忙状态
	USART_SendData(USART3,Data);
	return(Result);
}

void usart3_change(u32 bound)
{
	//USART1 初始化设置
	USART_InitTypeDef USART_InitStructure;
	
	USART_Cmd(USART3, DISABLE);
	
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART3, ENABLE);  //使能串口1 
}

void USART3_IRQHandler(void)                	//串口1中断服务程序
{
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);	//清楚中断标志位
		Uart4RecBuf[Uart4RecBufIn] = USART_ReceiveData(USART3);			//接收字符存入数组
//		printf("%c", Uart1RecBuf[Uart1RecBufIn]);
		
		if (++Uart4RecBufIn >= UART_RECBUF_SIZE)
      {
        Uart4RecBufIn  =  0;
      }
		UsartBreakTimer[3]  =  0;
  }
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	

 




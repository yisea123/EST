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
#include "Bluetooth.h"
#include "C_Protocol.h"

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
void _sys_exit(int x) 
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
u8 USART6_RX_BUF[USART6_REC_LEN];     //接收缓冲,最大UART_REC_LEN个字节.
////接收状态
////bit15，	接收完成标志
////bit14，	接收到0x0d
////bit13~0，	接收到的有效字节数目
u16 USART6_RX_STA=0;       //接收状态标记	

//初始化IO 串口1 
//bound:波特率
void usart6_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOC、GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能UART5时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOC12复用为UART5
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOD2复用为UART5
	
	//UART5端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //GPIOC12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化GPIOC12
	//UART5端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //GPIOD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化GPIOD2

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART6, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART6, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(UART5, USART_FLAG_TC);
	
#if EN_USART6_RX
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断

	//Uart5 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}

void USART6_Send_Byte(u16 Data)
{
	while(!USART_GetFlagStatus(USART6,USART_FLAG_TXE));
	//USART_GetFlagStatus：得到发送状态位
	//USART_FLAG_TXE:发送寄存器为空 1：为空；0：忙状态
	USART_SendData(USART6,Data);
}

int usart6_flag = -1;
u8 status=0;
u8 DATA_LEN;
u8 dis_sign=0;
u8 dis_data_len=0;
void USART6_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res2,i=0,t,j=0;
	
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USART_ClearITPendingBit(USART6,USART_IT_RXNE);	//清楚中断标志位
		
//		Res2=USART_ReceiveData(USART6);			//接收字符存入数组
//		USART6_Send_Byte(Res2);
//		
//		usart6_flag = Res2-'0';
		
		Res2=USART_ReceiveData(USART6);			//接收字符存入数组
		

			if(!Receive_start&&!USB_Rx_Length)
			{
				if(Res2 == 0x68) 	//接收到开始符
				{
					Receive_start = 1;
					USB_TimePic = 3000; 								//接收时间计时
				}
				else
				{
					switch (status)
					{
						case 0:						
							if( Res2==0x4F)   //O
							{
								status++;
							}
							break;
						case 1:
							if( Res2==0x4B)  //K
								status++;
							else status = 0;
							break;
						case 2:
							if( Res2==0x2B)  //+
								status++;
							else status = 0;
							break;
						case 3:
							if(Res2==0x41)  //A
							{
								DATA_LEN = 16;
								status++;
							}
							else if(Res2==0x4C)  //L
							{
								DATA_LEN = 3;
								status++;
							}
							else if(Res2==0x53)  //S
							{
								DATA_LEN = 4;
								status++;
							}
							else if(Res2==0x4E)  //N
							{
								status++;
								status++;
							}
							else if(Res2==0x43)  //C
							{
								DATA_LEN = 3;
								status++;
							}
							else if(Res2==0x44)  //D
							{
								dis_sign=1;
								status++;
							}
							else status = 0;
							break;
						case 4:
							if(dis_sign!=0)
							{
								if(Res2==0x49)  //I
								dis_sign=2;
								else if((Res2==0x53)&&(dis_sign==2))  //S
								dis_sign=3;
								else if((Res2>=0x30)&&(Res2<=0x37)&&(dis_sign==3))  //0~7
								{
									USART6_RX_BUF[USART6_RX_STA]=Res2;
									USART6_RX_STA++;
									dis_sign=4;
								}
								else if(dis_sign==4)
								{
									USART6_RX_BUF[USART6_RX_STA]=Res2;
									USART6_RX_STA++;
									if(Res2==0X0D) 
									{ 
										status = 0; 
										dis_sign=0;
										if(memcmp(USART6_RX_BUF+17,"NAME:",strlen("NAME:"))==0)
										{
											dis_sum++;
											j=1;
											t=0;
											while(j)
											{
												device[dis_sum-1][t] = USART6_RX_BUF[t+22];
												t++;
												if(USART6_RX_BUF[t+22]==0x0d)
													j=0;
											}
											device[dis_sum-1][t] = '(';
											t++;
												for(j=0;j<4;j++)
												{
													device[dis_sum-1][t] = USART6_RX_BUF[j+10];
													t++;												
												}
											device[dis_sum-1][t] = ')';
											device[dis_sum-1][t+1]='\0';
										}		
										USART6_RX_STA = 0;   //清零
									}
								}
								else
								{
										status = 0; 
										dis_sign=0;
								}
							}
							else
							{
								USART6_RX_BUF[USART6_RX_STA]=Res2 ;
								USART6_RX_STA++;
								if(USART6_RX_STA==DATA_LEN) 
								{ 
									status = 0; 
									bluetooth_msg();
									USART6_RX_STA = 0;   //清零
								}
							}
							break;
						case 5:
							USART6_RX_BUF[USART6_RX_STA]=Res2 ;
							USART6_RX_STA++;
								if(Res2==0X0D) 
								{ 
									status = 0; 
									bluetooth_msg();
									USART6_RX_STA = 0;   //清零
								}
							break;					
					}
				}
			}
			if(Receive_start)
			{
				USB_Rx_Buff[USB_Rx_Length++] = Res2; 
				if(USB_Rx_Length-1>4)		//进入数据域
				{
					if((USB_Rx_Length>5+USB_Rx_Buff[3]+USB_Rx_Buff[4]*256))  				 //避开数据域数据等于结束符问题
					{
//						for(i=6+USB_Rx_Buff[3]+USB_Rx_Buff[4]*256; i<USB_Rx_Length; i++) //寻找结束符
//						{
							if(Res2 == 0x16) 		//接收到结束符
							{
								Receive_start = 0; 					 		//本帧数据接收结束，但无权接收下一帧数据
								Receive_end = 1;
//								USB_Rx_Length = i+1; 				 		//除去结束符后无关的数据
								
//								for(i=0;i<USB_Rx_Length;i++)
//									printf("%x ",USB_Rx_Buff[i]);
//								break;
							}
//						}		
					}		
				}
			}

  }
	
//	if(bluetooth_state==0||bluetooth_state==1)
//		bluetooth_msg();
//	else
//		Show_Str(10,10,150,100,USB_Rx_Buff,12,0);
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
}
#endif	
 
 int USART6_Receive(const char *str)//判断接收的字符串
{
	int i;
		if(memcmp(USART6_RX_BUF,str,strlen(str))==0)
		{
//			Show_Str(10,10,150,100,USART6_RX_BUF,12,0);
//				printf("bluetooth:");
//			for(i=0;i<strlen(str);i++)
//			{
//				printf("%x",USART6_RX_BUF[i]);
//			}
//				printf("\n\r");
			return 1;
		}
		else
		{
//			Show_Str(10,10,150,100,USART6_RX_BUF,12,0);
				return 0;
		}
		
}
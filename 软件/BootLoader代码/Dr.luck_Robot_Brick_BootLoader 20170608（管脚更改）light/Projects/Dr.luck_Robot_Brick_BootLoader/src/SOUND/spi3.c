/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/spi3.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief  
  ******************************************************************************
**/
#define SPI_GLOBALS

#include "sys.h"
#include "spi3.h"

#define	MP3SPI			SPI3
#define DMA_Stream0_IT_MASK     (uint32_t)(DMA_LISR_FEIF0 | DMA_LISR_DMEIF0 | \
                                           DMA_LISR_TEIF0 | DMA_LISR_HTIF0 | \
                                           DMA_LISR_TCIF0)
#define DMA_Stream1_IT_MASK     (uint32_t)(DMA_Stream0_IT_MASK << 6)
#define DMA_Stream5_IT_MASK     (uint32_t)(DMA_Stream1_IT_MASK | (uint32_t)0x20000000)

void SPI_Mp3_Init(void)
{	SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	/* PB15-MOSI2,PB13-SCK2*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* SPI2 configuration */
	SPI_Cmd(MP3SPI, DISABLE); 												//必须先禁能,才能改变MODE
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//两线全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//主
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//8位
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;								//CPOL=1 时钟悬空高
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;							//CPHA=1 数据捕获第2个
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//软件NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;		//2分频
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//高位在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC7    
	SPI_Init(MP3SPI, &SPI_InitStructure);
	
    SPI_Cmd(MP3SPI, ENABLE); 
}
/***********************************************************************
* 名称：main()
* 功能：主程序，用于GUI测试及演示。
***********************************************************************/
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
void SPI_Mp3_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	SPI_Cmd(MP3SPI, DISABLE);
	MP3SPI->CR1&=0XFFC7;
	MP3SPI->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(MP3SPI,ENABLE);
} 
void SPI_Mp3_SetDMA(u8 en)
{	SPI_Cmd(MP3SPI, DISABLE);
	if( en == ENABLE )
	{	MP3SPI->CR2 |= 0x0003;	}
	else
	{	MP3SPI->CR2 &= 0xfffc;	}
	SPI_Cmd(MP3SPI,ENABLE);
}
/***********************************************************************
* 名称：main()
* 功能：主程序，用于GUI测试及演示。
***********************************************************************/
u8 SPI_Mp3_Byte(u8 byte)
{
	/*等待发送寄存器空*/
	while((MP3SPI->SR & SPI_I2S_FLAG_TXE)==RESET);
    /*发送一个字节*/
	MP3SPI->DR = byte;
	/* 等待接收寄存器有效*/
	while((MP3SPI->SR & SPI_I2S_FLAG_RXNE)==RESET);
	return(MP3SPI->DR);
}
void SPI_MP3_ReceiveSendByte( u16 num )
{
// 	DMA1_Stream7->CCR &= ~( 1 << 0 ) ;         //关闭DMA通道3
// 	DMA1_Stream0->CCR &= ~( 1 << 0 ) ;         //关闭DMA通道2

// 	DMA1_Stream0->CNDTR = num ;         //传输数量设置为buffersize个
// 	DMA1_Stream7->CNDTR = num ;         //传输数量设置为buffersize个

// 	DMA1->IFCR = 0xF000 ;                         //清除通道4的标志位
// 	DMA1->IFCR = 0xF0000 ;                        //清除通道5的标志位

// 	MP3SPI->DR ;								//接送前读一次SPI1->DR，保证接收缓冲区为空
// 	while( ( MP3SPI->SR & 0x02 ) == 0 );	
// 	DMA1_Stream7->CCR |= 1 << 0 ;              //开启DMA通道3
// 	DMA1_Stream0->CCR |= 1 << 0 ;              //开启DMA通道2	
}
/***********************************************************************
* 名称：main()
* 功能：主程序，用于GUI测试及演示。
***********************************************************************/
void SPI_MP3_DMA_Configuration( void )
{   
// 	RCC->AHBENR |= 1<<0 ;                     //DMA1时钟使能
// 	/*------------------配置SPI2_RX_DMA通道Channel4---------------------*/
//     DMA1_Stream0->CCR &= ~( 1<<14 ) ;        //非存储器到存储器模式
// 	DMA1_Stream0->CCR |=    2<<12   ;        //通道优先级高
// 	DMA1_Stream0->CCR &= ~( 3<<10 ) ;        //存储器数据宽度8bit
// 	DMA1_Stream0->CCR &= ~( 3<<8  ) ;        //外设数据宽度8bit
// 	DMA1_Stream0->CCR |=    1<<7    ;        //存储器地址增量模式
// 	DMA1_Stream0->CCR &= ~( 1<<6  ) ;        //不执行外设地址增量模式
// 	DMA1_Stream0->CCR &= ~( 1<<5  ) ;        //执行循环操作
// 	DMA1_Stream0->CCR &= ~( 1<<4  ) ;        //从外设读

// 	DMA1_Stream0->CNDTR &= 0x0000   ;        //传输数量寄存器清零
// 	DMA1_Stream0->CNDTR = SPI_MP3_BUFNUM ;       //传输数量设置为buffersize个

// 	DMA1_Stream0->CPAR = (u32)(&(MP3SPI->DR));   //设置外设地址，注意PSIZE
// 	DMA1_Stream0->CMAR = (u32)SPI_Mp3_RxBuf ; //设置DMA存储器地址，注意MSIZE

// 	/*------------------配置SPI2_TX_DMA通道Channel5---------------------*/
// 	DMA1_Stream7->CCR &= ~( 1<<14 ) ;        //非存储器到存储器模式
// 	DMA1_Stream7->CCR |=    0<<12   ;        //通道优先级最低
// 	DMA1_Stream7->CCR &= ~( 3<<10 ) ;        //存储器数据宽度8bit
// 	DMA1_Stream7->CCR &= ~( 3<<8 )  ;        //外设数据宽度8bit
// 	DMA1_Stream7->CCR |=    1<<7    ;        //存储器地址增量模式
// 	DMA1_Stream7->CCR &= ~( 1<<6 )  ;        //不执行外设地址增量模式
// 	DMA1_Stream7->CCR &= ~( 1<<5 ) ;         //不执行循环操作
// 	DMA1_Stream7->CCR |=    1<<4    ;        //从存储器读

// 	DMA1_Stream7->CNDTR &= 0x0000   ;        //传输数量寄存器清零
// 	DMA1_Stream7->CNDTR = SPI_MP3_BUFNUM ;       //传输数量设置为buffersize个
// 	
// 	DMA1_Stream7->CPAR = (u32)(&(MP3SPI->DR)) ;      //设置外设地址，注意PSIZE
// 	DMA1_Stream7->CMAR = (u32)SPI_Mp3_TxBuf ; //设置DMA存储器地址，注意MSIZE				 
}

//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25Q64/NRF24L01						  
//SPI口初始化
//这里针是对SPI2的初始化

void SPI3_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOC, ENABLE );//PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI3,  ENABLE );//SPI2时钟使能 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;											//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;									//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC

 	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);  //PC10/11/12上拉
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_SPI3); 							//PB3复用为 SPI1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_SPI3); 							//PB4复用为 SPI1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_SPI3);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI3, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI3, ENABLE); //使能SPI外设
	
	SPI3_ReadWriteByte(0xff);//启动传输	
}   
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
  
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI3->CR1&=0XFFC7;
	SPI3->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SPI3,ENABLE);
} 

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI3_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI3, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI3); //返回通过SPIx最近接收的数据					    
}

void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 
		
	}else 
	{
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	}
  DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//等待DMA可配置 
	
  /* 配置 DMA Stream */
  DMA_InitStructure.DMA_Channel = chx;  //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
  DMA_InitStructure.DMA_BufferSize = ndtr;//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA_Streamx, &DMA_InitStructure);//初始化DMA Stream

} 

//开启一次DMA传输
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量  
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
	
	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
	
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //数据传输量  

	DMA1->HIFCR = DMA_Stream5_IT_MASK;
		
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA传输 
		
	while (DMA_GetCmdStatus(DMA_Streamx) != ENABLE){}
}	

void MYDMA_Disable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
	
	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
	
//	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //数据传输量  

	DMA1->HIFCR = DMA_Stream5_IT_MASK;
		
}

////SPI1初始化
//void SPI1_Init(void)
//{	    
// 	GPIO_InitTypeDef GPIO_InitStructure;
//  	SPI_InitTypeDef  SPI_InitStructure;
//	 
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);								//使能GPIOB时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);								//使能SPI1时钟

//	//GPIOF9,F10初始化设置
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;			//PB3~5复用功能输出	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;							//复用功能
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;						//100MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;							//上拉
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

// 	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5); //PB3.4.5上拉
//		
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); 							//PB3复用为 SPI1
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); 							//PB4复用为 SPI1
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);
//	
//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为16
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
//	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
//	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
// 
//	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
//	
//	SPI1_ReadWriteByte(0xff);//启动传输	
//}



//void SPI1_SetSpeed(u8 SpeedSet)
//{
//	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
//	SPI1->CR1&=0XFFC7; 
//	SPI1->CR1|=SpeedSet;
//	SPI_Cmd(SPI1,ENABLE); 
//}


////SPIx 读写一个字节
////TxData:要写入的字节
////返回值:读取到的字节
//u8 SPI1_ReadWriteByte(u8 TxData)
//{				   			 
//	u8 retry=0;				 	
//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
//	{
//		retry++;
//		if(retry>200)return 0;
//	}			  
//	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
//	retry=0;

//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
//	{
//		retry++;
//		if(retry>200)return 0;
//	}	  						    
//	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					    
//}






























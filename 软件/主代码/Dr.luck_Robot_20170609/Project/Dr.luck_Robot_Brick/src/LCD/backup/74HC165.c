#include "74HC165.h"

void Touch_74HC165_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE);			//使能GPIOB时钟

	//74HC165 Input
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//74HC165 Control
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                                                    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10; //load
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                                                    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	HC165_CK_1;
	HC165_nPL_1;
}

#define	HC165_DELAY_TIME		100		//大约3us
void HC165_Delay(u16 delay)
{
	while(delay--);
}

unsigned char readByteHC165(void)
{
	u8 i,i_data=0;
	HC165_nPL_1;					//Keycon=1;
	HC165_Delay(HC165_DELAY_TIME);
	HC165_nPL_0;					//Keycon=0;	/*锁存数据*/
	HC165_Delay(HC165_DELAY_TIME);				//延时
	HC165_nPL_1;					//Keycon=1;
	HC165_Delay(HC165_DELAY_TIME);			//延时
	for(i=0;i<8;i++)					
	{	HC165_CK_0;				//Keyclk=0;
		HC165_Delay(HC165_DELAY_TIME);		//延时
		i_data=i_data<<1;
		if(HC165_OUT!=0)			//读取数据
		{	i_data=i_data|0x01;	}
		HC165_Delay(HC165_DELAY_TIME);		//延时
		HC165_CK_1;				//Keyclk=1;
		HC165_Delay(HC165_DELAY_TIME);		//延时
	}
	return(i_data);
}




//unsigned char readByteHC165(void)
//{
//	unsigned char n,mTemp = 0;
//	
//	for(n=0;n<8;n++)
//	{
//		HC165_CK_0;
//		HC165_delay(HC165_DELAY_DEF);
//		HC165_CK_1;
//		HC165_delay(HC165_DELAY_DEF);
//	}
//	
//		HC165_CK_0;
//		HC165_delay(HC165_DELAY_DEF);

//		HC165_nPL_0;
//	
//		HC165_CK_1; 
//	
//		HC165_delay(HC165_DELAY_DEF);
//	
//	
//		HC165_CK_0;
//		HC165_delay(HC165_DELAY_DEF);
//	
//	
//		HC165_nPL_1;
//	
//	for(n=0;n<8;n++)
//	{
//		mTemp <<= 1;

//		HC165_CK_1;
//		HC165_delay(HC165_DELAY_DEF);
//		
//		mTemp |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);
//		
//		HC165_CK_0; 
//		HC165_delay(HC165_DELAY_DEF);

//	}

//	
//	return(mTemp);
//}

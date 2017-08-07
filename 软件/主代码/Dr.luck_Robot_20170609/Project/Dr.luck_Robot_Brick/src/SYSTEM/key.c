/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/key.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#include "key.h"	
#include "stdio.h"

void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOC,GPIOE,GPIOF时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;								//KEY0 KEY1 KEY4对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;													//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;										//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;													//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);																//初始化GPIOC
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;									//KEY3 KEY5 对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;													//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;										//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;													//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);																//初始化GPIOE
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1;									//KEY2 KEY4 对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;													//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;										//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;													//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);																//初始化GPIOF
}

u8 KEY_ShiBie(u8 in)
{
	if( in == 0x01 )//back
		return 6;
	else if(in == 0x20)//enter
		return 5;
	else if(in == 0x04)//up
		return 1;
	else if(in == 0x08)//down
		return 2;
	else if(in == 0x02)//left
		return 3;
	else if(in == 0x10)//right
		return 4;
	return(0);
}

u8 KEY_ShiBie_long(u8 in)
{
	if( in == 0x01 )//back
		return 18;
	else if(in == 0x20)//enter
		return 17;
	else if(in == 0x04)//up
		return 13;
	else if(in == 0x08)//down
		return 14;
	else if(in == 0x02)//left
		return 15;
	else if(in == 0x10)//right
		return 16;
	return(0);
}

u8 KEY_ShiBie_down(u8 in)
{
	if( in == 0x01 )//back
		return 12;
	else if(in == 0x20)//enter
		return 11;
	else if(in == 0x04)//up
		return 7;
	else if(in == 0x08)//down
		return 8;
	else if(in == 0x02)//left
		return 9;
	else if(in == 0x10)//right
		return 10;
	return(0);
}

#define  keyCnttime 50 //500ms,长按识别时间
u8 keyState = 0, keyCnt = 0;

u8 KEY_Scan( u8 mode )
{	u8 tmp = 0;
	static u8 tmp_last = 0;
	if( KEY1 == 0 )	{	tmp |= 0x02;	}
	else if( KEY0 == 0 )	{	tmp |= 0x01;	}
	else if( KEY3 == 0 )	{	tmp |= 0x08;	}
	else if( KEY2 == 0 )	{	tmp |= 0x04;	}
	else if( KEY5 == 0 )	{	tmp |= 0x20;	}
	else if( KEY4 == 0 )	{	tmp |= 0x10;	}
	//---------------------------
	switch(keyState)
	{
		case 0:	if(tmp != 0 )
						{	
			 				keyState = 1;	keyCnt = 0;
							tmp_last = 0;
						}
						break;
		case 1:	if(tmp != 0 )//确实按下
						{	
							keyState = 2;	keyCnt ++;
							tmp_last = tmp;
						}
						else
						{
							keyState = 0;
						}
						break;
		case 2: if(tmp != 0 )
						{
							keyCnt ++;
							if(keyCnt > keyCnttime)
							{
								keyState = 4;
								keyCnt = keyCnttime;
								tmp_last = tmp;
								return(KEY_ShiBie_long(tmp));
							}
						}
						else
						{
							keyState = 3;
						}
						break;
		case 3: if(tmp == 0)//确实松开
						{
							keyCnt = 0;
							keyState = 6;
							return(KEY_ShiBie_down(tmp_last));	
						}
						else
						{
							keyState = 2;
						}
						break;
		case 4: if(tmp != 0 )
						{
							//return(KEY_ShiBie_long(tmp));								
						}
						else
						{
							keyState = 5;
							//return(KEY_ShiBie_long(tmp_last));
						}
						break;
		case 5: if(tmp == 0)//确实松开
						{
							keyCnt = 0;
							keyState = 0;
							return(KEY_ShiBie_down(tmp_last));
						}
						else
						{
							keyState = 4;
							//return(KEY_ShiBie_long(tmp));
						}
						break;
		case 6: keyState = 0;
			      return(KEY_ShiBie(tmp_last));
						break;						
		default: break;     
	}
	return(0);
}
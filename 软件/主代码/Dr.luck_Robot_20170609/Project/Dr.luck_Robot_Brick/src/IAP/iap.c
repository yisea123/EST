/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/iap.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief  
  ******************************************************************************
**/
#include "iap.h"
#include "stdio.h"
#include "sys.h"

#define FLASH_IAP_ADDR		(uint32_t)0x0800C000  	//iap程序起始地址(存放在FLASH)
											//保留0X08000000~0X0800FFFF的空间为IAP使用

typedef  void (*iapfun)(void);				//定义一个函数类型的参数.
iapfun jump2iap;


////设置栈顶地址
////addr:栈顶地址
//__asm void MSR_MSP(u32 addr) 
//{
//    MSR MSP, r0 			//set Main Stack value
//    BX r14
//}


void iap_jump(u32 iapxaddr)
{
	if(((*(vu32*)iapxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.0x20000000是sram的起始地址,也是程序的栈顶地址
	{ 		
		jump2iap=(iapfun)*(vu32*)(iapxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)iapxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2iap();									//跳转到APP.
	}
}


void iap_Func(u32 Update_Data_Length)
{
	FLASH_Unlock();	
	//修改Update_Data_Length长度
	FLASH_EraseSector(FLASH_Sector_2, VoltageRange_3);
	Test_Write(Update_Data_Length_L16,Update_Data_Length%0x10000);
	Test_Write(Update_Data_Length_H16,Update_Data_Length/0x10000);
	//将APP_CONFIG_ADDR置空
	Test_Write(APP_CONFIG_ADDR,APP_CONFIG_CLEAR_VALUE) ;
	FLASH_Lock();	
	
//	printf("读取结束:%x\n\r",Update_Data_Length%0x10000);
//	printf("读取结束:%x\n\r",Update_Data_Length/0x10000);
//	printf("读取结束\n\r");
	
	NVIC_SystemReset();
//	iap_jump(FLASH_IAP_ADDR);//跳转到iap的复位向量地址
}


 












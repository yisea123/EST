#include "iap.h"		
#include "uc1638c.h"						
											
#define Update_Buff_Size 1024						//1K

//u16 iapbuf[1024] = {0}; //用于缓存数据的数组
//u16 receiveDataCur = 0;	//当前iapbuffer中已经填充的数据长度,一次填充满了之后写入flash并清零
//u32 addrCur = FLASH_APP1_ADDR;			//当前系统写入地址,每次写入之后地址增加2048


void App_Update_Check(void)
{
	static uint32_t Update_Count_Adderss  = UPDATE_ADDRESS;
	static uint32_t App_Count_Adderss  = APPLICATION_ADDRESS;
	static u16 Update_Buff[Update_Buff_Size] = {0x0000};
	u16 Buff_Num = 0;
	u16 Buff_Remain = 0;	
	u16 Update_Size;
	u32 Update_data_length;
	u16 i;
	
	//检查是否有程序更新固化标志
	if(STMFLASH_ReadHalfWord(Update_Data_Length_L16)!=0xffff || STMFLASH_ReadHalfWord(Update_Data_Length_H16)!=0xffff)
	{
		Update_data_length = STMFLASH_ReadHalfWord(Update_Data_Length_L16)+STMFLASH_ReadHalfWord(Update_Data_Length_H16)*0x10000;
		printf("正在更新...\n");	
		printf("长度：%x\n\r",STMFLASH_ReadHalfWord(Update_Data_Length_L16));	
		printf("长度：%x\n\r",STMFLASH_ReadHalfWord(Update_Data_Length_H16));	
		
		LCD_Display_pic(0, 0, 180, 128, UPDATE);
		LCD_refresh();
		
		FLASH_Unlock();			//解锁flash FLASH_If_GetSectorNumber
		FLASH_EraseSector(FLASH_If_GetSectorNumber(ADDR_FLASH_SECTOR_3),VoltageRange_3);
		FLASH_EraseSector(FLASH_If_GetSectorNumber(ADDR_FLASH_SECTOR_4),VoltageRange_3);
		FLASH_EraseSector(FLASH_If_GetSectorNumber(ADDR_FLASH_SECTOR_5),VoltageRange_3);
		FLASH_EraseSector(FLASH_If_GetSectorNumber(ADDR_FLASH_SECTOR_6),VoltageRange_3);
		FLASH_EraseSector(FLASH_If_GetSectorNumber(ADDR_FLASH_SECTOR_7),VoltageRange_3);
		
		Buff_Num = Update_data_length/Update_Buff_Size;
		Buff_Remain = Update_data_length%Update_Buff_Size;
		
		printf("Buff_Num:%x,Buff_Remain:%x\n",Buff_Num,Buff_Remain);
		
		for(i=0;i<=Buff_Num;i++)
		{
			if(Buff_Num != 0)
				Update_Size = Update_Buff_Size;
			else					//最后一段
				Update_Size = Buff_Remain;

			//获取Update区数据
			STMFLASH_Read(Update_Count_Adderss,Update_Buff,Update_Size);//读出整个扇区的内容
			Update_Count_Adderss += Update_Size;
			
			printf("获取Update区数据\n");
		
			//将数据写入App区，检查写入时是否需要分扇区
			STMFLASH_Write_NoCheck(App_Count_Adderss,Update_Buff,Update_Size);
			App_Count_Adderss += Update_Size;
			
			if(i == Buff_Num/10 || i == 2*Buff_Num/10 || i == 3*Buff_Num/10 || i == 4*Buff_Num/10 || i == 5*Buff_Num/10 ||
				 i == 6*Buff_Num/10 || i == 7*Buff_Num/10 || i == 8*Buff_Num/10 || i == 9*Buff_Num/10 || i == 10*Buff_Num/10 )
			{
				fupd_prog(114,70,16,Buff_Num,i);	
			}
			printf("Buff_Num:%x,Buff_Remain:%x\n",Buff_Num,i);
			printf("将数据写入App区\n");
		}
		FLASH_Lock();//上锁
		printf("完成写入\n");
	}
	else
	{
		printf("无需更新\n");
	}
	if(STMFLASH_ReadHalfWord(APP_CONFIG_ADDR)!=APP_CONFIG_SET_VALUE )
	{
		FLASH_Unlock();			//解锁flash
		//将Update_Data_Length清零
		FLASH_EraseSector(FLASH_Sector_2, VoltageRange_3);
		FLASH_Lock();//上锁
		
		Test_Write(Update_Data_Length_L16,0xFFFF) ;
		Test_Write(Update_Data_Length_H16,0xFFFF) ;
		//将APP_CONFIG_ADDR置位，程序会在主函数中跳转到App区
		Test_Write(APP_CONFIG_ADDR,APP_CONFIG_SET_VALUE) ;
	}
}

typedef  void (*iapfun)(void);				//定义一个函数类型的参数.
iapfun jump2app;

////设置栈顶地址
////addr:栈顶地址
//__asm void MSR_MSP(u32 addr) 
//{
//    MSR MSP, r0 			//set Main Stack value
//    BX r14
//}


//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(u32 appxaddr)
{
	printf("appxaddr:%x\n\r",appxaddr);
	printf("*appxaddr:%x\n\r",((*(vu32*)appxaddr)));
	printf("*appxaddr&0x2FFD0000:%x\n\r",((*(vu32*)appxaddr)&0x2FFD0000));
	if(((*(vu32*)appxaddr)&0x2FFD0000)==0x20000000)	//检查栈顶地址是否合法.0x20000000是sram的起始地址,也是程序的栈顶地址 //(0x2FFD0000)
	{
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									//跳转到APP.
	}
	else
	{	
//		printf("program in flash is error\r\n");
	}
}

//跳转到app区域运行
void iap_jump_app_s(void)
{
	iap_load_app(APPLICATION_ADDRESS);//跳转到app的复位向量地址
}




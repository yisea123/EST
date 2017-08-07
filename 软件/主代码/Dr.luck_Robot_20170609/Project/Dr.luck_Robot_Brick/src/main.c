/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/main.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   Main program body according to ST source
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

//系统信息
u8 factorytest=0;
const u8 version[7]={'0','1','.','0','3','B'};
const u8 System_ID[15] = {'0','0','1','6','5','3','4','5','0','2','7','5','6','6'};
const u8 System_Name[8] = {'D','r','.','L','u','c','k'};

//Flash保存信息
u8 sound_volume = 0;		       //0~100
u8 sleep_time		= 0;		//0~5
u8 language			= 0;		//0：汉字  1：英语
u8 light_volume = 0;

u8 format_flag=0;
u8 bluetooth_flag=0;

//usb define
__ALIGN_BEGIN USB_OTG_CORE_HANDLE           USB_OTG_Core_dev __ALIGN_END ;
__ALIGN_BEGIN USBH_HOST                     	  USB_Host __ALIGN_END ;
__ALIGN_BEGIN USB_OTG_CORE_HANDLE		  USB_OTG_dev __ALIGN_END ;
extern HID_Machine_TypeDef HID_Machine;	

//声明
void config(void);
void USBH_HID_Reconnect(void);
void USBH_Connect_Check(void);
void USBD_ConnectionCheck(USB_OTG_CORE_HANDLE  *pdev);
void delayms(int n);
void deal_daisy_Chain(u8 Port, u8 DeviceId, u8 mode,	u8 bufflength, u8 buff[13]);
uint8_t GetByte(uint32_t rambase);
float GetFloat(uint32_t rambase);
void LightControl(void);
void BriButton_Handle(u8 button);
void Sleephandle(void);
void SoundPlay(void);
void bluetoothAT(void);
	
//状态灯状态标志位
extern u8 BriLight_flag;

//BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
//BYTE WriteBuffer[] =              /* 写缓冲区*/
//"乐博士文件系统测试\r\n";
/**
* @brief  Main routine for HID mouse / keyboard class application
* @param  None
* @retval int
*/
int main(void)
{
	u8 i,tmp;

	config();																								//初始化外设
	
#ifdef	SYSTEM_DEBUG
	printf("%s","system started\n\r");
#endif
//	W25QXX_Read((u8*)&sound_volume,16*1024*1024,sizeof(sound_volume));
//	W25QXX_Read((u8*)&sleep_time,16*1024*1024+100,sizeof(sleep_time));
//	W25QXX_Read((u8*)&bluetooth_start,16*1024*1024+300,sizeof(bluetooth_start));
//	W25QXX_Read((u8*)&bluetooth_visual,16*1024*1024+200,sizeof(bluetooth_visual));
//	W25QXX_Read((u8*)&device_sum,16*1024*1024+400,sizeof(device_sum));
//	W25QXX_Read((u8*)&device_bond[0],16*1024*1024+500,sizeof(device_bond[0]));
//	W25QXX_Read((u8*)&device_bond[1],16*1024*1024+600,sizeof(device_bond[1]));
//	W25QXX_Read((u8*)&device_bond[2],16*1024*1024+700,sizeof(device_bond[2]));
//	W25QXX_Read((u8*)&device_bond[3],16*1024*1024+800,sizeof(device_bond[3]));
//	blue_break_Init();
  while(1)
  {
		USBH_Process(&USB_OTG_Core_dev,	&USB_Host);						// Host Task handler
		if( SysTick_1ms == 1 )//1ms
		{
			FrameOrFlashBuffLoseCheck();												//对数据包、结束符丢失检查			
			ENC_SumAngleProcess(SumPluse,SumPluseForDisplay);		//角度检测进程
			UartDevice1TimerInterrupt1();	
			SysTick_1ms = 0;
		}
		if( SysTick_2ms == 1 )//2ms
		{
			iicDevice1TimerInterrupt1();												//iic
			
//			printf("Bributton_value:%d\r\n",Bributton_value);
			
			SysTick_2ms = 0;
		}
		if( SysTick_10ms == 1 )//10ms
		{
			ChannelMotorCheck(Channel_State);										//电机端口检测、缓冲区赋值
			Device3TimerInterrupt1();														//DCM传感器类型检测
//			CheckOrder();																				//usb数据帧分析
			USBD_ConnectionCheck(&USB_OTG_dev);									//从端口连接状态检测
			tmp = KEY_Scan(0);                                  //按键检测
			if( tmp != 0 )
			{
				SystemCountTime = 0;
				ButtonAction = tmp;
//				printf("P:%d\r\n",tmp);
			}
			Bributton_value = tmp;
//			BriButton_Handle(tmp);

			SysTick_10ms = 0;
		}
		if( SysTick_50ms == 1 )//50ms
		{
			USBH_SendDataProcess(&USB_OTG_Core_dev, &USB_Host, reportBuff, &reportLen);		//hid host 连续发送数据进程
			
			SysTick_50ms = 0;
		}
		if( SysTick_100ms == 1 )//100ms
		{
			//系统时间、程序运行时间、8个程序定时器
			SystemTime++;
			SystemCountTime++;
			if(programflag == 1)
			{
				ProgramTime++;
				for(i=0; i<8; i++)
				{
					Timer[i].time++;
				}
			}
//			SleepTime++;
			SoundTime++;
			
			MotorRunTime_Check();
			
			//旧板软件开关机
//			if(power_flag>0) power_flag--;
//			else if(power_flag <= 0 && 2*ReadADC3AverageValue(0)*2500/4096 <= 1500)
//			{
//				GPIO_ResetBits(GPIOE, GPIO_Pin_2);
//			}
			
			ENC_CurrentPowerProcess(CurrentPower);							//电机测速进程
			USBH_Connect_Check();																//断开自动重连
			
//			SensorDataUpload_Process();												//从机传感器数据上传进程
//			UpdateCascadeNum_Process();												//级联编号更新进程		

			if(factorytest == 1)
				UI_Test(ButtonAction);
			else
			{
				UI_ButtonrRespond(&ButtonAction);										//屏幕UI响应更新
//				bluetooth_msg();
//				bluetooch_waiting();
			}
			
//			printf("touch:%d\r\n",Channel_State[0][3]);
			
			SysTick_100ms = 0;
		}
		if( SysTick_300ms == 1 )//300ms
		{
			LightControl();
//			IWDG_Feed();																				//喂狗
			
			SysTick_300ms = 0;
		}
		if( SysTick_1s == 1 )//1s
		{
			bluetoothAT();
//			PortDetect();																			//测试信息打印
			Sleephandle();
			if(ReadADC3AverageValue(0)*2500/4096 < 1450)
			{
				NVIC_SystemReset();
			}
//			for(i=0;i<USB_Rx_Length;i++)
//				printf("%x ",USB_Rx_Buff[i]);
			
			SysTick_1s = 0;
		}
		CheckOrder();																				//usb数据帧分析
		if(factorytest == 1)																	//工厂测试模式电机汇编函数运行
			Motor_Movement();

		if(usart6_flag>=0)                                     //测试用
		{
			ButtonAction = usart6_flag;													//串口控制
			
			filetest(usart6_flag);															//文件系统测试

			//级联测试
//			if(uart5_flag==0)					//重启usb
//			{
//				usbrestart = 1;
//			}
//			else if(uart5_flag==1)		//重启从机usb
//			{
//				HostSend_USBReset_Packet();
//				usbrestart = 1;
//			}
//			else if(uart5_flag==2)		//设备级联数量与level
//			{
////#ifdef	CASCADE_DEBUG
//				printf("CascadeNum:%d;CascadeLevel:%d\n",CascadeNum,CascadeLevel);
//				printf("HostComConnected:%d;SlaveComConnected:%d\n",HostComConnected,SlaveComConnected);
////#endif
//			}
//			else if(uart5_flag==3)		
//			{
//				USBH_HID_Reconnect();
//			}
//			else if(uart5_flag==4)		
//			{
//			}
//			else if(uart5_flag==5)		//设备控制指令
//			{
//				HostSend_DControl_Packet(1,6,0x80,0x03,buf,9);
//			}
//			else if(uart5_flag==6)		//传感器模式设置指令
//			{
//				HostSend_DControl_Packet(1,6,0x80,0x03,buf2,9);
////				HostSend_DDataGet_Packet(1,1,1,1);
//			}
//			else if(uart5_flag==7)		//设备控制反馈指令
//			{
//				SlaveReply_DControl_Packet(1,1,1,1);
//			}
//			else if(uart5_flag==8)		//传感器上传数据指令
//			{
//				SlaveReply_DDataGet_Packet(1,Channel_State);
//			}
//			else if(uart5_flag==9)		
//			{
//#ifdef	CASCADE_DEBUG
//				for(i=0;i<4;i++)
//				{
//					for(j=0;j<8;j++)
//					{
//						printf("0x%x ",Channel_State[i*8+j][1]);
//					}
//					printf("\n");
//				}
//#endif

//			}
				usart6_flag = -1;
		}
		
		if(programflag == 1)                            				//用户程序运行
		{
			SystemCountTime = 0;
			for(ThreadNow = 0; ThreadNow<ThreadSize; ThreadNow++)
			{
				if(Thread[ThreadNow].state != 0)
				{
//#ifdef INSTRUCT_DEBUG
//				printf("INSTRUCT-Thread-%d:0x%2x \r\n",ThreadNow,Flash_Buff[Thread[ThreadNow].Threadpc]);
////				printf("INSTRUCT-Thread %d:0x%x\r\n",ThreadNow,Thread[ThreadNow].state);
//#endif
					pc = Thread[ThreadNow].Threadpc;
					Byte_code_excute[Flash_Buff[pc]]();
					Thread[ThreadNow].Threadpc = pc;
				}
			}
		}else if(programflag == 0 && UIRUN_flag == 1)
		{UIRUN_flag = 0;}

		SoundPlay();

		if(Cascade_Function.allflag == 1)										//直接指令、或者级联执行
		{
			for(i=0; i<8; i++)
			{
				if(Cascade_Function.Cascade_Order[i].flag == 1)
				{
					deal_daisy_Chain(i+1,
															Cascade_Function.Cascade_Order[i].id,
																Cascade_Function.Cascade_Order[i].mode,
																	Cascade_Function.Cascade_Order[i].bufflength,
																		Cascade_Function.Cascade_Order[i].buff);			
#ifdef 	CASCADE_DEBUG
					printf("i %x  \r\n",i);
					printf("ID %x  \r\n",Cascade_Function.Cascade_Order[i].id);
					printf("MODE %x  \r\n",Cascade_Function.Cascade_Order[i].mode);
					printf("FLAG %x  \r\n",Cascade_Function.Cascade_Order[i].flag);
					printf("bufflength %x  \r\n",Cascade_Function.Cascade_Order[i].bufflength);
#endif

				}
			}
			if(Cascade_Function.Cascade_Order[0].flag == 0	&& Cascade_Function.Cascade_Order[1].flag == 0	&&
							Cascade_Function.Cascade_Order[2].flag == 0	&& Cascade_Function.Cascade_Order[3].flag == 0	&&
									Cascade_Function.Cascade_Order[4].flag == 0	&& Cascade_Function.Cascade_Order[5].flag == 0	&&
											Cascade_Function.Cascade_Order[6].flag == 0	&& Cascade_Function.Cascade_Order[7].flag == 0)
			{
				Cascade_Function.allflag = 0;
			}
		}
  }
}

void bluetoothAT(void)
{
	if(bluetooth_flag==0)
		{
			printf("AT+BAUD4");
			bluetooth_flag=1;
		}
		else if(bluetooth_flag==1)
		{
			printf("AT+RESET");
			bluetooth_flag=2;
			
		}
		else if(bluetooth_flag==2)
		{
			printf("AT+RESET");
			bluetooth_flag=3;
			
		}
		else if(bluetooth_flag==3)
		{
			printf("AT+ROLE0");
			bluetooth_flag=4;
			
		}
		else 	if(bluetooth_flag==4)
		{
			usart6_init(115200);
			bluetooth_init();
			bluetooth_flag=5;
		}
}

void BriButton_Handle(u8 button)
{
	switch(Bributton_state)
	{
		case 0:
			if( button <= 5)
			{
				Bributton_value = button;
			}else if(button >= 7 && button <= 11)
			{
				Bributton_value = button;
				Bributton_state = 1;
			}
			if(++Bributton_value >= 2)
			{
				Bributton_value = 0;
			}
			break;
		case 1:
			if(++Bributton_value >= 1)
			{
				Bributton_value = 0;
			}
			break;
	}
}

void Sleephandle(void)
{
	u8 sleep_value;
	switch(sleep_time)
	{
		case 0:sleep_value = 0;
			break;
		case 1:sleep_value = 2;
			break;
		case 2:sleep_value = 5;
			break;
		case 3:sleep_value = 10;
			break;
		case 4:sleep_value = 30;
			break;
		case 5:sleep_value = 60;
			break;
		default:break;
	}
	if(updateflash_start == 0 && SystemCountTime >= sleep_value*60*10 && sleep_value != 0)
	{
		NVIC_SystemReset();
	}
}

void delayms(int n)
{
	int i,j;
	for(i=0;i<n;i++)
		for(j=0;j<1000;j++);
}

void config(void){  

	SystemInit();	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0xC000);
	usart6_init(9600);
	
#ifdef	SYSTEM_DEBUG
	printf("%s","booting...\n\r");
#endif
	
	W25QXX_Init();																				//初始化W25Q128
	if(W25QXX_ReadID()!=W25Q256)													//检测不到W25Q128
	{
		printf("W25Q128 Check Failed!  ID:%x  \n\r",W25QXX_TYPE);
	}else
	{
		printf("W25Q128 completed! ID:%x  \n\r",W25QXX_TYPE);
	}

	if(font_init())//判断flash有无字库
  {
		printf("font Failed\n\r");
	}else
	{
		printf("font completed\n\r");
	}
	
	//在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
	res_flash = f_mount(&fs,"1:",1);
	
	//如果没有文件系统就格式化创建创建文件系统
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("formatting...\r\n");
    //格式化
		res_flash=f_mkfs("1:",0,0);							
		
		if(res_flash == FR_OK)
		{
			printf("formatting completed\r\n");
			res_flash = f_mount(NULL,"1:",1);			     				 //格式化后，先取消挂载
			res_flash = f_mount(&fs,"1:",1);      							//重新挂载
		}
		else
		{
			printf("formatting Failed!\r\n");
//			while(1);
		}
	}
  else if(res_flash!=FR_OK)
  {
    printf("FATFS Mount Failed! (%d)\r\n",res_flash);
//		while(1);
  }
  else
  {
    printf("FATFS mounted\r\n");
  }
	
//	STM_EVAL_LEDInit(LED1);
//	STM_EVAL_LEDOn(LED1);
	
	STM_EVAL_LEDInit(LED2);
	STM_EVAL_LEDOn(LED2);
	TIM3_Int_Init(10-1,8400-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数10次为1ms  
	
	//屏幕驱动初始化
	UI_Control_Init();																		
	LCD_clear();
	LCD_refresh();
	
#ifdef	SYSTEM_DEBUG
	printf("%s","UI completed\n\r");
#endif	
		
	//Init Host Library 
  USBH_Init(&USB_OTG_Core_dev, 
            USB_OTG_HS_CORE_ID,
            &USB_Host,
            &HID_cb, 
            &USR_Callbacks);
//	//Init VCP
//	USBD_Init(&USB_OTG_dev, 
//					USB_OTG_FS_CORE_ID, 
//					&USR_desc, 
//					&USBD_CDC_cb, 
//					&USR_cb);
	//init HID
	USBD_Init(&USB_OTG_dev,
						USB_OTG_FS_CORE_ID,
						&USR_desc, 
						&USBD_HID_cb, 
						&USR_cb);

#ifdef	SYSTEM_DEBUG
	printf("%s","usb completed\n\r");
#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SysTick_Config(SystemCoreClock / 20000);	//20000		
	
	//状态灯初始化
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDOff(LED4);
  STM_EVAL_LEDOff(LED3);
	
	//软件开关机复位脚
	POWER_Init();
	
	usart1_init(2400);																		//USART初始化
	usart2_init(2400);
	usart3_init(2400);
	uart4_init(2400);	
	Input_init();																					//输入端口相应管脚初始化
	IIC_Init();																						//IIC初始化	
	
#ifdef	SYSTEM_DEBUG
	printf("%s","input completed\n\r");
#endif

	Adc1_Init();																					//ADC初始化
	Adc3_Init();

#ifdef	SYSTEM_DEBUG
	printf("%s","adc completed\n\r");
#endif

	Battery_Init();																				//电池电量检测初始化
	
#ifdef	SYSTEM_DEBUG
	printf("%s","Battery completed\n\r");
#endif

	W25QXX_Read(&sound_volume, volume_ADDR,1);						//获取FLASH保存的系统信息
		if(sound_volume > 100 )
	{sound_volume = 100;W25QXX_Write(&sound_volume, volume_ADDR,1);}
	
	//音频播放初始化
	SPI_I2S_DeInit(SPI3);
	Mp3_GPIO_Init();//IO SPI
	MYDMA_Config(DMA1_Stream5,DMA_Channel_0,(u32)(&(SPI3->DR)),(u32)SPI_Mp3_TxBuf,0);
  Mp3Reset();
	VS_Set_Vol(sound_volume);
  SPI3_SetSpeed(SPI_BaudRatePrescaler_16);
	SPI_I2S_DMACmd(SPI3,SPI_I2S_DMAReq_Tx,ENABLE);
	
//	Mp3_GPIO_Init();
//	Mp3Reset();
//	MYDMA_Config(DMA1_Stream5,DMA_Channel_0,(u32)(&(SPI3->DR)),(u32)SPI_Mp3_TxBuf,0);
//	SPI_I2S_DMACmd(SPI3,SPI_I2S_DMAReq_Tx,ENABLE);
	XDCS_0();

#ifdef	SYSTEM_DEBUG
	printf("%s","sound completed\n\r");
#endif
//  
///*----------------------- 文件系统测试：写测试 -----------------------------*/
//	/* 打开文件，如果文件不存在则创建它 */
//	printf("\r\n****** 即将进行文件写入测试... ******\r\n");	
//	res_flash = f_open(&file1, "1:乐博士测试文件.dbf",FA_CREATE_ALWAYS | FA_WRITE );
//	if ( res_flash == FR_OK )
//	{
//		printf("》打开/创建乐博士测试文件.dbf文件成功，向文件写入数据。\r\n");
//    /* 将指定存储区内容写入到文件内 */
//		res_flash=f_write(&file1,WriteBuffer,sizeof(WriteBuffer),&fnum);
//    if(res_flash==FR_OK)
//    {
//      printf("》文件写入成功，写入字节数据：%d\n",fnum);
//      printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
//    }
//    else
//    {
//      printf("！！文件写入失败：(%d)\n",res_flash);
//    }    
//		/* 不再读写，关闭文件 */
//    f_close(&file1);
//	}
//	else
//	{	
//		printf("！！打开/创建文件失败。\r\n");
//	}
//	
///*------------------- 文件系统测试：读测试 ------------------------------------*/
//	printf("****** 即将进行文件读取测试... ******\r\n");
//	res_flash = f_open(&file1, "1:乐博士测试文件.dbf", FA_OPEN_EXISTING | FA_READ); 	 
//	if(res_flash == FR_OK)
//	{
//		printf("》打开文件成功。\r\n");
//		res_flash = f_read(&file1, ReadBuffer, sizeof(ReadBuffer), &fnum); 
//    if(res_flash==FR_OK)
//    {
//      printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
//      printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
//    }
//    else
//    {
//      printf("！！文件读取失败：(%d)\n",res_flash);
//    }		
//		/* 不再读写，关闭文件 */
//		f_close(&file1);	
//	}
//	else
//	{
//		printf("！！打开文件失败。\r\n");
//	}
//	/* 不再使用文件系统，取消挂载文件系统 */
//	f_mount(NULL,"1:",1);
	
	KEY_Init();																						//按键IO初始化
	
#ifdef	SYSTEM_DEBUG
	printf("%s","key completed\n\r");
#endif

	RNG_Init();																						//随机数产生初始化
	
#ifdef	SYSTEM_DEBUG
	printf("%s","RNG completed\n\r");
#endif

	EXTIX_Init();																					//电机外部中断检测编码盘数据管脚初始化
	Output_Init();																				//电机管脚初始化
	
#ifdef	SYSTEM_DEBUG
	printf("%s","output completed\n\r");
#endif
	
	W25QXX_Read(&light_volume, light_ADDR,1);						//获取FLASH保存的系统信息
	W25QXX_Read(&sleep_time, sleep_ADDR,1);
	W25QXX_Read(&language, language_ADDR,1);
		
	if(light_volume > 100 )
	{light_volume = 100;W25QXX_Write(&light_volume, light_ADDR,1);}
	if(sleep_time > 5 )
	{sleep_time = 5;W25QXX_Write(&sleep_time, sleep_ADDR,1);}
	if(language > 1)
	{language = 1;W25QXX_Write(&language, language_ADDR,1);}
	
#ifdef	SYSTEM_DEBUG
	printf("%s","Get sound-sleep value completed\n\r");
#endif

//	W25QXX_Read((u8*)&sound_volume,16*1024*1024,sizeof(sound_volume));
//	W25QXX_Read((u8*)&sleep_time,16*1024*1024+100,sizeof(sleep_time));
	W25QXX_Read((u8*)&bluetooth_start,16*1024*1024+300,sizeof(bluetooth_start));
	W25QXX_Read((u8*)&bluetooth_visual,16*1024*1024+200,sizeof(bluetooth_visual));
	W25QXX_Read((u8*)&device_sum,16*1024*1024+400,sizeof(device_sum));
	W25QXX_Read((u8*)&device_bond[0],16*1024*1024+500,sizeof(device_bond[0]));
	W25QXX_Read((u8*)&device_name,16*1024*1024+600,sizeof(device_name));
	blue_break_Init();
	
#ifdef	SYSTEM_DEBUG
	printf("%s","bluetooth completed\n\r");
#endif

	//看门狗初始化
//	IWDG_Init(4,2500); 																	//与分频数为64,重载值为500,溢出时间为1s	
	
#ifdef	SYSTEM_DEBUG
	printf("%s","IWDG completed\n\r");
#endif
}

void SoundPlay(void)
{
	u8 i;
		if(DREQ() != 0)																					//声音定时执行
		{
			if(DMA_GetCurrDataCounter(DMA1_Stream5)==0)
			{
				if(SOUND_Buff_flag == 0 && tab_flag.sound_flag == 1 && downloadsound_flag == 0) 												//按键声音
				{
					if( buttonsound_index < 940 )
					{
						for(i=0; i<SPI_MP3_BUFNUM; i++)
						{
							SPI_Mp3_TxBuf[i] = Buttonsound[i+buttonsound_index];
						}
						buttonsound_index += SPI_MP3_BUFNUM;
				
						MYDMA_Enable(DMA1_Stream5,SPI_MP3_BUFNUM);	
					}
					if( buttonsound_index >= 940 )//循环复位
					{
						buttonsound_index = 0;
						tab_flag.sound_flag = 0;
					}
				}
				
				if(SOUND_Buff_flag == 0 && tab_flag.sound_flag == 0 && downloadsound_flag == 1) 												//下载成功声音
				{
					if( downloadsound_index < 7200 )
					{
						for(i=0; i<SPI_MP3_BUFNUM; i++)
						{
							SPI_Mp3_TxBuf[i] = downloadsound[i+downloadsound_index];
						}
						downloadsound_index += SPI_MP3_BUFNUM;
				
						MYDMA_Enable(DMA1_Stream5,SPI_MP3_BUFNUM);	
					}
					if( downloadsound_index >= 7200 )//循环复位
					{
						downloadsound_index = 0;
						downloadsound_flag = 0;
					}
				}
				
				if(SOUND_Buff_flag == 1 && tab_flag.sound_flag == 0 && downloadsound_flag == 0)													//播放音乐
				{
					if( SOUND_Buff_index < file3.fsize)//&& SOUND_Buff_frame == 0)
					{
//							printf("SOUND_Buff_index:%x--%d\r\n",SOUND_Buff_index,file3.fsize);
							f_lseek (&file3, SOUND_Buff_index);
							f_read(&file3, SPI_Mp3_TxBuf,  SPI_MP3_BUFNUM, &fnum);
//						for(i=0; i<SPI_MP3_BUFNUM; i++)
//						{
//							SPI_Mp3_TxBuf[i] = SOUND_Buff[i+(SOUND_Buff_index-3*1024*SOUND_Buff_frame)];
//							
////							printf("0x%2x ",SPI_Mp3_TxBuf[i]);
//						}
						SOUND_Buff_index += SPI_MP3_BUFNUM;
//						printf("%4x \r\n",SOUND_Buff_index);
						MYDMA_Enable(DMA1_Stream5,SPI_MP3_BUFNUM);	
					}
					if( SOUND_Buff_index >= file3.fsize )
					{
						if(flush_buffer()==1)
						{
							SOUND_Buff_index += 32;
//							SOUND_Buff_index = 0;
//							SOUND_Buff_flag = 0;
//							SOUND_Buff_frame=0;
//							SOUND_update = 0;
							if(SOUND_UI == 1)
							{
								SOUND_UI=0;
								SOUND_Buff_index = 0;
								SOUND_Buff_flag = 0;
								f_close(&file3);
							}
						}
					}
				}
				if(piano_Buff_flag == 1 && tab_flag.sound_flag == 0 && downloadsound_flag == 0)													//播放钢琴音
				{
				if( piano_index < PIANO_LEN[piano_num-1] )
					{
						for(i=0; i<SPI_MP3_BUFNUM; i++)
						{
							SPI_Mp3_TxBuf[i] = piano_Buff[i+piano_index];
						}
						piano_index += SPI_MP3_BUFNUM;
				
						MYDMA_Enable(DMA1_Stream5,SPI_MP3_BUFNUM);	
					}
					if( piano_index >= PIANO_LEN[piano_num-1] )//循环复位
					{
						if(flush_buffer()==1)
						{
							piano_index += 32;
							if(SOUND_UI == 1)
							{
								SOUND_UI=0;
								piano_index = 0;
								piano_Buff_flag = 0;
							}
						}
					}
				}
			}
		}
}

void LightControl(void)
{
	if(BriLight_flag == 0 )																//蓝色
	{
		STM_EVAL_LEDOn(LED4);																	
		STM_EVAL_LEDOff(LED3);
	}
	if(BriLight_flag == 1 )																//蓝色闪烁  
	{
		STM_EVAL_LEDToggle(LED4);																	
		STM_EVAL_LEDOff(LED3);
	}
	if(BriLight_flag == 2 )																//红蓝色
	{
		STM_EVAL_LEDOn(LED4);
		STM_EVAL_LEDOn(LED3);
	}
	if(BriLight_flag == 3 )																//红蓝色闪烁
	{
		STM_EVAL_LEDToggle(LED4);
		STM_EVAL_LEDToggle(LED3);
	}
	if(BriLight_flag == 4 )																//红色
	{
		STM_EVAL_LEDOff(LED4);																
		STM_EVAL_LEDOn(LED3);
	}
	if(BriLight_flag == 5 )																//红色闪烁
	{
		STM_EVAL_LEDOff(LED4);																
		STM_EVAL_LEDToggle(LED3);
	}if(BriLight_flag == 6 )															//关闭
	{
		STM_EVAL_LEDOff(LED4);																
		STM_EVAL_LEDOff(LED3);
	}
}

//usb function
//HID重新连接
void USBH_HID_Reconnect(void)
{
	//关闭之前的连接
	USBH_DeInit(&USB_OTG_Core_dev,&USB_Host);															//复位USB HOST
//	USB_OTG_StopHost(&USB_OTG_Core_dev);																//停止USBhost
	if(USB_Host.usr_cb->DeviceDisconnected)																//存在,才禁止
	{
		USB_Host.usr_cb->DeviceDisconnected(); 															//关闭USB连接
		USBH_DeInit(&USB_OTG_Core_dev, &USB_Host);
		USB_Host.usr_cb->DeInit();
		USB_Host.class_cb->DeInit(&USB_OTG_Core_dev,&USB_Host.device_prop);
	}
	USB_OTG_DisableGlobalInt(&USB_OTG_Core_dev);													//关闭所有中断
	//重新复位USB
	RCC_AHB2PeriphClockCmd(RCC_AHB1Periph_OTG_HS,ENABLE);									//USB OTG HS 复位
//	delay_ms(5);
	RCC_AHB2PeriphClockCmd(RCC_AHB1Periph_OTG_HS,DISABLE);								//复位结束  

	memset(&USB_OTG_Core_dev,0,sizeof(USB_OTG_CORE_HANDLE));
	memset(&USB_Host,0,sizeof(USB_Host));
	//重新连接USB HID设备
	USBH_Init(&USB_OTG_Core_dev,USB_OTG_HS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks);  

}

//100ms执行一次
void USBH_Connect_Check(void)
{
	static u8 count = 0;
	
	if(bDeviceState==1)																										//连接建立了	bDeviceState在USBH_Process中置位
	{ 
				
	}else																																	//连接未建立的时候,检测
	{
		if(USBH_Check_EnumeDead(&USB_Host))																	//检测USB HOST 枚举是否死机了?死机了,则重新初始化 
		{ 	    
			printf("EnumeDead\n");
			USBH_HID_Reconnect();																							//重连
		}			
	}
	if(USBH_Check_HIDCommDead(&USB_OTG_Core_dev,&HID_Machine))						//检测USB HID通信,是否还正常? 
	{ 	    
		printf("HIDCommDead\n");
		USBH_HID_Reconnect();																								//重连
	}		
	
	count++;
	if(count>=10)
	{
		if((HostComInsert && HostComConnected==0))													//如果主机端usb线插入 但 未成功连接
		{	
			USBH_DeInit(&USB_OTG_Core_dev, &USB_Host);												//重初始化
		}
		count = 0;
	}
	
	if(usbrestart)
	{
		HostComConnected = 0;
		USBH_HID_Reconnect();
//		HostComInsert = 1;
		usbrestart = 0;
	}
}

//hid从设备定时检测与级联主机的连接状态函数
void USBD_ConnectionCheck(USB_OTG_CORE_HANDLE  *pdev)
{
	if(pdev->dev.device_status == USB_OTG_CONFIGURED)											//如果 host与slave连接成功
	{
		SlaveComConnected = 1;
	}
	else{
		SlaveComConnected = 0;
		if(programflag == 0)
		{
			BriLight_flag = 0;
		}
	}
}

//HID发送数据到pc函数
void HID_SendBuff(u8 *buff,u16 len)
{
	USBD_HID_SendReport (&USB_OTG_dev,buff,len);
}

//HID发送数据到级联主机host函数
//一次发送最大长度：63byte 
void HID_SendBuffToHost(u8 *buff,u8 len)
{
	u8 buf[64];
	int i;
	
	if(SlaveComConnected)
	{
		buf[0] = len;																					//首位代表数据长度
		for(i=0;i<len;i++)
		{
			buf[1+i] = *(buff+i);
		}
		USBD_HID_SendReport (&USB_OTG_dev,buf,len+1);
//		printf("slave端口发送数据\n");
	}
	else{
//		printf("本机没有上级设备\n");
	}
}

//级联主机host连续发送数据给hid从机函数
//一次发送最大长度：61byte
//u8 USBH_SendData(uint8_t DataBuff[], int DataLen);

//级联主机host端口连接状
//HostComConnected = 1; 代表连接到从机
//HostComConnected = 0; 代表未连接到从机

//级联从机slave端口连接状态
//SlaveComConnected = 1;代表被级联主机连接
//SlaveComConnected = 0;代表不被级联主机连接

//级联主机host数据接收
//uint8_t *BufferFromDevice;
//int BufferLengthFromDevice;

//级联从机slave数据接收
//u8 BufferFromHost[64];
//u8 BufferLengthFromHost;

#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
  number,ex: printf("Wrong parameters value: file %s on line %d\r\n", 
  file, line) */
  
  /* Infinite loop */
  while (1)
  {}
}

#endif

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/

/************************ (C) COPYRIGHT STU AI & Robot Lab *****END OF FILE****/

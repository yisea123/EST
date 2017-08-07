#include "main.h"
#include "stdio.h"
#include "vs10xx.h"
#include "spi3.h"
#include "timer3.h"
#include "spi.h"
#include "w25qxx.h"

u8 lcd_value=0;
u8 light_volume = 0;

#define light_ADDR  		1024*(1024*28+3205) 			//light

int main(void)
{
	u8 app_check_state = 1;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置系统中断
	SystemInit();
	usart6_init(9600);
	
	/* Initialize LEDs available on STM32F429I-DISCO */
  STM_EVAL_LEDInit(LED3);//red
  STM_EVAL_LEDInit(LED4);//blue
  /* Turn on LED3 */
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);
	
	//音频播放初始化
	Mp3_GPIO_Init();
	Mp3Reset();
	MYDMA_Config(DMA1_Stream5,DMA_Channel_0,(u32)(&(SPI3->DR)),(u32)SPI_Mp3_TxBuf,0);
	SPI_I2S_DMACmd(SPI3,SPI_I2S_DMAReq_Tx,ENABLE);
	XDCS_0();
	
	printf("进入IAP\n");
	WKUP_Init();				//待机唤醒初始化
	Sys_Enter_Stopby();
	
	TIM3_Int_Init(10-1,8400-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数10次为1ms  
	W25QXX_Init();																				//初始化W25Q128
	if(W25QXX_ReadID()!=W25Q256)													//检测不到W25Q128
	{
		printf("W25Q128 Check Failed!  ID:%x  \n\r",W25QXX_TYPE);
	}else
	{
		printf("W25Q128 completed! ID:%x  \n\r",W25QXX_TYPE);
	}
	printf("light_volume:%x  \n\r",light_volume);
	W25QXX_Read(&light_volume, light_ADDR,1);						//获取FLASH保存的系统信息
//	if(light_volume > 100 )
//	{light_volume = 100;}
	printf("light_volume:%x  \n\r",light_volume);
	
	delay_init(180);
	
	STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDOn(LED2);
	
	LCD_UC1638C_Init();
  LCD_Display_pic(0, 0, 180, 128, LOGO);
	LCD_refresh();
	delay_ms(1000);
	
	LCD_Display_pic(0, 0, 180, 128, STARTUP);
	LCD_refresh();
	delay_ms(1000);
	
  while(1)
  {
		if(app_check_state)
		{
			printf("检查更新\n");	
			App_Update_Check();
			
			app_check_state = 0;
		}

		//如果通过设置app固化配置，APP_CONFIG_ADDR（0X08001FFC）永久置为0x5555
		if(STMFLASH_ReadHalfWord(APP_CONFIG_ADDR) == APP_CONFIG_SET_VALUE)			
		{
//			if(STMFLASH_ReadHalfWord(0X08080000)!=0xffff )
//			{
//				FLASH_Unlock();			//解锁flash FLASH_If_GetSectorNumber
//				FLASH_EraseSector(FLASH_If_GetSectorNumber(ADDR_FLASH_SECTOR_8),VoltageRange_3);
//				FLASH_EraseSector(FLASH_If_GetSectorNumber(ADDR_FLASH_SECTOR_9),VoltageRange_3);
//				FLASH_EraseSector(FLASH_If_GetSectorNumber(ADDR_FLASH_SECTOR_10),VoltageRange_3);
//				FLASH_EraseSector(FLASH_If_GetSectorNumber(ADDR_FLASH_SECTOR_11),VoltageRange_3);
//				FLASH_Lock();//上锁
//			}
			
			printf("准备跳出IAP\n\r");
			app_check_state = 1;
			//直接跳转到APP
			TIM_Cmd(TIM3,DISABLE); //使能定时器3
			iap_jump_app_s();
			printf("跳出IAP失败\n");
			
			//若跳转出错则可默认更新
			FLASH_Unlock();			//解锁flash
//			FLASH_EraseSector(FLASH_Sector_1, VoltageRange_3);
			Test_Write(APP_CONFIG_ADDR,APP_CONFIG_NONE_VALUE);
			FLASH_Lock();			//解锁flash
			delay_ms(10000);			

		}
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/**
  ******************************************************************************
  * @file    SysTick_Example/main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This example shows how to configure the SysTick to generate a time 
  *          base equal to 1 ms. the SysTick is clocked by the AHB clock(HCLK).
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "uc1638c.h"
#include "fontupd.h"
#include "w25qxx.h"
#include "usart6.h"
#include "spi.h"
#include "text.h"	
#include "piano.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
static __IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);   

int main(void)
{
  u8 i=0,j=0;
  SystemInit();	
	LCD_UC1638C_Init();
  usart6_init(115200);		//初始化串口波特率为115200 
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
	W25QXX_Init();				//初始化W25Q64
  STM_EVAL_LEDOn(LED3);
	
	printf("System begain!\r\n");
	
//	W25QXX_Erase_Chip();
//	
	while(font_init())//判断flash有无字库
  {
		update_font(10,30,12);//更新字库
	}
//	LCD_clear();
	
	W25QXX_Write((u8*)&PIANO_DATA_40,PIANO_adr(3328+7*i),PIANO_LEN[j]);
	W25QXX_Write((u8*)&PIANO_DATA_41,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_42,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_43,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_44,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_45,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_46,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_47,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_48,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_49,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_50,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_51,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_52,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_53,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_54,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_55,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_56,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_57,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_58,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_59,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_60,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_61,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_62,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_63,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_64,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_65,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_66,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_67,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_68,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_69,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_70,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_71,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_72,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_73,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_74,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_75,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	W25QXX_Write((u8*)&PIANO_DATA_76,PIANO_adr(3328+7*(++i)),PIANO_LEN[++j]);
	
	Show_Str(0,20,180,12,"汕头大学人工智能与机器人实验室",12,1);	
	Show_Str(0,36,180,16,"汕头大学人工智能与机器",16,1);
	Show_Str(0,56,180,16,"人实验室",16,1);
	Show_Str(0,76,180,24,"汕头大学人工智",24,1);
	Show_Str(0,100,180,24,"能与机器人实验室",24,1);

	LCD_refresh();
	
  while(1)
  {

    STM_EVAL_LEDToggle(LED4);

    Delay(5000);

    STM_EVAL_LEDToggle(LED3);

    Delay(10000);
	
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */


/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0)
  { 
    TimingDelay--;
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

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

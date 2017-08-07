/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/stm32fxxx_it.c
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   This file includes the interrupt handlers for the application
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "stm32fxxx_it.h"
#include "usb_core.h"
#include "usbh_core.h"
#include "outputworkmode.h"
#include "usb_hcd_int.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//-------------------usb define----------
#define CURSOR_STEP     10
extern USB_OTG_CORE_HANDLE          USB_OTG_Core_dev;
extern USBH_HOST                    USB_Host;
extern USB_OTG_CORE_HANDLE           USB_OTG_dev;
extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
__IO uint32_t remote_wakeup =0;
extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
extern void USB_OTG_BSP_TimerIRQ (void);
//-----------------------------------------

/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*             Cortex-M Processor Exceptions Handlers                         */
/******************************************************************************/
/**
  * @brief  NMI_Handler
  *         This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}


/**
  * @brief  HardFault_Handler
  *         This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  MemManage_Handler
  *         This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  BusFault_Handler
  *         This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  UsageFault_Handler
  *         This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  SVC_Handler
  *         This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  DebugMon_Handler
  *         This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}


/**
  * @brief  PendSV_Handler
  *         This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

//hid引用
/**
* @brief  This function handles SysTick Handler.
* @param  None
* @retval None
*/

/*****系统时间*****/
uint32_t SystemTime = 0, ProgramTime = 0, SystemCountTime = 0, SoundTime = 0;

uint32_t GetSystemTimeMs(void)
{
	return SystemTime;
}

uint32_t GetProgramTimeMs(void)
{
	return ProgramTime;
}

/*****系统滴答时钟中断*****/

unsigned int SysTick_50us		= 0;													//时间片时基50us
unsigned char SysTick_1ms		= 0;													//时间片时基1ms

/* Time Define ---------------------------------------------------------------*/
unsigned char SysTick_2ms 				= 0;										//时间片时基2ms
unsigned char SysTick_10ms 				= 0;										//时间片时基10ms
unsigned char SysTick_50ms 				= 0;										//时间片时基50ms
unsigned char SysTick_100ms 			= 0;										//时间片时基100ms
unsigned char SysTick_300ms 			= 0;										//时间片时基300ms
unsigned char SysTick_1s 					= 0;										//时间片时基1s

unsigned char SysTick_count_2ms		= 0;										//时间片时基2ms
unsigned char SysTick_count_10ms	= 0;										//时间片时基10ms
unsigned char SysTick_count_50ms	= 0;										//时间片时基50ms
unsigned char SysTick_count_100ms	= 0;										//时间片时基100ms
uint16_t			SysTick_count_300ms	= 0;										//时间片时基300ms
uint16_t 			SysTick_count_1s 		= 0;										//时间片时基1s

u8 lcd_value=0;

void SysTick_Handler(void)
{
//	STM_EVAL_LEDToggle(LED2);
	SysTick_50us++;

	if(	SysTick_50us == 20)
	{
		SysTick_50us		= 0;
		SysTick_1ms			= 1; 
		
		SysTick_count_2ms++;
		SysTick_count_10ms++;
		SysTick_count_50ms++;
		SysTick_count_100ms++;
		SysTick_count_300ms++;
		SysTick_count_1s++;
					
//		if(lcd_value >= light_volume/10 && lcd_value <= 10)
//			STM_EVAL_LEDOff(LED2);
//		else if( lcd_value >= 0 && lcd_value < light_volume/10)
//			STM_EVAL_LEDOn(LED2);
//		
//		if(lcd_value == 10)
//			lcd_value = 0;
//		else
//			lcd_value++;
	}

	if( SysTick_count_2ms == 2 )														//2ms
		{ 
			SysTick_count_2ms = 0;SysTick_2ms = 1;
		}
		
	if( SysTick_count_10ms == 10 )													//10ms
		{ 
			SysTick_count_10ms = 0;SysTick_10ms = 1;
		}				
		
	if( SysTick_count_50ms == 50 )													//50ms
		{ SysTick_count_50ms = 0; SysTick_50ms = 1;}
		
	if( SysTick_count_100ms == 100 )												//100ms
		{ SysTick_count_100ms = 0; SysTick_100ms = 1;}			
		
	if( SysTick_count_300ms == 300 )												//300ms
		{ SysTick_count_300ms = 0; SysTick_300ms = 1;}	
		
	if( SysTick_count_1s == 1000 )													//1s
		{ SysTick_count_1s = 0; SysTick_1s = 1; }

	if(	iic_flag[0] == 1 || iic_flag[1] == 1 ||
			iic_flag[2] == 1 || iic_flag[3] == 1	)
	{
		iic_flag_handler();
	}
}

/**
  * @brief  TIM2_IRQHandler
  *         This function handles Timer2 Handler.
  * @param  None
  * @retval None
  */
//usb需要用到
void TIM2_IRQHandler(void)
{
  USB_OTG_BSP_TimerIRQ();
}


/**
  * @brief  OTG_FS_IRQHandler
  *          This function handles USB-On-The-Go FS global interrupt request.
  *          requests.
  * @param  None
  * @retval None
  */

void OTG_HS_IRQHandler(void)
{
  USBH_OTG_ISR_Handler(&USB_OTG_Core_dev);
}

void OTG_FS_IRQHandler(void)
{
	USBD_OTG_ISR_Handler (&USB_OTG_dev);
}

//虚拟串口需要使用
//USE_USB_OTG_FS
void OTG_FS_WKUP_IRQHandler(void)
{
  if(USB_OTG_dev.cfg.low_power)
  {
    *(uint32_t *)(0xE000ED10) &= 0xFFFFFFF9 ; 
		
		/* Reset SLEEPDEEP and SLEEPONEXIT bits */
    SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
    /* After wake-up from sleep mode, reconfigure the system clock */
    RCC_HSEConfig(RCC_HSE_ON);
    
    /* Wait till HSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
    {}
    
    /* Enable PLL */
    RCC_PLLCmd(ENABLE);
    
    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}
    
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    
    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {}
		
		
    SystemInit();
    USB_OTG_UngateClock(&USB_OTG_dev);
  }

}


/******************************************************************************/
/*                 STM32Fxxx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32fxxx.s).                                                */
/******************************************************************************/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

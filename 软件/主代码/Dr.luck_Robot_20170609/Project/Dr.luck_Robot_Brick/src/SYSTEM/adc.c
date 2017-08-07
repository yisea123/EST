/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/adc.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#include "adc.h"
                             // 0       1     2      3      4      5     6      7      8      9
u16 ADC1_Channel_Vol[10][10];//ADCB0、ADCB1、ADCC0、ADCC1、ADCD0、ADCD1、ADCMA、ADCMB、ADCMC、ADCMD
															//C1     C0     A5     C3     A7     A6    C4     C5     B0     B1    

u16 ADC3_Channel_Vol[10][3];//PB-VOL 、ADCA0、ADCA1、
														//F3       F4     F5     

//初始化ADC															   
void  Adc1_Init(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADC1, DMA2 and GPIO clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /* DMA2 Stream0 channel2 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC1_Channel_Vol;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 10*10;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);

  /* Configure ADC1 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 10;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC3 regular channel13 configuration *************************************/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_3Cycles); //B0 C1 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 2, ADC_SampleTime_3Cycles); //B1 C0 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_3Cycles);  //C0 A5 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_3Cycles); //C1 C3
  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 5, ADC_SampleTime_3Cycles);  //D0 A7 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 6, ADC_SampleTime_3Cycles);  //D1 A6
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 7, ADC_SampleTime_3Cycles); //MA C4 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 8, ADC_SampleTime_3Cycles); //MB C5 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 9, ADC_SampleTime_3Cycles);  //MC B0 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 10, ADC_SampleTime_3Cycles); //MD B1

 /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
	
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConv(ADC1);
}

void  Adc3_Init(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  /* Enable ADC1, DMA2 and GPIO clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOF, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  /* DMA2 Stream0 channel2 configuration **************************************/
  DMA_InitStructure.DMA_Channel = DMA_Channel_2;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC3->DR);
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(&ADC3_Channel_Vol);
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 10*3;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream1, ENABLE);

  /* Configure ADC1 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC3 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 3;
  ADC_Init(ADC3, &ADC_InitStructure);

  /* ADC3 regular channel13 configuration *************************************/
  ADC_RegularChannelConfig(ADC3, ADC_Channel_9, 1, ADC_SampleTime_3Cycles);  //PB-VOL F3 
  ADC_RegularChannelConfig(ADC3, ADC_Channel_14, 2, ADC_SampleTime_3Cycles); //A0			F4  
  ADC_RegularChannelConfig(ADC3, ADC_Channel_15, 3, ADC_SampleTime_3Cycles); //A1 		F5 
	
 /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC3, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC3, ENABLE);
	
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConv(ADC3);
}


////获得ADC值
////ch: @ref ADC_channels 
////通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
////返回值:转换结果
//u16 Get_Adc(u8 ch)   
//{
//	  	//设置指定ADC的规则组通道，一个序列，采样时间
////	ADC_RegularChannelConfig(ADC3, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
////  
////	ADC_SoftwareStartConv(ADC3);		//使能指定的ADC1的软件转换启动功能	
//	 
////	while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));//等待转换结束

//	return ADC_GetConversionValue(ADC3);	//返回最近一次ADC1规则组的转换结果
//}
////获取通道ch的转换值，取times次,然后平均 
////ch:通道编号
////times:获取次数
////返回值:通道ch的times次转换结果平均值
//u16 Get_Adc_Average(u8 ch,u8 times)
//{
//	u32 temp_val=0;
//	u8 t;
//	for(t=0;t<times;t++)
//	{
//		temp_val+=Get_Adc(ch);
//		delay_ms(5);
//	}
//	return temp_val/times;
//} 

u16 ReadADC1AverageValue(u16 Channel)
{
	uint8_t i;
	uint32_t sum = 0;
	for(i=0; i<10; i++)
	{
	sum+=ADC1_Channel_Vol[i][Channel];
	}
	return (sum/10);
}

u16 ReadADC3AverageValue(u16 Channel)
{
	uint8_t i;
	uint32_t sum = 0;
	for(i=0; i<10; i++)
	{
	sum+=ADC3_Channel_Vol[i][Channel];
	}
	return (sum/10);
}


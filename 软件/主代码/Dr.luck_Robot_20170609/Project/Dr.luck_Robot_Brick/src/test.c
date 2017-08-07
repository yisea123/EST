/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/test.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   Header for test.c module
  ******************************************************************************
  */

#include "main.h"
#include "myfont.h"
#include "music.h"

//测试界面
u8 motor1_on = 0;
u8 motor2_on = 0;
u8 motor3_on = 0;
u8 motor4_on = 0;
u8 motor1_ok = 0;
u8 motor2_ok = 0;
u8 motor3_ok = 0;
u8 motor4_ok = 0;
//u8 motor_state = 0;
//u8 count = 0;
u8 soundtest = 0;
u8 motor_invert_flag=0;

void Motor_Movement(void)//while(1)一直执行
{
	int i;
	if(motor1_on)
	{
//		Meduim_Motor_On(5 ,  100, 0 );
//		Meduim_Motor_On(6 ,  100, 0 );
//		Meduim_Motor_On(7 ,  100, 0 );
//		Meduim_Motor_On(8 ,  100, 0 );
		
		if(Channel_State[4][1]==5)//中
			motor1_ok = 	Meduim_Motor_OnForDegrees(5,
																	          100,
																				      360,
																								1,
																									motor_invert_flag);
		if(Channel_State[4][1]==4)//大
		motor1_ok =   Large_Motor_OnForDegrees(5,
																						100,
																							360,
																								1,
																									motor_invert_flag);
		if(Channel_State[4][1]==0)
		{
			SumPluseForDisplay[0] = 0;
		}
	}
	if(motor2_on)
	{
		if(Channel_State[5][1]==5)//中
		motor2_ok = 	Meduim_Motor_OnForDegrees(6,
																	          100,
																				      360,
																								1,
																									motor_invert_flag);
		if(Channel_State[5][1]==4)//大
		motor2_ok =   Large_Motor_OnForDegrees(6,
																						100,
																							360,
																								1,
																									motor_invert_flag);
		if(Channel_State[5][1]==0)
		{
			SumPluseForDisplay[1] = 0;
		}
	}
	if(motor3_on)
	{
		if(Channel_State[6][1]==5)//中
		motor3_ok = 	Meduim_Motor_OnForDegrees(7,
																	          100,
																				      360,
																								1,
																									motor_invert_flag);
		
		if(Channel_State[6][1]==4)//大
		motor3_ok =   Large_Motor_OnForDegrees(7,
																						100,
																							360,
																								1,
																									motor_invert_flag);
		if(Channel_State[6][1]==0)
		{
			SumPluseForDisplay[2] = 0;
		}
	}
	if(motor4_on)
	{
		if(Channel_State[7][1]==5)//中
		motor4_ok = 	Meduim_Motor_OnForDegrees(8,
																	          100,
																				      360,
																								1,
																									motor_invert_flag);
		if(Channel_State[7][1]==4)//大
		motor4_ok =   Large_Motor_OnForDegrees(8,
																						100,
																							360,
																								1,
																									motor_invert_flag);
		if(Channel_State[7][1]==0)
		{
			SumPluseForDisplay[3] = 0;
		}
	}
	
	if(motor1_ok) motor1_on = 0;
	if(motor2_ok) motor2_on = 0;
	if(motor3_ok) motor3_on = 0;
	if(motor4_ok) motor4_on = 0;
	
	if(soundtest == 1) 												//按键声音
	{	
		if(DREQ() != 0)//定时执行
		{
			if(DMA_GetCurrDataCounter(DMA1_Stream5)==0)
			{
				if( buttonsound_index < 15000 )
				{
					for(i=0; i<SPI_MP3_BUFNUM; i++)
					{
						SPI_Mp3_TxBuf[i] = music[i+buttonsound_index];
					}
					buttonsound_index += SPI_MP3_BUFNUM;
			
					MYDMA_Enable(DMA1_Stream5,SPI_MP3_BUFNUM);	
				}
				if( buttonsound_index >= 15000 )//循环复位
				{
					buttonsound_index = 0;
					tab_flag.sound_flag = 0;
				}
			}
		}
	}
}

void UI_Test(u8 button)//100ms执行
{
	int i;
	u32 total,free;
	u8 storage[32];
	float temp;
	
	switch(button)
	{
		case 0://
//			soundtest = 0;
//			LCD_clear();
//			LCD_Display_pic(0, 0, 180, 128, LOGO);
//			LCD_refresh();
			break;
		  
		case 1://颜色
			motor1_on = 0;
			motor2_on = 0;
			motor3_on = 0;
			motor4_on = 0;
			motor1_ok = 0;
			motor2_ok = 0;
			motor3_ok = 0;
			motor4_ok = 0;
//			count = 0;
			//电机端口脉冲复位
			for(i=0;i<4;i++)
			{
				SumPluseForDisplay[i]=0;
				Pluse[i] = 0;
			}
			soundtest = 0;
			
			if(Channel_State[0][1] == 1)
			{
				if(Channel_State[0][2] != 1)
					ChangeMode(1,2);
			}
			if(Channel_State[1][1] == 1)
			{
				if(Channel_State[1][2] != 1)
					ChangeMode(2,2);
			}
			if(Channel_State[2][1] == 1)
			{
				if(Channel_State[2][2] != 1)
					ChangeMode(3,2);
			}
			if(Channel_State[3][1] == 1)
			{
				if(Channel_State[3][2] != 1)
					ChangeMode(4,2);
			}
			
			LCD_clear();
			LCD_ShowString(10 ,10 ,"Col1:",16,0);
		  LCD_ShowString(100,10 ,"Col2:",16,0);
			LCD_ShowString(10 ,70 ,"Col3:",16,0);
		  LCD_ShowString(100,70 ,"Col4:",16,0);	
		
			LCD_ShowNum(52 ,10 ,2*ReadADC3AverageValue(1)*2500/4096,4,16);//1电压
			LCD_ShowNum(142,10 ,2*ReadADC1AverageValue(0)*2500/4096,4,16);//2电压
			LCD_ShowNum(52 ,70 ,2*ReadADC1AverageValue(2)*2500/4096,4,16);//3电压
			LCD_ShowNum(142,70 ,2*ReadADC1AverageValue(4)*2500/4096,4,16);//4电压
		
			LCD_ShowNum(72 ,50 ,Channel_State[0][2],1,16);//1模式
			LCD_ShowNum(162,50 ,Channel_State[1][2],1,16);//2模式
			LCD_ShowNum(72 ,110 ,Channel_State[2][2],1,16);//3模式
			LCD_ShowNum(162,110 ,Channel_State[3][2],1,16);//4模式
			
			if(Channel_State[0][1] == 1 || Channel_State[0][1] == 2 || Channel_State[0][1] == 3 || Channel_State[0][1] == 8)
				LCD_ShowString(10 ,30 ,"YES",16,0);
			else
				LCD_ShowString(10 ,30 ,"NO",16,0);
			
			if(Channel_State[1][1] == 1 || Channel_State[1][1] == 2 || Channel_State[1][1] == 3 || Channel_State[1][1] == 8)
				LCD_ShowString(100 ,30 ,"YES",16,0);
			else
				LCD_ShowString(100 ,30 ,"NO",16,0);
			
			if(Channel_State[2][1] == 1 || Channel_State[2][1] == 2 || Channel_State[2][1] == 3 || Channel_State[2][1] == 8)
				LCD_ShowString(10 ,90 ,"YES",16,0);
			else
				LCD_ShowString(10 ,90 ,"NO",16,0);
			
			if(Channel_State[3][1] == 1 || Channel_State[3][1] == 2 || Channel_State[3][1] == 3 || Channel_State[3][1] == 8)
				LCD_ShowString(100 ,90 ,"YES",16,0);
			else
				LCD_ShowString(100 ,90 ,"NO",16,0);
			
			if(Channel_State[0][1] == 1)
			{
				LCD_ShowNum(10 ,50 ,Channel_State[0][3],1,16);
			}else if(Channel_State[0][1] == 3 && Channel_State[0][2] == 2)
			{
				temp = Byte4ToFloat(Channel_State[0],1);
				if(temp > 180)
				{
					temp = -(255-temp);
				}
				LCD_ShowFloat(10 ,50 ,temp,16);
			}else if(Channel_State[0][1] == 3 && Channel_State[0][2] == 3)
			{
				LCD_ShowNum(10 ,50 ,Channel_State[0][4],1,16);
			}else{LCD_ShowFloat(10 ,50 ,Byte4ToFloat(Channel_State[0],0),16);}//1数值
			
			if(Channel_State[1][1] == 1)
			{
				LCD_ShowNum(100 ,50 ,Channel_State[1][3],1,16);
			}else if(Channel_State[1][1] == 3 )//&& Channel_State[1][2] == 2)
			{
				ChangeMode(2,1);
				Channel_State[1][2] = 2;
				temp = Byte4ToFloat(Channel_State[1],1);
				if(temp > 180)
				{
					temp = -(255-temp);
				}
				LCD_ShowFloat(100 ,50 ,temp,16);
			}
//			else if(Channel_State[1][1] == 3 && Channel_State[1][2] == 3)
//			{
//				LCD_ShowNum(100 ,50 ,Channel_State[1][4],1,16);
//			}
			else{LCD_ShowFloat(100 ,50 ,Byte4ToFloat(Channel_State[1],0),16);}//1数值
			
			if(Channel_State[2][1] == 1)
			{
				LCD_ShowNum(10 ,110 ,Channel_State[2][3],1,16);
			}
//			else if(Channel_State[2][1] == 3 && Channel_State[2][2] == 2)
//			{
//				temp = Byte4ToFloat(Channel_State[2],1);
//				if(temp > 180)
//				{
//					temp = -(255-temp);
//				}
//				LCD_ShowFloat(10 ,110 ,temp,16);
//			}
			else if(Channel_State[2][1] == 3)// && Channel_State[2][2] == 3)
			{
				ChangeMode(3,2);
				Channel_State[2][2] = 3;
				LCD_ShowNum(10 ,110 ,Channel_State[2][4],1,16);
			}
			else{LCD_ShowFloat(10 ,110 ,Byte4ToFloat(Channel_State[2],0),16);}//1数值
			
			if(Channel_State[3][1] == 1)
			{
				LCD_ShowNum(100 ,110 ,Channel_State[3][3],1,16);
			}else if(Channel_State[3][1] == 3 && Channel_State[3][2] == 2)
			{
				temp = Byte4ToFloat(Channel_State[3],1);
				if(temp > 180)
				{
					temp = -(255-temp);
				}
				LCD_ShowFloat(100 ,110 ,temp,16);
			}else if(Channel_State[3][1] == 3 && Channel_State[3][2] == 3)
			{
				LCD_ShowNum(100 ,110 ,Channel_State[3][4],1,16);
			}else{LCD_ShowFloat(100 ,110 ,Byte4ToFloat(Channel_State[3],0),16);}//1数值

			LCD_refresh();
			break;

		case 2://声音
			motor1_on = 0;
			motor2_on = 0;
			motor3_on = 0;
			motor4_on = 0;
			motor1_ok = 0;
			motor2_ok = 0;
			motor3_ok = 0;
			motor4_ok = 0;
//			count = 0;
			//电机端口脉冲复位
			for(i=0;i<4;i++)
			{
				SumPluseForDisplay[i]=0;
				Pluse[i] = 0;
			}
			soundtest = 0;
			LCD_clear();
			LCD_ShowString(10 ,10 ,"Sou1:",16,0);
		  LCD_ShowString(100,10 ,"Sou2:",16,0);
			LCD_ShowString(10 ,70 ,"Sou3:",16,0);
		  LCD_ShowString(100,70 ,"Sou4:",16,0);	
		
			LCD_ShowNum(52 ,10 ,GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_8),4,16);//1电压
			LCD_ShowNum(142,10 ,GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_14),4,16);//2电压
			LCD_ShowNum(52 ,70 ,GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7),4,16);//3电压
			LCD_ShowNum(142,70 ,GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10),4,16);//4电压
		
			LCD_ShowFloat(10 ,50 ,Byte4ToFloat(Channel_State[0],0),16);//1数值
			LCD_ShowFloat(100,50 ,Byte4ToFloat(Channel_State[1],0),16);//2数值
			LCD_ShowFloat(10 ,110 ,Byte4ToFloat(Channel_State[2],0),16);//3数值
			LCD_ShowFloat(100,110 ,Byte4ToFloat(Channel_State[3],0),16);//4数值

			if(Channel_State[0][1] == 0x0a)
				LCD_ShowString(10 ,30 ,"YES",16,0);
			else
				LCD_ShowString(10 ,30 ,"NO",16,0);
			
			if(Channel_State[1][1] == 0x0a)
				LCD_ShowString(100 ,30 ,"YES",16,0);
			else
				LCD_ShowString(100 ,30 ,"NO",16,0);
			
			if(Channel_State[2][1] == 0x0a)
				LCD_ShowString(10 ,90 ,"YES",16,0);
			else
				LCD_ShowString(10 ,90 ,"NO",16,0);
			
			if(Channel_State[3][1] == 0x0a)
				LCD_ShowString(100 ,90 ,"YES",16,0);
			else
				LCD_ShowString(100 ,90 ,"NO",16,0);
			
			LCD_refresh();			
			break;
		
		case 3://触碰
			motor1_on = 0;
			motor2_on = 0;
			motor3_on = 0;
			motor4_on = 0;
			motor1_ok = 0;
			motor2_ok = 0;
			motor3_ok = 0;
			motor4_ok = 0;
//			count = 0;
			for(i=0;i<4;i++)
			{
				SumPluseForDisplay[i]=0;
				Pluse[i] = 0;
			}
			soundtest = 0;
			LCD_clear();
			LCD_ShowString(10 ,10 ,"Tou1:",16,0);
		  LCD_ShowString(100,10 ,"Tou2:",16,0);
			LCD_ShowString(10 ,70 ,"Tou3:",16,0);
		  LCD_ShowString(100,70 ,"Tou4:",16,0);	
		
			LCD_ShowNum(52 ,10 ,2*ReadADC3AverageValue(1)*2500/4096,4,16);//1电压
			LCD_ShowNum(142,10 ,2*ReadADC1AverageValue(0)*2500/4096,4,16);//2电压
			LCD_ShowNum(52 ,70 ,2*ReadADC1AverageValue(2)*2500/4096,4,16);//3电压
			LCD_ShowNum(142,70 ,2*ReadADC1AverageValue(4)*2500/4096,4,16);//4电压
					
			LCD_ShowNum(10 ,50 ,Channel_State[0][3],1,16);//1数值
			LCD_ShowNum(100,50 ,Channel_State[1][3],1,16);//2数值
			LCD_ShowNum(10 ,110 ,Channel_State[2][3],1,16);//3数值
			LCD_ShowNum(100,110 ,Channel_State[3][3],1,16);//4数值

			if(Channel_State[0][1] == 7)
				LCD_ShowString(10 ,30 ,"YES",16,0);
			else
				LCD_ShowString(10 ,30 ,"NO",16,0);
			
			if(Channel_State[1][1] == 7)
				LCD_ShowString(100 ,30 ,"YES",16,0);
			else
				LCD_ShowString(100 ,30 ,"NO",16,0);
			
			if(Channel_State[2][1] == 7)
				LCD_ShowString(10 ,90 ,"YES",16,0);
			else
				LCD_ShowString(10 ,90 ,"NO",16,0);
			
			if(Channel_State[3][1] == 7)
				LCD_ShowString(100 ,90 ,"YES",16,0);
			else
				LCD_ShowString(100 ,90 ,"NO",16,0);

			LCD_refresh();
			break;
		
		case 4://温度
			motor1_on = 0;
			motor2_on = 0;
			motor3_on = 0;
			motor4_on = 0;
			motor1_ok = 0;
			motor2_ok = 0;
			motor3_ok = 0;
			motor4_ok = 0;
//			count = 0;
			//电机端口脉冲复位
			for(i=0;i<4;i++)
			{
				SumPluseForDisplay[i]=0;
				Pluse[i] = 0;
			}
			soundtest = 0;
			LCD_clear();
			LCD_ShowString(10 ,10 ,"Tem1:",16,0);
		  LCD_ShowString(100,10 ,"Tem2:",16,0);
			LCD_ShowString(10 ,70 ,"Tem3:",16,0);
		  LCD_ShowString(100,70 ,"Tem4:",16,0);	
		
			LCD_ShowNum(52 ,10 ,2*ReadADC3AverageValue(2)*2500/4096,4,16);//1电压
			LCD_ShowNum(142,10 ,2*ReadADC1AverageValue(1)*2500/4096,4,16);//2电压
			LCD_ShowNum(52 ,70 ,2*ReadADC1AverageValue(3)*2500/4096,4,16);//3电压
			LCD_ShowNum(142,70 ,2*ReadADC1AverageValue(5)*2500/4096,4,16);//4电压
					
			LCD_ShowFloat(10 ,50 ,Byte4ToFloat(Channel_State[0],0),16);//1数值
			LCD_ShowFloat(100,50 ,Byte4ToFloat(Channel_State[1],0),16);//2数值
			LCD_ShowFloat(10 ,110 ,Byte4ToFloat(Channel_State[2],0),16);//3数值
			LCD_ShowFloat(100,110 ,Byte4ToFloat(Channel_State[3],0),16);//4数值

			if(Channel_State[0][1] == 6 )
				LCD_ShowString(10 ,30 ,"YES",16,0);
			else
				LCD_ShowString(10 ,30 ,"NO",16,0);
			
			if(Channel_State[1][1] == 6 )
				LCD_ShowString(100 ,30 ,"YES",16,0);
			else
				LCD_ShowString(100 ,30 ,"NO",16,0);
			
			if(Channel_State[2][1] == 6 )
				LCD_ShowString(10 ,90 ,"YES",16,0);
			else
				LCD_ShowString(10 ,90 ,"NO",16,0);
			
			if(Channel_State[3][1] == 6 )
				LCD_ShowString(100 ,90 ,"YES",16,0);
			else
				LCD_ShowString(100 ,90 ,"NO",16,0);

			LCD_refresh();
			break;
		
		case 5://USB_Flash_Battery_Music
			motor1_on = 0;
			motor2_on = 0;
			motor3_on = 0;
			motor4_on = 0;
			motor1_ok = 0;
			motor2_ok = 0;
			motor3_ok = 0;
			motor4_ok = 0;
//			count = 0;
			//电机端口脉冲复位
			for(i=0;i<4;i++)
			{
				SumPluseForDisplay[i]=0;
				Pluse[i] = 0;
			}
			STM_EVAL_LEDOn(LED3);
			STM_EVAL_LEDOff(LED4);
			soundtest = 1;
			LCD_clear();
			LCD_ShowString(10 ,10 ,"USB:",16,0);
			LCD_ShowString(80 ,10 ,"USB_M:",16,0);
		  LCD_ShowString(10 ,40 ,"Flash:",16,0);
			LCD_ShowString(10 ,100 ,"Battery:",16,0);
		
			LCD_ShowNum(50,10,SlaveComConnected,1, 16);
			LCD_ShowNum(150,10,HostComConnected,1, 16);
		
			if(W25QXX_TYPE == W25Q256)
			{
				LCD_ShowString(80,40,"0xEF18",16,0);
			}else
			{
				LCD_ShowString(80,40,"None",16,0);
			}
			
			free = mf_showfree("1:",&total);
			sprintf(&storage[0], "%.1f MB",((float)free)/1024);
			LCD_ShowString(80, 70, storage,16,0);//空闲内存
			
			LCD_ShowNum(80,100,ReadADC3AverageValue(0)*2500/4096,4, 16);
			
			if(BATTERY() == 0)
				LCD_ShowString(120 ,100 ,"YES",16,0);
			else
				LCD_ShowString(120 ,100 ,"NO",16,0);
			
			LCD_refresh();
			break;	
		
		case 6://电机
			STM_EVAL_LEDOff(LED3);
			STM_EVAL_LEDOn(LED4);
			soundtest = 0;
			LCD_clear();
			LCD_ShowString(10 ,10 ,"MA:",16,0);
			LCD_ShowString(100,10 ,"MB:",16,0);
			LCD_ShowString(10 ,70 ,"MC:",16,0);
			LCD_ShowString(100,70 ,"MD:",16,0);
			LCD_ShowString(10 ,28 ,"type:",16,0);
			LCD_ShowString(100,28 ,"type:",16,0);
			LCD_ShowString(10 ,88 ,"type:",16,0);
			LCD_ShowString(100,88 ,"type:",16,0);

			LCD_ShowNum(52 ,10 ,2*ReadADC1AverageValue(6)*2500/4096,4,16);//A电压
			LCD_ShowNum(142,10 ,2*ReadADC1AverageValue(7)*2500/4096,4,16);//B电压
		  LCD_ShowNum(52 ,70 ,2*ReadADC1AverageValue(8)*2500/4096,4,16);//C电压
			LCD_ShowNum(142,70 ,2*ReadADC1AverageValue(9)*2500/4096,4,16);//D电压
		
			if(Channel_State[4][1] == 4)
				LCD_Display_pic(60,30, 12, 12, hanzi_14 );
			else if(Channel_State[4][1] == 5)
				LCD_Display_pic(60,30, 12, 12, hanzi_104 );
			else{}
			
			if(Channel_State[5][1] == 4)
				LCD_Display_pic(150,30, 12, 12, hanzi_14 );
			else if(Channel_State[5][1] == 5)
				LCD_Display_pic(150,30, 12, 12, hanzi_104 );
			else{}
				
			if(Channel_State[6][1] == 4)
				LCD_Display_pic(60,90, 12, 12, hanzi_14 );
			else if(Channel_State[6][1] == 5)
				LCD_Display_pic(60,90, 12, 12, hanzi_104 );
			else{}	
				
			if(Channel_State[7][1] == 4)
				LCD_Display_pic(150,90, 12, 12, hanzi_14 );
			else if(Channel_State[7][1] == 5)
				LCD_Display_pic(150,90, 12, 12, hanzi_104 );
			else{}
				
//			ButtonAction=0;
				
//			switch(motor_state)
//			{
//				case 0:
//					if(motor1_ok == 0)
//					{
//						motor1_on = 1;
//					}
//					else
//					{
//						LCD_ShowString(10,46,"OK",16,0);
//						count++;
//						if(count == 5)
//						{
//							count = 0;
//							motor_state ++;
//						}
//					}
//					break;
//				case 1:
//					if(motor2_ok == 0)
//					{
//						motor2_on = 1;
//					}
//					else
//					{
//						LCD_ShowString(100,46,"OK",16,0);				
//						count++;
//						if(count == 5)
//						{
//							count = 0;
//							motor_state ++;
//						}
//					}					
//					break;
//				case 2:
//					if(motor3_ok == 0)
//					{
//						motor3_on = 1;
//					}
//					else
//					{
//						LCD_ShowString(10,108,"OK",16,0);				
//						count++;
//						if(count == 5)
//						{
//							count = 0;
//							motor_state ++;
//						}
//					}					
//					break;
//				case 3:
//					if(motor4_ok == 0)
//					{
//						motor4_on = 1;
//					}
//					else
//					{
//						LCD_ShowString(100,108,"OK",16,0);	
//						count++;
//						if(count == 5)
//						{
//							count = 0;
//							motor_state = 0;
//							motor1_ok = 0;
//							motor2_ok = 0;
//							motor3_ok = 0;
//							motor4_ok = 0;
//						}
//					}
//					break;
//			}
			LCD_refresh();
			break;
			
		case 13:
//			i=0;
			for(i=3; i<4; i++)
			{
				if(Channel_State[i][1] == 3)
				{
					if(Channel_State[i][2] == 1)
					{
						ChangeMode(i+1,1);
						Channel_State[i][2] = 2;
					}else if(Channel_State[i][2] == 2)
					{
						ChangeMode(i+1,2);
						Channel_State[i][2] = 3;
					}else if(Channel_State[i][2] == 3)
					{
						ChangeMode(i+1,0);
						Channel_State[i][2] = 1;
					}
				}
			}
			ButtonAction=1;
			LCD_refresh();
		break;
		case 17:
			if(factorytest == 0)
			{
				factorytest = 1;
			}else
			{
				factorytest = 0;
			}
			ButtonAction=0;
			LCD_refresh();
		break;
		case 18:
			
			motor1_on = 1;
			motor2_on = 1;
			motor3_on = 1;
			motor4_on = 1;
		

			if(motor_invert_flag == 0)
			{
				motor_invert_flag = 1 ;
			}else
			{
				motor_invert_flag = 0 ;
			}
			
			ButtonAction=6;
			LCD_refresh();
		break;

		default:break;
	}
}

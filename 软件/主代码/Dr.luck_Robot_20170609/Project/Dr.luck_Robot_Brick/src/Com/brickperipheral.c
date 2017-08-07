/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/brickperipheral.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#include "stm32fxxx_it.h"
#include "brickperipheral.h"
#include "disassembly.h"
#include "ui_control.h"
#include "C_Protocol.h"
#include "usart_sensor.h"
#include "iic_sensor.h"
#include "adc.h"
#include "myfatfs.h"
#include "vs10xx.h"
#include "music.h"
#include "spi3.h"
#include "piano.h"
#include "w25qxx.h"
#include "text.h"

uint32_t GetRamBase(uint8_t par1, uint8_t par2, uint8_t par3, uint8_t par4);
uint8_t GetByte(uint32_t rambase);
float GetFloat(uint32_t rambase);
void SetByte(uint32_t rambase, uint8_t par);
void SetFloat(uint32_t rambase, float par);

float Byte4ToFloat(uint8_t byte[], uint8_t len);
void FloatToByte4(float temp, uint8_t byte[],uint8_t len);

u8 BriLight_flag=0;//蓝色产亮
u8 Bributton_state=0;
u8 Bributton_value=0;
u8 Bributton_time=0;

void BriDisplay(uint8_t mode, uint8_t BufferSize, uint32_t bufferrambase)
{
	uint32_t textadr,j;
	uint8_t i,len,clear,x,y,x1,y1,x2,y2,radius,full,width,heigth,colour,size;
	if(BufferSize == 10 )
	{
		textadr = GetRamBase(RAM_Buff[bufferrambase],RAM_Buff[bufferrambase+1],RAM_Buff[bufferrambase+2],RAM_Buff[bufferrambase+3]);
		len 		= RAM_Buff[bufferrambase+4];
		clear 	= RAM_Buff[bufferrambase+5];
		x 			= RAM_Buff[bufferrambase+6];
		y				= RAM_Buff[bufferrambase+7];
		colour	= RAM_Buff[bufferrambase+8];
		size		= RAM_Buff[bufferrambase+9];
		if(size == 0)
		{size = 12;}
		else if(size == 1)
		{size = 16;}
		else if(size == 2)
		{size = 24;}
	}else if(BufferSize == 6)
	{
		if(mode == 3 )  //直线
		{
			clear 	= RAM_Buff[bufferrambase];
			x1 			= RAM_Buff[bufferrambase+1];
			y1 			= RAM_Buff[bufferrambase+2];
			x2 			= RAM_Buff[bufferrambase+3];
			y2 			= RAM_Buff[bufferrambase+4];
			colour	= RAM_Buff[bufferrambase+5];	
		}else if(mode == 4 )  //圆
		{
			clear 	= RAM_Buff[bufferrambase];
			x 			= RAM_Buff[bufferrambase+1];
			y 			= RAM_Buff[bufferrambase+2];
			radius 	= RAM_Buff[bufferrambase+3];
			full 		= RAM_Buff[bufferrambase+4];
			colour	= RAM_Buff[bufferrambase+5];
		}
	}else if(BufferSize == 7) //四边形
	{
			clear 	= RAM_Buff[bufferrambase];
			x 			= RAM_Buff[bufferrambase+1];
			y 			= RAM_Buff[bufferrambase+2];
			width 	= RAM_Buff[bufferrambase+3];
			heigth 	= RAM_Buff[bufferrambase+4];
			full 		= RAM_Buff[bufferrambase+5];
			colour	= RAM_Buff[bufferrambase+6];
	}else if(BufferSize == 4) //点
	{
			clear 	= RAM_Buff[bufferrambase];
			x 			= RAM_Buff[bufferrambase+1];
			y 			= RAM_Buff[bufferrambase+2];
			colour	= RAM_Buff[bufferrambase+3];
	}else if(BufferSize == 8) //图片
	{
		textadr = GetRamBase(RAM_Buff[bufferrambase],RAM_Buff[bufferrambase+1],RAM_Buff[bufferrambase+2],RAM_Buff[bufferrambase+3]);
		len 		= RAM_Buff[bufferrambase+4];
		clear 	= RAM_Buff[bufferrambase+5];
		x 			= RAM_Buff[bufferrambase+6];
		y				= RAM_Buff[bufferrambase+7];
//		width 	= RAM_Buff[bufferrambase+8];
//		heigth 	= RAM_Buff[bufferrambase+9];
	}
	
	switch(mode)
	{
		case 0:
			LCD_clear();																		//清屏
			LCD_Display_pic(0, 12, 180, 128, UI_RUN);
			break;
		
		case 1:
			if(clear == 1)
			{
				LCD_clear();
			}
			Show_Str(x, y,180-x,128-y,(Flash_Buff+textadr),size,colour);		
			break;
			
		case 2:			
			if(clear == 1)
			{
				LCD_clear();
			}
			Show_Str(8*x, 8*y,180-x,128-y,(Flash_Buff+textadr),size,colour);		
			break;
			
		case 3:
			if(clear == 1)
			{
				LCD_clear();
			}
			LCD_Draw_Line(x1, y1, x2, y2, colour);
			break;
			
		case 4:
			if(clear == 1)
			{
				LCD_clear();
			}
			if(full == 1)
			{
				colour = -colour;
			}
			LCD_Draw_Circle( x, y, radius,full, colour);
//			full 		= RAM_Buff[bufferrambase+4];
//			printf("Her:%d-%d-%d-%d-%d\r\n" , x, y, radius,full, colour);
//			LCD_Draw_Circle(0, 0, 10, 1, 1);
			break;
			
		case 5:
			if(clear == 1)
			{
				LCD_clear();
			}
			if(full == 1)
			{
				colour = -colour;
			}
			LCD_Draw_Rectangle( x, y, x+width, y+heigth,full, colour);
//			full 		= RAM_Buff[bufferrambase+5];
			break;
		
		case 6:
			if(clear == 1)
			{
				LCD_clear();
			}
			LCD_DrawPoint( x, y, colour);
			break;
		
		case 7:
			if(clear == 1)
			{
				LCD_clear();
			}
			
			for(i=0; i<len+5; i++)
			{
				myfilename[i] = 0;
			}
			myfilename[0] = 0x31;//代表(1)
			myfilename[1] = 0x3A;//代表(:)
			for(i=0; i< len; i++)
			{
				myfilename[2+i] = *(Flash_Buff+textadr+i);
//				printf("myfilename[%x]：%x\n",2+i,myfilename[2+i]);
			}
			for(j=0; j < sizeof(PICTURE_Buff); j++)
			{
				PICTURE_Buff[j]=0;
			}
			res_flash = f_open(&file2,(const TCHAR*)myfilename, FA_OPEN_EXISTING | FA_READ);	
			if(res_flash == FR_OK)
			{
//				printf("》打开文件成功。\r\n");
				res_flash = f_read(&file2, PICTURE_Buff,  file2.fsize, &fnum); 
				if(res_flash==FR_OK)
				{
//					printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
//					printf("》读取得的文件数据为：\r\n%s \r\n", PICTURE_Buff);	
				}
				else
				{
//					printf("！！文件读取失败：(%d)\n",res_flash);
				}		
				/* 不再读写，关闭文件 */
				f_close(&file2);	
			}
//			LCD_Display_pic(0, 0,178, 128, TEST);
			LCD_Display_pic(x, y,PICTURE_Buff[0], PICTURE_Buff[1], PICTURE_Buff+2);
			
			break;
		
		case 8:
			if(len == 1)
				{
					if(clear == 1)
					{
						LCD_clear();
					}
					LCD_ShowNum(x,y,GetByte(GetRamBase(RAM_Buff[bufferrambase], RAM_Buff[bufferrambase+1],
																							RAM_Buff[bufferrambase+2], RAM_Buff[bufferrambase+3])),len,size);
				}
				if(len == 4)
				{
					if(clear == 1)
					{
						LCD_clear();
					}
					LCD_ShowFloat(x,y,GetFloat(GetRamBase(RAM_Buff[bufferrambase], RAM_Buff[bufferrambase+1],
																							RAM_Buff[bufferrambase+2], RAM_Buff[bufferrambase+3])),size);
				}
			break;
				
		case 9:
			if(len == 1)
				{
					if(clear == 1)
					{
						LCD_clear();
					}
				
					LCD_ShowNum(8*x,8*y,GetByte(GetRamBase(RAM_Buff[bufferrambase], RAM_Buff[bufferrambase+1],
																							RAM_Buff[bufferrambase+2], RAM_Buff[bufferrambase+3])),len,size);
				}
				if(len == 4)
				{
					if(clear == 1)
					{
						LCD_clear();
					}
					LCD_ShowFloat(8*x,8*y,GetFloat(GetRamBase(RAM_Buff[bufferrambase], RAM_Buff[bufferrambase+1],
																							RAM_Buff[bufferrambase+2], RAM_Buff[bufferrambase+3])),size);
				}
			break;
				
			default:break;
	}
//	printf("mode：%x\n",mode);
}

u8 BriSound(uint8_t mode, uint32_t bufferrambase)
{	
	uint32_t textadr,i;
	uint8_t len,style,result=0;
	float frequency,volume,continuetime;
//	int i;
	
	switch(mode)
	{
		case 0:
			SOUND_Buff_index = 0;
			SOUND_Buff_flag = 0;
			piano_index = 0;
			piano_Buff_flag = 0;
			result = 1;
			break;
		
		case 1:
			frequency     = GetFloat(bufferrambase);
			continuetime  = GetFloat(bufferrambase+4);
			volume        = GetFloat(bufferrambase+8);
			style         = RAM_Buff[bufferrambase+12];
			
			if(piano_Buff_flag == 0 && piano_index == 0)
			{
				if(frequency == PIANO_fre[0])						//C  	261.63
				{
					piano_num = 1;
				}else if(frequency == PIANO_fre[1])     //C#  277.18
				{
					piano_num = 2;
				}else if(frequency == PIANO_fre[2])     //D  	293.67
				{
					piano_num = 3;
				}else if(frequency == PIANO_fre[3])     //D#  311.13
				{
					piano_num = 4;
				}else if(frequency == PIANO_fre[4])     //E  	329.63
				{
					piano_num = 5;
				}else if(frequency == PIANO_fre[5])     //F  	349.23
				{
					piano_num = 6;
				}else if(frequency == PIANO_fre[6])     //F#  369.99
				{
					piano_num = 7;
				}else if(frequency == PIANO_fre[7])     //G  	392
				{
					piano_num = 8;
				}else if(frequency == PIANO_fre[8])     //G#  415.31
				{
					piano_num = 9;
				}else if(frequency == PIANO_fre[9])     //A  	440
				{
					piano_num = 10;
				}else if(frequency == PIANO_fre[10])     //A#  466.16
				{
					piano_num = 11;
				}else if(frequency == PIANO_fre[11])     //B  	493.88
				{
					piano_num = 12;
				}else if(frequency == PIANO_fre[12])     //C  	523.25
				{
					piano_num = 13;
				}else if(frequency == PIANO_fre[13])     //C#  554.37
				{
					piano_num = 14;
				}else if(frequency == PIANO_fre[14])     //D  	587.33
				{
					piano_num = 15;
				}else if(frequency == PIANO_fre[15])     //D#  622.25
				{
					piano_num = 16;
				}else if(frequency == PIANO_fre[16])     //E  	659.26
				{
					piano_num = 17;
				}else if(frequency == PIANO_fre[17])     //F  	698.46
				{
					piano_num = 18;
				}else if(frequency == PIANO_fre[18])     //F#  739.99
				{
					piano_num = 19;
				}else if(frequency == PIANO_fre[19])     //G  	783.99
				{
					piano_num = 20;
				}else if(frequency == PIANO_fre[20])     //G#  830.61
				{
					piano_num = 21;
				}else if(frequency == PIANO_fre[21])     //A  	880
				{
					piano_num = 22;
				}else if(frequency == PIANO_fre[22])     //A#  932.33
				{
					piano_num = 23;
				}else if(frequency == PIANO_fre[23])     //B  	987.77
				{
					piano_num = 24;
				}else if(frequency == PIANO_fre[24])     //C  	1046.5
				{
					piano_num = 25;
				}else if(frequency == PIANO_fre[25])     //C#  1108.73
				{
					piano_num = 26;
				}else if(frequency == PIANO_fre[26])     //D  	1174.66
				{
					piano_num = 27;
				}else if(frequency == PIANO_fre[27])     //D  	1244.51
				{
					piano_num = 28;
				}else if(frequency == PIANO_fre[28])     //E  	1318.51
				{
					piano_num = 29;
				}else if(frequency == PIANO_fre[29])     //F   1396.91
				{
					piano_num = 30;
				}else if(frequency == PIANO_fre[30])     //F#  1479.98
				{
					piano_num = 31;
				}else if(frequency == PIANO_fre[31])     //G  	1567.98
				{
					piano_num = 32;
				}else if(frequency == PIANO_fre[32])     //G#  1661.22
				{
					piano_num = 33;
				}else if(frequency == PIANO_fre[33])     //A  	1760
				{
					piano_num = 34;
				}else if(frequency == PIANO_fre[34])     //A#  1864.66
				{
					piano_num = 35;
				}else if(frequency == PIANO_fre[35])     //B  	1975.53
				{
					piano_num = 36;
				}else if(frequency == PIANO_fre[36])     //C  	2093
				{
					piano_num = 37;
				}else
				{	piano_num = -1;}

				piano_Buff_flag = 1;	
				piano_index = 0;
				W25QXX_Read((u8*)piano_Buff,PIANO_adr(3328+7*(piano_num-1)),PIANO_LEN[piano_num-1]);
				SoundTime=0;
//				for(i=0;i<PIANO_LEN[piano_num-1];i++)
//				{printf("%2x ",piano_Buff[i]);}
			}
			
			if(style == 1)
			{
				if(SoundTime >= continuetime*10)
				{
					piano_index = 0;
					piano_Buff_flag = 0;
					result = 1;
				}
				if(piano_index >= PIANO_LEN[piano_num-1]+32 )
				{
					piano_index = 0;
					piano_Buff_flag = 0;
					result = 1;
				}
			}else if(style == 2)
			{
				SOUND_UI = 1;
				result = 1;
			}else if(style == 3)
			{
				if( piano_index >= PIANO_LEN[piano_num-1]+32)
				{
					piano_index = 0;
					piano_Buff_flag = 0;
				}
				if(SoundTime >= continuetime*10 )
				{
					piano_index = 0;
					piano_Buff_flag = 0;
					result = 1;
				}
			}
			
			break;
		
		case 2:
			textadr = GetRamBase(RAM_Buff[bufferrambase],RAM_Buff[bufferrambase+1],RAM_Buff[bufferrambase+2],RAM_Buff[bufferrambase+3]);
			len 		= RAM_Buff[bufferrambase+4];
			volume 	= GetFloat(bufferrambase+5);
			style 	= RAM_Buff[bufferrambase+9];
//			style 	= GetFloat(bufferrambase+9);
				
			if(SOUND_Buff_flag==0 && SOUND_Buff_index == 0)//SOUND_Buff_frame == 0 && SOUND_Buff_index ==0 && SOUND_Buff_flag==0)
			{
				for(i=0; i<len+5; i++)
				{
					myfilename[i] = 0;
				}
				myfilename[0] = 0x31;//代表(1)
				myfilename[1] = 0x3A;//代表(:)
				for(i=0; i< len; i++)
				{
					myfilename[2+i] = *(Flash_Buff+textadr+i);
//					printf("myfilename[%x]：%x\n",2+i,myfilename[2+i]);
				}
				res_flash = f_open(&file3,(const TCHAR*)myfilename, FA_OPEN_EXISTING | FA_READ);
				if(res_flash == FR_OK)
				{
					SOUND_Buff_index = 0;
					SOUND_Buff_flag = 1;
//					f_read(&file3, SOUND_Buff,  file3.fsize, &fnum);
//					printf("res_flash:%x--%d\r\n",res_flash,file3.fsize);
				}
			}
			if(1)//res_flash == FR_OK)
			{
				if(style == 1)
				{
					if(SOUND_Buff_index >= file3.fsize+32)
					{
//						printf("end-SOUND_Buff_index:%x\r\n",SOUND_Buff_index);
//							printf("SOUND_Buff_frame:%x\r\n",SOUND_Buff_frame);
							SOUND_Buff_index = 0;
							SOUND_Buff_flag = 0;
//							SOUND_Buff_frame=0;
//							SOUND_update = 0;
							f_close(&file3);
							result = 1;
					}
				}
				else if(style == 2)
				{
					SOUND_UI = 1;
					result = 1;
				}else if(style == 3)
				{
					if(SOUND_Buff_index >= file3.fsize+32)
					{
//						printf("end-SOUND_Buff_index:%x\r\n",SOUND_Buff_index);
//							printf("SOUND_Buff_frame:%x\r\n",SOUND_Buff_frame);
							SOUND_Buff_index = 0;
							SOUND_Buff_flag = 0;
//							SOUND_Buff_frame=0;
//							SOUND_update = 0;
							f_close(&file3);
//							result = 1;
					}
				}else{}

			}else
			{
//				printf("》打开声音文件失败：%x。\r\n",res_flash);
			}
			break;
			default:
				break;
	}
	return result;
}

void BriButton(uint8_t mode, uint32_t bufferrambase)
{
	switch(mode)
	{
		case 0:
				switch(Bributton_value)
				{
					case 1:
						SetByte(bufferrambase, 16);//上
						SetByte(bufferrambase+1, 0);
						break;
					case 2:
						SetByte(bufferrambase, 32);//下
						SetByte(bufferrambase+1, 0);
						break;				
					case 3:
						SetByte(bufferrambase, 2);//左
						SetByte(bufferrambase+1, 0);
						break;	
					case 4:
						SetByte(bufferrambase, 8);//右
						SetByte(bufferrambase+1, 0);
						break;	
					case 5:
						SetByte(bufferrambase, 4);//确认
						SetByte(bufferrambase+1, 0);
						break;
					
					case 7:
						SetByte(bufferrambase, 0);//上（碰撞）
						SetByte(bufferrambase+1, 16);
						break;
					case 8:
						SetByte(bufferrambase, 0);//下（碰撞）
						SetByte(bufferrambase+1, 32);
						break;				
					case 9:
						SetByte(bufferrambase, 0);//左（碰撞）
						SetByte(bufferrambase+1, 2);
						break;	
					case 10:
						SetByte(bufferrambase, 0);//右（碰撞）
						SetByte(bufferrambase+1, 8);
						break;	
					case 11:
						SetByte(bufferrambase, 0);//确认（碰撞）
						SetByte(bufferrambase+1, 4);
						break;
					
					default:
						SetByte(bufferrambase, 1);//无
						SetByte(bufferrambase+1, 1);
						break;
				}
//				ButtonAction = 0;
			break;
		
			default:
				break;
	}
}
	
void BriLight(uint8_t mode, uint32_t bufferrambase)
{
	uint8_t color,pulse;
	
	switch(mode)
	{
		case 0:
					BriLight_flag = 6;//关闭
			break;
		
		case 1:
				color 	= RAM_Buff[bufferrambase];
				pulse 	= RAM_Buff[bufferrambase+1];
				
				if(color == 0 && pulse ==0)       		//蓝色
				{
					BriLight_flag = 0;
				}else if(color == 0 && pulse == 1)		//蓝色闪烁
				{
					BriLight_flag = 1;
				}else if(color == 1 && pulse == 0)		//红蓝色
				{
					BriLight_flag = 2;
				}else if(color == 1 && pulse == 1)		//红蓝色闪烁
				{
					BriLight_flag = 3;
				}else if(color == 2 && pulse == 0)		//红色
				{
					BriLight_flag = 4;
				}else if(color == 2 && pulse == 1)		//红色闪烁
				{
					BriLight_flag = 5;
				}
			break;
		
		case 2:
				BriLight_flag = 1;//默认 蓝色闪烁
			break;
		
			default:break;
	}
}
	
void BriBt(uint8_t mode, uint32_t bufferrambase)
{
	switch(mode)
	{
		case 0:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 1:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 2:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 3:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
			default:break;
	}
}
	
void BriMessage(uint8_t mode, uint32_t bufferrambase)
{
	switch(mode)
	{
		case 0:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 1:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 2:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 3:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 4:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 5:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
			default:break;
	}
}
	
void BriFile(uint8_t mode, uint32_t bufferrambase)
{
	switch(mode)
	{
		case 0:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 1:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 2:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 3:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 4:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
			default:break;
	}
}

void BriWaveform(uint8_t mode, uint32_t bufferrambase)
{
	switch(mode)
	{
		case 0:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 1:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 2:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 3:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
		case 4:
//				SetByte(bufferrambase, Motor_Degrees(RAM_Buff[portrambase], 0));
			break;
		
			default:break;
	}
}

void BriTimerSensor(uint8_t mode, uint32_t bufferrambase)
{
	uint8_t temp;
	temp = RAM_Buff[bufferrambase];

	switch(mode)
	{
		case 0:
			if(1 <= temp && temp <= 8)
			{
				SetFloat(bufferrambase+1, (Timer[temp - 1].time)/10);
			}
			break;
		
		case 1:
			if(1 <= temp && temp <= 8)
			{
				Timer[temp - 1].time = 0;
			}
			break;
			
		default:break;
	}
}

void BriRawSensor(uint8_t mode, uint32_t bufferrambase)
{
	uint8_t port;
	uint16_t temp; 
	
	switch(mode)
	{
		case 0:
			port = RAM_Buff[bufferrambase];
			if(1 <= port && port <= 4)
			{
				if(Channel_State[port-1][1] == 0x01 	|| 																//颜色、陀螺仪、红外
						Channel_State[port-1][1] == 0x02 	||
							Channel_State[port-1][1] == 0x03 )
				{
					SetFloat(bufferrambase+1, Byte4ToFloat(Channel_State[port-1],0));
				}else if(Channel_State[port-1][1] == 0x08)															//超声波
				{
					SetFloat(bufferrambase+1, Byte4ToFloat(Channel_State[port-1],0)*10);
				}else if(Channel_State[port-1][1] == 0x07) 															//按键
				{
					if(port == 1)
					{
						SetFloat(bufferrambase+1, 2*ReadADC3AverageValue(2)*2500/4096);
					}else
					{
						SetFloat(bufferrambase+1, 2*ReadADC3AverageValue(2*port-3)*2500/4096);
					}				
				}else if(Channel_State[port-1][1] == 0x0A) 															//声音
				{
					if(port == 1)
					{
						SetFloat(bufferrambase+1, 2*ReadADC3AverageValue(1)*2500/4096);
					}else
					{
						SetFloat(bufferrambase+1, 2*ReadADC3AverageValue(2*port-4)*2500/4096);
					}	
				}else if(Channel_State[port-1][1] == 0x06) 															//温度
				{
					temp = ((u16)(Byte4ToFloat(Channel_State[port-1],0)/0.0625f))<<4;//换算原始值
					SetFloat(bufferrambase+1, temp);
				}else if(Channel_State[port-1][1] == 0x09) 															//能量计
				{
					SetFloat(bufferrambase+1, Byte4ToFloat(Channel_State[port-1],0)*1000);
				}
				SetFloat(bufferrambase+1, Byte4ToFloat(Channel_State[port-1],0));
			}
			break;
			default:break;
	}
}



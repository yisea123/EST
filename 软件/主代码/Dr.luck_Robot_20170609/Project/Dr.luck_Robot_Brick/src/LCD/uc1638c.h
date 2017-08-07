/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/uc1638.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief  
  ******************************************************************************
**/
#ifndef __UC1638C_H
#define __UC1638C_H

#include "stm32fxxx_it.h" 
#include "stdio.h"

#define LCD_UC1638C_SCK_0 GPIO_ResetBits(GPIOB, GPIO_Pin_13) 					//SCK
#define LCD_UC1638C_SCK_1 GPIO_SetBits(GPIOB, GPIO_Pin_13)

#define LCD_UC1638C_SDA_0 GPIO_ResetBits(GPIOD, GPIO_Pin_9) 					//SDA
#define LCD_UC1638C_SDA_1 GPIO_SetBits(GPIOD, GPIO_Pin_9)

#define LCD_UC1638C_RST_0 GPIO_ResetBits(GPIOD, GPIO_Pin_8) 					//RST
#define LCD_UC1638C_RST_1 GPIO_SetBits(GPIOD, GPIO_Pin_8) 

void LCD_UC1638C_Init(void);
void LCD_refresh(void);
void LCD_clear(void);
void LCD_DrawPoint(u8 x,u8 y,u8 t);
void LCD_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void LCD_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);									//显示字符
void LCD_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);										//显示数字
void LCD_ShowFloat(u8 x,u8 y,float decnum,u8 size);										//显示浮点数
void LCD_ShowString(u8 x,u8 y,const u8 *p,u8 size,u8 mode);						//显示字符串
void LCD_Display_pic(u8 x, u8 y, u8 width, u8 hight, const u8 *pic);	//显示图片
void LCD_Draw_Line(u16 x1, u16 y1, u16 x2, u16 y2, u8 colour); 				//画线
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u8 full, u8 colour);						 			//画圆
void LCD_Draw_Rectangle(u16 x1, u16 y1, u16 x2, u16 y2,u8 full, u8 colour);		//画矩形

u8   LCD_Getcolor(u8 x, u8 y);
void LCD_Invert(u8 x, u8 y, u8 width, u8 hight);											//白底黑字反转

extern u8 LCD_GRAM[180][16];			//180*128
extern u32 fupd_prog(u16 x,u16 y,u8 size,u32 fsize,u32 pos);					//进度条

#endif


#ifndef __UC1638C_H
#define __UC1638C_H

#include "sys.h" 
#include "stdio.h"
#define LCD_UC1638C_SCK_0 GPIO_ResetBits(GPIOB, GPIO_Pin_13) 					//SCK
#define LCD_UC1638C_SCK_1 GPIO_SetBits(GPIOB, GPIO_Pin_13)

#define LCD_UC1638C_SDA_0 GPIO_ResetBits(GPIOD, GPIO_Pin_9) 					//SDA
#define LCD_UC1638C_SDA_1 GPIO_SetBits(GPIOD, GPIO_Pin_9)

#define LCD_UC1638C_RST_0 GPIO_ResetBits(GPIOD, GPIO_Pin_8) 					//RST
#define LCD_UC1638C_RST_1 GPIO_SetBits(GPIOD, GPIO_Pin_8) 

void LCD_refresh(void);
void LCD_clear(void);
void LCD_DrawPoint(u8 x,u8 y,u8 t);
void LCD_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void LCD_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void LCD_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void LCD_ShowFloat(u8 x,u8 y,float decnum,u8 size);
void LCD_ShowString(u8 x,u8 y,const u8 *p,u8 size,u8 mode);
void LCD_Display_pic(u8 x, u8 y, u8 width, u8 hight, const u8 *pic);

u8   LCD_Getcolor(u8 x, u8 y);
void LCD_Invert(u8 x, u8 y, u8 width, u8 hight);

void LCD_Draw_Line(u16 x1, u16 y1, u16 x2, u16 y2, u8 colour); //»­Ïß
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r, u8 colour);						 			//»­Ô²
void LCD_Draw_Rectangle(u16 x1, u16 y1, u16 x2, u16 y2, u8 colour);		   				//»­¾ØÐÎ


void LCD_UC1638C_Init(void);
//void clearScreen(void);
//void display_white(u8 d1, u8 d2);
//void Display_Char(u8 page,u8 column,u8 num);   //Ò³  ÁÐ  ×Ö·û
//void Display_Hex2(u8 row,u8 col,u8 hexnum);
//void Display_Dec2(u8 row,u8 col,u8 decnum);
//void Display_Dec4(u8 row,u8 col,u16 decnum);
//void Display_Float(u8 row,u8 col,float decnum) ;
//void Display_Chinese(u8 page,u8 column,const u8* dp);
//void Display_String_Eng( u8 row, u8 col, char* str);
//void Display_String_Chs( u8 row, u8 col, u8 numChar, const u8* dp);
//void Display_pic(u8 row, u8 col, u8 hight, u8 width, const u8 *pic);

//void display_test(u8 d1);

#endif

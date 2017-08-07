/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/text.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief  
  ******************************************************************************
**/
#ifndef __TEXT_H__
#define __TEXT_H__	 
#include <stm32f4xx.h>

//字体信息保存地址,占33个字节,第1个字节用于标记字库是否存在.后续每8个字节一组,分别保存起始地址和文件大小														   
extern u32 FONTINFOADDR;	

#define volume_ADDR   	1024*(1024*28+3200)				//volume
#define sleep_ADDR   		1024*(1024*28+3201)				//sleep
#define language_ADDR  	1024*(1024*28+3202)				//language
#define BT_START_ADDR  	1024*(1024*28+3203) 			//bt
#define BT_VIEW_ADDR  	1024*(1024*28+3204) 			//bt
#define light_ADDR  		1024*(1024*28+3205) 			//light

//字库信息结构体定义
//用来保存字库基本信息，地址，大小等
__packed typedef struct 
{
	u8 fontok;				//字库存在标志，0XAA，字库正常；其他，字库不存在
	u32 ugbkaddr; 			//unigbk的地址
	u32 ugbksize;			//unigbk的大小	 
	u32 f12addr;			//gbk12地址	
	u32 gbk12size;			//gbk12的大小	 
	u32 f16addr;			//gbk16地址
	u32 gbk16size;			//gbk16的大小		 
	u32 f24addr;			//gbk24地址
	u32 gkb24size;			//gbk24的大小 
}_font_info; 


extern _font_info ftinfo;	//字库信息结构体
 					     
u8 font_init(void);										//初始化字库
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);			//得到汉字的点阵码
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode);					//在指定位置显示一个汉字
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode);	//在指定位置显示一个字符串 
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len);
#endif

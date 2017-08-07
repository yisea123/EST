/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/text.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief  
  ******************************************************************************
**/
#include "sys.h" 
#include "w25qxx.h"
#include "uc1638c.h"
#include "text.h"	
#include "string.h"												    
#include "usart6.h"												    

//字库区域占用的总扇区数大小(3个字库+unigbk表+字库信息=3238700字节,约占791个W25QXX扇区)
#define FONTSECSIZE	 	791
//字库存放起始地址 
#define FONTINFOADDR 	1024*1024*28		//字库存放首地址
//定义各个字库的大小
#define UNIGBK         171*1024        //171KB
#define GBK12_FONSIZE  562*1024        //562KB
#define GBK16_FONSIZE  749*1024			   //749KB
#define GBK24_FONSIZE  1684*1024       //1684KB

 //用来保存字库基本信息，地址，大小等
_font_info ftinfo;

//初始化字体
//返回值:0,字库完好.
//		 其他,字库丢失
u8 font_init(void)
{		
	u8 t=0;
	W25QXX_Init();  
	while(t<10)//连续读取10次,都是错误,说明确实是有问题,得更新字库了
	{
		t++;
		W25QXX_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//读出ftinfo结构体数据
		if(ftinfo.fontok==0XAA)break;
	//	delay_ms(20);
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;		    
}

//code 字符指针开始
//从字库中查找出字模
//code 字符串的开始地址,GBK码
//mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小	
//size:字体大小
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{		    
	unsigned char qh,ql;
	unsigned char i;					  
	unsigned long foffset; 
	u8 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非 常用汉字
	{   		    
	    for(i=0;i<csize;i++)*mat++=0x00;//填充满格
			LCD_ShowString(10,60,(u8*)"Font no found...",12,1);
//			LCD_clear();
	    return; //结束访问
	}

	if(ql<0x7f)ql-=0x40;//注意!
	else ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*csize;	//得到字库中的字节偏移量  		  
	switch(size)
	{
		case 12:
			W25QXX_Read(mat,foffset+ftinfo.f12addr,csize);
			break;
		case 16:
			W25QXX_Read(mat,foffset+ftinfo.f16addr,csize);
			break;
		case 24:
			W25QXX_Read(mat,foffset+ftinfo.f24addr,csize);
			break;
			
	} 
}  
//显示一个指定大小的汉字
//x,y :汉字的坐标
//font:汉字GBK码
//size:字体大小
//mode:0,正常显示,1,叠加显示	   
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u16 y0=y;
	u8 dzk[72];   
	u8 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
	if(size!=12&&size!=16&&size!=24)return;	//不支持的size
	Get_HzMat(font,dzk,size);	//得到相应大小的点阵数据 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//得到点阵数据   

		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)
	//			LCD_Fast_DrawPoint(x,y,POINT_COLOR);
	{
//		LCD_ShowString(10,100,(u8*)"Erasing sectors... ",12,1);
//	LCD_refresh();
			LCD_DrawPoint(x,y,1);
	}
			else if(mode==0)
		//		LCD_Fast_DrawPoint(x,y,BACK_COLOR); 
			{
//				LCD_ShowString(10,100,(u8*)"Erasing ",12,1);
//	LCD_refresh();
			LCD_DrawPoint(x,y,0);
			}
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	} 
LCD_refresh();
}
//在指定位置开始显示一个字符串	    
//支持自动换行
//(x,y):起始坐标
//width,height:区域
//str  :字符串
//size :字体大小
//mode:0,非叠加方式;1,叠加方式    	   		   
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode)
{
	u16 x0=x;
	u16 y0=y;							  	  
    u8 bHz=0;     //字符或者中文  	    				    				  	  
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {      
                if(x>(x0+width-size/2))//换行
				{				   
					y+=size;
					x=x0;	   
				}							    
		        if(y>(y0+height-size))break;//越界返回      
		        if(*str==13)//换行符号
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else LCD_ShowChar(x,y,*str,size,mode);//有效部分写入 
				str++; 
		        x+=size/2; //字符,为全字的一半 
	        }
        }else//中文 
        {     
            bHz=0;//有汉字库    
            if(x>(x0+width-size))//换行
			{	    
				y+=size;
				x=x0;		  
			}
	        if(y>(y0+height-size))break;//越界返回  						     
	        Show_Font(x,y,str,size,mode); //显示这个汉字,空心显示 
	        str+=2; 
	        x+=size;//下一个汉字偏移	    
        }
    }   
}  			 		 
//在指定宽度的中间显示字符串
//如果字符长度超过了len,则用Show_Str显示
//len:指定要显示的宽度			  
/*void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len)
{
	u16 strlenth=0;
   	strlenth=strlen((const char*)str);
	strlenth*=size/2;
	if(strlenth>len)
		Show_Str(x,y,lcddev.width,lcddev.height,str,size,1);
	else
	{
		strlenth=(len-strlenth)/2;
	    Show_Str(strlenth+x,y,lcddev.width,lcddev.height,str,size,1);
	}
}  */ 

























		  







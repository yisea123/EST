
#include "math.h"
#include "uc1638c.h"
#include "font.h"      //字体

//LCD的显存
//存放格式如下.
//[0]0 1 2 3 ... 239	
//[1]0 1 2 3 ... 239	
//[2]0 1 2 3 ... 239	
//	.
//	.
//	.	
//[19]0 1 2 3 ... 239 		   
static u8 LCD_GRAM[180][16]={0};			//160*240

void LCD_UC1638C_GPIO_Init(void);
void Delay(u16 tt);
void initialize(void);
void iic_start(u8 cd)	;
void iic_send(unsigned char y);
void iic_stop(void);
void setWindowsProgame(void);
void set_uc1638c_Addr(u8 page,u8 column);
//void LCD_ShowString(u8 x,u8 y,const u8 *p,u8 size,u8 mode);

void LCD_UC1638C_Init(void)
{
	LCD_UC1638C_GPIO_Init();
	initialize();
//	setWindowsProgame();
}

void LCD_UC1638C_GPIO_Init(void)
{
	//GPIO参数初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	//启动GPIO口时钟  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 ;		//SDA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                                                    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
		 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;		//RST
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                                                    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;		//SCK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                                                    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
}

void initialize(void)
{
	Delay(1000);
	LCD_UC1638C_RST_1;
	Delay(400);
	LCD_UC1638C_RST_0;
	Delay(3200);
	LCD_UC1638C_RST_1;
	Delay(8000);
	
  //	unsigned char i;
	iic_start(0);			 //序列头
	iic_send(0xe1);	   //system reset
	iic_stop();				 //序列尾
	iic_start(1);
	iic_send(0xe2);		
	iic_stop();       
	Delay(200);
	
	iic_start(0);
	iic_send(0x04);		//column start		
	iic_stop();
	iic_start(1);
	iic_send(0x00);		//设置SRAM列地址
	iic_stop();       
				 
	iic_start(0);
	iic_send(0x2d);	  //internal booster	SET PUMP CONTROL
	iic_send(0x24);	  //tc
	iic_send(0xeb);		//1/12bias
	iic_stop();

	iic_start(0);
	iic_send(0x81);		 //pm set	SET VBIAS POTENTIOMETER
	iic_stop();
	iic_start(1);
	iic_send(85);  	//85=14.5V			？？？？
	iic_stop();

	iic_start(0);
	iic_send(0x40);		   //scroll line	设置滚动条
	iic_send(0x50);
	iic_send(0x86);		   //com scan mode
	iic_send(0x89);
	iic_send(0xC4);
	
//	iic_send(0xA7);			//Set Inverse Display(黑白颠倒)
	
	iic_send(0xa3);		   //line rate
	iic_send(0x95);		   //display mode
	iic_stop();

	iic_start(0);
	iic_send(0xc8);		   //n-line 
	iic_stop();
	iic_start(1);
	iic_send(57);
	iic_stop();


	iic_start(0);
	iic_send(0xf1);			//com end
	iic_stop();
	iic_start(1);
	iic_send(127);
	iic_stop();

	iic_start(0);
	iic_send(0xf2);		   //partial display com start
	iic_stop();
	iic_start(1);
	iic_send(0);
	iic_stop();

	iic_start(0);
	iic_send(0xf3);			//partial display com end 
	iic_stop();
	iic_start(1);
	iic_send(127);
	iic_stop();

	iic_start(0);
	iic_send(0x85);			//partial display enable
	iic_stop();   

	iic_start(0);
	iic_send(0xc9);			//display enable
	iic_stop();
	iic_start(1);
	iic_send(0xad);			 //b/w dispay mode
	iic_stop();
	

}

void setWindowsProgame(void)     
{
//	//设置从哪行哪列开始显示
//	iic_start(0);
//	iic_send(0xF4); //colum 		设置SRAM读写访问列地址为0x00
//	iic_stop();
//	iic_start(1);
//	iic_send(0x00);
//	iic_stop();
//	
//	iic_start(0);
//	iic_send(0x04); //page 			设置SRAM读写访问页地址从第0页开始
//	iic_stop();
//	iic_start(1);
//	iic_send(0x00);
//	iic_stop();
	
	//设置显示范围
	
	iic_start(0);
	iic_send(0xf4); //Set Window Programming Starting Column Address
	iic_stop();
	iic_start(1);
	iic_send(0x00); //startx  从0列开始（左）
	iic_stop();

	iic_start(0);
	iic_send(0xf6); //Set Window Programming Ending Column Address 
	iic_stop();
	iic_start(1);
	iic_send(239);//endx 			到239列结束（右）
	iic_stop();
	
	iic_start(0);
	iic_send(0xf5); //Set Window Programming Starting Page Address
	iic_stop();
	iic_start(1);
	iic_send(0x00); //从0页开始（上）
	iic_stop();
	
	iic_start(0);
	iic_send(0xf7); //Set Window Programming Ending Page Address
	iic_stop();
	iic_start(1);
	iic_send(127);	//到127页结束（下）
	iic_stop();
	
	iic_start(0);		//Enable Window Program 使能对屏幕的操作
	iic_send(0xf9);  
	iic_stop();
}



/*============================
  I2c start condition          
 SDA high->low while SCL=high 
     _________                 
  SCL         \_________       
     _______                  
  SDA       \___________      
============================*/

void iic_start(u8 cd)				//1send IIC start condition
{
	LCD_UC1638C_SDA_1;
	LCD_UC1638C_SCK_1;
	LCD_UC1638C_SDA_0;__NOP();
	LCD_UC1638C_SCK_0;__NOP();__NOP();__NOP();__NOP();
	
	LCD_UC1638C_SDA_0;LCD_UC1638C_SCK_1;LCD_UC1638C_SCK_0;
	LCD_UC1638C_SDA_1;LCD_UC1638C_SCK_1;LCD_UC1638C_SCK_0;
	LCD_UC1638C_SDA_1;LCD_UC1638C_SCK_1;LCD_UC1638C_SCK_0;
	LCD_UC1638C_SDA_1;LCD_UC1638C_SCK_1;LCD_UC1638C_SCK_0;			//序列头（0111B）
	
	LCD_UC1638C_SDA_1;LCD_UC1638C_SCK_1;LCD_UC1638C_SCK_0;
	LCD_UC1638C_SDA_0;LCD_UC1638C_SCK_1;LCD_UC1638C_SCK_0;			//设备地址 device address [1,0] 
	
	if(!cd)	LCD_UC1638C_SDA_0;			//contorl										//模式选择，control和data,control用于选择模式，data用于在该模式下收发数据
	else LCD_UC1638C_SDA_1;					//data
	LCD_UC1638C_SCK_1;LCD_UC1638C_SCK_0;
	LCD_UC1638C_SDA_0;LCD_UC1638C_SCK_1;LCD_UC1638C_SCK_0;		
	
	LCD_UC1638C_SCK_1;__NOP();__NOP();			//---signal A-----------//
	LCD_UC1638C_SCK_0;__NOP();__NOP();
	
}

//----------------------write_iic{cd=0(control);  cd=1(data)}---------------------//
void iic_send(unsigned char y)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		if(y&0x80)
			LCD_UC1638C_SDA_1;
		else
		  LCD_UC1638C_SDA_0;
		LCD_UC1638C_SCK_1;
		LCD_UC1638C_SCK_0;
		y=y<<1;
	}  
  LCD_UC1638C_SCK_1;       //---signal A-----------//
  __NOP();
  __NOP();       
  LCD_UC1638C_SCK_0;
  __NOP();
  __NOP();

}

/*============================
  I2c stop condition          
  SDA low->high while SCL=high 
            _________                 
  SCL______/       
             _______                  
  SDA_______/      
============================*/

void iic_stop(void)
{
  LCD_UC1638C_SDA_0;__NOP(); __NOP();
  LCD_UC1638C_SCK_0;__NOP();__NOP();
  LCD_UC1638C_SCK_1;__NOP();__NOP();__NOP();__NOP();__NOP();
  LCD_UC1638C_SDA_1;__NOP();__NOP();__NOP();__NOP();
}


//设置显示位置
void set_uc1638c_Addr(u8 page,u8 column) 
{
	iic_start(0);
	iic_send(0x04); //colum 		设置SRAM读写访问列地址
	iic_stop();
	iic_start(1);
	iic_send(column);
	iic_stop();
	
	iic_start(0);
	iic_send((page%256)|0x60); //page   0-3位			设置SRAM读写访问页地址从第0页开始
	iic_send((page/256&0x03)|0x70);		//4、5位
	iic_stop();
	
	iic_start(0);		//Enable Window Program 使能对屏幕的操作
	iic_send(0xf9);  
	iic_stop();

}

////显示字符
//void Display_Char(u8 page,u8 column,u8 num)   //页  列  字符
//{
////	u8 i,j;
////	const u8 *dp = FontNumber;       //指向数字取模数组地址的指针
////	
////	num -= 0x20;
////	
////	dp = &FontNumber[num*16];
////	
////	for(j=0; j<2; j++)
////	{
////		set_uc1638c_Addr(32+page+j,column);
////		for(i=0; i<8; i++)
////		{
////			iic_start(0);
////			iic_send(0x01);
////			iic_stop();
////			iic_start(1);
////			iic_send(*dp);
////			iic_stop();
////			
////			dp++;
////		}
////	}
//}

////单个文字显示：Display_Chinese(0,0,&MenuChinese[0*28]);
//void Display_Chinese(u8 page,u8 column,const u8* dp)
//{
//	u8 i,j;
//	
//	for(j=0; j<2; j++)	//2
//	{
//		set_uc1638c_Addr(32+page+j,column);
//		for(i=0; i<14; i++)	//14
//		{
//			iic_start(0);
//			iic_send(0x01);
//			iic_stop();
//			iic_start(1);
//			iic_send(*dp);
//			iic_stop();
//			dp++;
//		}
//	}
//}

//void Display_Hex2(u8 row,u8 col,u8 hexnum)   //行 列  16进制数   //修改了不能正确显示16进制
//{
//	 if((hexnum&0x0F) < 10)   Display_Char(row,col+8,(hexnum&0x0F)+48);
//	 else                   Display_Char(row,col+8,(hexnum&0x0F)+55);
//	
//	 if((hexnum>>4) < 10)    Display_Char(row,col,(hexnum>>4)+48);
//	 else                  Display_Char(row,col,(hexnum>>4)+55);
//}

//void Display_Dec2(u8 row,u8 col,u8 decnum)   //十进制数  2位
//{
//	Display_Char(row,col+8,decnum%10+48);
//	Display_Char(row,col,decnum/10+48);
//}

//void Display_Dec4(u8 row,u8 col,u16 decnum)    //十进制数  4位
//{
//	Display_Char(row,col+24,decnum%10+48);
//	decnum = decnum / 10;
//	Display_Char(row,col+16,decnum%10+48);
//	decnum = decnum / 10;
//	Display_Char(row,col+8,decnum%10+48);
//	decnum = decnum / 10;
//	Display_Char(row,col,decnum%10+48);
//}

//void Display_Float(u8 row,u8 col,float decnum)    //十进制浮点数  5位,带正负  1000.1
//{
//	if(decnum<0)			//负数
//	{
//		Display_Char(row,col,'-');
//		decnum = -decnum;
//		col += 8;
//	}
//	if((int)decnum/1000%10!=0)
//	{
//		Display_Char(row,col,(int)decnum/1000%10+48);
//		Display_Char(row,col+8,(int)decnum/100%10+48);
//		Display_Char(row,col+16,(int)decnum/10%10+48);
//		Display_Char(row,col+24,(int)decnum%10+48);
//		Display_Char(row,col+32,0x2E);
//		Display_Char(row,col+40,(int)(decnum*10)%10+48);
//	}
//	else if((int)decnum/100%10!=0)
//	{
//		Display_Char(row,col,(int)decnum/100%10+48);
//		Display_Char(row,col+8,(int)decnum/10%10+48);
//		Display_Char(row,col+16,(int)decnum%10+48);
//		Display_Char(row,col+24,0x2E);
//		Display_Char(row,col+32,(int)(decnum*10)%10+48);
//	}
//	else if((int)decnum/10%10!=0)
//	{
//		Display_Char(row,col,(int)decnum/10%10+48);
//		Display_Char(row,col+8,(int)decnum%10+48);
//		Display_Char(row,col+16,0x2E);
//		Display_Char(row,col+24,(int)(decnum*10)%10+48);
//	}else{
//		Display_Char(row,col,(int)decnum%10+48);
//		Display_Char(row,col+8,0x2E);
//		Display_Char(row,col+16,(int)(decnum*10)%10+48);
//	}
//}

//void Display_String_Chs( u8 row, u8 col, u8 numChar, const u8* dp )   //显示一串中文字
//{
//	u8 i;
//	for( i=0; i<numChar; i++ )
//	{
//		Display_Chinese(row,col+i*14,dp);
//		dp = dp + 28;
//	}
//}

//void Display_String_Eng( u8 row, u8 col, char* str )     //显示一串英文
//{
//	while(*str != '\0')
//	{
////		Display_Char(row,col,(*str));
//		col = col + 8;
//		str++;
//	};
//}

void Delay(u16 tt)
{
	u16 i,j;
	for(i=0;i<2000;i++)
		for(j=0;j<tt;j++);
}

////清屏
//void display_white(u8 d1, u8 d2)
//{
//  unsigned char i;
//  unsigned int j;
// 
//	for(i=0;i<16;i++)			 //宽，每排128个点，所以一共要16个8位2进制数才能全部控制
//  {
//		set_uc1638c_Addr(32+i,0);
//		for(j=0;j<180;j++)	  //长，一共120排
//		{
//		 iic_start(0);
//		 iic_send(0x01);
//		 iic_stop();
//		 iic_start(1);
//		 iic_send(d1);
//		 iic_stop();

//		 iic_start(0);
//		 iic_send(0x01);
//		 iic_stop();
//		 iic_start(1);
//		 iic_send(d2);
//		 iic_stop();
//		}
//	}
//	
//}

//void clearScreen(void)
//{
//	display_white(0x00, 0x00);
//}

//void display_test(u8 d1)
//{
//  unsigned char i;
//  unsigned int j;
// 
//	for(i=0;i<8;i++)			 //宽，每排128个点，所以一共要16个8位2进制数才能全部控制
//  {
//		set_uc1638c_Addr(32+i,0);
//		for(j=0;j<90;j++)	  //长，一共120排
//		{
//		 iic_start(0);
//		 iic_send(0x01);
//		 iic_stop();
//		 iic_start(1);
//		 iic_send(d1);
//		 iic_stop();

//		}
//	}
//	
//}

////row:0-16;
////col:0-180;
//void Display_pic(u8 row, u8 col, u8 hight, u8 width, const u8 *pic )
//{
//  unsigned char i;
//  unsigned int j;
//  
//	for(i=0;i<hight/8;i++)			 //16 高 
//  {
//		for(j=0;j<width;j++)	//240 宽
//		{
//			set_uc1638c_Addr(32+row+i,col+j);
//			iic_start(0);
//			iic_send(0x01);
//			iic_stop();
//			iic_start(1);
//			iic_send(*(pic++));
//			iic_stop();     
//	  }
//	}
//}







void LCD_refresh(void)
{
	u8 i,j,k;
	for(i=0; i<23; i++)
	{
		for(j=0; j<16; j++)
		{
			set_uc1638c_Addr(32+j,8*i);
			for(k=0; k<8; k++)
			{
				iic_start(0);
				iic_send(0x01);
				iic_stop();
				iic_start(1);
				iic_send(LCD_GRAM[8*i+k][j]);
				iic_stop();
			}
		}
	}
}

void LCD_clear(void)
{
	uint8_t i,j;
	
	for(j = 0; j< 16; j++)
	{
		for(i = 0; i< 180; i++)
		{
			LCD_GRAM[i][j]=0;
		}
	}
//	display_white(0x00, 0x00);
}

//画点 
//x:0~240
//y:0~160
//t:1 填充 0,清空				   
void LCD_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>180||y>128)return;//超出范围了.
//		LCD_ShowString(10,100,(u8*)"Erasing sectors... ",12,1);
//	LCD_refresh();
	pos=y/8;
	bx=y%8;
	temp=1<<(bx);
	if(t)LCD_GRAM[x][pos]|=temp;
	else LCD_GRAM[x][pos]&=~temp;
	
}

//在指定位置显示一个字符,包括部分字符
//x:0~240
//y:0~160
//mode:0,反白显示;1,正常显示				 
//size:选择字体 12/16/24
void LCD_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
	for(t=0;t<csize;t++)
	{
		if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_DrawPoint(x,y,mode);
			else LCD_DrawPoint(x,y,!mode);
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
////调试用：显示 LCD_GRAM[][] 数据
//	for(i = 0; i<240; i++)
//	{
//		for(j = 0; j<20; j++)
//		{
//			printf("%x ",LCD_GRAM[i][j]);
//		}
//		printf("\n\r");
//	}
}

//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void LCD_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
}

uint8_t Getlen(int num)
{
	uint8_t i=0;
	for(; num > 0; num /= 10){i++;}
	return i;
}
//显示4个Byte浮点数
//x,y :起点坐标	 
//len :显示数字的位数
//size:字体带大小
//mode:模式0,填充模式;1,叠加模式
void LCD_ShowFloat(u8 x,u8 y,float decnum,u8 size)
{
	int tmpint = 0, x1=0,len;
	
	if(decnum != NULL)
	{
		if(decnum < 0)
		{
			LCD_ShowChar(x,y,'-',size,1);
			decnum = - decnum;
			x1 += size/2;
		}
		tmpint = (int)decnum;
		len = Getlen(tmpint);
		LCD_ShowNum(x1+x,y, tmpint, len, size);
		
		LCD_ShowChar(x1+x+(size/2)*len,y,'.',size,1);
	
		decnum = decnum - tmpint;
		tmpint = (int)(decnum * pow(10,Getlen(tmpint)));
		if(tmpint == 0)
		{
			LCD_ShowNum(x1+x+(size/2)*(len+1),y, 0, 1, size);
		}else
		{
			LCD_ShowNum(x1+x+(size/2)*(len+1),y, tmpint, Getlen(tmpint), size);
		}
		
	}else
	{
		LCD_ShowNum(x1+x,y, 0, 1, size);		
		LCD_ShowChar(x1+x+(size/2),y,'.',size,1);
		LCD_ShowNum(x1+x+(size/2)*2,y, 0, 1, size);
	}
}

//显示字符串
//x,y:起点坐标  
//size:字体大小 
//*p:字符串起始地址 
//mode:0:白底黑字; 1:白字黑底
void LCD_ShowString(u8 x,u8 y,const u8 *p,u8 size,u8 mode)
{	
	while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
	{
		if(x>(240-(size/2))){x=0;y+=size;}
		if(y>(160-size)){y=x=0; LCD_clear();}
		if(mode) LCD_ShowChar(x,y,*p,size,1);	
		 else LCD_ShowChar(x,y,*p,size,0);	
		x+=size/2;
		p++;
	}
}

void LCD_Display_pic(u8 x, u8 y, u8 width, u8 hight, const u8 *pic )
{
	u8 temp,t,t1,i;
	u8 y0=y,x0=x;

	for(i=0;i<(hight/8+((hight%8)?1:0));i++)
	{
		x=x0;
		for(t=0;t<width;t++)
		{
			y=y0;
			temp = *(pic++);
			for(t1=0;t1<8;t1++)
			{
				if(temp&0x01)LCD_DrawPoint(x,y,1);
				else LCD_DrawPoint(x,y,0);
				temp>>=1;
				y++;
				if(hight%8!=0 && i==hight/8 && t1==hight%8)
				{
					break;
				}
			}
			x++;
		}
		y0 += 8;
	}
	
//	size = (hight/8+((hight%8)?1:0))*width;
//	for(t=0;t<size;t++)
//	{
//		temp = *(pic++);
//		for(t1=0;t1<8;t1++)
//		{
//			if(temp&0x80)LCD_DrawPoint(x,y,1);
//			else LCD_DrawPoint(x,y,0);
//			temp<<=1;
//			y++;
//			if((y-y0)==hight)
//			{
//				y=y0;
//				x++;
//				break;
//			}
//		}
//	}
}

u8 LCD_Getcolor(u8 x, u8 y)
{
	u8 pos,bx,temp=0;
	pos=y/8;
	bx=y%8;
	temp=1<<(bx);
	if((LCD_GRAM[x][pos]&temp) == 0)return 0;
	else return 1;
}

//使范围内的像数点反转（黑白反转）
void LCD_Invert(u8 x, u8 y, u8 width, u8 hight)
{
	u8 i,j;
	for(i=x; i<(x+width);i++)
	{
		for(j=y; j<(y+hight);j++)
		{
			if(LCD_Getcolor(i,j) == 1) LCD_DrawPoint(i,j,0);
			else if((LCD_Getcolor(i,j) == 0)) LCD_DrawPoint(i,j,1);
		}
	}
}

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r, u8 colour)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b,colour);             //5
 		LCD_DrawPoint(x0+b,y0-a,colour);             //0           
		LCD_DrawPoint(x0+b,y0+a,colour);             //4               
		LCD_DrawPoint(x0+a,y0+b,colour);             //6 
		LCD_DrawPoint(x0-a,y0+b,colour);             //1       
 		LCD_DrawPoint(x0-b,y0+a,colour);             
		LCD_DrawPoint(x0-a,y0-b,colour);             //2             
  		LCD_DrawPoint(x0-b,y0-a,colour);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_Draw_Line(u16 x1, u16 y1, u16 x2, u16 y2, u8 colour)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol,colour);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    

//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_Draw_Rectangle(u16 x1, u16 y1, u16 x2, u16 y2, u8 colour)
{
	LCD_Draw_Line(x1,y1,x2,y1, colour);
	LCD_Draw_Line(x1,y1,x1,y2, colour);
	LCD_Draw_Line(x1,y2,x2,y2, colour);
	LCD_Draw_Line(x2,y1,x2,y2, colour);
}

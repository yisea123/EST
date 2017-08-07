/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/myfatfs.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#include "myfatfs.h"
#include <string.h>
#include <stdio.h>
#include "sys.h"

/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
FATFS fs;																									/* FatFs文件系统对象 */
FIL file1,file2,file3;																		/* 文件对象*/  							//二进制、图片、声音
FRESULT res_flash;                												/* 文件操作结果 */
UINT fnum;            					  												/* 文件成功读写数量 */
u8 fpath[32];                  														/* 保存当前扫描路径 */
u8 oldfpath[32];                 													/* 保存上一次扫描路径 */

u8 myfilename[256]={0};																		/* 从汇编指令或屏幕选择获取当前文件名 */

//屏幕显示信息扫描
u8 window_top=0;																					//窗口顶部坐标
u8 file_name_num=0;																				//当前路径文件数
le_file file_load[file_max];															/* 保存当前目录数或者文件数 */ //一个路径下最多显示256个文件

//文件系统信息上传PC
u8 allfile[256][64];
float allfilesize[256];
u8 allfile_name_num=0;																		//文件系统里全部文件数

//图片、声音文件缓存
uint8_t PICTURE_Buff[1024*3];															//PICTURE缓存

//uint8_t SOUND_Buff[1024*3];																//SOUND缓存
u32 SOUND_Buff_index=0;
u8 SOUND_Buff_flag=0;
//u8 SOUND_Buff_frame=0;
u8 SOUND_UI=0;

u32 buttonsound_index=0;

u32 downloadsound_index=0;
u8 downloadsound_flag=0;

//测试
BYTE myReadBuffer[1024]={0};      												/* 读缓冲区 */
BYTE myWriteBuffer[] =            												/* 写缓冲区*/
"乐博士文件系统测试1\r\n";

//char last_path[256][256];//上一级字符串 ？如何保存
//char mypath[256];//当前路径字符串 ？字符串长度
//unsigned char level = 0;//文件夹层数
//unsigned char level_max = 255;//文件夹最大层数
//char scanned = 0;//已经扫描过

 //文件类型列表
#define FILE_MAX_TYPE_NUM		5															//最多FILE_MAX_TYPE_NUM个大类
u8*const FILE_TYPE_TBL[FILE_MAX_TYPE_NUM]=
{
		{"dbf"},																							//十六进制执行文件	0x01
		{"dpf"},																							//图片文件					0x02
		{"mp3"},																							//声音文件					0x04
		{"ddf"},																							//波形文件					0x08
		{"dtf"},																							//文本文件					0x10
};
u8 f_typetell(char *fname);

/**
  * @brief  scan_all_files 递归扫描FatFs内的文件
  * @param  path:初始扫描路径
  * @retval result:文件系统的返回值
  */
FRESULT scan_all_files (char* path) 
{ 
  FRESULT res; 		//部分在递归过程被修改的变量，不用全局变量	
  FILINFO fno; 
  DIR dir; 
  int i,j;            
  char *fn;        // 文件名	
	
#if _USE_LFN 
  /* 长文件名支持 */
  /* 简体中文需要2个字节保存一个“字”*/
  static char lfn[_MAX_LFN*2 + 1]; 	
  fno.lfname = lfn; 
  fno.lfsize = sizeof(lfn); 
#endif 
  //打开目录
  res = f_opendir(&dir, path); 
  if (res == FR_OK) 
	{ 
    i = strlen(path); 
    for (j=0;;j++) 
		{ 
      //读取目录下的内容，再读会自动读下一个文件
      res = f_readdir(&dir, &fno); 								
      //为空时表示所有项目读取完毕，跳出
      if (res != FR_OK || fno.fname[0] == 0) break; 	
#if _USE_LFN 
      fn = *fno.lfname ? fno.lfname : fno.fname; 
#else 
      fn = fno.fname; 
#endif 
      //点表示当前目录，跳过			
      if (*fn == '.') continue; 	
      //目录，递归读取      
      if (fno.fattrib & AM_DIR)         
			{ 			
        //合成完整目录名        
        sprintf(&path[i], "/%s", fn); 		
        //递归遍历         
        res = scan_all_files(path);	
        path[i] = 0;         
        //打开失败，跳出循环        
        if (res != FR_OK) 
					break; 
      } 
			else 
			{			
				sprintf(&allfile[allfile_name_num][strlen(allfile[allfile_name_num])], "%s",path); 
				sprintf(&allfile[allfile_name_num][strlen(allfile[allfile_name_num])], "/%s",fn); 
				allfilesize[allfile_name_num]=fno.fsize;
				allfile_name_num++;
//				printf("%s/%s--%d\r\n", path, fn, (int)fno.fsize);
				
//				printf("%s/%s\r\n", path, fn);								//输出文件名	
        /* 可以在这里提取特定格式的文件路径 */        
      }//else
    } //for
  } 
  return res; 
}

//当前路径文件前后位置倒置，
void inversion_file_load(void)
{
	u8 i;
	
	for(i=0; i<file_name_num/2; i++)
	{
		strcpy((char *)file_load[file_name_num].path,(char *)file_load[i].path);
		strcpy((char *)file_load[file_name_num].name,(char *)file_load[i].name);
		file_load[file_name_num].type = file_load[i].type;
		file_load[file_name_num].dir = file_load[i].dir;
		
		strcpy((char *)file_load[i].path,(char *)file_load[file_name_num-1-i].path);
		strcpy((char *)file_load[i].name,(char *)file_load[file_name_num-1-i].name);
		file_load[i].type = file_load[file_name_num-1-i].type;
		file_load[i].dir = file_load[file_name_num-1-i].dir;
		
		strcpy((char *)file_load[file_name_num-1-i].path,(char *)file_load[file_name_num].path);
		strcpy((char *)file_load[file_name_num-1-i].name,(char *)file_load[file_name_num].name);
		file_load[file_name_num-1-i].type = file_load[file_name_num].type;
		file_load[file_name_num-1-i].dir = file_load[file_name_num].dir;
		
	}
//	printf("\n\r");
//	for(i=0; i<file_name_num; i++)
//	{
//		printf("%d:%s\n\r",i,file_load[i].name);
//	}
}

/**
  * @brief  scan_files 递归扫描FatFs内的文件
  * @param  path:初始扫描路径
  * @retval result:文件系统的返回值
  */
	FRESULT scan_files (char* path, u8 mode) //0:主界面的0号，扫描dbf文件   1:主界面的1号，扫描目录和文件
{ 
  FRESULT res; 		//部分在递归过程被修改的变量，不用全局变量	
  FILINFO fno; 
  DIR dir; 
  int i,j;            
  char *fn;        // 文件名	
	
#if _USE_LFN 
  /* 长文件名支持 */
  /* 简体中文需要2个字节保存一个“字”*/
  static char lfn[_MAX_LFN*2 + 1]; 	
  fno.lfname = lfn; 
  fno.lfsize = sizeof(lfn); 
#endif 
  //打开目录
  res = f_opendir(&dir,path); 
  if (res == FR_OK) 
	{ 
    i = strlen(path); 
    for (j=0;;j++) 
		{ 
      //读取目录下的内容，再读会自动读下一个文件
      res = f_readdir(&dir, &fno); 								
      //为空时表示所有项目读取完毕，跳出
      if (res != FR_OK || fno.fname[0] == 0) 
			{
				res = f_closedir(&dir); 
				break;
			}
#if _USE_LFN 
      fn = *fno.lfname ? fno.lfname : fno.fname; 
#else 
      fn = fno.fname; 
#endif 
      //点表示当前目录，跳过			
      if (*fn == '.') continue; 	

			if(mode == 1)
			{
				if (fno.fattrib & AM_DIR)         
				{
					//合成完整目录名        
					sprintf(&path[i], "/%s", fn); 	
					strcpy((char *)file_load[file_name_num].name,fn);								//获取目录	
					strcpy((char *)file_load[file_name_num].path,path);
					file_load[file_name_num].dir = 1;
					file_load[file_name_num].type = 0;
					file_name_num++;
					path[i] = 0; 
        
					//打开失败，跳出循环        
					if (res != FR_OK) 
						break; 	
				}
				else 
				{
					/* 可以在这里提取特定格式的文件路径 */  
					strcpy((char *)file_load[file_name_num].path,path);					
					strcpy((char *)file_load[file_name_num].name,fn);										//获取文件
					file_load[file_name_num].dir = 0;
					file_load[file_name_num].type = f_typetell(file_load[file_name_num].name);	
					sprintf(&file_load[file_name_num].name[strlen(file_load[file_name_num].name)-4], "%s",' '); 			
					file_name_num++;
				}//else
			}else if(mode == 0)
			{
				//目录，递归读取      
				if (fno.fattrib & AM_DIR)         
				{ 			
					//合成完整目录名        
					sprintf(&path[i], "/%s", fn); 		
					//递归遍历         
					res = scan_files(path,0);	
					path[i] = 0;         
					//打开失败，跳出循环        
					if (res != FR_OK) 
						break; 
				} 
				else 
				{ 
					if(f_typetell(fn) == 0x01)
					{
						strcpy((char *)file_load[file_name_num].path,path);	
						strcpy((char *)file_load[file_name_num].name,fn);									//输出文件名	
						file_load[file_name_num].dir = 0;
						file_load[file_name_num].type = 0x01;
						sprintf(&file_load[file_name_num].name[strlen(file_load[file_name_num].name)-4], "%s",' '); 
						file_name_num++;
					}			
				}//else
			}
    } //for
  } 
  return res; 
}

//显示剩余容量
//drv:盘符
//返回值:剩余容量(字节)
u32 mf_showfree(u8 *drv, u32 *total)
{
	FATFS *fs1;
	u8 res;
  u32 fre_clust=0, fre_sect=0, tot_sect=0;
  //得到磁盘信息及空闲簇数量
  res = f_getfree((const TCHAR*)drv,(DWORD*)&fre_clust, &fs1);	
  if(res==0)
	{											   
	    tot_sect = (fs1->n_fatent - 2) * fs1->csize;//得到总扇区数
	    fre_sect = fre_clust * fs1->csize;			//得到空闲扇区数	   
		
#if _MAX_SS!=512
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
//		if(tot_sect<20480)//总容量小于10M
//		{
		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
		
//		    printf("\r\n磁盘总容量:%d KB\r\n"
//		           "可用空间:%d KB\r\n",
//		           tot_sect>>1,fre_sect>>1);
		
//		}else
//		{
//		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
//		    printf("\r\n磁盘总容量:%d MB\r\n"
//		           "可用空间:%d MB\r\n",
//		           tot_sect>>11,fre_sect>>11);
//		}
			(*total) = tot_sect>>1;
	}
	return (fre_sect>>1);
}

//将大写字母转为小写字母,如果是数字,则保持不变.
u8 char_lower(u8 c)
{
	if(c<'A')return c;//数字,保持不变.
	
	if(c>='A'&&c<='Z')return c+0x20;//变为小写.
		else return c;//小写,保持不变
}	

//报告文件的类型
//fname:文件名
//返回值:0XFF,表示无法识别的文件类型编号.
//		 其他,高四位表示所属大类,低四位表示所属小类.
u8 f_typetell(char *fname)
{
	
	u8 tbuf[5];
	char *attr='\0';//后缀名
	u8 i=0;
	while(i<250)
	{
		i++;
		if(*fname=='\0')break;//偏移到了最后了.
		fname++;
	}
	if(i==250)return 0XFF;//错误的字符串.
 	for(i=0;i<5;i++)//得到后缀名
	{
		fname--;
		if(*fname=='.')
		{
			fname++;
			attr=fname;
			break;
		}
  }
	strcpy((char *)tbuf,(const char*)attr);//copy
 	for(i=0;i<4;i++)tbuf[i]=char_lower(tbuf[i]);//全部变为小写 
	for(i=0;i<FILE_MAX_TYPE_NUM;i++)	//对比
	{
		if(*FILE_TYPE_TBL[i]==0)break;//此组已经没有可对比的成员了.
		if(strcmp((const char *)FILE_TYPE_TBL[i],(const char *)tbuf)==0)//找到了
		{
			return 0x01<<i;
		}
	}
	return 0XFF;//没找到		 			   
}	 

//删除文件夹及其子路径文件
FRESULT f_deldir(const TCHAR *path)
{
    FRESULT res;
    DIR   dir;     /* 文件夹属对象 */ //36  bytes
    FILINFO fno;   /* 文件属性 */   //32  bytes
    TCHAR file[_MAX_LFN + 2] = {0};
#if _USE_LFN
    TCHAR lname[_MAX_LFN + 2] = {0};
#endif
    
#if _USE_LFN
    fno.lfsize = _MAX_LFN;
    fno.lfname = lname;    //必须赋初值
#endif
    //打开文件夹
    res = f_opendir(&dir, path);
    
    //持续读取文件夹内容
    while((res == FR_OK) && (FR_OK == f_readdir(&dir, &fno)))
    {
        //若是"."或".."文件夹，跳过
        if(0 == strlen(fno.fname))          break;      //若读到的文件名为空
        if(0 == strcmp(fno.fname, "."))     continue;   //若读到的文件名为当前文件夹
        if(0 == strcmp(fno.fname, ".."))    continue;   //若读到的文件名为上一级文件夹
        
        memset(file, 0, sizeof(file));
#if _USE_LFN
        sprintf((char*)file, "%s/%s", path, (*fno.lfname) ? fno.lfname : fno.fname);
#else
        sprintf((char*)file, "%s/%s", path, fno.fname);
#endif
        if (fno.fattrib & AM_DIR)
        {//若是文件夹，递归删除
            res = f_deldir(file);
        }
        else
        {//若是文件，直接删除
            res = f_unlink(file);
        }
    }
    
    //删除本身
    if(res == FR_OK)    res = f_unlink(path);
    
    return res;
}

//文件系统测试
void filetest(u8 button)
{
	u8 i;
	u32 total;
//	FATFS *pfs;
//	FILINFO fno;
	DIR dir;
//  DWORD fre_clust, fre_sect, tot_sect;
	
//	printf("button:%x\r\n",button);
	
	switch(button)
	{
		case 1:                                                                  //扫描文件
			printf("%s \r\n", fpath);
		
			strcpy(fpath,"1:");
			for(i=0; i<file_name_num; i++)
			{
				strcpy(file_load[i].name,"");
				file_load[i].dir=0;
				file_load[i].type=0;
			}
			file_name_num = 0;
			scan_files(fpath,0);
		
			printf("%s \r\n", fpath);
		
//			sprintf(&fpath[strlen(fpath)], "/%s", file_load[0].name); 
//			scan_files(fpath,1);
//		
//			printf("%s \r\n", fpath);
			break;
		
		case 2:                                                                  //FLASH容量
			mf_showfree("1:",&total);
			break;
		
		case 3:                                                                  //删除文件
			res_flash = f_unlink ("1:Project/乐博士测试文件.dbf");
			if(res_flash==FR_OK)
			{
				printf("》删除成功：\n");
			}else
			{
				printf("》删除失败。\r\n");
			}
			break;
		
		case 4:                                                                  //重命名
			res_flash=f_rename("1:Project/乐博士测试文件3.dsf","1:Project/乐博士测试文件3.mp3");      
			if(res_flash==FR_OK)
			{
				printf("》重命名文件成功。\r\n");
			}else
			{
				printf("》重命名文件失败。\r\n");
			}
			break;
		
		case 5:                                                                  //

			break;
		
		case 6:                                                                  //写入文件
			res_flash = f_open(&file1, "1:Project/乐博士测试文件1.dbf",FA_CREATE_ALWAYS | FA_WRITE );
			if ( res_flash == FR_OK )
			{
				printf("》打开/创建文件成功，向文件写入数据。\r\n");
				/* 将指定存储区内容写入到文件内 */
				res_flash=f_write(&file1,myWriteBuffer,sizeof(myWriteBuffer),&fnum);
				if(res_flash==FR_OK)
				{
					printf("》文件写入成功，写入字节数据：%d\n",fnum);
					printf("》向文件写入的数据为：\r\n%s\r\n",myWriteBuffer);
				}
				else
				{
					printf("！！文件写入失败：(%d)\n",res_flash);
				}    
				/* 不再读写，关闭文件 */
				f_close(&file1);
			}
			else
			{	
				printf("！！打开/创建文件失败。\r\n");
			}
			res_flash = f_open(&file1, "1:Project/乐博士测试文件2.dpf",FA_CREATE_ALWAYS | FA_WRITE );
			if ( res_flash == FR_OK )
			{
				printf("》打开/创建文件成功，向文件写入数据。\r\n");
				/* 将指定存储区内容写入到文件内 */
				res_flash=f_write(&file1,myWriteBuffer,sizeof(myWriteBuffer),&fnum);
				if(res_flash==FR_OK)
				{
					printf("》文件写入成功，写入字节数据：%d\n",fnum);
					printf("》向文件写入的数据为：\r\n%s\r\n",myWriteBuffer);
				}
				else
				{
					printf("！！文件写入失败：(%d)\n",res_flash);
				}    
				/* 不再读写，关闭文件 */
				f_close(&file1);
			}
			else
			{	
				printf("！！打开/创建文件失败。\r\n");
			}
			res_flash = f_open(&file1, "1:Project/乐博士测试文件3.mp3",FA_CREATE_ALWAYS | FA_WRITE );
			if ( res_flash == FR_OK )
			{
				printf("》打开/创建文件成功，向文件写入数据。\r\n");
				/* 将指定存储区内容写入到文件内 */
				res_flash=f_write(&file1,myWriteBuffer,sizeof(myWriteBuffer),&fnum);
				if(res_flash==FR_OK)
				{
					printf("》文件写入成功，写入字节数据：%d\n",fnum);
					printf("》向文件写入的数据为：\r\n%s\r\n",myWriteBuffer);
				}
				else
				{
					printf("！！文件写入失败：(%d)\n",res_flash);
				}    
				/* 不再读写，关闭文件 */
				f_close(&file1);
			}
			else
			{	
				printf("！！打开/创建文件失败。\r\n");
			}
			res_flash = f_open(&file1, "1:Project/乐博士测试文件4.dbf",FA_CREATE_ALWAYS | FA_WRITE );
			if ( res_flash == FR_OK )
			{
				printf("》打开/创建文件成功，向文件写入数据。\r\n");
				/* 将指定存储区内容写入到文件内 */
				res_flash=f_write(&file1,myWriteBuffer,sizeof(myWriteBuffer),&fnum);
				if(res_flash==FR_OK)
				{
					printf("》文件写入成功，写入字节数据：%d\n",fnum);
					printf("》向文件写入的数据为：\r\n%s\r\n",myWriteBuffer);
				}
				else
				{
					printf("！！文件写入失败：(%d)\n",res_flash);
				}    
				/* 不再读写，关闭文件 */
				f_close(&file1);
			}
			else
			{	
				printf("！！打开/创建文件失败。\r\n");
			}
			res_flash = f_open(&file1, "1:Project/乐博士测试文件5.dbf",FA_CREATE_ALWAYS | FA_WRITE );
			if ( res_flash == FR_OK )
			{
				printf("》打开/创建文件成功，向文件写入数据。\r\n");
				/* 将指定存储区内容写入到文件内 */
				res_flash=f_write(&file1,myWriteBuffer,sizeof(myWriteBuffer),&fnum);
				if(res_flash==FR_OK)
				{
					printf("》文件写入成功，写入字节数据：%d\n",fnum);
					printf("》向文件写入的数据为：\r\n%s\r\n",myWriteBuffer);
				}
				else
				{
					printf("！！文件写入失败：(%d)\n",res_flash);
				}    
				/* 不再读写，关闭文件 */
				f_close(&file1);
			}
			else
			{	
				printf("！！打开/创建文件失败。\r\n");
			}
			res_flash = f_open(&file1, "1:Project/乐博士测试文件6.dbf",FA_CREATE_ALWAYS | FA_WRITE );
			if ( res_flash == FR_OK )
			{
				printf("》打开/创建文件成功，向文件写入数据。\r\n");
				/* 将指定存储区内容写入到文件内 */
				res_flash=f_write(&file1,myWriteBuffer,sizeof(myWriteBuffer),&fnum);
				if(res_flash==FR_OK)
				{
					printf("》文件写入成功，写入字节数据：%d\n",fnum);
					printf("》向文件写入的数据为：\r\n%s\r\n",myWriteBuffer);
				}
				else
				{
					printf("！！文件写入失败：(%d)\n",res_flash);
				}    
				/* 不再读写，关闭文件 */
				f_close(&file1);
			}
			else
			{	
				printf("！！打开/创建文件失败。\r\n");
			}
			res_flash = f_open(&file1, "1:Project/乐博士测试文件7.dbf",FA_CREATE_ALWAYS | FA_WRITE );
			if ( res_flash == FR_OK )
			{
				printf("》打开/创建文件成功，向文件写入数据。\r\n");
				/* 将指定存储区内容写入到文件内 */
				res_flash=f_write(&file1,myWriteBuffer,sizeof(myWriteBuffer),&fnum);
				if(res_flash==FR_OK)
				{
					printf("》文件写入成功，写入字节数据：%d\n",fnum);
					printf("》向文件写入的数据为：\r\n%s\r\n",myWriteBuffer);
				}
				else
				{
					printf("！！文件写入失败：(%d)\n",res_flash);
				}    
				/* 不再读写，关闭文件 */
				f_close(&file1);
			}
			else
			{	
				printf("！！打开/创建文件失败。\r\n");
			}
			res_flash = f_open(&file1, "1:Project/乐博士测试文件8.dbf",FA_CREATE_ALWAYS | FA_WRITE );
			if ( res_flash == FR_OK )
			{
				printf("》打开/创建文件成功，向文件写入数据。\r\n");
				/* 将指定存储区内容写入到文件内 */
				res_flash=f_write(&file1,myWriteBuffer,sizeof(myWriteBuffer),&fnum);
				if(res_flash==FR_OK)
				{
					printf("》文件写入成功，写入字节数据：%d\n",fnum);
					printf("》向文件写入的数据为：\r\n%s\r\n",myWriteBuffer);
				}
				else
				{
					printf("！！文件写入失败：(%d)\n",res_flash);
				}    
				/* 不再读写，关闭文件 */
				f_close(&file1);
			}
			else
			{	
				printf("！！打开/创建文件失败。\r\n");
			}
			break;
		
		case 7:                                                                  //读取文件
				res_flash = f_open(&file1, "1:Project/乐博士测试文件.dbf", FA_OPEN_EXISTING | FA_READ); 	 
				if(res_flash == FR_OK)
				{
					printf("》打开文件成功。\r\n");
					res_flash = f_read(&file1, myReadBuffer,  file1.fsize, &fnum); 
					if(res_flash==FR_OK)
					{
						printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
						printf("》读取得的文件数据为：\r\n%s \r\n", myReadBuffer);	
					}
					else
					{
						printf("！！文件读取失败：(%d)\n",res_flash);
					}		
					/* 不再读写，关闭文件 */
					f_close(&file1);	
				}
				else
				{
					printf("！！打开文件失败。\r\n");
				}
			break;
		
		case 8:                                                                  //创建目录
			/* 尝试打开目录 */
			res_flash=f_opendir(&dir,"1:Project");
			if(res_flash!=FR_OK)
			{
				/* 打开目录失败，创建目录 */
				res_flash=f_mkdir("1:Project");
				printf("》打开目录失败，就创建目录：\n");
			}
			else
			{
				/* 如果目录已经存在，关闭它 */
				res_flash=f_closedir(&dir);
				printf("》目录已存在，关闭目录：\n");
			}
			/* 尝试打开目录 */
			res_flash=f_opendir(&dir,"1:Project1");
			if(res_flash!=FR_OK)
			{
				/* 打开目录失败，创建目录 */
				res_flash=f_mkdir("1:Project1");
				printf("》打开目录失败，就创建目录：\n");
			}
			else
			{
				/* 如果目录已经存在，关闭它 */
				res_flash=f_closedir(&dir);
				printf("》目录已存在，关闭目录：\n");
			}
			/* 尝试打开目录 */
			res_flash=f_opendir(&dir,"1:Project2");
			if(res_flash!=FR_OK)
			{
				/* 打开目录失败，创建目录 */
				res_flash=f_mkdir("1:Project2");
				printf("》打开目录失败，就创建目录：\n");
			}
			else
			{
				/* 如果目录已经存在，关闭它 */
				res_flash=f_closedir(&dir);
				printf("》目录已存在，关闭目录：\n");
			}
			/* 尝试打开目录 */
			res_flash=f_opendir(&dir,"1:Project3");
			if(res_flash!=FR_OK)
			{
				/* 打开目录失败，创建目录 */
				res_flash=f_mkdir("1:Project3");
				printf("》打开目录失败，就创建目录：\n");
			}
			else
			{
				/* 如果目录已经存在，关闭它 */
				res_flash=f_closedir(&dir);
				printf("》目录已存在，关闭目录：\n");
			}
			/* 尝试打开目录 */
			res_flash=f_opendir(&dir,"1:Project4");
			if(res_flash!=FR_OK)
			{
				/* 打开目录失败，创建目录 */
				res_flash=f_mkdir("1:Project4");
				printf("》打开目录失败，就创建目录：\n");
			}
			else
			{
				/* 如果目录已经存在，关闭它 */
				res_flash=f_closedir(&dir);
				printf("》目录已存在，关闭目录：\n");
			}/* 尝试打开目录 */
			res_flash=f_opendir(&dir,"1:Project5");
			if(res_flash!=FR_OK)
			{
				/* 打开目录失败，创建目录 */
				res_flash=f_mkdir("1:Project5");
				printf("》打开目录失败，就创建目录：\n");
			}
			else
			{
				/* 如果目录已经存在，关闭它 */
				res_flash=f_closedir(&dir);
				printf("》目录已存在，关闭目录：\n");
			}/* 尝试打开目录 */
			res_flash=f_opendir(&dir,"1:Project6");
			if(res_flash!=FR_OK)
			{
				/* 打开目录失败，创建目录 */
				res_flash=f_mkdir("1:Project6");
				printf("》打开目录失败，就创建目录：\n");
			}
			else
			{
				/* 如果目录已经存在，关闭它 */
				res_flash=f_closedir(&dir);
				printf("》目录已存在，关闭目录：\n");
			}
			break;
		
		case 9:                                                                  //格式化		
			printf("》正在格式化文件系统。\r\n");
			res_flash=f_mkfs("1:",0,0);							
			if(res_flash == FR_OK)
			{
				printf("》FLASH已成功格式化文件系统。\r\n");
				/* 格式化后，先取消挂载 */
				res_flash = f_mount(NULL,"1:",1);			
				/* 重新挂载	*/			
				res_flash = f_mount(&fs,"1:",1);
				printf("》重新挂载文件系统。\r\n");
			}else
			{
				printf("》FLASH格式化文件系统失败。\r\n");
			}
			break;
		
		default:
			break;
	}
}



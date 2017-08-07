/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/myfatfs.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __MYFATFS_H
#define __MYFATFS_H

#include "ff.h"
#include "sys.h"

#define file_max 256

/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
extern FATFS fs;								/* FatFs文件系统对象 */
extern FIL file1,file2,file3;		/* 文件对象 */
extern FRESULT res_flash;				/* 文件操作结果 */
extern UINT fnum;								/* 文件成功读写数量 */
//extern BYTE ReadBuffer[];				/* 读缓冲区 */
//extern BYTE WriteBuffer[];			/* 写缓冲区*/
extern u8 fpath[32];					/* 保存当前扫描路径 */
extern u8 oldfpath[32];                  /* 保存当前扫描路径 */

extern u8 allfile[256][64];
extern float allfilesize[256];
extern u8 allfile_name_num;			//全部文件数

extern u8 window_top;					//窗口顶部坐标
extern u8 file_name_num;			//文件数

typedef struct 
{
  u8 path[32];					//文件路径
	u8 name[32];			//文件名
	u8 type;					//0：无、1：二进制文件、2：图片文件、3：声音文件
	u8 dir;						//0：非目录 1：目录
}le_file;
extern le_file file_load[];//一个路径下最多显示256个文件

extern uint8_t PICTURE_Buff[1024*3];					//PICTURE缓存
//extern uint8_t SOUND_Buff[1024*3];					//SOUND缓存
extern u32 buttonsound_index;
extern u32 SOUND_Buff_index;
extern u8 SOUND_Buff_flag;
//extern u8 SOUND_Buff_frame;
extern u8 SOUND_UI;

extern u32 downloadsound_index;
extern u8 downloadsound_flag;

extern u8 myfilename[];

extern void filetest(u8 button);
extern u32 mf_showfree(u8 *drv, u32 *total);
extern FRESULT scan_all_files(char* path); 
extern FRESULT scan_files(char* path, u8 mode);
extern FRESULT f_deldir(const TCHAR *path);
extern void inversion_file_load(void);

#endif


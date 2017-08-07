/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/iwdg.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief  
  ******************************************************************************
**/
#ifndef _IWDG_H
#define _IWDG_H
#include "sys.h"

void IWDG_Init(u8 prer,u16 rlr);//IWDG³õÊ¼»¯
void IWDG_Feed(void);  //Î¹¹·º¯Êý
#endif

#ifndef __IAP_H_
#define __IAP_H_

#include "stm32f429i_discovery.h"
#include "flash.h"
#include "sys.h"
#include "delay.h"
#include "flash_if.h"
#include "uart5.h"

#define APP_CONFIG_ADDR 		0X08008010	//≈‰÷√µÿ÷∑£¨“≥1
#define APP_CONFIG_SET_VALUE	0X5555		//…Ë÷√÷µ
#define APP_CONFIG_CLEAR_VALUE	0X0000	//«Â¡„÷µ
#define APP_CONFIG_NONE_VALUE	0XFFFF		//≥ı º≈‰÷√÷µ

#define Update_Data_Length_L16 		0x08008000	//≈‰÷√µÿ÷∑£¨“≥2
#define Update_Data_Length_H16 		0x08008008	//≈‰÷√µÿ÷∑£¨“≥2

void iap_jump_app_s(void);

void App_Update_Check(void);

#endif




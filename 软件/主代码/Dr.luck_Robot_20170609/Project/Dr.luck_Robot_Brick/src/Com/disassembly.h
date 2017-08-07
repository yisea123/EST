/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/disassembly.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __DISASSEMBLY_H
#define __DISASSEMBLY_H

#include "sys.h"

#define instructsize		128
#define Flash_Buffsize  64*1024
#define RAM_Buffsize		8*1024
#define ThreadSize			64
#define TimerSize				8

extern uint8_t programflag;
extern unsigned int Flash_Buff_Length;
extern uint8_t flashbuff_flag;

extern uint8_t Flash_Buff[Flash_Buffsize];	//Flash存放program空间
//extern uint16_t flash_base;								//用户flash地址
extern u8 Flash_Buff_frame;

extern uint8_t RAM_Buff[RAM_Buffsize];			//RAM存放变量空间
extern uint32_t ram_base;										//用户ram地址

typedef	void	(*PRIM)(void);      					//!< Prototype for all byte codes

extern	PRIM	Byte_code_excute[instructsize];
extern const uint8_t InstructParsMap[];

typedef struct
{
  uint32_t Threadpc;
  uint8_t state;//0:closed	1:opened	2:waiting
	uint32_t timetmp;
}THREAD;

extern THREAD Thread[ThreadSize];						//用户程序指针
extern uint32_t ThreadNow;
extern uint32_t pc;

typedef struct
{
  uint16_t time;
}TIMER;
extern TIMER Timer[TimerSize];

typedef   enum
{
	NOP												= 0x00,	

	//	//				赋值指令						
	MOV_C_BYTE								= 0x01,
	MOV_C_FLOAT								= 0x02,
	MOV_R_BYTE								= 0x03,
	MOV_R_FLOAT								= 0x04,
	MOV_C_TO_R								= 0x05,
	MOV_R_TO_R								= 0x06,
	CHANGE_FLOAT_TO_BYTE			= 0x07,
	CHANGE_BYTE_TO_FLOAT			= 0x08,
	
	//	//				计算指令
	ADD												= 0x09,
	SUB												= 0x0A,
	MUL												= 0x0B,
	DIV												= 0x0C,
	MATH											= 0x0D,

	RAN_F											= 0x0E,
	RAN_B											= 0x0F,
	
	//	//				比较指令	
	COMPARE										= 0x10,
	
	//				逻辑指令	
	AND												= 0x11,
	OR												= 0x12,
	NOT												= 0x13,
	XOR												= 0x14,
	
	AND_2b										= 0x15,
	OR_2b											= 0x16,
	NOT_2b										= 0x17,
	XOR_2b										= 0x18,

	//	//				跳转指令
	JMP												= 0x19,
	CJMP_TRUR									= 0x1A,
	CJMP_FALSE								= 0x1B,
	DJMP											= 0x1C,
	//	
	//	//				线程操作指令
	THREAD_START							= 0x1D,
	THREAD_True_START         = 0x1E,
	THREAD_False_START        = 0x1F,
	THREAD_IF									= 0x20,
	THREAD_END								= 0x21,
	
	//	//				控制指令
	STARTPROGRAM							= 0x22,
	DELAY											= 0x23,
	ENDPROGRAM								= 0x24,
							
	GET_PROGRAM_TIME					= 0x25,
	GET_TIMER									= 0x26,
	RESET_TIME								= 0x27,
	
	NOP1											= 0x28,
	NOP2											= 0x29,
	NOP3											= 0x2A,
	NOP4											= 0x2B,
	NOP5											= 0x2C,
	NOP6											= 0x2D,
	NOP7											= 0x2E,
	NOP8											= 0x2F,
	
	//	//				端口指令
	READ_PORT									= 0x30,
	WRITE_PORT								= 0x31,
	READ_DEVICE								= 0x32,
	WRITE_DEVICE							= 0x33,
							
	//	//				文件操作指令
	FILENAME									= 0x34,
	FILE_											= 0x35,
	
	//	//				字符串操作指令
	STRING										= 0x36,
}
instruct;

#endif 

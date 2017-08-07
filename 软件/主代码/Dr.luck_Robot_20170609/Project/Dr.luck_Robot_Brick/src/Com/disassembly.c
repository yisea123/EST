/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/disassembly.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/

#include "disassembly.h"

#define CCMRAM __attribute__((section("ccmram")))

uint8_t programflag = 0; // 0:系统正在运行 1:用户程序正在运行 2:正在下载工程
unsigned int Flash_Buff_Length = 0;
uint8_t flashbuff_flag = 0;

CCMRAM uint8_t Flash_Buff[Flash_Buffsize];			//Flash存放program空间
//uint16_t flash_base;									//用户flash地址
u8 Flash_Buff_frame=0;

uint8_t RAM_Buff[RAM_Buffsize];					//RAM存放变量空间
uint32_t ram_base;											//用户ram地址

THREAD Thread[ThreadSize];							//用户程序指针，最多65536个线程
uint32_t ThreadNow;
uint32_t pc = 0x00000000;

TIMER Timer[TimerSize];									//用户8个定时器

//内存操作函数
uint32_t Getpc(uint8_t par1, uint8_t par2, uint8_t par3, uint8_t par4);
uint32_t GetRamBase(uint8_t par1, uint8_t par2, uint8_t par3, uint8_t par4);

uint8_t GetByte(uint32_t rambase);
float GetFloat(uint32_t rambase);
void SetByte(uint32_t rambase, uint8_t par);
void SetFloat(uint32_t rambase, float par);

//汇编指令函数
extern void OpNop(void);
extern void OpMov_C_Byte(void);
extern void OpMov_C_Floart(void);
extern void OpMov_R_Byte(void);
extern void OpMov_R_Floart(void);
extern void OpMov_C_To_R(void);
extern void OpMov_R_To_R(void);
extern void OpChange_Float_To_Byte(void);
extern void OpChange_Byte_To_Float(void);

extern void OpAdd(void);
extern void OpSub(void);
extern void OpMUL(void);
extern void OpDiv(void);
extern void OpMath(void);

extern void OpRan_f(void);
extern void OpRan_b(void);

extern void OpCompare(void);

extern void OpAnd(void);
extern void OpOr(void);
extern void OpNot(void);
extern void OpXor(void);
extern void OpAnd_2b(void);
extern void OpOr_2b(void);
extern void OpNot_2b(void);
extern void OpXor_2b(void);

extern void OpJmp(void);
extern void OpJmp_True(void);
extern void OpJmp_False(void);
extern void OpDjmp(void);

extern void OpThread_Start(void);
extern void OpThread_True_Start(void);
extern void OpThread_False_Start(void);
extern void OpThread_End_IF(void);
extern void OpThread_End(void);


extern void OpStartprogram(void);
extern void OpDelay(void);
extern void OpEndprogram(void);

extern void OpGet_Time(void);
extern void OpReset_Time(void);

extern void OpRead_Port(void);
extern void OpWrite_Port(void);
extern void OpRead_Device(void);
extern void OpWrite_Device(void);

extern void OpFilename(void);

extern void OpFile(void);

extern void OpString(void);

/*!   
 *	\brief	每条指令的长度
 */
const uint8_t InstructParsMap[instructsize] =
{
	0,				  //	NOP												= 0x00,	

							//	//				赋值指令						
	6,					//	MOV_C_BYTE								= 0x01,
	9,					//	MOV_C_FLOAT								= 0x02,
	9,					//	MOV_R_BYTE								= 0x03,
	9,					//	MOV_R_FLOAT								= 0x04,
	11,					//	MOV_C_TO_R								= 0x05,
	11,					//	MOV_R_TO_R								= 0x06,
	9,					//	CHANGE_FLOAT_TO_BYTE			= 0x07,
	9,					//	CHANGE_BYTE_TO_FLOAT			= 0x08,

							//	//				计算指令
	13,					//	ADD												= 0x09,
	13,					//	SUB												= 0x0A,
	13,					//	MUL												= 0x0B,
	13,					//	DIV												= 0x0C,
	2,					//	MATH											= 0x0D,

	13,					//	RAN_F											= 0x0E,
	9,					//	RAN_B											= 0x0F,
	
							//	//				比较指令	
	2,					//	COMPARE										= 0x10,
	
							//	//				逻辑指令	
	13,					//	AND												= 0x11,
	13,					//	OR												= 0x12,
	9,					//	NOT												= 0x13,
	13,					//	XOR												= 0x14,
	
	13,					//	AND（2Byte）							= 0x15,
	13,					//	OR（2Byte）								= 0x16,
	9,					//	NOT（2Byte）							= 0x17,
	13,					//	XOR（2Byte）							= 0x18,

							//	//				跳转指令
	5,					//	JMP												= 0x19,
	9,					//	CJMP_TRUR									= 0x1A,
	9,					//	CJMP_FALSE								= 0x1B,
	9,					//	DJMP											= 0x1C,
							//	
							//	//				线程操作指令
	7,					//	THREAD_START							= 0x1D,
	11,         //  THREAD_True_START         = 0x1E,
	11,         //  THREAD_False_START        = 0x1F,
	3,					//	THREAD_IF									= 0x20,
	3,					//	THREAD_END								= 0x21,
	
							//	//				控制指令
	1,					//	STARTPROGRAM							= 0x22,
	5,					//	DELAY											= 0x23,
	1,					//	ENDPROGRAM								= 0x24,
							
	5,					//	GET_PROGRAM_TIME					= 0x25,
	6,					//	GET_TIMER									= 0x26,
	2,					//	RESET_TIME								= 0x27,
	
	0,					//	NOP1											= 0x28,
	0,					//	NOP2											= 0x29,
	0,					//	NOP3											= 0x2A,
	0,					//	NOP4											= 0x2B,
	0,					//	NOP5											= 0x2C,
	0,					//	NOP6											= 0x2D,
	0,					//	NOP7											= 0x2E,
	0,					//	NOP8											= 0x2F,
	
							//	//				端口指令
	13,					//	READ_PORT									= 0x30,
	13,					//	WRITE_PORT								= 0x31,
	8,					//	READ_DEVICE								= 0x32,
	8,					//	WRITE_DEVICE							= 0x33,
							
							//	//				文件操作指令
	2,					//	FILENAME									= 0x34,（）
	2,					//	FILE_											= 0x35,（）
	
							//	//				字符串操作指令
	2,					//	STRING										= 0x36,（）
	
};

PRIM	Byte_code_excute[instructsize] =
{
	&OpNop,				  					//	NOP												= 0x00,	

														//	//				赋值指令						
	&OpMov_C_Byte,						//	MOV_C_BYTE								= 0x01,
	&OpMov_C_Floart,					//	MOV_C_FLOAT								= 0x02,
	&OpMov_R_Byte,						//	MOV_R_BYTE								= 0x03,
	&OpMov_R_Floart,					//	MOV_R_FLOAT								= 0x04,
	&OpMov_C_To_R,						//	MOV_C_TO_R								= 0x05,
	&OpMov_R_To_R,						//	MOV_R_TO_R								= 0x06,
	&OpChange_Float_To_Byte,	//	CHANGE_FLOAT_TO_BYTE			= 0x07,
	&OpChange_Byte_To_Float,	//	CHANGE_BYTE_TO_FLOAT			= 0x08,

														//	//				计算指令
	&OpAdd,										//	ADD												= 0x09,
	&OpSub,										//	SUB												= 0x0A,
	&OpMUL,										//	MUL												= 0x0B,
	&OpDiv,										//	DIV												= 0x0C,
	&OpMath,									//	MATH											= 0x0D,（）

	&OpRan_f,									//	RAN_F											= 0x0E,
	&OpRan_b,									//	RAN_B											= 0x0F,
	
														//	//				比较指令	
	&OpCompare,								//	COMPARE										= 0x10,（）
	
														//	//				逻辑指令	
	&OpAnd,										//	AND												= 0x11,
	&OpOr,										//	OR												= 0x12,
	&OpNot,										//	NOT												= 0x13,
	&OpXor,										//	XOR												= 0x14,
	
	&OpAnd_2b,								//	AND（2Byte）							= 0x15,
	&OpOr_2b,									//	OR（2Byte）								= 0x16,
	&OpNot_2b,								//	NOT（2Byte）							= 0x17,
	&OpXor_2b,								//	XOR（2Byte）							= 0x18,

														//	//				跳转指令
	&OpJmp,										//	JMP												= 0x19,
	&OpJmp_True,							//	CJMP_TRUR									= 0x1A,
	&OpJmp_False,							//	CJMP_FALSE								= 0x1B,
	&OpDjmp,									//	DJMP											= 0x1C,
														//	
														//	//				线程操作指令
	&OpThread_Start,					//	THREAD_START							= 0x1D,
	&OpThread_True_Start,     //  THREAD_True_START         = 0x1E,
	&OpThread_False_Start,    //  THREAD_False_START        = 0x1F,
	&OpThread_End_IF,					//	THREAD_IF									= 0x20,
	&OpThread_End,						//	THREAD_END								= 0x21,
	
														//	//				控制指令
	&OpStartprogram,					//	STARTPROGRAM							= 0x22,
	&OpDelay,									//	DELAY											= 0x23,
	&OpEndprogram,						//	ENDPROGRAM								= 0x24,
							
	&OpNop,										//	GET_PROGRAM_TIME					= 0x25,
	&OpGet_Time,							//	GET_TIMER									= 0x26,
	&OpReset_Time,						//	RESET_TIME								= 0x27,
	
	&OpNop,										//	NOP1											= 0x28,
	&OpNop,										//	NOP2											= 0x29,
	&OpNop,										//	NOP3											= 0x2A,
	&OpNop,										//	NOP4											= 0x2B,
	&OpNop,										//	NOP5											= 0x2C,
	&OpNop,										//	NOP6											= 0x2D,
	&OpNop,										//	NOP7											= 0x2E,
	&OpNop,										//	NOP8											= 0x2F,
	
														//	//				端口指令
	&OpRead_Port,							//	READ_PORT									= 0x30,
	&OpWrite_Port,						//	WRITE_PORT								= 0x31,
	&OpRead_Device,						//	READ_DEVICE								= 0x32,
	&OpWrite_Device,					//	WRITE_DEVICE							= 0x33,
											
														//	//				文件操作指令
	&OpNop,												//	FILENAME									= 0x34,（取最小值）
	&OpNop,												//	FILE											= 0x35,（取最小值）
	
														//	//				字符串操作指令
	&OpNop,												//	STRING										= 0x36,（取最小值）
};


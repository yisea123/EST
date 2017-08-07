/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/excutefunction.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __EXCUTEFUNCTION_H
#define __EXCUTEFUNCTION_H

#include "sys.h"

extern uint32_t Getpc(uint8_t par1, uint8_t par2, uint8_t par3, uint8_t par4);
extern uint32_t GetRamBase(uint8_t par1, uint8_t par2, uint8_t par3, uint8_t par4);

extern uint8_t GetByte(uint32_t rambase);
extern float GetFloat(uint32_t rambase);
extern void SetByte(uint32_t rambase, uint8_t par);
extern void SetFloat(uint32_t rambase, float par);

extern void FloatToByte4(float temp, uint8_t byte[],uint8_t len);
extern float Byte4ToFloat(uint8_t byte[], uint8_t len);

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

extern void Exp(void);
extern void Mod(void);
extern void Rej(void);
extern void Inj(void);
extern void Appr(void);
extern void Abs(void);
extern void Negate(void);
extern void Sqrt(void);
extern void Log(void);
extern void Ln(void);
extern void Sin(void);
extern void Cos(void);
extern void Tan(void);
extern void Asin(void);
extern void Acos(void);
extern void Atan(void);
extern void Pow(void);
extern void Trun(void);
extern void Inside(void);
extern void Outside(void);

extern void OpRan_f(void);
extern void OpRan_b(void);

extern void OpCompare(void);
extern void Equ(void);
extern void Nequ(void);
extern void Gt(void);
extern void Gte(void);
extern void Lt(void);
extern void Lte(void);
extern void Select(void);

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

extern void OpGet_Program_Time(void);
extern void OpGet_Time(void);
extern void OpReset_Time(void);

extern void OpRead_Port(void);
extern void OpWrite_Port(void);
extern void OpRead_Device(void);
extern void OpWrite_Device(void);

extern void OpFilename(void);

extern void OpFile(void);
extern void File_Byte(void);
extern void File_picture(void);
extern void File_Sound(void);
extern void File_Datalog(void);
extern void File_Txt(void);
extern void File_close(void);

extern void OpString(void);
extern void Sting_Getsize(void);
extern void Sting_Add(void);
extern void Sting_Compare(void);
extern void Sting_Duplicate(void);
extern void Sting_FloatToString(void);
extern void Sting_StringToFloat(void);
extern void Sting_Strip(void);
extern void Sting_Sub(void);

extern u8 daisy_Write_Port(u8 Port, u8 DeviceId, u8 mode,	u8 bufflength, u8 buff[13]); 		//级联电机控制函数
extern u8 daisy_sensor_control(u8 Port, u8 DeviceId, u8 mode,	u8 bufflength, u8 buff[13]); //级联传感器模式更换函数
extern void deal_daisy_Chain(u8 Port, u8 DeviceId, u8 mode,	u8 bufflength, u8 buff[13]);

#endif


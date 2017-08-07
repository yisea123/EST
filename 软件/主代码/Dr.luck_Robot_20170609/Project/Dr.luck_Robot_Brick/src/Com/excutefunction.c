/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/excutefunction.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "math.h"
#include "outputworkmode.h"
#include "stm32fxxx_it.h"
#include "input.h"
#include "rng.h"
#include "usart6.h"
#include "disassembly.h"
#include "excutefunction.h"
#include "cascade.h"

_ARMABI_FPEXCEPT float roundf(float /*x*/);

//void BriDisplay(uint8_t mode, uint32_t bufferrambase);
//void BriSound(uint8_t mode, uint32_t bufferrambase);
//void BriButton(uint8_t mode, uint32_t bufferrambase);
//void BriLight(uint8_t mode, uint32_t bufferrambase);
//void BriBt(uint8_t mode, uint32_t bufferrambase);
//void BriMessage(uint8_t mode, uint32_t bufferrambase);
//void BriFile(uint8_t mode, uint32_t bufferrambase);
//void BriWaveform(uint8_t mode, uint32_t bufferrambase);
//void BriRawSensor(uint8_t mode, uint32_t bufferrambase);
//void BriTimerSensor(uint8_t mode, uint32_t bufferrambase);		

#define   DegToRad(X)           (X * 0.0174532925f)
#define   RadToDeg(X)           (X * 57.2957795f)

/*!
 *  \param  (uint8_t)			par1   - 程序地址24~31位
 *  \param  (uint8_t)			par2   - 程序地址16~23位
 *  \param  (uint8_t)			par3   - 程序地址8~15位
 *  \param  (uint8_t)			par4   - 程序地址0~7位
 *	\return (uint32_t)		pc地址
 *	\brief  从先高后低的四个Byte,提取程序地址
 */
 uint32_t Getpc(uint8_t par1, uint8_t par2, uint8_t par3, uint8_t par4)
{
	return (((uint32_t)par1<<24) + ((uint32_t)par2<<16) + ((uint32_t)par3<<8) + (((uint32_t)par4)));
}

/*!
 *  \param  (uint8_t)			par1   - 数据地址24~31位
 *  \param  (uint8_t)			par2   - 数据地址16~23位
 *  \param  (uint8_t)			par3   - 数据地址8~15位
 *  \param  (uint8_t)			par4   - 数据地址0~7位
 *	\return (uint32_t)		ram地址
 *	\brief	从先高后低的四个Byte,提取数据地址
 */
uint32_t GetRamBase(uint8_t par1, uint8_t par2, uint8_t par3, uint8_t par4)
{
	return (((uint32_t)par1<<24) + ((uint32_t)par2<<16) + ((uint32_t)par3<<8) + (((uint32_t)par4)));
}

/*!
 *  \param  (uint32_t)	rambase		-ram地址
 *	\return (uint8_t)		Byte数据
 *	\brief	提取一个Byte
 */
uint8_t GetByte(uint32_t rambase)
{
	return RAM_Buff[rambase];
}

/*! 
 *  \param  (uint32_t)	rambase - ram地址
 *	\return (uint32_t)  32位float数据
 *	\brief	从先低后高的四个Byte,提取float
 */
float GetFloat(uint32_t rambase)
{
	float temp;
	uint8_t *p,i;
	p=(uint8_t *)(&temp);
	
	for(i=0; i<4; i++)
	{
		*(p+i) = RAM_Buff[rambase+i];
	}
	
	return temp;
}

/*! 
 *  \param  (uint32_t)    rambase		ram地址
 *  \param  (uint8_t)     par     	Byte数据
 *  \return	
 *	\brief	写入Byte
 */
void SetByte(uint32_t rambase, uint8_t par)
{
	RAM_Buff[rambase] = par;
}

/*! 
 *  \param  (uint32_t)    rambase		ram地址
 *  \param  (float)    		par     	float数据
 *  \return	
 *	\brief	从先低后高写入四个Byte, float型
 */
void SetFloat(uint32_t rambase, float par)
{
	uint8_t *p,i;
	p=(uint8_t *)(&par);
	
	for (i=0; i<4; i++) 
	{
		RAM_Buff[rambase+i] = *(p+i);
	}
}

/*! 
 *  \param  (float)			temp		float数据
 *  \param  (uint8_t)		byte[]  Byte数组
 *  \return	
 *	\brief	float数据转为Byte数组
 */
void FloatToByte4(float temp, uint8_t byte[],uint8_t len)
{
	uint8_t *p,i;
	p=(uint8_t *)(&temp);
	
	for (i=0; i<4; i++)
	{
		byte[3+len+i] = *(p+i);
	}
}

/*! 
 *  \param  (uint8_t)			byte[] 		Byte数组
 *  \return	(float)				temp     	float数据
 *	\brief	Byte数组转为float数据
 */
float Byte4ToFloat(uint8_t byte[], uint8_t len)
{
	float temp;
	uint8_t *p,i;
	p=(uint8_t *)(&temp);
	
	for(i=0; i<4; i++)
	{
		*(p+i) = byte[3+len+i];
	}
	return temp;
}

/*! 
 *  \param  
 *  \return	
 *	\brief	不做任何事
 */
void OpNop(void)
{
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*! 
 *  \return		(uint32_t)		value（b）	数据地址
 *  \param		(uint8_t)			ConstByte		Byte型常数
 *	\brief	将Byte型常数写入value（b）地址内
 */
void OpMov_C_Byte(void)
{
	uint8_t i,Par[5];
	for(i=0; i<5; i++)
	{
		Par[i]	=	Flash_Buff[pc+1+i];
	}
	
	RAM_Buff[GetRamBase(Par[0],Par[1],Par[2],Par[3])] = Par[4];
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*! 
 *  \return		(uint32_t)		value（f）		数据地址
 *  \param		(uint8_t)			ConstSingle		float型常数
 *	\brief	将float型常数写入value（f）地址内
 */
void OpMov_C_Floart(void)
{
	uint8_t i,j,Par[8];
	for(i=0; i<8; i++)
	{
		Par[i]	=	Flash_Buff[pc+1+i];
	}

	for(j=0; j<4; j++)
	{
		RAM_Buff[GetRamBase(Par[0],Par[1],Par[2],Par[3])+j]	=	Par[4+j];
	}
	
//	printf("buffer:%x\r\n",GetRamBase(Par[0],Par[1],Par[2],Par[3]));
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*! 
 *  \return		(uint32_t)			value2（b）		数据地址
 *  \param		(uint32_t)			value1（b）		数据地址
 *	\brief	将value1（b）地址内容写入value2（b）地址内
 */
void OpMov_R_Byte(void)
{
	uint8_t i,Par[8];
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	RAM_Buff[GetRamBase(Par[0],Par[1],Par[2],Par[3])]	 = RAM_Buff[GetRamBase(Par[4],Par[5],Par[6],Par[7])];
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*! 
 *  \return		(uint32_t)			value2（f）		数据地址
 *  \param		(uint32_t)			value1（f）		数据地址
 *	\brief	将value1（f）地址内容写入value2（f）地址内
 */
void OpMov_R_Floart(void)
{
	uint8_t i,j,Par[8];
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	for(j=0; j<4; j++)
	{
		RAM_Buff[GetRamBase(Par[0],Par[1],Par[2],Par[3])+j]	 = RAM_Buff[GetRamBase(Par[4],Par[5],Par[6],Par[7])+j];
	}
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)    value(4b)		数据地址
 *  \param  (uint32_t)    pc(4b)			程序地址
 *  \param  (uint8_t)     LEN			长度低八位
 *  \param  (uint8_t)     LEN			长度高八位
 *	\brief	From RAM copy to ROM
 */
void OpMov_C_To_R(void)
{
	uint8_t i,Par[10];
	uint16_t j;
	for(i=0; i<10; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	for(j=0; j<(((uint16_t)Par[9]<<8) + ((uint16_t)Par[8])); j++)
	{
		RAM_Buff[GetRamBase(Par[0],Par[1],Par[2],Par[3])+j]	= Flash_Buff[GetRamBase(Par[4],Par[5],Par[6],Par[7])+j];
	}
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)    value2(4b)		数据地址
 *  \param  (uint32_t)    value1(4b)		数据地址
 *  \param  (uint8_t)     LEN				长度低八位
 *  \param  (uint8_t)     LEN				长度高八位
 *	\brief	From ROM copy to ROM
 */
void OpMov_R_To_R(void)
{
	uint8_t i,Par[10];
	uint16_t j;
	for(i=0; i<10; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	for(j=0; j<(((uint16_t)Par[9]<<8) + ((uint16_t)Par[8])); j++)
	{
		RAM_Buff[GetRamBase(Par[0],Par[1],Par[2],Par[3])+j]	 = RAM_Buff[GetRamBase(Par[4],Par[5],Par[6],Par[7])+j];
	}
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint8_t)    value2(b)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *	\brief	Change Float To Byte
 */
void OpChange_Float_To_Byte(void)
{
	uint8_t i,Par[8];
	float temp;
	
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp = GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
	RAM_Buff[GetRamBase(Par[0],Par[1],Par[2],Par[3])] = (uint8_t) temp;
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param  (uint8_t)    value1(b)		数据地址
 *	\brief	Change Byte To Float
 */
void OpChange_Byte_To_Float(void)
{
	uint8_t i,Par[8];
	uint8_t temp;
	
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp = GetByte(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]),(float) temp);
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)   value3(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(f)		数据地址
 *	\brief	value3(f) = value1(f) + value2(f)
 */
void OpAdd(void)
{
	uint8_t i,Par[12];
	float temp;
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp = GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
	temp += GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11]));
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)   value3(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(f)		数据地址
 *	\brief	value3(f) = value1(f) - value2(f)
 */
void OpSub(void)
{
	uint8_t i,Par[12];
	float temp;
//	printf("%a ‘float’\n");
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp = GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
	temp -= GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11]));
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)   value3(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(f)		数据地址
 *	\brief	value3(f) = value1(f) x value2(f)
 */
void OpMUL(void)
{
		uint8_t i,Par[12];
	float temp;
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp = GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
	temp *= GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11]));
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)   value3(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(f)		数据地址
 *	\brief	value3(f) = value1(f) / value2(f)
 */
void OpDiv(void)
{
	uint8_t i,Par[12];
	float temp;
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp = GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
	temp /= GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11]));
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param	(uint32_t)   value1(f)		数据地址
 *	\brief	value2(f) = expf（value1(f)）
 */
void Exp(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = expf(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
}

/*!
 *  \return	(uint32_t)   value3(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(f)		数据地址
 *	\brief	value3(f) = value1(f) \ value2(f)（10\3 = 1）
 */
void Mod(void)
{
	uint8_t i,Par[12];
	float temp;
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = fmod(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])),
								GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11])));
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *	\brief	value2(f) = floor（value1(f)）
 */
void Rej(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = floorf(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);

}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *	\brief	value2(f) = ceil（value1(f)）
 */
void Inj(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = ceilf(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *	\brief	value2(f) = roundf（value1(f)）
 */
void Appr(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = roundf(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *	\brief	value2(f) = fabs（value1(f)）
 */
void Abs(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = fabs(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *	\brief	value2(f) = -（value1(f)）
 */
void Negate(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = 0 - (GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *	\brief	value2(f) = √value1(f)
 */
void Sqrt(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
	if(temp > 0)
	{
		temp = sqrtf(temp);
		SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	}

}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *	\brief	value2(f) = log10(value1(f))
 */
void Log(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = log10(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *	\brief	value2(f) = log(value1(f))
 */
void Ln(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = log(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *	\brief	value2(f) = sinf(value1(f))
 */
void Sin(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = DegToRad(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	temp = sinf(temp);
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *	\brief	value2(f) = cosf(value1(f))
 */
void Cos(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = DegToRad(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	temp = cosf(temp);
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *	\brief	value2(f) = tanf(value1(f))
 */
void Tan(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = DegToRad(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	temp = tanf(temp);
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *	\brief	value2(f) = asinf(value1(f))
 */
void Asin(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = asinf(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	temp = RadToDeg(temp);
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *	\brief	value2(f) = acosf(value1(f))
 */
void Acos(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = acosf(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	temp = RadToDeg(temp);
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
}

/*!
 *  \return	(uint32_t)   value2(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *	\brief	value2(f) = atanf(value1(f))
 */
void Atan(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = atanf(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	temp = RadToDeg(temp);
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
}

/*!
 *  \return	(uint32_t)   value3(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(f)		数据地址
 *	\brief	value3(f) = value1(f) ^ value2(f)
 */
void Pow(void)
{
	uint8_t i,Par[12];
	float temp;
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	temp = powf(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])),
								GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11])));
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
}

/*!
 *  \return	(uint32_t)   value3(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(b)		数据地址
 *	\brief	截取有效小数： Value1(f)：输入数值
 *												Value(b)：保留小数点位数
 */
void Trun(void)
{
  char s[20];
	uint8_t i,Par[9];
	int temp;
	double Value;
	for(i=0; i<9; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}

	temp = Par[8];
	if( 0 <= temp && temp<= 4)
	{
		temp = floorf(temp);
	}else if(temp < 0)
	{
		temp = 0;
	}else if(temp > 4)
	{
		temp = 4;
	}
	
	Value = GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
	sprintf(s,"%.*lf",temp,Value);
  sscanf(s,"%lf",&Value);
	SetFloat(GetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3])),(float) Value);
	
}

/*!
 *  \return	(uint32_t)   value3(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(f)		数据地址
 *	\brief	value3(f) = value1(f) / value2(f)
 */
void Inside(void)
{
	uint8_t i,Par[16];
	for(i=0; i<16; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	if( GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11])) <= GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])) && 
				GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7]))<= GetFloat(GetRamBase(Par[12],Par[13],Par[14],Par[15])) )
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x01);
	}
	else
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x00);
	}
			
}

/*!
 *  \return	(uint32_t)   value3(f)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(f)		数据地址
 *	\brief	value3(f) = value1(f) / value2(f)
 */
void Outside(void)
{
	uint8_t i,Par[16];
	for(i=0; i<16; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	if( GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11])) <= GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])) && 
				GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7]))<= GetFloat(GetRamBase(Par[12],Par[13],Par[14],Par[15])) )
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x00);
	}
	else
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x01);
	}

}

/*!
 *  \return
 *  \param
 *	\brief	各种高级运算符
 */
void OpMath(void)
{
	uint8_t cmd, pclength = 0;
	
	cmd = Flash_Buff[pc+1];

	switch(cmd)
	{
		case 0x01:Exp();pclength = 8;
			break;
		case 0x02:Mod();pclength = 12;
			break;
		case 0x03:Rej();pclength = 8;
			break;
		case 0x04:Inj();pclength = 8;
			break;
		case 0x05:Appr();pclength = 8;
			break;
		case 0x06:Abs();pclength = 8;
			break;
		case 0x07:Negate();pclength = 8;
			break;
		case 0x08:Sqrt();pclength = 8;
			break;
		case 0x09:Log();pclength = 8;
			break;
		case 0x0A:Ln();pclength = 8;
			break;
		case 0x0B:Sin();pclength = 8;
			break;
		case 0x0C:Cos();pclength = 8;
			break;
		case 0x0D:Tan();pclength = 8;
			break;
		case 0x0E:Asin();pclength = 8;
			break;
		case 0x0F:Acos();pclength = 8;
			break;		
		case 0x10:Atan();pclength = 8;
			break;
		case 0x11:Pow();pclength = 12;
			break;
		case 0x12:Trun();pclength = 9;
			break;
		case 0x13:Inside();pclength = 16;
			break;
		case 0x14:Outside();pclength = 16;
			break;
		default:break;
	}
	
	pc += InstructParsMap[Flash_Buff[pc]]+pclength;
}

/*!
 *  \return	(uint32_t)   value(f)			随机数
 *  \param  (uint32_t)   low(f)				下限
 *  \param  (uint32_t)   up(f)				上限
 *	\brief	产生随机数
 */
void OpRan_f(void)
{
	uint8_t i,Par[12];
	int Min,Max,temp;
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	Min = (int) GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
	Max = (int) GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11]));
	
  temp  =  RNG_Get_RandomRange(Min,Max);
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), (float) temp);
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)   value(f)			随机布尔数
 *  \param  (uint32_t)   prob(f)			概率
 *	\brief	按概率产生随机布尔值
 */
void OpRan_b(void)
{
	uint8_t i,temp,Par[8];
	float pro;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	pro = GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
	if(pro < 0)
	{
		temp = 0x00;
	}else if(pro > 100)
	{
		temp = 0x01;
	}else
	{
		if( RNG_Get_RandomRange(0,100) < pro)
		{
			temp = 0x01;
		}else
		{
			temp = 0x00;
		}
	}
	
	SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)   logic(b)			真假
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(f)		数据地址
 *	\brief	比较： value1(f) == value2(f)
 */
void Equ(void)
{
	uint8_t i,Par[12];
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	if( GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])) 
		== GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11])) )
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x01);
	}
	else
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x00);
	}
			
}

/*!
 *  \return	(uint32_t)   logic(b)			真假
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(f)		数据地址
 *	\brief	比较： value1(f) != value2(f)
 */
void Nequ(void)
{
	uint8_t i,Par[12];
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	if( GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])) 
		!= GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11])) )
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x01);
	}
	else
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x00);
	}

}

/*!
 *  \return	(uint32_t)   logic(b)			真假
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(f)		数据地址
 *	\brief	比较： value1(f) > value2(f)
 */
void Gt(void)
{
	float value1,value2;
	uint8_t i,Par[12];
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	value1 = GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
	value2 = GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11]));
	
	if( value1 
		> value2 )
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x01);
	}
	else
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x00);
	}
	
//	printf("value1:%f\r\n",value1);
//	printf("value2:%f\r\n",value2);
//	printf("Channel_State:%f\n\r",Byte4ToFloat(Channel_State[0],0));
	
}

/*!
 *  \return	(uint32_t)   logic(b)			真假
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(f)		数据地址
 *	\brief	比较： value1(f) >= value2(f)
 */
void Gte(void)
{
	uint8_t i,Par[12];
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	if( GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])) 
		>= GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11])) )
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x01);
	}
	else
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x00);
	}
			
}

/*!
 *  \return	(uint32_t)   logic(b)			真假
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(f)		数据地址
 *	\brief	比较： value1(f) < value2(f)
 */
void Lt(void)
{
	uint8_t i,Par[12];
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	if( GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])) 
		< GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11])) )
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x01);
	}
	else
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x00);
	}

}

/*!
 *  \return	(uint32_t)   logic(b)			真假
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(f)		数据地址
 *	\brief	比较： value1(f) <= value2(f)
 */
void Lte(void)
{
	uint8_t i,Par[12];
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	if( GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])) 
		<= GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11])) )
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x01);
	}
	else
	{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x00);
	}
			
}

/*!
 *  \return	(uint32_t)   value3(b)		数据地址
 *  \param  (uint32_t)   value1(f)		数据地址
 *  \param  (uint32_t)   value2(f)		数据地址
 *  \param  (uint32_t)   flag(b)			数据地址
 *	\brief	选择：如果 flag == 1 则  value3(b)	== value1(b);
 *								如果 flag == 0 则  value3(b)	== value2(b);
 */
void Select()
{
	uint8_t i,Par[16];
	for(i=0; i<16; i++)
	{
		Par[i]		= Flash_Buff[pc+2+i];
	}
	
	if( GetFloat(GetRamBase(Par[12],Par[13],Par[14],Par[15])) >= 0x01)
	{
		SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	}
	else if( GetFloat(GetRamBase(Par[12],Par[13],Par[14],Par[15])) == 0x00)
	{
		SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]), GetFloat(GetRamBase(Par[8],Par[9],Par[10],Par[11])));
	}

}

/*!
 *  \return
 *  \param
 *	\brief	各种比较指令
 */
void OpCompare(void)
{
	uint8_t cmd,pclength = 0;
	
	cmd = Flash_Buff[pc+1];
	
	switch(cmd)
	{
		case 0x01:Equ();pclength = 12;
			break;
		case 0x02:Nequ();pclength = 12;
			break;
		case 0x03:Gt();pclength = 12;
			break;
		case 0x04:Gte();pclength = 12;
			break;
		case 0x05:Lt();pclength = 12;
			break;
		case 0x06:Lte();pclength = 12;
			break;
		case 0x07:Select();pclength = 16;
			break;
		default:break;
	}
	
	pc += InstructParsMap[Flash_Buff[pc]]+pclength;
}

/*!
 *  \return	(uint32_t)   value3(b)		数据地址
 *  \param  (uint32_t)   value1(b)		数据地址
 *  \param  (uint32_t)   value2(b)		数据地址
 *	\brief	逻辑： value3(b) = value1(b) && value2(b)
 */
void OpAnd(void)
{
	uint8_t i,Par[12];
	uint8_t  temp,temp1,temp2;
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp1 = GetByte(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
//	if( temp1 >= 0x01)
//	{
//		temp1 = 0x01;
//	}else if(temp1 == 0x00)
//	{
//		temp1 = 0x00;
//	}
	
	temp2 = GetByte(GetRamBase(Par[8],Par[9],Par[10],Par[11]));
//	if( temp2 >= 0x01)
//	{
//		temp2 = 0x01;
//	}else if(temp2 == 0x00)
//	{
//		temp2 = 0x00;
//	}
//	
	temp = temp1 & temp2;
//	printf("temp1:%x\n\r",temp1);
//	printf("temp2:%x\n\r",temp2);
//	printf("temp:%x\n\r",temp);
//	printf("OpAnd\n\r");
	SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)   value3(b)		数据地址
 *  \param  (uint32_t)   value1(b)		数据地址
 *  \param  (uint32_t)   value2(b)		数据地址
 *	\brief	逻辑： value3(b) = value1(b) || value2(b)
 */
void OpOr(void)
{
	uint8_t i,Par[12];
	uint8_t  temp,temp1,temp2;
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp1 = GetByte(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
//	if( temp1 >= 0x01)
//	{
//		temp1 = 0x01;
//	}else if(temp1 == 0x00)
//	{
//		temp1 = 0x00;
//	}
	
	temp2 = GetByte(GetRamBase(Par[8],Par[9],Par[10],Par[11]));
//	if( temp2 >= 0x01)
//	{
//		temp2 = 0x01;
//	}else if(temp2 == 0x00)
//	{
//		temp2 = 0x00;
//	}
	
	temp = temp1 | temp2;
	
//	printf("OpOr\n\r");
	SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)   value2(b)		数据地址
 *  \param  (uint32_t)   value1(b)		数据地址
 *	\brief	逻辑： value2(b) = !value1(b)
 */
void OpNot(void)
{
	uint8_t i,Par[8];
	uint8_t  temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp = GetByte(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
//	if( temp >= 0x01)
//	{
//		temp = 0x00;
//	}else if(temp == 0x00)
//	{
//		temp = 0x01;
//	}
	
	SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), ~temp);
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)   value3(b)		数据地址
 *  \param  (uint32_t)   value1(b)		数据地址
 *  \param  (uint32_t)   value2(b)		数据地址
 *	\brief	逻辑： value3(b) = value1(b) ^ value2(b)
 */
void OpXor(void)
{
	uint8_t i,Par[12];
	uint8_t  temp,temp1,temp2;
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp1 = GetByte(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
//	if( temp1 >= 0x01)
//	{
//		temp1 = 0x01;
//	}else if(temp1 == 0x00)
//	{
//		temp1 = 0x00;
//	}
	
	temp2 = GetByte(GetRamBase(Par[8],Par[9],Par[10],Par[11]));
//	if( temp2 >= 0x01)
//	{
//		temp2 = 0x01;
//	}else if(temp2 == 0x00)
//	{
//		temp2 = 0x00;
//	}
	
	temp = temp1^temp2;
	
	SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), temp);
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)   value3(2b)		数据地址
 *  \param  (uint32_t)   value1(2b)		数据地址
 *  \param  (uint32_t)   value2(2b)		数据地址
 *	\brief	逻辑： value3(2b) = value1(2b) && value2(2b)
 */
void OpAnd_2b(void)
{
	uint8_t i,Par[12];
	uint16_t  temp,temp1,temp2;
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp1 = ( (((uint16_t) GetByte(GetRamBase(Par[4],Par[5],Par[6],Par[7])+1))<<8) + 
						(((uint16_t) GetByte(GetRamBase(Par[4],Par[5],Par[6],Par[7])  )))     );
//	if( temp1 >= 0x01)
//	{
//		temp1 = 0x01;
//	}else if(temp1 == 0x00)
//	{
//		temp1 = 0x00;
//	}
	
	temp2 = ( (((uint16_t) GetByte(GetRamBase(Par[8],Par[9],Par[10],Par[11])+1))<<8) + 
						(((uint16_t) GetByte(GetRamBase(Par[8],Par[9],Par[10],Par[11])  )))     );
//	if( temp2 >= 0x01)
//	{
//		temp2 = 0x01;
//	}else if(temp2 == 0x00)
//	{
//		temp2 = 0x00;
//	}
	
	temp = temp1 & temp2;
//	printf("temp1:%x\n\r",temp1);
//	printf("temp2:%x\n\r",temp2);
//	printf("temp:%x\n\r",temp);
//	printf("OpAnd_2b\n\r");

	if(temp > 0){
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x01);
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3])+1, 0x00);
	}else{
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x00);
		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3])+1, 0x00);
	}

//	printf("And_2b1:%x\n\r",temp1);
//	printf("And_2b1:%x\n\r",(u8)(temp1>>8));
//	printf("And_2b2:%x\n\r",temp2);
//	printf("And_2b2:%x\n\r",(u8)(temp2>>8));
	printf("And_2b3:%x\n\r",temp);
//	printf("And_2b3:%x\n\r",(u8)(temp>>8));
	
//	SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), (uint8_t)temp);
//	SetByte((GetRamBase(Par[0],Par[1],Par[2],Par[3])+1),(uint8_t)(temp>>8));
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)   value3(2b)		数据地址
 *  \param  (uint32_t)   value1(2b)		数据地址
 *  \param  (uint32_t)   value2(2b)		数据地址
 *	\brief	逻辑： value3(2b) = value1(2b) || value2(2b)
 */
void OpOr_2b(void)
{
	uint8_t i,Par[12];
	uint16_t  temp,temp1,temp2;
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp1 = ( (((uint16_t) GetByte(GetRamBase(Par[4],Par[5],Par[6],Par[7])+1))<<8) + 
						(((uint16_t) GetByte(GetRamBase(Par[4],Par[5],Par[6],Par[7])  )))     );
//	if( temp1 >= 0x01)
//	{
//		temp1 = 0x01;
//	}else if(temp1 == 0x00)
//	{
//		temp1 = 0x00;
//	}
//	
	temp2 = ( (((uint16_t) GetByte(GetRamBase(Par[8],Par[9],Par[10],Par[11])+1))<<8) + 
						(((uint16_t) GetByte(GetRamBase(Par[8],Par[9],Par[10],Par[11])  )))     );
//	if( temp2 >= 0x01)
//	{
//		temp2 = 0x01;
//	}else if(temp2 == 0x00)
//	{
//		temp2 = 0x00;
//	}
	
	temp = temp1 | temp2;
//	printf("temp1:%x\n\r",temp1);
//	printf("temp2:%x\n\r",temp2);
//	printf("temp:%x\n\r",temp);
//	printf("OpOr_2b\n\r");
//	if(temp > 0){
//		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x01);
//	}else{
//		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x00);
//	}

	SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), (uint8_t)temp);
	SetByte((GetRamBase(Par[0],Par[1],Par[2],Par[3])+1),(uint8_t)(temp>>8));
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)   value2(2b)		数据地址
 *  \param  (uint32_t)   value1(2b)		数据地址
 *	\brief	逻辑： value2(2b) = !value1(2b) 
 */
void OpNot_2b(void)
{
	uint8_t i,Par[8];
	uint16_t temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp = ( (((uint16_t) GetByte(GetRamBase(Par[4],Par[5],Par[6],Par[7])+1))<<8) + 
						(((uint16_t) GetByte(GetRamBase(Par[4],Par[5],Par[6],Par[7])  )))     );
//	if( temp >= 0x01)
//	{
//		temp = 0x00;
//	}else if(temp == 0x00)
//	{
//		temp = 0x01;
//	}
//	printf("temp:%x\n\r",temp);
	temp = ~temp;
//	printf("temp:%x\n\r",temp);
	SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), (uint8_t)temp);
	SetByte((GetRamBase(Par[0],Par[1],Par[2],Par[3])+1),(uint8_t)(temp>>8));
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint32_t)   value3(2b)		数据地址
 *  \param  (uint32_t)   value1(2b)		数据地址
 *  \param  (uint32_t)   value2(2b)		数据地址
 *	\brief	逻辑： value3(2b) = value1(2b) ^ value2(2b)
 */
void OpXor_2b(void)
{
	uint8_t i,Par[12];
	uint16_t  temp,temp1,temp2;
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp1 = ( (((uint16_t) GetByte(GetRamBase(Par[4],Par[5],Par[6],Par[7])+1))<<8) + 
						(((uint16_t) GetByte(GetRamBase(Par[4],Par[5],Par[6],Par[7])  )))     );
//	if( temp1 >= 0x01)
//	{
//		temp1 = 0x01;
//	}else if(temp1 == 0x00)
//	{
//		temp1 = 0x00;
//	}
	
	temp2 = ( (((uint16_t) GetByte(GetRamBase(Par[8],Par[9],Par[10],Par[11])+1))<<8) + 
						(((uint16_t) GetByte(GetRamBase(Par[8],Par[9],Par[10],Par[11])  )))     );
//	if( temp2 >= 0x01)
//	{
//		temp2 = 0x01;
//	}else if(temp2 == 0x00)
//	{
//		temp2 = 0x00;
//	}
	
	temp = temp1 ^ temp2;
	
//	if(temp > 0){
//		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x01);
//	}else{
//		SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), 0x00);
//	}
	
	SetByte(GetRamBase(Par[0],Par[1],Par[2],Par[3]), (uint8_t)temp);
	SetByte((GetRamBase(Par[0],Par[1],Par[2],Par[3])+1),(uint8_t)(temp>>8));
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \param  (uint32_t)   pc(4b)				程序地址
 *	\brief	jump
 */
void OpJmp(void)
{
	uint8_t i,Par[4];
	for(i=0; i<4; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	pc = Getpc(Par[0],Par[1],Par[2],Par[3]);
}

/*!
 *  \param  (uint32_t)   pc(4b)				程序地址
 *  \param  (uint32_t)   log(b)				条件标志
 *	\brief	如果logic为真时跳转  为假时跳过该指令
 */
void OpJmp_True(void)
{
	uint8_t i,Par[8];
	uint8_t temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp = GetByte(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
	if( temp >= 0x01)
	{
		temp = 0x01;
	}else if(temp == 0x00)
	{
		temp = 0x00;
	}
	
	if(temp == 0x01)
	{
		pc = Getpc(Par[0],Par[1],Par[2],Par[3]);
	}else
	{
		pc += InstructParsMap[Flash_Buff[pc]];
	}
}

/*!
 *  \param  (uint32_t)   pc(4b)				程序地址
 *  \param  (uint32_t)   log(b)				条件标志
 *	\brief	如果logic为假时跳转  为真时跳过该指令
 */
void OpJmp_False(void)
{
	uint8_t i,Par[8];
	uint8_t temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp = GetByte(GetRamBase(Par[4],Par[5],Par[6],Par[7]));
	if( temp >= 0x01)
	{
		temp = 0x01;
	}else if(temp == 0x00)
	{
		temp = 0x00;
	}
	
	if(temp == 0x00)
	{
		pc = Getpc(Par[0],Par[1],Par[2],Par[3]);
	}else
	{
		pc += InstructParsMap[Flash_Buff[pc]];
	}
}

/*!
 *  \param  (uint32_t)   pc(4b)				程序地址
 *  \param  (uint32_t)   value(f)			循环次数
 *	\brief	循环跳转
 */
void OpDjmp(void)
{
	uint8_t i,Par[8];
	float temp;
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp = floorf(GetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7])));
	if( temp > 0.000000f)
	{
		temp = temp - 1.000000f;
		SetFloat(GetRamBase(Par[4],Par[5],Par[6],Par[7]),temp);
		pc = Getpc(Par[0],Par[1],Par[2],Par[3]);
	}else if(temp <= 0.000000f)
	{
		pc += InstructParsMap[Flash_Buff[pc]];
	}
}

/*!
 *  \param  (uint32_t)   pc(4b)				程序地址
 *  \param  (uint8_t)   ID-L(b)			线程ID低位
 *  \param  (uint8_t)   ID-H(b)			线程ID高位
 *	\brief	开启线程
 */
void OpThread_Start(void)
{
	uint8_t i,Par[6];
	uint16_t Par_16;
	for(i=0; i<6; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	Par_16 = ((uint16_t)Par[5]<<8) + ((uint16_t)Par[4]);
	Thread[Par_16].Threadpc = Getpc(Par[0],Par[1],Par[2],Par[3]);
	Thread[Par_16].state = 1;
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \param  (uint32_t)   pc(4b)				程序地址
 *  \param  (uint32_t)   log(b)				条件标志
 *  \param  (uint8_t)   ID-L(b)			线程ID低位
 *  \param  (uint8_t)   ID-H(b)			线程ID高位
 *	\brief	如果logic为真时开启线程 为假时跳过该指令
 */
void OpThread_True_Start(void)
{
	uint8_t i,Par[10];
	uint16_t Par_16;
	for(i=0; i<10; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	if(RAM_Buff[GetRamBase(Par[4],Par[5],Par[6],Par[7])] != 0)
	{
		Par_16 = ((uint16_t)Par[9]<<8) + ((uint16_t)Par[8]);
		Thread[Par_16].Threadpc = Getpc(Par[0],Par[1],Par[2],Par[3]);
		Thread[Par_16].state = 1;
	}
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \param  (uint32_t)   pc(4b)				程序地址
 *  \param  (uint32_t)   log(b)				条件标志
 *  \param  (uint8_t)   ID-L(b)			线程ID低位
 *  \param  (uint8_t)   ID-H(b)			线程ID高位
 *	\brief	如果logic为假时开启线程 为真时跳过该指令
 */
void OpThread_False_Start(void)
{
	uint8_t i,Par[10];
	uint16_t Par_16;
	for(i=0; i<10; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	if(RAM_Buff[GetRamBase(Par[4],Par[5],Par[6],Par[7])] == 0)
	{
		Par_16 = ((uint16_t)Par[9]<<8) + ((uint16_t)Par[8]);
		Thread[Par_16].Threadpc = Getpc(Par[0],Par[1],Par[2],Par[3]);
		Thread[Par_16].state = 1;
	}
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \param  (uint8_t)   ID-L(b)			线程ID低位
 *  \param  (uint8_t)   ID-H(b)			线程ID高位
 *	\brief	线程等待结束，如果结果自动跳转指令，如果还没结束则一直在此指令停留等待
 */
void OpThread_End_IF(void)
{
	uint8_t i,Par[2];
	uint16_t Par_16;
	for(i=0; i<2; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	Par_16 = ((uint16_t)Par[1]<<8) + ((uint16_t)Par[0]);
	if( Thread[Par_16].Threadpc == 0x0000)
	{
		pc += InstructParsMap[Flash_Buff[pc]];
	}
}

/*!
 *  \param  (uint8_t)   ID-L(b)			线程ID低位
 *  \param  (uint8_t)   ID-H(b)			线程ID高位
 *	\brief	结束该线程
 */
void OpThread_End(void)
{
	uint8_t i,Par[2];
	uint16_t Par_16;
	for(i=0; i<2; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	Par_16 = ((uint16_t)Par[1]<<8) + ((uint16_t)Par[0]);
	Thread[Par_16].Threadpc = 0x0000;
	Thread[Par_16].state = 0;
	
	if( Par_16 == ThreadNow)
	{
		pc = 0x0000;
	}else
	{
		pc += InstructParsMap[Flash_Buff[pc]];
	}
}

/*!
 *	\brief	程序开启
 */
void OpStartprogram(void)
{
	if(flashbuff_flag == 1 )  //当flash烧写成功才能运行程序
	{
		programflag = 1;
		pc += InstructParsMap[Flash_Buff[pc]];
	}
}

/*!
 *  \param  (uint32_t)   time(f)			时间
 *	\brief	延时
 */
void OpDelay(void)
{
	uint8_t i,Par[4];
	uint32_t temp;
	for(i=0; i<4; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
//#ifdef INSTRUCT_DEBUG
//	printf("Delay:%d-%d-%d\r\n",ThreadNow,Thread[ThreadNow].state,temp);
//#endif
	
	if( Thread[ThreadNow].state == 1)
	{
		Thread[ThreadNow].state = 2;
		Thread[ThreadNow].timetmp = GetProgramTimeMs();
//		printf("Delay:%d-%d\r\n",ThreadNow,Thread[ThreadNow].timetmp);
	}
	temp = GetProgramTimeMs() - Thread[ThreadNow].timetmp;
	if( Thread[ThreadNow].state == 2
				&& temp >= 10*GetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3])))
	{
//		printf("Delay:%d-%d\r\n",ThreadNow,temp);
		Thread[ThreadNow].state = 1;
		pc += InstructParsMap[Flash_Buff[pc]];
	}
}

/*!
 *	\brief	程序结束
 */
void OpEndprogram(void)//乐博士测试
{
	EndProgram();
}

/*!
 *  \param  (uint32_t)   time(f)
 *	\brief	
 */
void OpGet_Program_Time(void)
{
	uint8_t i,Par[4];
	
	for(i=0; i<4; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]),ProgramTime/10);
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return  (uint32_t)   time(f)		时间
 *  \param  (uint32_t)   	ID(b)			定时器ID
 *	\brief	获取定时器的时间
 */
void OpGet_Time(void)
{
	uint8_t i,Par[5];
	uint8_t temp;
	for(i=0; i<5; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp = Par[4];
	if(0 < temp && temp <= 8)
	{
		SetFloat(GetRamBase(Par[0],Par[1],Par[2],Par[3]),(Timer[temp - 1].time)/10);
	}

	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \param  (uint32_t)   	ID(b)			定时器ID
 *	\brief	重置定时器的时间
 */
void OpReset_Time(void)
{
	uint8_t i,Par[1];
	uint8_t temp;
	for(i=0; i<2; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	temp = Par[0];
	if(0 < temp && temp <= 8)
	{
		Timer[temp - 1].time = 0;
	}
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*! 
 *  \param  (uint8_t)    Par[0]   LAY				级联层
 *  \param  (uint8_t)    Par[1]   PORT			端口号数据地址24~31位
 *  \param  (uint8_t)    Par[2]   PORT			端口号数据地址16~23位
 *  \param  (uint8_t)    Par[3]   PORT			端口号数据地址8~15位
 *  \param  (uint8_t)    Par[4]   PORT			端口号数据地址0~7位
 *  \param  (uint8_t)    Par[5]   ID
 *  \param  (uint8_t)    Par[6]   MODE
 *  \param  (uint8_t)    Par[7]   BufferSize
 *  \param  (uint8_t)    Par[8]   Buffer			端口号数据地址24~31位
 *  \param  (uint8_t)    Par[9]   Buffer			端口号数据地址16~23位
 *  \param  (uint8_t)    Par[10]  Buffer			端口号数据地址8~15位
 *  \param  (uint8_t)    Par[11]  Buffer			端口号数据地址0~7位
 *	\brief		传感端口读取指令
 */
void OpRead_Port(void)
{
	uint8_t i,Par[12];
	uint8_t lay,port,id,mode,buffersize,*buffer;
	uint32_t bufferAdress;
//	static u8 cascade_funt_send[4][8];

	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	lay 					= Par[0];
	port 					= RAM_Buff[GetRamBase(Par[1],Par[2],Par[3],Par[4])];
	id      			= Par[5];
	mode    			= Par[6];
	buffersize		= Par[7];
	bufferAdress 	= GetRamBase(Par[8],Par[9],Par[10],Par[11]);
	
	buffer = (u8 *) malloc(buffersize * sizeof(u8));//u8 可以用*buffer来代替
	for(i=0; i<buffersize; i++)
	{
		buffer[i] = GetByte(bufferAdress+i);
	}
	
	if(lay > 0) //级联
	{
		HostSend_DDataGet_Packet(lay, port, id, mode);

		
	}
	else if(lay == 0)//非级联
	{
		if(id == 0x01) 				//颜色
		{
			Color(port,mode,bufferAdress);
		}
		if(id == 0x02)				//陀螺仪
		{
			Gyro(port,mode,bufferAdress);
		}
		if(id == 0x03) 				//红外传感
		{
			Infrared(port,mode,bufferAdress);
		}
		if(id == 0x04) 				//大型电机
		{
			Motor(port,mode,bufferAdress);
		}
		if(id == 0x05) 				//中型电机
		{
			Motor(port,mode,bufferAdress);
		}
		if(id == 0x06) 				//温度
		{
			Temperature(port,mode,bufferAdress);
		}
		if(id == 0x07) 				//触碰
		{
			Touch(port,mode,bufferAdress);
		}
		if(id == 0x08) 				//超声波
		{
			Ultrasonic(port,mode,bufferAdress);
		}
		if(id == 0x09) 				//能量
		{
			Recharge(port,mode,bufferAdress);
		}
		if(id == 0x0a) 				//NXT声音
		{
			Sound(port,mode,bufferAdress);
		}
	}
	free(buffer);
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*! 
 *  \param  (uint8_t)    Par[0]   LAY				级联层
 *  \param  (uint8_t)    Par[1]   PORT			端口号数据地址24~31位
 *  \param  (uint8_t)    Par[2]   PORT			端口号数据地址16~23位
 *  \param  (uint8_t)    Par[3]   PORT			端口号数据地址8~15位
 *  \param  (uint8_t)    Par[4]   PORT			端口号数据地址0~7位
 *  \param  (uint8_t)    Par[5]   ID
 *  \param  (uint8_t)    Par[6]   MODE
 *  \param  (uint8_t)    Par[7]   BufferSize
 *  \param  (uint8_t)    Par[8]   Buffer			端口号数据地址24~31位
 *  \param  (uint8_t)    Par[9]   Buffer			端口号数据地址16~23位
 *  \param  (uint8_t)    Par[10]  Buffer			端口号数据地址8~15位
 *  \param  (uint8_t)    Par[11]  Buffer			端口号数据地址0~7位
 *	\brief		电机端口写入指令
 */
void OpWrite_Port(void)
{
	uint8_t i,Par[12],result=0;
	uint8_t lay,port,port_1,port_2,id,mode,buffersize,*buffer;
	uint32_t bufferAdress;
	static u8 cascade_funt_send[4][8];
	
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	lay 					= Par[0];
	port 					= RAM_Buff[GetRamBase(Par[1],Par[2],Par[3],Par[4])];
	port_1 				= (RAM_Buff[GetRamBase(Par[1],Par[2],Par[3],Par[4])] & 0x30) >> 4;
	port_2 				= (RAM_Buff[GetRamBase(Par[1],Par[2],Par[3],Par[4])] & 0xc0) >> 6;
	id      			= Par[5];
	mode    			= Par[6];
	buffersize		= Par[7];
	bufferAdress 	= GetRamBase(Par[8],Par[9],Par[10],Par[11]);
	
	buffer = (u8 *) malloc(buffersize * sizeof(u8));//u8 可以用*buffer来代替
	for(i=0; i<buffersize; i++)
	{
		buffer[i] = GetByte(bufferAdress+i);
	}
	
	if(lay > 0) //级联
	{
		if(cascade_funt_send[lay][port] == 0 )
		{
			HostSend_DControl_Packet(lay, port, id, mode, buffer, buffersize);
			cascade_funt_send[lay][port] = 1;
		}
		
		if(Cascade_Function.Cascade_flag[lay][port].flag == 1)
		{
			result = 1;
			cascade_funt_send[lay][port] = 0;
			Cascade_Function.Cascade_flag[lay][port].flag = 0;
		}
		
	}else if(lay == 0)//非级联
	{
//		printf("port%x \n\r",port);

		if(id == 0x80) 	//中型电机   //电机修改12.12
		{
			if(mode == 1)							//中型电机模式1 off
			{
				result = Meduim_Motor_Off(port, GetByte(bufferAdress));
			}
			if(mode == 2)							//中型电机模式2 on
			{
				result = Meduim_Motor_On(port, GetFloat(bufferAdress),Motor_Invert[port-5]);																			
			}
			if(mode == 3)							//中型电机模式3 on for second
			{
				result = Meduim_Motor_OnForSecond(port,
																						GetFloat(bufferAdress),
																							GetFloat(bufferAdress+4),
																								GetByte(bufferAdress+8),
																									Motor_Invert[port-5]);
			}
			if(mode == 4)							//中型电机模式4 on for degrees
			{
				result = Meduim_Motor_OnForDegrees(port,
																						GetFloat(bufferAdress),
																							GetFloat(bufferAdress+4),
																								GetByte(bufferAdress+8),
																									Motor_Invert[port-5]);
			}
			if(mode == 5)							//中型电机模式5 on for rotation
			{
				result = Meduim_Motor_OnForRotations(port,
																							GetFloat(bufferAdress),
																								GetFloat(bufferAdress+4),
																									GetByte(bufferAdress+8),
																										Motor_Invert[port-5]);
			}
		}
		if(id == 0x81)	//大型电机
		{
			if(mode == 1)							//大型电机模式1 off
			{
				result = Large_Motor_Off(port, GetByte(bufferAdress));
			}
			if(mode == 2)							//大型电机模式2 on
			{
				result = Large_Motor_On(port, GetFloat(bufferAdress),Motor_Invert[port-5]);
			}
			if(mode == 3)							//大型电机模式3 on for second
			{
				result = Large_Motor_OnForSecond(port,
																					GetFloat(bufferAdress),
																						GetFloat(bufferAdress+4),
																							GetByte(bufferAdress+8),
																								Motor_Invert[port-5]);
			}
			if(mode == 4)							//大型电机模式4 on for degrees
			{
				result = Large_Motor_OnForDegrees(port,
																						GetFloat(bufferAdress),
																							GetFloat(bufferAdress+4),
																								GetByte(bufferAdress+8),
																									Motor_Invert[port-5]);
		
	//			printf("INSTRUCT:%s %d\r\n","result",result);

			}
			if(mode == 5)							//大型电机模式5 on for rotation
			{
				result = Large_Motor_OnForRotations(port,
																							GetFloat(bufferAdress),
																								GetFloat(bufferAdress+4),
																									GetByte(bufferAdress+8),
																										Motor_Invert[port-5]);
			}
		}
		if(id == 0x82)	//驾驶控制               
		{
			if(mode == 1)							//驾驶控制模式1 off
			{
				result = Move_Steering_Off(port_1+5, port_2+5, GetByte(bufferAdress));
			}
			if(mode == 2)							//驾驶控制模式2 on
			{
				result = Move_Steering_On(port_1+5,port_2+5,
																		GetFloat(bufferAdress),
																			GetFloat(bufferAdress+4),
																				Motor_Invert[port_1-5]);
			}
			if(mode == 3)							//驾驶控制模式3 on for second
			{
				result = Move_Steering_OnForSecond(port_1+5,port_2+5,
																						GetFloat(bufferAdress),
																							GetFloat(bufferAdress+4),
																								GetFloat(bufferAdress+8),
																									GetByte(bufferAdress+12),
																										Motor_Invert[port_1-5]);
			}
			if(mode == 4)							//驾驶控制模式4 on for degrees
			{
				result = Move_Steering_OnForDegrees(port_1+5,port_2+5,
																							GetFloat(bufferAdress),
																								GetFloat(bufferAdress+4),
																									GetFloat(bufferAdress+8),
																										GetByte(bufferAdress+12),
																											Motor_Invert[port_1-5]);

			}
			if(mode == 5)							//驾驶控制模式5 on for rotation
			{
				result = Move_Steering_OnForRotations(port_1+5,port_2+5,
																								GetFloat(bufferAdress),
																									GetFloat(bufferAdress+4),
																										GetFloat(bufferAdress+8),
																											GetByte(bufferAdress+12),
																												Motor_Invert[port_1-5]);
			}
		}
		if(id == 0x83)	//坦克控制
		{
			if(mode == 1)							//坦克控制模式1 off
			{
				result = Move_Tank_Off(port_1+5, port_2+5, GetByte(bufferAdress));
			}
			if(mode == 2)							//坦克控制模式2 on
			{
				result = Move_Tank_On(port_1+5,port_2+5,
																GetFloat(bufferAdress),
																	GetFloat(bufferAdress+4),
																		Motor_Invert[port_1-5]);
			}
			if(mode == 3)							//坦克控制模式3 on for second
			{
				result = Move_Tank_OnForSecond(port_1+5,port_2+5,
																				GetFloat(bufferAdress),
																					GetFloat(bufferAdress+4),
																						GetFloat(bufferAdress+8),
																							GetByte(bufferAdress+12),
																								Motor_Invert[port_1-5]);
			}
			if(mode == 4)							//坦克控制模式4 on for degrees
			{
				result = Move_Tank_OnForDegrees(port_1+5,port_2+5,
																					GetFloat(bufferAdress),
																						GetFloat(bufferAdress+4),
																							GetFloat(bufferAdress+8),
																								GetByte(bufferAdress+12),
																									Motor_Invert[port_1-5]);
	//			printf("INSTRUCT:%s %d\r\n","result",result);

			}
			if(mode == 5)							//坦克控制模式5 on for rotation
			{
				result = Move_Tank_OnForRotations(port_1+5,port_2+5,
																						GetFloat(bufferAdress),
																							GetFloat(bufferAdress+4),
																								GetFloat(bufferAdress+8),
																									GetByte(bufferAdress+12),
																										Motor_Invert[port_1-5]);
			}
		}
		if(id == 0x84) 				//未校准电机
		{
			if(mode == 1)				
			{
				result = Unregulated_Motor_On(port, GetFloat(bufferAdress), Motor_Invert[port-5]);
			}
			
		}
		if(id == 0x85) 				//反转电机
		{
			if(mode == 1)
			{
				if(GetByte(bufferAdress) == 1)
				{
					Motor_Invert[port-5] = 0x01;
				}else if(GetByte(bufferAdress) == 0)
				{
					Motor_Invert[port-5] = 0x00;
				}
				result = 1;
			}
		}
	}
	free(buffer);
	
	if(result == 1)
	{
//		printf("buffer:%x\r\n",bufferAdress);
//		printf("iii:%d \r\n",Pluse[port-5]++);
//		printf("iii:%d \r\n",SumPluseForDisplay[port-5]++);
//		Pluse[port-5]=0;
//		SumPluseForDisplay[port-5]=0;
		pc += InstructParsMap[Flash_Buff[pc]];
	}
}

/*! 
 *  \param  (uint8_t)    Par[0]   DEVICE
 *  \param  (uint8_t)    Par[1]   MODE
 *  \param  (uint8_t)    Par[2]   BufferSize
 *  \param  (uint8_t)    Par[3]   Buffer			端口号数据地址24~31位
 *  \param  (uint8_t)    Par[4]   Buffer			端口号数据地址16~23位
 *  \param  (uint8_t)    Par[5]  	Buffer			端口号数据地址8~15位
 *  \param  (uint8_t)    Par[6]  	Buffer			端口号数据地址0~7位
 *	\brief		设备读取指令
 */
void OpRead_Device(void)
{
	uint8_t i,Par[7];
	uint8_t device,mode;
	uint32_t bufferAdress;
//	uint8_t BufferSize;
	for(i=0; i<7; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	device				= Par[0];
	mode 					= Par[1];
//	BufferSize 		= Par[2];
	bufferAdress 				= GetRamBase(Par[3],Par[4],Par[5],Par[6]);
	
	if(device == 0x01) 				//显示屏
	{
	}
	if(device == 0x02) 				//声音
	{
	}
	if(device == 0x03) 				//按键
	{
		BriButton(mode,bufferAdress);
	}
	if(device == 0x04) 				//指示灯
	{
	}
	if(device == 0x05) 				//蓝牙操作
	{
	}
	if(device == 0x06) 				//消息传输
	{
	}
	if(device == 0x07) 				//文件操作
	{
	}
	if(device == 0x08) 				//波形上传
	{
	}
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*! 
 *  \param  (uint8_t)    Par[0]   DEVICE
 *  \param  (uint8_t)    Par[1]   MODE
 *  \param  (uint8_t)    Par[2]   BufferSize
 *  \param  (uint8_t)    Par[3]   Buffer			端口号数据地址24~31位
 *  \param  (uint8_t)    Par[4]   Buffer			端口号数据地址16~23位
 *  \param  (uint8_t)    Par[5]  	Buffer			端口号数据地址8~15位
 *  \param  (uint8_t)    Par[6]  	Buffer			端口号数据地址0~7位
 *	\brief		设备写入指令
 */
void OpWrite_Device(void)
{
	uint8_t i,result,Par[7];
	uint8_t device,mode,BufferSize;
	uint32_t bufferAdress;
	for(i=0; i<7; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	device				= Par[0];
	mode 					= Par[1];
	BufferSize 		= Par[2];
	bufferAdress 	= GetRamBase(Par[3],Par[4],Par[5],Par[6]);
	
	if(device == 0x01) 				//显示屏
	{
		BriDisplay(mode,BufferSize,bufferAdress);
		LCD_flag = 1;
		LCD_refresh();
		result = 1;
	}
	if(device == 0x02) 				//声音
	{
		result = BriSound(mode,bufferAdress);
	}
	if(device == 0x03) 				//按键
	{
		result = 1;
	}
	if(device == 0x04) 				//指示灯
	{
		BriLight(mode,bufferAdress);result = 1;
	}
	if(device == 0x05) 				//蓝牙操作
	{
		BriBt(mode,bufferAdress);result = 1;
	}
	if(device == 0x06) 				//消息传输
	{
		BriMessage(mode,bufferAdress);result = 1;
	}
	if(device == 0x07) 				//文件操作
	{
		BriFile(mode,bufferAdress);result = 1;
	}
	if(device == 0x08) 				//波形上传
	{
		BriWaveform(mode,bufferAdress);result = 1;
	}
	if(device == 0x09) 			//计时器
	{
		BriTimerSensor(mode,bufferAdress);result = 1;
	}
	if(device == 0x0a) 				//原始值
	{
		BriRawSensor(mode,bufferAdress);result = 1;
	}
		
	if(result == 1)
	{
		pc += InstructParsMap[Flash_Buff[pc]];
	}
}

/*!
 *  \return
 *  \param
 *	\brief	各种文件名操作
 */
void OpFilename(void)
{
	uint8_t cmd;
	
	cmd = Flash_Buff[pc+1];
	
	switch(cmd)
	{
		case 0x01:
			break;
		case 0x02:
			break;
		case 0x03:
			break;
		case 0x04:
			break;
		case 0x05:
			break;
		case 0x06:
			break;
		default:break;
	}
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return
 *  \param
 *	\brief	
 */
void File_Byte(void)
{
	uint8_t i,Par[16];
	for(i=0; i<16; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	pc += 16;
}

/*!
 *  \return
 *  \param
 *	\brief	
 */
void File_picture(void)
{
	uint8_t i,Par[16];
	for(i=0; i<16; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	pc += 16;
}	

/*!
 *  \return
 *  \param
 *	\brief	
 */
void File_Sound(void)
{
	uint8_t i,Par[16];
	for(i=0; i<16; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	pc += 16;
}
	
/*!
 *  \return
 *  \param
 *	\brief	
 */
void File_Datalog(void)
{
	uint8_t i,Par[16];
	for(i=0; i<16; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	pc += 16;
}	

/*!
 *  \return
 *  \param
 *	\brief	
 */
void File_Txt(void)
{
	uint8_t i,Par[16];
	for(i=0; i<16; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	pc += 16;
}	

/*!
 *  \return
 *  \param
 *	\brief	
 */
void File_close(void)
{
	uint8_t i,Par[16];
	for(i=0; i<16; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	pc += 16;
}

/*!
 *  \return
 *  \param
 *	\brief	各种文件操作
 */
void OpFile(void)
{
	uint8_t cmd;
	
	cmd = Flash_Buff[pc+1];
	
	switch(cmd)
	{
		case 0x01:File_Byte();
			break;
		case 0x02:File_picture();
			break;
		case 0x03:File_Sound();
			break;
		case 0x04:File_Datalog();
			break;
		case 0x05:File_Txt();
			break;
		case 0x06:File_close();
			break;
		default:break;
	}
	
	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return	(uint8_t)		size			字符串长度
 *  \param	(uint8_t)		string		字符串
 *	\brief	获取字符串的长度
 */
void Sting_Getsize(void)
{
	uint8_t i,Par[8];
	uint16_t  temp=0x0000;
	uint32_t address;
	
	for(i=0; i<8; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	address = Getpc(Par[4],Par[5],Par[6],Par[7]);
	while(Flash_Buff[address] <= '~' && Flash_Buff[address] >= ' ' )
	{
		address++;
		temp++;
	}
	SetByte(Getpc(Par[0],Par[1],Par[2],Par[3]), (uint8_t) temp>>8);
	SetByte(Getpc(Par[0],Par[1],Par[2],Par[3])+1, (uint8_t) temp);
	
	pc += 8;
}

/*!
 *  \return
 *  \param
 *	\brief	
 */
void Sting_Add(void)
{
	uint8_t i,Par[12];
//	uint16_t  temp=0x0000;
	uint32_t address1,address2,address3;
	
	for(i=0; i<12; i++)
	{
		Par[i]		= Flash_Buff[pc+1+i];
	}
	
	address1 = Getpc(Par[0],Par[1],Par[2],Par[3]);
	address2 = Getpc(Par[4],Par[5],Par[6],Par[7]);
	address3 = Getpc(Par[8],Par[9],Par[10],Par[11]);
	
//	while(Flash_Buff[address1] <= '~' && Flash_Buff[address1] >= ' ' )
//	{
//		address++;
//		temp++;
//	}
	
	
	pc += 12;
}

/*!
 *  \return
 *  \param
 *	\brief	
 */
void Sting_Compare(void)
{
	
}

/*!
 *  \return
 *  \param
 *	\brief	
 */
void Sting_Duplicate(void)
{
	
}

/*!
 *  \return
 *  \param
 *	\brief	
 */
void Sting_FloatToString(void)
{
	
}

/*!
 *  \return
 *  \param
 *	\brief	
 */
void Sting_StringToFloat(void)
{
	
}

/*!
 *  \return
 *  \param
 *	\brief	
 */
void Sting_Strip(void)
{
	
}

/*!
 *  \return
 *  \param
 *	\brief	
 */
void Sting_Sub(void)
{
	
}

/*!
 *  \return
 *  \param
 *	\brief	各种字符串操作
 */
void OpString(void)
{
	uint8_t cmd;
	
	cmd = Flash_Buff[pc+1];
	
	switch(cmd)
	{
		case 0x01:Sting_Getsize();
			break;
		case 0x02:Sting_Add();
			break;
		case 0x03:Sting_Compare();
			break;
		case 0x04:Sting_Duplicate();
			break;
		case 0x05:Sting_FloatToString();
			break;
		case 0x06:Sting_StringToFloat();
			break;
		case 0x07:Sting_Strip();
			break;
		case 0x08:Sting_Sub();
			break;
		default:break;
	}

	pc += InstructParsMap[Flash_Buff[pc]];
}

/*!
 *  \return （uint8_t）		return				结果
 *  \param （uint8_t）		port					端口
 *  \param （uint8_t）		DeviceId			设备ID
 *  \param （uint8_t）		mode					设备模式
 *  \param （uint8_t）		buffersize		
 *  \param	(uint8_t）		Buffer[]			
 *	\brief	级联电机处理函数
 */
u8 daisy_Write_Port(u8 port, u8 DeviceId, u8 mode,	u8 buffersize, u8 buff[13])  //级联电机控制函数
{
	u8 result=0;
	if(DeviceId == 0x80) 	//中型电机
		{
			if(mode == 1)							//中型电机模式1 off
			{
				result = Meduim_Motor_Off(port, GetByte(buff[0]));
			}
			if(mode == 2)							//中型电机模式2 on
			{
				result = Meduim_Motor_On(port, GetFloat(buff[0]),Motor_Invert[port-5]);																			
			}
			if(mode == 3)							//中型电机模式3 on for second
			{
				result = Meduim_Motor_OnForSecond(port,
																						60,//GetFloat(GetRamBase(buff[0],buff[1],buff[2],buff[3])),
																							10,//GetFloat(GetRamBase(buff[4],buff[5],buff[6],buff[7])),
																								buff[8],
																									Motor_Invert[port-5]);
//				result =1;
//					printf("GetFloat %f  \r\n",GetFloat(GetRamBase(buff[0],buff[1],buff[2],buff[3])));
//					printf("GetFloat %f  \r\n",GetFloat(GetRamBase(buff[4],buff[5],buff[6],buff[7])));
//					printf("GetByte %x  \r\n",buff[8]);
			}
			if(mode == 4)							//中型电机模式4 on for degrees
			{
				result = Meduim_Motor_OnForDegrees(port,
																						GetFloat(buff[0]),
																							GetFloat(buff[4]),
																								GetByte(buff[8]),
																									Motor_Invert[port-5]);
			}
			if(mode == 5)							//中型电机模式5 on for rotation
			{
				result = Meduim_Motor_OnForRotations(port,
																							GetFloat(buff[0]),
																								GetFloat(buff[4]),
																									GetByte(buff[8]),
																										Motor_Invert[port-5]);
			}
		}
		if(DeviceId == 0x81)	//大型电机
		{
			if(mode == 1)							//大型电机模式1 off
			{
				result = Large_Motor_Off(port, GetByte(buff[0]));
			}
			if(mode == 2)							//大型电机模式2 on
			{
				result = Large_Motor_On(port, GetFloat(buff[0]),Motor_Invert[port-5]);
			}
			if(mode == 3)							//大型电机模式3 on for second
			{
				result = Large_Motor_OnForSecond(port,
																							GetFloat(buff[0]),
																								GetFloat(buff[4]),
																									GetByte(buff[8]),
																										Motor_Invert[port-5]);
			}
			if(mode == 4)							//大型电机模式4 on for degrees
			{
				result = Large_Motor_OnForDegrees(port,
																						GetFloat(buff[0]),
																							GetFloat(buff[4]),
		//																						180,
																								GetByte(buff[8]),
																									Motor_Invert[port-5]);
	//			printf("INSTRUCT:%s %d\r\n","result",result);

			}
			if(mode == 5)							//大型电机模式5 on for rotation
			{
				result = Large_Motor_OnForRotations(port,
																							GetFloat(buff[0]),
																								GetFloat(buff[4]),
																									GetByte(buff[8]),
																										Motor_Invert[port-5]);
			}
		}
		return result;
}

/*!
 *  \return （uint8_t）		return				结果
 *  \param （uint8_t）		port					端口
 *  \param （uint8_t）		DeviceId			设备ID
 *  \param （uint8_t）		mode					设备模式
 *  \param （uint8_t）		buffersize		
 *  \param	(uint8_t）		Buffer[]			
 *	\brief	级联传感器控制函数
 */
u8 daisy_sensor_control(u8 Port, u8 DeviceId, u8 mode,	u8 bufflength, u8 buff[13])  //级联传感器模式更换函数
{
	if(DeviceId == 0x01)
	{
		if(mode == 0x01)
		{
			ChangeMode(Port,2);
		}
		if(mode == 0x02)
		{
			ChangeMode(Port,0);
		}
		if(mode == 0x03)
		{
			ChangeMode(Port,1);
		}
	}
	if(DeviceId == 0x02)
	{
		if(mode == 0x01)
		{
			ChangeMode(Port,0);
		}
		if(mode == 0x02)
		{
			ChangeMode(Port,1);
		}
	}
	if(DeviceId == 0x03)
	{
		if(mode == 0x01)
		{
			ChangeMode(Port,0);
		}
	}
	if(DeviceId == 0x08)
	{
		if(mode == 0x03)
		{
			ChangeMode(Port,0);
		}
		if(mode == 0x04)
		{
			ChangeMode(Port,1);
		}
		if(mode == 0x05)
		{
			ChangeMode(Port,2);
		}
	}
	else
	{
		Channel_State[Port][2] = mode;
	}
	
	return 1;
}

/*!
 *  \return （uint8_t）		return				结果
 *  \param （uint8_t）		port					端口
 *  \param （uint8_t）		DeviceId			设备ID
 *  \param （uint8_t）		mode					设备模式
 *  \param （uint8_t）		buffersize		
 *  \param	(uint8_t）		Buffer[]			
 *	\brief	级联指令处理
 */
void deal_daisy_Chain(u8 Port, u8 DeviceId, u8 mode,	u8 bufflength, u8 buff[13])
{
	u8 result=0;
	
	if(Port > 4)
	{
		result = daisy_Write_Port(Port,DeviceId,mode,bufflength,buff);
	}
	else
	{
		result = daisy_sensor_control(Port,DeviceId,mode,bufflength,buff);
	}
	
//		result = 1;
	
	if(result == 1)
	{
		//返回指令完成
		SlaveReply_DControl_Packet(CascadeLevel,
																Port,
																	Cascade_Function.Cascade_Order[Port].id,
																		Cascade_Function.Cascade_Order[Port].mode);	
		Cascade_Function.Cascade_Order[Port-1].flag = 0;
	}
}

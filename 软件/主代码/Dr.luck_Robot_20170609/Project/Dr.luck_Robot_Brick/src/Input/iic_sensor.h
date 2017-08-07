/**
  ******************************************************************************
  * @file    EST Brick of Dr.luck/iic_sensor.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __IIC_SENSOR_H
#define __IIC_SENSOR_H 	

#include "sys.h" 
#include "lego_include.h" 

#define   NO_OF_IIC_PORTS					INPUTS

#define   IIC_TIMER_RESOLUTION		20 // [100u]

#define		IIC_INPUTS							4

#define   Iic1 										0
#define   Iic2 										1
#define   Iic3 										2
#define   Iic4 										3

enum      IIC_STATE
{
  IIC_IDLE,									//00
  IIC_INIT,									//01
  IIC_RESTART,							//02
  IIC_ENABLE,								//03
  IIC_NXT_TEMP_START,				//04
  IIC_NXT_TEMP_WRITE,				//05
  IIC_NXT_TEMP_READ,				//06
  IIC_MANUFACTURER_START,		//07
  IIC_MANUFACTURER_WRITE,		//08
  IIC_MANUFACTURER_READ,		//09
  IIC_TYPE_START,						//0A
  IIC_TYPE_WRITE,						//0B
  IIC_TYPE_READ,						//0C
  IIC_SETUP_WAIT,						//0D
  IIC_SETUP_START,					//0E
  IIC_SETUP_WRITE,					//0F
  IIC_SETUP_READ,						//10
  IIC_WAITING,							//11
  IIC_WRITING,							//12
  IIC_READING,							//13
  IIC_REPEAT,								//14
  IIC_ERROR,								//15
  IIC_EXIT,									//16
  IIC_STATES,								//17
	IIC_SETUP_RECEIVE					//18
};

enum TransferState {
  TRANSFER_IDLE,						//00
  TRANSFER_START,						//01
  TRANSFER_START2,					//02
  TRANSFER_ADDR,						//03
  TRANSFER_DATA,						//04
  TRANSFER_WRITE,						//05
  TRANSFER_READ,						//06
  TRANSFER_WBIT,						//07
  TRANSFER_RBIT,						//08
  TRANSFER_WACK,						//09
  TRANSFER_RACK,						//0A
  TRANSFER_STOP,						//0B
  TRANSFER_STOP2,						//0C
  TRANSFER_STOP3,						//0D
  TRANSFER_RESTART,					//0E
  TRANSFER_WAIT,						//0F
  TRANSFER_TESTING					//10
};

typedef struct
{
  UWORD   Timer;
  UWORD   Time;
  UBYTE   Initialised;
  UBYTE   ChangeMode;
  UBYTE   State;
  UBYTE   OldState;
  UBYTE   SubState;
  UBYTE   Repeat;
  UBYTE   Cmd;
  UBYTE   Mode;
  UBYTE   Addr;
  UBYTE   Retries;
  SBYTE   Name[TYPE_NAME_LENGTH + 1];
  UBYTE   Reverse;
  UBYTE   InLength;
  UBYTE   InPointer;
  UBYTE   OutLength;
  UBYTE   OutPointer;
  UBYTE   InBuffer[IIC_DATA_LENGTH];
  UBYTE   OutBuffer[IIC_DATA_LENGTH];
}
IICPORT;

typedef   struct
{
  DATA8   Port;
  DATA16  Time;
  DATA8   Type;
  DATA8   Mode;
  DATA8   Manufacturer[IIC_NAME_LENGTH + 1];
  DATA8   SensorType[IIC_NAME_LENGTH + 1];
  DATA8   SetupLng;
  ULONG   SetupString;
  DATA8   PollLng;
  ULONG   PollString;
  DATA8   ReadLng;
}
IICSTR;

struct IIC_data_package {
  uint8_t addr;
  uint8_t data[32];
  uint8_t read_length;
  uint8_t write_length;
  uint8_t clock_state;
  uint8_t bit;
  uint8_t byte;
  uint8_t data_byte;
  uint8_t transfer_state;
  uint8_t read_start_send;
  uint8_t nacked;
  uint8_t port;
  uint8_t wait_cycles;
};

struct IIC_control_t {
  // For communication to FIQ and timer functions
  struct IIC_data_package data_package[IIC_INPUTS];
  uint8_t  port_enabled;
  uint8_t  transfers_active;
};

extern UBYTE IICPortFlag[INPUTS];

extern IICSTR    IicStrings[INPUTS];

extern IICPORT   IicPort[INPUTS];

extern unsigned char iic_flag[];

RESULT IicPortSend(UBYTE Port);
void IicSdaIn(UBYTE Port);
void IicSdaOut(UBYTE Port);
void IicScl(UBYTE Port, UBYTE level);
void IicSda(UBYTE Port, UBYTE level);
UBYTE IicReadSda(UBYTE Port);

void iicDevice1TimerInterrupt1(void);
int iic_flag_handler(void);
#endif

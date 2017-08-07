/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/usart_sensor.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#include <stdio.h>
#include <string.h> 
#include "usart_sensor.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "uart4.h"
#include "stm324xg_eval.h"
#include "ui_control.h"
#include "C_Protocol.h"
#include "excutefunction.h"
#include "input.h"
u8 myi,mybuff[8];
UBYTE UsartBreakTimer[4];

UWORD Uart1RecBufOut;

UBYTE Uart1RecMesLng = 0;
UBYTE Uart1RecMes[UART_BUFFER_SIZE];
UBYTE Uart1RecMesIn = 0;
UBYTE Uart1RecCheck = 0;

UWORD Uart2RecBufOut;

UBYTE Uart2RecMesLng = 0;
UBYTE Uart2RecMes[UART_BUFFER_SIZE];
UBYTE Uart2RecMesIn = 0;
UBYTE Uart2RecCheck = 0;

UWORD Uart3RecBufOut;

UBYTE Uart3RecMesLng = 0;
UBYTE Uart3RecMes[UART_BUFFER_SIZE];
UBYTE Uart3RecMesIn = 0;
UBYTE Uart3RecCheck = 0;

UWORD Uart4RecBufOut;

UBYTE Uart4RecMesLng = 0;
UBYTE Uart4RecMes[UART_BUFFER_SIZE];
UBYTE Uart4RecMesIn = 0;
UBYTE Uart4RecCheck = 0;
	
UARTPORT  UartPortDefault =
{
  INFODATA_INIT,            // InfoData
  (ULONG)LOWEST_BITRATE,    // BitRate
  (ULONG)LOWEST_BITRATE,    // BitRateMax
  0,                        // Timer
  0,                        // WatchDog
  0,                        // BreakTimer
  0,                        // Initialised
  0,                        // ChangeMode
  UART_SYNC,                // State
  (unsigned char)-1,        // OldState
  0,                        // SubState
  0,                        // Cmd
  0,                        // InfoCmd
  0,                        // Check
  0,                        // Types
  0,                        // Views
  0,                        // Mode
  TYPE_UNKNOWN,             // Type
  0,                        // DataOk
  0,                        // DataErrors
  "",                       // Name
  0,                        // InLength
  0,                        // InPointer
  0,                        // OutLength
  0,                        // OutPointer
	1													// channel
};

UBYTE UartPortFlag[INPUTS];
UARTPORT  UartPort[INPUTS];
UBYTE     UartConfigured[INPUTS];
TYPES     TypeData[INPUTS][MAX_DEVICE_MODES]; //!< TypeData  
DATA8     Changed[INPUTS][MAX_DEVICE_MODES];

static	UBYTE		MyTypes;

TYPES     TypeDefaultUart[] =
{
//   Name                    Type                   Connection        Mode  DataSets  Format  Figures   Decimals    Views   RawMin  RawMax  PctMin  PctMax  SiMin   SiMax   Time   IdValue  Pins  Symbol
  { ""                     , TYPE_UNKNOWN         , CONN_UNKNOWN    , 0   , 1       , 1     , 4     ,   0     ,     0   ,      0.0, 1023.0,    0.0,  100.0,    0.0, 1023.0,   10,     0,    '-',  ""    },
  { "TERMINAL"             , TYPE_TERMINAL        , CONN_INPUT_UART , 0   , 0       , 0     , 4     ,   0     ,     0   ,      0.0, 4095.0,    0.0,  100.0,    0.0, 1000.0,    0,     0,    '-',  ""    },
  { "\0" }
};

void Myprintf(UBYTE Port)
{
	int i;
	
	printf("设备号:%d\n\r 模式:%d个\n\r 显示模式:%d 个\n\r 波特率:%d\n\r",
	UartPort[Port].Type,
	MyTypes,
	UartPort[Port].Views,
	UartPort[Port].BitRateMax);

	for(i = 6; i >= 0 ; i--)
	{
		printf("\n\r");
		printf("模式:%d\n\r 名称:%s\n\r RAW:%d\n\r PCT:%d\n\r SI:%d\n\r SYMBOL:%s\n\r FORMAT:%d\n\r",
		TypeData[Port][i].Mode,
		TypeData[Port][i].Name,
		TypeData[Port][i].RawMin || TypeData[Port][UartPort[Port].Types].RawMax,
		TypeData[Port][i].PctMin || TypeData[Port][UartPort[Port].Types].PctMax,
		TypeData[Port][i].SiMin  || TypeData[Port][UartPort[Port].Types].SiMax,
		TypeData[Port][i].Symbol,
		TypeData[Port][i].Format);
	}
}

void ChangeMode(UBYTE Port, UBYTE mode)
{
	uint8_t Tmp;
	Port -= 1;
	if(UartPort[Port].Mode != mode)
	{
		UartPort[Port].Mode 				= mode;
		UartPort[Port].ChangeMode   =  1;
		(*pUart).Status[Port]      &= ~UART_DATA_READY;
		
		for (Tmp = 0;Tmp < 13;Tmp++)
		{
			Channel_State[Port][Tmp]=0;
		}
		Channel_State[Port][2]=1;//模式初始化为1
	}
}

static UBYTE UartRead(UBYTE Port,UBYTE *pByte)
{
  UBYTE   Result = 0;

  switch (Port)
  {
    case Uart1 :
    {
      Result  =  Uart1Read(pByte);
    }
    break;

    case Uart2 :
    {
      Result  =  Uart2Read(pByte);
    }
    break;

    case Uart3 :
    {
      Result  =  Uart3Read(pByte);
    }
    break;

    case Uart4 :
    {
      Result  =  Uart4Read(pByte);
    }
    break;
		
		default:break;
  }
  return (Result);
}

static    UBYTE Uart1Read(UBYTE *pByte)
{
  UBYTE   Result = 0;
	
  if (Uart1RecBufIn != Uart1RecBufOut)
  {
    *pByte  =  Uart1RecBuf[Uart1RecBufOut];
		
//		printf("%c",*pByte);
		
    if (++Uart1RecBufOut >= UART_RECBUF_SIZE)
    {
      Uart1RecBufOut  =  0;
    }
    Result    =  1;
  }

  return (Result);
}

static    UBYTE Uart2Read(UBYTE *pByte)
{
  UBYTE   Result = 0;
	
  if (Uart2RecBufIn != Uart2RecBufOut)
  {
    *pByte  =  Uart2RecBuf[Uart2RecBufOut];
		
//		printf("%c",*pByte);
		
    if (++Uart2RecBufOut >= UART_RECBUF_SIZE)
    {
      Uart2RecBufOut  =  0;
    }
    Result    =  1;
  }

  return (Result);
}

static    UBYTE Uart3Read(UBYTE *pByte)
{
  UBYTE   Result = 0;
	
  if (Uart3RecBufIn != Uart3RecBufOut)
  {
    *pByte  =  Uart3RecBuf[Uart3RecBufOut];
		
//		printf("%c",*pByte);
		
    if (++Uart3RecBufOut >= UART_RECBUF_SIZE)
    {
      Uart3RecBufOut  =  0;
    }
    Result    =  1;
  }

  return (Result);
}

static    UBYTE Uart4Read(UBYTE *pByte)
{
  UBYTE   Result = 0;
	
  if (Uart4RecBufIn != Uart4RecBufOut)
  {
    *pByte  =  Uart4RecBuf[Uart4RecBufOut];
		
//		printf("%c",*pByte);
		
    if (++Uart4RecBufOut >= UART_RECBUF_SIZE)
    {
      Uart4RecBufOut  =  0;
    }
    Result    =  1;
  }

  return (Result);
}

static UBYTE UartPortReadData(UBYTE Port,UBYTE *pCmd,UBYTE *pData,UBYTE *pCheck,UBYTE *pFail)
{
  UBYTE   Result = 0;
	
  switch (Port)
  {
    case Uart1 :
    {
      Result  =  Uart1ReadData(pCmd,pData,pCheck,pFail);
    }
    break;

    case Uart2 :
    {
      Result  =  Uart2ReadData(pCmd,pData,pCheck,pFail);
    }
    break;

    case Uart3 :
    {
      Result  =  Uart3ReadData(pCmd,pData,pCheck,pFail);
    }
    break;

    case Uart4 :
    {
      Result  =  Uart4ReadData(pCmd,pData,pCheck,pFail);
    }
    break;
		
		default:break;
  }
  return (Result);
}

static UBYTE Uart1ReadData(UBYTE *pCmd,UBYTE *pData,UBYTE *pCheck,UBYTE *pFail)
{
  UBYTE   Byte;
  UBYTE   Length;
  UBYTE   Collect;

  Length    =  0;
  *pFail    =  0xFF;
  Collect   =  1;

  while (Collect)
  {
//			USART1_Send_Byte(0xff);
    if (Uart1Read(&Byte))
    {
      if (Uart1RecMesIn == 0)
      { // Wait for data message start

        if (GET_MESSAGE_TYPE(Byte) == MESSAGE_DATA)
        {

          Uart1RecMesLng  =  GET_MESSAGE_LENGTH(Byte) + 2;

          if (Uart1RecMesLng <= UART_BUFFER_SIZE)
          { // Valid length

            Uart1RecMes[Uart1RecMesIn]  =  Byte;
            Uart1RecMesIn++;
          }
        }
      }
      else
      {
        Uart1RecMes[Uart1RecMesIn]  =  Byte;

        if (++Uart1RecMesIn >= Uart1RecMesLng)
        { // Message ready
          *pCmd    =  Uart1RecMes[0];
          *pFail  ^=  *pCmd;

          while (Length < (Uart1RecMesLng - 2))
          {
            pData[Length]  = Uart1RecMes[Length + 1];
            *pFail       ^=  pData[Length];
            Length++;
          }
          *pCheck  =  Uart1RecMes[Length + 1];
//					*pFail  ^=  *pCheck;
					
          if(*pFail  ==  *pCheck)
					{
						Uart1RecCheck = 1;

//						if(Uart1RecMes[1] == 0xd9)
//						{
//							for(myi=0; myi<8;myi++)
//							{
//								mybuff[myi] = Uart1RecMes[myi];
//							}
//							for(myi=0; myi<8;myi++)
//							{
//								printf("mybuff[%x]:%x\n\r",myi,mybuff[myi]);
//							}
//							printf("pFail:%x\n\r",*pFail);
//							printf("pCheck:%x\n\r",*pCheck);
//							printf("Uart1RecMes[1]:%x\n\r",Uart1RecMes[1]);
//						}
						
					}else
					{
						Uart1RecCheck = 0;
					}

          Uart1RecMesIn  =  0;
          Collect  =  0;
        }
      }
    }
    else
    {
      Collect  =  0;
    }
  }

  return (Length);
}

static UBYTE Uart2ReadData(UBYTE *pCmd,UBYTE *pData,UBYTE *pCheck,UBYTE *pFail)
{
  UBYTE   Byte;
  UBYTE   Length;
  UBYTE   Collect;

  Length    =  0;
  *pFail    =  0xFF;
  Collect   =  1;

  while (Collect)
  {
//			USART1_Send_Byte(0xff);
    if (Uart2Read(&Byte))
    {
      if (Uart2RecMesIn == 0)
      { // Wait for data message start

        if (GET_MESSAGE_TYPE(Byte) == MESSAGE_DATA)
        {

          Uart2RecMesLng  =  GET_MESSAGE_LENGTH(Byte) + 2;

          if (Uart2RecMesLng <= UART_BUFFER_SIZE)
          { // Valid length

            Uart2RecMes[Uart2RecMesIn]  =  Byte;
            Uart2RecMesIn++;
          }
        }
      }
      else
      {
        Uart2RecMes[Uart2RecMesIn]  =  Byte;

        if (++Uart2RecMesIn >= Uart2RecMesLng)
        { // Message ready
          *pCmd    =  Uart2RecMes[0];
          *pFail  ^=  *pCmd;

          while (Length < (Uart2RecMesLng - 2))
          {
            pData[Length]  = Uart2RecMes[Length + 1];
            *pFail       ^=  pData[Length];
            Length++;
          }
          *pCheck  =  Uart2RecMes[Length + 1];
//          *pFail  ^=  *pCheck;

					if(*pFail  ==  *pCheck)
					{
						Uart2RecCheck = 1;
					}else
					{
						Uart2RecCheck = 0;
					}
					
          Uart2RecMesIn  =  0;
          Collect  =  0;
        }
      }
    }
    else
    {
      Collect  =  0;
    }
  }

  return (Length);
}

static UBYTE Uart3ReadData(UBYTE *pCmd,UBYTE *pData,UBYTE *pCheck,UBYTE *pFail)
{
  UBYTE   Byte;
  UBYTE   Length;
  UBYTE   Collect;

  Length    =  0;
  *pFail    =  0xFF;
  Collect   =  1;

  while (Collect)
  {
//			USART1_Send_Byte(0xff);
    if (Uart3Read(&Byte))
    {
      if (Uart3RecMesIn == 0)
      { // Wait for data message start

        if (GET_MESSAGE_TYPE(Byte) == MESSAGE_DATA)
        {

          Uart3RecMesLng  =  GET_MESSAGE_LENGTH(Byte) + 2;

          if (Uart3RecMesLng <= UART_BUFFER_SIZE)
          { // Valid length

            Uart3RecMes[Uart3RecMesIn]  =  Byte;
            Uart3RecMesIn++;
          }
        }
      }
      else
      {
        Uart3RecMes[Uart3RecMesIn]  =  Byte;

        if (++Uart3RecMesIn >= Uart3RecMesLng)
        { // Message ready
          *pCmd    =  Uart3RecMes[0];
          *pFail  ^=  *pCmd;

          while (Length < (Uart3RecMesLng - 2))
          {
            pData[Length]  = Uart3RecMes[Length + 1];
            *pFail       ^=  pData[Length];
            Length++;
          }
          *pCheck  =  Uart3RecMes[Length + 1];
//          *pFail  ^=  *pCheck;

					if(*pFail  ==  *pCheck)
					{
						Uart3RecCheck = 1;
					}else
					{
						Uart3RecCheck = 0;
					}
					
          Uart3RecMesIn  =  0;
          Collect  =  0;
        }
      }
    }
    else
    {
      Collect  =  0;
    }
  }

  return (Length);
}

static UBYTE Uart4ReadData(UBYTE *pCmd,UBYTE *pData,UBYTE *pCheck,UBYTE *pFail)
{
  UBYTE   Byte;
  UBYTE   Length;
  UBYTE   Collect;

  Length    =  0;
  *pFail    =  0xFF;
  Collect   =  1;

  while (Collect)
  {
//			USART1_Send_Byte(0xff);
    if (Uart4Read(&Byte))
    {
      if (Uart4RecMesIn == 0)
      { // Wait for data message start

        if (GET_MESSAGE_TYPE(Byte) == MESSAGE_DATA)
        {

          Uart4RecMesLng  =  GET_MESSAGE_LENGTH(Byte) + 2;

          if (Uart4RecMesLng <= UART_BUFFER_SIZE)
          { // Valid length

            Uart4RecMes[Uart4RecMesIn]  =  Byte;
            Uart4RecMesIn++;
          }
        }
      }
      else
      {
        Uart4RecMes[Uart4RecMesIn]  =  Byte;

        if (++Uart4RecMesIn >= Uart4RecMesLng)
        { // Message ready
          *pCmd    =  Uart4RecMes[0];
          *pFail  ^=  *pCmd;

          while (Length < (Uart4RecMesLng - 2))
          {
            pData[Length]  = Uart4RecMes[Length + 1];
            *pFail       ^=  pData[Length];
            Length++;
          }
          *pCheck  =  Uart4RecMes[Length + 1];
//          *pFail  ^=  *pCheck;

					if(*pFail  ==  *pCheck)
					{
						Uart4RecCheck = 1;
					}else
					{
						Uart4RecCheck = 0;
					}
					
          Uart4RecMesIn  =  0;
          Collect  =  0;
        }
      }
    }
    else
    {
      Collect  =  0;
    }
  }

  return (Length);
}

void UsartChange(UBYTE Port, u32 bound)
{
	switch (Port)
  {
    case Uart1 :
    {
      usart4_change(bound);
    }
    break;

    case Uart2 :
    {
      usart2_change(bound);
    }
    break;

    case Uart3 :
    {
      usart1_change(bound);
    }
    break;

    case Uart4 :
    {
      usart3_change(bound);
    }
    break;
		
		default:break;
  }
}

static UBYTE UartPortSend(UBYTE Port, UBYTE ack)
{
  UBYTE   Result = 0;
	
	switch (Port)
  {
    case Uart1 :
    {
      Result = USART4_Send_Byte(ack);
    }
    break;

    case Uart2 :
    {
      Result = USART2_Send_Byte(ack);
    }
    break;

    case Uart3 :
    {
      Result = USART1_Send_Byte(ack);
    }
    break;

    case Uart4 :
    {
      Result = USART3_Send_Byte(ack);
    }
    break;
		
		default:break;
  }
  return (Result);
}

//1ms 中断一次
void UartDevice1TimerInterrupt1(void)
{
	u8 value1,value2,value3,value4,value5,value6,value7,value8;
  UBYTE   Port = 0;
  UBYTE   Byte;
  UBYTE   CrcError = 0;
  UBYTE   Tmp = 0;
  ULONG   TmpL;
//  UBYTE   Chksum;
//  UBYTE   Pointer;
  UBYTE   Mode;
  UBYTE   TmpBuffer[UART_DATA_LENGTH]={0};
	
//#ifdef  DEBUG_TRACE_US
//  UWORD   In;
//#endif
//#ifdef  DEBUG_TRACE_ANGLE
//  UWORD   Angle;
//#endif
	
//  hrtimer_forward_now(pTimer,Device1Time);
//	printf("%c",UartPort[Port].State);
//	printf("%c",UsartBreakTimer);
//	USART1_Send_Byte(UartPort[Port].State);
	
  for (Port = 0;Port < 4;Port++)  //NO_OF_INPUT_PORTS
  { // look at one port at a time

//    if ((UartPort[Port].State > UART_ENABLE) && (!TestMode))
//    { // If port active
			
		if(UartPortFlag[Port])
		{
			//传感器缓存区
			Channel_State[Port][0]=Port+1;
			
			Channel_State[Port][1]=LegoIdtoEstId(UartPort[Port].Type);
			
			if(UartPort[Port].Mode == 0x11 && UartPort[Port].OutPointer == 3)//重置模式复位
			{
				UartPort[Port].Mode = 0x00;
			}
			
			Channel_State[Port][2]=LegoModetoEstMode(UartPort[Port].Type,UartPort[Port].Mode);
			
			if(UartPort[Port].Type==29)//颜色
			{
				switch(Port)
				{
					case 0:
//						check = (0xFF^Uart1RecMes[0]^Uart1RecMes[1]^Uart1RecMes[2]);
						if(Uart1RecCheck == 1)//校验
						{
							value1 = Uart1RecMes[1];
							if(UartPort[Port].Mode == 2 && value1 <= 8 )//范围限定
							{
								Channel_State[Port][3] = value1;
							}else if(value1 <= 100)
							{
								FloatToByte4(value1, Channel_State[Port], 0);
							}
							Uart1RecCheck = 0;
						}
						break;
					case 1:
						if(Uart2RecCheck == 1)//校验
						{
							value1 = Uart2RecMes[1];
							if(UartPort[Port].Mode == 2 && value1 <= 8)//范围限定
							{
								 Channel_State[Port][3] = value1;
							}else if(value1 <= 100)
							{
								FloatToByte4(value1, Channel_State[Port], 0);
							}
							Uart2RecCheck = 0;
						}
						break;
					case 2:
						if(Uart3RecCheck == 1)//校验
						{
							value1 = Uart3RecMes[1];
							if(UartPort[Port].Mode == 2 && value1 <= 8)//范围限定
							{
								 Channel_State[Port][3] = value1;
							}else if(value1 <= 100 )
							{
								FloatToByte4(value1, Channel_State[Port], 0);
							}
							Uart3RecCheck = 0;
						}
						break;
					case 3:
						if(Uart4RecCheck == 1)//校验
						{
							value1 = Uart4RecMes[1];
							if(UartPort[Port].Mode == 2 && value1 <= 8)//范围限定
							{
								 Channel_State[Port][3] = value1;
							}else if(value1 <= 100)
							{
								FloatToByte4(value1, Channel_State[Port], 0);
							}
							Uart4RecCheck = 0;
						}
						break;
						
					default:break;
				}
			}else if(UartPort[Port].Type==33 )//红外
			{
				switch(Port)
				{
					case 0:
						if(Uart1RecCheck == 1)
						{
							value1 = Uart1RecMes[1];value2 = Uart1RecMes[2];value3 = Uart1RecMes[3];value4 = Uart1RecMes[4];
							value5 = Uart1RecMes[5];value6 = Uart1RecMes[6];value7 = Uart1RecMes[7];value8 = Uart1RecMes[8];
							if(UartPort[Port].Mode == 0 && value1 <= 100 )
							{
								FloatToByte4(value1, Channel_State[Port], 0);
							}else if(UartPort[Port].Mode == 1)
							{
								Channel_State[Port][3] = UartPort[Port].channel;
								if(UartPort[Port].channel == 0 && value2 <= 100)//channel
								{
									FloatToByte4(value1, Channel_State[Port], 1);
									FloatToByte4(value2, Channel_State[Port], 5);
									if(Uart1RecMes[2] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}else if(UartPort[Port].channel == 1 && value4 <= 100)
								{
									FloatToByte4(value3, Channel_State[Port], 1);
									FloatToByte4(value4, Channel_State[Port], 5);
									if(Uart1RecMes[4] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}else if(UartPort[Port].channel == 2 && value6 <= 100)
								{
									FloatToByte4(value5, Channel_State[Port], 1);
									FloatToByte4(value6, Channel_State[Port], 5);
									if(Uart1RecMes[6] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}else if(UartPort[Port].channel == 3 && value8 <= 100)
								{
									FloatToByte4(value7, Channel_State[Port], 1);
									FloatToByte4(value8, Channel_State[Port], 5);
									if(Uart1RecMes[8] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}
							}else if(UartPort[Port].Mode == 2)
							{
								Channel_State[Port][3] = UartPort[Port].channel;
								if(UartPort[Port].channel == 0 && value1 <= 12)//channel
								{
									Channel_State[Port][4] = value1;
								}else if(UartPort[Port].channel == 1 && value2 <= 12 )
								{
									Channel_State[Port][4] = value2;
								}else if(UartPort[Port].channel == 2 && value3 <= 12)
								{
									Channel_State[Port][4] = value3;
								}else if(UartPort[Port].channel == 3 && value4 <= 12)
								{
									Channel_State[Port][4] = value4;
								}
							}
							Uart1RecCheck = 0;
						}
						break;
					case 1:
						if(Uart2RecCheck == 1)
						{
							value1 = Uart2RecMes[1];value2 = Uart2RecMes[2];value3 = Uart2RecMes[3];value4 = Uart2RecMes[4];
							value5 = Uart2RecMes[5];value6 = Uart2RecMes[6];value7 = Uart2RecMes[7];value8 = Uart2RecMes[8];
							if(UartPort[Port].Mode == 0 && value1 <= 100)
							{
								FloatToByte4(value1, Channel_State[Port], 0);
							}else if(UartPort[Port].Mode == 1)
							{
								Channel_State[Port][3] = UartPort[Port].channel;
								if(UartPort[Port].channel == 0 && value2 <= 100)//channel
								{
									FloatToByte4(value1, Channel_State[Port], 1);
									FloatToByte4(value2, Channel_State[Port], 5);
									if(Uart1RecMes[2] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}else if(UartPort[Port].channel == 1 && value4 <= 100)
								{
									FloatToByte4(value3, Channel_State[Port], 1);
									FloatToByte4(value4, Channel_State[Port], 5);
									if(Uart1RecMes[4] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}else if(UartPort[Port].channel == 2 && value6 <= 100)
								{
									FloatToByte4(value5, Channel_State[Port], 1);
									FloatToByte4(value6, Channel_State[Port], 5);
									if(Uart1RecMes[6] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}else if(UartPort[Port].channel == 3 && value8 <= 100)
								{
									FloatToByte4(value7, Channel_State[Port], 1);
									FloatToByte4(value8, Channel_State[Port], 5);
									if(Uart1RecMes[8] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}
							}else if(UartPort[Port].Mode == 2)
							{
								Channel_State[Port][3] = UartPort[Port].channel;
								if(UartPort[Port].channel == 0 && value1 <= 12)//channel
								{
									Channel_State[Port][4] = value1;
								}else if(UartPort[Port].channel == 1 && value2 <= 12)
								{
									Channel_State[Port][4] = value2;
								}else if(UartPort[Port].channel == 2 && value3 <= 12)
								{
									Channel_State[Port][4] = value3;
								}else if(UartPort[Port].channel == 3 && value4 <= 12)
								{
									Channel_State[Port][4] = value4;
								}
							}
							Uart2RecCheck = 0;
						}
						break;
					case 2:
						if(Uart3RecCheck == 1)
						{
							value1 = Uart3RecMes[1];value2 = Uart3RecMes[2];value3 = Uart3RecMes[3];value4 = Uart3RecMes[4];
							value5 = Uart3RecMes[5];value6 = Uart3RecMes[6];value7 = Uart3RecMes[7];value8 = Uart3RecMes[8];
							if(UartPort[Port].Mode == 0 && value1 <= 100 )
							{
								FloatToByte4(value1, Channel_State[Port], 0);
							}else if(UartPort[Port].Mode == 1)
							{
								Channel_State[Port][3] = UartPort[Port].channel;
								if(UartPort[Port].channel == 0 && value2 <= 100)//channel
								{
									FloatToByte4(value1, Channel_State[Port], 1);
									FloatToByte4(value2, Channel_State[Port], 5);
									if(Uart1RecMes[2] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}else if(UartPort[Port].channel == 1 && value4 <= 100)
								{
									FloatToByte4(value3, Channel_State[Port], 1);
									FloatToByte4(value4, Channel_State[Port], 5);
									if(Uart1RecMes[4] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}else if(UartPort[Port].channel == 2 && value6 <= 100)
								{
									FloatToByte4(value5, Channel_State[Port], 1);
									FloatToByte4(value6, Channel_State[Port], 5);
									if(Uart1RecMes[6] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}else if(UartPort[Port].channel == 3 && value8 <= 100)
								{
									FloatToByte4(value7, Channel_State[Port], 1);
									FloatToByte4(value8, Channel_State[Port], 5);
									if(Uart1RecMes[8] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}
							}else if(UartPort[Port].Mode == 2)
							{
								Channel_State[Port][3] = UartPort[Port].channel;
								if(UartPort[Port].channel == 0 && value1 <= 12)//channel
								{
									Channel_State[Port][4] = value1;
								}else if(UartPort[Port].channel == 1 && value2 <= 12)
								{
									Channel_State[Port][4] = value2;
								}else if(UartPort[Port].channel == 2 && value3 <= 12)
								{
									Channel_State[Port][4] = value3;
								}else if(UartPort[Port].channel == 3 && value4 <= 12)
								{
									Channel_State[Port][4] = value4;
								}
							}
							Uart3RecCheck = 0;
						}
						break;
					case 3:
						if(Uart4RecCheck == 1)
						{
							value1 = Uart4RecMes[1];value2 = Uart4RecMes[2];value3 = Uart4RecMes[3];value4 = Uart4RecMes[4];
							value5 = Uart4RecMes[5];value6 = Uart4RecMes[6];value7 = Uart4RecMes[7];value8 = Uart4RecMes[8];
							if(UartPort[Port].Mode == 0 && value1 <= 100)
							{
								FloatToByte4(value1, Channel_State[Port], 0);
							}else if(UartPort[Port].Mode == 1)
							{
								Channel_State[Port][3] = UartPort[Port].channel;
								if(UartPort[Port].channel == 0 && value2 <= 100)//channel
								{
									FloatToByte4(value1, Channel_State[Port], 1);
									FloatToByte4(value2, Channel_State[Port], 5);
									if(Uart1RecMes[2] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}else if(UartPort[Port].channel == 1 && value4 <= 100)
								{
									FloatToByte4(value3, Channel_State[Port], 1);
									FloatToByte4(value4, Channel_State[Port], 5);
									if(Uart1RecMes[4] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}else if(UartPort[Port].channel == 2 && value6 <= 100)
								{
									FloatToByte4(value5, Channel_State[Port], 1);
									FloatToByte4(value6, Channel_State[Port], 5);
									if(Uart1RecMes[6] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}else if(UartPort[Port].channel == 3 && value8 <= 100)
								{
									FloatToByte4(value7, Channel_State[Port], 1);
									FloatToByte4(value8, Channel_State[Port], 5);
									if(Uart1RecMes[8] != 0x80 )
										Channel_State[Port][12] = 0x01;
									else
										Channel_State[Port][12] = 0x00;
								}
							}else if(UartPort[Port].Mode == 2)
							{
								Channel_State[Port][3] = UartPort[Port].channel;
								if(UartPort[Port].channel == 0 && value1 <= 12)//channel
								{
									Channel_State[Port][4] = value1;
								}else if(UartPort[Port].channel == 1 && value2 <= 12)
								{
									Channel_State[Port][4] = value2;
								}else if(UartPort[Port].channel == 2 && value3 <= 12)
								{
									Channel_State[Port][4] = value3;
								}else if(UartPort[Port].channel == 3 && value4 <= 12)
								{
									Channel_State[Port][4] = value4;
								}
							}
							Uart4RecCheck = 0;
						}
						break;
							
					default:break;
				}
			}else if(UartPort[Port].Type == 30 )//超声波
			{
				switch(Port)
				{
					case 0:
						if(Uart1RecCheck == 1)
						{
							value1 = Uart1RecMes[1]; value2 = Uart1RecMes[2];
							if(UartPort[Port].Mode == 2 && value1 <= 2)
							{
								Channel_State[Port][3] = value1;
							}else if((UartPort[Port].Mode == 3 || UartPort[Port].Mode == 4 ) && value2 <= 0x0a)
							{
								FloatToByte4((s16)((u16)value1 | (((u16)value2)<<8))/10, Channel_State[Port], 1);
							}else if( value2 <= 0x0a)
							{
								FloatToByte4((s16)((u16)value1 | (((u16)value2)<<8))/10, Channel_State[Port], 0);
							}
							Uart1RecCheck = 0;
						}
						break;
					case 1:
						if(Uart2RecCheck == 1)
						{
							value1 = Uart2RecMes[1]; value2 = Uart2RecMes[2];
							if(UartPort[Port].Mode == 2 && value1 <= 2)
							{
								Channel_State[Port][3] = value1;
							}else if((UartPort[Port].Mode == 3 || UartPort[Port].Mode == 4 ) && value2 <= 0x0a)
							{
								FloatToByte4((s16)((u16)value1 | (((u16)value2)<<8))/10, Channel_State[Port], 1);
							}else if( value2 <= 0x0a)
							{
								FloatToByte4((s16)((u16)value1 | (((u16)value2)<<8))/10, Channel_State[Port], 0);
							}
							Uart2RecCheck = 0;
						}
						break;
					case 2:
						if(Uart3RecCheck == 1)
						{
							value1 = Uart3RecMes[1]; value2 = Uart3RecMes[2];
							if(UartPort[Port].Mode == 2 && value1 <= 2)
							{
								Channel_State[Port][3] = value1;
							}else if((UartPort[Port].Mode == 3 || UartPort[Port].Mode == 4 ) && value2 <= 0x0a)
							{
								FloatToByte4((s16)((u16)value1 | (((u16)value2)<<8))/10, Channel_State[Port], 1);
							}else if( value2 <= 0x0a)
							{
								FloatToByte4((s16)((u16)value1 | (((u16)value2)<<8))/10, Channel_State[Port], 0);
							}
							Uart3RecCheck = 0;
						}
						break;
					case 3:
						if(Uart4RecCheck == 1)
						{
							value1 = Uart4RecMes[1]; value2 = Uart4RecMes[2];
							if(UartPort[Port].Mode == 2 && value1 <= 2)
							{
								Channel_State[Port][3] = value1;
							}else if((UartPort[Port].Mode == 3 || UartPort[Port].Mode == 4 ) && value2 <= 0x0a)
							{
								FloatToByte4((s16)((u16)value1 | (((u16)value2)<<8))/10, Channel_State[Port], 1);
							}else if( value2 <= 0x0a)
							{
								FloatToByte4((s16)((u16)value1 | (((u16)value2)<<8))/10, Channel_State[Port], 0);
							}
							Uart4RecCheck = 0;
						}
						break;
							
					default:break;
				}
			}else if(UartPort[Port].Type==32 )//陀螺仪
			{
				switch(Port)
				{
					case 0:
						if(Uart1RecCheck == 1)
						{
							FloatToByte4((s16)((u16)Uart1RecMes[1] | (((u16)Uart1RecMes[2])<<8)), Channel_State[Port], 0);
							if(UartPort[Port].Mode == 3)
							{
								FloatToByte4((s16)((u16)Uart1RecMes[3] | (((u16)Uart1RecMes[4])<<8)), Channel_State[Port], 4);
							}
							Uart1RecCheck = 0;
						}
						break;
					case 1:
						if(Uart2RecCheck == 1)
						{
							FloatToByte4((s16)((u16)Uart2RecMes[1] | (((u16)Uart2RecMes[2])<<8)), Channel_State[Port], 0);
							if(UartPort[Port].Mode == 3)
							{
								FloatToByte4((s16)((u16)Uart2RecMes[3] | (((u16)Uart2RecMes[4])<<8)), Channel_State[Port], 4);
							}
							Uart2RecCheck = 0;
						}
						break;
					case 2:
						if(Uart3RecCheck == 1)
						{
							FloatToByte4((s16)((u16)Uart3RecMes[1] | (((u16)Uart3RecMes[2])<<8)), Channel_State[Port], 0);
							if(UartPort[Port].Mode == 3)
							{
								FloatToByte4((s16)((u16)Uart3RecMes[3] | (((u16)Uart3RecMes[4])<<8)), Channel_State[Port], 4);
							}
							Uart3RecCheck = 0;
						}
						break;
					case 3:
						if(Uart4RecCheck == 1)
						{
							FloatToByte4((s16)((u16)Uart4RecMes[1] | (((u16)Uart4RecMes[2])<<8)), Channel_State[Port], 0);
							if(UartPort[Port].Mode == 3)
							{	
								FloatToByte4((s16)((u16)Uart4RecMes[3] | (((u16)Uart4RecMes[4])<<8)), Channel_State[Port], 4);
							}
							Uart4RecCheck = 0;
						}
						break;
						
					default:break;
				}
			}
			
		if (++UsartBreakTimer[Port] >= 2*(UART_BREAK_TIME / UART_TIMER_RESOLUTION))
		{ // Reset state machine if break received

//        if (PUARTRead(Port,INPUT_UART_PIN6))
//        {

//#ifdef DEBUG_D_UART_ERROR
//          snprintf(UartBuffer,UARTBUFFERSIZE,"    %d BREAK\r\n",Port);
//          UartWrite(UartBuffer);
//#endif

//          UartPortDisable(Port);
			
				UartPort[Port]                          =  UartPortDefault;
			
//          UartPortEnable(Port);
//          UartPortSetup(Port,UartPort[Port].BitRate);
			UsartChange(Port,UartPort[Port].BitRate);
			
				for (Tmp = 0;Tmp < MAX_DEVICE_MODES;Tmp++)
				{
					TypeData[Port][Tmp]           =  TypeDefaultUart[0];
					Changed[Port][Tmp]            =  0;
				}
//#ifndef DISABLE_FAST_DATALOG_BUFFER
//          (*pUart).Actual[Port]           =  0;
//          (*pUart).LogIn[Port]            =  0;
//#endif
				(*pUart).Status[Port]           =  0;
				
//          UartPortFlush(Port);
				
					UartPort[Port].State            =  UART_SYNC;
				
//					printf("%c",UART_SYNC);
//        }
//				if (!GPIO_ReadInputDataBit(GPIOB,GPIO_PinSource7))
//				{
//					UartPort[Port].BreakTimer  =  0;
//				}
				UsartBreakTimer[Port] =0;
		}
		}else{;}
			
    if (UartPortFlag[Port])  //Port != DEBUG_UART
    { // If port not used for debug

//  #ifdef DEBUG
//      ShowTimer[Port]++;
//  #endif

      if (!TestMode)
      {

        switch (UartPort[Port].State)
        { // Main state machine

          case UART_IDLE :
          { // Port inactive

            (*pUart).Status[Port] &= ~UART_WRITE_REQUEST;
            (*pUart).Status[Port] &= ~UART_DATA_READY;
//            WriteRequest[Port]     =  0;
          }
          break;

          case UART_INIT :
          { // Initialise port hardware

//            PUARTFloat(Port,INPUT_UART_PIN5);
//            PUARTFloat(Port,INPUT_UART_PIN6);
            UartPort[Port].State        =  UART_ENABLE;
          }
          break;

          case UART_RESTART :
          {
//            UartPortDisable(Port);
            UartPort[Port].State        =  UART_ENABLE;
          }
          break;

          case UART_ENABLE :
          { // Initialise port variables

            UartPort[Port]                          =  UartPortDefault;
//            UartPortEnable(Port);
//            UartPortSetup(Port,UartPort[Port].BitRate);
						UsartChange(Port,UartPort[Port].BitRate);
						
            for (Tmp = 0;Tmp < MAX_DEVICE_MODES;Tmp++)
            {
              TypeData[Port][Tmp]           =  TypeDefaultUart[0];
              Changed[Port][Tmp]            =  0;
            }
//  #ifndef DISABLE_FAST_DATALOG_BUFFER
//            (*pUart).Actual[Port]           =  0;
//            (*pUart).LogIn[Port]            =  0;
//  #endif
            (*pUart).Status[Port]           =  0;
//            UartPortFlush(Port);
            UartPort[Port].State            =  UART_SYNC;
          }
          break;

          case UART_SYNC :
          { // Look for UART_CMD, TYPE in rolling buffer windows
//						if (Uart1Read(&Byte))
            if (UartRead(Port, &Byte))
            {
//							printf("%c",Byte);							
//							USART1_Send_Byte(0x00);
              if (UartPort[Port].InPointer < 3)
              { // 0,1,2
                UartPort[Port].InBuffer[UartPort[Port].InPointer]  =  Byte;
                UartPort[Port].InPointer++;
              }
              if (UartPort[Port].InPointer >= 3)
              {
                // Validate
                UartPort[Port].Check      =  0xFF;
                for (Tmp = 0;Tmp < 2;Tmp++)
                {
                  UartPort[Port].Check   ^=  UartPort[Port].InBuffer[Tmp];
                }
                if ((UartPort[Port].Check == UartPort[Port].InBuffer[2]) && (UartPort[Port].InBuffer[0] == 0x40) && (UartPort[Port].InBuffer[1] > 0) && (UartPort[Port].InBuffer[1] <= MAX_VALID_TYPE))
                {
									
                  UartPort[Port].Type       =  UartPort[Port].InBuffer[1];
                  UartPort[Port].InfoData  |=  INFODATA_CMD_TYPE;
									
//  #ifdef HIGHDEBUG
//                  snprintf(UartBuffer,UARTBUFFERSIZE,"    %d TYPE   = %-3u\r\n",Port,(UWORD)UartPort[Port].Type & 0xFF);
//                  UartWrite(UartBuffer);
//  #endif
                  UartPort[Port].State      =  UART_MESSAGE_START;
									
                }
                else
                {
//  #ifdef DEBUG_D_UART_ERROR
//  //                snprintf(UartBuffer,UARTBUFFERSIZE,"[%02X]",Byte);
//  //                UartWrite(UartBuffer);
//  //                snprintf(UartBuffer,UARTBUFFERSIZE,"    %d No sync %02X %02X %02X\r\n",Port,UartPort[Port].InBuffer[0],UartPort[Port].InBuffer[1],UartPort[Port].InBuffer[2]);
//  //                UartWrite(UartBuffer);
//  #endif
                  for (Tmp = 0;Tmp < 2;Tmp++)
                  {
                    UartPort[Port].InBuffer[Tmp]  =  UartPort[Port].InBuffer[Tmp + 1];
                  }

                  UartPort[Port].InPointer--;
                }
              }
            }
//            if ((++(UartPort[Port].Timer) >= (UART_TERMINAL_DELAY / UART_TIMER_RESOLUTION)))
//            {
//              UartPort[Port].BitRate      =  115200;
////              UartPortSetup(Port,UartPort[Port].BitRate);
//							usart1_change(UartPort[Port].BitRate);
//							
//              TypeData[Port][0]           =  TypeDefaultUart[1];
//              UartPort[Port].State        =  UART_TERMINAL;
//              Changed[Port][0]            =  1;
//              (*pUart).Status[Port]      |=  UART_PORT_CHANGED;
//            }
          }
          break;

          default :
          { // Get sensor informations
//						if(Uart1Read(&Byte))
            if (UartRead(Port, &Byte))
            {
//							printf("%c", Byte);
							
              switch (UartPort[Port].State)
              {

  //** INTERPRETER **************************************************************
								
                case UART_MESSAGE_START :
                {
                  UartPort[Port].InPointer  =  0;
                  UartPort[Port].SubState   =  0;
                  UartPort[Port].Check      =  0xFF;
                  UartPort[Port].Cmd        =  Byte;
									
//									printf("%c", Byte); //帧头
									
                  switch (GET_MESSAGE_TYPE(Byte))
                  {
                    case MESSAGE_CMD :
                    {
                      UartPort[Port].InLength   =  GET_MESSAGE_LENGTH(Byte);
                      UartPort[Port].State      =  UART_CMD;
                    }
                    break;

                    case MESSAGE_INFO :
                    {
                      UartPort[Port].InLength   =  GET_MESSAGE_LENGTH(Byte);
                      UartPort[Port].State      =  UART_INFO;
                    }
                    break;

                    case MESSAGE_DATA :
                    {
                    }
                    break;

                    default :
                    {
                      switch (Byte)
                      {
                        case BYTE_ACK :
                        {
//    #ifdef HIGHDEBUG
//                          snprintf(UartBuffer,UARTBUFFERSIZE,"    %d ACK RECEIVED\r\n",Port);
//                          UartWrite(UartBuffer);
//    #endif
													
                          if (UartPort[Port].Types == 0)
                          {
                            if ((UartPort[Port].InfoData & INFODATA_NEEDED) == INFODATA_NEEDED)
                            {
                              UartPort[Port].Timer    =  0;
                              UartPort[Port].State    =  UART_ACK_WAIT;
                            }
                            else
                            {
                              UartPort[Port].State  =  UART_INFO_ERROR;
                            }
                          }
                          else
                          {
                            UartPort[Port].State  =  UART_INFO_ERROR;
                          }
                        }
                        break;

                        case BYTE_NACK :
                        {
                        }
                        break;

                        case BYTE_SYNC :
                        {
                        }
                        break;

                        default :
                        {
                        }
                        break;

                      }

                    }
                    break;

                  }
                }
                break;

  //** CMD **********************************************************************

                case UART_CMD :
                { // Command message in progress

                  if (UartPort[Port].InPointer < UartPort[Port].InLength)
                  {
                    UartPort[Port].InBuffer[UartPort[Port].InPointer]  =  Byte;
                    UartPort[Port].InPointer++;
                  }
                  else
                  { // Message complete

                    UartPort[Port].InBuffer[UartPort[Port].InPointer]  =  0;
                    UartPort[Port].State  =  UART_MESSAGE_START;

                    if (UartPort[Port].Check !=  Byte)
                    { // Check not correct
//  #ifdef DEBUG_D_UART_ERROR
//                      snprintf(UartBuffer,UARTBUFFERSIZE," c  %d %02X[",Port,UartPort[Port].Cmd & 0xFF);
//                      UartWrite(UartBuffer);
//                      for (Tmp = 0;Tmp < UartPort[Port].InLength;Tmp++)
//                      {
//                        snprintf(UartBuffer,UARTBUFFERSIZE,"%02X",UartPort[Port].InBuffer[Tmp] & 0xFF);
//                        UartWrite(UartBuffer);
//                      }
//                      snprintf(UartBuffer,UARTBUFFERSIZE,"]%02X\r\n",UartPort[Port].Check & 0xFF);
//                      UartWrite(UartBuffer);
//  #endif
                      UartPort[Port].State  =  UART_CMD_ERROR;
                    }
                    else
                    { // Command message valid
											
//											printf("%c", Byte); //UART_CMD帧尾
											
//  #ifdef HIGHDEBUG
//                      snprintf(UartBuffer,UARTBUFFERSIZE,"    %d %02X[",Port,UartPort[Port].Cmd & 0xFF);
//                      UartWrite(UartBuffer);
//                      for (Tmp = 0;Tmp < UartPort[Port].InLength;Tmp++)
//                      {
//                        snprintf(UartBuffer,UARTBUFFERSIZE,"%02X",UartPort[Port].InBuffer[Tmp] & 0xFF);
//                        UartWrite(UartBuffer);
//                      }
//                      snprintf(UartBuffer,UARTBUFFERSIZE,"]%02X\r\n",UartPort[Port].Check & 0xFF);
//                      UartWrite(UartBuffer);
//  #endif
											
                      switch (GET_CMD_COMMAND(UartPort[Port].Cmd))
                      { // Command message type

                        case CMD_MODES :
                        { // Number of modes
													
                          if (/*(UartPort[Port].InBuffer[0] >= 0) && */(UartPort[Port].InBuffer[0] < MAX_DEVICE_MODES))
                          { // Number of modes valid

                            if ((UartPort[Port].InfoData & INFODATA_CMD_MODES))
                            { // Modes already given

//  #ifdef DEBUG_D_UART_ERROR
//                              snprintf(UartBuffer,UARTBUFFERSIZE," ## %d MODES ALREADY GIVEN\r\n",Port);
//                              UartWrite(UartBuffer);
//  #endif
                              UartPort[Port].State  =  UART_CMD_ERROR;
                            }
                            else
                            {

                              UartPort[Port].Types  =  UartPort[Port].InBuffer[0] + 1;
															MyTypes 							=  UartPort[Port].Types;
                              if (UartPort[Port].InLength >= 2)
                              { // Both modes and views present

                                UartPort[Port].Views  =  UartPort[Port].InBuffer[1] + 1;
																
//													printf("%c", UartPort[Port].Views);
//  #ifdef HIGHDEBUG
//                                snprintf(UartBuffer,UARTBUFFERSIZE,"    %d MODES  = %u  VIEWS  = %u\r\n",Port,(UWORD)UartPort[Port].Types & 0xFF,(UWORD)UartPort[Port].Views & 0xFF);
//                                UartWrite(UartBuffer);
//  #endif
                              }
                              else
                              { // Only modes present

                                UartPort[Port].Views  =  UartPort[Port].Types;
//  #ifdef HIGHDEBUG
//                                snprintf(UartBuffer,UARTBUFFERSIZE,"    %d MODES  = %u = VIEWS\r\n",Port,(UWORD)UartPort[Port].Types & 0xFF);
//                                UartWrite(UartBuffer);
//  #endif
                              }
                              UartPort[Port].InfoData |=  INFODATA_CMD_MODES;
                            }
                          }
                          else
                          { // Number of modes invalid
//  #ifdef DEBUG_D_UART_ERROR
//                            snprintf(UartBuffer,UARTBUFFERSIZE," ## %d MODES ERROR  %u\r\n",Port,(UWORD)UartPort[Port].Types & 0xFF);
//                            UartWrite(UartBuffer);
//  #endif
                            UartPort[Port].State  =  UART_CMD_ERROR;
                          }
                        }
                        break;

                        case CMD_SPEED :
                        { // Highest communication speed

                          TmpL  =  0;
                          for (Tmp = 0;Tmp < UartPort[Port].InLength;Tmp++)
                          {
                            TmpL |=  (ULONG)UartPort[Port].InBuffer[Tmp] << (8 * Tmp);
                          }

                          if ((TmpL >= LOWEST_BITRATE) && (TmpL <= HIGHEST_BITRATE))
                          { // Speed valid

                            if ((UartPort[Port].InfoData & INFODATA_CMD_SPEED))
                            { // Speed already given

//  #ifdef DEBUG_D_UART_ERROR
//                              snprintf(UartBuffer,UARTBUFFERSIZE," ## %d SPEED ALREADY GIVEN\r\n",Port);
//                              UartWrite(UartBuffer);
//  #endif
                              UartPort[Port].State  =  UART_CMD_ERROR;
                            }
                            else
                            {
                              if ((UartPort[Port].BitRate != LOWEST_BITRATE) && (TmpL <= MIDLE_BITRATE))
                              { // allow bit rate adjust
//  #ifdef HIGHDEBUG
//                                snprintf(UartBuffer,UARTBUFFERSIZE,"    %d SPEED ADJUST\r\n",Port);
//                                UartWrite(UartBuffer);
//  #endif
                                UartPort[Port].BitRateMax  =  (UartPort[Port].BitRate * TmpL) / LOWEST_BITRATE;
                              }
                              else
                              {

                                UartPort[Port].BitRateMax  =  TmpL;
                              }
//  #ifdef HIGHDEBUG
//                              snprintf(UartBuffer,UARTBUFFERSIZE,"    %d SPEED  = %lu\r\n",Port,(unsigned long)UartPort[Port].BitRateMax);
//                              UartWrite(UartBuffer);
//  #endif
                              UartPort[Port].InfoData |=  INFODATA_CMD_SPEED;
                            }
                          }
                          else
                          { // Speed invalid
//  #ifdef DEBUG_D_UART_ERROR
//                            snprintf(UartBuffer,UARTBUFFERSIZE," ## %d SPEED ERROR\r\n",Port);
//                            UartWrite(UartBuffer);
//  #endif
                            UartPort[Port].State  =  UART_CMD_ERROR;
                          }
                        }
                        break;

                      }
                    }
                  }
                }
                break;

  //** INFO *********************************************************************

                case UART_INFO :
                { // Info messages in progress
//									printf("%c", Byte);
                  switch (UartPort[Port].SubState)
                  {
                    case 0 :
                    {
                      UartPort[Port].InfoCmd      =  Byte;

                      // validate length
											
                      switch (GET_INFO_COMMAND(UartPort[Port].InfoCmd))
                      {

                        case INFO_FORMAT :
                        {
                          if (UartPort[Port].InLength < 2)
                          {
//  #ifdef DEBUG_D_UART_ERROR
//                            snprintf(UartBuffer,UARTBUFFERSIZE," ## %d FORMAT ERROR\r\n",Port);
//                            UartWrite(UartBuffer);
//  #endif
                            UartPort[Port].State  =  UART_INFO_ERROR;
                          }
                        }
                        break;

                      }
                      UartPort[Port].SubState++;
                    }
                    break;

                    default :
                    {
                      if (UartPort[Port].InPointer < UartPort[Port].InLength)
                      { // Info message in progress
												
                        UartPort[Port].InBuffer[UartPort[Port].InPointer]  =  Byte;
                        UartPort[Port].InPointer++;
//												printf("%c", Byte);
                      }
                      else
                      { // Message complete
												
//												printf("%c", Byte);
												
                        UartPort[Port].InBuffer[UartPort[Port].InPointer]  =  0;
                        UartPort[Port].State  =  UART_MESSAGE_START;
												
                        if (UartPort[Port].Check !=  Byte)
                        {
//  #ifdef DEBUG_D_UART_ERROR
//                          snprintf(UartBuffer,UARTBUFFERSIZE," c  %d %02X ",Port,UartPort[Port].Cmd & 0xFF);
//                          UartWrite(UartBuffer);
//                          snprintf(UartBuffer,UARTBUFFERSIZE,"%02X[",UartPort[Port].InfoCmd & 0xFF);
//                          UartWrite(UartBuffer);
//                          for (Tmp = 0;Tmp < UartPort[Port].InLength;Tmp++)
//                          {
//                            snprintf(UartBuffer,UARTBUFFERSIZE,"%02X",UartPort[Port].InBuffer[Tmp] & 0xFF);
//                            UartWrite(UartBuffer);
//                          }
//                          snprintf(UartBuffer,UARTBUFFERSIZE,"]%02X\r\n",UartPort[Port].Check & 0xFF);
//                          UartWrite(UartBuffer);
//  #endif
                          UartPort[Port].State  =  UART_INFO_ERROR;
                        }
                        else
                        {
//													printf("%c", Byte); //UART_INFO帧尾
//  #ifdef HIGHDEBUG
//                          snprintf(UartBuffer,UARTBUFFERSIZE,"    %d %02X ",Port,UartPort[Port].Cmd & 0xFF);
//                          UartWrite(UartBuffer);
//                          snprintf(UartBuffer,UARTBUFFERSIZE,"%02X[",UartPort[Port].InfoCmd & 0xFF);
//                          UartWrite(UartBuffer);
//                          for (Tmp = 0;Tmp < UartPort[Port].InLength;Tmp++)
//                          {
//                            snprintf(UartBuffer,UARTBUFFERSIZE,"%02X",UartPort[Port].InBuffer[Tmp] & 0xFF);
//                            UartWrite(UartBuffer);
//                          }
//                          snprintf(UartBuffer,UARTBUFFERSIZE,"]%02X\r\n",UartPort[Port].Check & 0xFF);
//                          UartWrite(UartBuffer);
//  #endif

                          Mode  =  GET_MODE(UartPort[Port].Cmd);
													
                          switch (GET_INFO_COMMAND(UartPort[Port].InfoCmd))
                          { // Info mesage type

                            case INFO_NAME :
                            { // Device name
															
                              UartPort[Port].InfoData &=  INFODATA_CLEAR;
                              if ((UartPort[Port].InBuffer[0] >= 'A') && (UartPort[Port].InBuffer[0] <= 'z') && (strlen((const char *)UartPort[Port].InBuffer) <= TYPE_NAME_LENGTH))
                              {
                                snprintf((char*)UartPort[Port].Name,TYPE_NAME_LENGTH + 1,"%s",(char*)UartPort[Port].InBuffer);
//  #ifdef HIGHDEBUG
//                                snprintf(UartBuffer,UARTBUFFERSIZE,"    %d NAME   = %s\r\n",Port,UartPort[Port].Name);
//                                UartWrite(UartBuffer);
//  #endif
                                TypeData[Port][Mode].Mode   =  Mode;
                                UartPort[Port].InfoData    |=  INFODATA_INFO_NAME;
                              }
                              else
                              {
//  #ifdef DEBUG_D_UART_ERROR
//                                UartPort[Port].InBuffer[TYPE_NAME_LENGTH]  =  0;
//                                snprintf(UartBuffer,UARTBUFFERSIZE," f  %d NAME = %s\r\n",Port,UartPort[Port].InBuffer);
//                                UartWrite(UartBuffer);
//  #endif
                                UartPort[Port].State  =  UART_INFO_ERROR;
                              }
                            }
                            break;

                            case INFO_RAW :
                            { // Raw scaling values

                              TmpL  =  0;
                              for (Tmp = 0;(Tmp < UartPort[Port].InLength) && (Tmp < 4);Tmp++)
                              {
                                TmpL |=  (ULONG)UartPort[Port].InBuffer[Tmp] << (8 * Tmp);
                              }
                              *((ULONG*)&TypeData[Port][Mode].RawMin)  =  TmpL;
                              TmpL  =  0;
                              for (Tmp = 0;(Tmp < (UartPort[Port].InLength - 4)) && (Tmp < 4);Tmp++)
                              {
                                TmpL |=  (ULONG)UartPort[Port].InBuffer[Tmp + 4] << (8 * Tmp);
                              }
                              *((ULONG*)&TypeData[Port][Mode].RawMax)  =  TmpL;

                              if (TypeData[Port][Mode].Mode == GET_MODE(UartPort[Port].Cmd))
                              {
                                if ((UartPort[Port].InfoData & INFODATA_INFO_RAW))
                                { // Raw already given

//  #ifdef DEBUG_D_UART_ERROR
//                                  snprintf(UartBuffer,UARTBUFFERSIZE," ## %d RAW ALREADY GIVEN\r\n",Port);
//                                  UartWrite(UartBuffer);
//  #endif
                                  UartPort[Port].State  =  UART_INFO_ERROR;
                                }
                                else
                                {
//  #ifdef HIGHDEBUG
//                                  snprintf(UartBuffer,UARTBUFFERSIZE,"    %d RAW = Min..Max\r\n",Port);
//                                  UartWrite(UartBuffer);
//  #endif
                                  UartPort[Port].InfoData |=  INFODATA_INFO_RAW;
                                }
                              }
                              else
                              {
//  #ifdef DEBUG_D_UART_ERROR
//                                snprintf(UartBuffer,UARTBUFFERSIZE," f  %d RAW = Min..Max\r\n",Port);
//                                UartWrite(UartBuffer);
//  #endif
                                UartPort[Port].State  =  UART_INFO_ERROR;
                              }
                            }
                            break;

                            case INFO_PCT :
                            { // Pct scaling values

                              TmpL  =  0;
                              for (Tmp = 0;(Tmp < UartPort[Port].InLength) && (Tmp < 4);Tmp++)
                              {
                                TmpL |=  (ULONG)UartPort[Port].InBuffer[Tmp] << (8 * Tmp);
                              }
                              *((ULONG*)&TypeData[Port][Mode].PctMin)  =  TmpL;
                              TmpL  =  0;
                              for (Tmp = 0;(Tmp < (UartPort[Port].InLength - 4)) && (Tmp < 4);Tmp++)
                              {
                                TmpL |=  (ULONG)UartPort[Port].InBuffer[Tmp + 4] << (8 * Tmp);
                              }
                              *((ULONG*)&TypeData[Port][Mode].PctMax)  =  TmpL;

                              if (TypeData[Port][Mode].Mode == GET_MODE(UartPort[Port].Cmd))
                              { // Mode valid

                                if ((UartPort[Port].InfoData & INFODATA_INFO_PCT))
                                { // Pct already given

//  #ifdef DEBUG_D_UART_ERROR
//                                  snprintf(UartBuffer,UARTBUFFERSIZE," ## %d PCT ALREADY GIVEN\r\n",Port);
//                                  UartWrite(UartBuffer);
//  #endif
                                  UartPort[Port].State  =  UART_INFO_ERROR;
                                }
                                else
                                {
//  #ifdef HIGHDEBUG
//                                  snprintf(UartBuffer,UARTBUFFERSIZE,"    %d PCT = Min..Max\r\n",Port);
//                                  UartWrite(UartBuffer);
//  #endif
                                  UartPort[Port].InfoData |=  INFODATA_INFO_PCT;
                                }
                              }
                              else
                              { // Mode invalid
//  #ifdef DEBUG_D_UART_ERROR
//                                snprintf(UartBuffer,UARTBUFFERSIZE," f  %d PCT = Min..Max\r\n",Port);
//                                UartWrite(UartBuffer);
//  #endif
                                UartPort[Port].State  =  UART_INFO_ERROR;
                              }
                            }
                            break;

                            case INFO_SI :
                            { // SI unit scaling values

                              TmpL  =  0;
                              for (Tmp = 0;(Tmp < UartPort[Port].InLength) && (Tmp < 4);Tmp++)
                              {
                                TmpL |=  (ULONG)UartPort[Port].InBuffer[Tmp] << (8 * Tmp);
                              }
                              *((ULONG*)&TypeData[Port][Mode].SiMin)  =  TmpL;
                              TmpL  =  0;
                              for (Tmp = 0;(Tmp < (UartPort[Port].InLength - 4)) && (Tmp < 4);Tmp++)
                              {
                                TmpL |=  (ULONG)UartPort[Port].InBuffer[Tmp + 4] << (8 * Tmp);
                              }
                              *((ULONG*)&TypeData[Port][Mode].SiMax)  =  TmpL;

                              if (TypeData[Port][Mode].Mode == GET_MODE(UartPort[Port].Cmd))
                              { // Mode valid

                                if ((UartPort[Port].InfoData & INFODATA_INFO_SI))
                                { // Si already given

//  #ifdef DEBUG_D_UART_ERROR
//                                  snprintf(UartBuffer,UARTBUFFERSIZE," ## %d SI ALREADY GIVEN\r\n",Port);
//                                  UartWrite(UartBuffer);
//  #endif
                                  UartPort[Port].State  =  UART_INFO_ERROR;
                                }
                                else
                                {
//  #ifdef HIGHDEBUG
//                                  snprintf(UartBuffer,UARTBUFFERSIZE,"    %d SI  = Min..Max\r\n",Port);
//                                  UartWrite(UartBuffer);
//  #endif
                                  UartPort[Port].InfoData |=  INFODATA_INFO_SI;
                                }
                              }
                              else
                              { // Mode invalid
//  #ifdef DEBUG_D_UART_ERROR
//                                snprintf(UartBuffer,UARTBUFFERSIZE," f  %d SI  = Min..Max\r\n",Port);
//                                UartWrite(UartBuffer);
//  #endif
                                UartPort[Port].State  =  UART_INFO_ERROR;
                              }
                            }
                            break;

                            case INFO_SYMBOL :
                            { // Presentation format

                              if ((UartPort[Port].InfoData & INFODATA_INFO_SYMBOL))
                              { // Symbol already given

//  #ifdef DEBUG_D_UART_ERROR
//                                snprintf(UartBuffer,UARTBUFFERSIZE," ## %d SYMBOL ALREADY GIVEN\r\n",Port);
//                                UartWrite(UartBuffer);
//  #endif
                                UartPort[Port].State  =  UART_INFO_ERROR;
                              }
                              else
                              {
                                snprintf((char*)TypeData[Port][Mode].Symbol,SYMBOL_LENGTH + 1,"%s",(char*)UartPort[Port].InBuffer);
//  #ifdef HIGHDEBUG
//                                snprintf(UartBuffer,UARTBUFFERSIZE,"    %d SYMBOL = %s\r\n",Port,TypeData[Port][Mode].Symbol);
//                                UartWrite(UartBuffer);
//  #endif
                                UartPort[Port].InfoData |=  INFODATA_INFO_SYMBOL;
                              }
                            }
                            break;

                            case INFO_FORMAT :
                            { // Data sets and format

                              if ((UartPort[Port].InfoData & INFODATA_INFO_FORMAT))
                              { // Format already given

//  #ifdef DEBUG_D_UART_ERROR
//                                snprintf(UartBuffer,UARTBUFFERSIZE," ## %d FORMAT ALREADY GIVEN\r\n",Port);
//                                UartWrite(UartBuffer);
//  #endif
                                UartPort[Port].State  =  UART_INFO_ERROR;
                              }
                              else
                              {
                                TypeData[Port][Mode].DataSets  =  UartPort[Port].InBuffer[0];
                                TypeData[Port][Mode].Format    =  UartPort[Port].InBuffer[1];


                                if (TypeData[Port][Mode].DataSets > 0)
                                { // Data sets valid

                                  if (UartPort[Port].Types)
                                  { // Modes left

                                    UartPort[Port].Types--;
                                    if (TypeData[Port][Mode].Mode == GET_MODE(UartPort[Port].Cmd))
                                    { // Mode valid

                                      if (UartPort[Port].InLength >= 4)
                                      { // Figures and decimals present

                                        UartPort[Port].InfoData |=  INFODATA_INFO_FORMAT;

                                        if ((UartPort[Port].InfoData & INFODATA_NEEDED) == INFODATA_NEEDED)
                                        {
                                          snprintf((char*)TypeData[Port][Mode].Name,TYPE_NAME_LENGTH + 1,"%s",(char*)UartPort[Port].Name);

                                          TypeData[Port][Mode].Type        =  UartPort[Port].Type;
                                          TypeData[Port][Mode].Connection  =  CONN_INPUT_UART;
                                          TypeData[Port][Mode].Views       =  UartPort[Port].Views;

                                          TypeData[Port][Mode].Figures     =  UartPort[Port].InBuffer[2];
                                          TypeData[Port][Mode].Decimals    =  UartPort[Port].InBuffer[3];

//!<  \todo IR seeker hack
                                          if (TypeData[Port][Mode].Type == 33)
                                          {
                                            TypeData[Port][Mode].InvalidTime  =  1100;
                                          }

                                          Changed[Port][Mode]              =  1;
//  #ifdef HIGHDEBUG
//                                          snprintf(UartBuffer,UARTBUFFERSIZE,"    %d FORMAT = %u * %u  %u.%u\r\n",Port,TypeData[Port][Mode].DataSets,TypeData[Port][Mode].Format,TypeData[Port][Mode].Figures,TypeData[Port][Mode].Decimals);
//                                          UartWrite(UartBuffer);
//  #endif
                                        }
                                        else
                                        { // Not enough info data given
//  #ifdef DEBUG_D_UART_ERROR
//                                          snprintf(UartBuffer,UARTBUFFERSIZE," ## %d NOT ENOUGH INFO GIVEN\r\n",Port);
//                                          UartWrite(UartBuffer);
//  #endif
                                          UartPort[Port].State  =  UART_INFO_ERROR;

                                        }
                                      }
                                      else
                                      { // Format invalid
//  #ifdef DEBUG_D_UART_ERROR
//                                        snprintf(UartBuffer,UARTBUFFERSIZE," ## %d FORMAT ERROR\r\n",Port);
//                                        UartWrite(UartBuffer);
//  #endif
                                        UartPort[Port].State  =  UART_INFO_ERROR;

                                      }
                                    }
                                    else
                                    { // Mode invalid
//  #ifdef DEBUG_D_UART_ERROR
//                                      snprintf(UartBuffer,UARTBUFFERSIZE," f  %d FORMAT = %u * %u  %u.%u\r\n",Port,TypeData[Port][Mode].DataSets,TypeData[Port][Mode].Format,TypeData[Port][Mode].Figures,TypeData[Port][Mode].Decimals);
//                                      UartWrite(UartBuffer);
//  #endif
                                      UartPort[Port].State  =  UART_INFO_ERROR;
                                    }
                                  }
                                  else
                                  { // No more modes left
//  #ifdef DEBUG_D_UART_ERROR
//                                    snprintf(UartBuffer,UARTBUFFERSIZE," ## %d TYPES ERROR\r\n",Port);
//                                    UartWrite(UartBuffer);
//  #endif
                                    UartPort[Port].State  =  UART_INFO_ERROR;

                                  }
                                }
                                else
                                { // Data sets invalid
//  #ifdef DEBUG_D_UART_ERROR
//                                  snprintf(UartBuffer,UARTBUFFERSIZE," f  %d FORMAT = %u * %u  %u.%u\r\n",Port,TypeData[Port][Mode].DataSets,TypeData[Port][Mode].Format,TypeData[Port][Mode].Figures,TypeData[Port][Mode].Decimals);
//                                  UartWrite(UartBuffer);
//  #endif
                                  UartPort[Port].State  =  UART_INFO_ERROR;
                                }
                              }
                            }
                            break;

                          }
                        }
                        break;

                      }
                    }
                  }

                  if (UartPort[Port].Type == UartPortDefault.Type)
                  {
//  #ifdef DEBUG_D_UART_ERROR
//                    snprintf(UartBuffer,UARTBUFFERSIZE," ## %d TYPE ERROR\r\n",Port);
//                    UartWrite(UartBuffer);
//  #endif
                    UartPort[Port].State  =  UART_INFO_ERROR;
                  }
                }
                break;

  //** ERRORS *******************************************************************

                case UART_CMD_ERROR :
                {
                  UartPort[Port].State  =  UART_ERROR;
                }
                break;

                case UART_INFO_ERROR :
                {
                  UartPort[Port].State  =  UART_ERROR;
                }
                break;

                default :
                {
                  UartPort[Port].State  =  UART_MESSAGE_START;
                }
                break;
              }

  //** END OF INFORMATIONS ******************************************************

              UartPort[Port].Check ^=  Byte;
            }
          }
          break;

  //** DATA *********************************************************************

          case UART_DATA :
          { // Get device data
						
            UartPort[Port].InLength   =  UartPortReadData(Port,&UartPort[Port].Cmd,TmpBuffer,&UartPort[Port].Check,&CrcError);
//						printf("%c",TmpBuffer[2]);
						
            if (UartPort[Port].InLength)
            {
              if (!CrcError)
              {
                if (UartPort[Port].Initialised == 0)
                {
                  UartPort[Port].Initialised  =  1;
                }
                if (!((*pUart).Status[Port]  &  UART_PORT_CHANGED))
                {
                  if (UartPort[Port].Mode == GET_MODE(UartPort[Port].Cmd))
                  {
                    if (!((*pUart).Status[Port] & UART_DATA_READY))
                    {
                      (*pUart).Status[Port] |=  UART_DATA_READY;
											
//  #ifdef DEBUG_TRACE_MODE_CHANGE
//                      snprintf(UartBuffer,UARTBUFFERSIZE,"d_uart %d   State machine: mode changed to  %d\n",Port,UartPort[Port].Mode);
//                      UartWrite(UartBuffer);
//  #endif
                    }

//#ifdef DEBUG_TRACE_US
//                    if (Port == 1)
//                    {
//                      if (GET_MODE(UartPort[Port].Cmd) == 1)
//                      {
//                        In  =  (UWORD)TmpBuffer[0];
//                        In |=  (UWORD)TmpBuffer[1] << 8;

//                        if (In > 1000)
//                        {
//                          if (CrcError)
//                          {
//                            snprintf(UartBuffer,UARTBUFFERSIZE," c  %d %02X[",Port,UartPort[Port].Cmd & 0xFF);
//                          }
//                          else
//                          {
//                            snprintf(UartBuffer,UARTBUFFERSIZE,"    %d %02X[",Port,UartPort[Port].Cmd & 0xFF);
//                          }
//                          printk(UartBuffer);

//                          for (Tmp = 0;Tmp < UartPort[Port].InLength;Tmp++)
//                          {
//                            snprintf(UartBuffer,UARTBUFFERSIZE,"%02X",TmpBuffer[Tmp] & 0xFF);
//                            printk(UartBuffer);
//                          }
//                          snprintf(UartBuffer,UARTBUFFERSIZE,"]%02X\n",UartPort[Port].Check & 0xFF);
//                          printk(UartBuffer);
//                        }
//                      }
//                    }
//#endif

//  #ifndef DISABLE_FAST_DATALOG_BUFFER


////                    memcpy((void*)(*pUart).Raw[Port][(*pUart).LogIn[Port]],(void*)TmpBuffer,UART_DATA_LENGTH);

//                    (*pUart).Actual[Port]  =  (*pUart).LogIn[Port];
//                    (*pUart).Repeat[Port][(*pUart).Actual[Port]]  =  0;

//                    if (++((*pUart).LogIn[Port]) >= DEVICE_LOGBUF_SIZE)
//                    {
//                      (*pUart).LogIn[Port]      =  0;
//                    }
//  #else
//                    memcpy((void*)(*pUart).Raw[Port],(void*)TmpBuffer,UART_DATA_LENGTH);
//  #endif
                    if (UartPort[Port].DataErrors)
                    {
                      UartPort[Port].DataErrors--;
                    }
                    UartPort[Port].DataOk       =  1;
                  }
                  else
                  {
                    UartPort[Port].ChangeMode   =  1;
                  }
                }
              }
              else
              {
//  #ifdef DEBUG_D_UART_ERROR
//                snprintf(UartBuffer,UARTBUFFERSIZE," c  %d %02X[",Port,UartPort[Port].Cmd & 0xFF);
//                UartWrite(UartBuffer);

//                for (Tmp = 0;Tmp < UartPort[Port].InLength;Tmp++)
//                {
//                  snprintf(UartBuffer,UARTBUFFERSIZE,"%02X",TmpBuffer[Tmp] & 0xFF);
//                  UartWrite(UartBuffer);
//                }
//                snprintf(UartBuffer,UARTBUFFERSIZE,"]%02X\r\n",UartPort[Port].Check & 0xFF);
//                UartWrite(UartBuffer);
//  #endif
//  #ifndef DISABLE_UART_DATA_ERROR
//                if (++UartPort[Port].DataErrors >= UART_ALLOWABLE_DATA_ERRORS)
//                {
////  #ifdef DEBUG_D_UART_ERROR
////                  snprintf(UartBuffer,UARTBUFFERSIZE," ## %d No valid data in %d messages\r\n",Port,UartPort[Port].DataErrors);
////                  UartWrite(UartBuffer);
////  #endif
//                  UartPort[Port].State      =  UART_DATA_ERROR;
//                }
//  #endif
              }
//  #ifdef DEBUG_TRACE_ANGLE
//              if (Port == 1)
//              {
//                Angle  =  (UWORD)(*pUart).Raw[Port][0];
//                Angle |=  (UWORD)(*pUart).Raw[Port][1] << 8;

//                if (Angle > 50)
//                {
//                  printk("Angle = %u\r",Angle);
//                  if (CrcError)
//                  {
//                    snprintf(UartBuffer,UARTBUFFERSIZE," c  %d %02X[",Port,UartPort[Port].Cmd & 0xFF);
//                  }
//                  else
//                  {
//                    snprintf(UartBuffer,UARTBUFFERSIZE,"    %d %02X[",Port,UartPort[Port].Cmd & 0xFF);
//                  }
//                  printk(UartBuffer);

//                  for (Tmp = 0;Tmp < UartPort[Port].InLength;Tmp++)
//                  {
//                    snprintf(UartBuffer,UARTBUFFERSIZE,"%02X",TmpBuffer[Tmp] & 0xFF);
//                    printk(UartBuffer);
//                  }
//                  snprintf(UartBuffer,UARTBUFFERSIZE,"]%02X\r\n",UartPort[Port].Check & 0xFF);
//                  printk(UartBuffer);
//                }
//              }
//  #endif

//  #ifdef DEBUG
//              if (ShowTimer[Port] >= (UART_SHOW_TIME / UART_TIMER_RESOLUTION))
//              {
//                ShowTimer[Port]  =  0;
//                if (CrcError)
//                {
//                  snprintf(UartBuffer,UARTBUFFERSIZE," c  %d %02X[",Port,UartPort[Port].Cmd & 0xFF);
//                }
//                else
//                {
//                  snprintf(UartBuffer,UARTBUFFERSIZE,"    %d %02X[",Port,UartPort[Port].Cmd & 0xFF);
//                }
//                UartWrite(UartBuffer);

//                for (Tmp = 0;Tmp < UartPort[Port].InLength;Tmp++)
//                {
//                  snprintf(UartBuffer,UARTBUFFERSIZE,"%02X",TmpBuffer[Tmp] & 0xFF);
//                  UartWrite(UartBuffer);
//                }
//                snprintf(UartBuffer,UARTBUFFERSIZE,"]%02X\r\n",UartPort[Port].Check & 0xFF);
//                UartWrite(UartBuffer);
//              }
//  #endif
            }
							
            if (UartPort[Port].ChangeMode)
            { // Try to change mode

              if (UartPort[Port].OutPointer >= UartPort[Port].OutLength)
              { // Transmitter ready

//  #ifdef DEBUG
//                ShowTimer[Port]  =  0;
//                if (CrcError)
//                {
//                  snprintf(UartBuffer,UARTBUFFERSIZE," c  %d %02X[",Port,UartPort[Port].Cmd & 0xFF);
//                }
//                else
//                {
//                  snprintf(UartBuffer,UARTBUFFERSIZE,"    %d %02X[",Port,UartPort[Port].Cmd & 0xFF);
//                }
//                UartWrite(UartBuffer);

//                for (Tmp = 0;Tmp < UartPort[Port].InLength;Tmp++)
//                {
//                  snprintf(UartBuffer,UARTBUFFERSIZE,"%02X",TmpBuffer[Tmp] & 0xFF);
//                  UartWrite(UartBuffer);
//                }
//                snprintf(UartBuffer,UARTBUFFERSIZE,"]%02X\r\n",UartPort[Port].Check & 0xFF);
//                UartWrite(UartBuffer);
//  #endif
                UartPort[Port].Cmd            =  UartPort[Port].Mode;
								if(UartPort[Port].Mode == 0x11)//模式重置
								{
									UartPort[Port].OutBuffer[0]   =  MAKE_CMD_COMMAND(CMD_SELECT,0)+1;
								}else
								{
									UartPort[Port].OutBuffer[0]   =  MAKE_CMD_COMMAND(CMD_SELECT,0);								
								}
                UartPort[Port].OutBuffer[1]   =  UartPort[Port].Mode;
                UartPort[Port].OutBuffer[2]   =  0xFF ^ UartPort[Port].OutBuffer[0] ^ UartPort[Port].OutBuffer[1];
                UartPort[Port].OutPointer     =  0;
                UartPort[Port].OutLength      =  3;

                UartPort[Port].ChangeMode  =  0;

//  #ifdef DEBUG_TRACE_MODE_CHANGE
//                snprintf(UartBuffer,UARTBUFFERSIZE," WR %d %02X[",Port,UartPort[Port].OutBuffer[0]);
//                UartWrite(UartBuffer);

//                for (Tmp = 1;Tmp < (UartPort[Port].OutLength - 1);Tmp++)
//                {
//                  snprintf(UartBuffer,UARTBUFFERSIZE,"%02X",UartPort[Port].OutBuffer[Tmp] & 0xFF);
//                  UartWrite(UartBuffer);
//                }
//                snprintf(UartBuffer,UARTBUFFERSIZE,"]%02X\r\n",UartPort[Port].OutBuffer[Tmp] & 0xFF);
//                UartWrite(UartBuffer);
//  #endif
              }
              (*pUart).Status[Port] &= ~UART_DATA_READY;
            }
						
            if (++UartPort[Port].WatchDog >= (UART_WATCHDOG_TIME / UART_TIMER_RESOLUTION) )
            { // Try to service watch dog

              if (UartPort[Port].OutPointer >= UartPort[Port].OutLength)
              { // Transmitter ready

                UartPort[Port].WatchDog       =  0;

                if (!UartPort[Port].DataOk)
                { // No ok data since last watch dog service

//  #ifndef DISABLE_UART_DATA_ERROR
//                  if (++UartPort[Port].DataErrors >= UART_ALLOWABLE_DATA_ERRORS)
//                  {
////  #ifdef DEBUG_D_UART_ERROR
////                    snprintf(UartBuffer,UARTBUFFERSIZE," ## %d No valid data in %d services\r\n",Port,UART_ALLOWABLE_DATA_ERRORS);
////                    UartWrite(UartBuffer);
////  #endif
//                    UartPort[Port].State        =  UART_DATA_ERROR;
//                  }
//                  else
//                  {
//                    UartPort[Port].DataOk       =  1;
//                  }
//  #else
                  UartPort[Port].DataOk         =  1;
//  #endif
                }
                if (UartPort[Port].DataOk)
                {
                  UartPort[Port].DataOk         =  0;

                  UartPort[Port].OutBuffer[0]   =  BYTE_NACK;
                  UartPort[Port].OutPointer     =  0;
                  UartPort[Port].OutLength      =  1;
//  #ifdef DEBUG
//                  snprintf(UartBuffer,UARTBUFFERSIZE," WD %d %02X\r\n",Port,UartPort[Port].OutBuffer[0]);
//                  UartWrite(UartBuffer);
//  #endif
                }
              }
            }

//            if (WriteRequest[Port])
//            { // Try to write message

//              if (UartPort[Port].OutPointer >= UartPort[Port].OutLength)
//              { // Transmitter ready

//                // convert length to length code
//                Byte  =  0;
//                Tmp  =  CONVERT_LENGTH(Byte);
//                while ((Tmp < UART_DATA_LENGTH) && (Tmp < (*pUart).OutputLength[Port]))
//                {
//                  Byte++;
//                  Tmp  =  CONVERT_LENGTH(Byte);
//                }

//                Chksum    =  0xFF;

//                UartPort[Port].OutBuffer[0]  =  MAKE_CMD_COMMAND(CMD_WRITE,Byte);
//                Chksum                      ^=  UartPort[Port].OutBuffer[0];

//                Pointer   =  0;
//                while (Pointer < Tmp)
//                {
//                  if (Pointer < (*pUart).OutputLength[Port])
//                  {
//                    UartPort[Port].OutBuffer[1 + Pointer]  =  (*pUart).Output[Port][Pointer];
//                  }
//                  else
//                  {
//                    UartPort[Port].OutBuffer[1 + Pointer]  =  0;
//                  }
//                  Chksum ^=  UartPort[Port].OutBuffer[1 + Pointer];
//                  Pointer++;

//                }
//                UartPort[Port].OutBuffer[1 + Pointer]   =  Chksum;
//                UartPort[Port].OutPointer               =  0;
//                UartPort[Port].OutLength                =  Tmp + 2;

//                WriteRequest[Port]                      =  0;
//                (*pUart).Status[Port]                  &= ~UART_WRITE_REQUEST;
//  #ifdef DEBUG
//                snprintf(UartBuffer,UARTBUFFERSIZE," WR %d %02X[",Port,UartPort[Port].OutBuffer[0]);
//                UartWrite(UartBuffer);

//                for (Tmp = 1;Tmp < (UartPort[Port].OutLength - 1);Tmp++)
//                {
//                  snprintf(UartBuffer,UARTBUFFERSIZE,"%02X",UartPort[Port].OutBuffer[Tmp] & 0xFF);
//                  UartWrite(UartBuffer);
//                }
//                snprintf(UartBuffer,UARTBUFFERSIZE,"]%02X\r\n",UartPort[Port].OutBuffer[Tmp] & 0xFF);
//                UartWrite(UartBuffer);
//  #endif
//              }
//            }

//  #ifndef DISABLE_FAST_DATALOG_BUFFER
//            ((*pUart).Repeat[Port][(*pUart).Actual[Port]])++;
//  #endif
          }
          break;

          case UART_ACK_WAIT :
          {
            if (++UsartBreakTimer[Port] )//>= (UART_ACK_DELAY / UART_TIMER_RESOLUTION)/20)
            {
							
              (*pUart).Status[Port]  |=  UART_PORT_CHANGED;
							
//							USART1_Send_Byte(BYTE_ACK);
              UartPortSend(Port,BYTE_ACK);
//							STM_EVAL_LEDToggle(LED3);
							
              UartPort[Port].Timer    =  0;
              UartPort[Port].State    =  UART_ACK_INFO;
//#ifdef DEBUG_D_UART_ERROR
//              snprintf(UartBuffer,UARTBUFFERSIZE,"    %d Type %-3d has changed modes: ",Port,TypeData[Port][0].Type);
//              UartWrite(UartBuffer);
//              for (Mode = 0;Mode < MAX_DEVICE_MODES;Mode++)
//              {
//                UartBuffer[Mode]  =  Changed[Port][Mode] + '0';
//              }
//              UartBuffer[Mode++]  =  '\r';
//              UartBuffer[Mode++]  =  '\n';
//              UartBuffer[Mode]    =  0;
//              UartWrite(UartBuffer);
//#endif
            }
          }
          break;

          case UART_ACK_INFO :
          {
					
            if (++UsartBreakTimer[Port] >= 2*(UART_CHANGE_BITRATE_DELAY / UART_TIMER_RESOLUTION))
            {
              UartPort[Port].DataOk       =  1;
              UartPort[Port].DataErrors   =  0;
              UartPort[Port].Mode         =  0;
              UartPort[Port].BitRate      =  UartPort[Port].BitRateMax;
							
//              UartPortSetup(Port,UartPort[Port].BitRate);
//							usart1_change(UartPort[Port].BitRate);
								UsartChange(Port,UartPort[Port].BitRate);
							
              UartPort[Port].WatchDog     =  (UART_WATCHDOG_TIME / UART_TIMER_RESOLUTION);
              UartPort[Port].State        =  UART_DATA;
            }
          }
          break;

          case UART_TERMINAL :
          {
          }
          break;

          case UART_DATA_ERROR :
          {
            UartPort[Port].State  =  UART_ERROR;
          }
          break;

          case UART_ERROR :
          {
          }
          break;

          case UART_EXIT :
          {
//            UartPortDisable(Port);
            UartPort[Port]                          =  UartPortDefault;


            for (Tmp = 0;Tmp < MAX_DEVICE_MODES;Tmp++)
            {
              TypeData[Port][Tmp].Name[0]           =  0;
              TypeData[Port][Tmp].Type              =  0;
              Changed[Port][Tmp]                    =  0;
            }
            (*pUart).Status[Port]                   =  0;

            UartPort[Port].State                    =  UART_IDLE;
          }
          break;

        }
        if (UartPort[Port].OutPointer < UartPort[Port].OutLength)
        {
//					if(USART1_Send_Byte(UartPort[Port].OutBuffer[UartPort[Port].OutPointer]))
          if(UartPortSend(Port,UartPort[Port].OutBuffer[UartPort[Port].OutPointer]))
          {
            UartPort[Port].OutPointer++;
          }
        }

      }
//      else
//      {
//        switch (UartPort[Port].State)
//        { // Test state machine

//          case UART_IDLE :
//          { // Port inactive

//          }
//          break;

//          case UART_INIT :
//          { // Initialise port hardware

////            UartPortDisable(Port);
////            PUARTFloat(Port,INPUT_UART_PIN5);
////            PUARTFloat(Port,INPUT_UART_PIN6);
//            UartPort[Port].State        =  UART_ENABLE;
//          }
//          break;

//          case UART_ENABLE :
//          { // Initialise port variables

////            UartPortEnable(Port);
////            UartPortSetup(Port,UartPort[Port].BitRate);
//						usart1_change(UartPort[Port].BitRate);
//						
//            (*pUart).Status[Port]           =  0;
////            UartPortFlush(Port);
//            UartPort[Port].InPointer        =  0;
//            UartPort[Port].State            =  UART_MESSAGE_START;
//          }
//          break;

//          case UART_MESSAGE_START :
//          {
//            if (UartPort[Port].OutPointer < UartPort[Port].OutLength)
//            {
////              if (UartPortSend(Port,UartPort[Port].OutBuffer[UartPort[Port].OutPointer]))
//							if (USART1_Send_Byte(UartPort[Port].OutBuffer[UartPort[Port].OutPointer]))
//              {
//                UartPort[Port].OutPointer++;
//              }
//            }
//            if (Uart1Read(&Byte))
//            {
////#ifdef HIGHDEBUG
////              snprintf(UartBuffer,UARTBUFFERSIZE,"[0x%02X]\r\n",Byte);
////              UartWrite(UartBuffer);
////#endif
//              if (UartPort[Port].InPointer < UART_BUFFER_SIZE)
//              {
//                UartPort[Port].InBuffer[UartPort[Port].InPointer]  =  Byte;
//                UartPort[Port].InPointer++;
//              }
//            }
//          }
//          break;

//        }

//      }

//#ifdef HIGHDEBUG
//      if (UartPort[Port].OldState != UartPort[Port].State)
//      {
//        UartPort[Port].OldState    =  UartPort[Port].State;
//        if (UartPort[Port].State  != UART_ENABLE)
//        {
//          snprintf(UartBuffer,UARTBUFFERSIZE,"    %d %s\r\n",Port,UartStateText[UartPort[Port].State]);
//        }
//        else
//        {
//          snprintf(UartBuffer,UARTBUFFERSIZE,"*** %d %s ***\r\n",Port,UartStateText[UartPort[Port].State]);
//        }
//        UartWrite(UartBuffer);
//      }
//#endif
    }else
		{
			UartPort[Port].State    =  UART_ENABLE;
		}
  }
}


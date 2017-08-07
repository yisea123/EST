/**
  ******************************************************************************
  * @file    EST Brick of Dr.luck/iic_sensor.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#include <stdio.h>
#include <string.h> 

#include "main.h"
#include "iic_sensor.h"
#include "iic.h"

#define true 1
#define false 0
	
UBYTE IICPortFlag[INPUTS];

UBYTE     IicConfigured[INPUTS];
IICPORT   IicPort[INPUTS];

DATA8     IicPortType[INPUTS];
IICSTR    IicStrings[INPUTS];

static    IIC  IicDefault;
static    IIC *pIic   =  &IicDefault;

struct IIC_control_t IicCtrl;

unsigned char iic_flag[4] ={0};

void FloatToByte4(float temp, uint8_t byte[],uint8_t len);
float Byte4ToFloat(uint8_t byte[], uint8_t len);

const IICPORT  IicPortDefault =
{
  0,                        // Timer
  0,                        // Time
  0,                        // Initialised
  0,                        // ChangeMode
  0,                        // State
  (unsigned char)-1,        // OldState
  0,                        // SubState
  0,                        // Repeat
  0,                        // Cmd
  0,                        // Mode
  1,                        // Addr
  3,                        // Retries
  "",                       // Name
  0,                        // Reverse
  0,                        // InLength
  0,                        // InPointer
  0,                        // OutLength
  0,                        // OutPointer
};

void IicPortDisable(UBYTE port)
{

}
void IicPortEnable(UBYTE port)
{

}

 RESULT IicPortSend(UBYTE Port)
{
 RESULT  Result = OK;

//#ifndef   DISABLE_FIQ_IIC
  DATA8   Tmp;

  IicCtrl.data_package[Port].addr             =  IicPort[Port].OutBuffer[0];

//#ifdef HIGHDEBUG
//  snprintf(IicBuffer,IICBUFFERSIZE,"    %d Send %02X   ",Port,IicPort[Port].OutBuffer[0]);
//  IicWrite(IicBuffer);
//#endif
  for (Tmp = 1;Tmp < IicPort[Port].OutLength;Tmp++)
  {
//#ifdef HIGHDEBUG
//    snprintf(IicBuffer,IICBUFFERSIZE,"%02X ",IicPort[Port].OutBuffer[Tmp]);
//    IicWrite(IicBuffer);
//#endif
    IicCtrl.data_package[Port].data[Tmp - 1]  =  IicPort[Port].OutBuffer[Tmp];
  }
//#ifdef HIGHDEBUG
//  snprintf(IicBuffer,IICBUFFERSIZE,"\r\n");
//  IicWrite(IicBuffer);
//#endif
  IicCtrl.data_package[Port].write_length     =  IicPort[Port].OutLength - 1;
  IicCtrl.data_package[Port].read_length      =  IicPort[Port].InLength;
  IicCtrl.data_package[Port].port             =  Port;

  IicCtrl.data_package[Port].nacked           =  0;
  IicCtrl.data_package[Port].clock_state      =  1;
  IicCtrl.data_package[Port].transfer_state   =  TRANSFER_START;

 // iic_flag_start_transfer(50,1);
  iic_flag[Port] = 1;
//#endif
	
//#ifdef INPUT_DEBUG
//	printf("IicCtrl.data_package[Port].addr:%x\n\r",IicCtrl.data_package[Port].addr );
//#endif

  return (Result);
}


static RESULT IicPortReceive(UBYTE Port,UBYTE *pTmpBuffer)
{
  RESULT  Result = BUSY;

#ifndef   DISABLE_FIQ_IIC
  DATA8   Tmp;

  if (IicCtrl.data_package[Port].transfer_state == TRANSFER_IDLE)
  {
    memset((void*)pTmpBuffer,0,IIC_DATA_LENGTH);
//#ifdef HIGHDEBUG
//    snprintf(IicBuffer,IICBUFFERSIZE,"    %d Receiving   ",Port);
//    IicWrite(IicBuffer);
//#endif
    for (Tmp = 0;Tmp < IicPort[Port].InLength;Tmp++)
    {
//#ifdef HIGHDEBUG
//      snprintf(IicBuffer,IICBUFFERSIZE,"%02X ",IicCtrl.data_package[Port].data[Tmp]);
//      IicWrite(IicBuffer);
//#endif
      pTmpBuffer[Tmp]        =  IicCtrl.data_package[Port].data[Tmp];
//			printf("pTmpBuffer[%x]:%x\n\r",Tmp,IicCtrl.data_package[Port].data[Tmp]);
    }
//#ifdef HIGHDEBUG
//      snprintf(IicBuffer,IICBUFFERSIZE,"\r\n");
//      IicWrite(IicBuffer);
//#endif

    if (!(IicCtrl.data_package[Port].nacked))
    {
      Result  =  OK;
    }
    else
    {
      Result  =  FAIL;
    }
  }

#endif

  return (Result);
}

static UBYTE IicPortBusy(UBYTE Port)
{
  UBYTE   Result = 0;


  return (Result);
}

void IicSdaIn(UBYTE Port)
{
	switch (Port)
  {
    case Iic1 :
    {
      SDA_IN_1();
    }
    break;
    case Iic2 :
    {
      SDA_IN_2();
    }
    break;
    case Iic3 :
    {
      SDA_IN_3();
    }
    break;
    case Iic4 :
    {
      SDA_IN_4();
    }
    break;
  }
}
void IicSdaOut(UBYTE Port)
{
	switch (Port)
  {
    case Iic1 :
    {
      SDA_OUT_1();
    }
    break;
    case Iic2 :
    {
      SDA_OUT_2();
    }
    break;
    case Iic3 :
    {
      SDA_OUT_3();
    }
    break;
    case Iic4 :
    {
      SDA_OUT_4();
    }
    break;
  }
}
void IicScl(UBYTE Port, UBYTE level)
{
	switch (Port)
  {
    case Iic1 :
    {
      IIC_SCL_1 = level;
    }
    break;
    case Iic2 :
    {
      IIC_SCL_2 = level;
    }
    break;
    case Iic3 :
    {
			IIC_SCL_3 = level;
    }
    break;
    case Iic4 :
    {
      IIC_SCL_4 = level;
    }
    break;
  }
}
void IicSda(UBYTE Port, UBYTE level)
{
	switch (Port)
  {
    case Iic1 :
    {
      IIC_SDA_1 = level;
    }
    break;
    case Iic2 :
    {
			IIC_SDA_2 = level;
    }
    break;
    case Iic3 :
    {
      IIC_SDA_3 = level;
    }
    break;
    case Iic4 :
    {
      IIC_SDA_4 = level;
    }
    break;
  }
}
UBYTE IicReadSda(UBYTE Port)
{
  UBYTE   Result = 0;
	switch (Port)
  {
    case Iic1 :
    {
      Result = READ_SDA_1; //GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1);
    }
    break;
    case Iic2 :
    {
      Result = READ_SDA_2; //GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3);
    }
    break;
    case Iic3 :
    {
      Result = READ_SDA_3; //GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5);
    }
    break;
    case Iic4 :
    {
      Result = READ_SDA_4; //GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7);
    }
    break;
  }
  return (Result);
}

int iic_flag_handler()
{
  u8 Port;
  struct IIC_data_package *iic;
  u8 restart_timer = false;

  for (Port=0; Port < INPUTS ; Port++) {
//#ifdef TIMING_CHANNEL
//    if (Port & 1) PINHigh(2,IIC_PORT_DATA) 
//    else PINLow(2,IIC_PORT_DATA);
//#endif
		iic = &IicCtrl.data_package[Port];
		if (iic->transfer_state != TRANSFER_IDLE) {
			restart_timer = true;
      
      // Check set high in TRANSFER_STOP2 state
      if (iic->clock_state)
				IicScl(Port, 1);//	PINHigh(Port,IIC_PORT_CLOCK)
      else
				IicScl(Port, 0);//	PINLow(Port,IIC_PORT_CLOCK);
    }
		
//#ifdef INPUT_DEBUG
//		printf("Port:%x  iic->transfer_state:%x\n\r",Port,iic->transfer_state);
//#endif
		
    switch (iic->transfer_state) {
			case TRANSFER_START  :// Make sure to SYNC into Timer settings  
														// to ensure first bit time having full length
														iic->transfer_state = TRANSFER_START2;
														break;

      case TRANSFER_START2 :// Generate start condition
														IicSda(Port, 0);													// PINLow(Port,IIC_PORT_DATA);
														IicSdaOut(Port);													//PINOutput(Port,IIC_PORT_DATAhttp://internet.stu.edu.cn/http://internet.stu.edu.cn/
                            iic->clock_state = 0;
                            iic->nacked = false;
                            iic->transfer_state = TRANSFER_ADDR;
														break;
			     
      case TRANSFER_ADDR  : iic->data_byte = (iic->addr << 1);
                            // Check if write_lenght = 0? In given case we are to do a read
                            if (iic->write_length)
                               iic->read_start_send = false;
														else
                               iic->data_byte |= 1;
														iic->byte = 0;       											// Byte is offset one as it's increased by writing address
														iic->write_length++; 											// Need to transfer Address as well

      case TRANSFER_DATA 	: if (iic->transfer_state == TRANSFER_DATA)
															iic->data_byte = iic->data[iic->byte-1];
														iic->transfer_state = TRANSFER_WBIT;
                            iic->bit  = 0x80;
														IicSdaOut(Port);													//PINOutput(Port,IIC_PORT_DATA);
	
      case TRANSFER_WBIT	: if(!iic->clock_state) 
														{
															if (iic->data_byte & iic->bit)
																IicSda(Port, 1);											//PINHigh(Port,IIC_PORT_DATA)
                               else
																 IicSda(Port, 0);											//PINLow(Port,IIC_PORT_DATA);
                              iic->bit >>= 1;
														}
														if ((!iic->bit) && (iic->clock_state))
														{
															iic->byte++;
															iic->write_length--;
															iic->transfer_state = TRANSFER_RACK;
														}
														iic->clock_state ^= 1;
                            break;

      case TRANSFER_READ  :  
														IicSdaIn(Port);														//PINInput(Port,IIC_PORT_DATA);
                            iic->transfer_state = TRANSFER_RBIT;
                            iic->bit  = 0x80;
                            iic->data_byte = 0;

      case TRANSFER_RBIT 	: if (iic->clock_state) 
														{
															iic->data_byte |= IicReadSda(Port)? iic->bit:0;
//															iic->data_byte |= GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)? iic->bit:0;
//                              iic->data_byte |= PINRead(Port, IIC_PORT_DATA)? iic->bit:0;
                              iic->bit >>= 1;
															if (!iic->bit)
															{
																iic->data[iic->byte-1] = iic->data_byte;
                                iic->byte++;
                                iic->read_length--;
                                iic->transfer_state = TRANSFER_WACK;
															}
														}
                            iic->clock_state ^= 1;
                            break;

      case TRANSFER_RACK  : if (!iic->clock_state)
														{
															IicSdaIn(Port);													//PINInput(Port,IIC_PORT_DATA)
														}
														else 
														{
															if(IicReadSda(Port) == 0){
//															if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 0) {
//															if (PINRead(Port, IIC_PORT_DATA) == 0) {
																if (iic->write_length) {
																	iic->wait_cycles = 4;
																	iic->transfer_state = TRANSFER_WAIT;
																}
																else if (iic->read_start_send) { 
																	iic->wait_cycles = 4;
																	iic->transfer_state = TRANSFER_WAIT;
																}
																else
																	iic->transfer_state = TRANSFER_STOP;
                              }
                              else
                              {
																iic->nacked = true;
                                iic->transfer_state = TRANSFER_STOP;
                              }
														}
														iic->clock_state ^= 1;
                            break;

      case TRANSFER_WACK  : if (!iic->clock_state) 
														{
															if (iic->read_length)
																IicSda(Port, 0);											//PINLow(Port,IIC_PORT_DATA)  // ACK
                              else
																IicSda(Port, 1);											//PINHigh(Port,IIC_PORT_DATA);  // NACK the last one
															IicSdaOut(Port);												//PINOutput(Port,IIC_PORT_DATA);
                             } 
														else 
														{
															if (iic->read_length) 
															{
																iic->wait_cycles = 2;
																iic->transfer_state = TRANSFER_WAIT;
															}
															else
																iic->transfer_state = TRANSFER_STOP;
                            }
														iic->clock_state ^= 1;
														break;

      case TRANSFER_WAIT  : if (iic->wait_cycles--) {}
														else if (iic->write_length)
															iic->transfer_state = TRANSFER_DATA;
														else if (iic->read_length)
															iic->transfer_state = TRANSFER_READ;
														break;

      case TRANSFER_STOP  : if (iic->read_length) 
														{
															IicSda(Port, 0);												//PINLow(Port,IIC_PORT_DATA);
 			       									IicSdaOut(Port);												//PINOutput(Port,IIC_PORT_DATA);
														}
														else
															IicSdaIn(Port);													//PINInput(Port,IIC_PORT_DATA);
														if (iic->clock_state)
															iic->transfer_state = TRANSFER_STOP2;
														iic->clock_state = 1;
                            break;

      case TRANSFER_STOP2 : //PINHigh(Port,IIC_PORT_DATA);
                            if (iic->nacked)
															iic->transfer_state = TRANSFER_STOP3;
														else if (iic->read_length) 
														{
															IicSda(Port, 1);												//PINHigh(Port,IIC_PORT_DATA);
															IicSdaOut(Port);												//PINOutput(Port,IIC_PORT_DATA);
															iic->clock_state ^= 1;
															if  (iic->clock_state) 
															{
																iic->transfer_state = TRANSFER_RESTART;
																iic->read_start_send = true;
															}
														}
														else
															iic->transfer_state = TRANSFER_STOP3;
														break;

      case TRANSFER_RESTART: iic->transfer_state = TRANSFER_START2;
                             break;

      case TRANSFER_STOP3 :  
														IicSdaIn(Port);														//PINInput(Port,IIC_PORT_DATA);
														//PINHigh(Port,IIC_PORT_DATA);
														//PINOutput(Port,IIC_PORT_DATA);
														iic->transfer_state = TRANSFER_IDLE;
														 
      case TRANSFER_IDLE	:	iic_flag[Port] = 0;
														break;
      default             : break;
    }
  }
  return (restart_timer);
}


void iicDevice1TimerInterrupt1()
{
  UBYTE   Port;
  UBYTE   Tmp;
  UBYTE   TmpBuffer[IIC_DATA_LENGTH];
		
  for (Port = 0;Port < NO_OF_IIC_PORTS;Port++)
  { // look at one port at a time

//#ifdef INPUT_DEBUG
//	printf("IicPort[%x].State:%x\n\r",Port,IicPort[Port].State);
//#endif
		if(IICPortFlag[Port])
		{
    switch (IicPort[Port].State)
    { // Main state machine

      case IIC_IDLE :
      { // Port inactive
				
					IicPort[Port].State         =  IIC_INIT;
					(*pIic).Status[Port]        =  0;
				
      }
      break;

      case IIC_INIT :
      { // Initialise port hardware

        memset((void*)IicStrings[Port].Manufacturer,0,IIC_NAME_LENGTH + 1);
        memset((void*)IicStrings[Port].SensorType,0,IIC_NAME_LENGTH + 1);
        IicPort[Port].State         =  IIC_ENABLE;

      }
      break;

      case IIC_RESTART :
      {
//        IicPortDisable(Port);
        IicPort[Port].State         =  IIC_ENABLE;
      }
      break;

      case IIC_ENABLE :
      { // Initialise port variables	
				
//        IicPortEnable(Port);

        IicPort[Port]               =  IicPortDefault;

        IicPort[Port].Timer         =  0;

        IicPort[Port].State         =  IIC_NXT_TEMP_START;
				
      }
      break;


      case IIC_NXT_TEMP_START :
      {
        if (++(IicPort[Port].Timer) >= 10)//(10 / IIC_TIMER_RESOLUTION))
        {
          IicPort[Port].OutBuffer[0]  =  0x4C;
          IicPort[Port].OutBuffer[1]  =  0x01;
          IicPort[Port].OutBuffer[2]  =  0x60;
          IicPort[Port].OutBuffer[3]  =  0x00;
          IicPort[Port].OutLength     =  3;
          IicPort[Port].InLength      =  1;
          IicPort[Port].Repeat        =  1;
          IicPort[Port].Time          =  0;
          IicPort[Port].Timer         =  0;
          IicPort[Port].State         =  IIC_NXT_TEMP_WRITE;
        }
      }
      break;

      case IIC_NXT_TEMP_WRITE :
      {
        IicPortSend(Port);
        IicPort[Port].Timer       =  0;
        IicPort[Port].State       =  IIC_NXT_TEMP_READ;
      }
      break;

      case IIC_NXT_TEMP_READ :
      {
        if (IicPortReceive(Port,TmpBuffer) != BUSY)
        {
          if (TmpBuffer[0] == 0x60)
          {
            memcpy((void*)IicStrings[Port].Manufacturer,(void*)"LEGO",IIC_NAME_LENGTH);
            IicStrings[Port].Manufacturer[IIC_NAME_LENGTH]  =  0;
            memcpy((void*)IicStrings[Port].SensorType,(void*)"Temp.",IIC_NAME_LENGTH);
            IicStrings[Port].SensorType[IIC_NAME_LENGTH]  =  0;

						IicStrings[Port].Type = 6;
						
            (*pIic).Changed[Port]       =  1;

            IicPort[Port].Initialised   =  1;
            IicPort[Port].Timer         =  0;
            IicPort[Port].State         =  IIC_SETUP_WAIT;
          }
          else
          {
            IicPort[Port].Timer       =  0;
            IicPort[Port].State         =  IIC_MANUFACTURER_START;
          }
        }
        if (++(IicPort[Port].Timer) >= (500 / IIC_TIMER_RESOLUTION))
        {
          IicPort[Port].Timer       =  0;
          IicPort[Port].State         =  IIC_MANUFACTURER_START;
        }
      }
      break;

      case IIC_MANUFACTURER_START :
      {
        if (++(IicPort[Port].Timer) >= 1)//(10 / IIC_TIMER_RESOLUTION))
        {
          IicPort[Port].OutBuffer[0]  =  IicPort[Port].Addr;
          IicPort[Port].OutBuffer[1]  =  0x08;
          IicPort[Port].OutBuffer[2]  =  0x00;
          IicPort[Port].OutBuffer[3]  =  0x00;
          IicPort[Port].OutLength     =  2;
          IicPort[Port].InLength      =  8;
          IicPort[Port].Repeat        =  1;
          IicPort[Port].Time          =  0;
          IicPort[Port].Timer         =  0;
          IicPort[Port].State         =  IIC_MANUFACTURER_WRITE;
        }
      }
      break;

      case IIC_MANUFACTURER_WRITE :
      {
        IicPortSend(Port);
        IicPort[Port].Timer       =  0;
        IicPort[Port].State       =  IIC_MANUFACTURER_READ;
      }
      break;

      case IIC_MANUFACTURER_READ :
      {
        if (IicPortReceive(Port,TmpBuffer) != BUSY)
        {
          memcpy((void*)IicStrings[Port].Manufacturer,(void*)TmpBuffer,IIC_NAME_LENGTH);
          IicStrings[Port].Manufacturer[IIC_NAME_LENGTH]  =  0;

          if (TmpBuffer[0] == 0x08)
          {
            if (IicPort[Port].Addr < 0x50)
            {
              IicPort[Port].Addr++;
              IicPort[Port].Timer         =  0;
              IicPort[Port].State         =  IIC_MANUFACTURER_START;
            }
            else
            {
              if (--IicPort[Port].Retries > 0)
              {
                IicPort[Port].Addr          =  0x01;
                IicPort[Port].Timer         =  0;
                IicPort[Port].State         =  IIC_MANUFACTURER_START;
              }
              else
              {
                IicPort[Port].Initialised   =  1;
                IicPort[Port].State         =  IIC_SETUP_START;
              }
            }
          }
          else
          {
            if (TmpBuffer[0] == 0)
            {
              memcpy((void*)IicStrings[Port].Manufacturer,(void*)"LEGO",IIC_NAME_LENGTH);
              IicStrings[Port].Manufacturer[IIC_NAME_LENGTH]  =  0;
            }
            IicPort[Port].Initialised     =  1;
            IicPort[Port].State           =  IIC_TYPE_START;
          }

        }
        if (++(IicPort[Port].Timer) >= (500 / IIC_TIMER_RESOLUTION))
        {
//          printk("IIC timeout\n");
          IicPort[Port].Addr        =  0x01;
          (*pIic).Status[Port]     &= ~IIC_WRITE_REQUEST;
          (*pIic).Status[Port]     &= ~IIC_DATA_READY;
          IicPort[Port].State       =  IIC_WAITING;
        }
      }
      break;

      case IIC_TYPE_START :
      {
        IicPort[Port].OutBuffer[0]  =  IicPort[Port].Addr;
        IicPort[Port].OutBuffer[1]  =  0x10;
        IicPort[Port].OutBuffer[2]  =  0x00;
        IicPort[Port].OutBuffer[3]  =  0x00;
        IicPort[Port].OutLength     =  2;
        IicPort[Port].InLength      =  8;
        IicPort[Port].Repeat        =  1;
        IicPort[Port].Time          =  0;
        IicPort[Port].Timer         =  0;
        IicPort[Port].State         =  IIC_TYPE_WRITE;
      }
      break;

      case IIC_TYPE_WRITE :
      {
        IicPortSend(Port);
        IicPort[Port].Timer       =  0;
        IicPort[Port].State       =  IIC_TYPE_READ;
      }
      break;

      case IIC_TYPE_READ :
      {
        if (IicPortReceive(Port,TmpBuffer) != BUSY)
        {
          memcpy((void*)IicStrings[Port].SensorType,(void*)TmpBuffer,IIC_NAME_LENGTH);
          IicStrings[Port].SensorType[IIC_NAME_LENGTH]  =  0;

          if (TmpBuffer[0] == 0)
          {
            memcpy((void*)IicStrings[Port].SensorType,(void*)"Store",IIC_NAME_LENGTH);
            IicStrings[Port].Manufacturer[IIC_NAME_LENGTH]  =  0;
						
						IicStrings[Port].Type = 99;
						
          }

          (*pIic).Changed[Port]       =  1;

          IicPort[Port].Initialised   =  1;
          IicPort[Port].Timer         =  0;
          IicPort[Port].State         =  IIC_SETUP_WAIT;
        }
        if (++(IicPort[Port].Timer) >= (500 / IIC_TIMER_RESOLUTION))
        {
          (*pIic).Status[Port]     &= ~IIC_WRITE_REQUEST;
          (*pIic).Status[Port]     &= ~IIC_DATA_READY;
          IicPort[Port].State       =  IIC_WAITING;
        }
      }
      break;

      case IIC_SETUP_WAIT :
      {
        if (++(IicPort[Port].Timer) >= (10000 / IIC_TIMER_RESOLUTION))// [100u]
        {
          IicPort[Port]               =  IicPortDefault;
          IicPort[Port].Timer         =  0;
          IicPort[Port].State         =  IIC_NXT_TEMP_START;
        }
				
				if (++(IicPort[Port].Timer) >= 100)       //IIC发送时间间隔
        {
          if(IicPort[Port].OutBuffer[0] == 0x4C)
					{
						IicPort[Port].OutBuffer[0]  =  0x4C;
						IicPort[Port].OutBuffer[1]  =  0x00;
						IicPort[Port].OutBuffer[2]  =  0x00;
						IicPort[Port].OutBuffer[3]  =  0x00;
						IicPort[Port].OutLength     =  2;
						IicPort[Port].InLength      =  2;
						
						IicPortSend(Port);
						IicPort[Port].State         =  IIC_SETUP_RECEIVE;
					}
					if(IicPort[Port].OutBuffer[0] == 0x02)
					{
						IicPort[Port].OutBuffer[0]  =  0x02;
						IicPort[Port].OutBuffer[1]  =  0x0A+2*IicPort[Port].Mode;
						IicPort[Port].OutBuffer[2]  =  0x00;
						IicPort[Port].OutBuffer[3]  =  0x00;
						IicPort[Port].OutLength     =  2;
						IicPort[Port].InLength      =  2;
						
						IicPortSend(Port);
						IicPort[Port].State         =  IIC_SETUP_RECEIVE;
					}
//					else
//					{
//						IicPort[Port]               =  IicPortDefault;
//						IicPort[Port].Timer         =  0;
//						IicPort[Port].State         =  IIC_NXT_TEMP_START;
//					}
					IicPort[Port].Timer       =  0;
        }
      }
      break;
			
			case IIC_SETUP_RECEIVE:
				if (IicPortReceive(Port,TmpBuffer) != BUSY)
				{
					//传感器缓存区
					Channel_State[Port][0]=Port+1;
					Channel_State[Port][1]=LegoIdtoEstId(IicStrings[Port].Type);
					Channel_State[Port][2]=LegoModetoEstMode(IicStrings[Port].Type, IicPort[Port].Mode);
//					printf("IicPort[Port].Mode:%x\n\r",IicPort[Port].Mode);
				
					if(Channel_State[Port][1] == 6 && Channel_State[Port][2] == 1)//温度模式1（°C）
					{
//						FloatToByte4((float)(((u16)TmpBuffer[1]) | ((u16)TmpBuffer[0])<<8), Channel_State[Port], 0);				//原始值
						FloatToByte4((float)((u16)TmpBuffer[1]>>4 | (((u16)TmpBuffer[0])<<4))*0.0625f, Channel_State[Port], 0);
					}else if(Channel_State[Port][1] == 6 && Channel_State[Port][2] == 2)//温度模式2（°F）
					{
						FloatToByte4((((u16)TmpBuffer[1]>>4 | (((u16)TmpBuffer[0])<<4))*0.0625f*1.8)+32.0f, Channel_State[Port], 0);
					}else if(Channel_State[Port][1] == 9) //能量计
					{
						FloatToByte4((float)((u16)TmpBuffer[1]<<8 | (((u16)TmpBuffer[0])))/1000, Channel_State[Port], 0);
					}

					IicPort[Port].State						= IIC_SETUP_WAIT;
				}
        if (++(IicPort[Port].Timer) >= 2*(10000 / IIC_TIMER_RESOLUTION))// [100u]
        {
          IicPort[Port]               =  IicPortDefault;
          IicPort[Port].Timer         =  0;
          IicPort[Port].State         =  IIC_NXT_TEMP_START;
        }
				break;
			
      case IIC_SETUP_START :
      {
#ifndef DISABLE_FAST_DATALOG_BUFFER
        (*pIic).Actual[Port]        =  0;
        (*pIic).LogIn[Port]         =  0;
#endif

        if (IicStrings[Port].SetupLng)
        {
          IicPort[Port].OutBuffer[0]  =  (UBYTE)((IicStrings[Port].SetupString >> 24) & 0xFF);
          IicPort[Port].OutBuffer[1]  =  (UBYTE)((IicStrings[Port].SetupString >> 16) & 0xFF);
          IicPort[Port].OutBuffer[2]  =  (UBYTE)((IicStrings[Port].SetupString >>  8) & 0xFF);
          IicPort[Port].OutBuffer[3]  =  (UBYTE)((IicStrings[Port].SetupString) & 0xFF);
          IicPort[Port].OutLength     =  IicStrings[Port].SetupLng;
          IicPort[Port].InLength      =  0;
          IicPort[Port].Repeat        =  1;
          IicPort[Port].Time          =  0;
          IicPort[Port].State         =  IIC_SETUP_WRITE;
        }
        else
        {
          IicPort[Port].State         =  IIC_SETUP_READ;
        }
        IicPort[Port].Timer           =  0;
      }
      break;

      case IIC_SETUP_WRITE :
      {
        IicPortSend(Port);
        IicPort[Port].Timer       =  0;
        IicPort[Port].State       =  IIC_SETUP_READ;
      }
      break;

      case IIC_SETUP_READ :
      {
        if (IicPortReceive(Port,TmpBuffer) != BUSY)
        {
          IicPort[Port].State           =  IIC_WAITING;
          if (IicStrings[Port].PollLng)
          {
            IicPort[Port].OutBuffer[0]  =  (UBYTE)((IicStrings[Port].PollString >> 24) & 0xFF);
            IicPort[Port].OutBuffer[1]  =  (UBYTE)((IicStrings[Port].PollString >> 16) & 0xFF);
            IicPort[Port].OutBuffer[2]  =  (UBYTE)((IicStrings[Port].PollString >>  8) & 0xFF);
            IicPort[Port].OutBuffer[3]  =  (UBYTE)((IicStrings[Port].PollString) & 0xFF);
            IicPort[Port].OutLength     =  IicStrings[Port].PollLng;
            if (IicStrings[Port].ReadLng < 0)
            {
              IicPort[Port].InLength    =  0 - IicStrings[Port].ReadLng;
              IicPort[Port].Reverse     =  1;
            }
            else
            {
              IicPort[Port].InLength    =  IicStrings[Port].ReadLng;
              IicPort[Port].Reverse     =  0;
            }
            IicPort[Port].Repeat        =  0;
            IicPort[Port].Time          =  IicStrings[Port].Time;
            IicPort[Port].Timer         =  0;
            IicPort[Port].State         =  IIC_WRITING;
            (*pIic).Status[Port]        =  0;
          }
          IicPort[Port].Initialised     =  1;
        }
        if (++(IicPort[Port].Timer) >= (500 / IIC_TIMER_RESOLUTION))
        {
          (*pIic).Status[Port]     &= ~IIC_WRITE_REQUEST;
          (*pIic).Status[Port]     &= ~IIC_DATA_READY;
          IicPort[Port].State       =  IIC_WAITING;
        }
      }
      break;

      case IIC_WAITING :
      {
        if ((*pIic).Status[Port] & IIC_WRITE_REQUEST)
        {
          if (IicPortBusy(Port) == 0)
          {
            IicPort[Port].Timer       =  0;
            IicPort[Port].State       =  IIC_WRITING;
          }
        }
      }
      break;

      case IIC_WRITING :
      {
        IicPortSend(Port);
        IicPort[Port].State       =  IIC_READING;
      }
      break;

      case IIC_READING :
      {
        if (IicPortReceive(Port,TmpBuffer) != BUSY)
        {

#ifndef DISABLE_FAST_DATALOG_BUFFER
          if (IicPort[Port].InLength > 1)
          {
            if (IicPort[Port].Reverse)
            {
              for (Tmp = 0;Tmp < IicPort[Port].InLength;Tmp++)
              {
                IicPort[Port].InBuffer[Tmp]                  =  TmpBuffer[Tmp];
                (*pIic).Raw[Port][(*pIic).LogIn[Port]][Tmp]  =  TmpBuffer[Tmp];
              }
            }
            else
            {
              for (Tmp = 0;Tmp < IicPort[Port].InLength;Tmp++)
              {
                IicPort[Port].InBuffer[Tmp]       =  TmpBuffer[(IicPort[Port].InLength - 1) - Tmp];
                (*pIic).Raw[Port][(*pIic).LogIn[Port]][Tmp]  =  TmpBuffer[(IicPort[Port].InLength - 1) - Tmp];
              }
            }
          }
          else
          {
            IicPort[Port].InBuffer[0]                  =  TmpBuffer[0];
            IicPort[Port].InBuffer[1]                  =  0;
            (*pIic).Raw[Port][(*pIic).LogIn[Port]][0]  =  TmpBuffer[0];
            (*pIic).Raw[Port][(*pIic).LogIn[Port]][1]  =  0;
          }

          (*pIic).Actual[Port]  =  (*pIic).LogIn[Port];
          (*pIic).Repeat[Port][(*pIic).Actual[Port]]  =  0;

          if (++((*pIic).LogIn[Port]) >= DEVICE_LOGBUF_SIZE)
          {
            (*pIic).LogIn[Port]      =  0;
          }
#else
          if (IicPort[Port].InLength > 1)
          {
            if (IicPort[Port].Reverse)
            {
              for (Tmp = 0;Tmp < IicPort[Port].InLength;Tmp++)
              {
                IicPort[Port].InBuffer[Tmp]       =  TmpBuffer[Tmp];
                (*pIic).Raw[Port][Tmp]            =  TmpBuffer[Tmp];
              }
            }
            else
            {
              for (Tmp = 0;Tmp < IicPort[Port].InLength;Tmp++)
              {
                IicPort[Port].InBuffer[Tmp]       =  TmpBuffer[(IicPort[Port].InLength - 1) - Tmp];
                (*pIic).Raw[Port][Tmp]            =  TmpBuffer[(IicPort[Port].InLength - 1) - Tmp];
              }
            }
          }
          else
          {
            IicPort[Port].InBuffer[0]                  =  TmpBuffer[0];
            IicPort[Port].InBuffer[1]                  =  0;
            (*pIic).Raw[Port][0]  =  TmpBuffer[0];
            (*pIic).Raw[Port][1]  =  0;
          }
#endif

          (*pIic).Status[Port]     |=  IIC_DATA_READY;
          IicPort[Port].State       =  IIC_REPEAT;

          if (IicPort[Port].Repeat != 0)
          {
            IicPort[Port].Repeat--;
            if (IicPort[Port].Repeat == 0)
            {
              IicPort[Port].State       =  IIC_WAITING;
            }
          }
        }
        if (++(IicPort[Port].Timer) >= (5000 / IIC_TIMER_RESOLUTION))
        {
          (*pIic).Status[Port]     &= ~IIC_WRITE_REQUEST;
          (*pIic).Status[Port]     &= ~IIC_DATA_READY;
          IicPort[Port].State       =  IIC_WAITING;
        }
      }
      break;

      case IIC_REPEAT :
      {
        if ((*pIic).Status[Port] & IIC_WRITE_REQUEST)
        {
          (*pIic).Status[Port]      =  IIC_WRITE_REQUEST;
          IicPort[Port].State       =  IIC_WAITING;
        }
        else
        {
          if (++(IicPort[Port].Timer) >= (UWORD)(IicPort[Port].Time / (IIC_TIMER_RESOLUTION / 10)))
          {
            IicPort[Port].Timer       =  0;
            IicPort[Port].State       =  IIC_WRITING;
          }
        }
      }
      break;

      case IIC_EXIT :
      {
        IicPortDisable(Port);
        IicPort[Port]                           =  IicPortDefault;

        (*pIic).Status[Port]                    =  0;

        IicPort[Port].State                     =  IIC_IDLE;
      }
      break;

    }
		}else
		{
			IicPort[Port].State                     =  IIC_IDLE;
			
		}
		
#ifndef DISABLE_FAST_DATALOG_BUFFER
    ((*pIic).Repeat[Port][(*pIic).Actual[Port]]) += (IIC_TIMER_RESOLUTION / 10);
#endif

#ifdef HIGHDEBUG
    if (IicPort[Port].OldState != IicPort[Port].State)
    {
      IicPort[Port].OldState    =  IicPort[Port].State;
      if (IicPort[Port].State  !=  IIC_ENABLE)
      {
        snprintf(IicBuffer,IICBUFFERSIZE,"    %d %s\r\n",Port,IicStateText[IicPort[Port].State]);
      }
      else
      {
        snprintf(IicBuffer,IICBUFFERSIZE,"*** %d %s ***\r\n",Port,IicStateText[IicPort[Port].State]);
      }
      IicWrite(IicBuffer);
    }
#endif

  }

}


/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/adc_sensor.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#include "adc_sensor.h"
#include "usart_sensor.h"
#include "iic_sensor.h"
#include "adc.h"
#include "usart6.h"
#include "ui_control.h"
#include "excutefunction.h"
#include "C_Protocol.h"
#include "input.h"
#include "exti.h"
#include "iic.h"

#define fazhi 2600

const SWORD StepSizeTable[STEP_SIZE_TABLE_ENTRIES] = { 7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
        19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
        50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
        130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
        337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
        876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
        2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
        5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
        15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
    };
const SWORD IndexTable[INDEX_TABLE_ENTRIES] = {
     -1, -1, -1, -1, 2, 4, 6, 8,
     -1, -1, -1, -1, 2, 4, 6, 8
   };

typedef struct
{
  //*****************************************************************************
  // Sound Global variables
  //*****************************************************************************

  int     SoundDriverDescriptor;
  int     hSoundFile;

  DATA8   SoundOwner;
  DATA8   cSoundState;
  SOUND   Sound;
  SOUND	 	*pSound;
  UWORD	  BytesLeft;
  UWORD	  SoundFileFormat;
  UWORD	  SoundDataLength;
  UWORD	  SoundSampleRate;
  UWORD	  SoundPlayMode;
  UWORD   SoundFileLength;
  SWORD   ValPrev;
  SWORD   Index;
  SWORD   Step;
  UBYTE   BytesToWrite;
//  char    PathBuffer[MAX_FILENAME_SIZE];
//  struct  stat FileStatus;
//  UBYTE   SoundData[SOUND_FILE_BUFFER_SIZE + 1]; // Add up for CMD
}
SOUND_GLOBALS;

SOUND_GLOBALS SoundInstance;

INPORT	InputPort[NO_OF_INPUT_PORTS];

//static	UBYTE TestMode = 0;

static	ANALOG  AnalogDefault;

static	ANALOG *pAnalog   =  &AnalogDefault;

//static    UWORD  *pInputs   = (UWORD*)&AnalogDefault;

static	UBYTE Device3State = 0;
static	UWORD Device3StateTimer;

void FloatToByte4(float temp, uint8_t byte[], uint8_t len);
 float Byte4ToFloat(uint8_t byte[], uint8_t len);

//int ADC_BUF0=0,ADC_BUF1=0;

 
void PortDetect(void)
{
//	int i,j;
	int	z = 0;
	
////#ifdef INPUT_DEBUG
	
//	printf("SumPluse:%d\n\r",SumPluse[0]);
//	printf("TIME1:%d\n\r",TIM1->CNT);

//	printf("ADC0:%d\n\r",2*ReadADC1AverageValue(0)*2500/4096);
//	printf("ADC1:%d\n\r",2*ReadADC1AverageValue(1)*2500/4096);
//	printf("ADC2:%d\n\r",2*ReadADC1AverageValue(2)*2500/4096);
//	printf("ADC3:%d\n\r",2*ReadADC1AverageValue(3)*2500/4096);
//	printf("ADC4:%d\n\r",2*ReadADC1AverageValue(4)*2500/4096);
//	printf("ADC5:%d\n\r",2*ReadADC1AverageValue(5)*2500/4096);
//	
//	printf("ADC6:%d\n\r",2*ReadADC1AverageValue(6)*2500/4096);
//	printf("ADC7:%d\n\r",2*ReadADC1AverageValue(7)*2500/4096);
//	printf("ADC8:%d\n\r",2*ReadADC1AverageValue(8)*2500/4096);
//	printf("ADC9:%d\n\r",2*ReadADC1AverageValue(9)*2500/4096);
//	
//	printf("ADC10:%d\n\r",2*ReadADC3AverageValue(0)*2500/4096);
//	printf("ADC11:%d\n\r",2*ReadADC3AverageValue(1)*2500/4096);
//	printf("ADC12:%d\n\r",2*ReadADC3AverageValue(2)*2500/4096);
//	
	printf("Channel_State1  port:%x\n\r",(Channel_State[z][0]));
	printf("Channel_State2    id:%x\n\r",(Channel_State[z][1]));
	printf("Channel_State3  mode:%x\n\r",(Channel_State[z][2]));
	printf("Channel_State4      :%x\n\r",(Channel_State[z][3]));
	printf("Channel_State5      :%x\n\r",(Channel_State[z][4]));
	printf("Channel_State6      :%x\n\r",(Channel_State[z][5]));
	printf("Channel_State7      :%x\n\r",(Channel_State[z][6]));
	printf("Channel_State8      :%x\n\r",(Channel_State[z][7]));
	printf("Channel_State9      :%x\n\r",(Channel_State[z][8]));
	printf("Channel_State10     :%x\n\r",(Channel_State[z][9]));
	printf("Channel_State11     :%x\n\r",(Channel_State[z][10]));
	printf("Channel_State12     :%x\n\r",(Channel_State[z][11]));
	printf("Channel_State13     :%x\n\r",(Channel_State[z][12]));

//	printf("Channel_State:%f\n\r",Byte4ToFloat(Channel_State[z],4));
//	printf("Channel_State:%f\n\r",Byte4ToFloat(Channel_State[z],1));	
//	printf("Channel_State:%f\n\r",Byte4ToFloat(Channel_State[z],5));
//	
//	printf("Uart1RecMes1:%x\n\r",Uart1RecMes[0]);
//	printf("Uart1RecMes1:%x\n\r",Uart1RecMes[1]);
//	printf("Uart1RecMes2:%x\n\r",Uart1RecMes[2]);
//	printf("Uart1RecMes3:%x\n\r",Uart1RecMes[3]);
//	printf("Uart1RecMes4:%x\n\r",Uart1RecMes[4]);
//	printf("Uart1RecMes5:%x\n\r",Uart1RecMes[5]);
//	printf("Uart1RecMes6:%x\n\r",Uart1RecMes[6]);
//	printf("Uart1RecMes7:%x\n\r",Uart1RecMes[7]);
//	printf("Uart1RecMes8:%x\n\r",Uart1RecMes[8]);
//	printf("Uart1RecMes9:%x\n\r",Uart1RecMes[9]);

//#endif

//	for(i=0; i<8; i++)
//	{
//		printf("Channel_State[%x]:%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-\n\r",i,
//							Channel_State[i][0],Channel_State[i][1],Channel_State[i][2],Channel_State[i][3],
//							Channel_State[i][4],Channel_State[i][5],Channel_State[i][6],Channel_State[i][7],
//							Channel_State[i][8],Channel_State[i][9],Channel_State[i][10],Channel_State[i][11],
//							Channel_State[i][12]);
//	}

//			printf("Pluse[0]:%d\r\n",Pluse[0]);
//			printf("Pluse[1]:%d\r\n",Pluse[1]);
//			printf("Pluse[2]:%d\r\n",Pluse[2]);
//			printf("Pluse[3]:%d\r\n",Pluse[3]);
	
//	printf("battery:%d\n\r",ReadADC3AverageValue(0)*2500/4096);

//////电机
//	printf("MOTOR1:%d\n\r",2*ReadADC1AverageValue(6)*2500/4096);
//	printf("MOTOR2:%d\n\r",2*ReadADC1AverageValue(7)*2500/4096);
//	printf("MOTOR3:%d\n\r",2*ReadADC1AverageValue(8)*2500/4096);
//	printf("MOTOR4:%d\n\r",2*ReadADC1AverageValue(9)*2500/4096);
//	
//	printf("Pluse[0]:%d\n\r",Pluse[0]);
//	printf("Pluse[1]:%d\n\r",Pluse[1]);
//	printf("Pluse[2]:%d\n\r",Pluse[2]);
//	printf("Pluse[3]:%d\n\r",Pluse[3]);

////1端口
//	printf("1PIN1:%d\n\r",2*ReadADC3AverageValue(1)*2500/4096);
//	printf("1PIN2:%x\n\r",GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_8));
//	printf("1PIN5:%x\n\r",GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_9));
//	printf("1PIN6:%d\n\r",2*ReadADC3AverageValue(2)*2500/4096);

////2端口
//	printf("2PIN1:%d\n\r",2*ReadADC1AverageValue(0)*2500/4096);
//	printf("2PIN2:%x\n\r",GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_14));
//	printf("2PIN5:%x\n\r",GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_13));
//	printf("2PIN6:%d\n\r",2*ReadADC1AverageValue(1)*2500/4096);

////3端口
//	printf("3PIN1:%d\n\r",2*ReadADC1AverageValue(2)*2500/4096);
//	printf("3PIN2:%x\n\r",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7));
//	printf("3PIN5:%x\n\r",GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_1));
//	printf("3PIN6:%d\n\r",2*ReadADC1AverageValue(3)*2500/4096);

////4端口
//	printf("4PIN1:%d\n\r",2*ReadADC1AverageValue(4)*2500/4096);
//	printf("4PIN2:%x\n\r",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10));
//	printf("4PIN5:%x\n\r",GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11));
//	printf("4PIN6:%d\n\r",2*ReadADC1AverageValue(5)*2500/4096);

//	printf("SystemTime:%d\r\n",SystemTime);
//	printf("SystemCountTime:%d\r\n",SystemCountTime);
//	printf("ProgramTime:%d\r\n",ProgramTime);
//	for(i=0; i<8; i++)
//	{
//		printf("Timer[i].time:%d\r\n",Timer[i].time);
//	}
}

UBYTE PortPinDigital(UBYTE Port, UBYTE Pin)
{
	UBYTE Result = 0;
	switch (Port)
  {
		case Uart1 :
    {
			if(Pin == 1)
			{
				Result = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6);//I_ONA
			}
			if(Pin == 2)
			{
				Result = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_8);//LEGDETA
			}
			if(Pin == 5)
			{
				Result = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_9);//DIGIA0
			}
			if(Pin == 6)
			{
				Result = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7);//DIGIA1
			}
    }
    break;

    case Uart2 :
    {
     if(Pin == 1)
			{
				Result = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_12);//I_ONB
			}
			if(Pin == 2)
			{
				Result = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_14);//LEGDETB
			}
			if(Pin == 5)
			{
				Result = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_13);//DIGIB0
			}
			if(Pin == 6)
			{
				Result = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_11);//DIGIB1
			}
    }
    break;

    case Uart3 :
    {
      if(Pin == 1)
			{
				Result = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_15);//I_ONC
			}
			if(Pin == 2)
			{
				Result = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7);//LEGDETC
			}
			if(Pin == 5)
			{
				Result = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_1);//DIGIC0
			}
			if(Pin == 6)
			{
				Result = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_0);//DIGIC1
			}
    }
    break;

    case Uart4 :
    {
      if(Pin == 1)
			{
				Result = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15);//I_OND
			}
			if(Pin == 2)
			{
				Result = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10);//LEGDETD
			}
			if(Pin == 5)
			{
				Result = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11);//DIGID0
			}
			if(Pin == 6)
			{
				Result = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12);//DIGID1
			}
    }
    break;
  }
	return Result;
}

u16 PortPinAdc(UBYTE Port, UBYTE Pin)
{
	u16 Result = 0.0;
	
	switch (Port)
  {
    case Uart1 :
    {
			if(Pin == 1)
			{
				Result = 2*ReadADC3AverageValue(1)*2500/4096; //ADC_Channel_Vol[14];
			}
			if(Pin == 6)
			{
				Result = 2*ReadADC3AverageValue(2)*2500/4096; //ADC_Channel_Vol[15];
			}
    }
    break;

    case Uart2 :
    {
     if(Pin == 1)
			{
				Result = 2*ReadADC1AverageValue(0)*2500/4096; //ADC_Channel_Vol[12];
			}
			if(Pin == 6)
			{
				Result = 2*ReadADC1AverageValue(1)*2500/4096; //ADC_Channel_Vol[13];
			}
    }
    break;

    case Uart3 :
    {
      if(Pin == 1)
			{
				Result = 2*ReadADC1AverageValue(2)*2500/4096; //ADC_Channel_Vol[10];
			}
			if(Pin == 6)
			{
				Result = 2*ReadADC1AverageValue(3)*2500/4096; //ADC_Channel_Vol[11];
			}
    }
    break;

    case Uart4 :
    {
      if(Pin == 1)
			{
				Result = 2*ReadADC1AverageValue(4)*2500/4096; //ADC_Channel_Vol[8];
			}
			if(Pin == 6)
			{
				Result = 2*ReadADC1AverageValue(5)*2500/4096; //ADC_Channel_Vol[9];
			}
    }
    break;
  }
	return Result;
}
/*
* PORT1 PIN1  PC2
* PORT1 PIN2  PC1
* PORT1 PIN5  PC0
* PORT1 PIN6  PC3
*
*/
u8 TouchTmp = 0, touch_state[4]={0};
void Device3TimerInterrupt1(void)
//static enum hrtimer_restart Device3TimerInterrupt1(struct hrtimer *pTimer)
{
  UBYTE   Port,Tmp;
  UBYTE   Event;
//  UWORD   Tmp;
	
//#ifndef DISABLE_OLD_COLOR
//  UBYTE   *pData;
//#endif
//  hrtimer_forward_now(pTimer,Device3Time);

  switch (Device3State)
  {
    case 0 :
    {
      if (TestMode == 0)
      {
        Device3State  =  1;
      }
    }
    break;

    case 1 :
    {
//      IGENOff;
      Device3StateTimer  =  0;
      Device3State++;
    }
    break;

    case 2 :
    {
      if (++Device3StateTimer >= (DCM_DEVICE_RESET_TIME / DCM_TIMER_RESOLUTION)/10)
      {
//        IGENOn;
        Device3State++;
      }
    }
    break;

    default :
    {
//      for (Port = 0;Port < NO_OF_INPUT_PORTS;Port++)
      for (Port = 0;Port < 4;Port++)
      { // look at one port at a time

        switch (InputPort[Port].State)
        {
          case DCM_INIT :
          { // set input port inactive

//#ifndef DISABLE_OLD_COLOR
//            NxtColorActive[Port]       =  0;
//            NxtColorCommStop(Port);
//#endif
//            InputPortFloat(Port);
            (*pAnalog).InDcm[Port]     =  TYPE_NONE;
            (*pAnalog).InConn[Port]    =  CONN_NONE;
						
						UartPortFlag[Port] = 0;
						IICPortFlag[Port]  = 0;
						
						for (Tmp = 0;Tmp < 13;Tmp++)
						{
							Channel_State[Port][Tmp]=0;
						}
						Channel_State[Port][2]=1;//模式初始化为1
						
//						DeviceID[Port] = 0;
#ifdef	INPUT_DEBUG
	printf("THIS IS PORT %d: TYPE_NONE!!\n\r", Port);
#endif				
            InputPort[Port].Timer      =  0;
            InputPort[Port].Event      =  0;
            InputPort[Port].State    =  DCM_FLOATING_DELAY;
          }
          break;

          case DCM_FLOATING_DELAY :
          { // wait for port pins to float

            if (++(InputPort[Port].Timer) >= (DCM_FLOAT_DELAY / DCM_TIMER_RESOLUTION))
            {
              InputPort[Port].Timer    =  0;
              InputPort[Port].State    =  DCM_FLOATING;
            }
          }
          break;

          case DCM_FLOATING :
          { // pins floating - check and for connection event
            Event  =  0;
            if (!PortPinDigital(Port, 2))
            { // pin 2 low

              Event |=  (0x01 << INPUT_PORT_PIN2);
							
            }
            //if ((*pAnalog).InPin1[Port] < VtoC(IN1_NEAR_5V))
						if (PortPinAdc(Port, 1) < 800)
            { // pin 1 loaded

              Event |=  (0x01 << INPUT_PORT_VALUE);
            }
						if (!PortPinDigital(Port, 5))
            { // pin 5 low

              Event |=  (0x01 << INPUT_PORT_PIN5);
            }
//						if (PortPinDigital(Port, 6))
						if (PortPinAdc(Port, 6)>450)
            { // pin 6 high

              Event |=  (0x01 << INPUT_PORT_PIN6);
            }
            if (InputPort[Port].Event != Event)
            { // pins has changed - reset timer

#ifdef DEBUG
              printk("i ! %d Event = %02X Old = %02X\n",Port,Event,InputPort[Port].Event);
#endif
              InputPort[Port].Event    =  Event;
              InputPort[Port].Timer    =  0;
            }

            if (InputPort[Port].Event)
            { // some event

              if (++(InputPort[Port].Timer) >= (DCM_CONNECT_STABLE_DELAY / DCM_TIMER_RESOLUTION))
              {
                // some event is stable

                InputPort[Port].State  =  DCM_CONNECTION;
              }
            }
//						printf("THIS IS : HERE!!%d",InputPort[Port].State);
          }
          break;

          case DCM_CONNECTION :
          { // something is connected - try to evaluate

            if (InputPort[Port].Event & (0x01 << INPUT_PORT_PIN2))
            { // pin 2 is low

              InputPort[Port].State  =  DCM_PIN2_LOW;
            }
            else
            {
              if (InputPort[Port].Event & (0x01 << INPUT_PORT_VALUE))
              { // pin 1 is loaded

                InputPort[Port].State  =  DCM_PIN1_LOADED;
              }
              else
              {
                if (InputPort[Port].Event & (0x01 << INPUT_PORT_PIN6))
                { // pin 6 is high

                  InputPort[Port].State  =  DCM_PIN6_HIGH;
                }
                else
                {
                  if (InputPort[Port].Event & (0x01 << INPUT_PORT_PIN6))
                  { // pin 5 is low

                    InputPort[Port].State  =  DCM_PIN5_LOW;
                  }
                  else
                  { // ?

                    InputPort[Port].State  =  DCM_INIT;
                  }
                }
              }
            }
#ifndef DISABLE_FAST_DATALOG_BUFFER
            (*pAnalog).Actual[Port]     =  0;
            (*pAnalog).LogIn[Port]      =  0;
            (*pAnalog).LogOut[Port]     =  0;
#endif
          }
          break;

          case DCM_PIN2_LOW :
          {
            InputPort[Port].Connected   =  1;
//            InputPortFloat(Port);
            InputPort[Port].Timer       =  0;
            InputPort[Port].State       =  DCM_CONNECTED_WAITING_FOR_PIN2_HIGH;

//            if ((!(InputPort[Port].Event & (0x01 << INPUT_PORT_PIN5))) && (InputPort[Port].Event & (0x01 << INPUT_PORT_PIN6)))
//            { // pin 5 and 6 is high

////              if ((*pAnalog).InPin1[Port] < VtoC(IN1_NEAR_GND))
//							if (PortPinAdc(Port, 1) < 0.1f)
//              { // nxt color sensor

//                (*pAnalog).InDcm[Port]      =  TYPE_NXT_COLOR;
////#ifndef DISABLE_OLD_COLOR
////                (*pAnalog).InConn[Port]     =  CONN_NXT_COLOR;
////                InputPort[Port].State       =  DCM_NXT_COLOR_INIT;
////#else
//                (*pAnalog).InConn[Port]     =  CONN_NXT_DUMB;
//								
//#ifdef	INPUT_DEBUG
//	printf("THIS IS PORT %d: NXT_COLOR!!\n\r",Port);
//#endif	
//								
////#endif
//              }
//              else
//              { // nxt IIC sensor

//                (*pAnalog).InDcm[Port]      =  TYPE_NXT_IIC;
//                (*pAnalog).InConn[Port]     =  CONN_NXT_IIC;
//								
//#ifdef	INPUT_DEBUG
//	printf("THIS IS PORT %d: NXT_IIC!!\n\r", Port);
//#endif	
//              }
//            }
//            else
//            {
//              if (InputPort[Port].Event & (0x01 << INPUT_PORT_PIN5))
//              { // nxt light sensor

//                if (InputPort[Port].Event & (0x01 << INPUT_PORT_PIN6))
//                { // nxt test sensor

//                  (*pAnalog).InDcm[Port]    =  TYPE_NXT_TEST;
//                  (*pAnalog).InConn[Port]   =  CONN_NXT_DUMB;
//									
//#ifdef	INPUT_DEBUG
//	printf("THIS IS PORT %d: NXT_TEST!!\n\r", Port);
//#endif	
//                }
//                else
//                {
//                  (*pAnalog).InDcm[Port]    =  TYPE_NXT_LIGHT;
//                  (*pAnalog).InConn[Port]   =  CONN_NXT_DUMB;
//									
//#ifdef	INPUT_DEBUG
//	printf("THIS IS PORT %d: NXT_LIGHT!!\n\r", Port);
//#endif	
//                }
//              }
//              else
//              {
////                if ((*pAnalog).InPin1[Port] < VtoC(IN1_NEAR_GND))
//								if(PortPinAdc(Port, 1) < 0.1f)
//                { // nxt color sensor

//                  (*pAnalog).InDcm[Port]      =  TYPE_NXT_COLOR;
////#ifndef DISABLE_OLD_COLOR
////                  (*pAnalog).InConn[Port]     =  CONN_NXT_COLOR;
////                  InputPort[Port].State       =  DCM_NXT_COLOR_INIT;
////#else
//                  (*pAnalog).InConn[Port]     =  CONN_NXT_DUMB;

//#ifdef	INPUT_DEBUG
//	printf("THIS IS PORT %d: NXT_COLOR!!\n\r", Port);
//#endif
//									
////#endif
//									
//                }
//                else
//                {
									
////                  if ((*pAnalog).InPin1[Port] > VtoC(IN1_NEAR_5V))
//									if (PortPinAdc(Port, 1) > 4.8f)
//                  { // nxt touch sensor

//                    (*pAnalog).InDcm[Port]    =  TYPE_NXT_TOUCH;
//                    (*pAnalog).InConn[Port]   =  CONN_NXT_DUMB;

//#ifdef	INPUT_DEBUG
//	printf("THIS IS PORT %d: NXT_TOUCH!!\n\r", Port);
//#endif
//										
//                  }
//                  else
//                  {
//										
////                    if (((*pAnalog).InPin1[Port] > VtoC(IN1_TOUCH_LOW)) && ((*pAnalog).InPin1[Port] < VtoC(IN1_TOUCH_HIGH)))
//										if (PortPinAdc(Port, 1) < 0.85f && PortPinAdc(Port, 1) > 0.95f)
//                    { // nxt touch sensor

//                      InputPort[Port].Timer     =  0;
//                      InputPort[Port].Value     =  ADC_Channel_Vol[14];// (*pAnalog).InPin1[Port];
//                      InputPort[Port].State     =  DCM_NXT_TOUCH_CHECK;
//                    }
//                    else
//                    { // nxt sound sensor

                      (*pAnalog).InDcm[Port]    =  TYPE_NXT_SOUND;
                      (*pAnalog).InConn[Port]   =  CONN_NXT_DUMB;

//											DeviceID[Port] = 9;
											
										//传感器缓存区
										Channel_State[Port][0]=Port+1;
										Channel_State[Port][1]=LegoIdtoEstId(0x03);;; //NXT_SOUND
//										Channel_State[Port][2]=0;
										
#ifdef	INPUT_DEBUG
	printf("THIS IS PORT %d: NXT_SOUND!!\n\r", Port);
#endif
//                    }
//                  }

//                }
//              }
//            }
          }
          break;

          case DCM_NXT_TOUCH_CHECK :
          {
//            if (++(InputPort[Port].Timer) >= (DCM_TOUCH_DELAY / DCM_TIMER_RESOLUTION))
//            {
//              InputPort[Port].State       =  DCM_CONNECTED_WAITING_FOR_PIN2_HIGH;
////              if (((*pAnalog).InPin1[Port] > (InputPort[Port].Value - 10)) && ((*pAnalog).InPin1[Port] < (InputPort[Port].Value + 10)))
//							if (PortPinAdc(Port, 1) < InputPort[Port].Value - 0.01 && PortPinAdc(Port, 1) > InputPort[Port].Value + 0.01 )
//              { // nxt touch sensor

//                (*pAnalog).InDcm[Port]    =  TYPE_NXT_TOUCH;
//                (*pAnalog).InConn[Port]   =  CONN_NXT_DUMB;
//								
//#ifdef	INPUT_DEBUG
//	printf("THIS IS PORT %d: NXT_TOUCH!!\n\r", Port);
//#endif
//              }
//              else
//              { // nxt sound sensor

//                (*pAnalog).InDcm[Port]    =  TYPE_NXT_SOUND;
//                (*pAnalog).InConn[Port]   =  CONN_NXT_DUMB;
//								
////								DeviceID[Port] = 9;
//#ifdef	INPUT_DEBUG
//	printf("THIS IS PORT %d: NXT_SOUND!!\n\r", Port);
//#endif
//              }
//            }
          }
          break;

//#ifndef DISABLE_OLD_COLOR

//          case DCM_NXT_COLOR_INIT :
//          {
//            NxtcolorCmd[Port]           =  0;
//            NxtColorCommStop(Port);

//            InputPort[Port].Timer       =  0;
//            InputPort[Port].State       =  DCM_NXT_COLOR_WAIT;
//          }
//          break;

//          case DCM_NXT_COLOR_WAIT :
//          {
//            if (NxtcolorCmd[Port] == NxtcolorLatchedCmd[Port])
//            {
//              NxtColorCommStart(Port,InputPort[Port].Cmd);
//              InputPort[Port].State       =  DCM_NXT_COLOR_BUSY;
//            }
//          }
//          break;

//          case DCM_NXT_COLOR_BUSY :
//          {
//            if (NxtColorCommReady(Port))
//            {
//              NxtcolorCmd[Port]           =  InputPort[Port].Cmd;
//              InputPort[Port].Timer       =  0;
//              InputPort[Port].State       =  DCM_CONNECTED_WAITING_FOR_PIN2_HIGH;

//              pData   =  (UBYTE*)&((*pAnalog).NxtCol[Port]);

//              for (Tmp = 0;Tmp < NXTCOLOR_BYTES;Tmp++)
//              {
//                *pData  =  NxtColorBuffer[Port][Tmp];
//                pData++;
//              }

//              NxtColorCommStop(Port);

//              NxtColorActive[Port]  =  1;
//            }
//            if (++(InputPort[Port].Timer) > (DCM_NXT_COLOR_TIMEOUT / DCM_TIMER_RESOLUTION))
//            {
////#ifdef DEBUG
////              printk("i ! %d NXT Color sensor timeout\n",Port);
////#endif
//              InputPort[Port].Timer       =  0;
//              InputPort[Port].State       =  DCM_CONNECTED_WAITING_FOR_PIN2_HIGH;
//              NxtColorCommStop(Port);
//            }
//          }
//          break;

//#endif

          case DCM_CONNECTED_WAITING_FOR_PIN2_HIGH :    //NXT 断开
          {
						//SOUND 数据存入

//						FloatToByte4(PortPinAdc(Port, 1), Channel_State[Port], 0);
						FloatToByte4(100 - (PortPinAdc(Port, 1))/14, Channel_State[Port], 0);
						
							//if (PINRead(Port,INPUT_PORT_PIN2))
            if(PortPinDigital(Port, 2))
            {
              if (++(InputPort[Port].Timer) >= 5*(DCM_EVENT_STABLE_DELAY / DCM_TIMER_RESOLUTION))
              {
								
								FloatToByte4(0.0, Channel_State[Port], 0);//清空SOUND数据
								
								SoundInstance.ValPrev = SOUND_ADPCM_INIT_VALPREV;
								SoundInstance.Index = SOUND_ADPCM_INIT_INDEX;
								SoundInstance.Step = StepSizeTable[SoundInstance.Index];
								
								
                InputPort[Port].Connected   =  0;
                InputPort[Port].State       =  DCM_INIT;
              }
            }
            else
            {
              InputPort[Port].Timer    =  0;
            }
          }
          break;

          case DCM_PIN1_LOADED :
          {
//            if ((*pAnalog).InPin1[Port] > VtoC(IN1_NEAR_PIN2))
						if (PortPinAdc(Port, 1) > 1500)
            {
              (*pAnalog).InDcm[Port]      =  TYPE_ERROR;
              (*pAnalog).InConn[Port]     =  CONN_ERROR;
							
#ifdef	INPUT_DEBUG
	printf("THIS IS PORT %d: ERROR!!\n\r", Port);
#endif
            }
            else
            {
//              if ((*pAnalog).InPin1[Port] < VtoC(IN1_NEAR_GND))
							if (PortPinAdc(Port, 1) < 100 )
              {
                (*pAnalog).InDcm[Port]    =  TYPE_UNKNOWN;
                (*pAnalog).InConn[Port]   =  CONN_INPUT_UART;
								
							InputPort[Port].State       =  DCM_CONNECTED_WAITING_FOR_PIN1_TO_FLOAT;
							UartPortFlag[Port] = 1;
#ifdef	INPUT_DEBUG
	printf("THIS IS PORT %d: INPUT_UART!!\n\r", Port);
#endif
								if(Port == 0)
								{
									GPIO_ResetBits(GPIOF,GPIO_Pin_10);  //A
								}
								if(Port == 1)
								{
									GPIO_ResetBits(GPIOC,GPIO_Pin_2);  //B
								}
								if(Port == 2)
								{
									GPIO_ResetBits(GPIOE,GPIO_Pin_8);  //C
								}
								if(Port == 3)
								{
									GPIO_ResetBits(GPIOE,GPIO_Pin_9);  //D
								}
              }
              else
              {
                (*pAnalog).InDcm[Port]    =  TYPE_UNKNOWN;
                (*pAnalog).InConn[Port]   =  CONN_INPUT_DUMB;
								
								//传感器缓存区
								Channel_State[Port][0]=Port+1;
								Channel_State[Port][1]=LegoIdtoEstId(0x10);//INPUT_DUMB
								Channel_State[Port][2]=1;
								
								InputPort[Port].State       =  DCM_CONNECTED_WAITING_TOUCH;
								
//								DeviceID[Port] = 6;
#ifdef	INPUT_DEBUG
	printf("THIS IS PORT %d: INPUT_DUMB!!\n\r", Port);
#endif
								
              }
            }
            InputPort[Port].Connected   =  1;
            InputPort[Port].Timer       =  0;
          }
          break;
					
					case DCM_CONNECTED_WAITING_TOUCH:
					{
						switch(touch_state[Port])
						{
							case 0:
								if( PortPinAdc(Port, 6) >= 1500)
								{
									Channel_State[Port][3] = 0x01;
									touch_state[Port] = 1;
								}
								break;
							case 1:
								if( PortPinAdc(Port, 6) < 800)
								{
									Channel_State[Port][3] = 0x02;
									touch_state[Port] = 2;
								}
								break;
							case 2:
								if (++TouchTmp >= 1)
								{
									Channel_State[Port][3] = 0x00;
									TouchTmp = 0;
									touch_state[Port] = 0;
								}
								break;
							default:touch_state[Port] = 0;
								break;
						}
						
						
//						//TOUCH 数据存入
//						if( PortPinAdc(Port, 6) >= 1500)
//						{
//							TouchTmp = 0x01;
//							Channel_State[Port][3] = 0x01;
////							FloatToByte4(1.0, Channel_State[Port], 0);
//						}else if( PortPinAdc(Port, 6) <=800)
//						{
////							if(TouchTmp == 0x01)
////							{
////								TouchTmp = 0x00;
////								Channel_State[Port][3] = 0x02;//(碰撞)
////							}else
////							{
//								Channel_State[Port][3] = 0x00;
////							}
////							FloatToByte4(0.0, Channel_State[Port], 0);
//						}else{}
						
							
							
//            if ((*pAnalog).InPin1[Port] > VtoC(IN1_NEAR_5V))
						if (PortPinAdc(Port, 1) > 800)
            { // pin 1 floating

              if (++(InputPort[Port].Timer) >= (DCM_EVENT_STABLE_DELAY / DCM_TIMER_RESOLUTION))
              {
								
								FloatToByte4(0.0, Channel_State[Port], 0);//清空TOUCH数据
								
                InputPort[Port].Connected   =  0;
                InputPort[Port].State       =  DCM_INIT;
              }
            }
            else
            {
              InputPort[Port].Timer    =  0;
            }
          }
					break;

          case DCM_CONNECTED_WAITING_FOR_PIN1_TO_FLOAT :
          {
//            if ((*pAnalog).InPin1[Port] > VtoC(IN1_NEAR_5V))
						if (PortPinAdc(Port, 1) > 800)
            { // pin 1 floating

              if (++(InputPort[Port].Timer) >= (DCM_EVENT_STABLE_DELAY / DCM_TIMER_RESOLUTION))
              {
                InputPort[Port].Connected   =  0;
                InputPort[Port].State       =  DCM_INIT;
              }
            }
            else
            {
              InputPort[Port].Timer    =  0;
            }
          }
          break;

          case DCM_PIN6_HIGH :
          { // nxt IIC sensor

            (*pAnalog).InDcm[Port]      =  TYPE_NXT_IIC;
            (*pAnalog).InConn[Port]     =  CONN_NXT_IIC;
						
						
#ifdef	INPUT_DEBUG
	printf("THIS IS PORT %d: NXT_IIC!!\n\r", Port);
#endif
            
//						IIC_init_port(Port);
//						IIC_Init();
						if(Port == 0)
						{
							GPIO_SetBits(GPIOF,GPIO_Pin_10);  //A
//							IIC_SCL_1	=	1;	IIC_SDA_1	=	1;
						}
						if(Port == 1)
						{
							GPIO_SetBits(GPIOC,GPIO_Pin_2);  //B
//							IIC_SCL_2	=	1;	IIC_SDA_2	=	1;
						}
						if(Port == 2)
						{
							GPIO_SetBits(GPIOE,GPIO_Pin_8);  //C
//							IIC_SCL_3	=	1;	IIC_SDA_3	=	1;
						}
						if(Port == 3)
						{
							GPIO_SetBits(GPIOE,GPIO_Pin_9);  //D
//							IIC_SCL_4	=	1;	IIC_SDA_4	=	1;
						}
						
						IICPortFlag[Port]  = 1;
						if (++(InputPort[Port].Timer) >= (DCM_EVENT_STABLE_DELAY / DCM_TIMER_RESOLUTION))
						{
							InputPort[Port].Connected   =  1;
							InputPort[Port].Timer       =  0;
							InputPort[Port].State       =  DCM_CONNECTED_WAITING_FOR_PIN6_LOW;
						}
          }
          break;

          case DCM_CONNECTED_WAITING_FOR_PIN6_LOW :
          {
//            if (!(PINRead(Port,INPUT_PORT_PIN6)))
//            if (!PortPinDigital(Port, 6))
            if (PortPinAdc(Port, 6)<fazhi)
						{
              if (++(InputPort[Port].Timer) >= (DCM_EVENT_STABLE_DELAY / DCM_TIMER_RESOLUTION))
              {
                InputPort[Port].Connected   =  0;
                InputPort[Port].State       =  DCM_INIT;
								
								
//								IIC_init_port(Port);
//								IIC_Init();
//								if(Port == 0)
//								{
//									IIC_SCL_1	=	0;	IIC_SDA_1	=	0;
//								}
//								if(Port == 1)
//								{
//									IIC_SCL_2	=	0;	IIC_SDA_2	=	0;
//								}
//								if(Port == 2)
//								{
//									IIC_SCL_3	=	0;	IIC_SDA_3	=	0;
//								}
//								if(Port == 3)
//								{
//									IIC_SCL_4	=	0;	IIC_SDA_4	=	0;
//								}
              }
            }
            else
            {
              InputPort[Port].Timer    =  0;
            }
          }
          break;

          case DCM_PIN5_LOW :
          {
//            (*pAnalog).InDcm[Port]      =  TYPE_ERROR;
//            (*pAnalog).InConn[Port]     =  CONN_ERROR;
//						
//#ifdef	INPUT_DEBUG
//	printf("THIS IS PORT %d: NXT_IIC!!\n\r", Port);
//#endif
//            InputPort[Port].Connected   =  1;
//            InputPort[Port].Timer       =  0;
//            InputPort[Port].State       =  DCM_CONNECTED_WAITING_FOR_PIN5_HIGH;
          }
          break;

          case DCM_CONNECTED_WAITING_FOR_PIN5_HIGH :
          {
						//if (!(PINRead(Port,INPUT_PORT_PIN5)))
            if (!PortPinDigital(Port, 5))
            {
              if (++(InputPort[Port].Timer) >= (DCM_EVENT_STABLE_DELAY / DCM_TIMER_RESOLUTION))
              {
                InputPort[Port].Connected   =  0;
                InputPort[Port].State       =  DCM_INIT;
              }
            }
            else
            {
              InputPort[Port].Timer    =  0;
            }
          }
          break;

          default :
          {
            InputPort[Port].State  =  DCM_INIT;
          }
          break;

        }
#ifdef DEBUG
        if (InputPort[Port].OldState != InputPort[Port].State)
        {
          InputPort[Port].OldState    =  InputPort[Port].State;

          printk("i   %d %s\n",Port,DcmStateText[InputPort[Port].State]);
        }
#endif

      }
/*
    *****************************************************************************

      for (Port = 0;Port < NO_OF_OUTPUT_PORTS;Port++)
      { // look at one port at a time

        switch (OutputPort[Port].State)
        {
          case DCM_INIT :
          { // set output port inactive

            OutputPortFloat(Port);
            (*pAnalog).OutDcm[Port]    =  TYPE_NONE;
            (*pAnalog).OutConn[Port]   =  CONN_NONE;
            OutputPort[Port].Timer     =  0;
            OutputPort[Port].Event     =  0;
            OutputPort[Port].State      =  DCM_FLOATING_DELAY;
          }
          break;

          case DCM_FLOATING_DELAY :
          { // wait for port pins to float

            if (++(OutputPort[Port].Timer) >= (DCM_FLOAT_DELAY / DCM_TIMER_RESOLUTION))
            {
              OutputPort[Port].Timer   =  0;
              OutputPort[Port].State   =  DCM_FLOATING;
            }
          }
          break;

          case DCM_FLOATING :
          { // pins floating - check and for connection event

            Event  =  0;

#ifdef FINALB
            if ((POUTRead(Port,OUTPUT_PORT_PIN6)))
            { // pin 6 low

              Event |=  (0x01 << OUTPUT_PORT_PIN6);
            }
#else
            if (!(POUTRead(Port,OUTPUT_PORT_PIN6)))
            { // pin 6 low

              Event |=  (0x01 << OUTPUT_PORT_PIN6);
            }
#endif
            if (((*pAnalog).OutPin5[Port] < VtoC(OUT5_BALANCE_LOW)) || ((*pAnalog).OutPin5[Port] > VtoC(OUT5_BALANCE_HIGH)))
            { // pin 5 out of balance

              Event |=  (0x01 << OUTPUT_PORT_VALUE);
            }

            if (OutputPort[Port].Event != Event)
            { // pins has changed - reset timer

              OutputPort[Port].Event   =  Event;
              OutputPort[Port].Timer   =  0;
            }

            if (OutputPort[Port].Event)
            { // some event

              if (++(OutputPort[Port].Timer) >= (DCM_CONNECT_STABLE_DELAY / DCM_TIMER_RESOLUTION))
              {
                // some event is stable - store value on connection 5

                OutputPort[Port].Value5Float  =  CtoV((*pAnalog).OutPin5[Port]);
                OutputPort[Port].Timer        =  0;
                POUTLow(Port,OUTPUT_PORT_PIN6);
                OutputPort[Port].State        =  DCM_WAITING_FOR_PIN6_LOW;
              }
            }
          }
          break;

          case DCM_WAITING_FOR_PIN6_LOW :
          {
            if (++(OutputPort[Port].Timer) >= (DCM_LOW_DELAY / DCM_TIMER_RESOLUTION))
            {
              OutputPort[Port].Value5Low    =  CtoV((*pAnalog).OutPin5[Port]);
              OutputPort[Port].State        =  DCM_CONNECTION;
              POUTFloat(Port,OUTPUT_PORT_PIN6);
            }
          }
          break;

          case DCM_CONNECTION :
          { // something is connected - try to evaluate

            OutputPort[Port].State      =  DCM_CONNECTED_WAITING_FOR_PORT_OPEN;
            Tmp  =  ADC_REF;
            Tmp +=  OutputPort[Port].Value5Float;
            Tmp -=  OutputPort[Port].Value5Low;

            if ((Tmp > (ADC_REF - 50)) && (Tmp < (ADC_REF + 50)))
            { // Value5Float is equal to Value5Low

              if ((OutputPort[Port].Value5Float >= OUT5_BALANCE_LOW) && (OutputPort[Port].Value5Float <= OUT5_BALANCE_HIGH) && (OutputPort[Port].Event & (0x01 << OUTPUT_PORT_PIN6)))
              { // NXT TOUCH SENSOR, NXT SOUND SENSOR or NEW UART SENSOR

                (*pAnalog).OutDcm[Port]     =  TYPE_ERROR;
                (*pAnalog).OutConn[Port]    =  CONN_ERROR;
                OutputPort[Port].Connected  =  1;
              }
              else
              {
                if (OutputPort[Port].Value5Float < OUT5_NEAR_GND)
                { // NEW DUMB SENSOR

                  (*pAnalog).OutDcm[Port]     =  TYPE_ERROR;
                  (*pAnalog).OutConn[Port]    =  CONN_ERROR;
                  OutputPort[Port].Connected  =  1;
                }
                else
                {
                  if ((OutputPort[Port].Value5Float >= OUT5_LIGHT_LOW) && (OutputPort[Port].Value5Float <= OUT5_LIGHT_HIGH))
                  { // NXT LIGHT SENSOR

                    (*pAnalog).OutDcm[Port]     =  TYPE_ERROR;
                    (*pAnalog).OutConn[Port]    =  CONN_ERROR;
                    OutputPort[Port].Connected  =  1;
                  }
                  else
                  {
                    if ((OutputPort[Port].Value5Float >= OUT5_IIC_LOW) && (OutputPort[Port].Value5Float <= OUT5_IIC_HIGH))
                    { // NXT IIC SENSOR

                      (*pAnalog).OutDcm[Port]     =  TYPE_ERROR;
                      (*pAnalog).OutConn[Port]    =  CONN_ERROR;
                      OutputPort[Port].Connected  =  1;
                    }
                    else
                    {
                      if (OutputPort[Port].Value5Float < OUT5_BALANCE_LOW)
                      {
                        if (OutputPort[Port].Value5Float > OUT5_MINITACHO_HIGH2)
                        {
                          (*pAnalog).OutDcm[Port]     =  TYPE_NEWTACHO;
                        }
                        else
                        {
                          if (OutputPort[Port].Value5Float > OUT5_MINITACHO_LOW2)
                          {
                            (*pAnalog).OutDcm[Port]   =  TYPE_MINITACHO;
                          }
                          else
                          {
                            (*pAnalog).OutDcm[Port]   =  TYPE_TACHO;
                          }
                        }
                        (*pAnalog).OutConn[Port]      =  CONN_OUTPUT_TACHO;
                        OutputPort[Port].Connected    =  1;
                      }
                      else
                      {
                        POUTHigh(Port,OUTPUT_PORT_PIN5W);
                        OutputPort[Port].State        =  DCM_WAITING_FOR_PIN5_LOW;
                      }
                    }
                  }
                }
              }

            }
            else
            { // Value5Float is NOT equal to Value5Low

              if ((OutputPort[Port].Value5Low > OUT5_NEAR_GND) && (OutputPort[Port].Value5Low < OUT5_BALANCE_LOW))
              { // NEW ACTUATOR

                (*pAnalog).OutPin5Low[Port]   =  OutputPort[Port].Value5Low;
                (*pAnalog).OutDcm[Port]       =  TYPE_UNKNOWN;
                (*pAnalog).OutConn[Port]      =  CONN_OUTPUT_DUMB;
                OutputPort[Port].Connected    =  1;
              }
              else
              {
                (*pAnalog).OutDcm[Port]       =  TYPE_ERROR;
                (*pAnalog).OutConn[Port]      =  CONN_ERROR;
                OutputPort[Port].Connected    =  1;
              }

            }
            OutputPort[Port].Timer      =  0;
#ifdef DEBUG
            if (OutputPort[Port].Connected)
            {
              printk("\r\no    %d Type = %c, Float = %u, Low = %u\r\n",Port,(char)(*pAnalog).OutDcm[Port],(unsigned int)OutputPort[Port].Value5Float,(unsigned int)OutputPort[Port].Value5Low);
            }
#endif
          }
          break;

          case DCM_WAITING_FOR_PIN5_LOW :
          {
            if (++(OutputPort[Port].Timer) >= (DCM_LOW_DELAY / DCM_TIMER_RESOLUTION))
            {
              OutputPort[Port].Value5Low    =  CtoV((*pAnalog).OutPin5[Port]);
              OutputPort[Port].State        =  DCM_CONNECTION;
              POUTLow(Port,OUTPUT_PORT_PIN5W);
              (*pAnalog).OutConn[Port]      =  CONN_OUTPUT_TACHO;
              if (OutputPort[Port].Value5Low < OUT5_MINITACHO_LOW1)
              {
                (*pAnalog).OutDcm[Port]     =  TYPE_ERROR;
                (*pAnalog).OutConn[Port]    =  CONN_ERROR;
              }
              else
              {
                if (OutputPort[Port].Value5Low < OUT5_MINITACHO_HIGH1)
                {
                  (*pAnalog).OutDcm[Port]   =  TYPE_MINITACHO;
                }
                else
                {
                  (*pAnalog).OutDcm[Port]   =  TYPE_TACHO;
                }
              }
              OutputPort[Port].Connected    =  1;
#ifdef DEBUG
              printk("\r\no   %d Type = %03u, Float = %u, Low = %u\r\n",Port,(char)(*pAnalog).OutDcm[Port],(unsigned int)OutputPort[Port].Value5Float,(unsigned int)OutputPort[Port].Value5Low);
#endif
              OutputPort[Port].State        =  DCM_CONNECTED_WAITING_FOR_PORT_OPEN;
            }
          }
          break;

          case DCM_CONNECTED_WAITING_FOR_PORT_OPEN :
          {
            if (((*pAnalog).OutPin5[Port] < VtoC(OUT5_BALANCE_LOW)) || ((*pAnalog).OutPin5[Port] > VtoC(OUT5_BALANCE_HIGH)))
            { // connection 5 out of balance

              OutputPort[Port].Timer      =  0;
            }
#ifdef FINALB
            if ((POUTRead(Port,OUTPUT_PORT_PIN6)))
            { // pin 6 low

              OutputPort[Port].Timer      =  0;
            }
#else
            if (!(POUTRead(Port,OUTPUT_PORT_PIN6)))
            { // pin 6 low

              OutputPort[Port].Timer      =  0;
            }
#endif
            if (++(OutputPort[Port].Timer) >= (DCM_EVENT_STABLE_DELAY / DCM_TIMER_RESOLUTION))
            {
              OutputPort[Port].Connected  =  0;
              OutputPort[Port].State      =  DCM_INIT;
            }
          }
          break;

          default :
          {
            OutputPort[Port].State        =  DCM_INIT;
          }
          break;

        }
#ifdef DEBUG
        if (OutputPort[Port].OldState != OutputPort[Port].State)
        {
          OutputPort[Port].OldState    =  OutputPort[Port].State;

          printk("o   %d %s\n",Port,DcmStateText[OutputPort[Port].State]);
        }
#endif

      }
			*/
    }
    break;

  }
}

UBYTE cSoundGetAdPcmValue(UBYTE Delta)  // Call ONLY when cSoundInitAdPcm has been called :-)
{
  SWORD VpDiff;
  UBYTE Sign;

  //
  SoundInstance.Step = StepSizeTable[SoundInstance.Index];
  SoundInstance.Index += IndexTable[Delta]; // Find new index value (for later)

  if(SoundInstance.Index < 0)
    SoundInstance.Index = 0;
  else
  {
    if(SoundInstance.Index > (STEP_SIZE_TABLE_ENTRIES - 1))
      SoundInstance.Index = STEP_SIZE_TABLE_ENTRIES - 1;
  }

  Sign = Delta & 8;                     // Separate sign
  Delta = Delta & 7;                    // Separate magnitude

  VpDiff = SoundInstance.Step >> 3;     // Compute difference and new predicted value

  if(Delta & 4) VpDiff += SoundInstance.Step;
  if(Delta & 2) VpDiff += SoundInstance.Step >> 1;
  if(Delta & 1) VpDiff += SoundInstance.Step >> 2;

  if(Sign)
    SoundInstance.ValPrev -= VpDiff;    // "Add" with sign
  else
    SoundInstance.ValPrev += VpDiff;

  if(SoundInstance.ValPrev > 255)       // Clamp value to 8-bit unsigned
  {
    SoundInstance.ValPrev = 255;
  }
  else
  {
    if(SoundInstance.ValPrev < 0)
    {
      SoundInstance.ValPrev = 0;
    }
  }

  SoundInstance.Step = StepSizeTable[SoundInstance.Index];  // Update step value

  return((UBYTE)SoundInstance.ValPrev);                     // Return decoded byte (nibble xlated -> 8 bit)
}



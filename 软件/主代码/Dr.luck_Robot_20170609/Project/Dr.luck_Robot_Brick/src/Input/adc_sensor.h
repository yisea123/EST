/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/adc_sensor.h
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __ADC_SENSOR_H
#define __ADC_SENSOR_H 	

#include "sys.h"
#include "lego_include.h"

#define   IN_CONNECT_STEADY_TIME        350   //  [mS]  time needed to be sure that the connection is steady
#define   IN_DISCONNECT_STEADY_TIME     100   //  [mS]  time needed to be sure that the disconnection is steady

#define   IN1_NEAR_5V                   4800  //  [mV]  higher values mean that connection 1 is floating
#define   IN1_NEAR_PIN2                 3100  //  [mV]  higher values mean that connection 1 is shorted to connection 2 (5000 * 18K / (18K + 10k))
#define   IN1_TOUCH_HIGH                950   //  [mV]  values in between these limits means that an old touch sensor is connected
#define   IN1_TOUCH_LOW                 850   //  [mV]
#define   IN1_NEAR_GND                  100   //  [mV]  lower  values mean that connection 1 is shorted to connection 3
#define   IN6_NEAR_GND                  150   //  [mV]  lower  values mean that connection 6 is floating

// DEVICE3 ********************************************************************

#define   DCM_TIMER_RESOLUTION          10                        // [mS]
#define   DCM_DEVICE_RESET_TIME         2000                      // [mS]
#define   DCM_FLOAT_DELAY               20                        // [mS]
#define   DCM_LOW_DELAY                 20                        // [mS]
#define   DCM_TOUCH_DELAY               20                        // [mS]
#define   DCM_CONNECT_STABLE_DELAY      IN_CONNECT_STEADY_TIME    // [mS]
#define   DCM_EVENT_STABLE_DELAY        IN_DISCONNECT_STEADY_TIME // [mS]

//#ifndef DISABLE_OLD_COLOR
//#define   DCM_NXT_COLOR_TIMEOUT         500                       // [mS]
//#define   DCM_NXT_COLOR_INIT_DELAY      100                       // [mS]
//#define   DCM_NXT_COLOR_HIGH_TIME       20                        // [mS]
//#endif

//#define   INPUTADCPORTS   12
//#define   INPUTADCPOWERS  4
//#define   INPUTADC        (INPUTADCPORTS + INPUTADCPOWERS)

#define   NO_OF_INPUT_PORTS             INPUTS
//#define   NO_OF_OUTPUT_PORTS            OUTPUTS

#define   VtoC(V)               ((UWORD)((V * ADC_RES) / ADC_REF))
#define   CtoV(C)               ((UWORD)((C * ADC_REF) / ADC_RES))
#define   MtoV(M)               ((UWORD)((M * ADC_REF * 100) / (ADC_RES * 52)))

#define   KB                    1024

//#define   PINRead(port,pin)             InputPortPin[port][pin]

#define STEP_SIZE_TABLE_ENTRIES 89
#define INDEX_TABLE_ENTRIES     16

typedef   struct
{
  DATA8   Status;                       //!< Status
}
SOUND;
													
#define SOUND_ADPCM_INIT_VALPREV  0x7F
#define SOUND_ADPCM_INIT_INDEX    20

enum      InputPortPins
{
  INPUT_PORT_PIN1,
  INPUT_PORT_PIN2,
  INPUT_PORT_PIN5,
  INPUT_PORT_PIN6,
  INPUT_PORT_BUF,
  INPUT_PORT_PINS,
  INPUT_PORT_VALUE
};

// InputPortPin[NO_OF_INPUT_PORTS][4]=
//	{
//		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2),0,0,0,
//		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1),0,0,0,
//		0,0,0,0,
//		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3),0,0,0
//	};

enum      DCM_STATE
{
  DCM_INIT,
  DCM_FLOATING_DELAY,
  DCM_FLOATING,
  DCM_WAITING_FOR_PIN5_LOW,
  DCM_WAITING_FOR_PIN6_LOW,
  DCM_CONNECTION,
  DCM_PIN2_LOW,
  DCM_NXT_TOUCH_CHECK,
//#ifndef DISABLE_OLD_COLOR
//  DCM_NXT_COLOR_INIT,
//  DCM_NXT_COLOR_WAIT,
//  DCM_NXT_COLOR_START,
//  DCM_NXT_COLOR_BUSY,
//#endif
  DCM_CONNECTED_WAITING_FOR_PIN2_HIGH,
  DCM_PIN1_LOADED,
  DCM_CONNECTED_WAITING_FOR_PIN1_TO_FLOAT,
  DCM_CONNECTED_WAITING_TOUCH,
  DCM_PIN6_HIGH,
  DCM_CONNECTED_WAITING_FOR_PIN6_LOW,
  DCM_PIN5_LOW,
  DCM_CONNECTED_WAITING_FOR_PIN5_HIGH,
  DCM_CONNECTED_WAITING_FOR_PORT_OPEN,
  DCM_STATES
};

typedef struct
{
  UWORD   Value;
  UBYTE   Connected;
  UBYTE   Cmd;
  UBYTE   State;
  UBYTE   OldState;
  UBYTE   Event;
  UBYTE   Timer;
}
INPORT;

void PortDetect(void);
void Device3TimerInterrupt1(void);
UBYTE PortPinDigital(UBYTE Port, UBYTE Pin);
u16 PortPinAdc(UBYTE Port, UBYTE Pin);
UBYTE cSoundGetAdPcmValue(UBYTE Delta);
#endif 

/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/lego_include.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __LEGO_INCLUDE_H
#define __LEGO_INCLUDE_H

//        BASIC DATA TYPES

typedef   unsigned char         UBYTE;  //!< Basic Type used to symbolise 8  bit unsigned values
typedef   unsigned short        UWORD;  //!< Basic Type used to symbolise 16 bit unsigned values
typedef   unsigned int          ULONG;  //!< Basic Type used to symbolise 32 bit unsigned values

typedef   signed char           SBYTE;  //!< Basic Type used to symbolise 8  bit signed values
typedef   signed short          SWORD;  //!< Basic Type used to symbolise 16 bit signed values
typedef   signed int            SLONG;  //!< Basic Type used to symbolise 32 bit signed values

typedef   float                 FLOAT;  //!< Basic Type used to symbolise 32 bit floating point values

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned char bool;
	
//        VM DATA TYPES

typedef   SBYTE                 DATA8;  //!< VM Type for 1 byte signed value
typedef   SWORD                 DATA16; //!< VM Type for 2 byte signed value
typedef   SLONG                 DATA32; //!< VM Type for 4 byte signed value
typedef   FLOAT                 DATAF;  //!< VM Type for 4 byte floating point value

//        VM VARIABLE TYPES

typedef   UBYTE             VARDATA;    //!< Variable base type
typedef   UBYTE             IMGDATA;    //!< Image base type

typedef   UWORD             PRGID;      //!< Program id type

typedef   UWORD             OBJID;      //!< Object id type
typedef   IMGDATA*          IP;         //!< Instruction pointer type
typedef   VARDATA*          LP;         //!< Local variable pointer type
typedef   VARDATA*          GP;         //!< global variable pointer type

typedef   ULONG             IMINDEX;    //!< ImageData index type
typedef   ULONG             GBINDEX;    //!< GlobalBytes index type
typedef   ULONG             LBINDEX;    //!< LocalBytes index type
typedef   UWORD             TRIGGER;    //!< TriggerCount type
typedef   UBYTE             PARS;       //!< NoOfParameters type
typedef   SLONG             IMOFFS;     //!< ImageData offset type

typedef   DATA16            HANDLER;    //!< Memory list index


#define   MAX_PROGRAMS          SLOTS                 //!< Max number of programs (including UI and direct commands) running at a time
#define   MAX_BREAKPOINTS       4                     //!< Max number of breakpoints (opCODES depends on this value)
#define   MAX_LABELS            32                    //!< Max number of labels per program
#define   MAX_DEVICE_TYPE       127                   //!< Highest type number (positive)
#define   MAX_VALID_TYPE        127  //vmMAX_VALID_TYPE    //!< Highest valid type
#define   MAX_DEVICE_MODES      8                     //!< Max number of modes in one device
#define   MAX_DEVICE_DATASETS   8                     //!< Max number of data sets in one device
#define   MAX_DEVICE_DATALENGTH 32                    //!< Max device data length

#define   MAX_DEVICE_BUSY_TIME  1200                  //!< Max number of mS a device can be busy when read

#define   MAX_DEVICE_TYPES      ((MAX_DEVICE_TYPE + 1) * MAX_DEVICE_MODES)//!< Max number of different device types and modes (max type data list size)

#define   MAX_FRAMES_PER_SEC    10                    //!< Max frames per second update in display

#define   CACHE_DEEPT           10                    //!< Max number of programs cached (in RECENT FILES MENU)
#define   MAX_HANDLES           250                   //!< Max number of handles to memory pools and arrays in one program

#define   MAX_ARRAY_SIZE        1000000000            //!< Max array size
#define   MIN_ARRAY_ELEMENTS    0                     //!< Min elements in a DATA8 array

#define   INSTALLED_MEMORY      6000                  //!< Flash allocated to hold user programs/data
#define   RESERVED_MEMORY       100                   //!< Memory reserve for system [KB]
#define   LOW_MEMORY            500                   //!< Low memory warning [KB]

#define   LOGBUFFER_SIZE        1000                  //!< Min log buffer size
#define   DEVICE_LOGBUF_SIZE    300                   //!< Device log buffer size (black layer buffer)
#define   MIN_LIVE_UPDATE_TIME  10                    //!< [mS] Min sample time when live update

#define   MIN_IIC_REPEAT_TIME   10                    //!< [mS] Min IIC device repeat time
#define   MAX_IIC_REPEAT_TIME   1000                  //!< [mS] Max IIC device repeat time

#define   MAX_COMMAND_BYTECODES 64                    //!< Max number of byte codes in a debug terminal direct command
#define   MAX_COMMAND_LOCALS    64                    //!< Max number of bytes allocated for direct command local variables
#define   MAX_COMMAND_GLOBALS   1021                  //!< Max number of bytes allocated for direct command global variables

#define   UI_PRIORITY           20                    //!< UI byte codes before switching VM thread
#define   C_PRIORITY            200                   //!< C call byte codes


#define   UART_DATA_LENGTH        MAX_DEVICE_DATALENGTH
#define   UART_BUFFER_SIZE        64

#define     TYPE_NAME_LENGTH    11
#define     SYMBOL_LENGTH       4       //!< Symbol leng th (not including zero)

#define   INPUTS									 4		
#define   OUTPUTS									 4

#define   ADC_REF               5000                  //!< [mV]  maximal value on ADC
#define   ADC_RES               4095                  //!< [CNT] maximal count on ADC

#define   IN1_ID_HYSTERESIS     50                    //!< [mV]  half of the span one Id takes up on input connection 1 voltage
#define   OUT5_ID_HYSTERESIS    100                   //!< [mV]  half of the span one Id takes up on output connection 5 voltage

#define   IIC_DATA_LENGTH         MAX_DEVICE_DATALENGTH
#define   IIC_NAME_LENGTH         8

#define   IIC_PORT_CHANGED       0x01         //!< Input port changed
#define   IIC_DATA_READY         0x08         //!< Data is ready
#define   IIC_WRITE_REQUEST      0x10         //!< Write request

//typedef   struct
//{
//  ULONG   Calibration[CALPOINTS][COLORS];
//  UWORD   CalLimits[CALPOINTS - 1];
//  UWORD   Crc;
//  UWORD   ADRaw[COLORS];
//  UWORD   SensorRaw[COLORS];
//}
//COLORSTRUCT;

typedef   enum
{
//  TYPE_KEEP                     =   0,  //!< Type value that won't change type in byte codes
  TYPE_NXT_TOUCH                =   1,  //!< Device is NXT touch sensor
  TYPE_NXT_LIGHT                =   2,  //!< Device is NXT light sensor
  TYPE_NXT_SOUND                =   3,  //!< Device is NXT sound sensor
  TYPE_NXT_COLOR                =   4,  //!< Device is NXT color sensor

  TYPE_TACHO                    =   7,  //!< Device is a tacho motor
  TYPE_MINITACHO                =   8,  //!< Device is a mini tacho motor
  TYPE_NEWTACHO                 =   9,  //!< Device is a new tacho motor

  TYPE_THIRD_PARTY_START        =  50,
  TYPE_THIRD_PARTY_END          =  99,

  TYPE_IIC_UNKNOWN              = 100,

  TYPE_NXT_TEST                 = 101,  //!< Device is a NXT ADC test sensor

  TYPE_NXT_IIC                  = 123,  //!< Device is NXT IIC sensor
  TYPE_TERMINAL                 = 124,  //!< Port is connected to a terminal
  TYPE_UNKNOWN                  = 125,  //!< Port not empty but type has not been determined
  TYPE_NONE                     = 126,  //!< Port empty or not available
  TYPE_ERROR                    = 127,  //!< Port not empty and type is invalid
}
TYPE;

typedef   enum
{
  CONN_UNKNOWN                  = 111,  //!< Connection is fake (test)

  CONN_DAISYCHAIN               = 117,  //!< Connection is daisy chained
  CONN_NXT_COLOR                = 118,  //!< Connection type is NXT color sensor
  CONN_NXT_DUMB                 = 119,  //!< Connection type is NXT analog sensor
  CONN_NXT_IIC                  = 120,  //!< Connection type is NXT IIC sensor

  CONN_INPUT_DUMB               = 121,  //!< Connection type is LMS2012 input device with ID resistor
  CONN_INPUT_UART               = 122,  //!< Connection type is LMS2012 UART sensor

  CONN_OUTPUT_DUMB              = 123,  //!< Connection type is LMS2012 output device with ID resistor
  CONN_OUTPUT_INTELLIGENT       = 124,  //!< Connection type is LMS2012 output device with communication
  CONN_OUTPUT_TACHO             = 125,  //!< Connection type is LMS2012 tacho motor with series ID resistance

  CONN_NONE                     = 126,  //!< Port empty or not available
  CONN_ERROR                    = 127,  //!< Port not empty and type is invalid
}
CONN;

typedef   enum
{
  OK            = 0,                    //!< No errors to report
  BUSY          = 1,                    //!< Busy - try again
  FAIL          = 2,                    //!< Something failed
  STOP          = 4                     //!< Stopped
}
RESULT;

typedef   struct // if data type changes - remember to change "cInputTypeDataInit" !
{
  SBYTE     Name[TYPE_NAME_LENGTH + 1]; //!< Device name
  DATA8     Type;                       //!< Device type
  DATA8     Connection;
  DATA8     Mode;                       //!< Device mode
  DATA8     DataSets;
  DATA8     Format;
  DATA8     Figures;
  DATA8     Decimals;
  DATA8     Views;
  DATAF     RawMin;                     //!< Raw minimum value      (e.c. 0.0)
  DATAF     RawMax;                     //!< Raw maximum value      (e.c. 1023.0)
  DATAF     PctMin;                     //!< Percent minimum value  (e.c. -100.0)
  DATAF     PctMax;                     //!< Percent maximum value  (e.c. 100.0)
  DATAF     SiMin;                      //!< SI unit minimum value  (e.c. -100.0)
  DATAF     SiMax;                      //!< SI unit maximum value  (e.c. 100.0)
  UWORD     InvalidTime;                //!< mS from type change to valid data
  UWORD     IdValue;                    //!< Device id value        (e.c. 0 ~ UART)
  DATA8     Pins;                       //!< Device pin setup
  SBYTE     Symbol[SYMBOL_LENGTH + 1];  //!< SI unit symbol
  UWORD     Align;
}
TYPES;

typedef   struct
{
  TYPES   TypeData[INPUTS][MAX_DEVICE_MODES]; //!< TypeData    

#ifndef DISABLE_FAST_DATALOG_BUFFER
  UWORD   Repeat[INPUTS][DEVICE_LOGBUF_SIZE];
  DATA8   Raw[INPUTS][DEVICE_LOGBUF_SIZE][UART_DATA_LENGTH];      //!< Raw value from UART device
  UWORD   Actual[INPUTS];
  UWORD   LogIn[INPUTS];
#else
  DATA8   Raw[INPUTS][UART_DATA_LENGTH];      //!< Raw value from UART device
#endif
  DATA8   Status[INPUTS];                     //!< Status
  DATA8   Output[INPUTS][UART_DATA_LENGTH];   //!< Bytes to UART device
  DATA8   OutputLength[INPUTS];
}
UART;

typedef   struct
{
  TYPES   TypeData[INPUTS][MAX_DEVICE_MODES]; //!< TypeData

#ifndef DISABLE_FAST_DATALOG_BUFFER
  UWORD   Repeat[INPUTS][DEVICE_LOGBUF_SIZE];
  DATA8   Raw[INPUTS][DEVICE_LOGBUF_SIZE][IIC_DATA_LENGTH];      //!< Raw value from IIC device
  UWORD   Actual[INPUTS];
  UWORD   LogIn[INPUTS];
#else
  DATA8   Raw[INPUTS][IIC_DATA_LENGTH];      //!< Raw value from IIC device
#endif
  DATA8   Status[INPUTS];                     //!< Status
  DATA8   Changed[INPUTS];
  DATA8   Output[INPUTS][IIC_DATA_LENGTH];    //!< Bytes to IIC device
  DATA8   OutputLength[INPUTS];
}
IIC;

typedef   struct
{
  DATA16  InPin1[INPUTS];         //!< Analog value at input port connection 1
  DATA16  InPin6[INPUTS];         //!< Analog value at input port connection 6
  DATA16  OutPin5[OUTPUTS];       //!< Analog value at output port connection 5
  DATA16  BatteryTemp;            //!< Battery temperature
  DATA16  MotorCurrent;           //!< Current flowing to motors
  DATA16  BatteryCurrent;         //!< Current flowing from the battery
  DATA16  Cell123456;             //!< Voltage at battery cell 1, 2, 3,4, 5, and 6
#ifndef DISABLE_FAST_DATALOG_BUFFER
  DATA16  Pin1[INPUTS][DEVICE_LOGBUF_SIZE];      //!< Raw value from analog device
  DATA16  Pin6[INPUTS][DEVICE_LOGBUF_SIZE];      //!< Raw value from analog device
  UWORD   Actual[INPUTS];
  UWORD   LogIn[INPUTS];
  UWORD   LogOut[INPUTS];
#endif
//#ifndef   DISABLE_OLD_COLOR
//  COLORSTRUCT  NxtCol[INPUTS];
//#endif
  DATA16  OutPin5Low[OUTPUTS];    //!< Analog value at output port connection 5 when connection 6 is low

  DATA8   Updated[INPUTS];

  DATA8   InDcm[INPUTS];          //!< Input port device types
  DATA8   InConn[INPUTS];

  DATA8   OutDcm[OUTPUTS];        //!< Output port device types
  DATA8   OutConn[OUTPUTS];
#ifndef DISABLE_PREEMPTED_VM
  UWORD   PreemptMilliSeconds;
#endif
}
ANALOG;

#endif

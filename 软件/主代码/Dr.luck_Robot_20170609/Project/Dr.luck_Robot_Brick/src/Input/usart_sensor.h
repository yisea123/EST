/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/usart_sensor.h 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   
  ******************************************************************************
**/
#ifndef __USART_SENSOR_H
#define __USART_SENSOR_H

#include "sys.h"
#include "lego_include.h"

#define   INPUTS                        	 4

#define   Uart1 													 0
#define   Uart2 													 1
#define   Uart3 													 2
#define   Uart4 													 3

//#define   UART_BUFFER_SIZE        				64

#define   LOWEST_BITRATE                2400				//  Lowest possible bit rate (always used for sync and info)  [b/S]
#define   MIDLE_BITRATE                57600  			//  Highest bit rate allowed when adjusting clock             [b/S]
#define   HIGHEST_BITRATE             460800  			//  Highest possible bit rate                                 [b/S]

#define   MIDLE_BITRATE_DEVIATION         40  			//  Largest deviation able to adjust clock to                 [%]
#define   MIDLE_BITRATE_INCREAMENT         1  			//  Amount to adjust per loop                                 [%]
#define   MIDLE_BITRATE_FIX                5  			//  Last adjustment to get in the middle of the bit           [%]

#define   UART_TIMER_RESOLUTION         	10        // [100uS]

#define   UART_BREAK_TIME               1000        // [100uS]
#define   UART_TERMINAL_DELAY          20000        // [100uS]
#define   UART_CHANGE_BITRATE_DELAY      100        // [100uS]
#define   UART_ACK_DELAY                 100        // [100uS]
#define   UART_SHOW_TIME                2500        // [100uS]

#define   UART_WATCHDOG_TIME            1000        // [100uS]

#define   UART_ALLOWABLE_DATA_ERRORS    	 6

//first Byte

#define   BYTE_SYNC                     0x00        // Synchronisation byte
#define   BYTE_ACK                      0x04        // Acknowledge byte
#define   BYTE_NACK                     0x02        // Not acknowledge byte

#define   MESSAGE_SYS                   0x00        // System message
#define   MESSAGE_CMD                   0x40        // Command message
#define   MESSAGE_INFO                  0x80        // Info message
#define   MESSAGE_DATA                  0xC0        // Data message
#define   GET_MESSAGE_TYPE(B)     (B & 0xC0)        // Get message type

#define   CMD_TYPE                      0x00        // CMD command - TYPE     (device type for VM reference)
#define   CMD_MODES                     0x01        // CMD command - MODES    (number of supported modes 0=1)
#define   CMD_SPEED                     0x02        // CMD command - SPEED    (maximun communication speed)
#define   CMD_SELECT                    0x03        // CMD command - SELECT   (select mode)
#define   CMD_WRITE                     0x04        // CMD command - WRITE    (write to device)
#define   GET_CMD_COMMAND(B)       (B & 0x07)        // Get CMD command

#define   GET_MODE(B)             (B & 0x07)        // Get mode

#define   CONVERT_LENGTH(C)             (1 << (C & 0x07))
#define   GET_MESSAGE_LENGTH(B)         (CONVERT_LENGTH(B >> 3))        // Get message length exclusive check byte

#define   MAKE_CMD_COMMAND(C,LC)        (MESSAGE_CMD + (C & 0x07) + ((LC & 0x07) << 3))

// SECOND INFO BYTE

#define   INFO_NAME                     0x00        // INFO command - NAME    (device name)
#define   INFO_RAW                      0x01        // INFO command - RAW     (device RAW value span)
#define   INFO_PCT                      0x02        // INFO command - PCT     (device PCT value span)
#define   INFO_SI                       0x03        // INFO command - SI      (device SI  value span)
#define   INFO_SYMBOL                   0x04        // INFO command - SYMBOL  (device SI  unit symbol)
#define   INFO_FORMAT                   0x80        // INFO command - FORMAT  (device data sets and format)
#define   GET_INFO_COMMAND(B)           (B)         // Get INFO command

#endif

/**************************Uart1**************************/

#define   UART_CLOCK1   				132000000
#define   UART_CLOCK2   				150000000
// \todo UART clock 1 and 2 are different?

#define   UART_RBR                      0
#define   UART_THR                      0
#define   UART_IER                      1
#define   UART_IIR                      2
#define   UART_FCR                      2
#define   UART_LCR                      3
#define   UART_MCR                      4
#define   UART_LSR                      5
#define   UART_MSR                      6
#define   UART_SCR                      7
#define   UART_DLL                      8
#define   UART_DLH                      9
#define   UART_REVID1                   10
#define   UART_REVID2                   11
#define   UART_PWREMU_MGMT              12
#define   UART_MDR                      13

#define   UART_RECBUF_SIZE              64
#define   UART_TXBUF_SIZE              64

enum UART_STATE
{
  UART_IDLE,							//00
  UART_INIT,							//01
  UART_RESTART,						//02
  UART_ENABLE,						//03
  UART_FLUSH,							//04
  UART_SYNC,							//05
  UART_MESSAGE_START,			//06
  UART_CMD,								//07
  UART_INFO,							//08
  UART_DATA,							//09
  UART_DATA_COPY,					//0A
  UART_ACK_WAIT,					//0B
  UART_ACK_INFO,					//0C
  UART_CMD_ERROR,					//0D
  UART_INFO_ERROR,				//0E
  UART_TERMINAL,					//0F
  UART_DATA_ERROR,				//10
  UART_ERROR,							//11
  UART_EXIT,							//12
  UART_STATES							//13
};

#define   UART_PORT_CHANGED       0x01        //!< Input port changed
#define   UART_DATA_READY         0x08        //!< Data is ready
#define   UART_WRITE_REQUEST      0x10        //!< Write request


// DEVICE1 ********************************************************************


#define   INFODATA_INIT                 0x00000000L
#define   INFODATA_CMD_TYPE             0x00000001L
#define   INFODATA_CMD_MODES            0x00000002L
#define   INFODATA_CMD_SPEED            0x00000004L

#define   INFODATA_INFO_NAME            0x00000100L
#define   INFODATA_INFO_RAW             0x00000200L
#define   INFODATA_INFO_PCT             0x00000400L
#define   INFODATA_INFO_SI              0x00000800L
#define   INFODATA_INFO_SYMBOL          0x00001000L
#define   INFODATA_INFO_FORMAT          0x00002000L

#define   INFODATA_CLEAR                (~(INFODATA_INFO_NAME | INFODATA_INFO_RAW | INFODATA_INFO_PCT | INFODATA_INFO_SI | INFODATA_INFO_SYMBOL | INFODATA_INFO_FORMAT))

#define   INFODATA_NEEDED               (INFODATA_CMD_TYPE | INFODATA_CMD_MODES | INFODATA_INFO_NAME | INFODATA_INFO_FORMAT)

//volatile  ULONG *Uart1Base;
static    char  Uart1Name[20];


extern    UBYTE Uart1RecMes[];
extern    UBYTE Uart2RecMes[];
extern    UBYTE Uart3RecMes[];
extern    UBYTE Uart4RecMes[];

static    UART UartDefault;
static    UART *pUart = &UartDefault;

//static    struct hrtimer Device1Timer;
//static    ktime_t        Device1Time;

static    UBYTE TestMode = 0;

typedef	 struct
{
  ULONG   InfoData;
  ULONG   BitRate;
  ULONG   BitRateMax;
  UWORD   Timer;
  UWORD   WatchDog;
  UWORD   BreakTimer;
  UBYTE   Initialised;
  UBYTE   ChangeMode;
	UBYTE   State;
  UBYTE   OldState;
  UBYTE   SubState;
  UBYTE   Cmd;
  UBYTE   InfoCmd;
  UBYTE   Check;
  UBYTE   Types;
  UBYTE   Views;
  UBYTE   Mode;
  UBYTE   Type;
  UBYTE   DataOk;
  UBYTE   DataErrors;
  SBYTE   Name[TYPE_NAME_LENGTH + 1];
  UBYTE   InLength;
  UBYTE   InPointer;
  UBYTE   OutLength;
  UBYTE   OutPointer;
  UBYTE   InBuffer[UART_BUFFER_SIZE];
  UBYTE   OutBuffer[UART_BUFFER_SIZE];
	
	UBYTE   channel;
}UARTPORT;

extern UARTPORT  UartPort[INPUTS];
extern UBYTE  UartPortFlag[INPUTS];
extern UBYTE UsartBreakTimer[];
extern TYPES TypeData[INPUTS][MAX_DEVICE_MODES];
//-----------------º¯Êý--------------

static UBYTE UartRead(UBYTE Port,UBYTE *pByte);
static UBYTE UartPortReadData(UBYTE Port,UBYTE *pCmd,UBYTE *pData,UBYTE *pCheck,UBYTE *pFail);

static UBYTE UartPortSend(UBYTE Port, UBYTE ack);
void UsartChange(UBYTE Port, u32 bound);

extern void ChangeMode(UBYTE Port, UBYTE mode);

static UBYTE Uart1Read(UBYTE *pByte);
static UBYTE Uart1ReadData(UBYTE *pCmd,UBYTE *pData,UBYTE *pCheck,UBYTE *pFail);
static UBYTE Uart2Read(UBYTE *pByte);
static UBYTE Uart2ReadData(UBYTE *pCmd,UBYTE *pData,UBYTE *pCheck,UBYTE *pFail);
static UBYTE Uart3Read(UBYTE *pByte);
static UBYTE Uart3ReadData(UBYTE *pCmd,UBYTE *pData,UBYTE *pCheck,UBYTE *pFail);
static UBYTE Uart4Read(UBYTE *pByte);
static UBYTE Uart4ReadData(UBYTE *pCmd,UBYTE *pData,UBYTE *pCheck,UBYTE *pFail);

void UartDevice1TimerInterrupt1(void);

void Myprintf(UBYTE Port);

/**
  ******************************************************************************
  * @file    usbd_hid_core.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   This file provides the HID core functions.
  *
  * @verbatim
  *      
  *          ===================================================================      
  *                                HID Class  Description
  *          =================================================================== 
  *           This module manages the HID class V1.11 following the "Device Class Definition
  *           for Human Interface Devices (HID) Version 1.11 Jun 27, 2001".
  *           This driver implements the following aspects of the specification:
  *             - The Boot Interface Subclass
  *             - The Mouse protocol
  *             - Usage Page : Generic Desktop
  *             - Usage : Joystick)
  *             - Collection : Application 
  *      
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *           
  *      
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_hid_core.h"
#include "usbd_desc.h"
#include "usbd_req.h"
#include "stdio.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_HID 
  * @brief usbd core module
  * @{
  */ 

/** @defgroup USBD_HID_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_HID_Private_Defines
  * @{
  */ 

/**
  * @}
  */ 
__ALIGN_BEGIN uint8_t USB_HID_Rx_Buffer   [HID_DATA_OUT_PACKET_SIZE] __ALIGN_END ;

__ALIGN_BEGIN uint8_t CmdBuff1[8] __ALIGN_END ;
//static uint32_t hidCmd = 0xFF;
//static uint32_t hidLen = 0;


/** @defgroup USBD_HID_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 




/** @defgroup USBD_HID_Private_FunctionPrototypes
  * @{
  */


static uint8_t  USBD_HID_Init (void  *pdev, 
                               uint8_t cfgidx);

static uint8_t  USBD_HID_DeInit (void  *pdev, 
                                 uint8_t cfgidx);

static uint8_t  USBD_HID_Setup (void  *pdev, 
                                USB_SETUP_REQ *req);

static uint8_t  *USBD_HID_GetCfgDesc (uint8_t speed, uint16_t *length);

static uint8_t  USBD_HID_DataIn (void  *pdev, uint8_t epnum);

static uint8_t  USBD_HID_DataOut (void  *pdev, uint8_t epnum);
/**
  * @}
  */ 

/** @defgroup USBD_HID_Private_Variables
  * @{
  */ 

USBD_Class_cb_TypeDef  USBD_HID_cb = 
{
  USBD_HID_Init,
  USBD_HID_DeInit,
  USBD_HID_Setup,
  NULL, /*EP0_TxSent*/  
  NULL, /*EP0_RxReady*/
  USBD_HID_DataIn, /*DataIn*/
  USBD_HID_DataOut, /*DataOut*/
  NULL, /*SOF */
  NULL,
  NULL,      
  USBD_HID_GetCfgDesc,
//#ifdef USB_OTG_HS_CORE  
//  USBD_HID_GetCfgDesc, /* use same config as per FS */
//#endif  
};

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */        
__ALIGN_BEGIN static uint32_t  USBD_HID_AltSet  __ALIGN_END = 0;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */      
__ALIGN_BEGIN static uint32_t  USBD_HID_Protocol  __ALIGN_END = 0;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */  
__ALIGN_BEGIN static uint32_t  USBD_HID_IdleState __ALIGN_END = 0;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */ 
/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_CfgDesc[USB_HID_CONFIG_DESC_SIZ] __ALIGN_END =
{
  0x09, /* bLength: Configuration Descriptor size */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
  USB_HID_CONFIG_DESC_SIZ,
  /* wTotalLength: Bytes returned */
  0x00,
  0x01,         /*bNumInterfaces: 1 interface*/
  0x01,         /*bConfigurationValue: Configuration value*/
  0x00,         /*iConfiguration: Index of string descriptor describing
  the configuration*/
  0xE0,         /*bmAttributes: bus powered and Support Remote Wake-up */
  0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/
  
  /************** Descriptor of Joystick Mouse interface ****************/
  /* 09 */
  0x09,         /*bLength: Interface Descriptor size*/
  USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
  0x00,         /*bInterfaceNumber: Number of Interface*/
  0x00,         /*bAlternateSetting: Alternate setting*/
  0x02,         /*bNumEndpoints*/
  0x03,         /*bInterfaceClass: HID*/
  0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/  //1代表需要引导
  0x02,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
  0,            /*iInterface: Index of string descriptor*/
  /******************** Descriptor of Joystick Mouse HID ********************/
  /* 18 */
  0x09,         /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*///0x11!!!
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  HID_MOUSE_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
  /******************** Descriptor of Mouse endpoint ********************/
  /* 27 */
  0x07,          /*bLength: Endpoint Descriptor size*/
  USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/
  
  HID_IN_EP,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,          /*bmAttributes: Interrupt endpoint*/
  HID_IN_PACKET, /*wMaxPacketSize: 4 Byte max */
  0x00,
  0x00,          /*bInterval: Polling Interval (10 ms)*/
  /* 34 */
	0x07,          /*bLength: Endpoint Descriptor size*/
  USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/
  
  HID_OUT_EP,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,          /*bmAttributes: Interrupt endpoint*/
  HID_OUT_PACKET, /*wMaxPacketSize: 4 Byte max */
  0x00,
  0x00,          /*bInterval: Polling Interval (10 ms)*/
  /*41*/
} ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_Desc[USB_HID_DESC_SIZ] __ALIGN_END=
{
  /* 18 */
  0x09,         /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  HID_MOUSE_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/	//即HID_MOUSE_ReportDesc长度
  0x00,
};
#endif 


#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */  
__ALIGN_BEGIN static uint8_t HID_MOUSE_ReportDesc[HID_MOUSE_REPORT_DESC_SIZE] __ALIGN_END =
{
//	//74
//  0x05,   0x01,
//  0x09,   0x02,
//  0xA1,   0x01,
//  0x09,   0x01,
//  
//  0xA1,   0x00,
//  0x05,   0x09,
//  0x19,   0x01,
//  0x29,   0x03,
//  
//  0x15,   0x00,
//  0x25,   0x01,
//  0x95,   0x03,
//  0x75,   0x01,
//  
//  0x81,   0x02,		//input
//  0x95,   0x01,
//  0x75,   0x05,
//  0x81,   0x01,
//  
//  0x05,   0x01,
//  0x09,   0x30,
//  0x09,   0x31,
//  0x09,   0x38,
//  
//  0x15,   0x81,
//  0x25,   0x7F,
//  0x75,   0x08,
//  0x95,   0x03,
//  
//  0x81,   0x06,
//  0xC0,   0x09,
//  0x3c,   0x05,
//  0xff,   0x09,
//  
//  0x01,   0x15,
//  0x00,   0x25,
//  0x01,   0x75,
//  0x01,   0x95,
//  
//  0x02,   0xb1,
//  0x22,   0x75,
//  0x06,   0x95,
//  0x01,   0xb1,
//  
//  0x01,   0xc0

	//33byte
	0x05, 0x8c, /* USAGE_PAGE (ST Page) */ 
	0x09, 0x01, /* USAGE (Demo Kit) */ 
	0xa1, 0x01, /* COLLECTION (Application) */ 
	
	// The Input report 
	0x09,0x03, // USAGE ID - Vendor defined 
	0x15,0x00, // LOGICAL_MINIMUM (0) 
	0x26,0x00,0xFF, // LOGICAL_MAXIMUM (255) 
	0x75,0x08, // REPORT_SIZE (8bit) 
	0x95,0x40, // REPORT_COUNT (64Byte) 
	0x81,0x02, // INPUT (Data,Var,Abs) 

	// The Output report 
	0x09,0x04, // USAGE ID - Vendor defined 
	0x15,0x00, // LOGICAL_MINIMUM (0) 
	0x26,0x00,0xFF, // LOGICAL_MAXIMUM (255) 
	0x75,0x08, // REPORT_SIZE (8bit) 
	0x95,0x40, // REPORT_COUNT (64Byte) 
	0x91,0x02, // OUTPUT (Data,Var,Abs) 

	0xc0 /* END_COLLECTION */ 
}; 

/**
  * @}
  */ 

/** @defgroup USBD_HID_Private_Functions
  * @{
  */ 

/**
  * @brief  USBD_HID_Init
  *         Initialize the HID interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_HID_Init (void  *pdev, 
                               uint8_t cfgidx)
{
  uint8_t *pbuf;
	
  /* Open EP IN */
  DCD_EP_Open(pdev,
              HID_IN_EP,
              HID_IN_PACKET,
              USB_OTG_EP_INT);
  
  /* Open EP OUT */ 
  DCD_EP_Open(pdev,
              HID_OUT_EP,
              HID_OUT_PACKET,
              USB_OTG_EP_INT);
	
	pbuf = (uint8_t *)USBD_DeviceDesc; //指向设备描述符
  pbuf[4] = 0x00;	//更改为接口描述符提供的类
  pbuf[5] = 0x00;
	
//	pbuf[10] = ((uint8_t)(0x5750 & 0x00FF));
//	pbuf[11] = ((uint8_t)((0x5750 & 0xFF00) >>8));
	
  /* Prepare Out endpoint to receive next packet */
  DCD_EP_PrepareRx(pdev,
                   HID_OUT_EP,
                   (uint8_t*)(USB_HID_Rx_Buffer),
                   HID_DATA_OUT_PACKET_SIZE);
  
  
  return USBD_OK;
}

/**
  * @brief  USBD_HID_Init
  *         DeInitialize the HID layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_HID_DeInit (void  *pdev, 
                                 uint8_t cfgidx)
{
  /* Close HID EPs */
  DCD_EP_Close (pdev , HID_IN_EP);
  DCD_EP_Close (pdev , HID_OUT_EP);
  
  
  return USBD_OK;
}

/**
  * @brief  USBD_HID_Setup
  *         Handle the HID specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_HID_Setup (void  *pdev, 
                                USB_SETUP_REQ *req)
{
//  uint16_t len = USB_HID_DESC_SIZ;
//  uint8_t  *pbuf = USBD_HID_CfgDesc+9;
	uint16_t len = 0;
	uint8_t  *pbuf = NULL;
  
  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS : 	
		//---------------------------------------
    switch (req->bRequest)
    {
    case HID_REQ_SET_PROTOCOL:
      USBD_HID_Protocol = (uint8_t)(req->wValue);
      break;
      
    case HID_REQ_GET_PROTOCOL:
      USBD_CtlSendData (pdev, 
                        (uint8_t *)&USBD_HID_Protocol,
                        1);    
      break;
      
    case HID_REQ_SET_IDLE:
      USBD_HID_IdleState = (uint8_t)(req->wValue >> 8);
      break;
      
    case HID_REQ_GET_IDLE:
      USBD_CtlSendData (pdev, 
                        (uint8_t *)&USBD_HID_IdleState,
                        1);        
      break;    
		case HID_REQ_SET_REPORT:
			printf("HID_REQ_SET_REPORT\n");
			printf("接收数据长度:%d\n",req->wLength);
			
			
			//req->wLength
			
//			printf("buff:%d\n",req->ReqBuff[0]);
//			printf("buff:%d\n",req->ReqBuff[1]);
//			printf("buff:%d\n",req->ReqBuff[2]);
//			printf("buff:%d\n",req->ReqBuff[3]);
//			printf("buff:%d\n",req->ReqBuff[4]);
//			printf("buff:%d\n",req->ReqBuff[5]);
			USBD_CtlError (pdev, req);
			break;
		case HID_REQ_GET_REPORT:
			printf("HID_REQ_GET_REPORT\n");
			USBD_CtlError (pdev, req);
			break;
      
    default:
      USBD_CtlError (pdev, req);
      return USBD_FAIL; 
    }
    break;
		
  /* Standard Requests ---------------------*/
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR: 
      if( req->wValue >> 8 == HID_REPORT_DESC)
      {
        len = MIN(HID_MOUSE_REPORT_DESC_SIZE , req->wLength);
        pbuf = HID_MOUSE_ReportDesc;
      }
      else if( req->wValue >> 8 == HID_DESCRIPTOR_TYPE)
      {
        
//#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
//        pbuf = USBD_HID_Desc;   
//#else
        pbuf = USBD_HID_CfgDesc + 0x12;
//#endif 
        len = MIN(USB_HID_DESC_SIZ , req->wLength);
      }
      
      USBD_CtlSendData (pdev, 
                        pbuf,
                        len);
      
      break;
      
    case USB_REQ_GET_INTERFACE :
      USBD_CtlSendData (pdev,
                        (uint8_t *)&USBD_HID_AltSet,
                        1);
      break;
      
    case USB_REQ_SET_INTERFACE :
      USBD_HID_AltSet = (uint8_t)(req->wValue);
      break;
    }
  }
  return USBD_OK;
}

/**
  * @brief  USBD_HID_SendReport 
  *         Send HID Report
  * @param  pdev: device instance
  * @param  buff: pointer to report
  * @retval status
  */
uint8_t USBD_HID_SendReport     (USB_OTG_CORE_HANDLE  *pdev, 
                                 uint8_t *report,
                                 uint16_t len)
{
  if (pdev->dev.device_status == USB_OTG_CONFIGURED )
  {
    DCD_EP_Tx (pdev, HID_IN_EP, report, len);
  }
  return USBD_OK;
}




/**
  * @brief  USBD_HID_GetCfgDesc 
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_HID_GetCfgDesc (uint8_t speed, uint16_t *length)
{
  *length = sizeof (USBD_HID_CfgDesc);
  return USBD_HID_CfgDesc;
}

/**
  * @brief  USBD_HID_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_HID_DataIn (void  *pdev, 
                              uint8_t epnum)
{
  
  /* Ensure that the FIFO is empty before a new transfer, this condition could 
  be caused by  a new transfer before the end of the previous transfer */
  DCD_EP_Flush(pdev, HID_IN_EP);
	
  return USBD_OK;
}

static uint8_t  USBD_HID_DataOut (void  *pdev, 
                              uint8_t epnum)
{
	uint16_t USB_Rx_Cnt;
	static int i;										//i必须为静态变量
	
  /* Get the received data buffer and update the counter */
  USB_Rx_Cnt = ((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[1].xfer_count;
//	
//	printf("从机接收到数据%s",USB_HID_Rx_Buffer);
	
	//-----------------------HID数据帧解析，每个包限定必须64个byte--------------------------------------
	if(!Receive_start&&!USB_Rx_Length)
	{
		for(i=0; i<USB_Rx_Cnt; i++)							//寻找开始符,USB_Rx_Cnt为64
		{
			if(*(USB_HID_Rx_Buffer + i) == 0x68) 	//接收到开始符
			{
//				STM_EVAL_LEDOn(LED2);
				Receive_start = 1;
				USB_TimePic = 3000; 								//接收时间计时
				break;															//跳转后i不会自加1
			}
		}
	}
	if(Receive_start)
	{
		if(!USB_Rx_Length)			//第一个数据包
			for(; i<USB_Rx_Cnt; i++) 
				USB_Rx_Buff[USB_Rx_Length++] = *(USB_HID_Rx_Buffer + i); //printf("%x ",USB_Rx_Buff[USB_Rx_Length-1]);}
		else 										//紧接下来的数据包
			for(i=0; i<USB_Rx_Cnt; i++) 
				USB_Rx_Buff[USB_Rx_Length++] = *(USB_HID_Rx_Buffer + i); //printf("%x ",USB_Rx_Buff[USB_Rx_Length-1]);}
		
		STM_EVAL_LEDOn(LED1);
//		printf("\n\r");
				
		if(USB_Rx_Length-1>4)		//进入数据域
		{
			if((USB_Rx_Length>5+USB_Rx_Buff[3]+USB_Rx_Buff[4]*256+1))  				 //避开数据域数据等于结束符问题
			{
				for(i=6+USB_Rx_Buff[3]+USB_Rx_Buff[4]*256; i<USB_Rx_Length; i++) //寻找结束符
				{
					if(*(USB_Rx_Buff + i) == 0x16) 		//接收到结束符
					{
//						STM_EVAL_LEDOff(LED2);
						
						Receive_start = 0; 					 		//本帧数据接收结束，但无权接收下一帧数据
						Receive_end = 1;
						USB_Rx_Length = i+1; 				 		//除去结束符后无关的数据
						break;
					}
				}		
			}		
		}
	}
	
	STM_EVAL_LEDOff(LED1);
	
	DCD_EP_Flush(pdev, HID_OUT_EP);	//接收数据处理
	/* Prepare Out endpoint to receive next packet */
  DCD_EP_PrepareRx(pdev,
                   HID_OUT_EP,
                   (uint8_t*)(USB_HID_Rx_Buffer),
                   HID_DATA_OUT_PACKET_SIZE);
	
	return USBD_OK;
}

/**
  * @}
  */ 
//static uint8_t  usbd_hid_SOF (void *pdev)
//{      
//  static uint32_t FrameCount = 0;
//  
//  if (FrameCount++ == CDC_IN_FRAME_INTERVAL)
//  {
//    /* Reset the frame counter */
//    FrameCount = 0;
//    
//    /* Check the data to be sent through IN pipe */
////    Handle_USBAsynchXfer(pdev);
//  }
//  
//  return USBD_OK;
//}

/**
  * @}
  */ 


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

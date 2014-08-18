/**
  ******************************************************************************
  * @file    usbd_cdc_vcp.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   Generic media access Layer.
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

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED 
#pragma     data_alignment = 4 
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_vcp.h"
#include "usb_conf.h"
#include <string.h>

//#define DEBUG_OTG	1
#ifdef DEBUG_OTG
#include "debug_printf.h"
#endif
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define VCP_RX_BUFFER_SIZE	30			//Size of ring buffer
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
LINE_CODING linecoding =
  {
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
  };


//USART_InitTypeDef USART_InitStructure;

/* These are external variables imported from CDC core to be used for IN 
   transfer management. */
extern uint8_t  APP_Rx_Buffer []; /* Write CDC received data in this buffer.
                                     These data will be sent over USB IN endpoint
                                     in the CDC core functions. */
extern uint32_t APP_Rx_ptr_in;    /* Increment this pointer or roll it back to
                                     start address when writing received data
                                     in the buffer APP_Rx_Buffer. */

static uint8_t VCP_Rx_Buffer[VCP_RX_BUFFER_SIZE];		//Ring buffer to hold received characters
static int VCP_Rx_BufferLen = 0;
static int VCP_Rx_BufferGetLen = 0;

/* Private function prototypes -----------------------------------------------*/
static uint16_t VCP_Init     (void);
static uint16_t VCP_DeInit   (void);
static uint16_t VCP_Ctrl     (uint32_t Cmd, uint8_t* Buf, uint32_t Len);
uint16_t VCP_DataTx   (uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataRx   (uint8_t* Buf, uint32_t Len);

static uint16_t VCP_COMConfig(uint8_t Conf);

CDC_IF_Prop_TypeDef VCP_fops = 
{
  VCP_Init,
  VCP_DeInit,
  VCP_Ctrl,
  VCP_DataTx,
  VCP_DataRx
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  VCP_Init
  *         Initializes the Media on the STM32
  * @param  None
  * @retval Result of the opeartion (USBD_OK in all cases)
  */
static uint16_t VCP_Init(void)
{

#ifdef DEBUG_OTG
	debug_printf("DEBUG_OTG: VCP_Init e\n\r");
#endif

  
#ifdef DEBUG_OTG
	debug_printf("DEBUG_OTG: VCP_Init r\n\r");
#endif

  return USBD_OK;
}

/**
  * @brief  VCP_DeInit
  *         DeInitializes the Media on the STM32
  * @param  None
  * @retval Result of the opeartion (USBD_OK in all cases)
  */
static uint16_t VCP_DeInit(void)
{
#ifdef DEBUG_OTG
	debug_printf("DEBUG_OTG: VCP_DeInit er\n\r");
#endif


  return USBD_OK;
}


/**
  * @brief  VCP_Ctrl
  *         Manage the CDC class requests
  * @param  Cmd: Command code            
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the opeartion (USBD_OK in all cases)
  */
static uint16_t VCP_Ctrl (uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{ 
#ifdef DEBUG_OTG
	debug_printf("DEBUG_OTG: VCP_Ctrl e\n\r");
#endif


  switch (Cmd)
  {
  case SEND_ENCAPSULATED_COMMAND:
    // Not  needed for this driver 
    break;

  case GET_ENCAPSULATED_RESPONSE:
    // Not  needed for this driver 
    break;

  case SET_COMM_FEATURE:
    // Not  needed for this driver 
    break;

  case GET_COMM_FEATURE:
    // Not  needed for this driver 
    break;

  case CLEAR_COMM_FEATURE:
    // Not  needed for this driver 
    break;

  case SET_LINE_CODING:
    linecoding.bitrate = (uint32_t)(Buf[0] | (Buf[1] << 8) | (Buf[2] << 16) | (Buf[3] << 24));
    linecoding.format = Buf[4];
    linecoding.paritytype = Buf[5];
    linecoding.datatype = Buf[6];
    // Set the new configuration 
    VCP_COMConfig(OTHER_CONFIG);
    break;

  case GET_LINE_CODING:
    Buf[0] = (uint8_t)(linecoding.bitrate);
    Buf[1] = (uint8_t)(linecoding.bitrate >> 8);
    Buf[2] = (uint8_t)(linecoding.bitrate >> 16);
    Buf[3] = (uint8_t)(linecoding.bitrate >> 24);
    Buf[4] = linecoding.format;
    Buf[5] = linecoding.paritytype;
    Buf[6] = linecoding.datatype; 
    break;

  case SET_CONTROL_LINE_STATE:
    // Not  needed for this driver 
    break;

  case SEND_BREAK:
    // Not  needed for this driver 
    break;    
    
  default:
    break;
  }//*/

#ifdef DEBUG_OTG
	debug_printf("DEBUG_OTG: VCP_Ctrl r\n\r");
#endif

  return USBD_OK;
}

/**
  * @brief  VCP_DataTx
  *         CDC received data to be send over USB IN endpoint are managed in 
  *         this function.
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
  */
uint16_t VCP_DataTx (uint8_t* Buf, uint32_t Len)
{
	uint32_t i;
#ifdef XDEBUG_OTG
	debug_printf("DEBUG_OTG: VCP_DataTx e\n\r");
#endif

  /*if (linecoding.datatype == 7)
  {
    APP_Rx_Buffer[APP_Rx_ptr_in] = USART_ReceiveData(EVAL_COM1) & 0x7F;
  }
  else if (linecoding.datatype == 8)
  {
    APP_Rx_Buffer[APP_Rx_ptr_in] = USART_ReceiveData(EVAL_COM1);
  }
  
  APP_Rx_ptr_in++;
  */
   //*/

	for (i = 0; i < Len; i++) {
    //USART_SendData(EVAL_COM1, *(Buf + i) );
    //while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET);
		APP_Rx_Buffer[APP_Rx_ptr_in] = *(Buf + i); 
		APP_Rx_ptr_in++;
#ifdef XDEBUG_OTG
	debug_printf("%c", *(Buf + i));
#endif 

		// To avoid buffer overflow 
  		if(APP_Rx_ptr_in >= APP_RX_DATA_SIZE) {
    		APP_Rx_ptr_in = 0;
  		} 

 	} 

// To avoid buffer overflow 
  	if(APP_Rx_ptr_in >= APP_RX_DATA_SIZE) {
    	APP_Rx_ptr_in = 0;
  	} 


//APP_Rx_ptr_in++;
#ifdef XDEBUG_OTG
	debug_printf("DEBUG_OTG: VCP_DataTx r\n\r");
#endif

  return USBD_OK;
}

uint16_t VCP_sendchar (char c)
{
	
	VCP_DataTx((uint8_t *)&c, 1);
	return USBD_OK;

}

/**
  * @brief  VCP_DataRx
  *         Data received over USB OUT endpoint are sent over CDC interface 
  *         through this function.
  *           
  *         @note
  *         This function will block any OUT packet reception on USB endpoint 
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result 
  *         in receiving more data while previous ones are still not sent.
  *                 
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
  */
static uint16_t VCP_DataRx (uint8_t* Buf, uint32_t Len)
{
  uint32_t i;
  
#ifdef XDEBUG_OTG
	debug_printf("DEBUG_OTG: VCP_DataRx e\n\r");
#endif

	for (i = 0; i < Len; i++) {
		VCP_Rx_Buffer[VCP_Rx_BufferLen] = *(Buf + i);
		VCP_Rx_BufferLen = (VCP_Rx_BufferLen +1)%VCP_RX_BUFFER_SIZE;

		//if (VCP_Rx_BufferLen == 0) {
		//	VCP_Rx_BufferLen++;
		//}

    //USART_SendData(EVAL_COM1, *(Buf + i) );
    //while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET); 
#ifdef DEBUG_OTG
		debug_printf("%c", *(Buf + i));
#endif
	} 

#ifdef DEBUG_OTG
	debug_printf(" DEBUG_OTG: VCP_DataRx %d\n\r", VCP_Rx_BufferLen);
#endif

  	return USBD_OK;
}

int VCP_getchar(uint8_t *rxbyte)
{

#ifdef XDEBUG_OTG
	debug_printf(" DEBUG_OTG: VCP_getchar G:%d L:%d\n\r", VCP_Rx_BufferGetLen, VCP_Rx_BufferLen);
#endif

	//Check for boundary condition on rx ring buffer.	
	if ((VCP_Rx_BufferGetLen < VCP_Rx_BufferLen) || (((VCP_Rx_BufferGetLen + 1)%VCP_RX_BUFFER_SIZE) == VCP_Rx_BufferLen)) {	
		*rxbyte = VCP_Rx_Buffer[VCP_Rx_BufferGetLen];
		VCP_Rx_BufferGetLen = (VCP_Rx_BufferGetLen + 1)%VCP_RX_BUFFER_SIZE;
		return 1;
	} else {
		return 0;
	}
}

void VCP_rxflush(void)
{

	memset(VCP_Rx_Buffer, 0, sizeof(VCP_Rx_Buffer));

	VCP_Rx_BufferLen = 0;
	VCP_Rx_BufferGetLen = 0;
}

/**
  * @brief  VCP_COMConfig
  *         Configure the COM Port with default values or values received from host.
  * @param  Conf: can be DEFAULT_CONFIG to set the default configuration or OTHER_CONFIG
  *         to set a configuration received from the host.
  * @retval None.
  */
static uint16_t VCP_COMConfig(uint8_t Conf)
{
#ifdef DEBUG_OTG
	debug_printf("DEBUG_OTG: VCP_COMConfig e\n\r");
#endif


#ifdef DEBUG_OTG
	debug_printf("DEBUG_OTG: VCP_COMconfig r\n\r");
#endif

  return USBD_OK;
}

/**
  * @brief  EVAL_COM_IRQHandler
  *         
  * @param  None.
  * @retval None.
  */
//void EVAL_COM_IRQHandler(void)
//{
//  if (USART_GetITStatus(EVAL_COM1, USART_IT_RXNE) != RESET)
//  {
    // Send the received data to the PC Host
//    VCP_DataTx (0,0);
//  }

  // If overrun condition occurs, clear the ORE flag and recover communication 
//  if (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_ORE) != RESET)
//  {
//    (void)USART_ReceiveData(EVAL_COM1);
//  }
//}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/**
  ******************************************************************************
  * @file    netduinoplus2.c
  * @author  MCD Application Team Modified by M. D'Souza
  * @version V1.1.0
  * @date    28-October-2011 - Modified 4-October-2013
  * @brief   This file provides set of firmware functions to manage Leds and
  *          push-button available on NetduinoPlus2.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "netduinoplus2.h"
#include "stm32f4xx_conf.h"
#include "core_cm4.h"

#ifdef ENABLE_VCP
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;

#endif


void NP2_debuguart_init()
{
	GPIO_InitTypeDef GPIO_serialtx;	
	USART_InitTypeDef USART_debug;

	RCC_APB1PeriphClockCmd(NP2_DEBUG_UART_CLK, ENABLE);	//Enable DEBUG UART clock

	/* Configure Debug UART settings */
	USART_debug.USART_BaudRate = NP2_DEBUG_UART_BAUDRATE ;
  	USART_debug.USART_WordLength = USART_WordLength_8b;
  	USART_debug.USART_StopBits = USART_StopBits_1;
  	USART_debug.USART_Parity = USART_Parity_No;
  	USART_debug.USART_Mode = USART_Mode_Tx;
  	USART_debug.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 

	USART_Init(NP2_DEBUG_UART, &USART_debug);		//Initialise DEBUG UART

	RCC_AHB1PeriphClockCmd(NP2_DEBUG_UART_GPIO_CLK, ENABLE);	//Enable DEBUG UART TX PIN

  	/* Configure the GPIO TX pin for DEBUG UART */ 
  	GPIO_serialtx.GPIO_Pin = NP2_DEBUG_UART_TX_PIN;
  	GPIO_serialtx.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_serialtx.GPIO_OType = GPIO_OType_PP;
  	GPIO_serialtx.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_serialtx.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(NP2_DEBUG_UART_GPIO_PORT, &GPIO_serialtx);

	GPIO_PinAFConfig(NP2_DEBUG_UART_GPIO_PORT, NP2_DEBUG_UART_TX_PINSOURCE, NP2_DEBUG_UART_AF); //Initialise DEBUG UART TX pin

	USART_Cmd(NP2_DEBUG_UART, ENABLE);
	
}

/**
  * @brief  Initialise the NP2 board by turning on the power headers
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED4

  *     @arg LED3
  *     @arg LED5
  *     @arg LED6
  * @retval None

  */

void NP2_boardinit()
{

	GPIO_InitTypeDef  GPIO_InitPowerHeader;

	//Enable fault exceptions - Bus, Usage and MemManage faults
	SCB->SHCSR |= (SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);

	/* Ensure all priority bits are assigned as preemption priority bits. */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
  
  	/* Enable the GPIO clock for NP2 power headers */
  	RCC_AHB1PeriphClockCmd(NP2_CTRL_OF_PWR_HEADERS_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Enable Ethernet clock */
	GPIO_InitPowerHeader.GPIO_Pin = NP2_ENC_CLK_PIN;
  	GPIO_InitPowerHeader.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitPowerHeader.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitPowerHeader.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_InitPowerHeader.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_Init(NP2_ENC_CLK_GPIO_PORT, &GPIO_InitPowerHeader);
	GPIO_PinAFConfig(NP2_ENC_CLK_GPIO_PORT, NP2_ENC_CLK_SOURCE, NP2_ENC_CLK_AF);

  	/* Configure the GPIO for NP2 power, SD MMC Card and Ethernet power control headers */
  	GPIO_InitPowerHeader.GPIO_Pin = NP2_CTRL_OF_PWR_HEADERS | NP2_MMC_PWR_PIN;
  	GPIO_InitPowerHeader.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitPowerHeader.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitPowerHeader.GPIO_PuPd = GPIO_PuPd_DOWN;
  	GPIO_InitPowerHeader.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(NP2_CTRL_OF_PWR_HEADERS_GPIO_PORT, &GPIO_InitPowerHeader);

	GPIO_InitPowerHeader.GPIO_Pin = NP2_ENC_PWR_PIN;
  	GPIO_InitPowerHeader.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitPowerHeader.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitPowerHeader.GPIO_PuPd = GPIO_PuPd_DOWN;
  	GPIO_InitPowerHeader.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(NP2_ENC_RST_GPIO_PORT, &GPIO_InitPowerHeader);

	/* Turn on NP2 Power headers */
	GPIO_SetBits(NP2_CTRL_OF_PWR_HEADERS_GPIO_PORT, NP2_CTRL_OF_PWR_HEADERS);

	/* Turn Ethernet Power on */	
	GPIO_ResetBits(NP2_ENC_PWR_GPIO_PORT, NP2_ENC_PWR_PIN);

	/* Turn SD MMC Card Power on */	
	GPIO_ResetBits(NP2_MMC_PWR_GPIO_PORT, NP2_MMC_PWR_PIN);

	//enable debug uart
#ifdef ENABLE_DEBUG_UART
	NP2_debuguart_init();
#endif

#ifdef ENABLE_VCP
	USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);
#endif
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:

  * @retval None
  */
void NP2_LEDInit()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  RCC_AHB1PeriphClockCmd(NP2_LED_BLUE_GPIO_CLK, ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = NP2_LED_BLUE_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(NP2_LED_BLUE_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:  
  * @retval None
  */
void NP2_LEDOn()
{
  NP2_LED_BLUE_GPIO_PORT->BSRRL |= NP2_LED_BLUE_PIN;
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  * @retval None
  */
void NP2_LEDOff()
{
  NP2_LED_BLUE_GPIO_PORT->BSRRH |= NP2_LED_BLUE_PIN;  
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:  
  * @retval None
  */
void NP2_LEDToggle()
{
  NP2_LED_BLUE_GPIO_PORT->ODR ^= NP2_LED_BLUE_PIN;
}

/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter should be: BUTTON_USER
  * @param  Button_Mode: Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO 
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability  
  * @retval None
  */ /*
void NP2_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  // Enable the BUTTON Clock 
  RCC_AHB1PeriphClockCmd(BUTTON_CLK[Button], ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // Configure Button pin as input 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
  GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

  if (Button_Mode == BUTTON_MODE_EXTI)
  {
    // Connect Button EXTI Line to Button GPIO Pin 
    SYSCFG_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

    // Configure Button EXTI line 
    EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // Enable and set Button EXTI Interrupt to the lowest priority 
    NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure); 
  }
}//*/

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter should be: BUTTON_USER  
  * @retval The Button GPIO pin value.
  *//*
uint32_t NP2_PBGetState(Button_TypeDef Button)
{
  return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}*/
    
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides all exceptions handler and peripherals interrupt
  *          service routine.
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
#include "stm32f4xx_it.h"

#ifdef ENABLE_VCP
#include "usb_core.h"
#include "usbd_core.h"

#ifdef ENABLE_FRVCP
#include "FreeRTOS.h"
#endif

#include "netduinoplus2.h"
#include "core_cm4.h"
#include "stm32f4xx_conf.h"
//#define DEBUG_IT	1
//#ifdef DEBUG_IT
//#include "debug_printf.h"
//#endif 

#endif




/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#ifdef ENABLE_VCP
extern USB_OTG_CORE_HANDLE           USB_OTG_dev;
extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
#endif

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{

//#ifdef DEBUG_IT
//	debug_printf("IT DEBUG: HardFault\n\r");
//#endif		

	//NP2_LEDOn();	//%%%%%%%%%%%%%%%%%%%%%
	if (SCB->HFSR == 0x40000000) { //|= 0xFFFFFFFF;
		//NP2_LEDOff();
	}
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
//#ifdef DEBUG_IT
//	debug_printf("IT DEBUG: MemManage\n\r");
//#endif
	NP2_LEDOn();
	if (SCB->CFSR == 0x00000001) {
		//NP2_LEDOff();
	}
  	/* Go to infinite loop when Memory Manage exception occurs */
  	while (1)
  	{
  	}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
//#ifdef DEBUG_IT
//	debug_printf("IT DEBUG: BusFault\n\r");
//#endif
	//NP2_LEDOn();
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
//#ifdef DEBUG_IT
//	debug_printf("IT DEBUG: UsageFault\n\r");
//#endif
	NP2_LEDOn();
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

void SPI3_IRQHandler (void)
{
	while (1)
  {
  }

}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
/*void SVC_Handler(void)
{
}*/

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
	//NP2_LEDOn();
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
/*void PendSV_Handler(void)
{
}*/

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
/*void SysTick_Handler(void)
{
  uint8_t *buf;
  uint8_t temp1, temp2 = 0x00;

#ifdef DEBUG_IT
	debug_printf("DEBUG_IT: SysTick_Handler\n\r");
#endif
 
}*/

/******************************************************************************/
/*                 STM32Fxxx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32fxxx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @brief  This function handles OTG FS Wakeup IRQ Handler.
  * @param  None
  * @retval None
  */
void OTG_FS_WKUP_IRQHandler(void)
{
#ifdef ENABLE_VCP

#ifdef ENABLE_FRVCP
	( void ) portSET_INTERRUPT_MASK_FROM_ISR();
#endif

  if(USB_OTG_dev.cfg.low_power)
  {
	// Reset SLEEPDEEP and SLEEPONEXIT bits 
	SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));

	// After wake-up from sleep mode, reconfigure the system clock 
	SystemInit();
    USB_OTG_UngateClock(&USB_OTG_dev);
  }
  EXTI_ClearITPendingBit(EXTI_Line18);

#ifdef ENABLE_FRVCP
	portCLEAR_INTERRUPT_MASK_FROM_ISR( 0 );
#endif

#endif

}

/**
  * @brief  This function handles OTG_HS Handler.
  * @param  None
  * @retval None
  */
void OTG_FS_IRQHandler(void)
{
#ifdef ENABLE_VCP

#ifdef ENABLE_FRVCP
	( void ) portSET_INTERRUPT_MASK_FROM_ISR();
#endif

  USBD_OTG_ISR_Handler(&USB_OTG_dev);

#ifdef ENABLE_FRVCP
	portCLEAR_INTERRUPT_MASK_FROM_ISR( 0 );
#endif

#endif
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

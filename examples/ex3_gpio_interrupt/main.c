/**
  ******************************************************************************
  * @file    ex3_gpio_interrupt.c 
  * @author  MDS
  * @date    10-January-2014
  * @brief   Enable external interrupt on pin D0.
  ******************************************************************************
  *  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "netduinoplus2.h"
#include "stm32f4xx_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int press_count;

/* Private function prototypes -----------------------------------------------*/
void Hardware_init();


/**
  * @brief  Main program - enable D0 external interrupt.
  * @param  None
  * @retval None
  */
void main(void) {
	
	NP2_boardinit();	//Initalise NP2
	Hardware_init();	//Initalise hardware modules
  	
	/* Main processing loop waiting for interrupt */
    while (1) {
  	}

}

/**
  * @brief  Initialise Hardware Peripherals used.
  * @param  None
  * @retval None
  */
void Hardware_init() {

  debug_printf("Initialising\n");

	GPIO_InitTypeDef  GPIO_InitStructure;
  	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	NP2_LEDInit();		//Initialise Blue LED
	NP2_LEDOff();		//Turn off Blue LED

  	/* Enable DO clock */
  	RCC_AHB1PeriphClockCmd(NP2_D0_GPIO_CLK, ENABLE);

  	/* Enable SYSCFG clock for interrupt hardware */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  	/* Configure D0 pin as pull down input */
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  	GPIO_InitStructure.GPIO_Pin = NP2_D0_PIN;
  	GPIO_Init(NP2_D0_GPIO_PORT, &GPIO_InitStructure);

  	/* Connect external interrupt EXTI Line0 to DO pin */
  	SYSCFG_EXTILineConfig(NP2_D0_EXTI_PORT, NP2_D0_EXTI_SOURCE);

  	/* Configure EXTI Line0 for rising edge detection */
  	EXTI_InitStructure.EXTI_Line = NP2_D0_EXTI_LINE;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);

  	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
  	NVIC_InitStructure.NVIC_IRQChannel = NP2_D0_EXTI_IRQ;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  NP2_D0_EXTI Interrupt handler - see netduinoplus2.h
  * @param  None.
  * @retval None
  */
void NP2_D0_EXTI_IRQ_HANDLER(void) {

  debug_printf("Interrupt\n");

	/* Check if D0 external interrupt has occured */
  	if (EXTI_GetITStatus(NP2_D0_EXTI_LINE) != RESET) {
    
		/* Toggle LED4 */
    	NP2_LEDToggle();

		debug_printf("Triggered - %d\n\r", press_count);    //Print press count value
		press_count++;

    	/* Clear the EXTI line 0 pending bit */
    	EXTI_ClearITPendingBit(NP2_D0_EXTI_LINE);
  	}
}


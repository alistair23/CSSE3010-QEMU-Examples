/**
  ******************************************************************************
  * @file    ex5_timer_interrupt.c 
  * @author  MDS
  * @date    10-January-2014
  * @brief   Enable Timer 5 update compare interrupt. Use interrupt to flash
  *			 LED every second.
  *			 See Section 18 (TIM5), P576 of the STM32F4xx Reference Manual.
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
/* Private function prototypes -----------------------------------------------*/
void Hardware_init(void);


/**
  * @brief  Main program - Timer 5 update compare interrupt.
  * @param  None
  * @retval None
  */
void main(void) {

	NP2_boardinit();	//Initalise NP2
	Hardware_init();	//Initalise hardware modules

  	/* Main processing loop waiting for interrupt */
  	while (1) {
      //debug_printf("Looping\n");
	}
}

/**
  * @brief  Configure the hardware, 
  * @param  None
  * @retval None
  */
void Hardware_init(void) {

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	unsigned short PrescalerValue;

	NP2_LEDInit();		//Initialise Blue LED
	NP2_LEDOff();		//Turn off Blue LED
  
  	/* TIM5 clock enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	/* Compute the prescaler value */
  	PrescalerValue = (uint16_t) ((SystemCoreClock /2)/50000) - 1;		//Set clock prescaler to 50kHz

  	/* Time base configuration */
  	TIM_TimeBaseStructure.TIM_Period = 50000;			//Set period to be 1s
  	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	/* Set Reload Value for 1s */
	TIM_SetAutoreload(TIM5, 50000);
  	TIM_ARRPreloadConfig(TIM5, ENABLE);

	/* Enable NVIC for Timer 5 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

	/* Enable timer 5 update interrupt - Page: 579, STM32F4xx Programmer's Reference Manual */
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

  	/* TIM5 enable counter */
  	TIM_Cmd(TIM5, ENABLE);
}

/**
  * @brief  Timer 5 Interrupt handler
  * @param  None.
  * @retval None
  */
void TIM5_IRQHandler (void) {

	/* Toggle LED if Timer 5 update compare interrupt has occured */
  debug_printf("One\n");
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) {
      debug_printf("Two\n");
    	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
		NP2_LEDToggle();
	}
}



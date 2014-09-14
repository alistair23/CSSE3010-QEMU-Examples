/**
  ******************************************************************************
  * @file    ex6_pwm/main.c 
  * @author  MDS
  * @date    10-January-2014
  * @brief   Enable the PWM output on pin DO.
  *			 See Section 18 (TIM3), P576 of the STM32F4xx Reference Manual.
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
void Delay(__IO unsigned long nCount);
void Hardware_init();

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void main(void) {

	NP2_boardinit();	//Initalise NP2
	Hardware_init();	//Initalise hardware modules

  	while (1) {

		NP2_LEDToggle();	//Toggle 'Alive' LED on/off
    	Delay(0x7FFF00);	//Delay function
	}
}

/**
  * @brief  Configure the hardware, 
  * @param  None
  * @retval None
  */
void Hardware_init(void) {

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t PrescalerValue = 0;

	NP2_LEDInit();		//Initialise Blue LED
	NP2_LEDOff();		//Turn off Blue LED


  	/* TIM3 clock enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  	/* Enable the D0 Clock */
  	RCC_AHB1PeriphClockCmd(NP2_D0_GPIO_CLK, ENABLE);

  	/* Configure the D0 pin */
  	GPIO_InitStructure.GPIO_Pin = NP2_D0_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  	GPIO_Init(NP2_D0_GPIO_PORT, &GPIO_InitStructure); 
  
  	/* Connect TIM3 output to D0 pin */  
  	GPIO_PinAFConfig(NP2_D0_GPIO_PORT, NP2_D0_PINSOURCE, GPIO_AF_TIM3);

	/* Compute the prescaler value. SystemCoreClock = 168000000 - set for 500Khz clock */
  	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 500000) - 1;

  	/* Time 3 mode and prescaler configuration */
  	TIM_TimeBaseStructure.TIM_Period = 500000/10; 	//Set for 100ms (10Hz) period
  	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	/* Configure Timer 3 mode and prescaler */
  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  	/* PWM Mode configuration for Channel2 - set pulse width*/
  	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//Set PWM MODE (1 or 2 - NOT CHANNEL)
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_Pulse = 500000/1000;		//1ms pulse width
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	/* Enable Output compare channel 2 */
  	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

  	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  	/* TIM3 enable counter */
 	TIM_Cmd(TIM3, ENABLE); 
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO unsigned long nCount) {
  	while(nCount--) {
  	}
}



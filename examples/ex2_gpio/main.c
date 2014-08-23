/**
  ******************************************************************************
  * @file    ex2_gpio.c 
  * @author  MDS
  * @date    10-January-2014
  * @brief   Toggle D0 high/low and read D1 pin.
  ******************************************************************************
  *  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "netduinoplus2.h"
#include "stm32f4xx_conf.h"
#include "debug_printf.h"

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

	int read_value;
	int write_value = 0;

	NP2_boardinit();	//Initalise NP2
	Hardware_init();	//Initalise hardware modules  	

	/* Main processing loop */
    while (1) {

		/* Toggle D0 high or low */
		write_value = ~write_value;
		GPIO_WriteBit(NP2_D0_GPIO_PORT, NP2_D0_PIN, write_value & 0x01);	//Write Digital 0 bit value
	
		/* Read D1 pin */
		read_value = GPIO_ReadInputDataBit(NP2_D1_GPIO_PORT, NP2_D1_PIN);

		debug_printf("D1 value %d\n\r", read_value);	//Display D1 value

    	NP2_LEDToggle();	//Toggle LED on/off
    	Delay(0x7FFF00);	//Delay function
  	}
}

/**
  * @brief  Initialise Hardware 
  * @param  None
  * @retval None
  */
void Hardware_init() {

	GPIO_InitTypeDef  GPIO_InitStructure;

	NP2_LEDInit();		//Initialise Blue LED
	NP2_LEDOff();		//Turn off Blue LED

	/* Enable the D0 & D1 Clock */
  	RCC_AHB1PeriphClockCmd(NP2_D0_GPIO_CLK | NP2_D1_GPIO_CLK, ENABLE);

  	/* Configure the D0 pin */
  	GPIO_InitStructure.GPIO_Pin = NP2_D0_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(NP2_D0_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = NP2_D1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_Init(NP2_D1_GPIO_PORT, &GPIO_InitStructure);
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



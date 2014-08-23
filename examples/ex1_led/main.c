/**
  ******************************************************************************
  * @file    ex1_led/main.c 
  * @author  MDS
  * @date    10-January-2014
  * @brief   NP2 onboard blue LED flashing example.
  ******************************************************************************
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
void Hardware_init(void);
void Delay(__IO unsigned long nCount);


/**
  * @brief  Main program - flashes onboard blue LED
  * @param  None
  * @retval None
  */
void main(void)  {

	NP2_boardinit();	//Initalise NP2
	Hardware_init();	//Initalise hardware modules
  	
	/* Main processing loop */
    while (1) {

		debug_printf("LED Toggle\n\r");	//Print debug message
    	
		NP2_LEDToggle();	//Toggle LED on/off
    	Delay(0x7FFF00);	//Delay 1s.
	}

}

/**
  * @brief  Initialise Hardware 
  * @param  None
  * @retval None
  */
void Hardware_init(void) {

	NP2_LEDInit();		//Initialise Blue LED
	NP2_LEDOff();		//Turn off Blue LED
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


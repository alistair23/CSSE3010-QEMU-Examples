/**
  ******************************************************************************
  * @file    ex6_usart/main.c 
  * @author  MDS
  * @date    10-January-2014
  * @brief   USART 6 polling example program - transmits '0' to '9' via USART 6.
  *			 Display received characters from USART 6.
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
	
	char tx_char;
	char rx_char;
	int tx_count;
	
	NP2_boardinit();	//Initalise NP2
	Hardware_init();	//Initalise hardware modules

	/* Main processing loop */
    while (1) {
  
		tx_char = '0' + tx_count;			//Send characters '0' to '9' in ASCII
		
		USART_SendData(USART6, tx_char);								//Send character via USART 6
		while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);	//Wait for Transmit Clear flag to be SET
		USART_ClearFlag(USART6, USART_FLAG_TC);							//Clear Transmit Clear flag

		tx_count = (tx_count +1)%10;		//Only send characters '0' to '9'.

		/* Check if receive buffer has received a character */
		if (USART_GetFlagStatus(USART6, USART_FLAG_RXNE) == SET) {
			rx_char = USART_ReceiveData(USART6);						//Read character received by USART 6
			USART_ClearFlag(USART6, USART_FLAG_RXNE);					//Clear receive buffer register flag

			debug_printf("RX %c\n\r", rx_char);
		}

    	NP2_LEDToggle();	//Toggle 'Alive' LED on/off
    	Delay(0x7FFF00);	//Delay function
  	}
}

/**
  * @brief  Initialise Hardware 
  * @param  None
  * @retval None
  */
void Hardware_init() {

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_debug;

	NP2_LEDInit();		//Initialise Blue LED
	NP2_LEDOff();		//Turn off Blue LED

	/* Enable D0 and D1 GPIO clocks */
	RCC_AHB1PeriphClockCmd(NP2_D0_GPIO_CLK | NP2_D1_GPIO_CLK, ENABLE);

	/* Enable USART 6 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	/* Configure USART 6 to 115200 baudrate, 8bits, 1 stop bit, no parity, no flow control */
	USART_debug.USART_BaudRate = 4800;	//115200;				
  	USART_debug.USART_WordLength = USART_WordLength_8b;
  	USART_debug.USART_StopBits = USART_StopBits_1;
  	USART_debug.USART_Parity = USART_Parity_No;
  	USART_debug.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	USART_debug.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  

	USART_Init(USART6, &USART_debug);

  	/* Configure the GPIO USART 2 pins */ 
  	GPIO_InitStructure.GPIO_Pin = NP2_D1_PIN | NP2_D0_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(NP2_D0_GPIO_PORT, &GPIO_InitStructure);

	/* Connect USART 6 TX and RX pins to D1 and D0 */
	GPIO_PinAFConfig(NP2_D1_GPIO_PORT, NP2_D1_PINSOURCE, GPIO_AF_USART6);
	GPIO_PinAFConfig(NP2_D0_GPIO_PORT, NP2_D0_PINSOURCE, GPIO_AF_USART6);

	/* Enable USART 6 */
  	USART_Cmd(USART6, ENABLE);

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



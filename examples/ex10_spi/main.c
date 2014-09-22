/**
  ******************************************************************************
  * @file    ex10_spi/main.c 
  * @author  MDS
  * @date    10-January-2014
  * @brief   SPI Read 32-bit Register nrf24l01plus status (0x07) register
  *			 NOTE: This example does not send or transmit with the nrf24l01plus.
  *			 REFERENCE: nrf24l01p_datasheet.pdf
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
void HardwareInit();
uint8_t spi_sendbyte(uint8_t sendbyte);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void) {

	uint8_t status;
	
	NP2_boardinit();
	HardwareInit();

	/* Main Processing Loop */
    while (1) {

		GPIO_ResetBits(NP2_D10_GPIO_PORT, NP2_D10_PIN);	//Set Chip Select low

		spi_sendbyte(0x07);										//Send status register address
	
		status = spi_sendbyte(0xFF);							//Send dummy byte, to read status register values			
		debug_printf("nrf24L01 Status Register Value: %X ", status);	//See page 59 of nrf24l01plus datasheet for status register definition

		debug_printf("\n\r");

		GPIO_SetBits(NP2_D10_GPIO_PORT, NP2_D10_PIN);		//Set Chip Select high	

    	NP2_LEDToggle();	//Toggle LED on/off
    	Delay(0x7FFF00);	//Delay function
  	}

}

/**
  * @brief  Initialise hardware modules
  * @param  None
  * @retval None
  */
void HardwareInit() {
	
	GPIO_InitTypeDef GPIO_spi;	
	SPI_InitTypeDef	NP2_spiInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;

	NP2_LEDInit();		//Initialise Blue LED
	NP2_LEDOff();		//Turn off Blue LED

	RCC_AHB1PeriphClockCmd(NP2_D10_GPIO_CLK, ENABLE);

	/* Set SPI clodk */
	RCC_APB1PeriphClockCmd(NP2_SPI_CLK, ENABLE);
	
	/* SPI SCK pin configuration */
  	GPIO_spi.GPIO_Mode = GPIO_Mode_AF;		//Alternate function
  	GPIO_spi.GPIO_OType = GPIO_OType_PP;
  	GPIO_spi.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_spi.GPIO_PuPd  = GPIO_PuPd_DOWN;	//Pull down resistor
  	GPIO_spi.GPIO_Pin = NP2_SPI_SCK_PIN;
  	GPIO_Init(NP2_SPI_SCK_GPIO_PORT, &GPIO_spi);

  	/* SPI MISO pin configuration */
	GPIO_spi.GPIO_Mode = GPIO_Mode_AF;		//Alternate function
	GPIO_spi.GPIO_OType = GPIO_OType_PP;
	GPIO_spi.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_spi.GPIO_PuPd  = GPIO_PuPd_UP;		//Pull up resistor
  	GPIO_spi.GPIO_Pin = NP2_SPI_MISO_PIN;
  	GPIO_Init(NP2_SPI_MISO_GPIO_PORT, &GPIO_spi);

	/* SPI  MOSI pin configuration */
	GPIO_spi.GPIO_Mode = GPIO_Mode_AF;		//Alternate function
	GPIO_spi.GPIO_OType = GPIO_OType_PP;	
	GPIO_spi.GPIO_PuPd  = GPIO_PuPd_DOWN;	//Pull down resistor	
  	GPIO_spi.GPIO_Pin =  NP2_SPI_MOSI_PIN;
  	GPIO_Init(NP2_SPI_MOSI_GPIO_PORT, &GPIO_spi);

	/* Connect SPI pins */
	GPIO_PinAFConfig(NP2_SPI_SCK_GPIO_PORT, NP2_SPI_SCK_SOURCE, NP2_SPI_SCK_AF);
	GPIO_PinAFConfig(NP2_SPI_MISO_GPIO_PORT, NP2_SPI_MISO_SOURCE, NP2_SPI_MISO_AF);
	GPIO_PinAFConfig(NP2_SPI_MOSI_GPIO_PORT, NP2_SPI_MOSI_SOURCE, NP2_SPI_MOSI_AF);

	/* SPI configuration */
  	SPI_I2S_DeInit(NP2_SPI);

  	NP2_spiInitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  	NP2_spiInitStruct.SPI_DataSize = SPI_DataSize_8b;
  	NP2_spiInitStruct.SPI_CPOL = SPI_CPOL_Low;	//SPI_CPOL_Low;
  	NP2_spiInitStruct.SPI_CPHA = SPI_CPHA_1Edge;	//SPI_CPHA_1Edge;
  	NP2_spiInitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;	//SPI_NSS_Soft;
  	NP2_spiInitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	//256;
  	NP2_spiInitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
  	NP2_spiInitStruct.SPI_CRCPolynomial = 0;	//7;
  	NP2_spiInitStruct.SPI_Mode = SPI_Mode_Master;
  	SPI_Init(NP2_SPI, &NP2_spiInitStruct);

  	/* Enable NP2 external SPI */
  	SPI_Cmd(NP2_SPI, ENABLE);

	/* Configure GPIO PIN for  Chip select */
  	GPIO_InitStructure.GPIO_Pin = NP2_SPI_CS_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(NP2_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

	/* Set chip select high */
	GPIO_SetBits(NP2_D10_GPIO_PORT, NP2_D10_PIN);
}

/**
  * @brief  Send byte through SPI.
  * @param  sendbyte: byte to be transmitted via SPI.
  * @retval None
  */
uint8_t spi_sendbyte(uint8_t sendbyte) {

	uint8_t readbyte;

	/* Loop while DR register in not empty */
	while (SPI_I2S_GetFlagStatus(NP2_SPI, SPI_I2S_FLAG_TXE) == RESET);
	
	/* Send a Byte through the SPI peripheral */
	SPI_I2S_SendData(NP2_SPI, sendbyte);

	/* Wait while busy flag is set */
	while (SPI_I2S_GetFlagStatus(NP2_SPI, SPI_I2S_FLAG_BSY) == SET);
	
	/* Wait to receive a Byte */
	while (SPI_I2S_GetFlagStatus(NP2_SPI, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the Byte read from the SPI bus */
	readbyte = (uint8_t)SPI_I2S_ReceiveData(NP2_SPI);

	/* Return the Byte read from the SPI bus */
	return readbyte;
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


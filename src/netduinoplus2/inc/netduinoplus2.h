/**
  ******************************************************************************
  * @file    netduinoplus2.h
  * @author  MDS
  * @date    4-October-2013
  * @brief   This file provides pin and peripheral definitions for the
  *          Netduino Plus 2.
  ******************************************************************************
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NETDUINOPLUS2_H
#define __NETDUINOPLUS2_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
 #include "stm32f4xx.h"

#define ENABLE_DEBUG_UART

//NP2 ANALOG PIN Definitions
#define NP2_A0						0						//Mask
#define NP2_A0_PIN					GPIO_Pin_0				//Pin Index
#define NP2_A0_PINSOURCE			GPIO_PinSource0			//Alternate Function Pin Index
#define NP2_A0_GPIO_PORT			GPIOC					//GPIO Port for pin
#define NP2_A0_GPIO_CLK				RCC_AHB1Periph_GPIOC	//GPIO Port clock
#define NP2_A0_EXTI_PORT			EXTI_PortSourceGPIOC	//External Interrupt Port
#define NP2_A0_EXTI_SOURCE			EXTI_PinSource0			//External Interrupt Pin Index
#define NP2_A0_EXTI_LINE			EXTI_Line0				//External Interrupt Line Index
#define NP2_A0_EXTI_IRQ				EXTI0_IRQn				//External Interrupt IRQ Index
#define NP2_A0_EXTI_IRQ_HANDLER		EXTI0_IRQHandler		//External Interrupt Handler Index
#define NP2_A0_ADC_CHAN				ADC_Channel_10			//NOTE only ADC 1,2 & 3 use

#define NP2_A1						1
#define NP2_A1_PIN					GPIO_Pin_1
#define NP2_A1_PINSOURCE			GPIO_PinSource1
#define NP2_A1_GPIO_PORT			GPIOC
#define NP2_A1_GPIO_CLK				RCC_AHB1Periph_GPIOC
#define NP2_A1_EXTI_PORT			EXTI_PortSourceGPIOC
#define NP2_A1_EXTI_SOURCE			EXTI_PinSource1
#define NP2_A1_EXTI_LINE			EXTI_Line1
#define NP2_A1_EXTI_IRQ				EXTI1_IRQn
#define NP2_A1_EXTI_IRQ_HANDLER		EXTI1_IRQHandler
#define NP2_A1_ADC_CHAN				ADC_Channel_11			//NOTE only ADC 1,2 & 3 use


#define NP2_A2						2
#define NP2_A2_PIN					GPIO_Pin_2
#define NP2_A2_PINSOURCE			GPIO_PinSource2
#define NP2_A2_GPIO_PORT			GPIOC
#define NP2_A2_GPIO_CLK				RCC_AHB1Periph_GPIOC
#define NP2_A2_EXTI_PORT			EXTI_PortSourceGPIOC
#define NP2_A2_EXTI_SOURCE			EXTI_PinSource2
#define NP2_A2_EXTI_LINE			EXTI_Line2
#define NP2_A2_EXTI_IRQ				EXTI2_IRQn
#define NP2_A2_EXTI_IRQ_HANDLER		EXTI2_IRQHandler
#define NP2_A2_ADC_CHAN				ADC_Channel_12			//NOTE only ADC 1,2 & 3 use

#define NP2_A3						3
#define NP2_A3_PIN					GPIO_Pin_3
#define NP2_A3_PINSOURCE			GPIO_PinSource3
#define NP2_A3_GPIO_PORT			GPIOC
#define NP2_A3_GPIO_CLK				RCC_AHB1Periph_GPIOC
#define NP2_A3_EXTI_PORT			EXTI_PortSourceGPIOC
#define NP2_A3_EXTI_SOURCE			EXTI_PinSource3
#define NP2_A3_EXTI_LINE			EXTI_Line3
#define NP2_A3_EXTI_IRQ				EXTI3_IRQn
#define NP2_A3_EXTI_IRQ_HANDLER		EXTI3_IRQHandler
#define NP2_A3_ADC_CHAN				ADC_Channel_13			//NOTE only ADC 1,2 & 3 use

#define NP2_A4						4
#define NP2_A4_PIN					GPIO_Pin_4
#define NP2_A4_PINSOURCE			GPIO_PinSource4
#define NP2_A4_GPIO_PORT 			GPIOC
#define NP2_A4_GPIO_CLK				RCC_AHB1Periph_GPIOC
#define NP2_A4_EXTI_PORT			EXTI_PortSourceGPIOC
#define NP2_A4_EXTI_SOURCE			EXTI_PinSource4
#define NP2_A4_EXTI_LINE			EXTI_Line4
#define NP2_A4_EXTI_IRQ				EXTI4_IRQn
#define NP2_A4_EXTI_IRQ_HANDLER		EXTI4_IRQHandler
#define NP2_A4_ADC_CHAN				ADC_Channel_14			//NOTE only ADC 1 & 2 use

#define NP2_A5						5
#define NP2_A5_PIN					GPIO_Pin_5
#define NP2_A5_PINSOURCE			GPIO_PinSource5
#define NP2_A5_GPIO_PORT			GPIOC
#define NP2_A5_GPIO_CLK				RCC_AHB1Periph_GPIOC
#define NP2_A5_EXTI_PORT			EXTI_PortSourceGPIOC
#define NP2_A5_EXTI_SOURCE			EXTI_PinSource5
#define NP2_A5_EXTI_LINE			EXTI_Line5
#define NP2_A5_EXTI_IRQ				EXTI9_5_IRQn
#define NP2_A5_EXTI_IRQ_HANDLER		EXTI9_5_IRQHandler
#define NP2_A5_ADC_CHAN				ADC_Channel_15			//NOTE only ADC 1 & 2 use


//NP2 DIGITAL PIN Definitions
#define NP2_D0						7						//Mask
#define NP2_D0_PIN					GPIO_Pin_7				//Pin INdex
#define NP2_D0_PINSOURCE			GPIO_PinSource7			//Alternate Function Pin Index
#define NP2_D0_GPIO_PORT			GPIOC					//GPIO Port for Pin
#define NP2_D0_GPIO_CLK				RCC_AHB1Periph_GPIOC	//GPIO Port Clock
#define NP2_D0_EXTI_PORT			EXTI_PortSourceGPIOC	//External Interrupt Port
#define NP2_D0_EXTI_SOURCE			EXTI_PinSource7			//External Interrupt Pin Index
#define NP2_D0_EXTI_LINE			EXTI_Line7				//External Interrupt Line Index	
#define NP2_D0_EXTI_IRQ				EXTI9_5_IRQn			//External Interrupt Index	
#define NP2_D0_EXTI_IRQ_HANDLER		EXTI9_5_IRQHandler	//External Interrupt Handler Index

#define NP2_D1						6
#define NP2_D1_PIN					GPIO_Pin_6
#define NP2_D1_PINSOURCE			GPIO_PinSource6
#define NP2_D1_GPIO_PORT			GPIOC
#define NP2_D1_GPIO_CLK				RCC_AHB1Periph_GPIOC
#define NP2_D1_EXTI_PORT			EXTI_PortSourceGPIOC
#define NP2_D1_EXTI_SOURCE			EXTI_PinSource6
#define NP2_D1_EXTI_LINE			EXTI_Line6
#define NP2_D1_EXTI_IRQ				EXTI9_5_IRQn
#define NP2_D1_EXTI_IRQ_HANDLER		EXTI9_5_IRQHandler

#define NP2_D2						3
#define NP2_D2_PIN					GPIO_Pin_3
#define NP2_D2_PINSOURCE			GPIO_PinSource3
#define NP2_D2_GPIO_PORT			GPIOA
#define NP2_D2_GPIO_CLK				RCC_AHB1Periph_GPIOA
#define NP2_D2_EXTI_PORT			EXTI_PortSourceGPIOA
#define NP2_D2_EXTI_SOURCE			EXTI_PinSource3
#define NP2_D2_EXTI_LINE			EXTI_Line3
#define NP2_D2_EXTI_IRQ				EXTI3_IRQn
#define NP2_D2_EXTI_IRQ_HANDLER		EXTI3_IRQHandler

#define NP2_D3						2
#define NP2_D3_PIN					GPIO_Pin_2
#define NP2_D3_PINSOURCE			GPIO_PinSource2
#define NP2_D3_GPIO_PORT			GPIOA
#define NP2_D3_GPIO_CLK				RCC_AHB1Periph_GPIOA
#define NP2_D3_EXTI_PORT			EXTI_PortSourceGPIOA
#define NP2_D3_EXTI_SOURCE			EXTI_PinSource2
#define NP2_D3_EXTI_LINE			EXTI_Line2
#define NP2_D3_EXTI_IRQ				EXTI2_IRQ2n
#define NP2_D3_EXTI_IRQ_HANDLER		EXTI2_IRQHandler

#define NP2_D4						12
#define NP2_D4_PIN					GPIO_Pin_12
#define NP2_D4_PINSOURCE			GPIO_PinSource12
#define NP2_D4_GPIO_PORT 			GPIOB
#define NP2_D4_GPIO_CLK				RCC_AHB1Periph_GPIOB
#define NP2_D4_EXTI_PORT			EXTI_PortSourceGPIOB
#define NP2_D4_EXTI_SOURCE			EXTI_PinSource12
#define NP2_D4_EXTI_LINE			EXTI_Line12
#define NP2_D4_EXTI_IRQ				EXTI15_10_IRQn
#define NP2_D4_EXTI_IRQ_HANDLER		EXTI15_10_IRQHandler

#define NP2_D5						8
#define NP2_D5_PIN					GPIO_Pin_8
#define NP2_D5_PINSOURCE			GPIO_PinSource8
#define NP2_D5_GPIO_PORT			GPIOB
#define NP2_D5_GPIO_CLK				RCC_AHB1Periph_GPIOB
#define NP2_D5_EXTI_PORT			EXTI_PortSourceGPIOB
#define NP2_D5_EXTI_SOURCE			EXTI_PinSource8
#define NP2_D5_EXTI_LINE			EXTI_Line8
#define NP2_D5_EXTI_IRQ				EXTI9_5_IRQn
#define NP2_D5_EXTI_IRQ_HANDLER		EXTI9_5_IRQHandler

#define NP2_D6						9
#define NP2_D6_PIN					GPIO_Pin_9
#define NP2_D6_PINSOURCE			GPIO_PinSource9	
#define NP2_D6_GPIO_PORT			GPIOB
#define NP2_D6_GPIO_CLK				RCC_AHB1Periph_GPIOB
#define NP2_D6_EXTI_PORT			EXTI_PortSourceGPIOB
#define NP2_D6_EXTI_SOURCE			EXTI_PinSource9
#define NP2_D6_EXTI_LINE			EXTI_Line9
#define NP2_D6_EXTI_IRQ				EXTI9_5_IRQn
#define NP2_D6_EXTI_IRQ_HANDLER		EXTI9_5_IRQHandler

#define NP2_D7						1
#define NP2_D7_PIN					GPIO_Pin_1
#define NP2_D7_PINSOURCE			GPIO_PinSource1
#define NP2_D7_GPIO_PORT 			GPIOA
#define NP2_D7_GPIO_CLK				RCC_AHB1Periph_GPIOA
#define NP2_D7_EXTI_PORT			EXTI_PortSourceGPIOA
#define NP2_D7_EXTI_SOURCE			EXTI_PinSource1
#define NP2_D7_EXTI_LINE			EXTI_Line1
#define NP2_D7_EXTI_IRQ				EXTI1_IRQn
#define NP2_D7_EXTI_IRQ_HANDLER		EXTI1_IRQHandler

#define NP2_D8						0
#define NP2_D8_PIN					GPIO_Pin_0
#define NP2_D8_PINSOURCE			GPIO_PinSource0
#define NP2_D8_GPIO_PORT 			GPIOA
#define NP2_D8_GPIO_CLK				RCC_AHB1Periph_GPIOA
#define NP2_D8_EXTI_PORT			EXTI_PortSourceGPIOA
#define NP2_D8_EXTI_SOURCE			EXTI_PinSource0
#define NP2_D8_EXTI_LINE			EXTI_Line0
#define NP2_D8_EXTI_IRQ				EXTI0_IRQn
#define NP2_D8_EXTI_IRQ_HANDLER		EXTI0_IRQHandler

#define NP2_D9						6
#define NP2_D9_PIN					GPIO_Pin_6
#define NP2_D9_PINSOURCE			GPIO_PinSource6
#define NP2_D9_GPIO_PORT 			GPIOA
#define NP2_D9_GPIO_CLK				RCC_AHB1Periph_GPIOA
#define NP2_D9_EXTI_PORT			EXTI_PortSourceGPIOA
#define NP2_D9_EXTI_SOURCE			EXTI_PinSource6
#define NP2_D9_EXTI_LINE			EXTI_Line6
#define NP2_D9_EXTI_IRQ				EXTI9_5_IRQn
#define NP2_D9_EXTI_IRQ_HANDLER		EXTI9_5_IRQHandler

#define NP2_D10						10
#define NP2_D10_PIN					GPIO_Pin_10
#define NP2_D10_PINSOURCE			GPIO_PinSource10
#define NP2_D10_GPIO_PORT			GPIOB
#define NP2_D10_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define NP2_D10_EXTI_PORT			EXTI_PortSourceGPIOB
#define NP2_D10_EXTI_SOURCE			EXTI_PinSource10
#define NP2_D10_EXTI_LINE			EXTI_Line10
#define NP2_D10_EXTI_IRQ			EXTI15_10_IRQn
#define NP2_D10_EXTI_IRQ_HANDLER	EXTI15_10_IRQHandler

#define NP2_D11						15
#define NP2_D11_PIN					GPIO_Pin_15
#define NP2_D11_PINSOURCE			GPIO_PinSource15
#define NP2_D11_GPIO_PORT			GPIOB
#define NP2_D11_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define NP2_D11_EXTI_PORT			EXTI_PortSourceGPIOB
#define NP2_D11_EXTI_SOURCE			EXTI_PinSource15
#define NP2_D11_EXTI_LINE			EXTI_Line15
#define NP2_D11_EXTI_IRQ			EXTI15_10_IRQn
#define NP2_D11_EXTI_IRQ_HANDLER	EXTI15_10_IRQHandler

#define NP2_D12						14
#define NP2_D12_PIN					GPIO_Pin_14
#define NP2_D12_PINSOURCE			GPIO_PinSource14
#define NP2_D12_GPIO_PORT			GPIOB
#define NP2_D12_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define NP2_D12_EXTI_PORT			EXTI_PortSourceGPIOB
#define NP2_D12_EXTI_SOURCE			EXTI_PinSource14
#define NP2_D12_EXTI_LINE			EXTI_Line14
#define NP2_D12_EXTI_IRQ			EXTI15_10_IRQn
#define NP2_D12_EXTI_IRQ_HANDLER	EXTI15_10_IRQHandler

#define NP2_D13						13	
#define NP2_D13_PIN					GPIO_Pin_13
#define NP2_D13_PINSOURCE			GPIO_PinSource13
#define NP2_D13_GPIO_PORT			GPIOB
#define NP2_D13_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define NP2_D13_EXTI_PORT			EXTI_PortSourceGPIOB
#define NP2_D13_EXTI_SOURCE			EXTI_PinSource13
#define NP2_D13_EXTI_LINE			EXTI_Line13
#define NP2_D13_EXTI_IRQ			EXTI15_10_IRQn

#define NP2_CTRL_OF_PWR_HEADERS	GPIO_Pin_2
#define NP2_CTRL_OF_PWR_HEADERS_GPIO_PORT	GPIOB
#define NP2_CTRL_OF_PWR_HEADERS_GPIO_CLK	RCC_AHB1Periph_GPIOB
#define NP2_PWR_CTRL_ETHERNET

//NP2 I2C SDA/SCL PIN Definitions
#define NP2_I2C				I2C1
#define NP2_I2C_CLK         RCC_APB1Periph_I2C1

#define NP2_SDA_PIN			GPIO_Pin_7
#define NP2_SDA_PINSOURCE	GPIO_PinSource7
#define NP2_SDA_GPIO_PORT	GPIOB
#define NP2_SDA_GPIO_CLK	RCC_AHB1Periph_GPIOB
#define NP2_SDA_AF          GPIO_AF_I2C1

#define NP2_SCL_PIN			GPIO_Pin_6
#define NP2_SCL_PINSOURCE	GPIO_PinSource6
#define NP2_SCL_GPIO_PORT	GPIOB
#define NP2_SCL_GPIO_CLK	RCC_AHB1Periph_GPIOB
#define NP2_SCL_AF          GPIO_AF_I2C1

//NP2 debug printf uart definitions
#define NP2_DEBUG_UART				UART4
#define NP2_DEBUG_UART_BAUDRATE		115200
#define NP2_DEBUG_UART_CLK			RCC_APB1Periph_UART4
#define NP2_DEBUG_UART_AF			GPIO_AF_UART4
#define NP2_DEBUG_UART_TX_PIN		GPIO_Pin_0
#define NP2_DEBUG_UART_TX_PINSOURCE	GPIO_PinSource0
#define NP2_DEBUG_UART_GPIO_PORT	GPIOA
#define NP2_DEBUG_UART_GPIO_CLK		RCC_AHB1Periph_GPIOA

//NP2 SPI definitions
#define NP2_SPI                       SPI2
#define NP2_SPI_CLK                   RCC_APB1Periph_SPI2

#define NP2_SPI_SCK_PIN               GPIO_Pin_13                 /* PC.13 */
#define NP2_SPI_SCK_GPIO_PORT         GPIOB                       /* GPIOB */
#define NP2_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define NP2_SPI_SCK_SOURCE            GPIO_PinSource13
#define NP2_SPI_SCK_AF                GPIO_AF_SPI2

#define NP2_SPI_MISO_PIN              GPIO_Pin_14                 /* PC.14 */
#define NP2_SPI_MISO_GPIO_PORT        GPIOB                       /* GPIOB */
#define NP2_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define NP2_SPI_MISO_SOURCE           GPIO_PinSource14
#define NP2_SPI_MISO_AF               GPIO_AF_SPI2

#define NP2_SPI_MOSI_PIN              GPIO_Pin_15                 /* PC.15 */
#define NP2_SPI_MOSI_GPIO_PORT        GPIOB                       /* GPIOB */
#define NP2_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define NP2_SPI_MOSI_SOURCE           GPIO_PinSource15
#define NP2_SPI_MOSI_AF               GPIO_AF_SPI2

#define NP2_SPI_CS_PIN                GPIO_Pin_10                 /* PB.10 */
#define NP2_SPI_CS_GPIO_PORT          GPIOB                       /* GPIOB */
#define NP2_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOB

//NP2 Micro SD Card definitions
#define NP2_MMC_SPI                       SPI3
#define NP2_MMC_SPI_CLK                   RCC_APB1Periph_SPI3

#define NP2_MMC_SPI_SCK_PIN               GPIO_Pin_10                 /* PC.10 */
#define NP2_MMC_SPI_SCK_GPIO_PORT         GPIOC                       
#define NP2_MMC_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOC
#define NP2_MMC_SPI_SCK_SOURCE            GPIO_PinSource10
#define NP2_MMC_SPI_SCK_AF                GPIO_AF_SPI3

#define NP2_MMC_SPI_MISO_PIN              GPIO_Pin_11                 /* PC.11 */
#define NP2_MMC_SPI_MISO_GPIO_PORT        GPIOC                       
#define NP2_MMC_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOC
#define NP2_MMC_SPI_MISO_SOURCE           GPIO_PinSource11
#define NP2_MMC_SPI_MISO_AF               GPIO_AF_SPI3

#define NP2_MMC_SPI_MOSI_PIN              GPIO_Pin_12                 /* PC.12 */
#define NP2_MMC_SPI_MOSI_GPIO_PORT        GPIOC                      
#define NP2_MMC_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOC
#define NP2_MMC_SPI_MOSI_SOURCE           GPIO_PinSource12
#define NP2_MMC_SPI_MOSI_AF               GPIO_AF_SPI3

#define NP2_MMC_SPI_CS_PIN                GPIO_Pin_0                  /* PB.00 */
#define NP2_MMC_SPI_CS_GPIO_PORT          GPIOB                       
#define NP2_MMC_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOB

#define NP2_MMC_PWR_PIN                	  GPIO_Pin_1                  /* PB.01 */
#define NP2_MMC_PWR_GPIO_PORT          	  GPIOB                       
#define NP2_MMC_PWR_GPIO_CLK           	  RCC_AHB1Periph_GPIOB

#define NP2_MMC_SPI_INT1_PIN              GPIO_Pin_5                  /* PB.05 */
#define NP2_MMC_SPI_INT1_GPIO_PORT        GPIOB                       
#define NP2_MMC_SPI_INT1_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define NP2_MMC_SPI_INT1_EXTI_LINE        EXTI_Line5
#define NP2_MMC_SPI_INT1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOB
#define NP2_MMC_SPI_INT1_EXTI_PIN_SOURCE  EXTI_PinSource5
#define NP2_MMC_SPI_INT1_EXTI_IRQn        EXTI5_IRQn 

//NP2 ENC28J60 Ethernet definitions
#define NP2_ENC_SPI                       SPI1
#define NP2_ENC_SPI_CLK                   RCC_APB2Periph_SPI1

#define NP2_ENC_SPI_SCK_PIN               GPIO_Pin_5                 /* PC.05 */
#define NP2_ENC_SPI_SCK_GPIO_PORT         GPIOA                       
#define NP2_ENC_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define NP2_ENC_SPI_SCK_SOURCE            GPIO_PinSource5
#define NP2_ENC_SPI_SCK_AF                GPIO_AF_SPI1

#define NP2_ENC_SPI_MISO_PIN              GPIO_Pin_4                 /* PB.04 */
#define NP2_ENC_SPI_MISO_GPIO_PORT        GPIOB                      
#define NP2_ENC_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define NP2_ENC_SPI_MISO_SOURCE           GPIO_PinSource4
#define NP2_ENC_SPI_MISO_AF               GPIO_AF_SPI1

#define NP2_ENC_SPI_MOSI_PIN              GPIO_Pin_7                 /* PC.07 */
#define NP2_ENC_SPI_MOSI_GPIO_PORT        GPIOA                      
#define NP2_ENC_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define NP2_ENC_SPI_MOSI_SOURCE           GPIO_PinSource7
#define NP2_ENC_SPI_MOSI_AF               GPIO_AF_SPI1

#define NP2_ENC_SPI_CS_PIN                GPIO_Pin_8                  /* PC.08 */
#define NP2_ENC_SPI_CS_GPIO_PORT          GPIOC                       
#define NP2_ENC_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOC

//Ethernet Power
#define NP2_ENC_PWR_PIN                	  GPIO_Pin_15                  /* PC.15 */
#define NP2_ENC_PWR_GPIO_PORT          	  GPIOC                      
#define NP2_ENC_PWR_GPIO_CLK           	  RCC_AHB1Periph_GPIOC

//Ethernet Reset
#define NP2_ENC_RST_PIN                	  GPIO_Pin_2                  /* PD.02 */
#define NP2_ENC_RST_GPIO_PORT          	  GPIOD                      
#define NP2_ENC_RST_GPIO_CLK           	  RCC_AHB1Periph_GPIOD

//Ethernet Clock input
#define NP2_ENC_CLK_PIN                	  GPIO_Pin_8                  /* PA.08 */
#define NP2_ENC_CLK_GPIO_PORT          	  GPIOA                      
#define NP2_ENC_CLK_GPIO_CLK           	  RCC_AHB1Periph_GPIOA
#define NP2_ENC_CLK_SOURCE		          GPIO_PinSource8
#define NP2_ENC_CLK_AF               	  GPIO_AF_MCO


//Ethernet Interrupt
#define NP2_ENC_SPI_INT_PIN              GPIO_Pin_4                  /* PA.04 */
#define NP2_ENC_SPI_INT_GPIO_PORT        GPIOA                       
#define NP2_ENC_SPI_INT_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define NP2_ENC_SPI_INT_EXTI_LINE        EXTI_Line4
#define NP2_ENC_SPI_INT_EXTI_PORT_SOURCE EXTI_PortSourceGPIOA
#define NP2_ENC_SPI_INT_EXTI_PIN_SOURCE  EXTI_PinSource4
#define NP2_ENC_SPI_INT_EXTI_IRQn        EXTI4_IRQn 


/** @addtogroup STM32F4_NETDUINOPLUS2_LOW_LEVEL_LED
  * @{
  */
//#define LEDn                             1	//4
#define NP2_LED_BLUE					10
#define NP2_LED_BLUE_PIN                GPIO_Pin_10	//12
#define NP2_LED_BLUE_GPIO_PORT          GPIOA		//D
#define NP2_LED_BLUE_GPIO_CLK           RCC_AHB1Periph_GPIOA	//D  
   
/**
 * @brief push-button
 */
#define NP2_PB_PIN                	GPIO_Pin_11
#define NP2_PB_GPIO_PORT          	GPIOB
#define NP2_PB_GPIO_CLK           	RCC_AHB1Periph_GPIOB
#define NP2_PB_EXTI_LINE          	EXTI_Line11
#define NP2_PB_EXTI_PORT   			EXTI_PortSourceGPIOB
#define NP2_PB_EXTI_SOURCE    		EXTI_PinSource11
#define NP2_PB_EXTI_IRQ          	EXTI15_10_IRQn
#define NP2_PB_EXTI_IRQ_HANDLER		EXTI15_10_IRQHandler 


//Temperature Sensor
#define NP2_TEMP_ADC_CHAN		ADC_Channel_18	//NOTE only ADC 1,2 & 3 use
#define NP2_VBATT_ADC_CHAN		ADC_Channel_18	//NOTE only ADC 1,2 & 3 use

void NP2_boardinit();

/** @defgroup NETDUINOPLUS2_LOW_LEVEL_Exported_Functions
  * @{
  */
void NP2_LEDInit();
void NP2_LEDOn();
void NP2_LEDOff();
void NP2_LEDToggle();

  
#ifdef __cplusplus
}
#endif

#endif /* __NETDUINOPLUS2_H */


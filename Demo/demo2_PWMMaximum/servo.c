/**
  ******************************************************************************
  * @file    * /servo.c 
  * @author  Jordan Yates
  * @date    8-March-2014
  * @brief   Servo control functions for Hextronic HXT900 servos on predesignated
  *			 pins.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "netduinoplus2.h"
#include "stm32f4xx_conf.h"
#include "debug_printf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PAN_PIN			NP2_D2_PIN
#define PAN_PINSOURCE	NP2_D2_PINSOURCE
#define PAN_PORT		NP2_D2_GPIO_PORT
#define PAN_CLOCK		NP2_D2_GPIO_CLK
#define TILT_PIN		NP2_D3_PIN
#define TILT_PINSOURCE	NP2_D3_PINSOURCE
#define TILT_PORT		NP2_D3_GPIO_PORT
#define TILT_CLOCK		NP2_D3_GPIO_CLK

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int8_t pan_angle;
int8_t tilt_angle;
/* Private function prototypes -----------------------------------------------*/
uint16_t Pan_Get_Pulse_Count(int8_t angle);
uint16_t Tilt_Get_Pulse_Count(int8_t angle);

/**
  * @brief  Initialise pan servo on pin D10, set initial angle to 0 degrees.
  * @param  None
  * @retval None
  */
void Servo_Pan_Init(void) {

	uint16_t PrescalerValue = 0;

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

  	/* Pan timer enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  	/* Enable the Pan Pin Clock */
  	RCC_AHB1PeriphClockCmd(PAN_CLOCK, ENABLE);

  	/* Configure the Pan Pin */
  	GPIO_InitStructure.GPIO_Pin = PAN_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  	GPIO_Init(PAN_PORT, &GPIO_InitStructure); 
  
  	/* Connect Timer output to Pan pin */  
  	GPIO_PinAFConfig(PAN_PORT, PAN_PINSOURCE, GPIO_AF_TIM2);

	/* Compute the prescaler value. SystemCoreClock = 168000000 - set for 500Khz clock */
  	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 500000) - 1;

  	/* Time 11 mode and prescaler configuration */
  	TIM_TimeBaseStructure.TIM_Period = 500000/50; 	//Set for 20ms (50Hz) period
  	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	/* Configure Timer 2 mode and prescaler */
  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* PWM2 Mode configuration: Channel4 - set pulse width*/
  	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_Pulse = Pan_Get_Pulse_Count(0);
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	/* Enable Output compare channel 4 */
  	TIM_OC4Init(TIM2, &TIM_OCInitStructure);

  	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

  	/* TIM2 enable counter */
 	TIM_Cmd(TIM2, ENABLE); 
}

/**
  * @brief  Sets PWM output to achieve the specified angle of the pan servo,
  *				limited to between -85 and 85 degrees.
  * @param  angle: specifies the angle of rotation (degrees).
  * @retval None
  */
void Servo_Pan_Set_Angle(int8_t angle) {
	pan_angle = angle;
  // Comment out the safe guards
	//pan_angle = (pan_angle > 85)? 85: pan_angle;
	//pan_angle = (pan_angle < -85)? -85: pan_angle;
	TIM_SetCompare4(TIM2, Pan_Get_Pulse_Count(pan_angle));
	//debug_printf("Pan:%3d Tilt:%3d\n", pan_angle, tilt_angle);
}

/**
  * @brief  Sets PWM output to achieve the specified angle of the pan servo 
  * @param  degree: specifies how much to increment angle by (degrees).
  * @retval None
  */
void Servo_Pan_Increment_Angle(int8_t angle) {
	pan_angle += angle;
	pan_angle = (pan_angle > 85)? 85: pan_angle;
	pan_angle = (pan_angle < -85)? -85: pan_angle;
	TIM_SetCompare4(TIM2, Pan_Get_Pulse_Count(pan_angle));
	//debug_printf("Pan:%3d Tilt:%3d\n", pan_angle, tilt_angle);
}

/**
  * @brief  Retrieves current angle of the pan servo 
  * @param  None
  * @retval Current pan servo angle
  */
int8_t Servo_Get_Pan_Angle(void) {
	return pan_angle;
}

/**
  * @brief  Initialise pan servo on pin D9, set initial angle to 0 degrees.
  * @param  None
  * @retval None
  */
void Servo_Tilt_Init(void) {

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

  	/* Enable the Tilt Clock */
  	RCC_AHB1PeriphClockCmd(TILT_CLOCK, ENABLE);

  	/* Configure the Tilt pin */
  	GPIO_InitStructure.GPIO_Pin = TILT_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  	GPIO_Init(TILT_PORT, &GPIO_InitStructure); 

  	/* Connect TIM2 output to Tilt pin */  
  	GPIO_PinAFConfig(TILT_PORT, TILT_PINSOURCE, GPIO_AF_TIM2);

	/* PWM1 Mode configuration: Channel3 - set pulse width*/
  	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_Pulse = Tilt_Get_Pulse_Count(0);
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	/* Enable Output compare channel 3 */
  	TIM_OC3Init(TIM2, &TIM_OCInitStructure);

  	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
}

/**
  * @brief  Sets PWM output to achieve the specified angle of the tilt servo,
  *				limited to between -85 and 85 degrees.
  * @param  angle: specifies the angle of rotation (degrees).
  * @retval None
  */
void Servo_Tilt_Set_Angle(int8_t angle) {
	tilt_angle = angle;
  // Comment out the safe guards
	//tilt_angle = (tilt_angle > 85)? 85: tilt_angle;
	//tilt_angle = (tilt_angle < -85)? -85: tilt_angle;
	TIM_SetCompare3(TIM2, Tilt_Get_Pulse_Count(tilt_angle));
	//debug_printf("Pan:%3d Tilt:%3d\n", pan_angle, tilt_angle);
}

/**
  * @brief  Sets PWM output to achieve the specified angle of the pan servo 
  * @param  degree: specifies how much to increment angle by (degrees).
  * @retval None
  */
void Servo_Tilt_Increment_Angle(int8_t angle) {
	tilt_angle += angle;
	tilt_angle = (tilt_angle > 85)? 85: tilt_angle;
	tilt_angle = (tilt_angle < -85)? -85: tilt_angle;
	TIM_SetCompare3(TIM2, Tilt_Get_Pulse_Count(tilt_angle));
	//debug_printf("Pan:%3d Tilt:%3d\n", pan_angle, tilt_angle);
}

/**
  * @brief  Retrieves current angle of the tilt servo 
  * @param  None
  * @retval Current tilt servo angle
  */
int8_t Servo_Get_Tilt_Angle(void) {
	return tilt_angle;
}

/**
  * @brief  Controls servo based on console input 
  * @param  console_rx: console input
  * @retval None
  */
void Servo_Console_Control(char console_rx) {
	switch(console_rx) {
		case 'w':
			Servo_Tilt_Increment_Angle(1);
			break;
		case 'a':
			Servo_Pan_Increment_Angle(1);
			break;
		case 's':
			Servo_Tilt_Increment_Angle(-1);
			break;
		case 'd':
			Servo_Pan_Increment_Angle(-1);
			break;
		default:
			return;
	}
}

/**
  * @brief  Controls servo based on radio message
  * @param  char*: payload, radio message
  * @retval None
  */
void Servo_Radio_Control(char* payload) {
	uint8_t direction;
	if (payload[1] == '+') {
		direction = 1;
	} else {
		direction = -1;
	}
	if (payload[0] == 'P') {
		Servo_Pan_Increment_Angle(direction);
	} else {
		Servo_Tilt_Increment_Angle(direction);
	}
}

/**
  * @brief  Converts the angle required for servo into tick count for PWM OCR. 
  * @param  degree:specifies the degree of rotation (-85 <= degree <= 85)
  * @retval Tick count for the PWM OCR
  */
uint16_t Pan_Get_Pulse_Count(int8_t angle) {
	return (uint16_t)((angle + 90)*11.11 + 500)/2;
}

/**
  * @brief  Converts the angle required for servo into tick count for PWM OCR. 
  * @param  degree:specifies the degree of rotation (-85 <= degree <= 85)
  * @retval Tick count for the PWM OCR
  */
uint16_t Tilt_Get_Pulse_Count(int8_t angle) {
	return (uint16_t)((angle + 90)*10 + 500)/2;
}

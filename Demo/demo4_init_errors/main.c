#include "netduinoplus2.h"
#include "stm32f4xx_conf.h"
#include "debug_printf.h"

void Delay(__IO unsigned long nCount);
void hardware_init();

int main(void) {
	unsigned int adc_value;

	NP2_boardinit();	//Initialise the NP2 board.
	hardware_init();	//Initialise Hardware peripherals
	
	/* Main processing loop performing ADC conversion every 1s. */
    while (1) {
  
		ADC_SoftwareStartConv(ADC1);	//Perform ADC conversions 	
			
		/* Wait for ADC conversion to finish by polling the ADC Over Flag. */
		/* Works */
		while ((ADC_GetFlagStatus(ADC1, ADC_FLAG_OVR) != RESET) && (ADC_GetFlagStatus(ADC1, ADC_FLAG_OVR) != RESET));
		/* Doesn't work */
		while ((ADC_GetFlagStatus(ADC1, ADC_FLAG_OVR) != SET) && (ADC_GetFlagStatus(ADC1, ADC_FLAG_OVR) != SET));

		/* Extract ADC conversion values */
		adc_value = ADC_GetConversionValue(ADC1);

		/* Print ADC conversion values */
		debug_printf("ADC Value: %x - CR1:%X CR2:%X JOFR_1:%X\n\r", adc_value, ADC1->CR1, ADC1->CR2, ADC1->JOFR1);
		ADC_ClearFlag(ADC1, ADC_FLAG_OVR);

		NP2_LEDToggle();	//Toggle LED on/off
		Delay(0xFFFFFF);
		 
  	}
}

void hardware_init() {

	GPIO_InitTypeDef GPIO_InitStructure;	
	ADC_InitTypeDef ADC_InitStructure;
  	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	NP2_LEDInit();		//Initialise Blue LED
	NP2_LEDOff();		//Turn off Blue LED

	/* Configure A0 as analog input */
  	GPIO_InitStructure.GPIO_Pin = NP2_A0_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  	GPIO_Init(NP2_A0_GPIO_PORT, &GPIO_InitStructure);

	/* Enable clock for ADC 1 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* ADC Common Init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC Specific Init for 12Bit resolution and continuous sampling */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;

	/* Works */
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	/* Doesn't work */
	//ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = 0;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;

	/* Works */
	ADC_Init(ADC1, &ADC_InitStructure);

	/* Configure ADC1 to connect to the NP2 A0 channel */
	ADC_RegularChannelConfig(ADC1, NP2_A0_ADC_CHAN, 1, ADC_SampleTime_3Cycles);

  	/* Enable ADC1 */
  	ADC_Cmd(ADC1, ENABLE);
}

void Delay(__IO unsigned long nCount) {
  	while(nCount--) {
  	}
}


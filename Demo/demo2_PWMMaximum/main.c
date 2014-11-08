#include "netduinoplus2.h"
#include "stm32f4xx_conf.h"
#include "debug_printf.h"
#include <stdio.h>
#include "servo.h"

void Hardware_init(void);
void Delay(__IO unsigned long nCount);


void main(void)    {
    char output[11] = "Hello World";
    int numbers[] = {1, 2, 3, 4};
    int *numbers_point = numbers;
    int i;

    NP2_boardinit();
    Hardware_init();
    Servo_Pan_Init();
    Servo_Tilt_Init();

    debug_printf("Starting...\n");

    debug_printf("Setting Correct Values\n");
    Servo_Pan_Set_Angle(40);
    Servo_Tilt_Set_Angle(60);

    Delay(0x30000000);

    debug_printf("Setting Incorrect Values\n");
    Servo_Pan_Set_Angle(-90);
    Servo_Tilt_Set_Angle(100);
}

void Hardware_init(void) {
    NP2_LEDInit();
    NP2_LEDOff();
}

void Delay(__IO unsigned long nCount) {

    while(nCount--) {
    }   
}

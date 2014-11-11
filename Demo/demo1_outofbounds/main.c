#include "netduinoplus2.h"
#include "stm32f4xx_conf.h"
#include "debug_printf.h"
#include <stdio.h>

void Hardware_init(void);
void Delay(__IO unsigned long nCount);


void main(void)    {
    char output[11] = "Hello World";
    int numbers[] = {1, 2, 3, 4};
    int *numbers_point = numbers;
    int i;

    NP2_boardinit();
    Hardware_init();

    while (1) {
        /* Out of bounds access - example 1 */
        debug_printf("\nExample 1\n");
        for (i = 0; i < 12; i++) {
            debug_printf("i: %d, val: %c\n", i, output[i]);
            NP2_LEDToggle();
        }

        /* Out of bounds access - example 2 */
        debug_printf("\nExample 2\n");
        for (i = 0; i < 5; i++) {
            debug_printf("i: %d, val: %d\n", i, numbers_point[i]);
            NP2_LEDToggle();
        }

        numbers_point = NULL;
        i = 0;

        /* Out of bounds access - example 3 */
        debug_printf("\nExample 3\n");
        debug_printf("val: %d\n", i, numbers_point);

        NP2_LEDToggle();
        Delay(0x7FFF00);    //Delay 1s.
        numbers_point = numbers;
    }
}

void Hardware_init(void) {
    NP2_LEDInit();
    NP2_LEDOff();
}

void Delay(__IO unsigned long nCount) {

    while(nCount--) {
    }
}

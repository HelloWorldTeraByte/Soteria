#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "stm32f413xx.h"
#include "sim808.h"
#include "delay.h"
#include "jwt.h"


int main(void)
{    
    GSM_init();

    /*Enable clock for the LEDs*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    /*Setup up LED on PB7*/
    GPIOB->MODER |= GPIO_MODER_MODER7_0;
    GPIOB->MODER &= ~(GPIO_MODER_MODER7_1);
    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_7);
    GPIOB->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR7_0);
    GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR7_1);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR7);

    /*Setup up LED on PB14*/
    GPIOB->MODER |= GPIO_MODER_MODER14_0;
    GPIOB->MODER &= ~(GPIO_MODER_MODER14_1);
    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_14);
    GPIOB->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR14_0);
    GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR14_1);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR14);

    /*GSM_sms_snd("+642108568818", pb64);*/

    jwt_create();

    while(1) {
        __asm__("NOP");
    }

    return 0;
}

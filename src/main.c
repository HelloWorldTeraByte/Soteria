#include <stdint.h>
#include <string.h>

#include "stm32f413xx.h"
#include "sim808.h"

void wait_unprecise(int moment)
{
    volatile uint32_t i, j;
    for(i = 0; i < moment; i++) {
        j++;
    }
}

int main(void)
{    
    /*Enable clock for GPIO on Port B and Port E*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;

    /*Enable clock for UART10 on Port E*/
    RCC->APB2ENR |= RCC_APB2ENR_UART10EN;

    setup_usart();

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

    char rx_buffer[256] = {0};

    /*usart_snd_str("ATE0\r\n");*/
    /*usart_recv_str(rx_buffer);*/
    /*wait_unprecise(200000);*/

    usart_snd_str("AT\r");
    GPIOB->BSRR |= GPIO_BSRR_BS_7;
    usart_recv_str(rx_buffer);


    if(rx_buffer[0] == 'O')
        GPIOB->BSRR |= GPIO_BSRR_BS_7;
    else
        GPIOB->BSRR |= GPIO_BSRR_BS_14;


    /*if(strncmp(rx_buffer, "OK\r", strlen("OK\r")) == 0)*/
        /*GPIOB->BSRR |= GPIO_BSRR_BS_7;*/
    /*else*/
        /*GPIOB->BSRR |= GPIO_BSRR_BS_14;*/

    wait_unprecise(200000);


    /*usart_snd_str("AT+CMGF=1\r");*/
    /*wait_unprecise(200000);*/

    /*usart_snd_str("AT+CMGS=\"+642108568818\"\rYeeeet");*/
    /*wait_unprecise(200000);*/
    /*usart_snd_char(0x1A);*/

    while(1) {
        __asm__("NOP");

    }
}

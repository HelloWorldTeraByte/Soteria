#define F_BUS_USART 16000000
#define USART_BAUD 9600

#include "stm32f413xx.h"

void wait_unprecise(int moment)
{
    volatile int i, j;
    for(i = 0; i < moment; i++) {
        j++;
    }
}

void setup_clock(void)
{    
    /*Enable clock for GPIO on Port B and Port E*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;

    /*Enable clock for UART10 on Port E*/
    RCC->APB2ENR |= RCC_APB2ENR_UART10EN;
}

void setup_usart(void)
{
    /*UART10 RX*/
    GPIOE->MODER |= GPIO_MODER_MODER2_1;
    GPIOE->MODER &= ~(GPIO_MODER_MODER2_0);
    GPIOE->OTYPER |= GPIO_OTYPER_OT_2;
    GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2;
    GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPDR2);

    /*Alternate mode for RX pin 3(1011)*/
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL2_0;
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL2_1;
    GPIOE->AFR[0] &= ~(GPIO_AFRL_AFSEL2_2);
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL2_3;

    /*UART10 TX*/
    GPIOE->MODER |= GPIO_MODER_MODER3_1;
    GPIOE->MODER &= ~(GPIO_MODER_MODER3_0);
    GPIOE->OTYPER |= GPIO_OTYPER_OT_3;
    GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_0;
    GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3_1;
    GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPDR3_0);
    GPIOE->PUPDR &= ~(GPIO_PUPDR_PUPDR3_1);

    /*Alternate mode for TX pin 3(1011)*/
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL3_0;
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL3_1;
    GPIOE->AFR[0] &= ~(GPIO_AFRL_AFSEL3_2);
    GPIOE->AFR[0] |= GPIO_AFRL_AFSEL3_3;

    /*Enable the USART*/
    UART10->CR1 |= USART_CR1_UE;

    /*Word length of 8 bits*/
    UART10->CR1 &= ~(USART_CR1_M);

    /*1 stop bit*/
    UART10->CR2 &= ~(USART_CR2_STOP_0);
    UART10->CR2 &= ~(USART_CR2_STOP_1);

    /*Set baudrate to be 9600*/
    UART10->BRR = F_BUS_USART / USART_BAUD;

    /*Parity disable*/
    UART10->CR1 &= ~(USART_CR1_PCE);

    /*Enable the UART10 transmit and recive*/
    UART10->CR1 |= USART_CR1_TE;
    UART10->CR1 |= USART_CR1_RE;

}

void usart_send_char(uint8_t data)
{
    while((UART10->SR & USART_SR_TXE) == 0)
        __asm__("NOP");
    UART10->DR = data;
}

void usart_send_str(char *str)
{
    char *tmp;
    for(tmp = str; *tmp != '\0'; tmp++) {
        switch(*tmp) {
            case '\r':
                usart_send_char(0x0D);
            default:
                usart_send_char(*tmp);
        }
    }
}

int main(void)
{
    setup_clock();
    setup_usart();

    /*Setup up LED on PB7*/
    GPIOB->MODER |= GPIO_MODER_MODER7_0;
    GPIOB->MODER &= ~(GPIO_MODER_MODER7_1);
    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_7);
    GPIOB->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR7_0);
    GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR7_1);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR7);

    usart_send_str("AT\r");
    wait_unprecise(200000);
    
    usart_send_str("AT+CMGF=1\r");
    wait_unprecise(200000);

    usart_send_str("AT+CMGS=\"+642108568818\"\rWhats up!!");
    wait_unprecise(200000);
    usart_send_char(0x1A);

    while(1) {
        GPIOB->BSRR |= GPIO_BSRR_BS_7;
        wait_unprecise(300000);
        GPIOB->BSRR |= GPIO_BSRR_BR_7;
        wait_unprecise(300000);
    }
}

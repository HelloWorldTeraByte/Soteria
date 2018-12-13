#include <stdint.h>
#include <string.h>

#include "stm32f4xx.h"
#include "sim808.h"
 
void setup_usart(void)
{
    /*memset(rx_buffer, 0, sizeof(rx_buffer));*/
    /*rx_buffer_i = 0;*/

    /*Enable the interrupts for the USART*/
    /*NVIC_EnableIRQ(UART10_IRQn); */

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

    /*Enable the interupt for recived data*/
    /*UART10->CR1 |= USART_CR1_RXNEIE;*/

}

void usart_snd_wait(void)
{
    while((UART10->SR & USART_SR_TXE) == 0)
        __asm__("NOP");
}

void usart_recv_wait(void)
{
    while((UART10->SR & USART_SR_RXNE) == 0)
        __asm__("NOP");
}

void usart_snd_char(uint16_t data)
{
    usart_snd_wait();
    UART10->DR = data;
}

uint16_t usart_recv_char(void)
{
    usart_recv_wait();
    uint16_t data = UART10->DR;
    return data;
}

void usart_recv_str(char *buff)
{
    uint8_t cnt = 0;
    buff[cnt] = usart_recv_char();
    cnt++;
    buff[cnt] = usart_recv_char();
    cnt++;
    buff[cnt] = usart_recv_char();

    while(buff[cnt] != '\n') {
        cnt++;
        if(cnt > (RX_BUFF_SIZE -1))
            break;
        buff[cnt] = usart_recv_char();
    }
}

void usart_snd_str(char *str)
{
    char *tmp;
    for(tmp = str; *tmp != '\0'; tmp++) {
        switch(*tmp) {
            case '\r':
                usart_snd_char(0x0D);
            case '\n':
                usart_snd_char(0x0A);
            default:
                usart_snd_char(*tmp);
        }
    }
}

#ifndef __SIM808__H
#define __SIM808__H

#define F_BUS_USART 16000000
#define USART_BAUD 9600

#define RX_BUFF_SIZE 256

void UART10_IRQHandler(void);

void setup_usart(void);

void usart_snd_wait(void);
void usart_recv_wait(void);

void usart_snd_char(uint16_t data);
uint16_t usart_recv_char(void);

void usart_snd_str(char *str);
void usart_recv_str(char *buff);

#endif

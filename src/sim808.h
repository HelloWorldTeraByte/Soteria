#ifndef __SIM808__H
#define __SIM808__H

#define F_BUS_USART 16000000
#define USART_BAUD 9600

void UART10_IRQHandler(void);

void setup_usart(void);

void usart_snd_wait(void);
void usart_recv_wait(void);

void usart_snd_char(uint8_t data);
char usart_recv_char();

void usart_snd_str(char *str);

#endif

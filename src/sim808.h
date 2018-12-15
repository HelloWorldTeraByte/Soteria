#ifndef __SIM808__H
#define __SIM808__H

#define F_BUS_USART 16000000
#define USART_BAUD 9600
#define GSM_PORT GPIOE
#define GSM_USART UART10

#define RX_BUFF_SIZE 256

void GSM_init(void);

void GSM_snd_wait(void);
void GSM_recv_wait(void);

void GSM_snd_char(uint16_t data);
uint16_t GSM_recv_char(void);

void GSM_snd_str(char *str);
void GSM_recv_str(char *buff);

int buff_cmp(char *buff, char *msg);

int GSM_sms_snd(char *number, char *msg);

void GSM_iat(char *iat);
void GSM_exp(char *exp);

#endif

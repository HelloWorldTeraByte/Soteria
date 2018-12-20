#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "sim808.h"
#include "delay.h"
#include "jwt.h"

void IOT_authenticate(char *jwt)
{
    char rx_buffer[RX_BUFF_SIZE] = {0};

    GSM_snd_str("ATE0\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CIPMUX=0\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CGATT=1\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CSTT=\"vodafone\",\"\",\"\"\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CIICR\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CIFSR\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CIPSTART=\"TCP\",\"cloudiotdevice.googleapis.com\",\"80\"\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CIPSEND\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    char snd_str[600];
    strcpy(snd_str, "GET /v1/projects/soteria-2020-4ever/locations/us-central1/registries/soteria/devices/b_1/config?local_version=1 HTTP/2\r\n");
    strcat(snd_str, "Host: cloudiotdevice.googleapis.com\r\n");
    strcat(snd_str, "Accept: */*");
    strcat(snd_str, "authorization: Bearer ");
    strcat(snd_str, jwt);
    strcat(snd_str, "\r\n");
    strcat(snd_str, "cache-control: no-cache\r\n");
    strcat(snd_str, "\r\n");

    GSM_snd_str(snd_str);
    GSM_snd_char(0x1A);
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CIPCLOSE\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CIPSHUT\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);


}
void IOT_POST(char *jwt)
{
    char rx_buffer[RX_BUFF_SIZE] = {0};

    GSM_snd_str("ATE0\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CIPMUX=0\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CGATT=1\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CSTT=\"vodafone\",\"\",\"\"\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CIICR\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CIFSR\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CIPSTART=\"TCP\",\"cloudiotdevice.googleapis.com\",\"80\"\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CIPSEND\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    char snd_str[600];
    strcpy(snd_str, "POST /v1/projects/soteria2020/locations/europe-west1/registries/soteria_registry/devices/b_0:publishEvent HTTP/2\r\n");
    strcat(snd_str, "Host: cloudiotdevice.googleapis.com\r\n");
    strcat(snd_str, "Accept: */*");
    strcat(snd_str, "authorization: Bearer ");
    strcat(snd_str, jwt);
    strcat(snd_str, "\r\n");
    strcat(snd_str, "content-type: application/json\r\n");
    strcat(snd_str, "cache-control: no-cache\r\n");
    strcat(snd_str, "Content-Length: 31\r\n");
    strcat(snd_str, "\r\n\r\n");
    strcat(snd_str, "eWVldGljdXM=");

    GSM_snd_str(snd_str);
    GSM_snd_char(0x1A);
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CIPCLOSE\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);

    GSM_snd_str("AT+CIPSHUT\r\n");
    GSM_recv_str(rx_buffer);
    wait_unprecise(10000);


}

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

    /* GSM_sms_snd("+642108568818", "asfasdf"); */

    char *jwt = (char*)malloc(1);
    jwt_create(&jwt);
    GPIOB->BSRR |= GPIO_BSRR_BS_7;
    /* IOT_authenticate(jwt); */
    IOT_POST(jwt);

    free(jwt);

    while(1) {
        GPIOB->BSRR |= GPIO_BSRR_BS_7;
        wait_unprecise(200000);
        GPIOB->BSRR |= GPIO_BSRR_BR_7;
        wait_unprecise(200000);
    }

    return 0;
}

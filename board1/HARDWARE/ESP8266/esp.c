#include "esp.h"
#include "delay.h"
#include "usart.h"

void esp_init(void)
{
	printf("AT+CWJAP=\"wifi\",\"passward\"\r\n");
	delay_ms(5000);
	printf("AT+CIPMUX=0\r\n");
	while(USART_ReceiveData(USART1));
}

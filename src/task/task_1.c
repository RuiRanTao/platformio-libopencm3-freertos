/***********official***********/
#include "FreeRTOS.h"
#include "task.h"
#include <libopencm3/stm32/usart.h>
/*****************************/
#include "task_.h"
#include "uart.h"

unsigned char rc = 0;
void fun_task_1(void)
{
	usart_wait_recv_ready(USART1);
	rc = (unsigned char)usart_recv(USART1);
	// vTaskDelay(pdMS_TO_TICKS(10));
	usart_wait_send_ready(USART1);
	usart_send_blocking(USART1,rc);
	if('\r' == rc)
	usart_send_blocking(USART1,'\n');
	// vTaskDelay(pdMS_TO_TICKS(500));
}
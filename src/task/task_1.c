/***********official***********/
#include "FreeRTOS.h"
#include "task.h"
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>

/*****************************/
#include "task_.h"
#include "uart.h"
#include "boot.h"
#include "at24c02.h"

void fun_task_1(void)
{
	vTaskDelay(pdMS_TO_TICKS(500));
}
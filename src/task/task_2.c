/***********official***********/
#include "FreeRTOS.h"
#include "task.h"
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/stm32/gpio.h>
/*****************************/
#include "spi.h"
#include "gpio.h"
#include "at24c02.h"
#include "ee_iic.h"
#include "task_.h"
#include "adc.h"
#include "stdio.h"



void fun_task_2(void)
{
    gpio_toggle(GPIOA ,GPIO1);
	// SPI_Flash_Read(readdata,FLASH_SIZE-100,array_count);				//从倒数第100个地址处开始,读出SIZE个字节
	// printf("%s\n", readdata);
	iwdg_reset();
	// AT24C02_iic_test();
	// deQueue( &front, rear);  printf("deQueue\n"); print_Link(H,front,rear);

	uint16_t input_adc0 = read_adc_naiive(2);
	uint16_t target = input_adc0 / 2;
	uint16_t input_adc1 = read_adc_naiive(3);
	printf(" %u, %d, %d\n",
		 input_adc0, target, input_adc1);

	vTaskDelay(pdMS_TO_TICKS(800));
}
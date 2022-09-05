/***********official***********/
#include "FreeRTOS.h"
#include "task.h"
#include <libopencm3/stm32/iwdg.h>
/*****************************/
#include "spi.h"
#include "at24c02.h"
#include "ee_iic.h"
#include "task_.h"



void fun_task_2(void)
{
    // gpio_toggle(GPIOA ,GPIO1);
	// SPI_Flash_Read(readdata,FLASH_SIZE-100,array_count);				//从倒数第100个地址处开始,读出SIZE个字节
	// printf("%s\n", readdata);
	iwdg_reset();
	// AT24C02_iic_test();
	// deQueue( &front, rear);  printf("deQueue\n"); print_Link(H,front,rear);
	vTaskDelay(pdMS_TO_TICKS(100));
}
/***********official***********/
#include "FreeRTOS.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/stm32/timer.h> 
#include <libopencm3/stm32/adc.h> 
#include "task.h"
#include<stdlib.h>
#include<stdio.h>
/*****************************/
#include "spi.h"
#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include "stdio.h"
#include "flash.h"
#include "buzzer.h"
#include "at24c02.h"
#include "ee_iic.h"
#include "queue.h"
#include "task_.h"
#include "boot.h"


int i, j = 0, c = 0;

uint16_t id = 0;
uint8_t readdata[18];
uint32_t FLASH_SIZE=2*1024*1024;	//FLASH 大小为2M字节
uint8_t const TEXT_Buffer[]={"MiniSTM32 SPI TEST"};
int array_count = sizeof(TEXT_Buffer) / sizeof(TEXT_Buffer[0]);
int front,rear;
int a[9] = {0,1,2,3,4,5,6,7,8};

LinkList H_link;


/*
* Handler in case our application overflows the stack
*/
void vApplicationStackOverflowHook(
TaskHandle_t xTask __attribute__((unused)),
char *pcTaskName __attribute__((unused))) {
	for (;;);
}

/*
* Task that transmit and receive
*/
static void task1(void *args __attribute__((unused))) {	for (;;) {		fun_task_1();	}
}


/*
* Task that toggles PA1, which is the LED
*/
static void task2(void *args __attribute__((unused))) {	for (;;) {		fun_task_2();	}
}


/*
* Task that 
*/
static void task3(void *args __attribute__((unused))) {	for (;;) { 		fun_task_3();	}
}


/**
* @brief  Toggles the specified GPIO pin
* @param  GPIOx: where x can be (A..G depending on device used) to select the GPIO peripheral
* @param  GPIO_Pin: Specifies the pins to be toggled.
* @retval None
*/


/*
* Main loop, this is where our program starts
*/
int main(void) {
// Setup main clock, using external 8MHz crystal 
rcc_clock_setup_in_hse_8mhz_out_72mhz();

gpio_setup();
usart_setup();
ee_iic_init();
// Set pinmode for PB5 BEEP
gpio_set_mode(
GPIOB,
GPIO_MODE_OUTPUT_50_MHZ,
GPIO_CNF_OUTPUT_PUSHPULL,
GPIO5);
tim3_setup();
timer_disable_counter(TIM3);
spi_setup();
iwdg_set_period_ms(1000);
iwdg_start();
id = SPI_Flash_ReadID();
AT24C02_write_data(str_gotoapp, 0,  3 );

// Turn LED off
gpio_set(GPIOA, GPIO1);
printf("start id: %d\n",id);

H_link = (LNode *)malloc(sizeof(LNode));  //定义一个带有头结点的链表
//设置队头指针和队尾指针，当队列中没有元素时，队头和队尾指向同一块地址
front=rear=0;
printf("front:   %d,   rear:   %d\n", front, rear);
//入队
enQueue( &rear, a);   print_Link(H_link,front,rear);printf("front:   %d,   rear:   %d\n", front, rear);
enQueue( &rear, a);   print_Link(H_link,front,rear);printf("front:   %d,   rear:   %d\n", front, rear);
enQueue( &rear, a);   print_Link(H_link,front,rear);printf("front:   %d,   rear:   %d\n", front, rear);
a[8]=99;
enQueue( &rear, a);    print_Link(H_link,front,rear);printf("front:   %d,   rear:   %d\n", front, rear);
deQueue( &front, rear);   print_Link(H_link,front,rear);printf("front:   %d,   rear:   %d\n", front, rear);
//出队
deQueue( &front, rear);   print_Link(H_link,front,rear);printf("front:   %d,   rear:   %d\n", front, rear);
deQueueall( &front, rear);

print_Link(H_link,front,rear);printf("front:   %d,   rear:   %d\n", front, rear);

// Tell FreeRTOS about our toggle task, and set it's stack and priority
xTaskCreate(task1, "task1", 100, NULL, 2, NULL);
xTaskCreate(task2, "task2", 100, NULL, 2, NULL);
xTaskCreate(task3, "task3", 100, NULL, 2, NULL);

// Start RTOS Task scheduler
vTaskStartScheduler();

// The task scheduler is blocking, so we should never come here...
for (;;);
return 0;
}


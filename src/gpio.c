#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include "gpio.h"



void gpio_setup(void)
{
	/* Set GPIO1 (in GPIO port A) to 'output push-pull'. */
    // Enable clock for GPIO channel A
    rcc_periph_clock_enable(RCC_GPIOA);

    // Set pinmode for PA1
	gpio_set_mode(
		GPIOA,
		GPIO_MODE_OUTPUT_10_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO1);


	/* Set GPIO13 (in GPIO port C) to 'output push-pull'.   nss for spi flash*/
    // Enable clock for GPIO channel C
    rcc_periph_clock_enable(RCC_GPIOC);

    // Set pinmode for PC13
	gpio_set_mode(
		GPIOC,
		GPIO_MODE_OUTPUT_10_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO13);
	gpio_set(GPIOC, GPIO13);



}
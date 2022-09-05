#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include "uart.h"
#include "string.h"
#include <stdlib.h>


/**********重定义函数**********/
#ifdef __GNUC__ 
/* With GCC, small printf (option LD Linker->Libraries->Small printf    set to 'Yes') calls __io_putchar() */ 
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch) 
#else 
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f) 
#endif /* __GNUC__ */ 
PUTCHAR_PROTOTYPE {
   /* Place your implementation of fputc here */ 
  /* e.g. write a character to the USART1 and Loop until the end of transmission */   
//   HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);   
	usart_send_blocking(USART1,ch);
   return ch;
}
__attribute__((weak)) int _write(int file, char *ptr, int len) {
    int DataIdx; 
        for (DataIdx = 0; DataIdx < len; DataIdx++)    
        {
			__io_putchar(*ptr++);    
        } 
        return len; 
}

void usart_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART1);
	AFIO_MAPR |= AFIO_MAPR_USART1_REMAP;

	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      	GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
				GPIO_CNF_INPUT_PULL_UPDOWN,GPIO_USART1_RX);

	/* Setup UART parameters. */
	usart_set_baudrate(USART1, 2000000);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART1);
}

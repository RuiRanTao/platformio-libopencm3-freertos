#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/nvic.h>
#include "uart.h"
#include "string.h"
#include <stdlib.h>
#include<stdio.h>
#include "at24c02.h"
#include "boot.h"


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

	/* Enable the USART1 interrupt. */
	nvic_enable_irq(NVIC_USART1_IRQ);

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

	/* Enable USART1 Receive interrupt. */
	USART_CR1(USART1) |= USART_CR1_RXNEIE;
	/* Finally enable the USART. */
	usart_enable(USART1);
}

extern uint8_t  entery_flag;
unsigned char rc = 0;
uint8_t str_rec [30] ;
uint8_t iter = 0;
void usart1_isr(void)
{
	/* Check if we were called because of RXNE. */
	if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
	    ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {
		/* Indicate that we got data. */
		gpio_toggle(GPIOC, GPIO12);
		/* Retrieve the data from the peripheral. */
		rc = (unsigned char)usart_recv(USART1);
		if(rc != '\n' && rc != '\r')
			str_rec[iter++] = rc;
		else
		{
			str_rec[iter++] = 0;
			printf("iter: 	    %d\n", iter-1);
			printf("str_rec:   %s\n",  str_rec );	
			if( 
			iter == 4
			&&str_rec[0] == 0xb0 
			&& str_rec[1] == 0x0b
			&& str_rec[2] == 0x66 
			){
				entery_flag = 1;
				AT24C02_write_data(str_program, 0,  3 );
				rcc_periph_clock_disable(RCC_GPIOB);
				rcc_periph_clock_disable(RCC_AFIO);
				// nvic_disable_irq(NVIC_USART1_IRQ);
				boot_entery();
			}
			iter = 0;
		}
		/* Enable transmit interrupt so it sends back the data. */
		USART_CR1(USART1) |= USART_CR1_TXEIE;
	}
	/* Check if we were called because of TXE. */
	if (((USART_CR1(USART1) & USART_CR1_TXEIE) != 0) &&
	    ((USART_SR(USART1) & USART_SR_TXE) != 0)) {
		/* Put data into the transmit register. */
		usart_send(USART1, rc);
		/* Disable the TXE interrupt, it's no longer needed. */
		USART_CR1(USART1) &= ~USART_CR1_TXEIE;
	}
}
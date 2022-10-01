#include<stdio.h>
#include<stdio.h>
#include <libopencm3/stm32/flash.h> 
#include <libopencm3/stm32/usart.h>
#include "uart.h"
#include "boot.h"
#include <libopencm3/cm3/scb.h>

uint8_t  entery_flag = 0;
uint8_t  str_program[3] = {0x11, 0x22, 0x33};
uint8_t  str_gotoapp[3] = {0xdd, 0xee, 0xff};

void boot_entery(void) 
{
  /* Check if the user button is depressed, if so launch the factory bootloader */
		/* Set vector table base address. */
		SCB_VTOR = ApplicationAddress & 0xFFFF;
		/* Initialise master stack pointer. */
		asm volatile("msr msp, %0"::"g"
				 (*(volatile uint32_t *)ApplicationAddress));
		/* Jump to bootloader. */
		(*(void (**)())(ApplicationAddress + 4))();
}

uint32_t flash_program_data(uint32_t start_address, uint8_t *input_data, uint16_t num_elements)
{
	uint16_t iter;
	uint32_t current_address = start_address;
	uint32_t page_address = start_address;
	uint32_t flash_status = 0;

	/*check if start_address is in proper range*/
	if((start_address - FLASH_BASE) >= (FLASH_PAGE_SIZE * (FLASH_PAGE_NUM_MAX+1)))
		return 1;

	/*calculate current page address*/
	if(start_address % FLASH_PAGE_SIZE)
		page_address -= (start_address % FLASH_PAGE_SIZE);

	flash_unlock();

	/*Erasing page*/
	flash_erase_page(page_address);
	flash_erase_page(page_address+0x400);
	flash_status = flash_get_status_flags();
	if(flash_status != FLASH_SR_EOP)
		return flash_status;

	/*programming flash memory*/
	for(iter=0; iter<num_elements; iter += 4)
	{
		/*programming word data*/
		flash_program_word(current_address+iter, *((uint32_t*)(input_data + iter)));
		flash_status = flash_get_status_flags();
		if(flash_status != FLASH_SR_EOP)
			return flash_status;

		/*verify if correct data is programmed*/
		if(*((uint32_t*)(current_address+iter)) != *((uint32_t*)(input_data + iter)))
			return FLASH_WRONG_DATA_WRITTEN;
	}

	return 0;
}

void flash_read_data(uint32_t start_address, uint16_t num_elements, uint8_t *output_data)
{
	uint16_t iter;
	uint32_t *memory_ptr= (uint32_t*)start_address;

	for(iter=0; iter<num_elements/4; iter++)
	{
		*(uint32_t*)output_data = *(memory_ptr + iter);
		output_data += 4;
	}
}

void usart_send_string(uint32_t usart, uint8_t *string, uint16_t str_size)
{
	uint16_t iter = 0;
	do
	{
		usart_send_blocking(usart, string[iter++]);
	}while(string[iter] != 0 && iter < str_size);
}

void usart_get_string(uint32_t usart, uint8_t *out_string, uint16_t str_max_size)
{
	uint8_t sign = 0;
	uint16_t iter = 0;

	while(iter < str_max_size)
	{
		sign = usart_recv_blocking(usart);

#if USART_ECHO_EN == 1
		usart_send_blocking(usart, sign);
#endif

		// if(sign != '\n' && sign != '\r')
		// 	out_string[iter++] = sign;
		// else
		// {
		// 	out_string[iter] = 0;
		// 	usart_send_string(USART1, (uint8_t*)"\r\n", 3);
		// 	break;
		// }
		// if(iter >= 0x500)
		// 	out_string[iter++] = 0xff;
		// else
		// {
			out_string[iter++] = sign;
		// }
			if(iter >= 2048)
				break;
	}
}

void local_ltoa_hex(uint32_t value, uint8_t *out_string)
{
	uint8_t iter;

	/*end of string*/
	out_string += 8;
	*(out_string--) = 0;

	for(iter=0; iter<8; iter++)
	{
		*(out_string--) = (((value&0xf) > 0x9) ? (0x40 + ((value&0xf) - 0x9)) : (0x30 | (value&0xf)));
		value >>= 4;
	}
}
#include "FreeRTOS.h"


extern void AT24C02_iic_test(void);
extern void AT24C02_read_data(uint8_t *read_data, uint16_t start_addr, uint16_t data_length);
extern void AT24C02_write_data(uint8_t *write_data, uint16_t start_addr, uint16_t data_length);
